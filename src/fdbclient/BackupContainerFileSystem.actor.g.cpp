#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
/*
 * BackupContainerFileSystem.actor.cpp
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

#include "fdbclient/BackupAgent.actor.h"
#include "fdbclient/BackupContainerAzureBlobStore.h"
#include "fdbclient/BackupContainerFileSystem.h"
#include "fdbclient/BackupContainerLocalDirectory.h"
#include "fdbclient/BackupContainerS3BlobStore.h"
#include "fdbclient/JsonBuilder.h"
#include "flow/StreamCipher.h"
#include "flow/UnitTest.h"

#include <algorithm>
#include <cinttypes>

#include "flow/actorcompiler.h" // This must be the last #include.

class BackupContainerFileSystemImpl {
public:
	// TODO:  Do this more efficiently, as the range file list for a snapshot could potentially be hundreds of
	// megabytes.
																#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via readKeyspaceSnapshot()
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class ReadKeyspaceSnapshotActor>
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ReadKeyspaceSnapshotActorState {
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ReadKeyspaceSnapshotActorState(Reference<BackupContainerFileSystem> const& bc,KeyspaceSnapshotFile const& snapshot) 
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   snapshot(snapshot)
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this));

	}
	~ReadKeyspaceSnapshotActorState() 
	{
		fdb_probe_actor_destroy("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<std::vector<RangeFile>> __when_expr_0 = bc->listRangeFiles(snapshot.beginVersion, snapshot.endVersion);
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 1;
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadKeyspaceSnapshotActor, 0, std::vector<RangeFile> >*>(static_cast<ReadKeyspaceSnapshotActor*>(this)));
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~ReadKeyspaceSnapshotActorState();
		static_cast<ReadKeyspaceSnapshotActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<RangeFile> const& files,int loopDepth) 
	{
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		rangeIndex = std::map<std::string, RangeFile>();
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto& f : files ) {
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			rangeIndex[f.fileName] = std::move(f);
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IAsyncFile>> __when_expr_1 = bc->readFile(snapshot.fileName);
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 2;
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadKeyspaceSnapshotActor, 1, Reference<IAsyncFile> >*>(static_cast<ReadKeyspaceSnapshotActor*>(this)));
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<RangeFile> && files,int loopDepth) 
	{
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		rangeIndex = std::map<std::string, RangeFile>();
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto& f : files ) {
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			rangeIndex[f.fileName] = std::move(f);
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IAsyncFile>> __when_expr_1 = bc->readFile(snapshot.fileName);
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 2;
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadKeyspaceSnapshotActor, 1, Reference<IAsyncFile> >*>(static_cast<ReadKeyspaceSnapshotActor*>(this)));
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<RangeFile> const& files,int loopDepth) 
	{
		loopDepth = a_body1cont1(files, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<RangeFile> && files,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(files), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state > 0) static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 0;
		static_cast<ReadKeyspaceSnapshotActor*>(this)->ActorCallback< ReadKeyspaceSnapshotActor, 0, std::vector<RangeFile> >::remove();

	}
	void a_callback_fire(ActorCallback< ReadKeyspaceSnapshotActor, 0, std::vector<RangeFile> >*,std::vector<RangeFile> const& value) 
	{
		fdb_probe_actor_enter("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadKeyspaceSnapshotActor, 0, std::vector<RangeFile> >*,std::vector<RangeFile> && value) 
	{
		fdb_probe_actor_enter("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadKeyspaceSnapshotActor, 0, std::vector<RangeFile> >*,Error err) 
	{
		fdb_probe_actor_enter("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<int64_t> __when_expr_2 = f->size();
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 3;
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReadKeyspaceSnapshotActor, 2, int64_t >*>(static_cast<ReadKeyspaceSnapshotActor*>(this)));
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(Reference<IAsyncFile> const& __f,int loopDepth) 
	{
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		f = __f;
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Reference<IAsyncFile> && __f,int loopDepth) 
	{
		f = std::move(__f);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state > 0) static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 0;
		static_cast<ReadKeyspaceSnapshotActor*>(this)->ActorCallback< ReadKeyspaceSnapshotActor, 1, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< ReadKeyspaceSnapshotActor, 1, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadKeyspaceSnapshotActor, 1, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadKeyspaceSnapshotActor, 1, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(int64_t const& size,int loopDepth) 
	{
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		buf = makeString(size);
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_3 = success(f->read(mutateString(buf), buf.size(), 0));
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 4;
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReadKeyspaceSnapshotActor, 3, Void >*>(static_cast<ReadKeyspaceSnapshotActor*>(this)));
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(int64_t && size,int loopDepth) 
	{
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		buf = makeString(size);
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_3 = success(f->read(mutateString(buf), buf.size(), 0));
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 4;
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReadKeyspaceSnapshotActor, 3, Void >*>(static_cast<ReadKeyspaceSnapshotActor*>(this)));
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(int64_t const& size,int loopDepth) 
	{
		loopDepth = a_body1cont4(size, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(int64_t && size,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(size), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state > 0) static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 0;
		static_cast<ReadKeyspaceSnapshotActor*>(this)->ActorCallback< ReadKeyspaceSnapshotActor, 2, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< ReadKeyspaceSnapshotActor, 2, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ReadKeyspaceSnapshotActor, 2, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ReadKeyspaceSnapshotActor, 2, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		json_spirit::mValue json;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		json_spirit::read_string(buf.toString(), json);
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		JSONDoc doc(json);
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		Version v;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!doc.tryGet("beginVersion", v) || v != snapshot.beginVersion)
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!doc.tryGet("endVersion", v) || v != snapshot.endVersion)
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		json_spirit::mValue& filesArray = doc.create("files");
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (filesArray.type() != json_spirit::array_type)
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::vector<RangeFile> results;
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int missing = 0;
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& fileValue : filesArray.get_array() ) {
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (fileValue.type() != json_spirit::str_type)
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			auto i = rangeIndex.find(fileValue.get_str());
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (i == rangeIndex.end())
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent(SevError, "FileRestoreMissingRangeFile") .detail("URL", bc->getURL()) .detail("File", fileValue.get_str());
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				++missing;
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (missing == 0)
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				results.push_back(i->second);
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (missing > 0)
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevError, "FileRestoreMissingRangeFileSummary") .detail("URL", bc->getURL()) .detail("Count", missing);
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_missing_data(), loopDepth);
															#line 469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::map<std::string, KeyRange> fileKeyRanges;
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		JSONDoc ranges = doc.subDoc("keyRanges");
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto i : ranges.obj() ) {
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			const std::string& filename = i.first;
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			JSONDoc fields(i.second);
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::string begin, end;
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (fields.tryGet("beginKey", begin) && fields.tryGet("endKey", end))
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent("ManifestFields") .detail("File", filename) .detail("Begin", printable(StringRef(begin))) .detail("End", printable(StringRef(end)));
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				fileKeyRanges.emplace(filename, KeyRange(KeyRangeRef(StringRef(begin), StringRef(end))));
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
			else
			{
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent("MalFormattedManifest").detail("Key", filename);
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<ReadKeyspaceSnapshotActor*>(this)->SAV<std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>>::futures) { (void)(std::make_pair(results, fileKeyRanges)); this->~ReadKeyspaceSnapshotActorState(); static_cast<ReadKeyspaceSnapshotActor*>(this)->destroy(); return 0; }
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<ReadKeyspaceSnapshotActor*>(this)->SAV< std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> >::value()) std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>(std::make_pair(results, fileKeyRanges));
		this->~ReadKeyspaceSnapshotActorState();
		static_cast<ReadKeyspaceSnapshotActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		json_spirit::mValue json;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		json_spirit::read_string(buf.toString(), json);
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		JSONDoc doc(json);
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		Version v;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!doc.tryGet("beginVersion", v) || v != snapshot.beginVersion)
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!doc.tryGet("endVersion", v) || v != snapshot.endVersion)
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		json_spirit::mValue& filesArray = doc.create("files");
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (filesArray.type() != json_spirit::array_type)
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::vector<RangeFile> results;
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int missing = 0;
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& fileValue : filesArray.get_array() ) {
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (fileValue.type() != json_spirit::str_type)
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			auto i = rangeIndex.find(fileValue.get_str());
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (i == rangeIndex.end())
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent(SevError, "FileRestoreMissingRangeFile") .detail("URL", bc->getURL()) .detail("File", fileValue.get_str());
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				++missing;
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (missing == 0)
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				results.push_back(i->second);
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (missing > 0)
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevError, "FileRestoreMissingRangeFileSummary") .detail("URL", bc->getURL()) .detail("Count", missing);
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_missing_data(), loopDepth);
															#line 591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::map<std::string, KeyRange> fileKeyRanges;
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		JSONDoc ranges = doc.subDoc("keyRanges");
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto i : ranges.obj() ) {
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			const std::string& filename = i.first;
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			JSONDoc fields(i.second);
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::string begin, end;
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (fields.tryGet("beginKey", begin) && fields.tryGet("endKey", end))
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent("ManifestFields") .detail("File", filename) .detail("Begin", printable(StringRef(begin))) .detail("End", printable(StringRef(end)));
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				fileKeyRanges.emplace(filename, KeyRange(KeyRangeRef(StringRef(begin), StringRef(end))));
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
			else
			{
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent("MalFormattedManifest").detail("Key", filename);
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<ReadKeyspaceSnapshotActor*>(this)->SAV<std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>>::futures) { (void)(std::make_pair(results, fileKeyRanges)); this->~ReadKeyspaceSnapshotActorState(); static_cast<ReadKeyspaceSnapshotActor*>(this)->destroy(); return 0; }
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<ReadKeyspaceSnapshotActor*>(this)->SAV< std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> >::value()) std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>(std::make_pair(results, fileKeyRanges));
		this->~ReadKeyspaceSnapshotActorState();
		static_cast<ReadKeyspaceSnapshotActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state > 0) static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 0;
		static_cast<ReadKeyspaceSnapshotActor*>(this)->ActorCallback< ReadKeyspaceSnapshotActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadKeyspaceSnapshotActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ReadKeyspaceSnapshotActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ReadKeyspaceSnapshotActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	KeyspaceSnapshotFile snapshot;
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::map<std::string, RangeFile> rangeIndex;
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IAsyncFile> f;
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Standalone<StringRef> buf;
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via readKeyspaceSnapshot()
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ReadKeyspaceSnapshotActor final : public Actor<std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>>, public ActorCallback< ReadKeyspaceSnapshotActor, 0, std::vector<RangeFile> >, public ActorCallback< ReadKeyspaceSnapshotActor, 1, Reference<IAsyncFile> >, public ActorCallback< ReadKeyspaceSnapshotActor, 2, int64_t >, public ActorCallback< ReadKeyspaceSnapshotActor, 3, Void >, public FastAllocated<ReadKeyspaceSnapshotActor>, public ReadKeyspaceSnapshotActorState<ReadKeyspaceSnapshotActor> {
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<ReadKeyspaceSnapshotActor>::operator new;
	using FastAllocated<ReadKeyspaceSnapshotActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadKeyspaceSnapshotActor, 0, std::vector<RangeFile> >;
friend struct ActorCallback< ReadKeyspaceSnapshotActor, 1, Reference<IAsyncFile> >;
friend struct ActorCallback< ReadKeyspaceSnapshotActor, 2, int64_t >;
friend struct ActorCallback< ReadKeyspaceSnapshotActor, 3, Void >;
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ReadKeyspaceSnapshotActor(Reference<BackupContainerFileSystem> const& bc,KeyspaceSnapshotFile const& snapshot) 
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>>(),
		   ReadKeyspaceSnapshotActorState<ReadKeyspaceSnapshotActor>(bc, snapshot)
	{
		fdb_probe_actor_enter("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readKeyspaceSnapshot");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readKeyspaceSnapshot", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadKeyspaceSnapshotActor, 0, std::vector<RangeFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReadKeyspaceSnapshotActor, 1, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ReadKeyspaceSnapshotActor, 2, int64_t >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ReadKeyspaceSnapshotActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>> readKeyspaceSnapshot( Reference<BackupContainerFileSystem> const& bc, KeyspaceSnapshotFile const& snapshot ) {
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>>(new ReadKeyspaceSnapshotActor(bc, snapshot));
															#line 756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

	// Backup log types
	static constexpr Version NON_PARTITIONED_MUTATION_LOG = 0;
	static constexpr Version PARTITIONED_MUTATION_LOG = 1;

	// Find what should be the filename of a path by finding whatever is after the last forward or backward slash, or
	// failing to find those, the whole string.
	static std::string fileNameOnly(const std::string& path) {
		// Find the last forward slash position, defaulting to 0 if not found
		int pos = path.find_last_of('/');
		if (pos == std::string::npos) {
			pos = 0;
		}
		// Find the last backward slash position after pos, and update pos if found
		int b = path.find_last_of('\\', pos);
		if (b != std::string::npos) {
			pos = b;
		}
		return path.substr(pos + 1);
	}

	static bool pathToRangeFile(RangeFile& out, const std::string& path, int64_t size) {
		std::string name = fileNameOnly(path);
		RangeFile f;
		f.fileName = path;
		f.fileSize = size;
		int len;
		if (sscanf(name.c_str(), "range,%" SCNd64 ",%*[^,],%u%n", &f.version, &f.blockSize, &len) == 2 &&
		    len == name.size()) {
			out = f;
			return true;
		}
		return false;
	}

																#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via writeKeyspaceSnapshotFile()
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class WriteKeyspaceSnapshotFileActor>
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class WriteKeyspaceSnapshotFileActorState {
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	WriteKeyspaceSnapshotFileActorState(Reference<BackupContainerFileSystem> const& bc,std::vector<std::string> const& fileNames,std::vector<std::pair<Key, Key>> const& beginEndKeys,int64_t const& totalBytes) 
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   fileNames(fileNames),
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   beginEndKeys(beginEndKeys),
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   totalBytes(totalBytes)
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this));

	}
	~WriteKeyspaceSnapshotFileActorState() 
	{
		fdb_probe_actor_destroy("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!fileNames.empty() && fileNames.size() == beginEndKeys.size());
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			minVer = std::numeric_limits<Version>::max();
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			maxVer = 0;
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			rf = RangeFile();
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			fileArray = json_spirit::mArray();
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			RangeForbody1Iterator0 = std::begin(fileNames);
															#line 839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~WriteKeyspaceSnapshotFileActorState();
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		json = json_spirit::mValue();
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		doc = JSONDoc(json);
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		doc.create("files") = std::move(fileArray);
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		doc.create("totalBytes") = totalBytes;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		doc.create("beginVersion") = minVer;
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		doc.create("endVersion") = maxVer;
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		auto ranges = doc.subDoc("keyRanges");
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for(int i = 0;i < beginEndKeys.size();i++) {
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			auto fileDoc = ranges.subDoc(fileNames[i], false);
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			fileDoc.create("beginKey") = beginEndKeys[i].first.toString();
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			fileDoc.create("endKey") = beginEndKeys[i].second.toString();
															#line 882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_1 = yield();
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 2;
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WriteKeyspaceSnapshotFileActor, 1, Void >*>(static_cast<WriteKeyspaceSnapshotFileActor*>(this)));
															#line 893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(RangeForbody1Iterator0 != std::end(fileNames)))
															#line 909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		const auto& f = *RangeForbody1Iterator0;
															#line 915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (pathToRangeFile(rf, f, 0))
															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				fileArray.push_back(f);
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (rf.version < minVer)
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					minVer = rf.version;
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (rf.version > maxVer)
															#line 933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					maxVer = rf.version;
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
			else
			{
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(restore_unknown_file_type(), std::max(0, loopDepth - 1));
															#line 944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_0 = yield();
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 1;
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WriteKeyspaceSnapshotFileActor, 0, Void >*>(static_cast<WriteKeyspaceSnapshotFileActor*>(this)));
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
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
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		++RangeForbody1Iterator0;
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state > 0) static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 0;
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->ActorCallback< WriteKeyspaceSnapshotFileActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteKeyspaceSnapshotFileActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WriteKeyspaceSnapshotFileActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WriteKeyspaceSnapshotFileActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		docString = json_spirit::write_string(json);
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IBackupFile>> __when_expr_2 = bc->writeFile(format("snapshots/snapshot,%lld,%lld,%lld", minVer, maxVer, totalBytes));
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 3;
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteKeyspaceSnapshotFileActor, 2, Reference<IBackupFile> >*>(static_cast<WriteKeyspaceSnapshotFileActor*>(this)));
															#line 1071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		docString = json_spirit::write_string(json);
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IBackupFile>> __when_expr_2 = bc->writeFile(format("snapshots/snapshot,%lld,%lld,%lld", minVer, maxVer, totalBytes));
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 3;
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteKeyspaceSnapshotFileActor, 2, Reference<IBackupFile> >*>(static_cast<WriteKeyspaceSnapshotFileActor*>(this)));
															#line 1089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state > 0) static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 0;
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->ActorCallback< WriteKeyspaceSnapshotFileActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteKeyspaceSnapshotFileActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WriteKeyspaceSnapshotFileActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WriteKeyspaceSnapshotFileActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(int loopDepth) 
	{
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_3 = f->append(docString.data(), docString.size());
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 4;
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WriteKeyspaceSnapshotFileActor, 3, Void >*>(static_cast<WriteKeyspaceSnapshotFileActor*>(this)));
															#line 1168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(Reference<IBackupFile> const& __f,int loopDepth) 
	{
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		f = __f;
															#line 1177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Reference<IBackupFile> && __f,int loopDepth) 
	{
		f = std::move(__f);
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state > 0) static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 0;
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->ActorCallback< WriteKeyspaceSnapshotFileActor, 2, Reference<IBackupFile> >::remove();

	}
	void a_callback_fire(ActorCallback< WriteKeyspaceSnapshotFileActor, 2, Reference<IBackupFile> >*,Reference<IBackupFile> const& value) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< WriteKeyspaceSnapshotFileActor, 2, Reference<IBackupFile> >*,Reference<IBackupFile> && value) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< WriteKeyspaceSnapshotFileActor, 2, Reference<IBackupFile> >*,Error err) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_4 = f->finish();
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont5when1(__when_expr_4.get(), loopDepth); };
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 5;
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< WriteKeyspaceSnapshotFileActor, 4, Void >*>(static_cast<WriteKeyspaceSnapshotFileActor*>(this)));
															#line 1251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_4 = f->finish();
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont5when1(__when_expr_4.get(), loopDepth); };
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 5;
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< WriteKeyspaceSnapshotFileActor, 4, Void >*>(static_cast<WriteKeyspaceSnapshotFileActor*>(this)));
															#line 1267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state > 0) static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 0;
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->ActorCallback< WriteKeyspaceSnapshotFileActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteKeyspaceSnapshotFileActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< WriteKeyspaceSnapshotFileActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< WriteKeyspaceSnapshotFileActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<WriteKeyspaceSnapshotFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteKeyspaceSnapshotFileActorState(); static_cast<WriteKeyspaceSnapshotFileActor*>(this)->destroy(); return 0; }
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<WriteKeyspaceSnapshotFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteKeyspaceSnapshotFileActorState();
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<WriteKeyspaceSnapshotFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteKeyspaceSnapshotFileActorState(); static_cast<WriteKeyspaceSnapshotFileActor*>(this)->destroy(); return 0; }
															#line 1351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<WriteKeyspaceSnapshotFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteKeyspaceSnapshotFileActorState();
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->finishSendAndDelPromiseRef();
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
	void a_exitChoose5() 
	{
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state > 0) static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 0;
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->ActorCallback< WriteKeyspaceSnapshotFileActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteKeyspaceSnapshotFileActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< WriteKeyspaceSnapshotFileActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< WriteKeyspaceSnapshotFileActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<std::string> fileNames;
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<std::pair<Key, Key>> beginEndKeys;
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int64_t totalBytes;
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version minVer;
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version maxVer;
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	RangeFile rf;
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	json_spirit::mArray fileArray;
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	decltype(std::begin(std::declval<std::vector<std::string>>())) RangeForbody1Iterator0;
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	json_spirit::mValue json;
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	JSONDoc doc;
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string docString;
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IBackupFile> f;
															#line 1448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via writeKeyspaceSnapshotFile()
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class WriteKeyspaceSnapshotFileActor final : public Actor<Void>, public ActorCallback< WriteKeyspaceSnapshotFileActor, 0, Void >, public ActorCallback< WriteKeyspaceSnapshotFileActor, 1, Void >, public ActorCallback< WriteKeyspaceSnapshotFileActor, 2, Reference<IBackupFile> >, public ActorCallback< WriteKeyspaceSnapshotFileActor, 3, Void >, public ActorCallback< WriteKeyspaceSnapshotFileActor, 4, Void >, public FastAllocated<WriteKeyspaceSnapshotFileActor>, public WriteKeyspaceSnapshotFileActorState<WriteKeyspaceSnapshotFileActor> {
															#line 1453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<WriteKeyspaceSnapshotFileActor>::operator new;
	using FastAllocated<WriteKeyspaceSnapshotFileActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WriteKeyspaceSnapshotFileActor, 0, Void >;
friend struct ActorCallback< WriteKeyspaceSnapshotFileActor, 1, Void >;
friend struct ActorCallback< WriteKeyspaceSnapshotFileActor, 2, Reference<IBackupFile> >;
friend struct ActorCallback< WriteKeyspaceSnapshotFileActor, 3, Void >;
friend struct ActorCallback< WriteKeyspaceSnapshotFileActor, 4, Void >;
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	WriteKeyspaceSnapshotFileActor(Reference<BackupContainerFileSystem> const& bc,std::vector<std::string> const& fileNames,std::vector<std::pair<Key, Key>> const& beginEndKeys,int64_t const& totalBytes) 
															#line 1468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   WriteKeyspaceSnapshotFileActorState<WriteKeyspaceSnapshotFileActor>(bc, fileNames, beginEndKeys, totalBytes)
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("writeKeyspaceSnapshotFile");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WriteKeyspaceSnapshotFileActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WriteKeyspaceSnapshotFileActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< WriteKeyspaceSnapshotFileActor, 2, Reference<IBackupFile> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< WriteKeyspaceSnapshotFileActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< WriteKeyspaceSnapshotFileActor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Void> writeKeyspaceSnapshotFile( Reference<BackupContainerFileSystem> const& bc, std::vector<std::string> const& fileNames, std::vector<std::pair<Key, Key>> const& beginEndKeys, int64_t const& totalBytes ) {
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new WriteKeyspaceSnapshotFileActor(bc, fileNames, beginEndKeys, totalBytes));
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

																#line 1504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via dumpFileList()
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class DumpFileListActor>
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class DumpFileListActorState {
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	DumpFileListActorState(Reference<BackupContainerFileSystem> const& bc,Version const& begin,Version const& end) 
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   begin(begin),
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   end(end),
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   fRanges(bc->listRangeFiles(begin, end)),
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   fSnapshots(bc->listKeyspaceSnapshots(begin, end)),
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   logs(),
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   pLogs()
															#line 1529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("dumpFileList", reinterpret_cast<unsigned long>(this));

	}
	~DumpFileListActorState() 
	{
		fdb_probe_actor_destroy("dumpFileList", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_0 = success(fRanges) && success(fSnapshots) && store(logs, bc->listLogFiles(begin, end, false)) && store(pLogs, bc->listLogFiles(begin, end, true));
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<DumpFileListActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DumpFileListActor*>(this)->actor_wait_state = 1;
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DumpFileListActor, 0, Void >*>(static_cast<DumpFileListActor*>(this)));
															#line 1551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~DumpFileListActorState();
		static_cast<DumpFileListActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs.insert(logs.end(), std::make_move_iterator(pLogs.begin()), std::make_move_iterator(pLogs.end()));
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<DumpFileListActor*>(this)->SAV<BackupFileList>::futures) { (void)(BackupFileList({ fRanges.get(), std::move(logs), fSnapshots.get() })); this->~DumpFileListActorState(); static_cast<DumpFileListActor*>(this)->destroy(); return 0; }
															#line 1576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<DumpFileListActor*>(this)->SAV< BackupFileList >::value()) BackupFileList(BackupFileList({ fRanges.get(), std::move(logs), fSnapshots.get() }));
		this->~DumpFileListActorState();
		static_cast<DumpFileListActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs.insert(logs.end(), std::make_move_iterator(pLogs.begin()), std::make_move_iterator(pLogs.end()));
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<DumpFileListActor*>(this)->SAV<BackupFileList>::futures) { (void)(BackupFileList({ fRanges.get(), std::move(logs), fSnapshots.get() })); this->~DumpFileListActorState(); static_cast<DumpFileListActor*>(this)->destroy(); return 0; }
															#line 1590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<DumpFileListActor*>(this)->SAV< BackupFileList >::value()) BackupFileList(BackupFileList({ fRanges.get(), std::move(logs), fSnapshots.get() }));
		this->~DumpFileListActorState();
		static_cast<DumpFileListActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<DumpFileListActor*>(this)->actor_wait_state > 0) static_cast<DumpFileListActor*>(this)->actor_wait_state = 0;
		static_cast<DumpFileListActor*>(this)->ActorCallback< DumpFileListActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DumpFileListActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("dumpFileList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dumpFileList", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DumpFileListActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("dumpFileList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dumpFileList", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DumpFileListActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("dumpFileList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dumpFileList", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version begin;
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version end;
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Future<std::vector<RangeFile>> fRanges;
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Future<std::vector<KeyspaceSnapshotFile>> fSnapshots;
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> logs;
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> pLogs;
															#line 1675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via dumpFileList()
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class DumpFileListActor final : public Actor<BackupFileList>, public ActorCallback< DumpFileListActor, 0, Void >, public FastAllocated<DumpFileListActor>, public DumpFileListActorState<DumpFileListActor> {
															#line 1680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<DumpFileListActor>::operator new;
	using FastAllocated<DumpFileListActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<BackupFileList>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DumpFileListActor, 0, Void >;
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	DumpFileListActor(Reference<BackupContainerFileSystem> const& bc,Version const& begin,Version const& end) 
															#line 1691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<BackupFileList>(),
		   DumpFileListActorState<DumpFileListActor>(bc, begin, end)
	{
		fdb_probe_actor_enter("dumpFileList", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("dumpFileList");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("dumpFileList", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DumpFileListActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<BackupFileList> dumpFileList( Reference<BackupContainerFileSystem> const& bc, Version const& begin, Version const& end ) {
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<BackupFileList>(new DumpFileListActor(bc, begin, end));
															#line 1718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

	static Version resolveRelativeVersion(Optional<Version> max, Version v, const char* name, Error e) {
		if (v == invalidVersion) {
			TraceEvent(SevError, "BackupExpireInvalidVersion").detail(name, v);
			throw e;
		}
		if (v < 0) {
			if (!max.present()) {
				TraceEvent(SevError, "BackupExpireCannotResolveRelativeVersion").detail(name, v);
				throw e;
			}
			v += max.get();
		}
		return v;
	}

	// For a list of log files specified by their indices (of the same tag),
	// returns if they are continous in the range [begin, end]. If "tags" is not
	// nullptr, then it will be populated with [begin, end] -> tags, where next
	// pair's begin <= previous pair's end + 1. On return, the last pair's end
	// version (inclusive) gives the continuous range from begin.
	static bool isContinuous(const std::vector<LogFile>& files,
	                         const std::vector<int>& indices,
	                         Version begin,
	                         Version end,
	                         std::map<std::pair<Version, Version>, int>* tags) {
		Version lastBegin = invalidVersion;
		Version lastEnd = invalidVersion;
		int lastTags = -1;

		ASSERT(tags == nullptr || tags->empty());
		for (int idx : indices) {
			const LogFile& file = files[idx];
			if (lastEnd == invalidVersion) {
				if (file.beginVersion > begin)
					return false;
				if (file.endVersion > begin) {
					lastBegin = begin;
					lastTags = file.totalTags;
				} else {
					continue;
				}
			} else if (lastEnd < file.beginVersion) {
				if (tags != nullptr) {
					tags->emplace(std::make_pair(lastBegin, lastEnd - 1), lastTags);
				}
				return false;
			}

			if (lastTags != file.totalTags) {
				if (tags != nullptr) {
					tags->emplace(std::make_pair(lastBegin, file.beginVersion - 1), lastTags);
				}
				lastBegin = file.beginVersion;
				lastTags = file.totalTags;
			}
			lastEnd = file.endVersion;
			if (lastEnd > end)
				break;
		}
		if (tags != nullptr && lastBegin != invalidVersion) {
			tags->emplace(std::make_pair(lastBegin, std::min(end, lastEnd - 1)), lastTags);
		}
		return lastBegin != invalidVersion && lastEnd > end;
	}

	// Returns the end version such that [begin, end] is continuous.
	// "logs" should be already sorted.
	static Version getPartitionedLogsContinuousEndVersion(const std::vector<LogFile>& logs, Version begin) {
		Version end = 0;

		std::map<int, std::vector<int>> tagIndices; // tagId -> indices in files
		for (int i = 0; i < logs.size(); i++) {
			ASSERT_GE(logs[i].tagId, 0);
			ASSERT_LT(logs[i].tagId, logs[i].totalTags);
			auto& indices = tagIndices[logs[i].tagId];
			// filter out if indices.back() is subset of files[i] or vice versa
			if (!indices.empty()) {
				if (logs[indices.back()].isSubset(logs[i])) {
					ASSERT_LE(logs[indices.back()].fileSize, logs[i].fileSize);
					indices.back() = i;
				} else if (!logs[i].isSubset(logs[indices.back()])) {
					indices.push_back(i);
				}
			} else {
				indices.push_back(i);
			}
			end = std::max(end, logs[i].endVersion - 1);
		}
		TraceEvent("ContinuousLogEnd").detail("Begin", begin).detail("InitVersion", end);

		// check partition 0 is continuous in [begin, end] and create a map of ranges to partitions
		std::map<std::pair<Version, Version>, int> tags; // range [start, end] -> partitions
		isContinuous(logs, tagIndices[0], begin, end, &tags);
		if (tags.empty() || end <= begin)
			return 0;
		end = std::min(end, tags.rbegin()->first.second);
		TraceEvent("ContinuousLogEnd").detail("Partition", 0).detail("EndVersion", end).detail("Begin", begin);

		// for each range in tags, check all partitions from 1 are continouous
		Version lastEnd = begin;
		for (const auto& [beginEnd, count] : tags) {
			Version tagEnd = beginEnd.second; // This range's minimum continous partition version
			for (int i = 1; i < count; i++) {
				std::map<std::pair<Version, Version>, int> rangeTags;
				isContinuous(logs, tagIndices[i], beginEnd.first, beginEnd.second, &rangeTags);
				tagEnd = rangeTags.empty() ? 0 : std::min(tagEnd, rangeTags.rbegin()->first.second);
				TraceEvent("ContinuousLogEnd")
				    .detail("Partition", i)
				    .detail("EndVersion", tagEnd)
				    .detail("RangeBegin", beginEnd.first)
				    .detail("RangeEnd", beginEnd.second);
				if (tagEnd == 0)
					return lastEnd == begin ? 0 : lastEnd;
			}
			if (tagEnd < beginEnd.second) {
				return tagEnd;
			}
			lastEnd = beginEnd.second;
		}

		return end;
	}

	// Analyze partitioned logs and set contiguousLogEnd for "desc" if larger
	// than the "scanBegin" version.
	static void updatePartitionedLogsContinuousEnd(BackupDescription* desc,
	                                               const std::vector<LogFile>& logs,
	                                               const Version scanBegin,
	                                               const Version scanEnd) {
		if (logs.empty())
			return;

		Version snapshotBeginVersion = desc->snapshots.size() > 0 ? desc->snapshots[0].beginVersion : invalidVersion;
		Version begin = std::max(scanBegin, desc->minLogBegin.get());
		TraceEvent("ContinuousLogEnd")
		    .detail("ScanBegin", scanBegin)
		    .detail("ScanEnd", scanEnd)
		    .detail("Begin", begin)
		    .detail("ContiguousLogEnd", desc->contiguousLogEnd.get());
		for (const auto& file : logs) {
			if (file.beginVersion > begin) {
				if (scanBegin > 0)
					return;

				// scanBegin is 0
				desc->minLogBegin = file.beginVersion;
				begin = file.beginVersion;
			}

			Version ver = getPartitionedLogsContinuousEndVersion(logs, begin);
			if (ver >= desc->contiguousLogEnd.get()) {
				// contiguousLogEnd is not inclusive, so +1 here.
				desc->contiguousLogEnd.get() = ver + 1;
				TraceEvent("UpdateContinuousLogEnd").detail("Version", ver + 1);
				if (ver > snapshotBeginVersion)
					return;
			}
		}
	}

	// Computes the continuous end version for non-partitioned mutation logs up to
	// the "targetVersion". If "outLogs" is not nullptr, it will be updated with
	// continuous log files. "*end" is updated with the continuous end version.
	static void computeRestoreEndVersion(const std::vector<LogFile>& logs,
	                                     std::vector<LogFile>* outLogs,
	                                     Version* end,
	                                     Version targetVersion) {
		auto i = logs.begin();
		if (outLogs != nullptr)
			outLogs->push_back(*i);

		// Add logs to restorable logs set until continuity is broken OR we reach targetVersion
		while (++i != logs.end()) {
			if (i->beginVersion > *end || i->beginVersion > targetVersion)
				break;

			// If the next link in the log chain is found, update the end
			if (i->beginVersion == *end) {
				if (outLogs != nullptr)
					outLogs->push_back(*i);
				*end = i->endVersion;
			}
		}
	}

																#line 1908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via describeBackup()
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class DescribeBackupActor>
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class DescribeBackupActorState {
															#line 1914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	DescribeBackupActorState(Reference<BackupContainerFileSystem> const& bc,bool const& deepScan,Version const& logStartVersionOverride) 
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   deepScan(deepScan),
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   logStartVersionOverride(logStartVersionOverride),
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   desc()
															#line 1927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("describeBackup", reinterpret_cast<unsigned long>(this));

	}
	~DescribeBackupActorState() 
	{
		fdb_probe_actor_destroy("describeBackup", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.url = bc->getURL();
															#line 412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.proxy = bc->getProxy();
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent("BackupContainerDescribe1") .detail("URL", bc->getURL()) .detail("LogStartVersionOverride", logStartVersionOverride);
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<bool> __when_expr_0 = bc->exists();
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DescribeBackupActor*>(this)->actor_wait_state = 1;
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 0, bool >*>(static_cast<DescribeBackupActor*>(this)));
															#line 1955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~DescribeBackupActorState();
		static_cast<DescribeBackupActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(bool const& e,int loopDepth) 
	{
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!e)
															#line 1978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarnAlways, "BackupContainerDoesNotExist").detail("URL", bc->getURL());
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(backup_does_not_exist(), loopDepth);
															#line 1984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (logStartVersionOverride != invalidVersion && logStartVersionOverride < 0)
															#line 1988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<BackupDescription> __when_expr_1 = bc->describeBackup(false, invalidVersion);
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<DescribeBackupActor*>(this)->actor_wait_state = 2;
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 1, BackupDescription >*>(static_cast<DescribeBackupActor*>(this)));
															#line 1999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(bool && e,int loopDepth) 
	{
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!e)
															#line 2013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarnAlways, "BackupContainerDoesNotExist").detail("URL", bc->getURL());
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(backup_does_not_exist(), loopDepth);
															#line 2019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (logStartVersionOverride != invalidVersion && logStartVersionOverride < 0)
															#line 2023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<BackupDescription> __when_expr_1 = bc->describeBackup(false, invalidVersion);
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<DescribeBackupActor*>(this)->actor_wait_state = 2;
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 1, BackupDescription >*>(static_cast<DescribeBackupActor*>(this)));
															#line 2034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(bool const& e,int loopDepth) 
	{
		loopDepth = a_body1cont1(e, loopDepth);

		return loopDepth;
	}
	int a_body1when1(bool && e,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(e), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DescribeBackupActor*>(this)->actor_wait_state > 0) static_cast<DescribeBackupActor*>(this)->actor_wait_state = 0;
		static_cast<DescribeBackupActor*>(this)->ActorCallback< DescribeBackupActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< DescribeBackupActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DescribeBackupActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DescribeBackupActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaLogBegin = Optional<Version>();
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaLogEnd = Optional<Version>();
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaExpiredEnd = Optional<Version>();
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaUnreliableEnd = Optional<Version>();
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaLogType = Optional<Version>();
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::vector<Future<Void>> metaReads;
															#line 441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaReads.push_back(store(metaExpiredEnd, bc->expiredEndVersion().get()));
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaReads.push_back(store(metaUnreliableEnd, bc->unreliableEndVersion().get()));
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaReads.push_back(store(metaLogType, bc->logType().get()));
															#line 446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!deepScan)
															#line 2129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaReads.push_back(store(metaLogBegin, bc->logBeginVersion().get()));
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaReads.push_back(store(metaLogEnd, bc->logEndVersion().get()));
															#line 2135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = waitForAll(metaReads);
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<DescribeBackupActor*>(this)->actor_wait_state = 3;
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 2, Void >*>(static_cast<DescribeBackupActor*>(this)));
															#line 2146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(BackupDescription const& tmp,int loopDepth) 
	{
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logStartVersionOverride = resolveRelativeVersion( tmp.maxLogEnd, logStartVersionOverride, "LogStartVersionOverride", invalid_option_value());
															#line 2155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(BackupDescription && tmp,int loopDepth) 
	{
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logStartVersionOverride = resolveRelativeVersion( tmp.maxLogEnd, logStartVersionOverride, "LogStartVersionOverride", invalid_option_value());
															#line 2164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(BackupDescription const& tmp,int loopDepth) 
	{
		loopDepth = a_body1cont4(tmp, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(BackupDescription && tmp,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(tmp), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DescribeBackupActor*>(this)->actor_wait_state > 0) static_cast<DescribeBackupActor*>(this)->actor_wait_state = 0;
		static_cast<DescribeBackupActor*>(this)->ActorCallback< DescribeBackupActor, 1, BackupDescription >::remove();

	}
	void a_callback_fire(ActorCallback< DescribeBackupActor, 1, BackupDescription >*,BackupDescription const& value) 
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DescribeBackupActor, 1, BackupDescription >*,BackupDescription && value) 
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DescribeBackupActor, 1, BackupDescription >*,Error err) 
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent("BackupContainerDescribe2") .detail("URL", bc->getURL()) .detail("LogStartVersionOverride", logStartVersionOverride) .detail("ExpiredEndVersion", metaExpiredEnd.orDefault(invalidVersion)) .detail("UnreliableEndVersion", metaUnreliableEnd.orDefault(invalidVersion)) .detail("LogBeginVersion", metaLogBegin.orDefault(invalidVersion)) .detail("LogEndVersion", metaLogEnd.orDefault(invalidVersion)) .detail("LogType", metaLogType.orDefault(-1));
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (logStartVersionOverride != invalidVersion && metaUnreliableEnd.orDefault(invalidVersion) < logStartVersionOverride)
															#line 2238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaUnreliableEnd = logStartVersionOverride;
															#line 2242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!metaLogBegin.present() || !metaLogEnd.present() || metaLogEnd.get() <= metaLogBegin.get() || metaLogEnd.get() < metaExpiredEnd.orDefault(invalidVersion) || metaLogEnd.get() < metaUnreliableEnd.orDefault(invalidVersion))
															#line 2246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarnAlways, "BackupContainerMetadataInvalid") .detail("URL", bc->getURL()) .detail("ExpiredEndVersion", metaExpiredEnd.orDefault(invalidVersion)) .detail("UnreliableEndVersion", metaUnreliableEnd.orDefault(invalidVersion)) .detail("LogBeginVersion", metaLogBegin.orDefault(invalidVersion)) .detail("LogEndVersion", metaLogEnd.orDefault(invalidVersion));
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaLogBegin = Optional<Version>();
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaLogEnd = Optional<Version>();
															#line 2254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!metaUnreliableEnd.present() || metaUnreliableEnd.get() < metaExpiredEnd.orDefault(0))
															#line 2258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaUnreliableEnd = metaExpiredEnd;
															#line 2262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc.unreliableEndVersion = metaUnreliableEnd;
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc.expiredEndVersion = metaExpiredEnd;
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		scanBegin = desc.unreliableEndVersion.orDefault(0);
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		scanEnd = std::numeric_limits<Version>::max();
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (metaLogBegin.present() && metaLogEnd.present())
															#line 2274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.minLogBegin = std::max(metaLogBegin.get(), desc.unreliableEndVersion.orDefault(0));
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.maxLogEnd = metaLogEnd.get();
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.contiguousLogEnd = desc.maxLogEnd;
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			scanBegin = desc.contiguousLogEnd.get();
															#line 2284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs = std::vector<LogFile>();
															#line 520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		plogs = std::vector<LogFile>();
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent("BackupContainerListFiles").detail("URL", bc->getURL());
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_3 = store(logs, bc->listLogFiles(scanBegin, scanEnd, false)) && store(plogs, bc->listLogFiles(scanBegin, scanEnd, true)) && store(desc.snapshots, bc->listKeyspaceSnapshots());
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<DescribeBackupActor*>(this)->actor_wait_state = 4;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 3, Void >*>(static_cast<DescribeBackupActor*>(this)));
															#line 2301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent("BackupContainerDescribe2") .detail("URL", bc->getURL()) .detail("LogStartVersionOverride", logStartVersionOverride) .detail("ExpiredEndVersion", metaExpiredEnd.orDefault(invalidVersion)) .detail("UnreliableEndVersion", metaUnreliableEnd.orDefault(invalidVersion)) .detail("LogBeginVersion", metaLogBegin.orDefault(invalidVersion)) .detail("LogEndVersion", metaLogEnd.orDefault(invalidVersion)) .detail("LogType", metaLogType.orDefault(-1));
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (logStartVersionOverride != invalidVersion && metaUnreliableEnd.orDefault(invalidVersion) < logStartVersionOverride)
															#line 2312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaUnreliableEnd = logStartVersionOverride;
															#line 2316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!metaLogBegin.present() || !metaLogEnd.present() || metaLogEnd.get() <= metaLogBegin.get() || metaLogEnd.get() < metaExpiredEnd.orDefault(invalidVersion) || metaLogEnd.get() < metaUnreliableEnd.orDefault(invalidVersion))
															#line 2320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarnAlways, "BackupContainerMetadataInvalid") .detail("URL", bc->getURL()) .detail("ExpiredEndVersion", metaExpiredEnd.orDefault(invalidVersion)) .detail("UnreliableEndVersion", metaUnreliableEnd.orDefault(invalidVersion)) .detail("LogBeginVersion", metaLogBegin.orDefault(invalidVersion)) .detail("LogEndVersion", metaLogEnd.orDefault(invalidVersion));
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaLogBegin = Optional<Version>();
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaLogEnd = Optional<Version>();
															#line 2328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!metaUnreliableEnd.present() || metaUnreliableEnd.get() < metaExpiredEnd.orDefault(0))
															#line 2332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaUnreliableEnd = metaExpiredEnd;
															#line 2336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc.unreliableEndVersion = metaUnreliableEnd;
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc.expiredEndVersion = metaExpiredEnd;
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		scanBegin = desc.unreliableEndVersion.orDefault(0);
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		scanEnd = std::numeric_limits<Version>::max();
															#line 505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (metaLogBegin.present() && metaLogEnd.present())
															#line 2348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.minLogBegin = std::max(metaLogBegin.get(), desc.unreliableEndVersion.orDefault(0));
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.maxLogEnd = metaLogEnd.get();
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.contiguousLogEnd = desc.maxLogEnd;
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			scanBegin = desc.contiguousLogEnd.get();
															#line 2358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs = std::vector<LogFile>();
															#line 520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		plogs = std::vector<LogFile>();
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent("BackupContainerListFiles").detail("URL", bc->getURL());
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_3 = store(logs, bc->listLogFiles(scanBegin, scanEnd, false)) && store(plogs, bc->listLogFiles(scanBegin, scanEnd, true)) && store(desc.snapshots, bc->listKeyspaceSnapshots());
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<DescribeBackupActor*>(this)->actor_wait_state = 4;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 3, Void >*>(static_cast<DescribeBackupActor*>(this)));
															#line 2375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<DescribeBackupActor*>(this)->actor_wait_state > 0) static_cast<DescribeBackupActor*>(this)->actor_wait_state = 0;
		static_cast<DescribeBackupActor*>(this)->ActorCallback< DescribeBackupActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DescribeBackupActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< DescribeBackupActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< DescribeBackupActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont8(Void const& _,int loopDepth) 
	{
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent("BackupContainerListFiles") .detail("URL", bc->getURL()) .detail("LogFiles", logs.size()) .detail("PLogsFiles", plogs.size()) .detail("Snapshots", desc.snapshots.size());
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (plogs.size() > 0)
															#line 2449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.partitioned = true;
															#line 535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			logs.swap(plogs);
															#line 2455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
		else
		{
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.partitioned = metaLogType.present() && metaLogType.get() == BackupContainerFileSystemImpl::PARTITIONED_MUTATION_LOG;
															#line 2461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::sort(logs.begin(), logs.end());
															#line 545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logs.empty())
															#line 2467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.maxLogEnd = logs.rbegin()->endVersion;
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!desc.contiguousLogEnd.present())
															#line 2473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				desc.minLogBegin = logs.begin()->beginVersion;
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (desc.partitioned)
															#line 2479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.contiguousLogEnd = logs.begin()->beginVersion;
															#line 2483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
				else
				{
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.contiguousLogEnd = logs.begin()->endVersion;
															#line 2489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (desc.partitioned)
															#line 2494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				updatePartitionedLogsContinuousEnd(&desc, logs, scanBegin, scanEnd);
															#line 2498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
			else
			{
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				Version& end = desc.contiguousLogEnd.get();
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				computeRestoreEndVersion(logs, nullptr, &end, std::numeric_limits<Version>::max());
															#line 2506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (logStartVersionOverride == invalidVersion)
															#line 2511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			try {
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				updates = Void();
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (desc.minLogBegin.present() && metaLogBegin != desc.minLogBegin)
															#line 2518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					updates = updates && bc->logBeginVersion().set(desc.minLogBegin.get());
															#line 2522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (desc.contiguousLogEnd.present() && metaLogEnd != desc.contiguousLogEnd)
															#line 2526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					updates = updates && bc->logEndVersion().set(desc.contiguousLogEnd.get());
															#line 2530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!metaLogType.present())
															#line 2534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					updates = updates && bc->logType().set(desc.partitioned ? BackupContainerFileSystemImpl::PARTITIONED_MUTATION_LOG : BackupContainerFileSystemImpl::NON_PARTITIONED_MUTATION_LOG);
															#line 2538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Void> __when_expr_4 = updates;
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1cont8Catch1(actor_cancelled(), loopDepth);
															#line 2544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1cont8Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont8when1(__when_expr_4.get(), loopDepth); };
				static_cast<DescribeBackupActor*>(this)->actor_wait_state = 5;
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 4, Void >*>(static_cast<DescribeBackupActor*>(this)));
															#line 2549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				loopDepth = 0;
			}
			catch (Error& error) {
				loopDepth = a_body1cont8Catch1(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1cont8Catch1(unknown_error(), loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1cont13(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont8(Void && _,int loopDepth) 
	{
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent("BackupContainerListFiles") .detail("URL", bc->getURL()) .detail("LogFiles", logs.size()) .detail("PLogsFiles", plogs.size()) .detail("Snapshots", desc.snapshots.size());
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (plogs.size() > 0)
															#line 2571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.partitioned = true;
															#line 535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			logs.swap(plogs);
															#line 2577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
		else
		{
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.partitioned = metaLogType.present() && metaLogType.get() == BackupContainerFileSystemImpl::PARTITIONED_MUTATION_LOG;
															#line 2583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::sort(logs.begin(), logs.end());
															#line 545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logs.empty())
															#line 2589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.maxLogEnd = logs.rbegin()->endVersion;
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!desc.contiguousLogEnd.present())
															#line 2595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				desc.minLogBegin = logs.begin()->beginVersion;
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (desc.partitioned)
															#line 2601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.contiguousLogEnd = logs.begin()->beginVersion;
															#line 2605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
				else
				{
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.contiguousLogEnd = logs.begin()->endVersion;
															#line 2611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (desc.partitioned)
															#line 2616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				updatePartitionedLogsContinuousEnd(&desc, logs, scanBegin, scanEnd);
															#line 2620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
			else
			{
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				Version& end = desc.contiguousLogEnd.get();
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				computeRestoreEndVersion(logs, nullptr, &end, std::numeric_limits<Version>::max());
															#line 2628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (logStartVersionOverride == invalidVersion)
															#line 2633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			try {
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				updates = Void();
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (desc.minLogBegin.present() && metaLogBegin != desc.minLogBegin)
															#line 2640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					updates = updates && bc->logBeginVersion().set(desc.minLogBegin.get());
															#line 2644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (desc.contiguousLogEnd.present() && metaLogEnd != desc.contiguousLogEnd)
															#line 2648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					updates = updates && bc->logEndVersion().set(desc.contiguousLogEnd.get());
															#line 2652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!metaLogType.present())
															#line 2656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					updates = updates && bc->logType().set(desc.partitioned ? BackupContainerFileSystemImpl::PARTITIONED_MUTATION_LOG : BackupContainerFileSystemImpl::NON_PARTITIONED_MUTATION_LOG);
															#line 2660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Void> __when_expr_4 = updates;
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1cont8Catch1(actor_cancelled(), loopDepth);
															#line 2666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1cont8Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont8when1(__when_expr_4.get(), loopDepth); };
				static_cast<DescribeBackupActor*>(this)->actor_wait_state = 5;
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 4, Void >*>(static_cast<DescribeBackupActor*>(this)));
															#line 2671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				loopDepth = 0;
			}
			catch (Error& error) {
				loopDepth = a_body1cont8Catch1(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1cont8Catch1(unknown_error(), loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1cont13(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont6when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<DescribeBackupActor*>(this)->actor_wait_state > 0) static_cast<DescribeBackupActor*>(this)->actor_wait_state = 0;
		static_cast<DescribeBackupActor*>(this)->ActorCallback< DescribeBackupActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DescribeBackupActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< DescribeBackupActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< DescribeBackupActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont13(int loopDepth) 
	{
															#line 600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto& s : desc.snapshots ) {
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			s.restorable = true;
															#line 604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (s.beginVersion != s.endVersion)
															#line 2758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!desc.minLogBegin.present() || desc.minLogBegin.get() > s.beginVersion)
															#line 2762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					s.restorable = false;
															#line 2766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!desc.contiguousLogEnd.present() || desc.contiguousLogEnd.get() <= s.endVersion)
															#line 2770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					s.restorable = false;
															#line 2774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.snapshotBytes += s.totalSize;
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (s.beginVersion == s.endVersion)
															#line 2781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!desc.minRestorableVersion.present() || s.endVersion < desc.minRestorableVersion.get())
															#line 2785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.minRestorableVersion = s.endVersion;
															#line 2789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!desc.maxRestorableVersion.present() || s.endVersion > desc.maxRestorableVersion.get())
															#line 2793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.maxRestorableVersion = s.endVersion;
															#line 2797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (desc.minLogBegin.present() && s.beginVersion >= desc.minLogBegin.get() && s.endVersion < desc.contiguousLogEnd.get())
															#line 2802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!desc.minRestorableVersion.present() || s.endVersion < desc.minRestorableVersion.get())
															#line 2806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.minRestorableVersion = s.endVersion;
															#line 2810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!desc.maxRestorableVersion.present() || (desc.contiguousLogEnd.get() - 1) > desc.maxRestorableVersion.get())
															#line 2814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.maxRestorableVersion = desc.contiguousLogEnd.get() - 1;
															#line 2818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
		}
															#line 635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<DescribeBackupActor*>(this)->SAV<BackupDescription>::futures) { (void)(desc); this->~DescribeBackupActorState(); static_cast<DescribeBackupActor*>(this)->destroy(); return 0; }
															#line 2824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<DescribeBackupActor*>(this)->SAV< BackupDescription >::value()) BackupDescription(std::move(desc)); // state_var_RVO
		this->~DescribeBackupActorState();
		static_cast<DescribeBackupActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont22(int loopDepth) 
	{
		loopDepth = a_body1cont13(loopDepth);

		return loopDepth;
	}
	int a_body1cont8Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 2843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarn, "BackupContainerMetadataUpdateFailure").error(e).detail("URL", bc->getURL());
															#line 2851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = a_body1cont22(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont23(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont23cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont23(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont23cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont23(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont8when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont23(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<DescribeBackupActor*>(this)->actor_wait_state > 0) static_cast<DescribeBackupActor*>(this)->actor_wait_state = 0;
		static_cast<DescribeBackupActor*>(this)->ActorCallback< DescribeBackupActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DescribeBackupActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont8when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont8Catch1(error, 0);
		} catch (...) {
			a_body1cont8Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< DescribeBackupActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont8when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont8Catch1(error, 0);
		} catch (...) {
			a_body1cont8Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< DescribeBackupActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont8Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont8Catch1(error, 0);
		} catch (...) {
			a_body1cont8Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont23cont1(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont22(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	bool deepScan;
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version logStartVersionOverride;
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	BackupDescription desc;
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<Version> metaLogBegin;
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<Version> metaLogEnd;
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<Version> metaExpiredEnd;
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<Version> metaUnreliableEnd;
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<Version> metaLogType;
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version scanBegin;
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version scanEnd;
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> logs;
															#line 520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> plogs;
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Future<Void> updates;
															#line 2978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via describeBackup()
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class DescribeBackupActor final : public Actor<BackupDescription>, public ActorCallback< DescribeBackupActor, 0, bool >, public ActorCallback< DescribeBackupActor, 1, BackupDescription >, public ActorCallback< DescribeBackupActor, 2, Void >, public ActorCallback< DescribeBackupActor, 3, Void >, public ActorCallback< DescribeBackupActor, 4, Void >, public FastAllocated<DescribeBackupActor>, public DescribeBackupActorState<DescribeBackupActor> {
															#line 2983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<DescribeBackupActor>::operator new;
	using FastAllocated<DescribeBackupActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<BackupDescription>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DescribeBackupActor, 0, bool >;
friend struct ActorCallback< DescribeBackupActor, 1, BackupDescription >;
friend struct ActorCallback< DescribeBackupActor, 2, Void >;
friend struct ActorCallback< DescribeBackupActor, 3, Void >;
friend struct ActorCallback< DescribeBackupActor, 4, Void >;
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	DescribeBackupActor(Reference<BackupContainerFileSystem> const& bc,bool const& deepScan,Version const& logStartVersionOverride) 
															#line 2998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<BackupDescription>(),
		   DescribeBackupActorState<DescribeBackupActor>(bc, deepScan, logStartVersionOverride)
	{
		fdb_probe_actor_enter("describeBackup", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("describeBackup");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("describeBackup", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DescribeBackupActor, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DescribeBackupActor, 1, BackupDescription >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< DescribeBackupActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< DescribeBackupActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< DescribeBackupActor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<BackupDescription> describeBackup( Reference<BackupContainerFileSystem> const& bc, bool const& deepScan, Version const& logStartVersionOverride ) {
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<BackupDescription>(new DescribeBackupActor(bc, deepScan, logStartVersionOverride));
															#line 3029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

																#line 3034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via expireData()
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class ExpireDataActor>
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ExpireDataActorState {
															#line 3040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ExpireDataActorState(Reference<BackupContainerFileSystem> const& bc,Version const& expireEndVersion,bool const& force,IBackupContainer::ExpireProgress* const& progress,Version const& restorableBeginVersion) 
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   expireEndVersion(expireEndVersion),
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   force(force),
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   progress(progress),
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   restorableBeginVersion(restorableBeginVersion)
															#line 3055 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("expireData", reinterpret_cast<unsigned long>(this));

	}
	~ExpireDataActorState() 
	{
		fdb_probe_actor_destroy("expireData", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (progress != nullptr)
															#line 3070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				progress->step = "Describing backup";
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				progress->total = 0;
															#line 3076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent("BackupContainerFileSystemExpire1") .detail("URL", bc->getURL()) .detail("ExpireEndVersion", expireEndVersion) .detail("RestorableBeginVersion", restorableBeginVersion);
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<BackupDescription> __when_expr_0 = bc->describeBackup(false, expireEndVersion);
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ExpireDataActor*>(this)->actor_wait_state = 1;
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 0, BackupDescription >*>(static_cast<ExpireDataActor*>(this)));
															#line 3089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~ExpireDataActorState();
		static_cast<ExpireDataActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		expireEndVersion = resolveRelativeVersion(desc.maxLogEnd, expireEndVersion, "ExpireEndVersion", invalid_option_value());
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		restorableBeginVersion = resolveRelativeVersion( desc.maxLogEnd, restorableBeginVersion, "RestorableBeginVersion", invalid_option_value());
															#line 663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (restorableBeginVersion < expireEndVersion)
															#line 3116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(backup_cannot_expire(), loopDepth);
															#line 3120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (expireEndVersion <= desc.expiredEndVersion.orDefault(invalidVersion))
															#line 3124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<ExpireDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExpireDataActorState(); static_cast<ExpireDataActor*>(this)->destroy(); return 0; }
															#line 3128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<ExpireDataActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ExpireDataActorState();
			static_cast<ExpireDataActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		forceNeeded = true;
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( KeyspaceSnapshotFile& s : desc.snapshots ) {
															#line 678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (s.restorable.orDefault(false) && s.beginVersion >= expireEndVersion && s.endVersion <= restorableBeginVersion)
															#line 3140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				forceNeeded = false;
															#line 3144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				break;
			}
		}
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (forceNeeded && !force)
															#line 3150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(backup_cannot_expire(), loopDepth);
															#line 3154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		scanBegin = desc.expiredEndVersion.orDefault(0);
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent("BackupContainerFileSystemExpire2") .detail("URL", bc->getURL()) .detail("ExpireEndVersion", expireEndVersion) .detail("RestorableBeginVersion", restorableBeginVersion) .detail("ScanBeginVersion", scanBegin);
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs = std::vector<LogFile>();
															#line 702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		pLogs = std::vector<LogFile>();
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ranges = std::vector<RangeFile>();
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (progress != nullptr)
															#line 3168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->step = "Listing files";
															#line 3172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_1 = store(logs, bc->listLogFiles(scanBegin, expireEndVersion - 1, false)) && store(pLogs, bc->listLogFiles(scanBegin, expireEndVersion - 1, true)) && store(ranges, bc->listRangeFiles(scanBegin, expireEndVersion - 1));
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ExpireDataActor*>(this)->actor_wait_state = 2;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 1, Void >*>(static_cast<ExpireDataActor*>(this)));
															#line 3183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(BackupDescription const& __desc,int loopDepth) 
	{
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc = __desc;
															#line 3192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(BackupDescription && __desc,int loopDepth) 
	{
		desc = std::move(__desc);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state > 0) static_cast<ExpireDataActor*>(this)->actor_wait_state = 0;
		static_cast<ExpireDataActor*>(this)->ActorCallback< ExpireDataActor, 0, BackupDescription >::remove();

	}
	void a_callback_fire(ActorCallback< ExpireDataActor, 0, BackupDescription >*,BackupDescription const& value) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ExpireDataActor, 0, BackupDescription >*,BackupDescription && value) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ExpireDataActor, 0, BackupDescription >*,Error err) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs.insert(logs.end(), std::make_move_iterator(pLogs.begin()), std::make_move_iterator(pLogs.end()));
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		newLogBeginVersion = Optional<Version>();
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logs.empty())
															#line 3263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			LogFile& last = *std::max_element(logs.begin(), logs.end());
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (last.endVersion == expireEndVersion)
															#line 3269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				newLogBeginVersion = expireEndVersion;
															#line 3273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
			else
			{
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (last.endVersion > expireEndVersion)
															#line 3279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					newLogBeginVersion = last.beginVersion;
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					last = LogFile();
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					expireEndVersion = newLogBeginVersion.get();
															#line 3287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
		}
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		toDelete = std::vector<std::string>();
															#line 741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& f : logs ) {
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!f.fileName.empty())
															#line 3297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				toDelete.push_back(std::move(f.fileName));
															#line 3301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs.clear();
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& f : ranges ) {
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (f.version < expireEndVersion)
															#line 3310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				toDelete.push_back(std::move(f.fileName));
															#line 3314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ranges.clear();
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& f : desc.snapshots ) {
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (f.endVersion < expireEndVersion)
															#line 3323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				toDelete.push_back(std::move(f.fileName));
															#line 3327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc = BackupDescription();
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (progress != nullptr)
															#line 3334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->step = "Initial metadata update";
															#line 3338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Optional<Version>> __when_expr_2 = bc->unreliableEndVersion().get();
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ExpireDataActor*>(this)->actor_wait_state = 3;
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 2, Optional<Version> >*>(static_cast<ExpireDataActor*>(this)));
															#line 3349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs.insert(logs.end(), std::make_move_iterator(pLogs.begin()), std::make_move_iterator(pLogs.end()));
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		newLogBeginVersion = Optional<Version>();
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logs.empty())
															#line 3362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			LogFile& last = *std::max_element(logs.begin(), logs.end());
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (last.endVersion == expireEndVersion)
															#line 3368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				newLogBeginVersion = expireEndVersion;
															#line 3372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
			else
			{
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (last.endVersion > expireEndVersion)
															#line 3378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					newLogBeginVersion = last.beginVersion;
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					last = LogFile();
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					expireEndVersion = newLogBeginVersion.get();
															#line 3386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
		}
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		toDelete = std::vector<std::string>();
															#line 741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& f : logs ) {
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!f.fileName.empty())
															#line 3396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				toDelete.push_back(std::move(f.fileName));
															#line 3400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs.clear();
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& f : ranges ) {
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (f.version < expireEndVersion)
															#line 3409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				toDelete.push_back(std::move(f.fileName));
															#line 3413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ranges.clear();
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& f : desc.snapshots ) {
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (f.endVersion < expireEndVersion)
															#line 3422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				toDelete.push_back(std::move(f.fileName));
															#line 3426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc = BackupDescription();
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (progress != nullptr)
															#line 3433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->step = "Initial metadata update";
															#line 3437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Optional<Version>> __when_expr_2 = bc->unreliableEndVersion().get();
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ExpireDataActor*>(this)->actor_wait_state = 3;
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 2, Optional<Version> >*>(static_cast<ExpireDataActor*>(this)));
															#line 3448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state > 0) static_cast<ExpireDataActor*>(this)->actor_wait_state = 0;
		static_cast<ExpireDataActor*>(this)->ActorCallback< ExpireDataActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExpireDataActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ExpireDataActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ExpireDataActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont10(Optional<Version> const& metaUnreliableEnd,int loopDepth) 
	{
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (metaUnreliableEnd.orDefault(0) < expireEndVersion)
															#line 3520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_3 = bc->unreliableEndVersion().set(expireEndVersion);
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont10when1(__when_expr_3.get(), loopDepth); };
			static_cast<ExpireDataActor*>(this)->actor_wait_state = 4;
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 3, Void >*>(static_cast<ExpireDataActor*>(this)));
															#line 3531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont10cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont10(Optional<Version> && metaUnreliableEnd,int loopDepth) 
	{
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (metaUnreliableEnd.orDefault(0) < expireEndVersion)
															#line 3545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_3 = bc->unreliableEndVersion().set(expireEndVersion);
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont10when1(__when_expr_3.get(), loopDepth); };
			static_cast<ExpireDataActor*>(this)->actor_wait_state = 4;
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 3, Void >*>(static_cast<ExpireDataActor*>(this)));
															#line 3556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont10cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3when1(Optional<Version> const& metaUnreliableEnd,int loopDepth) 
	{
		loopDepth = a_body1cont10(metaUnreliableEnd, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Optional<Version> && metaUnreliableEnd,int loopDepth) 
	{
		loopDepth = a_body1cont10(std::move(metaUnreliableEnd), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state > 0) static_cast<ExpireDataActor*>(this)->actor_wait_state = 0;
		static_cast<ExpireDataActor*>(this)->ActorCallback< ExpireDataActor, 2, Optional<Version> >::remove();

	}
	void a_callback_fire(ActorCallback< ExpireDataActor, 2, Optional<Version> >*,Optional<Version> const& value) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ExpireDataActor, 2, Optional<Version> >*,Optional<Version> && value) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ExpireDataActor, 2, Optional<Version> >*,Error err) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont10cont1(int loopDepth) 
	{
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (progress != nullptr)
															#line 3633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->step = "Deleting files";
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->total = toDelete.size();
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->done = 0;
															#line 3641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		deleteFutures = std::list<Future<Void>>();
															#line 788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 3647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont10cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont10when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont10when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state > 0) static_cast<ExpireDataActor*>(this)->actor_wait_state = 0;
		static_cast<ExpireDataActor*>(this)->ActorCallback< ExpireDataActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExpireDataActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont10when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ExpireDataActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont10when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ExpireDataActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont10cont3(int loopDepth) 
	{
															#line 810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (progress != nullptr)
															#line 3731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->step = "Final metadata update";
															#line 812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->total = 0;
															#line 3737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Optional<Version>> __when_expr_5 = bc->expiredEndVersion().get();
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont10cont3when1(__when_expr_5.get(), loopDepth); };
		static_cast<ExpireDataActor*>(this)->actor_wait_state = 6;
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 5, Optional<Version> >*>(static_cast<ExpireDataActor*>(this)));
															#line 3748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont10cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont10cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1(int loopDepth) 
	{
															#line 788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(!toDelete.empty() || !deleteFutures.empty()))
															#line 3764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont10cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for(;!toDelete.empty() && deleteFutures.size() < CLIENT_KNOBS->BACKUP_CONCURRENT_DELETES;) {
															#line 792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			deleteFutures.push_back(bc->deleteFile(toDelete.back()));
															#line 793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			toDelete.pop_back();
															#line 3774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		targetFuturesSize = toDelete.empty() ? 0 : (CLIENT_KNOBS->BACKUP_CONCURRENT_DELETES - 1);
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 3780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont10cont1loopBody1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1break1(int loopDepth) 
	{
		try {
			return a_body1cont10cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1cont10cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont10cont1loopBody1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1loopBody1(int loopDepth) 
	{
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(deleteFutures.size() > targetFuturesSize))
															#line 3815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont10cont1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_4 = deleteFutures.front();
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 3823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 2)); else return a_body1cont10cont1loopBody1loopBody1when1(__when_expr_4.get(), loopDepth); };
		static_cast<ExpireDataActor*>(this)->actor_wait_state = 5;
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 4, Void >*>(static_cast<ExpireDataActor*>(this)));
															#line 3828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1break1(int loopDepth) 
	{
		try {
			return a_body1cont10cont1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (progress != nullptr)
															#line 3850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			++progress->done;
															#line 3854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		deleteFutures.pop_front();
															#line 3858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont10cont1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (progress != nullptr)
															#line 3867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			++progress->done;
															#line 3871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		deleteFutures.pop_front();
															#line 3875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont10cont1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1loopBody1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1loopBody1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state > 0) static_cast<ExpireDataActor*>(this)->actor_wait_state = 0;
		static_cast<ExpireDataActor*>(this)->ActorCallback< ExpireDataActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExpireDataActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont10cont1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< ExpireDataActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont10cont1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< ExpireDataActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont10cont5(Optional<Version> const& metaExpiredEnd,int loopDepth) 
	{
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (metaExpiredEnd.orDefault(0) < expireEndVersion)
															#line 3947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_6 = bc->expiredEndVersion().set(expireEndVersion);
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont10cont5when1(__when_expr_6.get(), loopDepth); };
			static_cast<ExpireDataActor*>(this)->actor_wait_state = 7;
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 6, Void >*>(static_cast<ExpireDataActor*>(this)));
															#line 3958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont10cont7(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont10cont5(Optional<Version> && metaExpiredEnd,int loopDepth) 
	{
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (metaExpiredEnd.orDefault(0) < expireEndVersion)
															#line 3972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_6 = bc->expiredEndVersion().set(expireEndVersion);
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont10cont5when1(__when_expr_6.get(), loopDepth); };
			static_cast<ExpireDataActor*>(this)->actor_wait_state = 7;
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 6, Void >*>(static_cast<ExpireDataActor*>(this)));
															#line 3983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont10cont7(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont10cont3when1(Optional<Version> const& metaExpiredEnd,int loopDepth) 
	{
		loopDepth = a_body1cont10cont5(metaExpiredEnd, loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont3when1(Optional<Version> && metaExpiredEnd,int loopDepth) 
	{
		loopDepth = a_body1cont10cont5(std::move(metaExpiredEnd), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state > 0) static_cast<ExpireDataActor*>(this)->actor_wait_state = 0;
		static_cast<ExpireDataActor*>(this)->ActorCallback< ExpireDataActor, 5, Optional<Version> >::remove();

	}
	void a_callback_fire(ActorCallback< ExpireDataActor, 5, Optional<Version> >*,Optional<Version> const& value) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont10cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< ExpireDataActor, 5, Optional<Version> >*,Optional<Version> && value) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont10cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< ExpireDataActor, 5, Optional<Version> >*,Error err) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1cont10cont7(int loopDepth) 
	{
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<ExpireDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExpireDataActorState(); static_cast<ExpireDataActor*>(this)->destroy(); return 0; }
															#line 4060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<ExpireDataActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ExpireDataActorState();
		static_cast<ExpireDataActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10cont8(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont8(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state > 0) static_cast<ExpireDataActor*>(this)->actor_wait_state = 0;
		static_cast<ExpireDataActor*>(this)->ActorCallback< ExpireDataActor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExpireDataActor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont10cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< ExpireDataActor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont10cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< ExpireDataActor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), 6);

	}
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version expireEndVersion;
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	bool force;
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	IBackupContainer::ExpireProgress* progress;
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version restorableBeginVersion;
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	BackupDescription desc;
															#line 676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	bool forceNeeded;
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version scanBegin;
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> logs;
															#line 702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> pLogs;
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<RangeFile> ranges;
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<Version> newLogBeginVersion;
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<std::string> toDelete;
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::list<Future<Void>> deleteFutures;
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int targetFuturesSize;
															#line 4173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via expireData()
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ExpireDataActor final : public Actor<Void>, public ActorCallback< ExpireDataActor, 0, BackupDescription >, public ActorCallback< ExpireDataActor, 1, Void >, public ActorCallback< ExpireDataActor, 2, Optional<Version> >, public ActorCallback< ExpireDataActor, 3, Void >, public ActorCallback< ExpireDataActor, 4, Void >, public ActorCallback< ExpireDataActor, 5, Optional<Version> >, public ActorCallback< ExpireDataActor, 6, Void >, public FastAllocated<ExpireDataActor>, public ExpireDataActorState<ExpireDataActor> {
															#line 4178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<ExpireDataActor>::operator new;
	using FastAllocated<ExpireDataActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ExpireDataActor, 0, BackupDescription >;
friend struct ActorCallback< ExpireDataActor, 1, Void >;
friend struct ActorCallback< ExpireDataActor, 2, Optional<Version> >;
friend struct ActorCallback< ExpireDataActor, 3, Void >;
friend struct ActorCallback< ExpireDataActor, 4, Void >;
friend struct ActorCallback< ExpireDataActor, 5, Optional<Version> >;
friend struct ActorCallback< ExpireDataActor, 6, Void >;
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ExpireDataActor(Reference<BackupContainerFileSystem> const& bc,Version const& expireEndVersion,bool const& force,IBackupContainer::ExpireProgress* const& progress,Version const& restorableBeginVersion) 
															#line 4195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   ExpireDataActorState<ExpireDataActor>(bc, expireEndVersion, force, progress, restorableBeginVersion)
	{
		fdb_probe_actor_enter("expireData", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("expireData");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("expireData", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ExpireDataActor, 0, BackupDescription >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ExpireDataActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ExpireDataActor, 2, Optional<Version> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ExpireDataActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< ExpireDataActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< ExpireDataActor, 5, Optional<Version> >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< ExpireDataActor, 6, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Void> expireData( Reference<BackupContainerFileSystem> const& bc, Version const& expireEndVersion, bool const& force, IBackupContainer::ExpireProgress* const& progress, Version const& restorableBeginVersion ) {
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new ExpireDataActor(bc, expireEndVersion, force, progress, restorableBeginVersion));
															#line 4228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

	// Returns true if logs are continuous in the range [begin, end].
	// "files" should be pre-sorted according to version order.
	static bool isPartitionedLogsContinuous(const std::vector<LogFile>& files, Version begin, Version end) {
		std::map<int, std::vector<int>> tagIndices; // tagId -> indices in files
		for (int i = 0; i < files.size(); i++) {
			ASSERT(files[i].tagId >= 0 && files[i].tagId < files[i].totalTags);
			auto& indices = tagIndices[files[i].tagId];
			indices.push_back(i);
		}

		// check partition 0 is continuous and create a map of ranges to tags
		std::map<std::pair<Version, Version>, int> tags; // range [begin, end] -> tags
		if (!isContinuous(files, tagIndices[0], begin, end, &tags)) {
			TraceEvent(SevWarn, "BackupFileNotContinuous")
			    .detail("Partition", 0)
			    .detail("RangeBegin", begin)
			    .detail("RangeEnd", end);
			return false;
		}

		// for each range in tags, check all tags from 1 are continouous
		for (const auto& [beginEnd, count] : tags) {
			for (int i = 1; i < count; i++) {
				if (!isContinuous(files, tagIndices[i], beginEnd.first, std::min(beginEnd.second - 1, end), nullptr)) {
					TraceEvent(SevWarn, "BackupFileNotContinuous")
					    .detail("Partition", i)
					    .detail("RangeBegin", beginEnd.first)
					    .detail("RangeEnd", beginEnd.second);
					return false;
				}
			}
		}
		return true;
	}

	// Returns log files that are not duplicated, or subset of another log.
	// If a log file's progress is not saved, a new log file will be generated
	// with the same begin version. So we can have a file that contains a subset
	// of contents in another log file.
	// PRE-CONDITION: logs are already sorted by (tagId, beginVersion, endVersion).
	static std::vector<LogFile> filterDuplicates(const std::vector<LogFile>& logs) {
		std::vector<LogFile> filtered;
		int i = 0;
		for (int j = 1; j < logs.size(); j++) {
			if (logs[j].isSubset(logs[i])) {
				ASSERT_LE(logs[j].fileSize, logs[i].fileSize);
				continue;
			}

			if (!logs[i].isSubset(logs[j])) {
				filtered.push_back(logs[i]);
			}
			i = j;
		}
		if (i < logs.size())
			filtered.push_back(logs[i]);
		return filtered;
	}

	static Optional<RestorableFileSet> getRestoreSetFromLogs(const std::vector<LogFile>& logs,
	                                                         Version targetVersion,
	                                                         RestorableFileSet restorable) {
		Version end = logs.begin()->endVersion;
		computeRestoreEndVersion(logs, &restorable.logs, &end, targetVersion);
		if (end >= targetVersion) {
			restorable.continuousBeginVersion = logs.begin()->beginVersion;
			restorable.continuousEndVersion = end;
			return Optional<RestorableFileSet>(restorable);
		}
		return Optional<RestorableFileSet>();
	}

	// Get a set of files that can restore the given "keyRangesFilter" to the "targetVersion".
	// If "keyRangesFilter" is empty, the file set will cover all key ranges present in the backup.
	// It's generally a good idea to specify "keyRangesFilter" to reduce the number of files for
	// restore times.
	//
	// If "logsOnly" is true, then only log files are returned and "keyRangesFilter" is ignored,
	// because the log can contain mutations of the whole key space, unlike range files that each
	// is limited to a smaller key range.
																#line 4313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via getRestoreSet()
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class GetRestoreSetActor>
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class GetRestoreSetActorState {
															#line 4319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	GetRestoreSetActorState(Reference<BackupContainerFileSystem> const& bc,Version const& targetVersion,VectorRef<KeyRangeRef> const& keyRangesFilter,bool const& logsOnly = false,Version const& beginVersion = invalidVersion) 
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   targetVersion(targetVersion),
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   keyRangesFilter(keyRangesFilter),
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   logsOnly(logsOnly),
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   beginVersion(beginVersion)
															#line 4334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("getRestoreSet", reinterpret_cast<unsigned long>(this));

	}
	~GetRestoreSetActorState() 
	{
		fdb_probe_actor_destroy("getRestoreSet", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			for( const auto& range : keyRangesFilter ) {
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent("BackupContainerGetRestoreSet").detail("RangeFilter", printable(range));
															#line 4351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (logsOnly)
															#line 4355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				restorableSet = RestorableFileSet();
															#line 915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				restorableSet.targetVersion = targetVersion;
															#line 916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				logFiles = std::vector<LogFile>();
															#line 917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				Version begin = beginVersion == invalidVersion ? 0 : beginVersion;
															#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Void> __when_expr_0 = store(logFiles, bc->listLogFiles(begin, targetVersion, false));
															#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 1;
															#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetRestoreSetActor, 0, Void >*>(static_cast<GetRestoreSetActor*>(this)));
															#line 4374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~GetRestoreSetActorState();
		static_cast<GetRestoreSetActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<std::vector<KeyspaceSnapshotFile>> __when_expr_1 = bc->listKeyspaceSnapshots();
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 2;
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetRestoreSetActor, 1, std::vector<KeyspaceSnapshotFile> >*>(static_cast<GetRestoreSetActor*>(this)));
															#line 4409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::sort(logFiles.begin(), logFiles.end());
															#line 921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logFiles.empty())
															#line 4420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(getRestoreSetFromLogs(logFiles, targetVersion, restorableSet)); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 4424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(getRestoreSetFromLogs(logFiles, targetVersion, restorableSet));
			this->~GetRestoreSetActorState();
			static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::sort(logFiles.begin(), logFiles.end());
															#line 921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logFiles.empty())
															#line 4440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(getRestoreSetFromLogs(logFiles, targetVersion, restorableSet)); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 4444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(getRestoreSetFromLogs(logFiles, targetVersion, restorableSet));
			this->~GetRestoreSetActorState();
			static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont1(loopDepth);

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
		if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state > 0) static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 0;
		static_cast<GetRestoreSetActor*>(this)->ActorCallback< GetRestoreSetActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetRestoreSetActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetRestoreSetActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetRestoreSetActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont6(int loopDepth) 
	{
															#line 928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		i = snapshots.size() - 1;
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 4523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont6loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(std::vector<KeyspaceSnapshotFile> const& __snapshots,int loopDepth) 
	{
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshots = __snapshots;
															#line 4532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(std::vector<KeyspaceSnapshotFile> && __snapshots,int loopDepth) 
	{
		snapshots = std::move(__snapshots);
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state > 0) static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 0;
		static_cast<GetRestoreSetActor*>(this)->ActorCallback< GetRestoreSetActor, 1, std::vector<KeyspaceSnapshotFile> >::remove();

	}
	void a_callback_fire(ActorCallback< GetRestoreSetActor, 1, std::vector<KeyspaceSnapshotFile> >*,std::vector<KeyspaceSnapshotFile> const& value) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetRestoreSetActor, 1, std::vector<KeyspaceSnapshotFile> >*,std::vector<KeyspaceSnapshotFile> && value) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetRestoreSetActor, 1, std::vector<KeyspaceSnapshotFile> >*,Error err) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont7(int loopDepth) 
	{
															#line 1034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(Optional<RestorableFileSet>()); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 4599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(Optional<RestorableFileSet>());
		this->~GetRestoreSetActorState();
		static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont6loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1(int loopDepth) 
	{
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(i >= 0))
															#line 4618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont6break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (targetVersion >= 0 && snapshots[i].beginVersion > targetVersion)
															#line 4624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont6continue1(loopDepth); // continue
		}
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		restorable = RestorableFileSet();
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		minKeyRangeVersion = MAX_VERSION;
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		maxKeyRangeVersion = -1;
															#line 939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>> __when_expr_2 = bc->readKeyspaceSnapshot(snapshots[i]);
															#line 939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont6loopBody1when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 3;
															#line 939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetRestoreSetActor, 2, std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> >*>(static_cast<GetRestoreSetActor*>(this)));
															#line 4643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6break1(int loopDepth) 
	{
		try {
			return a_body1cont7(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont6continue1(int loopDepth) 
	{
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		i--;
															#line 4665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont6loopHead1(0);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1(std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> const& results,int loopDepth) 
	{
															#line 943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (keyRangesFilter.size() && results.second.empty() && !results.first.empty())
															#line 4674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(backup_not_filterable_with_key_ranges(), std::max(0, loopDepth - 1));
															#line 4678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (keyRangesFilter.empty())
															#line 4682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			restorable.ranges = std::move(results.first);
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			restorable.keyRanges = std::move(results.second);
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			minKeyRangeVersion = snapshots[i].beginVersion;
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			maxKeyRangeVersion = snapshots[i].endVersion;
															#line 4692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
		else
		{
															#line 954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			for( const auto& rangeFile : results.first ) {
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				const auto& keyRange = results.second.at(rangeFile.fileName);
															#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (keyRange.intersects(keyRangesFilter))
															#line 4702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					restorable.ranges.push_back(rangeFile);
															#line 958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					restorable.keyRanges[rangeFile.fileName] = keyRange;
															#line 959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					minKeyRangeVersion = std::min(minKeyRangeVersion, rangeFile.version);
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					maxKeyRangeVersion = std::max(maxKeyRangeVersion, rangeFile.version);
															#line 4712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
															#line 964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (restorable.ranges.empty())
															#line 4717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(backup_not_overlapped_with_keys_filter(), std::max(0, loopDepth - 1));
															#line 4721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		restorable.targetVersion = targetVersion == latestVersion ? maxKeyRangeVersion : targetVersion;
															#line 971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (restorable.targetVersion < maxKeyRangeVersion)
															#line 4728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont6continue1(loopDepth); // continue
		}
															#line 974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		restorable.snapshot = snapshots[i];
															#line 976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (false && g_network->isSimulated())
															#line 4736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			rit = std::map<std::string, KeyRange>::iterator();
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			rit = restorable.keyRanges.begin();
															#line 4742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = a_body1cont6loopBody1cont1loopHead1(loopDepth);
		}
		else
		{
			loopDepth = a_body1cont6loopBody1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1(std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> && results,int loopDepth) 
	{
															#line 943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (keyRangesFilter.size() && results.second.empty() && !results.first.empty())
															#line 4756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(backup_not_filterable_with_key_ranges(), std::max(0, loopDepth - 1));
															#line 4760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (keyRangesFilter.empty())
															#line 4764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			restorable.ranges = std::move(results.first);
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			restorable.keyRanges = std::move(results.second);
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			minKeyRangeVersion = snapshots[i].beginVersion;
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			maxKeyRangeVersion = snapshots[i].endVersion;
															#line 4774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
		else
		{
															#line 954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			for( const auto& rangeFile : results.first ) {
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				const auto& keyRange = results.second.at(rangeFile.fileName);
															#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (keyRange.intersects(keyRangesFilter))
															#line 4784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					restorable.ranges.push_back(rangeFile);
															#line 958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					restorable.keyRanges[rangeFile.fileName] = keyRange;
															#line 959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					minKeyRangeVersion = std::min(minKeyRangeVersion, rangeFile.version);
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					maxKeyRangeVersion = std::max(maxKeyRangeVersion, rangeFile.version);
															#line 4794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
															#line 964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (restorable.ranges.empty())
															#line 4799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(backup_not_overlapped_with_keys_filter(), std::max(0, loopDepth - 1));
															#line 4803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		restorable.targetVersion = targetVersion == latestVersion ? maxKeyRangeVersion : targetVersion;
															#line 971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (restorable.targetVersion < maxKeyRangeVersion)
															#line 4810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont6continue1(loopDepth); // continue
		}
															#line 974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		restorable.snapshot = snapshots[i];
															#line 976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (false && g_network->isSimulated())
															#line 4818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			rit = std::map<std::string, KeyRange>::iterator();
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			rit = restorable.keyRanges.begin();
															#line 4824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = a_body1cont6loopBody1cont1loopHead1(loopDepth);
		}
		else
		{
			loopDepth = a_body1cont6loopBody1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont6loopBody1when1(std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> const& results,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1cont1(results, loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1when1(std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> && results,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1cont1(std::move(results), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state > 0) static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 0;
		static_cast<GetRestoreSetActor*>(this)->ActorCallback< GetRestoreSetActor, 2, std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> >::remove();

	}
	void a_callback_fire(ActorCallback< GetRestoreSetActor, 2, std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> >*,std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> const& value) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont6loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetRestoreSetActor, 2, std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> >*,std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> && value) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont6loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetRestoreSetActor, 2, std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> >*,Error err) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont6loopBody1cont4(int loopDepth) 
	{
															#line 990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (minKeyRangeVersion == maxKeyRangeVersion && maxKeyRangeVersion == restorable.targetVersion)
															#line 4901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			restorable.continuousBeginVersion = restorable.continuousEndVersion = invalidVersion;
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent("BackupContainerGetRestorableFilesWithoutLogs") .detail("KeyRangeVersion", restorable.targetVersion) .detail("NumberOfRangeFiles", restorable.ranges.size()) .detail("KeyRangesFilter", printable(keyRangesFilter));
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(Optional<RestorableFileSet>(restorable)); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 4909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(Optional<RestorableFileSet>(restorable));
			this->~GetRestoreSetActorState();
			static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs = std::vector<LogFile>();
															#line 1001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		plogs = std::vector<LogFile>();
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_4 = store(logs, bc->listLogFiles(minKeyRangeVersion, restorable.targetVersion, false)) && store(plogs, bc->listLogFiles(minKeyRangeVersion, restorable.targetVersion, true));
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1cont6loopBody1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 5;
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< GetRestoreSetActor, 4, Void >*>(static_cast<GetRestoreSetActor*>(this)));
															#line 4928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6loopBody1cont12(int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont6loopBody1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1loopBody1(int loopDepth) 
	{
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(rit != restorable.keyRanges.end()))
															#line 4950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont6loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		auto it = std::find_if(restorable.ranges.begin(), restorable.ranges.end(), [file = rit->first](const RangeFile f) { return f.fileName == file; });
															#line 983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(it != restorable.ranges.end());
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<KeyRange> __when_expr_3 = bc->getSnapshotFileKeyRange(*it);
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 4962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 2)); else return a_body1cont6loopBody1cont1loopBody1when1(__when_expr_3.get(), loopDepth); };
		static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 4;
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GetRestoreSetActor, 3, KeyRange >*>(static_cast<GetRestoreSetActor*>(this)));
															#line 4967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1break1(int loopDepth) 
	{
		try {
			return a_body1cont6loopBody1cont12(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1loopBody1cont1(KeyRange const& result,int loopDepth) 
	{
															#line 985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(rit->second.begin <= result.begin && rit->second.end >= result.end);
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		rit++;
															#line 4991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont6loopBody1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1loopBody1cont1(KeyRange && result,int loopDepth) 
	{
															#line 985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(rit->second.begin <= result.begin && rit->second.end >= result.end);
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		rit++;
															#line 5002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont6loopBody1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1loopBody1when1(KeyRange const& result,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1cont1loopBody1cont1(result, loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1loopBody1when1(KeyRange && result,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1cont1loopBody1cont1(std::move(result), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state > 0) static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 0;
		static_cast<GetRestoreSetActor*>(this)->ActorCallback< GetRestoreSetActor, 3, KeyRange >::remove();

	}
	void a_callback_fire(ActorCallback< GetRestoreSetActor, 3, KeyRange >*,KeyRange const& value) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6loopBody1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< GetRestoreSetActor, 3, KeyRange >*,KeyRange && value) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6loopBody1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< GetRestoreSetActor, 3, KeyRange >*,Error err) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont6loopBody1cont13(Void const& _,int loopDepth) 
	{
															#line 1005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (plogs.size() > 0)
															#line 5074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			logs.swap(plogs);
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(logs.begin(), logs.end(), [](const LogFile& a, const LogFile& b) { return std::tie(a.tagId, a.beginVersion, a.endVersion) < std::tie(b.tagId, b.beginVersion, b.endVersion); });
															#line 1014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::vector<LogFile> filtered = filterDuplicates(logs);
															#line 1015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			restorable.logs.swap(filtered);
															#line 1017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(restorable.logs.begin(), restorable.logs.end());
															#line 1018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (isPartitionedLogsContinuous(restorable.logs, minKeyRangeVersion, restorable.targetVersion))
															#line 5088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				restorable.continuousBeginVersion = minKeyRangeVersion;
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				restorable.continuousEndVersion = restorable.targetVersion + 1;
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(Optional<RestorableFileSet>(restorable)); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 5096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(Optional<RestorableFileSet>(restorable));
				this->~GetRestoreSetActorState();
				static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(Optional<RestorableFileSet>()); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 5104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(Optional<RestorableFileSet>());
			this->~GetRestoreSetActorState();
			static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::sort(logs.begin(), logs.end());
															#line 1030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logs.empty() && logs.front().beginVersion <= minKeyRangeVersion)
															#line 5114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(getRestoreSetFromLogs(logs, targetVersion, restorable)); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 5118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(getRestoreSetFromLogs(logs, targetVersion, restorable));
			this->~GetRestoreSetActorState();
			static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		i--;
															#line 5126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont6loopHead1(0);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont13(Void && _,int loopDepth) 
	{
															#line 1005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (plogs.size() > 0)
															#line 5135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			logs.swap(plogs);
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(logs.begin(), logs.end(), [](const LogFile& a, const LogFile& b) { return std::tie(a.tagId, a.beginVersion, a.endVersion) < std::tie(b.tagId, b.beginVersion, b.endVersion); });
															#line 1014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::vector<LogFile> filtered = filterDuplicates(logs);
															#line 1015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			restorable.logs.swap(filtered);
															#line 1017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(restorable.logs.begin(), restorable.logs.end());
															#line 1018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (isPartitionedLogsContinuous(restorable.logs, minKeyRangeVersion, restorable.targetVersion))
															#line 5149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				restorable.continuousBeginVersion = minKeyRangeVersion;
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				restorable.continuousEndVersion = restorable.targetVersion + 1;
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(Optional<RestorableFileSet>(restorable)); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 5157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(Optional<RestorableFileSet>(restorable));
				this->~GetRestoreSetActorState();
				static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(Optional<RestorableFileSet>()); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 5165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(Optional<RestorableFileSet>());
			this->~GetRestoreSetActorState();
			static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::sort(logs.begin(), logs.end());
															#line 1030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logs.empty() && logs.front().beginVersion <= minKeyRangeVersion)
															#line 5175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(getRestoreSetFromLogs(logs, targetVersion, restorable)); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 5179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(getRestoreSetFromLogs(logs, targetVersion, restorable));
			this->~GetRestoreSetActorState();
			static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		i--;
															#line 5187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont6loopHead1(0);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1cont13(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1cont13(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state > 0) static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 0;
		static_cast<GetRestoreSetActor*>(this)->ActorCallback< GetRestoreSetActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetRestoreSetActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont6loopBody1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< GetRestoreSetActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont6loopBody1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< GetRestoreSetActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version targetVersion;
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	VectorRef<KeyRangeRef> keyRangesFilter;
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	bool logsOnly;
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version beginVersion;
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	RestorableFileSet restorableSet;
															#line 916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> logFiles;
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<KeyspaceSnapshotFile> snapshots;
															#line 928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int i;
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	RestorableFileSet restorable;
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version minKeyRangeVersion;
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version maxKeyRangeVersion;
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::map<std::string, KeyRange>::iterator rit;
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> logs;
															#line 1001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> plogs;
															#line 5285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via getRestoreSet()
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class GetRestoreSetActor final : public Actor<Optional<RestorableFileSet>>, public ActorCallback< GetRestoreSetActor, 0, Void >, public ActorCallback< GetRestoreSetActor, 1, std::vector<KeyspaceSnapshotFile> >, public ActorCallback< GetRestoreSetActor, 2, std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> >, public ActorCallback< GetRestoreSetActor, 3, KeyRange >, public ActorCallback< GetRestoreSetActor, 4, Void >, public FastAllocated<GetRestoreSetActor>, public GetRestoreSetActorState<GetRestoreSetActor> {
															#line 5290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<GetRestoreSetActor>::operator new;
	using FastAllocated<GetRestoreSetActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<RestorableFileSet>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetRestoreSetActor, 0, Void >;
friend struct ActorCallback< GetRestoreSetActor, 1, std::vector<KeyspaceSnapshotFile> >;
friend struct ActorCallback< GetRestoreSetActor, 2, std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> >;
friend struct ActorCallback< GetRestoreSetActor, 3, KeyRange >;
friend struct ActorCallback< GetRestoreSetActor, 4, Void >;
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	GetRestoreSetActor(Reference<BackupContainerFileSystem> const& bc,Version const& targetVersion,VectorRef<KeyRangeRef> const& keyRangesFilter,bool const& logsOnly = false,Version const& beginVersion = invalidVersion) 
															#line 5305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Optional<RestorableFileSet>>(),
		   GetRestoreSetActorState<GetRestoreSetActor>(bc, targetVersion, keyRangesFilter, logsOnly, beginVersion)
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getRestoreSet");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetRestoreSetActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetRestoreSetActor, 1, std::vector<KeyspaceSnapshotFile> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GetRestoreSetActor, 2, std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< GetRestoreSetActor, 3, KeyRange >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< GetRestoreSetActor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Optional<RestorableFileSet>> getRestoreSet( Reference<BackupContainerFileSystem> const& bc, Version const& targetVersion, VectorRef<KeyRangeRef> const& keyRangesFilter, bool const& logsOnly = false, Version const& beginVersion = invalidVersion ) {
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Optional<RestorableFileSet>>(new GetRestoreSetActor(bc, targetVersion, keyRangesFilter, logsOnly, beginVersion));
															#line 5336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

	static std::string versionFolderString(Version v, int smallestBucket) {
		ASSERT_LT(smallestBucket, 14);
		// Get a 0-padded fixed size representation of v
		std::string vFixedPrecision = format("%019lld", v);
		ASSERT_EQ(vFixedPrecision.size(), 19);
		// Truncate smallestBucket from the fixed length representation
		vFixedPrecision.resize(vFixedPrecision.size() - smallestBucket);

		// Split the remaining digits with a '/' 4 places from the right
		vFixedPrecision.insert(vFixedPrecision.size() - 4, 1, '/');

		return vFixedPrecision;
	}

	// This useful for comparing version folder strings regardless of where their "/" dividers are, as it is possible
	// that division points would change in the future.
	static std::string cleanFolderString(std::string f) {
		f.erase(std::remove(f.begin(), f.end(), '/'), f.end());
		return f;
	}

	// The innermost folder covers 100 seconds (1e8 versions) During a full speed backup it is possible though very
	// unlikely write about 10,000 snapshot range files during that time.
	static std::string old_rangeVersionFolderString(Version v) {
		return format("ranges/%s/", versionFolderString(v, 8).c_str());
	}

	// Get the root folder for a snapshot's data based on its begin version
	static std::string snapshotFolderString(Version snapshotBeginVersion) {
		return format("kvranges/snapshot.%018" PRId64, snapshotBeginVersion);
	}

	// Extract the snapshot begin version from a path
	static Version extractSnapshotBeginVersion(const std::string& path) {
		Version snapshotBeginVersion;
		if (sscanf(path.c_str(), "kvranges/snapshot.%018" SCNd64, &snapshotBeginVersion) == 1) {
			return snapshotBeginVersion;
		}
		return invalidVersion;
	}

	// The innermost folder covers 100,000 seconds (1e11 versions) which is 5,000 mutation log files at current
	// settings.
	static std::string logVersionFolderString(Version v, bool partitioned) {
		return format("%s/%s/", (partitioned ? "plogs" : "logs"), versionFolderString(v, 11).c_str());
	}

	static bool pathToLogFile(LogFile& out, const std::string& path, int64_t size) {
		std::string name = fileNameOnly(path);
		LogFile f;
		f.fileName = path;
		f.fileSize = size;
		int len;
		if (sscanf(name.c_str(),
		           "log,%" SCNd64 ",%" SCNd64 ",%*[^,],%u%n",
		           &f.beginVersion,
		           &f.endVersion,
		           &f.blockSize,
		           &len) == 3 &&
		    len == name.size()) {
			out = f;
			return true;
		} else if (sscanf(name.c_str(),
		                  "log,%" SCNd64 ",%" SCNd64 ",%*[^,],%d-of-%d,%u%n",
		                  &f.beginVersion,
		                  &f.endVersion,
		                  &f.tagId,
		                  &f.totalTags,
		                  &f.blockSize,
		                  &len) == 5 &&
		           len == name.size() && f.tagId >= 0) {
			out = f;
			return true;
		}
		return false;
	}

	static bool pathToKeyspaceSnapshotFile(KeyspaceSnapshotFile& out, const std::string& path) {
		std::string name = fileNameOnly(path);
		KeyspaceSnapshotFile f;
		f.fileName = path;
		int len;
		if (sscanf(name.c_str(),
		           "snapshot,%" SCNd64 ",%" SCNd64 ",%" SCNd64 "%n",
		           &f.beginVersion,
		           &f.endVersion,
		           &f.totalSize,
		           &len) == 3 &&
		    len == name.size()) {
			out = f;
			return true;
		}
		return false;
	}

#if ENCRYPTION_ENABLED
																#line 5437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via createTestEncryptionKeyFile()
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class CreateTestEncryptionKeyFileActor>
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class CreateTestEncryptionKeyFileActorState {
															#line 5443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	CreateTestEncryptionKeyFileActorState(std::string const& filename) 
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : filename(filename)
															#line 5450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this));

	}
	~CreateTestEncryptionKeyFileActorState() 
	{
		fdb_probe_actor_destroy("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Reference<IAsyncFile>> __when_expr_0 = IAsyncFileSystem::filesystem()->open( filename, IAsyncFile::OPEN_ATOMIC_WRITE_AND_CREATE | IAsyncFile::OPEN_READWRITE | IAsyncFile::OPEN_CREATE, 0600);
															#line 1134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<CreateTestEncryptionKeyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CreateTestEncryptionKeyFileActor*>(this)->actor_wait_state = 1;
															#line 1134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CreateTestEncryptionKeyFileActor, 0, Reference<IAsyncFile> >*>(static_cast<CreateTestEncryptionKeyFileActor*>(this)));
															#line 5472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~CreateTestEncryptionKeyFileActorState();
		static_cast<CreateTestEncryptionKeyFileActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StreamCipherKey testKey(AES_256_KEY_LENGTH);
															#line 1139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		testKey.initializeRandomTestKey();
															#line 1140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		keyFile->write(testKey.data(), testKey.size(), 0);
															#line 1141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_1 = keyFile->sync();
															#line 1141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<CreateTestEncryptionKeyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CreateTestEncryptionKeyFileActor*>(this)->actor_wait_state = 2;
															#line 1141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CreateTestEncryptionKeyFileActor, 1, Void >*>(static_cast<CreateTestEncryptionKeyFileActor*>(this)));
															#line 5508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> const& __keyFile,int loopDepth) 
	{
															#line 1134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		keyFile = __keyFile;
															#line 5517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> && __keyFile,int loopDepth) 
	{
		keyFile = std::move(__keyFile);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CreateTestEncryptionKeyFileActor*>(this)->actor_wait_state > 0) static_cast<CreateTestEncryptionKeyFileActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTestEncryptionKeyFileActor*>(this)->ActorCallback< CreateTestEncryptionKeyFileActor, 0, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTestEncryptionKeyFileActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CreateTestEncryptionKeyFileActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CreateTestEncryptionKeyFileActor, 0, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 1142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<CreateTestEncryptionKeyFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CreateTestEncryptionKeyFileActorState(); static_cast<CreateTestEncryptionKeyFileActor*>(this)->destroy(); return 0; }
															#line 5584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<CreateTestEncryptionKeyFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CreateTestEncryptionKeyFileActorState();
		static_cast<CreateTestEncryptionKeyFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<CreateTestEncryptionKeyFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CreateTestEncryptionKeyFileActorState(); static_cast<CreateTestEncryptionKeyFileActor*>(this)->destroy(); return 0; }
															#line 5596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<CreateTestEncryptionKeyFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CreateTestEncryptionKeyFileActorState();
		static_cast<CreateTestEncryptionKeyFileActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<CreateTestEncryptionKeyFileActor*>(this)->actor_wait_state > 0) static_cast<CreateTestEncryptionKeyFileActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTestEncryptionKeyFileActor*>(this)->ActorCallback< CreateTestEncryptionKeyFileActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTestEncryptionKeyFileActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CreateTestEncryptionKeyFileActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CreateTestEncryptionKeyFileActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string filename;
															#line 1134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IAsyncFile> keyFile;
															#line 5671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via createTestEncryptionKeyFile()
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class CreateTestEncryptionKeyFileActor final : public Actor<Void>, public ActorCallback< CreateTestEncryptionKeyFileActor, 0, Reference<IAsyncFile> >, public ActorCallback< CreateTestEncryptionKeyFileActor, 1, Void >, public FastAllocated<CreateTestEncryptionKeyFileActor>, public CreateTestEncryptionKeyFileActorState<CreateTestEncryptionKeyFileActor> {
															#line 5676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<CreateTestEncryptionKeyFileActor>::operator new;
	using FastAllocated<CreateTestEncryptionKeyFileActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CreateTestEncryptionKeyFileActor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< CreateTestEncryptionKeyFileActor, 1, Void >;
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	CreateTestEncryptionKeyFileActor(std::string const& filename) 
															#line 5688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   CreateTestEncryptionKeyFileActorState<CreateTestEncryptionKeyFileActor>(filename)
	{
		fdb_probe_actor_enter("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("createTestEncryptionKeyFile");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("createTestEncryptionKeyFile", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CreateTestEncryptionKeyFileActor, 0, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CreateTestEncryptionKeyFileActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Void> createTestEncryptionKeyFile( std::string const& filename ) {
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new CreateTestEncryptionKeyFileActor(filename));
															#line 5716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

																#line 5721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via readEncryptionKey()
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class ReadEncryptionKeyActor>
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ReadEncryptionKeyActorState {
															#line 5727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ReadEncryptionKeyActorState(std::string const& encryptionKeyFileName) 
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : encryptionKeyFileName(encryptionKeyFileName),
															#line 1146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   keyFile(),
															#line 1147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   cipherKey(StreamCipherKey::getGlobalCipherKey())
															#line 5738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("readEncryptionKey", reinterpret_cast<unsigned long>(this));

	}
	~ReadEncryptionKeyActorState() 
	{
		fdb_probe_actor_destroy("readEncryptionKey", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Reference<IAsyncFile>> __when_expr_0 = IAsyncFileSystem::filesystem()->open(encryptionKeyFileName, 0x0, 0400);
															#line 1149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<ReadEncryptionKeyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 5756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ReadEncryptionKeyActor*>(this)->actor_wait_state = 1;
															#line 1149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadEncryptionKeyActor, 0, Reference<IAsyncFile> >*>(static_cast<ReadEncryptionKeyActor*>(this)));
															#line 5761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~ReadEncryptionKeyActorState();
		static_cast<ReadEncryptionKeyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<int> __when_expr_1 = keyFile->read(cipherKey->data(), cipherKey->size(), 0);
															#line 1158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadEncryptionKeyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadEncryptionKeyActor*>(this)->actor_wait_state = 2;
															#line 1158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadEncryptionKeyActor, 1, int >*>(static_cast<ReadEncryptionKeyActor*>(this)));
															#line 5797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarnAlways, "FailedToOpenEncryptionKeyFile") .error(e) .detail("FileName", encryptionKeyFileName);
															#line 1156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 5809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Reference<IAsyncFile> const& _keyFile,int loopDepth) 
	{
															#line 1151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		keyFile = _keyFile;
															#line 5823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Reference<IAsyncFile> && _keyFile,int loopDepth) 
	{
															#line 1151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		keyFile = _keyFile;
															#line 5832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> const& _keyFile,int loopDepth) 
	{
		loopDepth = a_body1cont2(_keyFile, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> && _keyFile,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(_keyFile), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadEncryptionKeyActor*>(this)->actor_wait_state > 0) static_cast<ReadEncryptionKeyActor*>(this)->actor_wait_state = 0;
		static_cast<ReadEncryptionKeyActor*>(this)->ActorCallback< ReadEncryptionKeyActor, 0, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< ReadEncryptionKeyActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("readEncryptionKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readEncryptionKey", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadEncryptionKeyActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("readEncryptionKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readEncryptionKey", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadEncryptionKeyActor, 0, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("readEncryptionKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readEncryptionKey", reinterpret_cast<unsigned long>(this), 0);

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
	int a_body1cont5(int const& bytesRead,int loopDepth) 
	{
															#line 1159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (bytesRead != cipherKey->size())
															#line 5917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarnAlways, "InvalidEncryptionKeyFileSize") .detail("ExpectedSize", cipherKey->size()) .detail("ActualSize", bytesRead);
															#line 1163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(invalid_encryption_key_file(), loopDepth);
															#line 5923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(bytesRead, cipherKey->size());
															#line 1166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<ReadEncryptionKeyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadEncryptionKeyActorState(); static_cast<ReadEncryptionKeyActor*>(this)->destroy(); return 0; }
															#line 5929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<ReadEncryptionKeyActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ReadEncryptionKeyActorState();
		static_cast<ReadEncryptionKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5(int && bytesRead,int loopDepth) 
	{
															#line 1159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (bytesRead != cipherKey->size())
															#line 5941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarnAlways, "InvalidEncryptionKeyFileSize") .detail("ExpectedSize", cipherKey->size()) .detail("ActualSize", bytesRead);
															#line 1163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(invalid_encryption_key_file(), loopDepth);
															#line 5947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(bytesRead, cipherKey->size());
															#line 1166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<ReadEncryptionKeyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadEncryptionKeyActorState(); static_cast<ReadEncryptionKeyActor*>(this)->destroy(); return 0; }
															#line 5953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<ReadEncryptionKeyActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ReadEncryptionKeyActorState();
		static_cast<ReadEncryptionKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(int const& bytesRead,int loopDepth) 
	{
		loopDepth = a_body1cont5(bytesRead, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(int && bytesRead,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(bytesRead), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReadEncryptionKeyActor*>(this)->actor_wait_state > 0) static_cast<ReadEncryptionKeyActor*>(this)->actor_wait_state = 0;
		static_cast<ReadEncryptionKeyActor*>(this)->ActorCallback< ReadEncryptionKeyActor, 1, int >::remove();

	}
	void a_callback_fire(ActorCallback< ReadEncryptionKeyActor, 1, int >*,int const& value) 
	{
		fdb_probe_actor_enter("readEncryptionKey", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readEncryptionKey", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadEncryptionKeyActor, 1, int >*,int && value) 
	{
		fdb_probe_actor_enter("readEncryptionKey", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readEncryptionKey", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadEncryptionKeyActor, 1, int >*,Error err) 
	{
		fdb_probe_actor_enter("readEncryptionKey", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readEncryptionKey", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string encryptionKeyFileName;
															#line 1146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IAsyncFile> keyFile;
															#line 1147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	StreamCipherKey const* cipherKey;
															#line 6030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via readEncryptionKey()
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ReadEncryptionKeyActor final : public Actor<Void>, public ActorCallback< ReadEncryptionKeyActor, 0, Reference<IAsyncFile> >, public ActorCallback< ReadEncryptionKeyActor, 1, int >, public FastAllocated<ReadEncryptionKeyActor>, public ReadEncryptionKeyActorState<ReadEncryptionKeyActor> {
															#line 6035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<ReadEncryptionKeyActor>::operator new;
	using FastAllocated<ReadEncryptionKeyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadEncryptionKeyActor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< ReadEncryptionKeyActor, 1, int >;
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ReadEncryptionKeyActor(std::string const& encryptionKeyFileName) 
															#line 6047 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   ReadEncryptionKeyActorState<ReadEncryptionKeyActor>(encryptionKeyFileName)
	{
		fdb_probe_actor_enter("readEncryptionKey", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readEncryptionKey");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readEncryptionKey", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadEncryptionKeyActor, 0, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReadEncryptionKeyActor, 1, int >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Void> readEncryptionKey( std::string const& encryptionKeyFileName ) {
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new ReadEncryptionKeyActor(encryptionKeyFileName));
															#line 6075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
#endif // ENCRYPTION_ENABLED

}; // class BackupContainerFileSystemImpl

Future<Reference<IBackupFile>> BackupContainerFileSystem::writeLogFile(Version beginVersion,
                                                                       Version endVersion,
                                                                       int blockSize) {
	return writeFile(BackupContainerFileSystemImpl::logVersionFolderString(beginVersion, false) +
	                 format("log,%lld,%lld,%s,%d",
	                        beginVersion,
	                        endVersion,
	                        deterministicRandom()->randomUniqueID().toString().c_str(),
	                        blockSize));
}

Future<Reference<IBackupFile>> BackupContainerFileSystem::writeTaggedLogFile(Version beginVersion,
                                                                             Version endVersion,
                                                                             int blockSize,
                                                                             uint16_t tagId,
                                                                             int totalTags) {
	return writeFile(BackupContainerFileSystemImpl::logVersionFolderString(beginVersion, true) +
	                 format("log,%lld,%lld,%s,%d-of-%d,%d",
	                        beginVersion,
	                        endVersion,
	                        deterministicRandom()->randomUniqueID().toString().c_str(),
	                        tagId,
	                        totalTags,
	                        blockSize));
}

Future<Reference<IBackupFile>> BackupContainerFileSystem::writeRangeFile(Version snapshotBeginVersion,
                                                                         int snapshotFileCount,
                                                                         Version fileVersion,
                                                                         int blockSize) {
	std::string fileName = format(
	    "range,%" PRId64 ",%s,%d", fileVersion, deterministicRandom()->randomUniqueID().toString().c_str(), blockSize);

	// In order to test backward compatibility in simulation, sometimes write to the old path format
	if (g_network->isSimulated() && deterministicRandom()->coinflip()) {
		return writeFile(BackupContainerFileSystemImpl::old_rangeVersionFolderString(fileVersion) + fileName);
	}

	return writeFile(BackupContainerFileSystemImpl::snapshotFolderString(snapshotBeginVersion) +
	                 format("/%d/", snapshotFileCount / (BUGGIFY ? 1 : 5000)) + fileName);
}

Future<std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>>
BackupContainerFileSystem::readKeyspaceSnapshot(KeyspaceSnapshotFile snapshot) {
	return BackupContainerFileSystemImpl::readKeyspaceSnapshot(Reference<BackupContainerFileSystem>::addRef(this),
	                                                           snapshot);
}

Future<Void> BackupContainerFileSystem::writeKeyspaceSnapshotFile(const std::vector<std::string>& fileNames,
                                                                  const std::vector<std::pair<Key, Key>>& beginEndKeys,
                                                                  int64_t totalBytes) {
	return BackupContainerFileSystemImpl::writeKeyspaceSnapshotFile(
	    Reference<BackupContainerFileSystem>::addRef(this), fileNames, beginEndKeys, totalBytes);
};

Future<std::vector<LogFile>> BackupContainerFileSystem::listLogFiles(Version beginVersion,
                                                                     Version targetVersion,
                                                                     bool partitioned) {
	// The first relevant log file could have a begin version less than beginVersion based on the knobs which
	// determine log file range size, so start at an earlier version adjusted by how many versions a file could
	// contain.
	//
	// Get the cleaned (without slashes) first and last folders that could contain relevant results.
	std::string firstPath =
	    BackupContainerFileSystemImpl::cleanFolderString(BackupContainerFileSystemImpl::logVersionFolderString(
	        std::max<Version>(0,
	                          beginVersion - CLIENT_KNOBS->BACKUP_MAX_LOG_RANGES * CLIENT_KNOBS->LOG_RANGE_BLOCK_SIZE),
	        partitioned));
	std::string lastPath = BackupContainerFileSystemImpl::cleanFolderString(
	    BackupContainerFileSystemImpl::logVersionFolderString(targetVersion, partitioned));

	std::function<bool(std::string const&)> pathFilter = [=](const std::string& folderPath) {
		// Remove slashes in the given folder path so that the '/' positions in the version folder string do not
		// matter

		std::string cleaned = BackupContainerFileSystemImpl::cleanFolderString(folderPath);
		return StringRef(firstPath).startsWith(cleaned) || StringRef(lastPath).startsWith(cleaned) ||
		       (cleaned > firstPath && cleaned < lastPath);
	};

	return map(listFiles((partitioned ? "plogs/" : "logs/"), pathFilter), [=](const FilesAndSizesT& files) {
		std::vector<LogFile> results;
		LogFile lf;
		for (auto& f : files) {
			if (BackupContainerFileSystemImpl::pathToLogFile(lf, f.first, f.second) && lf.endVersion > beginVersion &&
			    lf.beginVersion <= targetVersion)
				results.push_back(lf);
		}
		return results;
	});
}

Future<std::vector<RangeFile>> BackupContainerFileSystem::old_listRangeFiles(Version beginVersion, Version endVersion) {
	// Get the cleaned (without slashes) first and last folders that could contain relevant results.
	std::string firstPath = BackupContainerFileSystemImpl::cleanFolderString(
	    BackupContainerFileSystemImpl::old_rangeVersionFolderString(beginVersion));
	std::string lastPath = BackupContainerFileSystemImpl::cleanFolderString(
	    BackupContainerFileSystemImpl::old_rangeVersionFolderString(endVersion));

	std::function<bool(std::string const&)> pathFilter = [=](const std::string& folderPath) {
		// Remove slashes in the given folder path so that the '/' positions in the version folder string do not
		// matter
		std::string cleaned = BackupContainerFileSystemImpl::cleanFolderString(folderPath);

		return StringRef(firstPath).startsWith(cleaned) || StringRef(lastPath).startsWith(cleaned) ||
		       (cleaned > firstPath && cleaned < lastPath);
	};

	return map(listFiles("ranges/", pathFilter), [=](const FilesAndSizesT& files) {
		std::vector<RangeFile> results;
		RangeFile rf;
		for (auto& f : files) {
			if (BackupContainerFileSystemImpl::pathToRangeFile(rf, f.first, f.second) && rf.version >= beginVersion &&
			    rf.version <= endVersion)
				results.push_back(rf);
		}
		return results;
	});
}

Future<std::vector<RangeFile>> BackupContainerFileSystem::listRangeFiles(Version beginVersion, Version endVersion) {
	// Until the old folder scheme is no longer supported, read files stored using old folder scheme
	Future<std::vector<RangeFile>> oldFiles = old_listRangeFiles(beginVersion, endVersion);

	// Define filter function (for listFiles() implementations that use it) to reject any folder
	// starting after endVersion
	std::function<bool(std::string const&)> pathFilter = [=](std::string const& path) {
		return BackupContainerFileSystemImpl::extractSnapshotBeginVersion(path) <= endVersion;
	};

	Future<std::vector<RangeFile>> newFiles = map(listFiles("kvranges/", pathFilter), [=](const FilesAndSizesT& files) {
		std::vector<RangeFile> results;
		RangeFile rf;
		for (auto& f : files) {
			if (BackupContainerFileSystemImpl::pathToRangeFile(rf, f.first, f.second) && rf.version >= beginVersion &&
			    rf.version <= endVersion)
				results.push_back(rf);
		}
		return results;
	});

	return map(success(oldFiles) && success(newFiles), [=](Void _) {
		std::vector<RangeFile> results = std::move(newFiles.get());
		std::vector<RangeFile> oldResults = std::move(oldFiles.get());
		results.insert(
		    results.end(), std::make_move_iterator(oldResults.begin()), std::make_move_iterator(oldResults.end()));
		return results;
	});
}

Future<std::vector<KeyspaceSnapshotFile>> BackupContainerFileSystem::listKeyspaceSnapshots(Version begin, Version end) {
	return map(listFiles("snapshots/"), [=](const FilesAndSizesT& files) {
		std::vector<KeyspaceSnapshotFile> results;
		KeyspaceSnapshotFile sf;
		for (auto& f : files) {
			if (BackupContainerFileSystemImpl::pathToKeyspaceSnapshotFile(sf, f.first) && sf.beginVersion < end &&
			    sf.endVersion >= begin)
				results.push_back(sf);
		}
		std::sort(results.begin(), results.end());
		return results;
	});
}

Future<BackupFileList> BackupContainerFileSystem::dumpFileList(Version begin, Version end) {
	return BackupContainerFileSystemImpl::dumpFileList(Reference<BackupContainerFileSystem>::addRef(this), begin, end);
}

Future<BackupDescription> BackupContainerFileSystem::describeBackup(bool deepScan, Version logStartVersionOverride) {
	return BackupContainerFileSystemImpl::describeBackup(
	    Reference<BackupContainerFileSystem>::addRef(this), deepScan, logStartVersionOverride);
}

Future<Void> BackupContainerFileSystem::expireData(Version expireEndVersion,
                                                   bool force,
                                                   ExpireProgress* progress,
                                                   Version restorableBeginVersion) {
	return BackupContainerFileSystemImpl::expireData(
	    Reference<BackupContainerFileSystem>::addRef(this), expireEndVersion, force, progress, restorableBeginVersion);
}

															#line 6264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
namespace {
// This generated class is to be used only via getSnapshotFileKeyRange_impl()
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class GetSnapshotFileKeyRange_implActor>
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class GetSnapshotFileKeyRange_implActorState {
															#line 6271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	GetSnapshotFileKeyRange_implActorState(Reference<BackupContainerFileSystem> const& bc,RangeFile const& file) 
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   file(file),
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   readFileRetries(0),
															#line 1355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   beginKeySet(false),
															#line 1356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   beginKey(),
															#line 1357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   endKey()
															#line 6288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this));

	}
	~GetSnapshotFileKeyRange_implActorState() 
	{
		fdb_probe_actor_destroy("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			;
															#line 6303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~GetSnapshotFileKeyRange_implActorState();
		static_cast<GetSnapshotFileKeyRange_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<GetSnapshotFileKeyRange_implActor*>(this)->SAV<KeyRange>::futures) { (void)(KeyRange(KeyRangeRef(beginKey, endKey))); this->~GetSnapshotFileKeyRange_implActorState(); static_cast<GetSnapshotFileKeyRange_implActor*>(this)->destroy(); return 0; }
															#line 6326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<GetSnapshotFileKeyRange_implActor*>(this)->SAV< KeyRange >::value()) KeyRange(KeyRange(KeyRangeRef(beginKey, endKey)));
		this->~GetSnapshotFileKeyRange_implActorState();
		static_cast<GetSnapshotFileKeyRange_implActor*>(this)->finishSendAndDelPromiseRef();
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
		try {
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Reference<IAsyncFile>> __when_expr_0 = bc->readFile(file.fileName);
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 6348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state = 1;
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetSnapshotFileKeyRange_implActor, 0, Reference<IAsyncFile> >*>(static_cast<GetSnapshotFileKeyRange_implActor*>(this)));
															#line 6353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
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
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (e.code() == error_code_restore_bad_read || e.code() == error_code_restore_unsupported_file_version || e.code() == error_code_restore_corrupted_data_padding)
															#line 6388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent(SevError, "BackupContainerGetSnapshotFileKeyRange").error(e);
															#line 1377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 6394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
			else
			{
															#line 1378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (e.code() == error_code_http_request_failed || e.code() == error_code_connection_failed || e.code() == error_code_timed_out || e.code() == error_code_lookup_failed)
															#line 6400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 1381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					TraceEvent(SevWarnAlways, "BackupContainerGetSnapshotFileKeyRangeConnectionFailure") .error(e) .detail("Retries", ++readFileRetries);
															#line 1384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					StrictFuture<Void> __when_expr_2 = delayJittered(0.1);
															#line 1384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					if (static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
					if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
					static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state = 3;
															#line 1384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetSnapshotFileKeyRange_implActor, 2, Void >*>(static_cast<GetSnapshotFileKeyRange_implActor*>(this)));
															#line 6413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
					loopDepth = 0;
				}
				else
				{
															#line 1386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					TraceEvent(SevError, "BackupContainerGetSnapshotFileKeyRangeUnexpectedError").error(e);
															#line 1387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 6422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
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
															#line 1361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		beginKeySet = false;
															#line 1362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		j = 0;
															#line 1363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 6442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Reference<IAsyncFile> const& __inFile,int loopDepth) 
	{
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		inFile = __inFile;
															#line 6451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Reference<IAsyncFile> && __inFile,int loopDepth) 
	{
		inFile = std::move(__inFile);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state > 0) static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state = 0;
		static_cast<GetSnapshotFileKeyRange_implActor*>(this)->ActorCallback< GetSnapshotFileKeyRange_implActor, 0, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< GetSnapshotFileKeyRange_implActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetSnapshotFileKeyRange_implActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetSnapshotFileKeyRange_implActor, 0, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

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
															#line 1363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(j < file.fileSize))
															#line 6531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1loopBody1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int64_t len = std::min<int64_t>(file.blockSize, file.fileSize - j);
															#line 1365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Standalone<VectorRef<KeyValueRef>>> __when_expr_1 = fileBackup::decodeRangeFileBlock(inFile, j, len);
															#line 1365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state = 2;
															#line 1365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetSnapshotFileKeyRange_implActor, 1, Standalone<VectorRef<KeyValueRef>> >*>(static_cast<GetSnapshotFileKeyRange_implActor*>(this)));
															#line 6546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2break1(int loopDepth) 
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
	int a_body1loopBody1cont2loopBody1cont1(Standalone<VectorRef<KeyValueRef>> const& blockData,int loopDepth) 
	{
															#line 1366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!beginKeySet)
															#line 6568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			beginKey = blockData.front().key;
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			beginKeySet = true;
															#line 6574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		endKey = blockData.back().key;
															#line 1363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		j += file.blockSize;
															#line 6580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont1(Standalone<VectorRef<KeyValueRef>> && blockData,int loopDepth) 
	{
															#line 1366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!beginKeySet)
															#line 6589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			beginKey = blockData.front().key;
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			beginKeySet = true;
															#line 6595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		endKey = blockData.back().key;
															#line 1363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		j += file.blockSize;
															#line 6601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1when1(Standalone<VectorRef<KeyValueRef>> const& blockData,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont1(blockData, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1when1(Standalone<VectorRef<KeyValueRef>> && blockData,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont1(std::move(blockData), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state > 0) static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state = 0;
		static_cast<GetSnapshotFileKeyRange_implActor*>(this)->ActorCallback< GetSnapshotFileKeyRange_implActor, 1, Standalone<VectorRef<KeyValueRef>> >::remove();

	}
	void a_callback_fire(ActorCallback< GetSnapshotFileKeyRange_implActor, 1, Standalone<VectorRef<KeyValueRef>> >*,Standalone<VectorRef<KeyValueRef>> const& value) 
	{
		fdb_probe_actor_enter("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetSnapshotFileKeyRange_implActor, 1, Standalone<VectorRef<KeyValueRef>> >*,Standalone<VectorRef<KeyValueRef>> && value) 
	{
		fdb_probe_actor_enter("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetSnapshotFileKeyRange_implActor, 1, Standalone<VectorRef<KeyValueRef>> >*,Error err) 
	{
		fdb_probe_actor_enter("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1Catch1cont1(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont3(int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state > 0) static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state = 0;
		static_cast<GetSnapshotFileKeyRange_implActor*>(this)->ActorCallback< GetSnapshotFileKeyRange_implActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetSnapshotFileKeyRange_implActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetSnapshotFileKeyRange_implActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetSnapshotFileKeyRange_implActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	RangeFile file;
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int readFileRetries;
															#line 1355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	bool beginKeySet;
															#line 1356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Key beginKey;
															#line 1357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Key endKey;
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IAsyncFile> inFile;
															#line 1362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int64_t j;
															#line 6772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via getSnapshotFileKeyRange_impl()
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class GetSnapshotFileKeyRange_implActor final : public Actor<KeyRange>, public ActorCallback< GetSnapshotFileKeyRange_implActor, 0, Reference<IAsyncFile> >, public ActorCallback< GetSnapshotFileKeyRange_implActor, 1, Standalone<VectorRef<KeyValueRef>> >, public ActorCallback< GetSnapshotFileKeyRange_implActor, 2, Void >, public FastAllocated<GetSnapshotFileKeyRange_implActor>, public GetSnapshotFileKeyRange_implActorState<GetSnapshotFileKeyRange_implActor> {
															#line 6777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<GetSnapshotFileKeyRange_implActor>::operator new;
	using FastAllocated<GetSnapshotFileKeyRange_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<KeyRange>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetSnapshotFileKeyRange_implActor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< GetSnapshotFileKeyRange_implActor, 1, Standalone<VectorRef<KeyValueRef>> >;
friend struct ActorCallback< GetSnapshotFileKeyRange_implActor, 2, Void >;
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	GetSnapshotFileKeyRange_implActor(Reference<BackupContainerFileSystem> const& bc,RangeFile const& file) 
															#line 6790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<KeyRange>(),
		   GetSnapshotFileKeyRange_implActorState<GetSnapshotFileKeyRange_implActor>(bc, file)
	{
		fdb_probe_actor_enter("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getSnapshotFileKeyRange_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getSnapshotFileKeyRange_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetSnapshotFileKeyRange_implActor, 0, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetSnapshotFileKeyRange_implActor, 1, Standalone<VectorRef<KeyValueRef>> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GetSnapshotFileKeyRange_implActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<KeyRange> getSnapshotFileKeyRange_impl( Reference<BackupContainerFileSystem> const& bc, RangeFile const& file ) {
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<KeyRange>(new GetSnapshotFileKeyRange_implActor(bc, file));
															#line 6820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 6825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
namespace {
// This generated class is to be used only via writeVersionProperty()
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class WriteVersionPropertyActor>
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class WriteVersionPropertyActorState {
															#line 6832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	WriteVersionPropertyActorState(Reference<BackupContainerFileSystem> const& bc,std::string const& path,Version const& v) 
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   path(path),
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   v(v)
															#line 6843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("writeVersionProperty", reinterpret_cast<unsigned long>(this));

	}
	~WriteVersionPropertyActorState() 
	{
		fdb_probe_actor_destroy("writeVersionProperty", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Reference<IBackupFile>> __when_expr_0 = bc->writeFile(path);
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 6861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state = 1;
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WriteVersionPropertyActor, 0, Reference<IBackupFile> >*>(static_cast<WriteVersionPropertyActor*>(this)));
															#line 6866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~WriteVersionPropertyActorState();
		static_cast<WriteVersionPropertyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarn, "BackupContainerWritePropertyFailed") .error(e) .detail("URL", bc->getURL()) .detail("Path", path);
															#line 1406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 6898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::string s = format("%lld", v);
															#line 1398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_1 = f->append(s.data(), s.size());
															#line 1398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 6916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state = 2;
															#line 1398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WriteVersionPropertyActor, 1, Void >*>(static_cast<WriteVersionPropertyActor*>(this)));
															#line 6921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IBackupFile> const& __f,int loopDepth) 
	{
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		f = __f;
															#line 6930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IBackupFile> && __f,int loopDepth) 
	{
		f = std::move(__f);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state > 0) static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state = 0;
		static_cast<WriteVersionPropertyActor*>(this)->ActorCallback< WriteVersionPropertyActor, 0, Reference<IBackupFile> >::remove();

	}
	void a_callback_fire(ActorCallback< WriteVersionPropertyActor, 0, Reference<IBackupFile> >*,Reference<IBackupFile> const& value) 
	{
		fdb_probe_actor_enter("writeVersionProperty", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeVersionProperty", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WriteVersionPropertyActor, 0, Reference<IBackupFile> >*,Reference<IBackupFile> && value) 
	{
		fdb_probe_actor_enter("writeVersionProperty", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeVersionProperty", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WriteVersionPropertyActor, 0, Reference<IBackupFile> >*,Error err) 
	{
		fdb_probe_actor_enter("writeVersionProperty", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeVersionProperty", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 1399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->finish();
															#line 1399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 6999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state = 3;
															#line 1399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteVersionPropertyActor, 2, Void >*>(static_cast<WriteVersionPropertyActor*>(this)));
															#line 7004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 1399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->finish();
															#line 1399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 7015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state = 3;
															#line 1399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteVersionPropertyActor, 2, Void >*>(static_cast<WriteVersionPropertyActor*>(this)));
															#line 7020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
	void a_exitChoose2() 
	{
		if (static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state > 0) static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state = 0;
		static_cast<WriteVersionPropertyActor*>(this)->ActorCallback< WriteVersionPropertyActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteVersionPropertyActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeVersionProperty", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeVersionProperty", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WriteVersionPropertyActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeVersionProperty", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeVersionProperty", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WriteVersionPropertyActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeVersionProperty", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeVersionProperty", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 1400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<WriteVersionPropertyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteVersionPropertyActorState(); static_cast<WriteVersionPropertyActor*>(this)->destroy(); return 0; }
															#line 7092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<WriteVersionPropertyActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteVersionPropertyActorState();
		static_cast<WriteVersionPropertyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 1400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<WriteVersionPropertyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteVersionPropertyActorState(); static_cast<WriteVersionPropertyActor*>(this)->destroy(); return 0; }
															#line 7104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<WriteVersionPropertyActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteVersionPropertyActorState();
		static_cast<WriteVersionPropertyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state > 0) static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state = 0;
		static_cast<WriteVersionPropertyActor*>(this)->ActorCallback< WriteVersionPropertyActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteVersionPropertyActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeVersionProperty", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeVersionProperty", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< WriteVersionPropertyActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeVersionProperty", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeVersionProperty", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< WriteVersionPropertyActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeVersionProperty", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeVersionProperty", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string path;
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version v;
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IBackupFile> f;
															#line 7183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via writeVersionProperty()
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class WriteVersionPropertyActor final : public Actor<Void>, public ActorCallback< WriteVersionPropertyActor, 0, Reference<IBackupFile> >, public ActorCallback< WriteVersionPropertyActor, 1, Void >, public ActorCallback< WriteVersionPropertyActor, 2, Void >, public FastAllocated<WriteVersionPropertyActor>, public WriteVersionPropertyActorState<WriteVersionPropertyActor> {
															#line 7188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<WriteVersionPropertyActor>::operator new;
	using FastAllocated<WriteVersionPropertyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WriteVersionPropertyActor, 0, Reference<IBackupFile> >;
friend struct ActorCallback< WriteVersionPropertyActor, 1, Void >;
friend struct ActorCallback< WriteVersionPropertyActor, 2, Void >;
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	WriteVersionPropertyActor(Reference<BackupContainerFileSystem> const& bc,std::string const& path,Version const& v) 
															#line 7201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   WriteVersionPropertyActorState<WriteVersionPropertyActor>(bc, path, v)
	{
		fdb_probe_actor_enter("writeVersionProperty", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("writeVersionProperty");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("writeVersionProperty", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WriteVersionPropertyActor, 0, Reference<IBackupFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WriteVersionPropertyActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< WriteVersionPropertyActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Void> writeVersionProperty( Reference<BackupContainerFileSystem> const& bc, std::string const& path, Version const& v ) {
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new WriteVersionPropertyActor(bc, path, v));
															#line 7231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 7236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
namespace {
// This generated class is to be used only via readVersionProperty()
															#line 1410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class ReadVersionPropertyActor>
															#line 1410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ReadVersionPropertyActorState {
															#line 7243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ReadVersionPropertyActorState(Reference<BackupContainerFileSystem> const& bc,std::string const& path) 
															#line 1410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 1410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   path(path)
															#line 7252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("readVersionProperty", reinterpret_cast<unsigned long>(this));

	}
	~ReadVersionPropertyActorState() 
	{
		fdb_probe_actor_destroy("readVersionProperty", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Reference<IAsyncFile>> __when_expr_0 = bc->readFile(path);
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 7270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state = 1;
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadVersionPropertyActor, 0, Reference<IAsyncFile> >*>(static_cast<ReadVersionPropertyActor*>(this)));
															#line 7275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~ReadVersionPropertyActorState();
		static_cast<ReadVersionPropertyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (e.code() == error_code_file_not_found)
															#line 7305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!static_cast<ReadVersionPropertyActor*>(this)->SAV<Optional<Version>>::futures) { (void)(Optional<Version>()); this->~ReadVersionPropertyActorState(); static_cast<ReadVersionPropertyActor*>(this)->destroy(); return 0; }
															#line 7309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				new (&static_cast<ReadVersionPropertyActor*>(this)->SAV< Optional<Version> >::value()) Optional<Version>(Optional<Version>());
				this->~ReadVersionPropertyActorState();
				static_cast<ReadVersionPropertyActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarn, "BackupContainerReadPropertyFailed") .error(e) .detail("URL", bc->getURL()) .detail("Path", path);
															#line 1434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 7319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<int64_t> __when_expr_1 = f->size();
															#line 1413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 7335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state = 2;
															#line 1413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadVersionPropertyActor, 1, int64_t >*>(static_cast<ReadVersionPropertyActor*>(this)));
															#line 7340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> const& __f,int loopDepth) 
	{
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		f = __f;
															#line 7349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> && __f,int loopDepth) 
	{
		f = std::move(__f);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state > 0) static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state = 0;
		static_cast<ReadVersionPropertyActor*>(this)->ActorCallback< ReadVersionPropertyActor, 0, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< ReadVersionPropertyActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("readVersionProperty", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readVersionProperty", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadVersionPropertyActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("readVersionProperty", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readVersionProperty", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadVersionPropertyActor, 0, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("readVersionProperty", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readVersionProperty", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 1414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		s = std::string();
															#line 1415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		s.resize(size);
															#line 1416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<int> __when_expr_2 = f->read((uint8_t*)s.data(), size, 0);
															#line 1416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 7422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state = 3;
															#line 1416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReadVersionPropertyActor, 2, int >*>(static_cast<ReadVersionPropertyActor*>(this)));
															#line 7427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(int64_t const& __size,int loopDepth) 
	{
															#line 1413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		size = __size;
															#line 7436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(int64_t && __size,int loopDepth) 
	{
		size = std::move(__size);
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state > 0) static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state = 0;
		static_cast<ReadVersionPropertyActor*>(this)->ActorCallback< ReadVersionPropertyActor, 1, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< ReadVersionPropertyActor, 1, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("readVersionProperty", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readVersionProperty", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadVersionPropertyActor, 1, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("readVersionProperty", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readVersionProperty", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadVersionPropertyActor, 1, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("readVersionProperty", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readVersionProperty", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(int const& rs,int loopDepth) 
	{
															#line 1417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		Version v;
															#line 1418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int len;
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (rs == size && sscanf(s.c_str(), "%" SCNd64 "%n", &v, &len) == 1 && len == size)
															#line 7507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<ReadVersionPropertyActor*>(this)->SAV<Optional<Version>>::futures) { (void)(v); this->~ReadVersionPropertyActorState(); static_cast<ReadVersionPropertyActor*>(this)->destroy(); return 0; }
															#line 7511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<ReadVersionPropertyActor*>(this)->SAV< Optional<Version> >::value()) Optional<Version>(v);
			this->~ReadVersionPropertyActorState();
			static_cast<ReadVersionPropertyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent(SevWarn, "BackupContainerInvalidProperty").detail("URL", bc->getURL()).detail("Path", path);
															#line 1424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		return a_body1Catch2(backup_invalid_info(), loopDepth);
															#line 7521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont4(int && rs,int loopDepth) 
	{
															#line 1417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		Version v;
															#line 1418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int len;
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (rs == size && sscanf(s.c_str(), "%" SCNd64 "%n", &v, &len) == 1 && len == size)
															#line 7533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<ReadVersionPropertyActor*>(this)->SAV<Optional<Version>>::futures) { (void)(v); this->~ReadVersionPropertyActorState(); static_cast<ReadVersionPropertyActor*>(this)->destroy(); return 0; }
															#line 7537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<ReadVersionPropertyActor*>(this)->SAV< Optional<Version> >::value()) Optional<Version>(v);
			this->~ReadVersionPropertyActorState();
			static_cast<ReadVersionPropertyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent(SevWarn, "BackupContainerInvalidProperty").detail("URL", bc->getURL()).detail("Path", path);
															#line 1424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		return a_body1Catch2(backup_invalid_info(), loopDepth);
															#line 7547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont3when1(int const& rs,int loopDepth) 
	{
		loopDepth = a_body1cont4(rs, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(int && rs,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(rs), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state > 0) static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state = 0;
		static_cast<ReadVersionPropertyActor*>(this)->ActorCallback< ReadVersionPropertyActor, 2, int >::remove();

	}
	void a_callback_fire(ActorCallback< ReadVersionPropertyActor, 2, int >*,int const& value) 
	{
		fdb_probe_actor_enter("readVersionProperty", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readVersionProperty", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ReadVersionPropertyActor, 2, int >*,int && value) 
	{
		fdb_probe_actor_enter("readVersionProperty", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readVersionProperty", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ReadVersionPropertyActor, 2, int >*,Error err) 
	{
		fdb_probe_actor_enter("readVersionProperty", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readVersionProperty", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 1410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 1410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string path;
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IAsyncFile> f;
															#line 1413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int64_t size;
															#line 1414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string s;
															#line 7624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via readVersionProperty()
															#line 1410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ReadVersionPropertyActor final : public Actor<Optional<Version>>, public ActorCallback< ReadVersionPropertyActor, 0, Reference<IAsyncFile> >, public ActorCallback< ReadVersionPropertyActor, 1, int64_t >, public ActorCallback< ReadVersionPropertyActor, 2, int >, public FastAllocated<ReadVersionPropertyActor>, public ReadVersionPropertyActorState<ReadVersionPropertyActor> {
															#line 7629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<ReadVersionPropertyActor>::operator new;
	using FastAllocated<ReadVersionPropertyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<Version>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadVersionPropertyActor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< ReadVersionPropertyActor, 1, int64_t >;
friend struct ActorCallback< ReadVersionPropertyActor, 2, int >;
															#line 1410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ReadVersionPropertyActor(Reference<BackupContainerFileSystem> const& bc,std::string const& path) 
															#line 7642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Optional<Version>>(),
		   ReadVersionPropertyActorState<ReadVersionPropertyActor>(bc, path)
	{
		fdb_probe_actor_enter("readVersionProperty", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readVersionProperty");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readVersionProperty", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadVersionPropertyActor, 0, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReadVersionPropertyActor, 1, int64_t >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ReadVersionPropertyActor, 2, int >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Optional<Version>> readVersionProperty( Reference<BackupContainerFileSystem> const& bc, std::string const& path ) {
															#line 1410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Optional<Version>>(new ReadVersionPropertyActor(bc, path));
															#line 7672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

Future<KeyRange> BackupContainerFileSystem::getSnapshotFileKeyRange(const RangeFile& file) {
	ASSERT(g_network->isSimulated());
	return getSnapshotFileKeyRange_impl(Reference<BackupContainerFileSystem>::addRef(this), file);
}

Future<Optional<RestorableFileSet>> BackupContainerFileSystem::getRestoreSet(Version targetVersion,
                                                                             VectorRef<KeyRangeRef> keyRangesFilter,
                                                                             bool logsOnly,
                                                                             Version beginVersion) {
	return BackupContainerFileSystemImpl::getRestoreSet(
	    Reference<BackupContainerFileSystem>::addRef(this), targetVersion, keyRangesFilter, logsOnly, beginVersion);
}

Future<Optional<Version>> BackupContainerFileSystem::VersionProperty::get() {
	return readVersionProperty(bc, path);
}
Future<Void> BackupContainerFileSystem::VersionProperty::set(Version v) {
	return writeVersionProperty(bc, path, v);
}
Future<Void> BackupContainerFileSystem::VersionProperty::clear() {
	return bc->deleteFile(path);
}

BackupContainerFileSystem::VersionProperty BackupContainerFileSystem::logBeginVersion() {
	return { Reference<BackupContainerFileSystem>::addRef(this), "log_begin_version" };
}
BackupContainerFileSystem::VersionProperty BackupContainerFileSystem::logEndVersion() {
	return { Reference<BackupContainerFileSystem>::addRef(this), "log_end_version" };
}
BackupContainerFileSystem::VersionProperty BackupContainerFileSystem::expiredEndVersion() {
	return { Reference<BackupContainerFileSystem>::addRef(this), "expired_end_version" };
}
BackupContainerFileSystem::VersionProperty BackupContainerFileSystem::unreliableEndVersion() {
	return { Reference<BackupContainerFileSystem>::addRef(this), "unreliable_end_version" };
}
BackupContainerFileSystem::VersionProperty BackupContainerFileSystem::logType() {
	return { Reference<BackupContainerFileSystem>::addRef(this), "mutation_log_type" };
}
bool BackupContainerFileSystem::usesEncryption() const {
	return encryptionSetupFuture.isValid();
}
Future<Void> BackupContainerFileSystem::encryptionSetupComplete() const {
	return encryptionSetupFuture;
}

void BackupContainerFileSystem::setEncryptionKey(Optional<std::string> const& encryptionKeyFileName) {
	if (encryptionKeyFileName.present()) {
#if ENCRYPTION_ENABLED
		encryptionSetupFuture = BackupContainerFileSystemImpl::readEncryptionKey(encryptionKeyFileName.get());
#else
		encryptionSetupFuture = Void();
#endif
	}
}
Future<Void> BackupContainerFileSystem::createTestEncryptionKeyFile(std::string const& filename) {
#if ENCRYPTION_ENABLED
	return BackupContainerFileSystemImpl::createTestEncryptionKeyFile(filename);
#else
	return Void();
#endif
}

// Get a BackupContainerFileSystem based on a container URL string
// TODO: refactor to not duplicate IBackupContainer::openContainer. It's the exact same
// code but returning a different template type because you can't cast between them
Reference<BackupContainerFileSystem> BackupContainerFileSystem::openContainerFS(
    const std::string& url,
    const Optional<std::string>& proxy,
    const Optional<std::string>& encryptionKeyFileName) {
	static std::map<std::string, Reference<BackupContainerFileSystem>> m_cache;

	Reference<BackupContainerFileSystem>& r = m_cache[url];
	if (r)
		return r;

	try {
		StringRef u(url);
		if (u.startsWith("file://"_sr)) {
			r = makeReference<BackupContainerLocalDirectory>(url, encryptionKeyFileName);
		} else if (u.startsWith("blobstore://"_sr)) {
			std::string resource;
			Optional<std::string> blobstoreProxy;

			// If no proxy is passed down to the openContainer method, try to fallback to the
			// fileBackupAgentProxy which is a global variable and will be set for the backup_agent.
			if (proxy.present()) {
				blobstoreProxy = proxy.get();
			} else if (fileBackupAgentProxy.present()) {
				blobstoreProxy = fileBackupAgentProxy.get();
			}

			// The URL parameters contain blobstore endpoint tunables as well as possible backup-specific options.
			S3BlobStoreEndpoint::ParametersT backupParams;
			Reference<S3BlobStoreEndpoint> bstore =
			    S3BlobStoreEndpoint::fromString(url, blobstoreProxy, &resource, &lastOpenError, &backupParams);

			if (resource.empty())
				throw backup_invalid_url();
			for (auto c : resource)
				if (!isalnum(c) && c != '_' && c != '-' && c != '.' && c != '/')
					throw backup_invalid_url();
			r = makeReference<BackupContainerS3BlobStore>(bstore, resource, backupParams, encryptionKeyFileName);
		}
#ifdef BUILD_AZURE_BACKUP
		else if (u.startsWith("azure://"_sr)) {
			u.eat("azure://"_sr);
			auto accountName = u.eat("@"_sr).toString();
			auto endpoint = u.eat("/"_sr).toString();
			auto containerName = u.eat("/"_sr).toString();
			r = makeReference<BackupContainerAzureBlobStore>(
			    endpoint, accountName, containerName, encryptionKeyFileName);
		}
#endif
		else {
			lastOpenError = "invalid URL prefix";
			throw backup_invalid_url();
		}

		r->encryptionKeyFileName = encryptionKeyFileName;
		r->URL = url;
		return r;
	} catch (Error& e) {
		if (e.code() == error_code_actor_cancelled)
			throw;

		TraceEvent m(SevWarn, "BackupContainer");
		m.error(e);
		m.detail("Description", "Invalid container specification.  See help.");
		m.detail("URL", url);
		if (e.code() == error_code_backup_invalid_url)
			m.detail("LastOpenError", lastOpenError);

		throw;
	}
}

namespace backup_test {

int chooseFileSize(std::vector<int>& sizes) {
	if (!sizes.empty()) {
		int size = sizes.back();
		sizes.pop_back();
		return size;
	}
	return deterministicRandom()->randomInt(0, 2e6);
}

															#line 7824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via writeAndVerifyFile()
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class WriteAndVerifyFileActor>
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class WriteAndVerifyFileActorState {
															#line 7830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	WriteAndVerifyFileActorState(Reference<IBackupContainer> const& c,Reference<IBackupFile> const& f,int const& size,FlowLock* const& lock) 
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : c(c),
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   f(f),
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   size(size),
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   lock(lock),
															#line 1589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   content()
															#line 7845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("writeAndVerifyFile", reinterpret_cast<unsigned long>(this));

	}
	~WriteAndVerifyFileActorState() 
	{
		fdb_probe_actor_destroy("writeAndVerifyFile", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_0 = lock->take(TaskPriority::DefaultYield, size);
															#line 1591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 1;
															#line 1591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 0, Void >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 7867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~WriteAndVerifyFileActorState();
		static_cast<WriteAndVerifyFileActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		releaser = FlowLock::Releaser(*lock, size);
															#line 1594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("writeAndVerify size=%d file=%s\n", size, f->getFileName().c_str());
															#line 1595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		content.resize(content.arena(), size);
															#line 1596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for(int i = 0;i < content.size();++i) {
															#line 1597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			content[i] = (uint8_t)deterministicRandom()->randomInt(0, 256);
															#line 7898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		sendBuf = content;
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 7904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		releaser = FlowLock::Releaser(*lock, size);
															#line 1594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("writeAndVerify size=%d file=%s\n", size, f->getFileName().c_str());
															#line 1595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		content.resize(content.arena(), size);
															#line 1596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for(int i = 0;i < content.size();++i) {
															#line 1597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			content[i] = (uint8_t)deterministicRandom()->randomInt(0, 256);
															#line 7921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		sendBuf = content;
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 7927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

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
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state > 0) static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 0;
		static_cast<WriteAndVerifyFileActor*>(this)->ActorCallback< WriteAndVerifyFileActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteAndVerifyFileActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WriteAndVerifyFileActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WriteAndVerifyFileActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->finish();
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 3;
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 2, Void >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 8006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(sendBuf.size() > 0))
															#line 8022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		n = std::min(sendBuf.size(), deterministicRandom()->randomInt(1, 16384));
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_1 = f->append(sendBuf.begin(), n);
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 8032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 2;
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 1, Void >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 8037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1break1(int loopDepth) 
	{
		try {
			return a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 1604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		sendBuf.pop_front(n);
															#line 8059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		sendBuf.pop_front(n);
															#line 8068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state > 0) static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 0;
		static_cast<WriteAndVerifyFileActor*>(this)->ActorCallback< WriteAndVerifyFileActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteAndVerifyFileActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WriteAndVerifyFileActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WriteAndVerifyFileActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 1608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IAsyncFile>> __when_expr_3 = c->readFile(f->getFileName());
															#line 1608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 4;
															#line 1608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 3, Reference<IAsyncFile> >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 8147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 1608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IAsyncFile>> __when_expr_3 = c->readFile(f->getFileName());
															#line 1608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 4;
															#line 1608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 3, Reference<IAsyncFile> >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 8163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state > 0) static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 0;
		static_cast<WriteAndVerifyFileActor*>(this)->ActorCallback< WriteAndVerifyFileActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteAndVerifyFileActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< WriteAndVerifyFileActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< WriteAndVerifyFileActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont5(int loopDepth) 
	{
															#line 1609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<int64_t> __when_expr_4 = inputFile->size();
															#line 1609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont5when1(__when_expr_4.get(), loopDepth); };
		static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 5;
															#line 1609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 4, int64_t >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 8242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4when1(Reference<IAsyncFile> const& __inputFile,int loopDepth) 
	{
															#line 1608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		inputFile = __inputFile;
															#line 8251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Reference<IAsyncFile> && __inputFile,int loopDepth) 
	{
		inputFile = std::move(__inputFile);
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state > 0) static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 0;
		static_cast<WriteAndVerifyFileActor*>(this)->ActorCallback< WriteAndVerifyFileActor, 3, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< WriteAndVerifyFileActor, 3, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< WriteAndVerifyFileActor, 3, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< WriteAndVerifyFileActor, 3, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont6(int64_t const& fileSize,int loopDepth) 
	{
															#line 1610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(size, fileSize);
															#line 1611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (size > 0)
															#line 8320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			buf = Standalone<VectorRef<uint8_t>>();
															#line 1613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			buf.resize(buf.arena(), fileSize);
															#line 1614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<int> __when_expr_5 = inputFile->read(buf.begin(), buf.size(), 0);
															#line 1614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont6when1(__when_expr_5.get(), loopDepth); };
			static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 6;
															#line 1614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 5, int >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 8335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont7(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont6(int64_t && fileSize,int loopDepth) 
	{
															#line 1610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(size, fileSize);
															#line 1611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (size > 0)
															#line 8351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			buf = Standalone<VectorRef<uint8_t>>();
															#line 1613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			buf.resize(buf.arena(), fileSize);
															#line 1614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<int> __when_expr_5 = inputFile->read(buf.begin(), buf.size(), 0);
															#line 1614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont6when1(__when_expr_5.get(), loopDepth); };
			static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 6;
															#line 1614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 5, int >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 8366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont7(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont5when1(int64_t const& fileSize,int loopDepth) 
	{
		loopDepth = a_body1cont6(fileSize, loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(int64_t && fileSize,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(fileSize), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state > 0) static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 0;
		static_cast<WriteAndVerifyFileActor*>(this)->ActorCallback< WriteAndVerifyFileActor, 4, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< WriteAndVerifyFileActor, 4, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< WriteAndVerifyFileActor, 4, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< WriteAndVerifyFileActor, 4, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont7(int loopDepth) 
	{
															#line 1618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<WriteAndVerifyFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteAndVerifyFileActorState(); static_cast<WriteAndVerifyFileActor*>(this)->destroy(); return 0; }
															#line 8443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<WriteAndVerifyFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteAndVerifyFileActorState();
		static_cast<WriteAndVerifyFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont8(int const& b,int loopDepth) 
	{
															#line 1615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(b, buf.size());
															#line 1616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(buf == content);
															#line 8457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont8(int && b,int loopDepth) 
	{
															#line 1615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(b, buf.size());
															#line 1616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(buf == content);
															#line 8468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(int const& b,int loopDepth) 
	{
		loopDepth = a_body1cont8(b, loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(int && b,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(b), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state > 0) static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 0;
		static_cast<WriteAndVerifyFileActor*>(this)->ActorCallback< WriteAndVerifyFileActor, 5, int >::remove();

	}
	void a_callback_fire(ActorCallback< WriteAndVerifyFileActor, 5, int >*,int const& value) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< WriteAndVerifyFileActor, 5, int >*,int && value) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< WriteAndVerifyFileActor, 5, int >*,Error err) 
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), 5);

	}
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IBackupContainer> c;
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IBackupFile> f;
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int size;
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowLock* lock;
															#line 1589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Standalone<VectorRef<uint8_t>> content;
															#line 1592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowLock::Releaser releaser;
															#line 1600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	VectorRef<uint8_t> sendBuf;
															#line 1602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int n;
															#line 1608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IAsyncFile> inputFile;
															#line 1612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Standalone<VectorRef<uint8_t>> buf;
															#line 8556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via writeAndVerifyFile()
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class WriteAndVerifyFileActor final : public Actor<Void>, public ActorCallback< WriteAndVerifyFileActor, 0, Void >, public ActorCallback< WriteAndVerifyFileActor, 1, Void >, public ActorCallback< WriteAndVerifyFileActor, 2, Void >, public ActorCallback< WriteAndVerifyFileActor, 3, Reference<IAsyncFile> >, public ActorCallback< WriteAndVerifyFileActor, 4, int64_t >, public ActorCallback< WriteAndVerifyFileActor, 5, int >, public FastAllocated<WriteAndVerifyFileActor>, public WriteAndVerifyFileActorState<WriteAndVerifyFileActor> {
															#line 8561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<WriteAndVerifyFileActor>::operator new;
	using FastAllocated<WriteAndVerifyFileActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WriteAndVerifyFileActor, 0, Void >;
friend struct ActorCallback< WriteAndVerifyFileActor, 1, Void >;
friend struct ActorCallback< WriteAndVerifyFileActor, 2, Void >;
friend struct ActorCallback< WriteAndVerifyFileActor, 3, Reference<IAsyncFile> >;
friend struct ActorCallback< WriteAndVerifyFileActor, 4, int64_t >;
friend struct ActorCallback< WriteAndVerifyFileActor, 5, int >;
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	WriteAndVerifyFileActor(Reference<IBackupContainer> const& c,Reference<IBackupFile> const& f,int const& size,FlowLock* const& lock) 
															#line 8577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   WriteAndVerifyFileActorState<WriteAndVerifyFileActor>(c, f, size, lock)
	{
		fdb_probe_actor_enter("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("writeAndVerifyFile");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("writeAndVerifyFile", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WriteAndVerifyFileActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WriteAndVerifyFileActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< WriteAndVerifyFileActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< WriteAndVerifyFileActor, 3, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< WriteAndVerifyFileActor, 4, int64_t >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< WriteAndVerifyFileActor, 5, int >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] Future<Void> writeAndVerifyFile( Reference<IBackupContainer> const& c, Reference<IBackupFile> const& f, int const& size, FlowLock* const& lock ) {
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new WriteAndVerifyFileActor(c, f, size, lock));
															#line 8609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

// Randomly advance version by up to 1 second of versions
Version nextVersion(Version v) {
	int64_t increment = deterministicRandom()->randomInt64(1, CLIENT_KNOBS->CORE_VERSIONSPERSECOND);
	return v + increment;
}

// Write a snapshot file with only begin & end key
															#line 8621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via testWriteSnapshotFile()
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class TestWriteSnapshotFileActor>
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class TestWriteSnapshotFileActorState {
															#line 8627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	TestWriteSnapshotFileActorState(Reference<IBackupFile> const& file,Key const& begin,Key const& end,uint32_t const& blockSize) 
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : file(file),
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   begin(begin),
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   end(end),
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   blockSize(blockSize)
															#line 8640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this));

	}
	~TestWriteSnapshotFileActorState() 
	{
		fdb_probe_actor_destroy("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT_GT(blockSize, 3 * sizeof(uint32_t) + begin.size() + end.size());
															#line 1631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			uint32_t fileVersion = BACKUP_AGENT_SNAPSHOT_FILE_VERSION;
															#line 1633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_0 = file->append((uint8_t*)&fileVersion, sizeof(fileVersion));
															#line 1633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 1;
															#line 1633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 0, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 8666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~TestWriteSnapshotFileActorState();
		static_cast<TestWriteSnapshotFileActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_1 = file->appendStringRefWithLen(begin);
															#line 1636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 2;
															#line 1636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 1, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 8696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_1 = file->appendStringRefWithLen(begin);
															#line 1636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 2;
															#line 1636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 1, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 8712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state > 0) static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 0;
		static_cast<TestWriteSnapshotFileActor*>(this)->ActorCallback< TestWriteSnapshotFileActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestWriteSnapshotFileActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TestWriteSnapshotFileActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TestWriteSnapshotFileActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 1639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = file->appendStringRefWithLen(end);
															#line 1639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 3;
															#line 1639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 2, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 8791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = file->appendStringRefWithLen(end);
															#line 1639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 3;
															#line 1639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 2, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 8807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state > 0) static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 0;
		static_cast<TestWriteSnapshotFileActor*>(this)->ActorCallback< TestWriteSnapshotFileActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestWriteSnapshotFileActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TestWriteSnapshotFileActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TestWriteSnapshotFileActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 1641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int bytesLeft = blockSize - file->size();
															#line 1642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (bytesLeft > 0)
															#line 8881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			Value paddings = fileBackup::makePadding(bytesLeft);
															#line 1644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_3 = file->append(paddings.begin(), bytesLeft);
															#line 1644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
			static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 4;
															#line 1644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 3, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 8894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 1641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int bytesLeft = blockSize - file->size();
															#line 1642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (bytesLeft > 0)
															#line 8910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			Value paddings = fileBackup::makePadding(bytesLeft);
															#line 1644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_3 = file->append(paddings.begin(), bytesLeft);
															#line 1644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
			static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 4;
															#line 1644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 3, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 8923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont4(loopDepth);
		}

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
		if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state > 0) static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 0;
		static_cast<TestWriteSnapshotFileActor*>(this)->ActorCallback< TestWriteSnapshotFileActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestWriteSnapshotFileActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< TestWriteSnapshotFileActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< TestWriteSnapshotFileActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont4(int loopDepth) 
	{
															#line 1646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_4 = file->finish();
															#line 1646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 5;
															#line 1646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 4, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 9007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state > 0) static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 0;
		static_cast<TestWriteSnapshotFileActor*>(this)->ActorCallback< TestWriteSnapshotFileActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestWriteSnapshotFileActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< TestWriteSnapshotFileActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< TestWriteSnapshotFileActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 1647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<TestWriteSnapshotFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestWriteSnapshotFileActorState(); static_cast<TestWriteSnapshotFileActor*>(this)->destroy(); return 0; }
															#line 9091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<TestWriteSnapshotFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TestWriteSnapshotFileActorState();
		static_cast<TestWriteSnapshotFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 1647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<TestWriteSnapshotFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestWriteSnapshotFileActorState(); static_cast<TestWriteSnapshotFileActor*>(this)->destroy(); return 0; }
															#line 9103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<TestWriteSnapshotFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TestWriteSnapshotFileActorState();
		static_cast<TestWriteSnapshotFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state > 0) static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 0;
		static_cast<TestWriteSnapshotFileActor*>(this)->ActorCallback< TestWriteSnapshotFileActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestWriteSnapshotFileActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< TestWriteSnapshotFileActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< TestWriteSnapshotFileActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IBackupFile> file;
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Key begin;
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Key end;
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	uint32_t blockSize;
															#line 9182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via testWriteSnapshotFile()
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class TestWriteSnapshotFileActor final : public Actor<Void>, public ActorCallback< TestWriteSnapshotFileActor, 0, Void >, public ActorCallback< TestWriteSnapshotFileActor, 1, Void >, public ActorCallback< TestWriteSnapshotFileActor, 2, Void >, public ActorCallback< TestWriteSnapshotFileActor, 3, Void >, public ActorCallback< TestWriteSnapshotFileActor, 4, Void >, public FastAllocated<TestWriteSnapshotFileActor>, public TestWriteSnapshotFileActorState<TestWriteSnapshotFileActor> {
															#line 9187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<TestWriteSnapshotFileActor>::operator new;
	using FastAllocated<TestWriteSnapshotFileActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TestWriteSnapshotFileActor, 0, Void >;
friend struct ActorCallback< TestWriteSnapshotFileActor, 1, Void >;
friend struct ActorCallback< TestWriteSnapshotFileActor, 2, Void >;
friend struct ActorCallback< TestWriteSnapshotFileActor, 3, Void >;
friend struct ActorCallback< TestWriteSnapshotFileActor, 4, Void >;
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	TestWriteSnapshotFileActor(Reference<IBackupFile> const& file,Key const& begin,Key const& end,uint32_t const& blockSize) 
															#line 9202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   TestWriteSnapshotFileActorState<TestWriteSnapshotFileActor>(file, begin, end, blockSize)
	{
		fdb_probe_actor_enter("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("testWriteSnapshotFile");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("testWriteSnapshotFile", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TestWriteSnapshotFileActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TestWriteSnapshotFileActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< TestWriteSnapshotFileActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< TestWriteSnapshotFileActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< TestWriteSnapshotFileActor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Void> testWriteSnapshotFile( Reference<IBackupFile> const& file, Key const& begin, Key const& end, uint32_t const& blockSize ) {
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new TestWriteSnapshotFileActor(file, begin, end, blockSize));
															#line 9233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 9238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via testBackupContainer()
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class TestBackupContainerActor>
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class TestBackupContainerActorState {
															#line 9244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	TestBackupContainerActorState(std::string const& url,Optional<std::string> const& proxy,Optional<std::string> const& encryptionKeyFileName) 
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : url(url),
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   proxy(proxy),
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   encryptionKeyFileName(encryptionKeyFileName),
															#line 1653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   lock(100e6)
															#line 9257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("testBackupContainer", reinterpret_cast<unsigned long>(this));

	}
	~TestBackupContainerActorState() 
	{
		fdb_probe_actor_destroy("testBackupContainer", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (encryptionKeyFileName.present())
															#line 9272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Void> __when_expr_0 = BackupContainerFileSystem::createTestEncryptionKeyFile(encryptionKeyFileName.get());
															#line 1656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 1;
															#line 1656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 0, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 9283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~TestBackupContainerActorState();
		static_cast<TestBackupContainerActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("BackupContainerTest URL %s\n", url.c_str());
															#line 1661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		c = IBackupContainer::openContainer(url, proxy, encryptionKeyFileName);
															#line 9313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		try {
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_1 = c->deleteContainer();
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 9319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 2;
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 1, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 9324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
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
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 1671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = c->create();
															#line 1671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 3;
															#line 1671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 2, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 9421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (e.code() != error_code_backup_invalid_url && e.code() != error_code_backup_does_not_exist)
															#line 9431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 9435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
			loopDepth = a_body1cont3(loopDepth);
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
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

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
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont5(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 1673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		writes = std::vector<Future<Void>>();
															#line 1674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshots = std::map<Version, std::vector<std::string>>();
															#line 1675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotSizes = std::map<Version, int64_t>();
															#line 1676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotBeginEndKeys = std::map<Version, std::vector<std::pair<Key, Key>>>();
															#line 1677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		nRangeFiles = 0;
															#line 1678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs = std::map<Version, std::string>();
															#line 1679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		v = deterministicRandom()->randomInt64(0, std::numeric_limits<Version>::max() / 2);
															#line 1682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		fileSizes = { 0 };
															#line 1683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (StringRef(url).startsWith(LiteralStringRef("blob")))
															#line 9555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			fileSizes.push_back(CLIENT_KNOBS->BLOBSTORE_MULTIPART_MIN_PART_SIZE);
															#line 1685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			fileSizes.push_back(CLIENT_KNOBS->BLOBSTORE_MULTIPART_MIN_PART_SIZE + 10);
															#line 9561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 9565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont6loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 1673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		writes = std::vector<Future<Void>>();
															#line 1674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshots = std::map<Version, std::vector<std::string>>();
															#line 1675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotSizes = std::map<Version, int64_t>();
															#line 1676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotBeginEndKeys = std::map<Version, std::vector<std::pair<Key, Key>>>();
															#line 1677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		nRangeFiles = 0;
															#line 1678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs = std::map<Version, std::string>();
															#line 1679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		v = deterministicRandom()->randomInt64(0, std::numeric_limits<Version>::max() / 2);
															#line 1682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		fileSizes = { 0 };
															#line 1683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (StringRef(url).startsWith(LiteralStringRef("blob")))
															#line 9590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			fileSizes.push_back(CLIENT_KNOBS->BLOBSTORE_MULTIPART_MIN_PART_SIZE);
															#line 1685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			fileSizes.push_back(CLIENT_KNOBS->BLOBSTORE_MULTIPART_MIN_PART_SIZE + 10);
															#line 9596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 9600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont6loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont7(int loopDepth) 
	{
															#line 1744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_6 = waitForAll(writes);
															#line 1744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont7when1(__when_expr_6.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 7;
															#line 1744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 6, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 9679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont6loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1(int loopDepth) 
	{
															#line 1689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logStart = v;
															#line 1690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		kvfiles = deterministicRandom()->randomInt(0, 3);
															#line 1691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		begin = LiteralStringRef("");
															#line 1692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		end = LiteralStringRef("");
															#line 1693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		blockSize = 3 * sizeof(uint32_t) + begin.size() + end.size() + 8;
															#line 1695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 9705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont6loopBody1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont6break1(int loopDepth) 
	{
		try {
			return a_body1cont7(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1(int loopDepth) 
	{
															#line 1728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (logStart == v || deterministicRandom()->coinflip())
															#line 9727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			v = nextVersion(v);
															#line 9731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IBackupFile>> __when_expr_5 = c->writeLogFile(logStart, v, 10);
															#line 1731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 9737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1cont6loopBody1cont1when1(__when_expr_5.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 6;
															#line 1731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 5, Reference<IBackupFile> >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 9742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6loopBody1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont6loopBody1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1loopBody1(int loopDepth) 
	{
															#line 1695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(kvfiles > 0))
															#line 9758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont6loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (snapshots.empty())
															#line 9764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshots[v] = {};
															#line 1698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshotBeginEndKeys[v] = {};
															#line 1699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshotSizes[v] = 0;
															#line 1700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (deterministicRandom()->coinflip())
															#line 9774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				v = nextVersion(v);
															#line 9778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 1704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IBackupFile>> __when_expr_3 = c->writeRangeFile(snapshots.rbegin()->first, 0, v, blockSize);
															#line 1704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 9785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 2)); else return a_body1cont6loopBody1loopBody1when1(__when_expr_3.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 4;
															#line 1704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 3, Reference<IBackupFile> >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 9790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6loopBody1break1(int loopDepth) 
	{
		try {
			return a_body1cont6loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1cont6loopBody1loopBody1cont1(Reference<IBackupFile> const& range,int loopDepth) 
	{
															#line 1705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		++nRangeFiles;
															#line 1706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		v = nextVersion(v);
															#line 1707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshots.rbegin()->second.push_back(range->getFileName());
															#line 1708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotBeginEndKeys.rbegin()->second.emplace_back(begin, end);
															#line 1710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int size = chooseFileSize(fileSizes);
															#line 1711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotSizes.rbegin()->second += size;
															#line 1714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_4 = testWriteSnapshotFile(range, begin, end, blockSize);
															#line 1714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 9826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 2)); else return a_body1cont6loopBody1loopBody1cont1when1(__when_expr_4.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 5;
															#line 1714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 4, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 9831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6loopBody1loopBody1cont1(Reference<IBackupFile> && range,int loopDepth) 
	{
															#line 1705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		++nRangeFiles;
															#line 1706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		v = nextVersion(v);
															#line 1707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshots.rbegin()->second.push_back(range->getFileName());
															#line 1708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotBeginEndKeys.rbegin()->second.emplace_back(begin, end);
															#line 1710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int size = chooseFileSize(fileSizes);
															#line 1711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotSizes.rbegin()->second += size;
															#line 1714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_4 = testWriteSnapshotFile(range, begin, end, blockSize);
															#line 1714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 9854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 2)); else return a_body1cont6loopBody1loopBody1cont1when1(__when_expr_4.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 5;
															#line 1714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 4, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 9859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6loopBody1loopBody1when1(Reference<IBackupFile> const& range,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1loopBody1cont1(range, loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1loopBody1when1(Reference<IBackupFile> && range,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1loopBody1cont1(std::move(range), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 3, Reference<IBackupFile> >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 3, Reference<IBackupFile> >*,Reference<IBackupFile> const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 3, Reference<IBackupFile> >*,Reference<IBackupFile> && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 3, Reference<IBackupFile> >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont6loopBody1loopBody1cont5(Void const& _,int loopDepth) 
	{
															#line 1716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (deterministicRandom()->random01() < .2)
															#line 9931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			writes.push_back(c->writeKeyspaceSnapshotFile( snapshots.rbegin()->second, snapshotBeginEndKeys.rbegin()->second, snapshotSizes.rbegin()->second));
															#line 1719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshots[v] = {};
															#line 1720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshotBeginEndKeys[v] = {};
															#line 1721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshotSizes[v] = 0;
															#line 9941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			return a_body1cont6loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		--kvfiles;
															#line 9946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont6loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont6loopBody1loopBody1cont5(Void && _,int loopDepth) 
	{
															#line 1716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (deterministicRandom()->random01() < .2)
															#line 9955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			writes.push_back(c->writeKeyspaceSnapshotFile( snapshots.rbegin()->second, snapshotBeginEndKeys.rbegin()->second, snapshotSizes.rbegin()->second));
															#line 1719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshots[v] = {};
															#line 1720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshotBeginEndKeys[v] = {};
															#line 1721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshotSizes[v] = 0;
															#line 9965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			return a_body1cont6loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		--kvfiles;
															#line 9970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont6loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont6loopBody1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1loopBody1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1loopBody1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont6loopBody1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont6loopBody1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont6loopBody1cont2(int loopDepth) 
	{
															#line 1732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs[logStart] = log->getFileName();
															#line 1733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int size = chooseFileSize(fileSizes);
															#line 1734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		writes.push_back(writeAndVerifyFile(c, log, size, &lock));
															#line 1737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (fileSizes.empty() && !snapshots.empty() && snapshots.rbegin()->second.empty() && deterministicRandom()->random01() < .2)
															#line 10048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshots.erase(snapshots.rbegin()->first);
															#line 10052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			return a_body1cont6break1(loopDepth==0?0:loopDepth-1); // break
		}
		if (loopDepth == 0) return a_body1cont6loopHead1(0);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1when1(Reference<IBackupFile> const& __log,int loopDepth) 
	{
															#line 1731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		log = __log;
															#line 10063 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont6loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1when1(Reference<IBackupFile> && __log,int loopDepth) 
	{
		log = std::move(__log);
		loopDepth = a_body1cont6loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 5, Reference<IBackupFile> >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 5, Reference<IBackupFile> >*,Reference<IBackupFile> const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont6loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 5, Reference<IBackupFile> >*,Reference<IBackupFile> && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont6loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 5, Reference<IBackupFile> >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1cont9(Void const& _,int loopDepth) 
	{
															#line 1746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<BackupFileList> __when_expr_7 = c->dumpFileList();
															#line 1746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1Catch1(__when_expr_7.getError(), loopDepth); else return a_body1cont9when1(__when_expr_7.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 8;
															#line 1746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 7, BackupFileList >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont9(Void && _,int loopDepth) 
	{
															#line 1746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<BackupFileList> __when_expr_7 = c->dumpFileList();
															#line 1746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1Catch1(__when_expr_7.getError(), loopDepth); else return a_body1cont9when1(__when_expr_7.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 8;
															#line 1746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 7, BackupFileList >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont7when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont7when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont7when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont7when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 6);

	}
	int a_body1cont10(int loopDepth) 
	{
															#line 1747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(listing.ranges.size(), nRangeFiles);
															#line 1748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(listing.logs.size(), logs.size());
															#line 1749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(listing.snapshots.size(), snapshots.size());
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<BackupDescription> __when_expr_8 = c->describeBackup();
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_8.isReady()) { if (__when_expr_8.isError()) return a_body1Catch1(__when_expr_8.getError(), loopDepth); else return a_body1cont10when1(__when_expr_8.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 9;
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_8.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 8, BackupDescription >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont9when1(BackupFileList const& __listing,int loopDepth) 
	{
															#line 1746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		listing = __listing;
															#line 10247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont9when1(BackupFileList && __listing,int loopDepth) 
	{
		listing = std::move(__listing);
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	void a_exitChoose8() 
	{
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 7, BackupFileList >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 7, BackupFileList >*,BackupFileList const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1cont9when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 7, BackupFileList >*,BackupFileList && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1cont9when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 7, BackupFileList >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 7);

	}
	int a_body1cont10cont1(int loopDepth) 
	{
															#line 1752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("\n%s\n", desc.toString().c_str());
															#line 1755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		i = 0;
															#line 1756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 10318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont10cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont10when1(BackupDescription const& __desc,int loopDepth) 
	{
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc = __desc;
															#line 10327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont10cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont10when1(BackupDescription && __desc,int loopDepth) 
	{
		desc = std::move(__desc);
		loopDepth = a_body1cont10cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose9() 
	{
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 8, BackupDescription >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 8, BackupDescription >*,BackupDescription const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose9();
		try {
			a_body1cont10when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 8, BackupDescription >*,BackupDescription && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose9();
		try {
			a_body1cont10when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 8, BackupDescription >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose9();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 8);

	}
	int a_body1cont10cont2(int loopDepth) 
	{
															#line 1788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("DELETING\n");
															#line 1789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_14 = c->deleteContainer();
															#line 1789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_14.isReady()) { if (__when_expr_14.isError()) return a_body1Catch1(__when_expr_14.getError(), loopDepth); else return a_body1cont10cont2when1(__when_expr_14.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 15;
															#line 1789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_14.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 14, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont10cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont10cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1(int loopDepth) 
	{
															#line 1756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(i < listing.snapshots.size()))
															#line 10419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont10cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
		{
															#line 1759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Optional<RestorableFileSet>> __when_expr_9 = c->getRestoreSet(desc.maxRestorableVersion.get());
															#line 1759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_9.isReady()) { if (__when_expr_9.isError()) return a_body1Catch1(__when_expr_9.getError(), std::max(0, loopDepth - 1)); else return a_body1cont10cont1loopBody1when1(__when_expr_9.get(), loopDepth); };
			static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 10;
															#line 1759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_9.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 9, Optional<RestorableFileSet> >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont10cont1break1(int loopDepth) 
	{
		try {
			return a_body1cont10cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont1(int loopDepth) 
	{
		{
															#line 1765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Optional<RestorableFileSet>> __when_expr_10 = c->getRestoreSet(listing.snapshots[i].endVersion);
															#line 1765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_10.isReady()) { if (__when_expr_10.isError()) return a_body1Catch1(__when_expr_10.getError(), std::max(0, loopDepth - 1)); else return a_body1cont10cont1loopBody1cont1when1(__when_expr_10.get(), loopDepth); };
			static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 11;
															#line 1765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_10.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 10, Optional<RestorableFileSet> >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont3(Optional<RestorableFileSet> const& rest,int loopDepth) 
	{
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(rest.present());
															#line 10474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont10cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont3(Optional<RestorableFileSet> && rest,int loopDepth) 
	{
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(rest.present());
															#line 10483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont10cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1when1(Optional<RestorableFileSet> const& rest,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1loopBody1cont3(rest, loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1when1(Optional<RestorableFileSet> && rest,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1loopBody1cont3(std::move(rest), loopDepth);

		return loopDepth;
	}
	void a_exitChoose10() 
	{
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 9, Optional<RestorableFileSet> >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 9, Optional<RestorableFileSet> >*,Optional<RestorableFileSet> const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 9);
		a_exitChoose10();
		try {
			a_body1cont10cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 9);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 9, Optional<RestorableFileSet> >*,Optional<RestorableFileSet> && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 9);
		a_exitChoose10();
		try {
			a_body1cont10cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 9);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 9, Optional<RestorableFileSet> >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 9);
		a_exitChoose10();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 9);

	}
	int a_body1cont10cont1loopBody1cont5(int loopDepth) 
	{
															#line 1770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		expireVersion = listing.snapshots[i].endVersion;
															#line 1773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		fmt::print("EXPIRE TO {}\n", expireVersion);
															#line 1774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		f = c->expireData(expireVersion);
															#line 1775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_11 = ready(f);
															#line 1775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_11.isReady()) { if (__when_expr_11.isError()) return a_body1Catch1(__when_expr_11.getError(), std::max(0, loopDepth - 1)); else return a_body1cont10cont1loopBody1cont5when1(__when_expr_11.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 12;
															#line 1775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_11.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 11, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont6(Optional<RestorableFileSet> const& rest,int loopDepth) 
	{
															#line 1766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(rest.present());
															#line 10577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont10cont1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont6(Optional<RestorableFileSet> && rest,int loopDepth) 
	{
															#line 1766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(rest.present());
															#line 10586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont10cont1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont1when1(Optional<RestorableFileSet> const& rest,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1loopBody1cont6(rest, loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont1when1(Optional<RestorableFileSet> && rest,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1loopBody1cont6(std::move(rest), loopDepth);

		return loopDepth;
	}
	void a_exitChoose11() 
	{
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 10, Optional<RestorableFileSet> >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 10, Optional<RestorableFileSet> >*,Optional<RestorableFileSet> const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 10);
		a_exitChoose11();
		try {
			a_body1cont10cont1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 10);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 10, Optional<RestorableFileSet> >*,Optional<RestorableFileSet> && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 10);
		a_exitChoose11();
		try {
			a_body1cont10cont1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 10);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 10, Optional<RestorableFileSet> >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 10);
		a_exitChoose11();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 10);

	}
	int a_body1cont10cont1loopBody1cont8(Void const& _,int loopDepth) 
	{
															#line 1778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (f.isError())
															#line 10658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT_EQ(f.getError().code(), error_code_backup_cannot_expire);
															#line 1780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT_EQ(i, listing.snapshots.size() - 1);
															#line 1781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_12 = c->expireData(expireVersion, true);
															#line 1781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_12.isReady()) { if (__when_expr_12.isError()) return a_body1Catch1(__when_expr_12.getError(), std::max(0, loopDepth - 1)); else return a_body1cont10cont1loopBody1cont8when1(__when_expr_12.get(), loopDepth); };
			static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 13;
															#line 1781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_12.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 12, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont10cont1loopBody1cont9(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont8(Void && _,int loopDepth) 
	{
															#line 1778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (f.isError())
															#line 10687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT_EQ(f.getError().code(), error_code_backup_cannot_expire);
															#line 1780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT_EQ(i, listing.snapshots.size() - 1);
															#line 1781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_12 = c->expireData(expireVersion, true);
															#line 1781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_12.isReady()) { if (__when_expr_12.isError()) return a_body1Catch1(__when_expr_12.getError(), std::max(0, loopDepth - 1)); else return a_body1cont10cont1loopBody1cont8when1(__when_expr_12.get(), loopDepth); };
			static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 13;
															#line 1781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_12.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 12, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont10cont1loopBody1cont9(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1loopBody1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1loopBody1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose12() 
	{
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 11, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 11, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 11);
		a_exitChoose12();
		try {
			a_body1cont10cont1loopBody1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 11);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 11, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 11);
		a_exitChoose12();
		try {
			a_body1cont10cont1loopBody1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 11);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 11, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 11);
		a_exitChoose12();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 11);

	}
	int a_body1cont10cont1loopBody1cont9(int loopDepth) 
	{
															#line 1784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<BackupDescription> __when_expr_13 = c->describeBackup();
															#line 1784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_13.isReady()) { if (__when_expr_13.isError()) return a_body1Catch1(__when_expr_13.getError(), std::max(0, loopDepth - 1)); else return a_body1cont10cont1loopBody1cont9when1(__when_expr_13.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 14;
															#line 1784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_13.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 13, BackupDescription >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont10(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1loopBody1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont10(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1loopBody1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont8when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1loopBody1cont10(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont8when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1loopBody1cont10(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose13() 
	{
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 12, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 12, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 12);
		a_exitChoose13();
		try {
			a_body1cont10cont1loopBody1cont8when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 12);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 12, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 12);
		a_exitChoose13();
		try {
			a_body1cont10cont1loopBody1cont8when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 12);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 12, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 12);
		a_exitChoose13();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 12);

	}
	int a_body1cont10cont1loopBody1cont11(BackupDescription const& d,int loopDepth) 
	{
															#line 1785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("\n%s\n", d.toString().c_str());
															#line 1756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		++i;
															#line 10872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont10cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont11(BackupDescription && d,int loopDepth) 
	{
															#line 1785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("\n%s\n", d.toString().c_str());
															#line 1756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		++i;
															#line 10883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont10cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont9when1(BackupDescription const& d,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1loopBody1cont11(d, loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont9when1(BackupDescription && d,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1loopBody1cont11(std::move(d), loopDepth);

		return loopDepth;
	}
	void a_exitChoose14() 
	{
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 13, BackupDescription >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 13, BackupDescription >*,BackupDescription const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 13);
		a_exitChoose14();
		try {
			a_body1cont10cont1loopBody1cont9when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 13);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 13, BackupDescription >*,BackupDescription && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 13);
		a_exitChoose14();
		try {
			a_body1cont10cont1loopBody1cont9when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 13);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 13, BackupDescription >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 13);
		a_exitChoose14();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 13);

	}
	int a_body1cont10cont3(Void const& _,int loopDepth) 
	{
															#line 1791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		d = c->describeBackup();
															#line 1792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_15 = ready(d);
															#line 1792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_15.isReady()) { if (__when_expr_15.isError()) return a_body1Catch1(__when_expr_15.getError(), loopDepth); else return a_body1cont10cont3when1(__when_expr_15.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 16;
															#line 1792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_15.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 15, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont10cont3(Void && _,int loopDepth) 
	{
															#line 1791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		d = c->describeBackup();
															#line 1792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_15 = ready(d);
															#line 1792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_15.isReady()) { if (__when_expr_15.isError()) return a_body1Catch1(__when_expr_15.getError(), loopDepth); else return a_body1cont10cont3when1(__when_expr_15.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 16;
															#line 1792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_15.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 15, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont10cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose15() 
	{
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 14, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 14, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 14);
		a_exitChoose15();
		try {
			a_body1cont10cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 14);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 14, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 14);
		a_exitChoose15();
		try {
			a_body1cont10cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 14);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 14, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 14);
		a_exitChoose15();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 14);

	}
	int a_body1cont10cont4(Void const& _,int loopDepth) 
	{
															#line 1793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(d.isError() && d.getError().code() == error_code_backup_does_not_exist);
															#line 1795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<BackupFileList> __when_expr_16 = c->dumpFileList();
															#line 1795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_16.isReady()) { if (__when_expr_16.isError()) return a_body1Catch1(__when_expr_16.getError(), loopDepth); else return a_body1cont10cont4when1(__when_expr_16.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 17;
															#line 1795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_16.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 16, BackupFileList >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 11063 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont10cont4(Void && _,int loopDepth) 
	{
															#line 1793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(d.isError() && d.getError().code() == error_code_backup_does_not_exist);
															#line 1795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<BackupFileList> __when_expr_16 = c->dumpFileList();
															#line 1795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_16.isReady()) { if (__when_expr_16.isError()) return a_body1Catch1(__when_expr_16.getError(), loopDepth); else return a_body1cont10cont4when1(__when_expr_16.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 17;
															#line 1795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_16.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 16, BackupFileList >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 11081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont10cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose16() 
	{
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 15, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 15, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 15);
		a_exitChoose16();
		try {
			a_body1cont10cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 15);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 15, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 15);
		a_exitChoose16();
		try {
			a_body1cont10cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 15);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 15, Void >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 15);
		a_exitChoose16();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 15);

	}
	int a_body1cont10cont5(BackupFileList const& empty,int loopDepth) 
	{
															#line 1796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(empty.ranges.size(), 0);
															#line 1797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(empty.logs.size(), 0);
															#line 1798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(empty.snapshots.size(), 0);
															#line 1800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("BackupContainerTest URL=%s PASSED.\n", url.c_str());
															#line 1802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<TestBackupContainerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestBackupContainerActorState(); static_cast<TestBackupContainerActor*>(this)->destroy(); return 0; }
															#line 11161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<TestBackupContainerActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TestBackupContainerActorState();
		static_cast<TestBackupContainerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10cont5(BackupFileList && empty,int loopDepth) 
	{
															#line 1796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(empty.ranges.size(), 0);
															#line 1797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(empty.logs.size(), 0);
															#line 1798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(empty.snapshots.size(), 0);
															#line 1800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("BackupContainerTest URL=%s PASSED.\n", url.c_str());
															#line 1802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<TestBackupContainerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestBackupContainerActorState(); static_cast<TestBackupContainerActor*>(this)->destroy(); return 0; }
															#line 11181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<TestBackupContainerActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TestBackupContainerActorState();
		static_cast<TestBackupContainerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10cont4when1(BackupFileList const& empty,int loopDepth) 
	{
		loopDepth = a_body1cont10cont5(empty, loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont4when1(BackupFileList && empty,int loopDepth) 
	{
		loopDepth = a_body1cont10cont5(std::move(empty), loopDepth);

		return loopDepth;
	}
	void a_exitChoose17() 
	{
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state > 0) static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 0;
		static_cast<TestBackupContainerActor*>(this)->ActorCallback< TestBackupContainerActor, 16, BackupFileList >::remove();

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 16, BackupFileList >*,BackupFileList const& value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 16);
		a_exitChoose17();
		try {
			a_body1cont10cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 16);

	}
	void a_callback_fire(ActorCallback< TestBackupContainerActor, 16, BackupFileList >*,BackupFileList && value) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 16);
		a_exitChoose17();
		try {
			a_body1cont10cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 16);

	}
	void a_callback_error(ActorCallback< TestBackupContainerActor, 16, BackupFileList >*,Error err) 
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), 16);
		a_exitChoose17();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), 16);

	}
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string url;
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<std::string> proxy;
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<std::string> encryptionKeyFileName;
															#line 1653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowLock lock;
															#line 1661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IBackupContainer> c;
															#line 1673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<Future<Void>> writes;
															#line 1674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::map<Version, std::vector<std::string>> snapshots;
															#line 1675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::map<Version, int64_t> snapshotSizes;
															#line 1676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::map<Version, std::vector<std::pair<Key, Key>>> snapshotBeginEndKeys;
															#line 1677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int nRangeFiles;
															#line 1678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::map<Version, std::string> logs;
															#line 1679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version v;
															#line 1682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<int> fileSizes;
															#line 1689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version logStart;
															#line 1690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int kvfiles;
															#line 1691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Key begin;
															#line 1692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Key end;
															#line 1693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int blockSize;
															#line 1731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IBackupFile> log;
															#line 1746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	BackupFileList listing;
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	BackupDescription desc;
															#line 1755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int i;
															#line 1770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version expireVersion;
															#line 1774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Future<Void> f;
															#line 1791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Future<BackupDescription> d;
															#line 11302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via testBackupContainer()
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class TestBackupContainerActor final : public Actor<Void>, public ActorCallback< TestBackupContainerActor, 0, Void >, public ActorCallback< TestBackupContainerActor, 1, Void >, public ActorCallback< TestBackupContainerActor, 2, Void >, public ActorCallback< TestBackupContainerActor, 3, Reference<IBackupFile> >, public ActorCallback< TestBackupContainerActor, 4, Void >, public ActorCallback< TestBackupContainerActor, 5, Reference<IBackupFile> >, public ActorCallback< TestBackupContainerActor, 6, Void >, public ActorCallback< TestBackupContainerActor, 7, BackupFileList >, public ActorCallback< TestBackupContainerActor, 8, BackupDescription >, public ActorCallback< TestBackupContainerActor, 9, Optional<RestorableFileSet> >, public ActorCallback< TestBackupContainerActor, 10, Optional<RestorableFileSet> >, public ActorCallback< TestBackupContainerActor, 11, Void >, public ActorCallback< TestBackupContainerActor, 12, Void >, public ActorCallback< TestBackupContainerActor, 13, BackupDescription >, public ActorCallback< TestBackupContainerActor, 14, Void >, public ActorCallback< TestBackupContainerActor, 15, Void >, public ActorCallback< TestBackupContainerActor, 16, BackupFileList >, public FastAllocated<TestBackupContainerActor>, public TestBackupContainerActorState<TestBackupContainerActor> {
															#line 11307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<TestBackupContainerActor>::operator new;
	using FastAllocated<TestBackupContainerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TestBackupContainerActor, 0, Void >;
friend struct ActorCallback< TestBackupContainerActor, 1, Void >;
friend struct ActorCallback< TestBackupContainerActor, 2, Void >;
friend struct ActorCallback< TestBackupContainerActor, 3, Reference<IBackupFile> >;
friend struct ActorCallback< TestBackupContainerActor, 4, Void >;
friend struct ActorCallback< TestBackupContainerActor, 5, Reference<IBackupFile> >;
friend struct ActorCallback< TestBackupContainerActor, 6, Void >;
friend struct ActorCallback< TestBackupContainerActor, 7, BackupFileList >;
friend struct ActorCallback< TestBackupContainerActor, 8, BackupDescription >;
friend struct ActorCallback< TestBackupContainerActor, 9, Optional<RestorableFileSet> >;
friend struct ActorCallback< TestBackupContainerActor, 10, Optional<RestorableFileSet> >;
friend struct ActorCallback< TestBackupContainerActor, 11, Void >;
friend struct ActorCallback< TestBackupContainerActor, 12, Void >;
friend struct ActorCallback< TestBackupContainerActor, 13, BackupDescription >;
friend struct ActorCallback< TestBackupContainerActor, 14, Void >;
friend struct ActorCallback< TestBackupContainerActor, 15, Void >;
friend struct ActorCallback< TestBackupContainerActor, 16, BackupFileList >;
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	TestBackupContainerActor(std::string const& url,Optional<std::string> const& proxy,Optional<std::string> const& encryptionKeyFileName) 
															#line 11334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   TestBackupContainerActorState<TestBackupContainerActor>(url, proxy, encryptionKeyFileName)
	{
		fdb_probe_actor_enter("testBackupContainer", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("testBackupContainer");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("testBackupContainer", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TestBackupContainerActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TestBackupContainerActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< TestBackupContainerActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< TestBackupContainerActor, 3, Reference<IBackupFile> >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< TestBackupContainerActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< TestBackupContainerActor, 5, Reference<IBackupFile> >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< TestBackupContainerActor, 6, Void >*)0, actor_cancelled()); break;
		case 8: this->a_callback_error((ActorCallback< TestBackupContainerActor, 7, BackupFileList >*)0, actor_cancelled()); break;
		case 9: this->a_callback_error((ActorCallback< TestBackupContainerActor, 8, BackupDescription >*)0, actor_cancelled()); break;
		case 10: this->a_callback_error((ActorCallback< TestBackupContainerActor, 9, Optional<RestorableFileSet> >*)0, actor_cancelled()); break;
		case 11: this->a_callback_error((ActorCallback< TestBackupContainerActor, 10, Optional<RestorableFileSet> >*)0, actor_cancelled()); break;
		case 12: this->a_callback_error((ActorCallback< TestBackupContainerActor, 11, Void >*)0, actor_cancelled()); break;
		case 13: this->a_callback_error((ActorCallback< TestBackupContainerActor, 12, Void >*)0, actor_cancelled()); break;
		case 14: this->a_callback_error((ActorCallback< TestBackupContainerActor, 13, BackupDescription >*)0, actor_cancelled()); break;
		case 15: this->a_callback_error((ActorCallback< TestBackupContainerActor, 14, Void >*)0, actor_cancelled()); break;
		case 16: this->a_callback_error((ActorCallback< TestBackupContainerActor, 15, Void >*)0, actor_cancelled()); break;
		case 17: this->a_callback_error((ActorCallback< TestBackupContainerActor, 16, BackupFileList >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] Future<Void> testBackupContainer( std::string const& url, Optional<std::string> const& proxy, Optional<std::string> const& encryptionKeyFileName ) {
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new TestBackupContainerActor(url, proxy, encryptionKeyFileName));
															#line 11377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 11382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via flowTestCase1805()
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class FlowTestCase1805Actor>
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1805ActorState {
															#line 11388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1805ActorState(UnitTestParameters const& params) 
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : params(params)
															#line 11395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1805", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1805ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1805", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_0 = testBackupContainer(format("file://%s/fdb_backups/%llx", params.getDataDir().c_str(), timer_int()), {}, {});
															#line 1806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<FlowTestCase1805Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase1805Actor*>(this)->actor_wait_state = 1;
															#line 1806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1805Actor, 0, Void >*>(static_cast<FlowTestCase1805Actor*>(this)));
															#line 11417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~FlowTestCase1805ActorState();
		static_cast<FlowTestCase1805Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<FlowTestCase1805Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1805ActorState(); static_cast<FlowTestCase1805Actor*>(this)->destroy(); return 0; }
															#line 11440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<FlowTestCase1805Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1805ActorState();
		static_cast<FlowTestCase1805Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<FlowTestCase1805Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1805ActorState(); static_cast<FlowTestCase1805Actor*>(this)->destroy(); return 0; }
															#line 11452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<FlowTestCase1805Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1805ActorState();
		static_cast<FlowTestCase1805Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase1805Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1805Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1805Actor*>(this)->ActorCallback< FlowTestCase1805Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1805Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1805", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1805", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1805Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1805", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1805", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1805Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1805", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1805", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	UnitTestParameters params;
															#line 11525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1805()
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1805Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1805Actor, 0, Void >, public FastAllocated<FlowTestCase1805Actor>, public FlowTestCase1805ActorState<FlowTestCase1805Actor> {
															#line 11530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1805Actor>::operator new;
	using FastAllocated<FlowTestCase1805Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1805Actor, 0, Void >;
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1805Actor(UnitTestParameters const& params) 
															#line 11541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1805ActorState<FlowTestCase1805Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1805", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1805");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1805", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1805Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
static Future<Void> flowTestCase1805( UnitTestParameters const& params ) {
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new FlowTestCase1805Actor(params));
															#line 11568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1805, "/backup/containers/localdir/unencrypted")

#line 1809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 11574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via flowTestCase1810()
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class FlowTestCase1810Actor>
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1810ActorState {
															#line 11580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1810ActorState(UnitTestParameters const& params) 
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : params(params)
															#line 11587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1810", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1810ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1810", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_0 = testBackupContainer(format("file://%s/fdb_backups/%llx", params.getDataDir().c_str(), timer_int()), {}, format("%s/test_encryption_key", params.getDataDir().c_str()));
															#line 1811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<FlowTestCase1810Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase1810Actor*>(this)->actor_wait_state = 1;
															#line 1811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1810Actor, 0, Void >*>(static_cast<FlowTestCase1810Actor*>(this)));
															#line 11609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~FlowTestCase1810ActorState();
		static_cast<FlowTestCase1810Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<FlowTestCase1810Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1810ActorState(); static_cast<FlowTestCase1810Actor*>(this)->destroy(); return 0; }
															#line 11632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<FlowTestCase1810Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1810ActorState();
		static_cast<FlowTestCase1810Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<FlowTestCase1810Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1810ActorState(); static_cast<FlowTestCase1810Actor*>(this)->destroy(); return 0; }
															#line 11644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<FlowTestCase1810Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1810ActorState();
		static_cast<FlowTestCase1810Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase1810Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1810Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1810Actor*>(this)->ActorCallback< FlowTestCase1810Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1810Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1810", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1810", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1810Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1810", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1810", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1810Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1810", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1810", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	UnitTestParameters params;
															#line 11717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1810()
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1810Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1810Actor, 0, Void >, public FastAllocated<FlowTestCase1810Actor>, public FlowTestCase1810ActorState<FlowTestCase1810Actor> {
															#line 11722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1810Actor>::operator new;
	using FastAllocated<FlowTestCase1810Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1810Actor, 0, Void >;
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1810Actor(UnitTestParameters const& params) 
															#line 11733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1810ActorState<FlowTestCase1810Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1810", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1810");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1810", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1810Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
static Future<Void> flowTestCase1810( UnitTestParameters const& params ) {
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new FlowTestCase1810Actor(params));
															#line 11760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1810, "/backup/containers/localdir/encrypted")

#line 1816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 11766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via flowTestCase1817()
															#line 1817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class FlowTestCase1817Actor>
															#line 1817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1817ActorState {
															#line 11772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1817ActorState(UnitTestParameters const& params) 
															#line 1817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : params(params)
															#line 11779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1817", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1817ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1817", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!g_network->isSimulated())
															#line 11794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				const char* url = getenv("FDB_TEST_BACKUP_URL");
															#line 1820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				ASSERT(url != nullptr);
															#line 1821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Void> __when_expr_0 = testBackupContainer(url, {}, {});
															#line 1821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<FlowTestCase1817Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<FlowTestCase1817Actor*>(this)->actor_wait_state = 1;
															#line 1821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1817Actor, 0, Void >*>(static_cast<FlowTestCase1817Actor*>(this)));
															#line 11809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~FlowTestCase1817ActorState();
		static_cast<FlowTestCase1817Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<FlowTestCase1817Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1817ActorState(); static_cast<FlowTestCase1817Actor*>(this)->destroy(); return 0; }
															#line 11837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<FlowTestCase1817Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1817ActorState();
		static_cast<FlowTestCase1817Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<FlowTestCase1817Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1817Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1817Actor*>(this)->ActorCallback< FlowTestCase1817Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1817Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1817", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1817", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1817Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1817", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1817", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1817Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1817", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1817", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	UnitTestParameters params;
															#line 11922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1817()
															#line 1817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1817Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1817Actor, 0, Void >, public FastAllocated<FlowTestCase1817Actor>, public FlowTestCase1817ActorState<FlowTestCase1817Actor> {
															#line 11927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1817Actor>::operator new;
	using FastAllocated<FlowTestCase1817Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1817Actor, 0, Void >;
															#line 1817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1817Actor(UnitTestParameters const& params) 
															#line 11938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1817ActorState<FlowTestCase1817Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1817", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1817");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1817", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1817Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
static Future<Void> flowTestCase1817( UnitTestParameters const& params ) {
															#line 1817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new FlowTestCase1817Actor(params));
															#line 11965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1817, "/backup/containers/url")

#line 1825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 11971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via flowTestCase1826()
															#line 1826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class FlowTestCase1826Actor>
															#line 1826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1826ActorState {
															#line 11977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1826ActorState(UnitTestParameters const& params) 
															#line 1826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : params(params)
															#line 11984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1826", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1826ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1826", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!g_network->isSimulated())
															#line 11999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				url = getenv("FDB_TEST_BACKUP_URL");
															#line 1829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				ASSERT(url != nullptr);
															#line 1830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				printf("Listing %s\n", url);
															#line 1831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<std::vector<std::string>> __when_expr_0 = IBackupContainer::listContainers(url, {});
															#line 1831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<FlowTestCase1826Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<FlowTestCase1826Actor*>(this)->actor_wait_state = 1;
															#line 1831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1826Actor, 0, std::vector<std::string> >*>(static_cast<FlowTestCase1826Actor*>(this)));
															#line 12016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~FlowTestCase1826ActorState();
		static_cast<FlowTestCase1826Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<FlowTestCase1826Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1826ActorState(); static_cast<FlowTestCase1826Actor*>(this)->destroy(); return 0; }
															#line 12044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<FlowTestCase1826Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1826ActorState();
		static_cast<FlowTestCase1826Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(std::vector<std::string> const& urls,int loopDepth) 
	{
															#line 1832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto& u : urls ) {
															#line 1833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			printf("%s\n", u.c_str());
															#line 12058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(std::vector<std::string> && urls,int loopDepth) 
	{
															#line 1832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto& u : urls ) {
															#line 1833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			printf("%s\n", u.c_str());
															#line 12070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<std::string> const& urls,int loopDepth) 
	{
		loopDepth = a_body1cont2(urls, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<std::string> && urls,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(urls), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<FlowTestCase1826Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1826Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1826Actor*>(this)->ActorCallback< FlowTestCase1826Actor, 0, std::vector<std::string> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1826Actor, 0, std::vector<std::string> >*,std::vector<std::string> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1826", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1826", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1826Actor, 0, std::vector<std::string> >*,std::vector<std::string> && value) 
	{
		fdb_probe_actor_enter("flowTestCase1826", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1826", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1826Actor, 0, std::vector<std::string> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1826", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1826", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	UnitTestParameters params;
															#line 1828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	const char* url;
															#line 12143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1826()
															#line 1826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1826Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1826Actor, 0, std::vector<std::string> >, public FastAllocated<FlowTestCase1826Actor>, public FlowTestCase1826ActorState<FlowTestCase1826Actor> {
															#line 12148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1826Actor>::operator new;
	using FastAllocated<FlowTestCase1826Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1826Actor, 0, std::vector<std::string> >;
															#line 1826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1826Actor(UnitTestParameters const& params) 
															#line 12159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1826ActorState<FlowTestCase1826Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1826", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1826");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1826", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1826Actor, 0, std::vector<std::string> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
static Future<Void> flowTestCase1826( UnitTestParameters const& params ) {
															#line 1826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new FlowTestCase1826Actor(params));
															#line 12186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1826, "/backup/containers_list")

#line 1838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 12192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via flowTestCase1839()
															#line 1839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class FlowTestCase1839Actor>
															#line 1839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1839ActorState {
															#line 12198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1839ActorState(UnitTestParameters const& params) 
															#line 1839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : params(params)
															#line 12205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1839", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1839ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1839", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			for(int i = 0;i < 1000;++i) {
															#line 1842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				int64_t ts = deterministicRandom()->randomInt64(0, std::numeric_limits<int32_t>::max());
															#line 1843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				ASSERT(BackupAgentBase::parseTime(BackupAgentBase::formatTime(ts)) == ts);
															#line 12224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 1846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupAgentBase::parseTime("2019/03/18.17:51:11-0600") == BackupAgentBase::parseTime("2019/03/18.16:51:11-0700"));
															#line 1848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupAgentBase::parseTime("2019/03/31.22:45:07-0700") == BackupAgentBase::parseTime("2019/04/01.03:45:07-0200"));
															#line 1850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupAgentBase::parseTime("2019/03/31.22:45:07+0000") == BackupAgentBase::parseTime("2019/04/01.03:45:07+0500"));
															#line 1852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupAgentBase::parseTime("2019/03/31.22:45:07+0030") == BackupAgentBase::parseTime("2019/04/01.03:45:07+0530"));
															#line 1854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupAgentBase::parseTime("2019/03/31.22:45:07+0030") == BackupAgentBase::parseTime("2019/04/01.04:00:07+0545"));
															#line 1857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<FlowTestCase1839Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1839ActorState(); static_cast<FlowTestCase1839Actor*>(this)->destroy(); return 0; }
															#line 12238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<FlowTestCase1839Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase1839ActorState();
			static_cast<FlowTestCase1839Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase1839ActorState();
		static_cast<FlowTestCase1839Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 1839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	UnitTestParameters params;
															#line 12262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1839()
															#line 1839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1839Actor final : public Actor<Void>, public FastAllocated<FlowTestCase1839Actor>, public FlowTestCase1839ActorState<FlowTestCase1839Actor> {
															#line 12267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1839Actor>::operator new;
	using FastAllocated<FlowTestCase1839Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 1839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1839Actor(UnitTestParameters const& params) 
															#line 12277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1839ActorState<FlowTestCase1839Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1839", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1839");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1839", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
															#line 1839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
static Future<Void> flowTestCase1839( UnitTestParameters const& params ) {
															#line 1839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new FlowTestCase1839Actor(params));
															#line 12303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1839, "/backup/time")

#line 1859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 12309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via flowTestCase1860()
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class FlowTestCase1860Actor>
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1860ActorState {
															#line 12315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1860ActorState(UnitTestParameters const& params) 
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : params(params)
															#line 12322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1860", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1860ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1860", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::vector<LogFile> files;
															#line 1864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 0, 100, 10, "file1", 100, 0, 2 });
															#line 1865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 0, 99));
															#line 1866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 0) == 0);
															#line 1868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 0, 100, 10, "file2", 200, 1, 2 });
															#line 1869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(files.begin(), files.end());
															#line 1870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 0, 99));
															#line 1871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 0, 100));
															#line 1872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 0) == 99);
															#line 1875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 100, 200, 10, "file3", 200, 0, 3 });
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 100, 250, 10, "file4", 200, 1, 3 });
															#line 1877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(files.begin(), files.end());
															#line 1878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 0, 99));
															#line 1879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 0, 100));
															#line 1880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 50, 150));
															#line 1881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 0) == 99);
															#line 1883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 100, 300, 10, "file5", 200, 2, 3 });
															#line 1884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(files.begin(), files.end());
															#line 1885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 50, 150));
															#line 1886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 50, 200));
															#line 1887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 10, 199));
															#line 1888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 0) == 199);
															#line 1889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 100) == 199);
															#line 1891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 250, 300, 10, "file6", 200, 0, 3 });
															#line 1892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(files.begin(), files.end());
															#line 1893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 50, 240));
															#line 1894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 100, 280));
															#line 1895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 99) == 199);
															#line 1897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 250, 300, 10, "file7", 200, 1, 3 });
															#line 1898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(files.begin(), files.end());
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 100, 280));
															#line 1901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 200, 250, 10, "file8", 200, 0, 3 });
															#line 1902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(files.begin(), files.end());
															#line 1903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 0, 299));
															#line 1904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 100, 280));
															#line 1905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 150) == 299);
															#line 1909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 300, 400, 10, "file10", 200, 0, 1 });
															#line 1910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(files.begin(), files.end());
															#line 1911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 0, 399));
															#line 1912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 100, 399));
															#line 1913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 150, 399));
															#line 1914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 250, 399));
															#line 1915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 0) == 399);
															#line 1916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 99) == 399);
															#line 1917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 250) == 399);
															#line 1919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<FlowTestCase1860Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1860ActorState(); static_cast<FlowTestCase1860Actor*>(this)->destroy(); return 0; }
															#line 12427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<FlowTestCase1860Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase1860ActorState();
			static_cast<FlowTestCase1860Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase1860ActorState();
		static_cast<FlowTestCase1860Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	UnitTestParameters params;
															#line 12451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1860()
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1860Actor final : public Actor<Void>, public FastAllocated<FlowTestCase1860Actor>, public FlowTestCase1860ActorState<FlowTestCase1860Actor> {
															#line 12456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1860Actor>::operator new;
	using FastAllocated<FlowTestCase1860Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1860Actor(UnitTestParameters const& params) 
															#line 12466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1860ActorState<FlowTestCase1860Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1860", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1860");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1860", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
static Future<Void> flowTestCase1860( UnitTestParameters const& params ) {
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new FlowTestCase1860Actor(params));
															#line 12492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1860, "/backup/continuous")

#line 1921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

} // namespace backup_test
