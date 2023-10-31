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
#include "fdbclient/BackupContainer.h"
#include "flow/BooleanParam.h"
#ifdef BUILD_AZURE_BACKUP
#include "fdbclient/BackupContainerAzureBlobStore.h"
#endif
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
																#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via readKeyspaceSnapshot()
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class ReadKeyspaceSnapshotActor>
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ReadKeyspaceSnapshotActorState {
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ReadKeyspaceSnapshotActorState(Reference<BackupContainerFileSystem> const& bc,KeyspaceSnapshotFile const& snapshot) 
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   snapshot(snapshot)
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<std::vector<RangeFile>> __when_expr_0 = bc->listRangeFiles(snapshot.beginVersion, snapshot.endVersion);
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 1;
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadKeyspaceSnapshotActor, 0, std::vector<RangeFile> >*>(static_cast<ReadKeyspaceSnapshotActor*>(this)));
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		rangeIndex = std::map<std::string, RangeFile>();
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto& f : files ) {
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			rangeIndex[f.fileName] = std::move(f);
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IAsyncFile>> __when_expr_1 = bc->readFile(snapshot.fileName);
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 2;
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadKeyspaceSnapshotActor, 1, Reference<IAsyncFile> >*>(static_cast<ReadKeyspaceSnapshotActor*>(this)));
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<RangeFile> && files,int loopDepth) 
	{
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		rangeIndex = std::map<std::string, RangeFile>();
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto& f : files ) {
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			rangeIndex[f.fileName] = std::move(f);
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IAsyncFile>> __when_expr_1 = bc->readFile(snapshot.fileName);
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 2;
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadKeyspaceSnapshotActor, 1, Reference<IAsyncFile> >*>(static_cast<ReadKeyspaceSnapshotActor*>(this)));
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<int64_t> __when_expr_2 = f->size();
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 3;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReadKeyspaceSnapshotActor, 2, int64_t >*>(static_cast<ReadKeyspaceSnapshotActor*>(this)));
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(Reference<IAsyncFile> const& __f,int loopDepth) 
	{
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		f = __f;
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		buf = makeString(size);
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_3 = success(f->read(mutateString(buf), buf.size(), 0));
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 4;
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReadKeyspaceSnapshotActor, 3, Void >*>(static_cast<ReadKeyspaceSnapshotActor*>(this)));
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(int64_t && size,int loopDepth) 
	{
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		buf = makeString(size);
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_3 = success(f->read(mutateString(buf), buf.size(), 0));
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<ReadKeyspaceSnapshotActor*>(this)->actor_wait_state = 4;
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReadKeyspaceSnapshotActor, 3, Void >*>(static_cast<ReadKeyspaceSnapshotActor*>(this)));
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		json_spirit::mValue json;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		json_spirit::read_string(buf.toString(), json);
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		JSONDoc doc(json);
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		Version v;
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!doc.tryGet("beginVersion", v) || v != snapshot.beginVersion)
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!doc.tryGet("endVersion", v) || v != snapshot.endVersion)
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		json_spirit::mValue& filesArray = doc.create("files");
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (filesArray.type() != json_spirit::array_type)
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::vector<RangeFile> results;
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int missing = 0;
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& fileValue : filesArray.get_array() ) {
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (fileValue.type() != json_spirit::str_type)
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			auto i = rangeIndex.find(fileValue.get_str());
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (i == rangeIndex.end())
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent(SevError, "FileRestoreMissingRangeFile") .detail("URL", bc->getURL()) .detail("File", fileValue.get_str());
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				++missing;
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (missing == 0)
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				results.push_back(i->second);
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (missing > 0)
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevError, "FileRestoreMissingRangeFileSummary") .detail("URL", bc->getURL()) .detail("Count", missing);
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_missing_data(), loopDepth);
															#line 473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::map<std::string, KeyRange> fileKeyRanges;
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		JSONDoc ranges = doc.subDoc("keyRanges");
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto i : ranges.obj() ) {
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			const std::string& filename = i.first;
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			JSONDoc fields(i.second);
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::string begin, end;
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (fields.tryGet("beginKey", begin) && fields.tryGet("endKey", end))
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent("ManifestFields") .detail("File", filename) .detail("Begin", printable(StringRef(begin))) .detail("End", printable(StringRef(end)));
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				fileKeyRanges.emplace(filename, KeyRange(KeyRangeRef(StringRef(begin), StringRef(end))));
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
			else
			{
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent("MalFormattedManifest").detail("Key", filename);
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<ReadKeyspaceSnapshotActor*>(this)->SAV<std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>>::futures) { (void)(std::make_pair(results, fileKeyRanges)); this->~ReadKeyspaceSnapshotActorState(); static_cast<ReadKeyspaceSnapshotActor*>(this)->destroy(); return 0; }
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<ReadKeyspaceSnapshotActor*>(this)->SAV< std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> >::value()) std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>(std::make_pair(results, fileKeyRanges));
		this->~ReadKeyspaceSnapshotActorState();
		static_cast<ReadKeyspaceSnapshotActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		json_spirit::mValue json;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		json_spirit::read_string(buf.toString(), json);
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		JSONDoc doc(json);
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		Version v;
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!doc.tryGet("beginVersion", v) || v != snapshot.beginVersion)
															#line 528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!doc.tryGet("endVersion", v) || v != snapshot.endVersion)
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		json_spirit::mValue& filesArray = doc.create("files");
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (filesArray.type() != json_spirit::array_type)
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::vector<RangeFile> results;
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int missing = 0;
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& fileValue : filesArray.get_array() ) {
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (fileValue.type() != json_spirit::str_type)
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			auto i = rangeIndex.find(fileValue.get_str());
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (i == rangeIndex.end())
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent(SevError, "FileRestoreMissingRangeFile") .detail("URL", bc->getURL()) .detail("File", fileValue.get_str());
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				++missing;
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (missing == 0)
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				results.push_back(i->second);
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (missing > 0)
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevError, "FileRestoreMissingRangeFileSummary") .detail("URL", bc->getURL()) .detail("Count", missing);
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(restore_missing_data(), loopDepth);
															#line 595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::map<std::string, KeyRange> fileKeyRanges;
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		JSONDoc ranges = doc.subDoc("keyRanges");
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto i : ranges.obj() ) {
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			const std::string& filename = i.first;
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			JSONDoc fields(i.second);
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::string begin, end;
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (fields.tryGet("beginKey", begin) && fields.tryGet("endKey", end))
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent("ManifestFields") .detail("File", filename) .detail("Begin", printable(StringRef(begin))) .detail("End", printable(StringRef(end)));
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				fileKeyRanges.emplace(filename, KeyRange(KeyRangeRef(StringRef(begin), StringRef(end))));
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
			else
			{
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent("MalFormattedManifest").detail("Key", filename);
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(restore_corrupted_data(), loopDepth);
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<ReadKeyspaceSnapshotActor*>(this)->SAV<std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>>::futures) { (void)(std::make_pair(results, fileKeyRanges)); this->~ReadKeyspaceSnapshotActorState(); static_cast<ReadKeyspaceSnapshotActor*>(this)->destroy(); return 0; }
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	KeyspaceSnapshotFile snapshot;
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::map<std::string, RangeFile> rangeIndex;
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IAsyncFile> f;
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Standalone<StringRef> buf;
															#line 711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via readKeyspaceSnapshot()
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ReadKeyspaceSnapshotActor final : public Actor<std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>>, public ActorCallback< ReadKeyspaceSnapshotActor, 0, std::vector<RangeFile> >, public ActorCallback< ReadKeyspaceSnapshotActor, 1, Reference<IAsyncFile> >, public ActorCallback< ReadKeyspaceSnapshotActor, 2, int64_t >, public ActorCallback< ReadKeyspaceSnapshotActor, 3, Void >, public FastAllocated<ReadKeyspaceSnapshotActor>, public ReadKeyspaceSnapshotActorState<ReadKeyspaceSnapshotActor> {
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ReadKeyspaceSnapshotActor(Reference<BackupContainerFileSystem> const& bc,KeyspaceSnapshotFile const& snapshot) 
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>> readKeyspaceSnapshot( Reference<BackupContainerFileSystem> const& bc, KeyspaceSnapshotFile const& snapshot ) {
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>>(new ReadKeyspaceSnapshotActor(bc, snapshot));
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

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

																#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via writeKeyspaceSnapshotFile()
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class WriteKeyspaceSnapshotFileActor>
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class WriteKeyspaceSnapshotFileActorState {
															#line 805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	WriteKeyspaceSnapshotFileActorState(Reference<BackupContainerFileSystem> const& bc,std::vector<std::string> const& fileNames,std::vector<std::pair<Key, Key>> const& beginEndKeys,int64_t const& totalBytes,IncludeKeyRangeMap const& includeKeyRangeMap) 
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   fileNames(fileNames),
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   beginEndKeys(beginEndKeys),
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   totalBytes(totalBytes),
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   includeKeyRangeMap(includeKeyRangeMap)
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!fileNames.empty() && fileNames.size() == beginEndKeys.size());
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			minVer = std::numeric_limits<Version>::max();
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			maxVer = 0;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			rf = RangeFile();
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			fileArray = json_spirit::mArray();
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			RangeForbody1Iterator0 = std::begin(fileNames);
															#line 845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		json = json_spirit::mValue();
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		doc = JSONDoc(json);
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		doc.create("files") = std::move(fileArray);
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		doc.create("totalBytes") = totalBytes;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		doc.create("beginVersion") = minVer;
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		doc.create("endVersion") = maxVer;
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (includeKeyRangeMap)
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			auto ranges = doc.subDoc("keyRanges");
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			for(int i = 0;i < beginEndKeys.size();i++) {
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				auto fileDoc = ranges.subDoc(fileNames[i], false);
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				fileDoc.create("beginKey") = beginEndKeys[i].first.toString();
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				fileDoc.create("endKey") = beginEndKeys[i].second.toString();
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_1 = yield();
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 2;
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WriteKeyspaceSnapshotFileActor, 1, Void >*>(static_cast<WriteKeyspaceSnapshotFileActor*>(this)));
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(RangeForbody1Iterator0 != std::end(fileNames)))
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		const auto& f = *RangeForbody1Iterator0;
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (pathToRangeFile(rf, f, 0))
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				fileArray.push_back(f);
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (rf.version < minVer)
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					minVer = rf.version;
															#line 940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (rf.version > maxVer)
															#line 944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					maxVer = rf.version;
															#line 948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
			else
			{
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(restore_unknown_file_type(), std::max(0, loopDepth - 1));
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_0 = yield();
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 1;
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WriteKeyspaceSnapshotFileActor, 0, Void >*>(static_cast<WriteKeyspaceSnapshotFileActor*>(this)));
															#line 966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		++RangeForbody1Iterator0;
															#line 989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		docString = json_spirit::write_string(json);
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IBackupFile>> __when_expr_2 = bc->writeFile(format("snapshots/snapshot,%lld,%lld,%lld", minVer, maxVer, totalBytes));
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1077 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 3;
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteKeyspaceSnapshotFileActor, 2, Reference<IBackupFile> >*>(static_cast<WriteKeyspaceSnapshotFileActor*>(this)));
															#line 1082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		docString = json_spirit::write_string(json);
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IBackupFile>> __when_expr_2 = bc->writeFile(format("snapshots/snapshot,%lld,%lld,%lld", minVer, maxVer, totalBytes));
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 3;
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteKeyspaceSnapshotFileActor, 2, Reference<IBackupFile> >*>(static_cast<WriteKeyspaceSnapshotFileActor*>(this)));
															#line 1100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
	int a_body1cont5(int loopDepth) 
	{
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_3 = f->append(docString.data(), docString.size());
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 4;
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WriteKeyspaceSnapshotFileActor, 3, Void >*>(static_cast<WriteKeyspaceSnapshotFileActor*>(this)));
															#line 1179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(Reference<IBackupFile> const& __f,int loopDepth) 
	{
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		f = __f;
															#line 1188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Reference<IBackupFile> && __f,int loopDepth) 
	{
		f = std::move(__f);
		loopDepth = a_body1cont5(loopDepth);

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
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_4 = f->finish();
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont6when1(__when_expr_4.get(), loopDepth); };
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 5;
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< WriteKeyspaceSnapshotFileActor, 4, Void >*>(static_cast<WriteKeyspaceSnapshotFileActor*>(this)));
															#line 1262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_4 = f->finish();
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont6when1(__when_expr_4.get(), loopDepth); };
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 5;
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< WriteKeyspaceSnapshotFileActor, 4, Void >*>(static_cast<WriteKeyspaceSnapshotFileActor*>(this)));
															#line 1278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state > 0) static_cast<WriteKeyspaceSnapshotFileActor*>(this)->actor_wait_state = 0;
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->ActorCallback< WriteKeyspaceSnapshotFileActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteKeyspaceSnapshotFileActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeKeyspaceSnapshotFile", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5when1(value, 0);
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
			a_body1cont5when1(std::move(value), 0);
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
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<WriteKeyspaceSnapshotFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteKeyspaceSnapshotFileActorState(); static_cast<WriteKeyspaceSnapshotFileActor*>(this)->destroy(); return 0; }
															#line 1350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<WriteKeyspaceSnapshotFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteKeyspaceSnapshotFileActorState();
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<WriteKeyspaceSnapshotFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteKeyspaceSnapshotFileActorState(); static_cast<WriteKeyspaceSnapshotFileActor*>(this)->destroy(); return 0; }
															#line 1362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<WriteKeyspaceSnapshotFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteKeyspaceSnapshotFileActorState();
		static_cast<WriteKeyspaceSnapshotFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

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
			a_body1cont6when1(value, 0);
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
			a_body1cont6when1(std::move(value), 0);
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
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<std::string> fileNames;
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<std::pair<Key, Key>> beginEndKeys;
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int64_t totalBytes;
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	IncludeKeyRangeMap includeKeyRangeMap;
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version minVer;
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version maxVer;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	RangeFile rf;
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	json_spirit::mArray fileArray;
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	decltype(std::begin(std::declval<std::vector<std::string>>())) RangeForbody1Iterator0;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	json_spirit::mValue json;
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	JSONDoc doc;
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string docString;
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IBackupFile> f;
															#line 1461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via writeKeyspaceSnapshotFile()
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class WriteKeyspaceSnapshotFileActor final : public Actor<Void>, public ActorCallback< WriteKeyspaceSnapshotFileActor, 0, Void >, public ActorCallback< WriteKeyspaceSnapshotFileActor, 1, Void >, public ActorCallback< WriteKeyspaceSnapshotFileActor, 2, Reference<IBackupFile> >, public ActorCallback< WriteKeyspaceSnapshotFileActor, 3, Void >, public ActorCallback< WriteKeyspaceSnapshotFileActor, 4, Void >, public FastAllocated<WriteKeyspaceSnapshotFileActor>, public WriteKeyspaceSnapshotFileActorState<WriteKeyspaceSnapshotFileActor> {
															#line 1466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	WriteKeyspaceSnapshotFileActor(Reference<BackupContainerFileSystem> const& bc,std::vector<std::string> const& fileNames,std::vector<std::pair<Key, Key>> const& beginEndKeys,int64_t const& totalBytes,IncludeKeyRangeMap const& includeKeyRangeMap) 
															#line 1481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   WriteKeyspaceSnapshotFileActorState<WriteKeyspaceSnapshotFileActor>(bc, fileNames, beginEndKeys, totalBytes, includeKeyRangeMap)
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
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Void> writeKeyspaceSnapshotFile( Reference<BackupContainerFileSystem> const& bc, std::vector<std::string> const& fileNames, std::vector<std::pair<Key, Key>> const& beginEndKeys, int64_t const& totalBytes, IncludeKeyRangeMap const& includeKeyRangeMap ) {
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new WriteKeyspaceSnapshotFileActor(bc, fileNames, beginEndKeys, totalBytes, includeKeyRangeMap));
															#line 1512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

																#line 1517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via dumpFileList()
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class DumpFileListActor>
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class DumpFileListActorState {
															#line 1523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	DumpFileListActorState(Reference<BackupContainerFileSystem> const& bc,Version const& begin,Version const& end) 
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   begin(begin),
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   end(end),
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   fRanges(bc->listRangeFiles(begin, end)),
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   fSnapshots(bc->listKeyspaceSnapshots(begin, end)),
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   logs(),
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   pLogs()
															#line 1542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_0 = success(fRanges) && success(fSnapshots) && store(logs, bc->listLogFiles(begin, end, false)) && store(pLogs, bc->listLogFiles(begin, end, true));
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<DumpFileListActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DumpFileListActor*>(this)->actor_wait_state = 1;
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DumpFileListActor, 0, Void >*>(static_cast<DumpFileListActor*>(this)));
															#line 1564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs.insert(logs.end(), std::make_move_iterator(pLogs.begin()), std::make_move_iterator(pLogs.end()));
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<DumpFileListActor*>(this)->SAV<BackupFileList>::futures) { (void)(BackupFileList({ fRanges.get(), std::move(logs), fSnapshots.get() })); this->~DumpFileListActorState(); static_cast<DumpFileListActor*>(this)->destroy(); return 0; }
															#line 1589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<DumpFileListActor*>(this)->SAV< BackupFileList >::value()) BackupFileList(BackupFileList({ fRanges.get(), std::move(logs), fSnapshots.get() }));
		this->~DumpFileListActorState();
		static_cast<DumpFileListActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs.insert(logs.end(), std::make_move_iterator(pLogs.begin()), std::make_move_iterator(pLogs.end()));
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<DumpFileListActor*>(this)->SAV<BackupFileList>::futures) { (void)(BackupFileList({ fRanges.get(), std::move(logs), fSnapshots.get() })); this->~DumpFileListActorState(); static_cast<DumpFileListActor*>(this)->destroy(); return 0; }
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version begin;
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version end;
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Future<std::vector<RangeFile>> fRanges;
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Future<std::vector<KeyspaceSnapshotFile>> fSnapshots;
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> logs;
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> pLogs;
															#line 1688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via dumpFileList()
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class DumpFileListActor final : public Actor<BackupFileList>, public ActorCallback< DumpFileListActor, 0, Void >, public FastAllocated<DumpFileListActor>, public DumpFileListActorState<DumpFileListActor> {
															#line 1693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<DumpFileListActor>::operator new;
	using FastAllocated<DumpFileListActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<BackupFileList>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DumpFileListActor, 0, Void >;
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	DumpFileListActor(Reference<BackupContainerFileSystem> const& bc,Version const& begin,Version const& end) 
															#line 1704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<BackupFileList> dumpFileList( Reference<BackupContainerFileSystem> const& bc, Version const& begin, Version const& end ) {
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<BackupFileList>(new DumpFileListActor(bc, begin, end));
															#line 1731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

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

																#line 1921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via describeBackup()
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class DescribeBackupActor>
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class DescribeBackupActorState {
															#line 1927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	DescribeBackupActorState(Reference<BackupContainerFileSystem> const& bc,bool const& deepScan,Version const& logStartVersionOverride) 
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   deepScan(deepScan),
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   logStartVersionOverride(logStartVersionOverride),
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   desc()
															#line 1940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.url = bc->getURL();
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.proxy = bc->getProxy();
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent("BackupContainerDescribe1") .detail("URL", bc->getURL()) .detail("LogStartVersionOverride", logStartVersionOverride);
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<bool> __when_expr_0 = bc->exists();
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DescribeBackupActor*>(this)->actor_wait_state = 1;
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 0, bool >*>(static_cast<DescribeBackupActor*>(this)));
															#line 1968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!e)
															#line 1991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarnAlways, "BackupContainerDoesNotExist").detail("URL", bc->getURL());
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(backup_does_not_exist(), loopDepth);
															#line 1997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (logStartVersionOverride != invalidVersion && logStartVersionOverride < 0)
															#line 2001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<BackupDescription> __when_expr_1 = bc->describeBackup(false, invalidVersion);
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<DescribeBackupActor*>(this)->actor_wait_state = 2;
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 1, BackupDescription >*>(static_cast<DescribeBackupActor*>(this)));
															#line 2012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!e)
															#line 2026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarnAlways, "BackupContainerDoesNotExist").detail("URL", bc->getURL());
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(backup_does_not_exist(), loopDepth);
															#line 2032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (logStartVersionOverride != invalidVersion && logStartVersionOverride < 0)
															#line 2036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<BackupDescription> __when_expr_1 = bc->describeBackup(false, invalidVersion);
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<DescribeBackupActor*>(this)->actor_wait_state = 2;
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 1, BackupDescription >*>(static_cast<DescribeBackupActor*>(this)));
															#line 2047 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaLogBegin = Optional<Version>();
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaLogEnd = Optional<Version>();
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaExpiredEnd = Optional<Version>();
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaUnreliableEnd = Optional<Version>();
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaLogType = Optional<Version>();
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::vector<Future<Void>> metaReads;
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaReads.push_back(store(metaExpiredEnd, bc->expiredEndVersion().get()));
															#line 449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaReads.push_back(store(metaUnreliableEnd, bc->unreliableEndVersion().get()));
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		metaReads.push_back(store(metaLogType, bc->logType().get()));
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!deepScan)
															#line 2142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaReads.push_back(store(metaLogBegin, bc->logBeginVersion().get()));
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaReads.push_back(store(metaLogEnd, bc->logEndVersion().get()));
															#line 2148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = waitForAll(metaReads);
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<DescribeBackupActor*>(this)->actor_wait_state = 3;
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 2, Void >*>(static_cast<DescribeBackupActor*>(this)));
															#line 2159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(BackupDescription const& tmp,int loopDepth) 
	{
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logStartVersionOverride = resolveRelativeVersion( tmp.maxLogEnd, logStartVersionOverride, "LogStartVersionOverride", invalid_option_value());
															#line 2168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(BackupDescription && tmp,int loopDepth) 
	{
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logStartVersionOverride = resolveRelativeVersion( tmp.maxLogEnd, logStartVersionOverride, "LogStartVersionOverride", invalid_option_value());
															#line 2177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent("BackupContainerDescribe2") .detail("URL", bc->getURL()) .detail("LogStartVersionOverride", logStartVersionOverride) .detail("ExpiredEndVersion", metaExpiredEnd.orDefault(invalidVersion)) .detail("UnreliableEndVersion", metaUnreliableEnd.orDefault(invalidVersion)) .detail("LogBeginVersion", metaLogBegin.orDefault(invalidVersion)) .detail("LogEndVersion", metaLogEnd.orDefault(invalidVersion)) .detail("LogType", metaLogType.orDefault(-1));
															#line 471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (logStartVersionOverride != invalidVersion && metaUnreliableEnd.orDefault(invalidVersion) < logStartVersionOverride)
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaUnreliableEnd = logStartVersionOverride;
															#line 2255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!metaLogBegin.present() || !metaLogEnd.present() || metaLogEnd.get() <= metaLogBegin.get() || metaLogEnd.get() < metaExpiredEnd.orDefault(invalidVersion) || metaLogEnd.get() < metaUnreliableEnd.orDefault(invalidVersion))
															#line 2259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarnAlways, "BackupContainerMetadataInvalid") .detail("URL", bc->getURL()) .detail("ExpiredEndVersion", metaExpiredEnd.orDefault(invalidVersion)) .detail("UnreliableEndVersion", metaUnreliableEnd.orDefault(invalidVersion)) .detail("LogBeginVersion", metaLogBegin.orDefault(invalidVersion)) .detail("LogEndVersion", metaLogEnd.orDefault(invalidVersion));
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaLogBegin = Optional<Version>();
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaLogEnd = Optional<Version>();
															#line 2267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!metaUnreliableEnd.present() || metaUnreliableEnd.get() < metaExpiredEnd.orDefault(0))
															#line 2271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaUnreliableEnd = metaExpiredEnd;
															#line 2275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc.unreliableEndVersion = metaUnreliableEnd;
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc.expiredEndVersion = metaExpiredEnd;
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		scanBegin = desc.unreliableEndVersion.orDefault(0);
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		scanEnd = std::numeric_limits<Version>::max();
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (metaLogBegin.present() && metaLogEnd.present())
															#line 2287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.minLogBegin = std::max(metaLogBegin.get(), desc.unreliableEndVersion.orDefault(0));
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.maxLogEnd = metaLogEnd.get();
															#line 520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.contiguousLogEnd = desc.maxLogEnd;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			scanBegin = desc.contiguousLogEnd.get();
															#line 2297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs = std::vector<LogFile>();
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		plogs = std::vector<LogFile>();
															#line 528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent("BackupContainerListFiles").detail("URL", bc->getURL());
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_3 = store(logs, bc->listLogFiles(scanBegin, scanEnd, false)) && store(plogs, bc->listLogFiles(scanBegin, scanEnd, true)) && store(desc.snapshots, bc->listKeyspaceSnapshots());
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<DescribeBackupActor*>(this)->actor_wait_state = 4;
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 3, Void >*>(static_cast<DescribeBackupActor*>(this)));
															#line 2314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent("BackupContainerDescribe2") .detail("URL", bc->getURL()) .detail("LogStartVersionOverride", logStartVersionOverride) .detail("ExpiredEndVersion", metaExpiredEnd.orDefault(invalidVersion)) .detail("UnreliableEndVersion", metaUnreliableEnd.orDefault(invalidVersion)) .detail("LogBeginVersion", metaLogBegin.orDefault(invalidVersion)) .detail("LogEndVersion", metaLogEnd.orDefault(invalidVersion)) .detail("LogType", metaLogType.orDefault(-1));
															#line 471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (logStartVersionOverride != invalidVersion && metaUnreliableEnd.orDefault(invalidVersion) < logStartVersionOverride)
															#line 2325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaUnreliableEnd = logStartVersionOverride;
															#line 2329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!metaLogBegin.present() || !metaLogEnd.present() || metaLogEnd.get() <= metaLogBegin.get() || metaLogEnd.get() < metaExpiredEnd.orDefault(invalidVersion) || metaLogEnd.get() < metaUnreliableEnd.orDefault(invalidVersion))
															#line 2333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarnAlways, "BackupContainerMetadataInvalid") .detail("URL", bc->getURL()) .detail("ExpiredEndVersion", metaExpiredEnd.orDefault(invalidVersion)) .detail("UnreliableEndVersion", metaUnreliableEnd.orDefault(invalidVersion)) .detail("LogBeginVersion", metaLogBegin.orDefault(invalidVersion)) .detail("LogEndVersion", metaLogEnd.orDefault(invalidVersion));
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaLogBegin = Optional<Version>();
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaLogEnd = Optional<Version>();
															#line 2341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!metaUnreliableEnd.present() || metaUnreliableEnd.get() < metaExpiredEnd.orDefault(0))
															#line 2345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			metaUnreliableEnd = metaExpiredEnd;
															#line 2349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc.unreliableEndVersion = metaUnreliableEnd;
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc.expiredEndVersion = metaExpiredEnd;
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		scanBegin = desc.unreliableEndVersion.orDefault(0);
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		scanEnd = std::numeric_limits<Version>::max();
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (metaLogBegin.present() && metaLogEnd.present())
															#line 2361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.minLogBegin = std::max(metaLogBegin.get(), desc.unreliableEndVersion.orDefault(0));
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.maxLogEnd = metaLogEnd.get();
															#line 520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.contiguousLogEnd = desc.maxLogEnd;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			scanBegin = desc.contiguousLogEnd.get();
															#line 2371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs = std::vector<LogFile>();
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		plogs = std::vector<LogFile>();
															#line 528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent("BackupContainerListFiles").detail("URL", bc->getURL());
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_3 = store(logs, bc->listLogFiles(scanBegin, scanEnd, false)) && store(plogs, bc->listLogFiles(scanBegin, scanEnd, true)) && store(desc.snapshots, bc->listKeyspaceSnapshots());
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<DescribeBackupActor*>(this)->actor_wait_state = 4;
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 3, Void >*>(static_cast<DescribeBackupActor*>(this)));
															#line 2388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent("BackupContainerListFiles") .detail("URL", bc->getURL()) .detail("LogFiles", logs.size()) .detail("PLogsFiles", plogs.size()) .detail("Snapshots", desc.snapshots.size());
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (plogs.size() > 0)
															#line 2462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.partitioned = true;
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			logs.swap(plogs);
															#line 2468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
		else
		{
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.partitioned = metaLogType.present() && metaLogType.get() == BackupContainerFileSystemImpl::PARTITIONED_MUTATION_LOG;
															#line 2474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::sort(logs.begin(), logs.end());
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logs.empty())
															#line 2480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.maxLogEnd = logs.rbegin()->endVersion;
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!desc.contiguousLogEnd.present())
															#line 2486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				desc.minLogBegin = logs.begin()->beginVersion;
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (desc.partitioned)
															#line 2492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.contiguousLogEnd = logs.begin()->beginVersion;
															#line 2496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
				else
				{
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.contiguousLogEnd = logs.begin()->endVersion;
															#line 2502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (desc.partitioned)
															#line 2507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				updatePartitionedLogsContinuousEnd(&desc, logs, scanBegin, scanEnd);
															#line 2511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
			else
			{
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				Version& end = desc.contiguousLogEnd.get();
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				computeRestoreEndVersion(logs, nullptr, &end, std::numeric_limits<Version>::max());
															#line 2519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (logStartVersionOverride == invalidVersion)
															#line 2524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			try {
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				updates = Void();
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (desc.minLogBegin.present() && metaLogBegin != desc.minLogBegin)
															#line 2531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					updates = updates && bc->logBeginVersion().set(desc.minLogBegin.get());
															#line 2535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (desc.contiguousLogEnd.present() && metaLogEnd != desc.contiguousLogEnd)
															#line 2539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					updates = updates && bc->logEndVersion().set(desc.contiguousLogEnd.get());
															#line 2543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!metaLogType.present())
															#line 2547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					updates = updates && bc->logType().set(desc.partitioned ? BackupContainerFileSystemImpl::PARTITIONED_MUTATION_LOG : BackupContainerFileSystemImpl::NON_PARTITIONED_MUTATION_LOG);
															#line 2551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Void> __when_expr_4 = updates;
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1cont8Catch1(actor_cancelled(), loopDepth);
															#line 2557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1cont8Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont8when1(__when_expr_4.get(), loopDepth); };
				static_cast<DescribeBackupActor*>(this)->actor_wait_state = 5;
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 4, Void >*>(static_cast<DescribeBackupActor*>(this)));
															#line 2562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent("BackupContainerListFiles") .detail("URL", bc->getURL()) .detail("LogFiles", logs.size()) .detail("PLogsFiles", plogs.size()) .detail("Snapshots", desc.snapshots.size());
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (plogs.size() > 0)
															#line 2584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.partitioned = true;
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			logs.swap(plogs);
															#line 2590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
		else
		{
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.partitioned = metaLogType.present() && metaLogType.get() == BackupContainerFileSystemImpl::PARTITIONED_MUTATION_LOG;
															#line 2596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::sort(logs.begin(), logs.end());
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logs.empty())
															#line 2602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.maxLogEnd = logs.rbegin()->endVersion;
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!desc.contiguousLogEnd.present())
															#line 2608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				desc.minLogBegin = logs.begin()->beginVersion;
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (desc.partitioned)
															#line 2614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.contiguousLogEnd = logs.begin()->beginVersion;
															#line 2618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
				else
				{
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.contiguousLogEnd = logs.begin()->endVersion;
															#line 2624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (desc.partitioned)
															#line 2629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				updatePartitionedLogsContinuousEnd(&desc, logs, scanBegin, scanEnd);
															#line 2633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
			else
			{
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				Version& end = desc.contiguousLogEnd.get();
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				computeRestoreEndVersion(logs, nullptr, &end, std::numeric_limits<Version>::max());
															#line 2641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (logStartVersionOverride == invalidVersion)
															#line 2646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			try {
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				updates = Void();
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (desc.minLogBegin.present() && metaLogBegin != desc.minLogBegin)
															#line 2653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					updates = updates && bc->logBeginVersion().set(desc.minLogBegin.get());
															#line 2657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (desc.contiguousLogEnd.present() && metaLogEnd != desc.contiguousLogEnd)
															#line 2661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					updates = updates && bc->logEndVersion().set(desc.contiguousLogEnd.get());
															#line 2665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!metaLogType.present())
															#line 2669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					updates = updates && bc->logType().set(desc.partitioned ? BackupContainerFileSystemImpl::PARTITIONED_MUTATION_LOG : BackupContainerFileSystemImpl::NON_PARTITIONED_MUTATION_LOG);
															#line 2673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Void> __when_expr_4 = updates;
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<DescribeBackupActor*>(this)->actor_wait_state < 0) return a_body1cont8Catch1(actor_cancelled(), loopDepth);
															#line 2679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1cont8Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont8when1(__when_expr_4.get(), loopDepth); };
				static_cast<DescribeBackupActor*>(this)->actor_wait_state = 5;
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< DescribeBackupActor, 4, Void >*>(static_cast<DescribeBackupActor*>(this)));
															#line 2684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto& s : desc.snapshots ) {
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			s.restorable = true;
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (s.beginVersion != s.endVersion)
															#line 2771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!desc.minLogBegin.present() || desc.minLogBegin.get() > s.beginVersion)
															#line 2775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					s.restorable = false;
															#line 2779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!desc.contiguousLogEnd.present() || desc.contiguousLogEnd.get() <= s.endVersion)
															#line 2783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					s.restorable = false;
															#line 2787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			desc.snapshotBytes += s.totalSize;
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (s.beginVersion == s.endVersion)
															#line 2794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!desc.minRestorableVersion.present() || s.endVersion < desc.minRestorableVersion.get())
															#line 2798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.minRestorableVersion = s.endVersion;
															#line 2802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!desc.maxRestorableVersion.present() || s.endVersion > desc.maxRestorableVersion.get())
															#line 2806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.maxRestorableVersion = s.endVersion;
															#line 2810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (desc.minLogBegin.present() && s.beginVersion >= desc.minLogBegin.get() && s.endVersion < desc.contiguousLogEnd.get())
															#line 2815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!desc.minRestorableVersion.present() || s.endVersion < desc.minRestorableVersion.get())
															#line 2819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.minRestorableVersion = s.endVersion;
															#line 2823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
															#line 636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!desc.maxRestorableVersion.present() || (desc.contiguousLogEnd.get() - 1) > desc.maxRestorableVersion.get())
															#line 2827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					desc.maxRestorableVersion = desc.contiguousLogEnd.get() - 1;
															#line 2831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
		}
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<DescribeBackupActor*>(this)->SAV<BackupDescription>::futures) { (void)(desc); this->~DescribeBackupActorState(); static_cast<DescribeBackupActor*>(this)->destroy(); return 0; }
															#line 2837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 2856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 2860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarn, "BackupContainerMetadataUpdateFailure").error(e).detail("URL", bc->getURL());
															#line 2864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	bool deepScan;
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version logStartVersionOverride;
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	BackupDescription desc;
															#line 441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<Version> metaLogBegin;
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<Version> metaLogEnd;
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<Version> metaExpiredEnd;
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<Version> metaUnreliableEnd;
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<Version> metaLogType;
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version scanBegin;
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version scanEnd;
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> logs;
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> plogs;
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Future<Void> updates;
															#line 2991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via describeBackup()
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class DescribeBackupActor final : public Actor<BackupDescription>, public ActorCallback< DescribeBackupActor, 0, bool >, public ActorCallback< DescribeBackupActor, 1, BackupDescription >, public ActorCallback< DescribeBackupActor, 2, Void >, public ActorCallback< DescribeBackupActor, 3, Void >, public ActorCallback< DescribeBackupActor, 4, Void >, public FastAllocated<DescribeBackupActor>, public DescribeBackupActorState<DescribeBackupActor> {
															#line 2996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	DescribeBackupActor(Reference<BackupContainerFileSystem> const& bc,bool const& deepScan,Version const& logStartVersionOverride) 
															#line 3011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<BackupDescription> describeBackup( Reference<BackupContainerFileSystem> const& bc, bool const& deepScan, Version const& logStartVersionOverride ) {
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<BackupDescription>(new DescribeBackupActor(bc, deepScan, logStartVersionOverride));
															#line 3042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

																#line 3047 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via expireData()
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class ExpireDataActor>
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ExpireDataActorState {
															#line 3053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ExpireDataActorState(Reference<BackupContainerFileSystem> const& bc,Version const& expireEndVersion,bool const& force,IBackupContainer::ExpireProgress* const& progress,Version const& restorableBeginVersion) 
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   expireEndVersion(expireEndVersion),
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   force(force),
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   progress(progress),
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   restorableBeginVersion(restorableBeginVersion)
															#line 3068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (progress != nullptr)
															#line 3083 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				progress->step = "Describing backup";
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				progress->total = 0;
															#line 3089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent("BackupContainerFileSystemExpire1") .detail("URL", bc->getURL()) .detail("ExpireEndVersion", expireEndVersion) .detail("RestorableBeginVersion", restorableBeginVersion);
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<BackupDescription> __when_expr_0 = bc->describeBackup(false, expireEndVersion);
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ExpireDataActor*>(this)->actor_wait_state = 1;
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 0, BackupDescription >*>(static_cast<ExpireDataActor*>(this)));
															#line 3102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		expireEndVersion = resolveRelativeVersion(desc.maxLogEnd, expireEndVersion, "ExpireEndVersion", invalid_option_value());
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		restorableBeginVersion = resolveRelativeVersion( desc.maxLogEnd, restorableBeginVersion, "RestorableBeginVersion", invalid_option_value());
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (restorableBeginVersion < expireEndVersion)
															#line 3129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(backup_cannot_expire(), loopDepth);
															#line 3133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (expireEndVersion <= desc.expiredEndVersion.orDefault(invalidVersion))
															#line 3137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<ExpireDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExpireDataActorState(); static_cast<ExpireDataActor*>(this)->destroy(); return 0; }
															#line 3141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<ExpireDataActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ExpireDataActorState();
			static_cast<ExpireDataActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		forceNeeded = true;
															#line 684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( KeyspaceSnapshotFile& s : desc.snapshots ) {
															#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (s.restorable.orDefault(false) && s.beginVersion >= expireEndVersion && s.endVersion <= restorableBeginVersion)
															#line 3153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				forceNeeded = false;
															#line 3157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				break;
			}
		}
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (forceNeeded && !force)
															#line 3163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(backup_cannot_expire(), loopDepth);
															#line 3167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		scanBegin = desc.expiredEndVersion.orDefault(0);
															#line 702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent("BackupContainerFileSystemExpire2") .detail("URL", bc->getURL()) .detail("ExpireEndVersion", expireEndVersion) .detail("RestorableBeginVersion", restorableBeginVersion) .detail("ScanBeginVersion", scanBegin);
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs = std::vector<LogFile>();
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		pLogs = std::vector<LogFile>();
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ranges = std::vector<RangeFile>();
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (progress != nullptr)
															#line 3181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->step = "Listing files";
															#line 3185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_1 = store(logs, bc->listLogFiles(scanBegin, expireEndVersion - 1, false)) && store(pLogs, bc->listLogFiles(scanBegin, expireEndVersion - 1, true)) && store(ranges, bc->listRangeFiles(scanBegin, expireEndVersion - 1));
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ExpireDataActor*>(this)->actor_wait_state = 2;
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 1, Void >*>(static_cast<ExpireDataActor*>(this)));
															#line 3196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(BackupDescription const& __desc,int loopDepth) 
	{
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc = __desc;
															#line 3205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs.insert(logs.end(), std::make_move_iterator(pLogs.begin()), std::make_move_iterator(pLogs.end()));
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		newLogBeginVersion = Optional<Version>();
															#line 723 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logs.empty())
															#line 3276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			LogFile& last = *std::max_element(logs.begin(), logs.end());
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (last.endVersion == expireEndVersion)
															#line 3282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				newLogBeginVersion = expireEndVersion;
															#line 3286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
			else
			{
															#line 733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (last.endVersion > expireEndVersion)
															#line 3292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					newLogBeginVersion = last.beginVersion;
															#line 737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					last = LogFile();
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					expireEndVersion = newLogBeginVersion.get();
															#line 3300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
		}
															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		toDelete = std::vector<std::string>();
															#line 748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& f : logs ) {
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!f.fileName.empty())
															#line 3310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				toDelete.push_back(std::move(f.fileName));
															#line 3314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs.clear();
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& f : ranges ) {
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (f.version < expireEndVersion)
															#line 3323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				toDelete.push_back(std::move(f.fileName));
															#line 3327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ranges.clear();
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& f : desc.snapshots ) {
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (f.endVersion < expireEndVersion)
															#line 3336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				toDelete.push_back(std::move(f.fileName));
															#line 3340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc = BackupDescription();
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (progress != nullptr)
															#line 3347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->step = "Initial metadata update";
															#line 3351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Optional<Version>> __when_expr_2 = bc->unreliableEndVersion().get();
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ExpireDataActor*>(this)->actor_wait_state = 3;
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 2, Optional<Version> >*>(static_cast<ExpireDataActor*>(this)));
															#line 3362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs.insert(logs.end(), std::make_move_iterator(pLogs.begin()), std::make_move_iterator(pLogs.end()));
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		newLogBeginVersion = Optional<Version>();
															#line 723 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logs.empty())
															#line 3375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			LogFile& last = *std::max_element(logs.begin(), logs.end());
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (last.endVersion == expireEndVersion)
															#line 3381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				newLogBeginVersion = expireEndVersion;
															#line 3385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
			else
			{
															#line 733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (last.endVersion > expireEndVersion)
															#line 3391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					newLogBeginVersion = last.beginVersion;
															#line 737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					last = LogFile();
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					expireEndVersion = newLogBeginVersion.get();
															#line 3399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
		}
															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		toDelete = std::vector<std::string>();
															#line 748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& f : logs ) {
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!f.fileName.empty())
															#line 3409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				toDelete.push_back(std::move(f.fileName));
															#line 3413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs.clear();
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& f : ranges ) {
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (f.version < expireEndVersion)
															#line 3422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				toDelete.push_back(std::move(f.fileName));
															#line 3426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ranges.clear();
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto const& f : desc.snapshots ) {
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (f.endVersion < expireEndVersion)
															#line 3435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				toDelete.push_back(std::move(f.fileName));
															#line 3439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc = BackupDescription();
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (progress != nullptr)
															#line 3446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->step = "Initial metadata update";
															#line 3450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Optional<Version>> __when_expr_2 = bc->unreliableEndVersion().get();
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ExpireDataActor*>(this)->actor_wait_state = 3;
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 2, Optional<Version> >*>(static_cast<ExpireDataActor*>(this)));
															#line 3461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (metaUnreliableEnd.orDefault(0) < expireEndVersion)
															#line 3533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_3 = bc->unreliableEndVersion().set(expireEndVersion);
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont10when1(__when_expr_3.get(), loopDepth); };
			static_cast<ExpireDataActor*>(this)->actor_wait_state = 4;
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 3, Void >*>(static_cast<ExpireDataActor*>(this)));
															#line 3544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (metaUnreliableEnd.orDefault(0) < expireEndVersion)
															#line 3558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_3 = bc->unreliableEndVersion().set(expireEndVersion);
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont10when1(__when_expr_3.get(), loopDepth); };
			static_cast<ExpireDataActor*>(this)->actor_wait_state = 4;
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 3, Void >*>(static_cast<ExpireDataActor*>(this)));
															#line 3569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (progress != nullptr)
															#line 3646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->step = "Deleting files";
															#line 787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->total = toDelete.size();
															#line 788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->done = 0;
															#line 3654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		deleteFutures = std::list<Future<Void>>();
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 3660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (progress != nullptr)
															#line 3744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->step = "Final metadata update";
															#line 819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			progress->total = 0;
															#line 3750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Optional<Version>> __when_expr_5 = bc->expiredEndVersion().get();
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont10cont3when1(__when_expr_5.get(), loopDepth); };
		static_cast<ExpireDataActor*>(this)->actor_wait_state = 6;
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 5, Optional<Version> >*>(static_cast<ExpireDataActor*>(this)));
															#line 3761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(!toDelete.empty() || !deleteFutures.empty()))
															#line 3777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont10cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for(;!toDelete.empty() && deleteFutures.size() < CLIENT_KNOBS->BACKUP_CONCURRENT_DELETES;) {
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			deleteFutures.push_back(bc->deleteFile(toDelete.back()));
															#line 800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			toDelete.pop_back();
															#line 3787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		targetFuturesSize = toDelete.empty() ? 0 : (CLIENT_KNOBS->BACKUP_CONCURRENT_DELETES - 1);
															#line 808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 3793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(deleteFutures.size() > targetFuturesSize))
															#line 3828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont10cont1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_4 = deleteFutures.front();
															#line 809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 3836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 2)); else return a_body1cont10cont1loopBody1loopBody1when1(__when_expr_4.get(), loopDepth); };
		static_cast<ExpireDataActor*>(this)->actor_wait_state = 5;
															#line 809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 4, Void >*>(static_cast<ExpireDataActor*>(this)));
															#line 3841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (progress != nullptr)
															#line 3863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			++progress->done;
															#line 3867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		deleteFutures.pop_front();
															#line 3871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont10cont1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (progress != nullptr)
															#line 3880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			++progress->done;
															#line 3884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		deleteFutures.pop_front();
															#line 3888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (metaExpiredEnd.orDefault(0) < expireEndVersion)
															#line 3960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_6 = bc->expiredEndVersion().set(expireEndVersion);
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont10cont5when1(__when_expr_6.get(), loopDepth); };
			static_cast<ExpireDataActor*>(this)->actor_wait_state = 7;
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 6, Void >*>(static_cast<ExpireDataActor*>(this)));
															#line 3971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (metaExpiredEnd.orDefault(0) < expireEndVersion)
															#line 3985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_6 = bc->expiredEndVersion().set(expireEndVersion);
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<ExpireDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont10cont5when1(__when_expr_6.get(), loopDepth); };
			static_cast<ExpireDataActor*>(this)->actor_wait_state = 7;
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< ExpireDataActor, 6, Void >*>(static_cast<ExpireDataActor*>(this)));
															#line 3996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<ExpireDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExpireDataActorState(); static_cast<ExpireDataActor*>(this)->destroy(); return 0; }
															#line 4073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version expireEndVersion;
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	bool force;
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	IBackupContainer::ExpireProgress* progress;
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version restorableBeginVersion;
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	BackupDescription desc;
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	bool forceNeeded;
															#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version scanBegin;
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> logs;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> pLogs;
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<RangeFile> ranges;
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<Version> newLogBeginVersion;
															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<std::string> toDelete;
															#line 793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::list<Future<Void>> deleteFutures;
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int targetFuturesSize;
															#line 4186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via expireData()
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ExpireDataActor final : public Actor<Void>, public ActorCallback< ExpireDataActor, 0, BackupDescription >, public ActorCallback< ExpireDataActor, 1, Void >, public ActorCallback< ExpireDataActor, 2, Optional<Version> >, public ActorCallback< ExpireDataActor, 3, Void >, public ActorCallback< ExpireDataActor, 4, Void >, public ActorCallback< ExpireDataActor, 5, Optional<Version> >, public ActorCallback< ExpireDataActor, 6, Void >, public FastAllocated<ExpireDataActor>, public ExpireDataActorState<ExpireDataActor> {
															#line 4191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ExpireDataActor(Reference<BackupContainerFileSystem> const& bc,Version const& expireEndVersion,bool const& force,IBackupContainer::ExpireProgress* const& progress,Version const& restorableBeginVersion) 
															#line 4208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Void> expireData( Reference<BackupContainerFileSystem> const& bc, Version const& expireEndVersion, bool const& force, IBackupContainer::ExpireProgress* const& progress, Version const& restorableBeginVersion ) {
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new ExpireDataActor(bc, expireEndVersion, force, progress, restorableBeginVersion));
															#line 4241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

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
																#line 4326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via getRestoreSet()
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class GetRestoreSetActor>
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class GetRestoreSetActorState {
															#line 4332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	GetRestoreSetActorState(Reference<BackupContainerFileSystem> const& bc,Version const& targetVersion,VectorRef<KeyRangeRef> const& keyRangesFilter,bool const& logsOnly = false,Version const& beginVersion = invalidVersion) 
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   targetVersion(targetVersion),
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   keyRangesFilter(keyRangesFilter),
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   logsOnly(logsOnly),
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   beginVersion(beginVersion)
															#line 4347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			for( const auto& range : keyRangesFilter ) {
															#line 917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent("BackupContainerGetRestoreSet").detail("RangeFilter", printable(range));
															#line 4364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (logsOnly)
															#line 4368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				restorableSet = RestorableFileSet();
															#line 922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				restorableSet.targetVersion = targetVersion;
															#line 923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				logFiles = std::vector<LogFile>();
															#line 924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				Version begin = beginVersion == invalidVersion ? 0 : beginVersion;
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Void> __when_expr_0 = store(logFiles, bc->listLogFiles(begin, targetVersion, false));
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 1;
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetRestoreSetActor, 0, Void >*>(static_cast<GetRestoreSetActor*>(this)));
															#line 4387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<std::vector<KeyspaceSnapshotFile>> __when_expr_1 = bc->listKeyspaceSnapshots();
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 2;
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetRestoreSetActor, 1, std::vector<KeyspaceSnapshotFile> >*>(static_cast<GetRestoreSetActor*>(this)));
															#line 4422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::sort(logFiles.begin(), logFiles.end());
															#line 928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logFiles.empty())
															#line 4433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(getRestoreSetFromLogs(logFiles, targetVersion, restorableSet)); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 4437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::sort(logFiles.begin(), logFiles.end());
															#line 928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logFiles.empty())
															#line 4453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(getRestoreSetFromLogs(logFiles, targetVersion, restorableSet)); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 4457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		i = snapshots.size() - 1;
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 4536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont6loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(std::vector<KeyspaceSnapshotFile> const& __snapshots,int loopDepth) 
	{
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshots = __snapshots;
															#line 4545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(Optional<RestorableFileSet>()); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 4612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(i >= 0))
															#line 4631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont6break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (targetVersion >= 0 && snapshots[i].beginVersion > targetVersion)
															#line 4637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont6continue1(loopDepth); // continue
		}
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		restorable = RestorableFileSet();
															#line 943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		minKeyRangeVersion = MAX_VERSION;
															#line 944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		maxKeyRangeVersion = -1;
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>>> __when_expr_2 = bc->readKeyspaceSnapshot(snapshots[i]);
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont6loopBody1when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 3;
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetRestoreSetActor, 2, std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> >*>(static_cast<GetRestoreSetActor*>(this)));
															#line 4656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		i--;
															#line 4678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont6loopHead1(0);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1(std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> const& results,int loopDepth) 
	{
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (keyRangesFilter.empty() || results.second.empty())
															#line 4687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			restorable.ranges = std::move(results.first);
															#line 953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			restorable.keyRanges = std::move(results.second);
															#line 954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			minKeyRangeVersion = snapshots[i].beginVersion;
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			maxKeyRangeVersion = snapshots[i].endVersion;
															#line 4697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
		else
		{
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			for( const auto& rangeFile : results.first ) {
															#line 958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				const auto& keyRange = results.second.at(rangeFile.fileName);
															#line 959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (keyRange.intersects(keyRangesFilter))
															#line 4707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					restorable.ranges.push_back(rangeFile);
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					restorable.keyRanges[rangeFile.fileName] = keyRange;
															#line 962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					minKeyRangeVersion = std::min(minKeyRangeVersion, rangeFile.version);
															#line 963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					maxKeyRangeVersion = std::max(maxKeyRangeVersion, rangeFile.version);
															#line 4717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (restorable.ranges.empty())
															#line 4722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(backup_not_overlapped_with_keys_filter(), std::max(0, loopDepth - 1));
															#line 4726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		restorable.targetVersion = targetVersion == latestVersion ? maxKeyRangeVersion : targetVersion;
															#line 974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (restorable.targetVersion < maxKeyRangeVersion)
															#line 4733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont6continue1(loopDepth); // continue
		}
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		restorable.snapshot = snapshots[i];
															#line 980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (minKeyRangeVersion == maxKeyRangeVersion && maxKeyRangeVersion == restorable.targetVersion)
															#line 4741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			restorable.continuousBeginVersion = restorable.continuousEndVersion = invalidVersion;
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent("BackupContainerGetRestorableFilesWithoutLogs") .detail("KeyRangeVersion", restorable.targetVersion) .detail("NumberOfRangeFiles", restorable.ranges.size()) .detail("KeyRangesFilter", printable(keyRangesFilter));
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(Optional<RestorableFileSet>(restorable)); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 4749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(Optional<RestorableFileSet>(restorable));
			this->~GetRestoreSetActorState();
			static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs = std::vector<LogFile>();
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		plogs = std::vector<LogFile>();
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_3 = store(logs, bc->listLogFiles(minKeyRangeVersion, restorable.targetVersion, false)) && store(plogs, bc->listLogFiles(minKeyRangeVersion, restorable.targetVersion, true));
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1cont6loopBody1cont1when1(__when_expr_3.get(), loopDepth); };
		static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 4;
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GetRestoreSetActor, 3, Void >*>(static_cast<GetRestoreSetActor*>(this)));
															#line 4768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1(std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> && results,int loopDepth) 
	{
															#line 951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (keyRangesFilter.empty() || results.second.empty())
															#line 4777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			restorable.ranges = std::move(results.first);
															#line 953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			restorable.keyRanges = std::move(results.second);
															#line 954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			minKeyRangeVersion = snapshots[i].beginVersion;
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			maxKeyRangeVersion = snapshots[i].endVersion;
															#line 4787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
		else
		{
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			for( const auto& rangeFile : results.first ) {
															#line 958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				const auto& keyRange = results.second.at(rangeFile.fileName);
															#line 959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (keyRange.intersects(keyRangesFilter))
															#line 4797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					restorable.ranges.push_back(rangeFile);
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					restorable.keyRanges[rangeFile.fileName] = keyRange;
															#line 962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					minKeyRangeVersion = std::min(minKeyRangeVersion, rangeFile.version);
															#line 963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					maxKeyRangeVersion = std::max(maxKeyRangeVersion, rangeFile.version);
															#line 4807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				}
			}
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (restorable.ranges.empty())
															#line 4812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(backup_not_overlapped_with_keys_filter(), std::max(0, loopDepth - 1));
															#line 4816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		restorable.targetVersion = targetVersion == latestVersion ? maxKeyRangeVersion : targetVersion;
															#line 974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (restorable.targetVersion < maxKeyRangeVersion)
															#line 4823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont6continue1(loopDepth); // continue
		}
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		restorable.snapshot = snapshots[i];
															#line 980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (minKeyRangeVersion == maxKeyRangeVersion && maxKeyRangeVersion == restorable.targetVersion)
															#line 4831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			restorable.continuousBeginVersion = restorable.continuousEndVersion = invalidVersion;
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent("BackupContainerGetRestorableFilesWithoutLogs") .detail("KeyRangeVersion", restorable.targetVersion) .detail("NumberOfRangeFiles", restorable.ranges.size()) .detail("KeyRangesFilter", printable(keyRangesFilter));
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(Optional<RestorableFileSet>(restorable)); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 4839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(Optional<RestorableFileSet>(restorable));
			this->~GetRestoreSetActorState();
			static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs = std::vector<LogFile>();
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		plogs = std::vector<LogFile>();
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_3 = store(logs, bc->listLogFiles(minKeyRangeVersion, restorable.targetVersion, false)) && store(plogs, bc->listLogFiles(minKeyRangeVersion, restorable.targetVersion, true));
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1cont6loopBody1cont1when1(__when_expr_3.get(), loopDepth); };
		static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 4;
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GetRestoreSetActor, 3, Void >*>(static_cast<GetRestoreSetActor*>(this)));
															#line 4858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

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
	int a_body1cont6loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (plogs.size() > 0)
															#line 4930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			logs.swap(plogs);
															#line 998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(logs.begin(), logs.end(), [](const LogFile& a, const LogFile& b) { return std::tie(a.tagId, a.beginVersion, a.endVersion) < std::tie(b.tagId, b.beginVersion, b.endVersion); });
															#line 1004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::vector<LogFile> filtered = filterDuplicates(logs);
															#line 1005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			restorable.logs.swap(filtered);
															#line 1007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(restorable.logs.begin(), restorable.logs.end());
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (isPartitionedLogsContinuous(restorable.logs, minKeyRangeVersion, restorable.targetVersion))
															#line 4944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				restorable.continuousBeginVersion = minKeyRangeVersion;
															#line 1010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				restorable.continuousEndVersion = restorable.targetVersion + 1;
															#line 1011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(Optional<RestorableFileSet>(restorable)); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 4952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(Optional<RestorableFileSet>(restorable));
				this->~GetRestoreSetActorState();
				static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(Optional<RestorableFileSet>()); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 4960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(Optional<RestorableFileSet>());
			this->~GetRestoreSetActorState();
			static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::sort(logs.begin(), logs.end());
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logs.empty() && logs.front().beginVersion <= minKeyRangeVersion)
															#line 4970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(getRestoreSetFromLogs(logs, targetVersion, restorable)); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 4974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(getRestoreSetFromLogs(logs, targetVersion, restorable));
			this->~GetRestoreSetActorState();
			static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		i--;
															#line 4982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont6loopHead1(0);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (plogs.size() > 0)
															#line 4991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			logs.swap(plogs);
															#line 998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(logs.begin(), logs.end(), [](const LogFile& a, const LogFile& b) { return std::tie(a.tagId, a.beginVersion, a.endVersion) < std::tie(b.tagId, b.beginVersion, b.endVersion); });
															#line 1004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::vector<LogFile> filtered = filterDuplicates(logs);
															#line 1005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			restorable.logs.swap(filtered);
															#line 1007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(restorable.logs.begin(), restorable.logs.end());
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (isPartitionedLogsContinuous(restorable.logs, minKeyRangeVersion, restorable.targetVersion))
															#line 5005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				restorable.continuousBeginVersion = minKeyRangeVersion;
															#line 1010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				restorable.continuousEndVersion = restorable.targetVersion + 1;
															#line 1011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(Optional<RestorableFileSet>(restorable)); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 5013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(Optional<RestorableFileSet>(restorable));
				this->~GetRestoreSetActorState();
				static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(Optional<RestorableFileSet>()); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 5021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(Optional<RestorableFileSet>());
			this->~GetRestoreSetActorState();
			static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::sort(logs.begin(), logs.end());
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!logs.empty() && logs.front().beginVersion <= minKeyRangeVersion)
															#line 5031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<GetRestoreSetActor*>(this)->SAV<Optional<RestorableFileSet>>::futures) { (void)(getRestoreSetFromLogs(logs, targetVersion, restorable)); this->~GetRestoreSetActorState(); static_cast<GetRestoreSetActor*>(this)->destroy(); return 0; }
															#line 5035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<GetRestoreSetActor*>(this)->SAV< Optional<RestorableFileSet> >::value()) Optional<RestorableFileSet>(getRestoreSetFromLogs(logs, targetVersion, restorable));
			this->~GetRestoreSetActorState();
			static_cast<GetRestoreSetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		i--;
															#line 5043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont6loopHead1(0);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<GetRestoreSetActor*>(this)->actor_wait_state > 0) static_cast<GetRestoreSetActor*>(this)->actor_wait_state = 0;
		static_cast<GetRestoreSetActor*>(this)->ActorCallback< GetRestoreSetActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetRestoreSetActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< GetRestoreSetActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getRestoreSet", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRestoreSet", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< GetRestoreSetActor, 3, Void >*,Error err) 
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
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version targetVersion;
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	VectorRef<KeyRangeRef> keyRangesFilter;
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	bool logsOnly;
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version beginVersion;
															#line 921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	RestorableFileSet restorableSet;
															#line 923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> logFiles;
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<KeyspaceSnapshotFile> snapshots;
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int i;
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	RestorableFileSet restorable;
															#line 943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version minKeyRangeVersion;
															#line 944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version maxKeyRangeVersion;
															#line 990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> logs;
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<LogFile> plogs;
															#line 5139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via getRestoreSet()
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class GetRestoreSetActor final : public Actor<Optional<RestorableFileSet>>, public ActorCallback< GetRestoreSetActor, 0, Void >, public ActorCallback< GetRestoreSetActor, 1, std::vector<KeyspaceSnapshotFile> >, public ActorCallback< GetRestoreSetActor, 2, std::pair<std::vector<RangeFile>, std::map<std::string, KeyRange>> >, public ActorCallback< GetRestoreSetActor, 3, Void >, public FastAllocated<GetRestoreSetActor>, public GetRestoreSetActorState<GetRestoreSetActor> {
															#line 5144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
friend struct ActorCallback< GetRestoreSetActor, 3, Void >;
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	GetRestoreSetActor(Reference<BackupContainerFileSystem> const& bc,Version const& targetVersion,VectorRef<KeyRangeRef> const& keyRangesFilter,bool const& logsOnly = false,Version const& beginVersion = invalidVersion) 
															#line 5158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		case 4: this->a_callback_error((ActorCallback< GetRestoreSetActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Optional<RestorableFileSet>> getRestoreSet( Reference<BackupContainerFileSystem> const& bc, Version const& targetVersion, VectorRef<KeyRangeRef> const& keyRangesFilter, bool const& logsOnly = false, Version const& beginVersion = invalidVersion ) {
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Optional<RestorableFileSet>>(new GetRestoreSetActor(bc, targetVersion, keyRangesFilter, logsOnly, beginVersion));
															#line 5188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

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

	// fallback for using existing write api if the underlying blob store doesn't support efficient writeEntireFile
																#line 5289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via writeEntireFileFallback()
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class WriteEntireFileFallbackActor>
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class WriteEntireFileFallbackActorState {
															#line 5295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	WriteEntireFileFallbackActorState(Reference<BackupContainerFileSystem> const& bc,std::string const& fileName,std::string const& fileContents) 
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   fileName(fileName),
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   fileContents(fileContents)
															#line 5306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("writeEntireFileFallback", reinterpret_cast<unsigned long>(this));

	}
	~WriteEntireFileFallbackActorState() 
	{
		fdb_probe_actor_destroy("writeEntireFileFallback", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Reference<IBackupFile>> __when_expr_0 = bc->writeFile(fileName);
															#line 1126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<WriteEntireFileFallbackActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WriteEntireFileFallbackActor*>(this)->actor_wait_state = 1;
															#line 1126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WriteEntireFileFallbackActor, 0, Reference<IBackupFile> >*>(static_cast<WriteEntireFileFallbackActor*>(this)));
															#line 5328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~WriteEntireFileFallbackActorState();
		static_cast<WriteEntireFileFallbackActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_1 = objectFile->append(&fileContents[0], fileContents.size());
															#line 1127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteEntireFileFallbackActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WriteEntireFileFallbackActor*>(this)->actor_wait_state = 2;
															#line 1127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WriteEntireFileFallbackActor, 1, Void >*>(static_cast<WriteEntireFileFallbackActor*>(this)));
															#line 5358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IBackupFile> const& __objectFile,int loopDepth) 
	{
															#line 1126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		objectFile = __objectFile;
															#line 5367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IBackupFile> && __objectFile,int loopDepth) 
	{
		objectFile = std::move(__objectFile);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WriteEntireFileFallbackActor*>(this)->actor_wait_state > 0) static_cast<WriteEntireFileFallbackActor*>(this)->actor_wait_state = 0;
		static_cast<WriteEntireFileFallbackActor*>(this)->ActorCallback< WriteEntireFileFallbackActor, 0, Reference<IBackupFile> >::remove();

	}
	void a_callback_fire(ActorCallback< WriteEntireFileFallbackActor, 0, Reference<IBackupFile> >*,Reference<IBackupFile> const& value) 
	{
		fdb_probe_actor_enter("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WriteEntireFileFallbackActor, 0, Reference<IBackupFile> >*,Reference<IBackupFile> && value) 
	{
		fdb_probe_actor_enter("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WriteEntireFileFallbackActor, 0, Reference<IBackupFile> >*,Error err) 
	{
		fdb_probe_actor_enter("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 1128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = objectFile->finish();
															#line 1128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteEntireFileFallbackActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteEntireFileFallbackActor*>(this)->actor_wait_state = 3;
															#line 1128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteEntireFileFallbackActor, 2, Void >*>(static_cast<WriteEntireFileFallbackActor*>(this)));
															#line 5441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = objectFile->finish();
															#line 1128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteEntireFileFallbackActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteEntireFileFallbackActor*>(this)->actor_wait_state = 3;
															#line 1128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteEntireFileFallbackActor, 2, Void >*>(static_cast<WriteEntireFileFallbackActor*>(this)));
															#line 5457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		if (static_cast<WriteEntireFileFallbackActor*>(this)->actor_wait_state > 0) static_cast<WriteEntireFileFallbackActor*>(this)->actor_wait_state = 0;
		static_cast<WriteEntireFileFallbackActor*>(this)->ActorCallback< WriteEntireFileFallbackActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteEntireFileFallbackActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WriteEntireFileFallbackActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WriteEntireFileFallbackActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 1129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<WriteEntireFileFallbackActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteEntireFileFallbackActorState(); static_cast<WriteEntireFileFallbackActor*>(this)->destroy(); return 0; }
															#line 5529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<WriteEntireFileFallbackActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteEntireFileFallbackActorState();
		static_cast<WriteEntireFileFallbackActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 1129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<WriteEntireFileFallbackActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteEntireFileFallbackActorState(); static_cast<WriteEntireFileFallbackActor*>(this)->destroy(); return 0; }
															#line 5541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<WriteEntireFileFallbackActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteEntireFileFallbackActorState();
		static_cast<WriteEntireFileFallbackActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<WriteEntireFileFallbackActor*>(this)->actor_wait_state > 0) static_cast<WriteEntireFileFallbackActor*>(this)->actor_wait_state = 0;
		static_cast<WriteEntireFileFallbackActor*>(this)->ActorCallback< WriteEntireFileFallbackActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteEntireFileFallbackActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< WriteEntireFileFallbackActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< WriteEntireFileFallbackActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string fileName;
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string fileContents;
															#line 1126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IBackupFile> objectFile;
															#line 5620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via writeEntireFileFallback()
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class WriteEntireFileFallbackActor final : public Actor<Void>, public ActorCallback< WriteEntireFileFallbackActor, 0, Reference<IBackupFile> >, public ActorCallback< WriteEntireFileFallbackActor, 1, Void >, public ActorCallback< WriteEntireFileFallbackActor, 2, Void >, public FastAllocated<WriteEntireFileFallbackActor>, public WriteEntireFileFallbackActorState<WriteEntireFileFallbackActor> {
															#line 5625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<WriteEntireFileFallbackActor>::operator new;
	using FastAllocated<WriteEntireFileFallbackActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WriteEntireFileFallbackActor, 0, Reference<IBackupFile> >;
friend struct ActorCallback< WriteEntireFileFallbackActor, 1, Void >;
friend struct ActorCallback< WriteEntireFileFallbackActor, 2, Void >;
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	WriteEntireFileFallbackActor(Reference<BackupContainerFileSystem> const& bc,std::string const& fileName,std::string const& fileContents) 
															#line 5638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   WriteEntireFileFallbackActorState<WriteEntireFileFallbackActor>(bc, fileName, fileContents)
	{
		fdb_probe_actor_enter("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("writeEntireFileFallback");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("writeEntireFileFallback", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WriteEntireFileFallbackActor, 0, Reference<IBackupFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WriteEntireFileFallbackActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< WriteEntireFileFallbackActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Void> writeEntireFileFallback( Reference<BackupContainerFileSystem> const& bc, std::string const& fileName, std::string const& fileContents ) {
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new WriteEntireFileFallbackActor(bc, fileName, fileContents));
															#line 5667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

																#line 5672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via createTestEncryptionKeyFile()
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class CreateTestEncryptionKeyFileActor>
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class CreateTestEncryptionKeyFileActorState {
															#line 5678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	CreateTestEncryptionKeyFileActorState(std::string const& filename) 
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : filename(filename)
															#line 5685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Reference<IAsyncFile>> __when_expr_0 = IAsyncFileSystem::filesystem()->open( filename, IAsyncFile::OPEN_ATOMIC_WRITE_AND_CREATE | IAsyncFile::OPEN_READWRITE | IAsyncFile::OPEN_CREATE, 0600);
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<CreateTestEncryptionKeyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CreateTestEncryptionKeyFileActor*>(this)->actor_wait_state = 1;
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CreateTestEncryptionKeyFileActor, 0, Reference<IAsyncFile> >*>(static_cast<CreateTestEncryptionKeyFileActor*>(this)));
															#line 5707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StreamCipherKey testKey(AES_256_KEY_LENGTH);
															#line 1138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		testKey.initializeRandomTestKey();
															#line 1139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		keyFile->write(testKey.data(), testKey.size(), 0);
															#line 1140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_1 = keyFile->sync();
															#line 1140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<CreateTestEncryptionKeyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CreateTestEncryptionKeyFileActor*>(this)->actor_wait_state = 2;
															#line 1140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CreateTestEncryptionKeyFileActor, 1, Void >*>(static_cast<CreateTestEncryptionKeyFileActor*>(this)));
															#line 5743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> const& __keyFile,int loopDepth) 
	{
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		keyFile = __keyFile;
															#line 5752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<CreateTestEncryptionKeyFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CreateTestEncryptionKeyFileActorState(); static_cast<CreateTestEncryptionKeyFileActor*>(this)->destroy(); return 0; }
															#line 5819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<CreateTestEncryptionKeyFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CreateTestEncryptionKeyFileActorState();
		static_cast<CreateTestEncryptionKeyFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<CreateTestEncryptionKeyFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CreateTestEncryptionKeyFileActorState(); static_cast<CreateTestEncryptionKeyFileActor*>(this)->destroy(); return 0; }
															#line 5831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string filename;
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IAsyncFile> keyFile;
															#line 5906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via createTestEncryptionKeyFile()
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class CreateTestEncryptionKeyFileActor final : public Actor<Void>, public ActorCallback< CreateTestEncryptionKeyFileActor, 0, Reference<IAsyncFile> >, public ActorCallback< CreateTestEncryptionKeyFileActor, 1, Void >, public FastAllocated<CreateTestEncryptionKeyFileActor>, public CreateTestEncryptionKeyFileActorState<CreateTestEncryptionKeyFileActor> {
															#line 5911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<CreateTestEncryptionKeyFileActor>::operator new;
	using FastAllocated<CreateTestEncryptionKeyFileActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CreateTestEncryptionKeyFileActor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< CreateTestEncryptionKeyFileActor, 1, Void >;
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	CreateTestEncryptionKeyFileActor(std::string const& filename) 
															#line 5923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Void> createTestEncryptionKeyFile( std::string const& filename ) {
															#line 1132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new CreateTestEncryptionKeyFileActor(filename));
															#line 5951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

																#line 5956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via readEncryptionKey()
															#line 1144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class ReadEncryptionKeyActor>
															#line 1144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ReadEncryptionKeyActorState {
															#line 5962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ReadEncryptionKeyActorState(std::string const& encryptionKeyFileName) 
															#line 1144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : encryptionKeyFileName(encryptionKeyFileName),
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   keyFile(),
															#line 1146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   cipherKey(StreamCipherKey::getGlobalCipherKey())
															#line 5973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Reference<IAsyncFile>> __when_expr_0 = IAsyncFileSystem::filesystem()->open(encryptionKeyFileName, 0x0, 0400);
															#line 1148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<ReadEncryptionKeyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 5991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ReadEncryptionKeyActor*>(this)->actor_wait_state = 1;
															#line 1148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadEncryptionKeyActor, 0, Reference<IAsyncFile> >*>(static_cast<ReadEncryptionKeyActor*>(this)));
															#line 5996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<int> __when_expr_1 = keyFile->read(cipherKey->data(), cipherKey->size(), 0);
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadEncryptionKeyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadEncryptionKeyActor*>(this)->actor_wait_state = 2;
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadEncryptionKeyActor, 1, int >*>(static_cast<ReadEncryptionKeyActor*>(this)));
															#line 6032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarnAlways, "FailedToOpenEncryptionKeyFile") .error(e) .detail("FileName", encryptionKeyFileName);
															#line 1155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 6044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		keyFile = _keyFile;
															#line 6058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Reference<IAsyncFile> && _keyFile,int loopDepth) 
	{
															#line 1150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		keyFile = _keyFile;
															#line 6067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (bytesRead != cipherKey->size())
															#line 6152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarnAlways, "InvalidEncryptionKeyFileSize") .detail("ExpectedSize", cipherKey->size()) .detail("ActualSize", bytesRead);
															#line 1162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(invalid_encryption_key_file(), loopDepth);
															#line 6158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(bytesRead, cipherKey->size());
															#line 1165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<ReadEncryptionKeyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadEncryptionKeyActorState(); static_cast<ReadEncryptionKeyActor*>(this)->destroy(); return 0; }
															#line 6164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<ReadEncryptionKeyActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ReadEncryptionKeyActorState();
		static_cast<ReadEncryptionKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5(int && bytesRead,int loopDepth) 
	{
															#line 1158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (bytesRead != cipherKey->size())
															#line 6176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarnAlways, "InvalidEncryptionKeyFileSize") .detail("ExpectedSize", cipherKey->size()) .detail("ActualSize", bytesRead);
															#line 1162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(invalid_encryption_key_file(), loopDepth);
															#line 6182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(bytesRead, cipherKey->size());
															#line 1165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<ReadEncryptionKeyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadEncryptionKeyActorState(); static_cast<ReadEncryptionKeyActor*>(this)->destroy(); return 0; }
															#line 6188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string encryptionKeyFileName;
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IAsyncFile> keyFile;
															#line 1146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	StreamCipherKey const* cipherKey;
															#line 6265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via readEncryptionKey()
															#line 1144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ReadEncryptionKeyActor final : public Actor<Void>, public ActorCallback< ReadEncryptionKeyActor, 0, Reference<IAsyncFile> >, public ActorCallback< ReadEncryptionKeyActor, 1, int >, public FastAllocated<ReadEncryptionKeyActor>, public ReadEncryptionKeyActorState<ReadEncryptionKeyActor> {
															#line 6270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<ReadEncryptionKeyActor>::operator new;
	using FastAllocated<ReadEncryptionKeyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadEncryptionKeyActor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< ReadEncryptionKeyActor, 1, int >;
															#line 1144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ReadEncryptionKeyActor(std::string const& encryptionKeyFileName) 
															#line 6282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Void> readEncryptionKey( std::string const& encryptionKeyFileName ) {
															#line 1144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new ReadEncryptionKeyActor(encryptionKeyFileName));
															#line 6310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

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
                                                                  int64_t totalBytes,
                                                                  IncludeKeyRangeMap includeKeyRangeMap) {
	return BackupContainerFileSystemImpl::writeKeyspaceSnapshotFile(
	    Reference<BackupContainerFileSystem>::addRef(this), fileNames, beginEndKeys, totalBytes, includeKeyRangeMap);
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

															#line 6499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
namespace {
// This generated class is to be used only via getSnapshotFileKeyRange_impl()
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class GetSnapshotFileKeyRange_implActor>
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class GetSnapshotFileKeyRange_implActorState {
															#line 6506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	GetSnapshotFileKeyRange_implActorState(Reference<BackupContainerFileSystem> const& bc,RangeFile const& file,Database const& cx) 
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   file(file),
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   cx(cx),
															#line 1355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   readFileRetries(0),
															#line 1356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   beginKeySet(false),
															#line 1357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   beginKey(),
															#line 1358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   endKey()
															#line 6525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			;
															#line 6540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<GetSnapshotFileKeyRange_implActor*>(this)->SAV<KeyRange>::futures) { (void)(KeyRange(KeyRangeRef(beginKey, endKey))); this->~GetSnapshotFileKeyRange_implActorState(); static_cast<GetSnapshotFileKeyRange_implActor*>(this)->destroy(); return 0; }
															#line 6563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Reference<IAsyncFile>> __when_expr_0 = bc->readFile(file.fileName);
															#line 1361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 6585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state = 1;
															#line 1361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetSnapshotFileKeyRange_implActor, 0, Reference<IAsyncFile> >*>(static_cast<GetSnapshotFileKeyRange_implActor*>(this)));
															#line 6590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (e.code() == error_code_restore_bad_read || e.code() == error_code_restore_unsupported_file_version || e.code() == error_code_restore_corrupted_data_padding)
															#line 6625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				TraceEvent(SevError, "BackupContainerGetSnapshotFileKeyRange").error(e);
															#line 1379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 6631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
			else
			{
															#line 1380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (e.code() == error_code_http_request_failed || e.code() == error_code_connection_failed || e.code() == error_code_timed_out || e.code() == error_code_lookup_failed)
															#line 6637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				{
															#line 1383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					TraceEvent(SevWarnAlways, "BackupContainerGetSnapshotFileKeyRangeConnectionFailure") .error(e) .detail("Retries", ++readFileRetries);
															#line 1386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					StrictFuture<Void> __when_expr_2 = delayJittered(0.1);
															#line 1386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					if (static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
					if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
					static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state = 3;
															#line 1386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetSnapshotFileKeyRange_implActor, 2, Void >*>(static_cast<GetSnapshotFileKeyRange_implActor*>(this)));
															#line 6650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
					loopDepth = 0;
				}
				else
				{
															#line 1388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					TraceEvent(SevError, "BackupContainerGetSnapshotFileKeyRangeUnexpectedError").error(e);
															#line 1389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
					return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 6659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		beginKeySet = false;
															#line 1363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		j = 0;
															#line 1364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 6679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Reference<IAsyncFile> const& __inFile,int loopDepth) 
	{
															#line 1361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		inFile = __inFile;
															#line 6688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(j < file.fileSize))
															#line 6768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1loopBody1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int64_t len = std::min<int64_t>(file.blockSize, file.fileSize - j);
															#line 1366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Standalone<VectorRef<KeyValueRef>>> __when_expr_1 = fileBackup::decodeRangeFileBlock(inFile, j, len, cx);
															#line 1366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetSnapshotFileKeyRange_implActor*>(this)->actor_wait_state = 2;
															#line 1366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetSnapshotFileKeyRange_implActor, 1, Standalone<VectorRef<KeyValueRef>> >*>(static_cast<GetSnapshotFileKeyRange_implActor*>(this)));
															#line 6783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!beginKeySet)
															#line 6805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			beginKey = blockData.front().key;
															#line 1370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			beginKeySet = true;
															#line 6811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		endKey = blockData.back().key;
															#line 1364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		j += file.blockSize;
															#line 6817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont1(Standalone<VectorRef<KeyValueRef>> && blockData,int loopDepth) 
	{
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!beginKeySet)
															#line 6826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			beginKey = blockData.front().key;
															#line 1370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			beginKeySet = true;
															#line 6832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		endKey = blockData.back().key;
															#line 1364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		j += file.blockSize;
															#line 6838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	RangeFile file;
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Database cx;
															#line 1355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int readFileRetries;
															#line 1356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	bool beginKeySet;
															#line 1357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Key beginKey;
															#line 1358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Key endKey;
															#line 1361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IAsyncFile> inFile;
															#line 1363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int64_t j;
															#line 7011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via getSnapshotFileKeyRange_impl()
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class GetSnapshotFileKeyRange_implActor final : public Actor<KeyRange>, public ActorCallback< GetSnapshotFileKeyRange_implActor, 0, Reference<IAsyncFile> >, public ActorCallback< GetSnapshotFileKeyRange_implActor, 1, Standalone<VectorRef<KeyValueRef>> >, public ActorCallback< GetSnapshotFileKeyRange_implActor, 2, Void >, public FastAllocated<GetSnapshotFileKeyRange_implActor>, public GetSnapshotFileKeyRange_implActorState<GetSnapshotFileKeyRange_implActor> {
															#line 7016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	GetSnapshotFileKeyRange_implActor(Reference<BackupContainerFileSystem> const& bc,RangeFile const& file,Database const& cx) 
															#line 7029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<KeyRange>(),
		   GetSnapshotFileKeyRange_implActorState<GetSnapshotFileKeyRange_implActor>(bc, file, cx)
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
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<KeyRange> getSnapshotFileKeyRange_impl( Reference<BackupContainerFileSystem> const& bc, RangeFile const& file, Database const& cx ) {
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<KeyRange>(new GetSnapshotFileKeyRange_implActor(bc, file, cx));
															#line 7059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 7064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
namespace {
// This generated class is to be used only via writeVersionProperty()
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class WriteVersionPropertyActor>
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class WriteVersionPropertyActorState {
															#line 7071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	WriteVersionPropertyActorState(Reference<BackupContainerFileSystem> const& bc,std::string const& path,Version const& v) 
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   path(path),
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   v(v)
															#line 7082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Reference<IBackupFile>> __when_expr_0 = bc->writeFile(path);
															#line 1398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 7100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state = 1;
															#line 1398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WriteVersionPropertyActor, 0, Reference<IBackupFile> >*>(static_cast<WriteVersionPropertyActor*>(this)));
															#line 7105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarn, "BackupContainerWritePropertyFailed") .error(e) .detail("URL", bc->getURL()) .detail("Path", path);
															#line 1408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 7137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		std::string s = format("%lld", v);
															#line 1400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_1 = f->append(s.data(), s.size());
															#line 1400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 7155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state = 2;
															#line 1400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WriteVersionPropertyActor, 1, Void >*>(static_cast<WriteVersionPropertyActor*>(this)));
															#line 7160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IBackupFile> const& __f,int loopDepth) 
	{
															#line 1398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		f = __f;
															#line 7169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->finish();
															#line 1401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 7238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state = 3;
															#line 1401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteVersionPropertyActor, 2, Void >*>(static_cast<WriteVersionPropertyActor*>(this)));
															#line 7243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 1401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->finish();
															#line 1401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 7254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteVersionPropertyActor*>(this)->actor_wait_state = 3;
															#line 1401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteVersionPropertyActor, 2, Void >*>(static_cast<WriteVersionPropertyActor*>(this)));
															#line 7259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<WriteVersionPropertyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteVersionPropertyActorState(); static_cast<WriteVersionPropertyActor*>(this)->destroy(); return 0; }
															#line 7331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<WriteVersionPropertyActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteVersionPropertyActorState();
		static_cast<WriteVersionPropertyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 1402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<WriteVersionPropertyActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteVersionPropertyActorState(); static_cast<WriteVersionPropertyActor*>(this)->destroy(); return 0; }
															#line 7343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string path;
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version v;
															#line 1398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IBackupFile> f;
															#line 7422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via writeVersionProperty()
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class WriteVersionPropertyActor final : public Actor<Void>, public ActorCallback< WriteVersionPropertyActor, 0, Reference<IBackupFile> >, public ActorCallback< WriteVersionPropertyActor, 1, Void >, public ActorCallback< WriteVersionPropertyActor, 2, Void >, public FastAllocated<WriteVersionPropertyActor>, public WriteVersionPropertyActorState<WriteVersionPropertyActor> {
															#line 7427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	WriteVersionPropertyActor(Reference<BackupContainerFileSystem> const& bc,std::string const& path,Version const& v) 
															#line 7440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Void> writeVersionProperty( Reference<BackupContainerFileSystem> const& bc, std::string const& path, Version const& v ) {
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new WriteVersionPropertyActor(bc, path, v));
															#line 7470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 7475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
namespace {
// This generated class is to be used only via readVersionProperty()
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class ReadVersionPropertyActor>
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ReadVersionPropertyActorState {
															#line 7482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ReadVersionPropertyActorState(Reference<BackupContainerFileSystem> const& bc,std::string const& path) 
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : bc(bc),
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   path(path)
															#line 7491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Reference<IAsyncFile>> __when_expr_0 = bc->readFile(path);
															#line 1414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 7509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state = 1;
															#line 1414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadVersionPropertyActor, 0, Reference<IAsyncFile> >*>(static_cast<ReadVersionPropertyActor*>(this)));
															#line 7514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (e.code() == error_code_file_not_found)
															#line 7544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (!static_cast<ReadVersionPropertyActor*>(this)->SAV<Optional<Version>>::futures) { (void)(Optional<Version>()); this->~ReadVersionPropertyActorState(); static_cast<ReadVersionPropertyActor*>(this)->destroy(); return 0; }
															#line 7548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				new (&static_cast<ReadVersionPropertyActor*>(this)->SAV< Optional<Version> >::value()) Optional<Version>(Optional<Version>());
				this->~ReadVersionPropertyActorState();
				static_cast<ReadVersionPropertyActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			TraceEvent(SevWarn, "BackupContainerReadPropertyFailed") .error(e) .detail("URL", bc->getURL()) .detail("Path", path);
															#line 1436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 7558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<int64_t> __when_expr_1 = f->size();
															#line 1415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 7574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state = 2;
															#line 1415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadVersionPropertyActor, 1, int64_t >*>(static_cast<ReadVersionPropertyActor*>(this)));
															#line 7579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> const& __f,int loopDepth) 
	{
															#line 1414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		f = __f;
															#line 7588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		s = std::string();
															#line 1417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		s.resize(size);
															#line 1418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<int> __when_expr_2 = f->read((uint8_t*)s.data(), size, 0);
															#line 1418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 7661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ReadVersionPropertyActor*>(this)->actor_wait_state = 3;
															#line 1418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReadVersionPropertyActor, 2, int >*>(static_cast<ReadVersionPropertyActor*>(this)));
															#line 7666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(int64_t const& __size,int loopDepth) 
	{
															#line 1415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		size = __size;
															#line 7675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		Version v;
															#line 1420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int len;
															#line 1421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (rs == size && sscanf(s.c_str(), "%" SCNd64 "%n", &v, &len) == 1 && len == size)
															#line 7746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<ReadVersionPropertyActor*>(this)->SAV<Optional<Version>>::futures) { (void)(v); this->~ReadVersionPropertyActorState(); static_cast<ReadVersionPropertyActor*>(this)->destroy(); return 0; }
															#line 7750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<ReadVersionPropertyActor*>(this)->SAV< Optional<Version> >::value()) Optional<Version>(v);
			this->~ReadVersionPropertyActorState();
			static_cast<ReadVersionPropertyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent(SevWarn, "BackupContainerInvalidProperty").detail("URL", bc->getURL()).detail("Path", path);
															#line 1426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		return a_body1Catch2(backup_invalid_info(), loopDepth);
															#line 7760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont4(int && rs,int loopDepth) 
	{
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		Version v;
															#line 1420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int len;
															#line 1421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (rs == size && sscanf(s.c_str(), "%" SCNd64 "%n", &v, &len) == 1 && len == size)
															#line 7772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<ReadVersionPropertyActor*>(this)->SAV<Optional<Version>>::futures) { (void)(v); this->~ReadVersionPropertyActorState(); static_cast<ReadVersionPropertyActor*>(this)->destroy(); return 0; }
															#line 7776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<ReadVersionPropertyActor*>(this)->SAV< Optional<Version> >::value()) Optional<Version>(v);
			this->~ReadVersionPropertyActorState();
			static_cast<ReadVersionPropertyActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		TraceEvent(SevWarn, "BackupContainerInvalidProperty").detail("URL", bc->getURL()).detail("Path", path);
															#line 1426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		return a_body1Catch2(backup_invalid_info(), loopDepth);
															#line 7786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"

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
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<BackupContainerFileSystem> bc;
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string path;
															#line 1414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IAsyncFile> f;
															#line 1415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int64_t size;
															#line 1416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string s;
															#line 7863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via readVersionProperty()
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class ReadVersionPropertyActor final : public Actor<Optional<Version>>, public ActorCallback< ReadVersionPropertyActor, 0, Reference<IAsyncFile> >, public ActorCallback< ReadVersionPropertyActor, 1, int64_t >, public ActorCallback< ReadVersionPropertyActor, 2, int >, public FastAllocated<ReadVersionPropertyActor>, public ReadVersionPropertyActorState<ReadVersionPropertyActor> {
															#line 7868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	ReadVersionPropertyActor(Reference<BackupContainerFileSystem> const& bc,std::string const& path) 
															#line 7881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Optional<Version>> readVersionProperty( Reference<BackupContainerFileSystem> const& bc, std::string const& path ) {
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Optional<Version>>(new ReadVersionPropertyActor(bc, path));
															#line 7911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

Future<KeyRange> BackupContainerFileSystem::getSnapshotFileKeyRange(const RangeFile& file, Database cx) {
	ASSERT(g_network->isSimulated());
	return getSnapshotFileKeyRange_impl(Reference<BackupContainerFileSystem>::addRef(this), file, cx);
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

Future<Void> BackupContainerFileSystem::writeEntireFileFallback(const std::string& fileName,
                                                                const std::string& fileContents) {
	return BackupContainerFileSystemImpl::writeEntireFileFallback(
	    Reference<BackupContainerFileSystem>::addRef(this), fileName, fileContents);
}

void BackupContainerFileSystem::setEncryptionKey(Optional<std::string> const& encryptionKeyFileName) {
	if (encryptionKeyFileName.present()) {
		encryptionSetupFuture = BackupContainerFileSystemImpl::readEncryptionKey(encryptionKeyFileName.get());
	}
}
Future<Void> BackupContainerFileSystem::createTestEncryptionKeyFile(std::string const& filename) {
	return BackupContainerFileSystemImpl::createTestEncryptionKeyFile(filename);
}

// Get a BackupContainerFileSystem based on a container URL string
// TODO: refactor to not duplicate IBackupContainer::openContainer. It's the exact same
// code but returning a different template type because you can't cast between them
Reference<BackupContainerFileSystem> BackupContainerFileSystem::openContainerFS(
    const std::string& url,
    const Optional<std::string>& proxy,
    const Optional<std::string>& encryptionKeyFileName,
    bool isBackup) {
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
			r = makeReference<BackupContainerS3BlobStore>(
			    bstore, resource, backupParams, encryptionKeyFileName, isBackup);
		}
#ifdef BUILD_AZURE_BACKUP
		else if (u.startsWith("azure://"_sr)) {
			u.eat("azure://"_sr);
			auto address = u.eat("/"_sr);
			if (address.endsWith(std::string(azure::storage_lite::constants::default_endpoint_suffix))) {
				CODE_PROBE(true, "Azure backup url with standard azure storage account endpoint");
				// <account>.<service>.core.windows.net/<resource_path>
				auto endPoint = address.toString();
				auto accountName = address.eat("."_sr).toString();
				auto containerName = u.eat("/"_sr).toString();
				r = makeReference<BackupContainerAzureBlobStore>(
				    endPoint, accountName, containerName, encryptionKeyFileName);
			} else {
				// resolve the network address if necessary
				std::string endpoint(address.toString());
				Optional<NetworkAddress> parsedAddress = NetworkAddress::parseOptional(endpoint);
				if (!parsedAddress.present()) {
					try {
						auto hostname = Hostname::parse(endpoint);
						auto resolvedAddress = hostname.resolveBlocking();
						if (resolvedAddress.present()) {
							CODE_PROBE(true, "Azure backup url with hostname in the endpoint");
							parsedAddress = resolvedAddress.get();
						}
					} catch (Error& e) {
						TraceEvent(SevError, "InvalidAzureBackupUrl").error(e).detail("Endpoint", endpoint);
						throw backup_invalid_url();
					}
				}
				if (!parsedAddress.present()) {
					TraceEvent(SevError, "InvalidAzureBackupUrl").detail("Endpoint", endpoint);
					throw backup_invalid_url();
				}
				auto accountName = u.eat("/"_sr).toString();
				// Avoid including ":tls" and "(fromHostname)"
				// note: the endpoint needs to contain the account name
				// so either "<account_name>.blob.core.windows.net" or "<ip>:<port>/<account_name>"
				endpoint =
				    fmt::format("{}/{}", formatIpPort(parsedAddress.get().ip, parsedAddress.get().port), accountName);
				auto containerName = u.eat("/"_sr).toString();
				r = makeReference<BackupContainerAzureBlobStore>(
				    endpoint, accountName, containerName, encryptionKeyFileName);
			}
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

															#line 8098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via writeAndVerifyFile()
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class WriteAndVerifyFileActor>
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class WriteAndVerifyFileActorState {
															#line 8104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	WriteAndVerifyFileActorState(Reference<IBackupContainer> const& c,Reference<IBackupFile> const& f,int const& size,FlowLock* const& lock) 
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : c(c),
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   f(f),
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   size(size),
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   lock(lock),
															#line 1626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   content()
															#line 8119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_0 = lock->take(TaskPriority::DefaultYield, size);
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 1;
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 0, Void >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 8141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		releaser = FlowLock::Releaser(*lock, size);
															#line 1631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("writeAndVerify size=%d file=%s\n", size, f->getFileName().c_str());
															#line 1632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		content.resize(content.arena(), size);
															#line 1633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for(int i = 0;i < content.size();++i) {
															#line 1634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			content[i] = (uint8_t)deterministicRandom()->randomInt(0, 256);
															#line 8172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		sendBuf = content;
															#line 1638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 8178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		releaser = FlowLock::Releaser(*lock, size);
															#line 1631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("writeAndVerify size=%d file=%s\n", size, f->getFileName().c_str());
															#line 1632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		content.resize(content.arena(), size);
															#line 1633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for(int i = 0;i < content.size();++i) {
															#line 1634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			content[i] = (uint8_t)deterministicRandom()->randomInt(0, 256);
															#line 8195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		sendBuf = content;
															#line 1638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 8201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->finish();
															#line 1643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 3;
															#line 1643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 2, Void >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 8280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(sendBuf.size() > 0))
															#line 8296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		n = std::min(sendBuf.size(), deterministicRandom()->randomInt(1, 16384));
															#line 1640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_1 = f->append(sendBuf.begin(), n);
															#line 1640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 8306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 2;
															#line 1640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 1, Void >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 8311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		sendBuf.pop_front(n);
															#line 8333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 1641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		sendBuf.pop_front(n);
															#line 8342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IAsyncFile>> __when_expr_3 = c->readFile(f->getFileName());
															#line 1645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 4;
															#line 1645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 3, Reference<IAsyncFile> >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 8421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 1645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IAsyncFile>> __when_expr_3 = c->readFile(f->getFileName());
															#line 1645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 4;
															#line 1645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 3, Reference<IAsyncFile> >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 8437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<int64_t> __when_expr_4 = inputFile->size();
															#line 1646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont5when1(__when_expr_4.get(), loopDepth); };
		static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 5;
															#line 1646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 4, int64_t >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 8516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4when1(Reference<IAsyncFile> const& __inputFile,int loopDepth) 
	{
															#line 1645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		inputFile = __inputFile;
															#line 8525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(size, fileSize);
															#line 1648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (size > 0)
															#line 8594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			buf = Standalone<VectorRef<uint8_t>>();
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			buf.resize(buf.arena(), fileSize);
															#line 1651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<int> __when_expr_5 = inputFile->read(buf.begin(), buf.size(), 0);
															#line 1651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont6when1(__when_expr_5.get(), loopDepth); };
			static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 6;
															#line 1651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 5, int >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 8609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(size, fileSize);
															#line 1648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (size > 0)
															#line 8625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			buf = Standalone<VectorRef<uint8_t>>();
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			buf.resize(buf.arena(), fileSize);
															#line 1651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<int> __when_expr_5 = inputFile->read(buf.begin(), buf.size(), 0);
															#line 1651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont6when1(__when_expr_5.get(), loopDepth); };
			static_cast<WriteAndVerifyFileActor*>(this)->actor_wait_state = 6;
															#line 1651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< WriteAndVerifyFileActor, 5, int >*>(static_cast<WriteAndVerifyFileActor*>(this)));
															#line 8640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<WriteAndVerifyFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteAndVerifyFileActorState(); static_cast<WriteAndVerifyFileActor*>(this)->destroy(); return 0; }
															#line 8717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<WriteAndVerifyFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteAndVerifyFileActorState();
		static_cast<WriteAndVerifyFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont8(int const& b,int loopDepth) 
	{
															#line 1652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(b, buf.size());
															#line 1653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(buf == content);
															#line 8731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont8(int && b,int loopDepth) 
	{
															#line 1652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(b, buf.size());
															#line 1653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(buf == content);
															#line 8742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IBackupContainer> c;
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IBackupFile> f;
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int size;
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowLock* lock;
															#line 1626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Standalone<VectorRef<uint8_t>> content;
															#line 1629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowLock::Releaser releaser;
															#line 1637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	VectorRef<uint8_t> sendBuf;
															#line 1639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int n;
															#line 1645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IAsyncFile> inputFile;
															#line 1649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Standalone<VectorRef<uint8_t>> buf;
															#line 8830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via writeAndVerifyFile()
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class WriteAndVerifyFileActor final : public Actor<Void>, public ActorCallback< WriteAndVerifyFileActor, 0, Void >, public ActorCallback< WriteAndVerifyFileActor, 1, Void >, public ActorCallback< WriteAndVerifyFileActor, 2, Void >, public ActorCallback< WriteAndVerifyFileActor, 3, Reference<IAsyncFile> >, public ActorCallback< WriteAndVerifyFileActor, 4, int64_t >, public ActorCallback< WriteAndVerifyFileActor, 5, int >, public FastAllocated<WriteAndVerifyFileActor>, public WriteAndVerifyFileActorState<WriteAndVerifyFileActor> {
															#line 8835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	WriteAndVerifyFileActor(Reference<IBackupContainer> const& c,Reference<IBackupFile> const& f,int const& size,FlowLock* const& lock) 
															#line 8851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] Future<Void> writeAndVerifyFile( Reference<IBackupContainer> const& c, Reference<IBackupFile> const& f, int const& size, FlowLock* const& lock ) {
															#line 1622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new WriteAndVerifyFileActor(c, f, size, lock));
															#line 8883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

// Randomly advance version by up to 1 second of versions
Version nextVersion(Version v) {
	int64_t increment = deterministicRandom()->randomInt64(1, CLIENT_KNOBS->CORE_VERSIONSPERSECOND);
	return v + increment;
}

// Write a snapshot file with only begin & end key
															#line 8895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via testWriteSnapshotFile()
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class TestWriteSnapshotFileActor>
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class TestWriteSnapshotFileActorState {
															#line 8901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	TestWriteSnapshotFileActorState(Reference<IBackupFile> const& file,Key const& begin,Key const& end,uint32_t const& blockSize) 
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : file(file),
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   begin(begin),
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   end(end),
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   blockSize(blockSize)
															#line 8914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT_GT(blockSize, 3 * sizeof(uint32_t) + begin.size() + end.size());
															#line 1668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			uint32_t fileVersion = BACKUP_AGENT_SNAPSHOT_FILE_VERSION;
															#line 1670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_0 = file->append((uint8_t*)&fileVersion, sizeof(fileVersion));
															#line 1670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 1;
															#line 1670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 0, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 8940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_1 = file->appendStringRefWithLen(begin);
															#line 1673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 2;
															#line 1673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 1, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 8970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_1 = file->appendStringRefWithLen(begin);
															#line 1673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 2;
															#line 1673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 1, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 8986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = file->appendStringRefWithLen(end);
															#line 1676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 3;
															#line 1676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 2, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 9065 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = file->appendStringRefWithLen(end);
															#line 1676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 3;
															#line 1676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 2, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 9081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int bytesLeft = blockSize - file->size();
															#line 1679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (bytesLeft > 0)
															#line 9155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			Value paddings = fileBackup::makePadding(bytesLeft);
															#line 1681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_3 = file->append(paddings.begin(), bytesLeft);
															#line 1681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
			static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 4;
															#line 1681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 3, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 9168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int bytesLeft = blockSize - file->size();
															#line 1679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (bytesLeft > 0)
															#line 9184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			Value paddings = fileBackup::makePadding(bytesLeft);
															#line 1681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_3 = file->append(paddings.begin(), bytesLeft);
															#line 1681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
			static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 4;
															#line 1681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 3, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 9197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_4 = file->finish();
															#line 1683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
		static_cast<TestWriteSnapshotFileActor*>(this)->actor_wait_state = 5;
															#line 1683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< TestWriteSnapshotFileActor, 4, Void >*>(static_cast<TestWriteSnapshotFileActor*>(this)));
															#line 9281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<TestWriteSnapshotFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestWriteSnapshotFileActorState(); static_cast<TestWriteSnapshotFileActor*>(this)->destroy(); return 0; }
															#line 9365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<TestWriteSnapshotFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TestWriteSnapshotFileActorState();
		static_cast<TestWriteSnapshotFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 1684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<TestWriteSnapshotFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestWriteSnapshotFileActorState(); static_cast<TestWriteSnapshotFileActor*>(this)->destroy(); return 0; }
															#line 9377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IBackupFile> file;
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Key begin;
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Key end;
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	uint32_t blockSize;
															#line 9456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via testWriteSnapshotFile()
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class TestWriteSnapshotFileActor final : public Actor<Void>, public ActorCallback< TestWriteSnapshotFileActor, 0, Void >, public ActorCallback< TestWriteSnapshotFileActor, 1, Void >, public ActorCallback< TestWriteSnapshotFileActor, 2, Void >, public ActorCallback< TestWriteSnapshotFileActor, 3, Void >, public ActorCallback< TestWriteSnapshotFileActor, 4, Void >, public FastAllocated<TestWriteSnapshotFileActor>, public TestWriteSnapshotFileActorState<TestWriteSnapshotFileActor> {
															#line 9461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	TestWriteSnapshotFileActor(Reference<IBackupFile> const& file,Key const& begin,Key const& end,uint32_t const& blockSize) 
															#line 9476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] static Future<Void> testWriteSnapshotFile( Reference<IBackupFile> const& file, Key const& begin, Key const& end, uint32_t const& blockSize ) {
															#line 1665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new TestWriteSnapshotFileActor(file, begin, end, blockSize));
															#line 9507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 9512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via testBackupContainer()
															#line 1687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class TestBackupContainerActor>
															#line 1687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class TestBackupContainerActorState {
															#line 9518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	TestBackupContainerActorState(std::string const& url,Optional<std::string> const& proxy,Optional<std::string> const& encryptionKeyFileName) 
															#line 1687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : url(url),
															#line 1687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   proxy(proxy),
															#line 1687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   encryptionKeyFileName(encryptionKeyFileName),
															#line 1690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		   lock(100e6)
															#line 9531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (encryptionKeyFileName.present())
															#line 9546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Void> __when_expr_0 = BackupContainerFileSystem::createTestEncryptionKeyFile(encryptionKeyFileName.get());
															#line 1693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 1;
															#line 1693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 0, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 9557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("BackupContainerTest URL %s\n", url.c_str());
															#line 1698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		c = IBackupContainer::openContainer(url, proxy, encryptionKeyFileName);
															#line 9587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		try {
															#line 1702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_1 = c->deleteContainer();
															#line 1702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 9593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 2;
															#line 1702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 1, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 9598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_2 = c->create();
															#line 1708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 3;
															#line 1708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 2, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 9695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (e.code() != error_code_backup_invalid_url && e.code() != error_code_backup_does_not_exist)
															#line 9705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 9709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		writes = std::vector<Future<Void>>();
															#line 1711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshots = std::map<Version, std::vector<std::string>>();
															#line 1712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotSizes = std::map<Version, int64_t>();
															#line 1713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotBeginEndKeys = std::map<Version, std::vector<std::pair<Key, Key>>>();
															#line 1714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		nRangeFiles = 0;
															#line 1715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs = std::map<Version, std::string>();
															#line 1716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		v = deterministicRandom()->randomInt64(0, std::numeric_limits<Version>::max() / 2);
															#line 1719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		fileSizes = { 0 };
															#line 1720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (StringRef(url).startsWith("blob"_sr))
															#line 9829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			fileSizes.push_back(CLIENT_KNOBS->BLOBSTORE_MULTIPART_MIN_PART_SIZE);
															#line 1722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			fileSizes.push_back(CLIENT_KNOBS->BLOBSTORE_MULTIPART_MIN_PART_SIZE + 10);
															#line 9835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 9839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont6loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 1710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		writes = std::vector<Future<Void>>();
															#line 1711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshots = std::map<Version, std::vector<std::string>>();
															#line 1712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotSizes = std::map<Version, int64_t>();
															#line 1713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotBeginEndKeys = std::map<Version, std::vector<std::pair<Key, Key>>>();
															#line 1714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		nRangeFiles = 0;
															#line 1715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs = std::map<Version, std::string>();
															#line 1716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		v = deterministicRandom()->randomInt64(0, std::numeric_limits<Version>::max() / 2);
															#line 1719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		fileSizes = { 0 };
															#line 1720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (StringRef(url).startsWith("blob"_sr))
															#line 9864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			fileSizes.push_back(CLIENT_KNOBS->BLOBSTORE_MULTIPART_MIN_PART_SIZE);
															#line 1722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			fileSizes.push_back(CLIENT_KNOBS->BLOBSTORE_MULTIPART_MIN_PART_SIZE + 10);
															#line 9870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 9874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_6 = waitForAll(writes);
															#line 1783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont7when1(__when_expr_6.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 7;
															#line 1783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 6, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 9953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logStart = v;
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		kvfiles = deterministicRandom()->randomInt(0, 3);
															#line 1728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		begin = ""_sr;
															#line 1729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		end = ""_sr;
															#line 1730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		blockSize = 3 * sizeof(uint32_t) + begin.size() + end.size() + 8;
															#line 1732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 9979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (logStart == v || deterministicRandom()->coinflip())
															#line 10001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			v = nextVersion(v);
															#line 10005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
															#line 1770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IBackupFile>> __when_expr_5 = c->writeLogFile(logStart, v, 10);
															#line 1770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1cont6loopBody1cont1when1(__when_expr_5.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 6;
															#line 1770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 5, Reference<IBackupFile> >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(kvfiles > 0))
															#line 10032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont6loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (snapshots.empty())
															#line 10038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshots[v] = {};
															#line 1735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshotBeginEndKeys[v] = {};
															#line 1736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshotSizes[v] = 0;
															#line 1737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (deterministicRandom()->coinflip())
															#line 10048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				v = nextVersion(v);
															#line 10052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
		}
															#line 1741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Reference<IBackupFile>> __when_expr_3 = c->writeRangeFile(snapshots.rbegin()->first, 0, v, blockSize);
															#line 1741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 10059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 2)); else return a_body1cont6loopBody1loopBody1when1(__when_expr_3.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 4;
															#line 1741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 3, Reference<IBackupFile> >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		++nRangeFiles;
															#line 1743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		v = nextVersion(v);
															#line 1744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshots.rbegin()->second.push_back(range->getFileName());
															#line 1745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotBeginEndKeys.rbegin()->second.emplace_back(begin, end);
															#line 1747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int size = chooseFileSize(fileSizes);
															#line 1748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotSizes.rbegin()->second += size;
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_4 = testWriteSnapshotFile(range, begin, end, blockSize);
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 10100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 2)); else return a_body1cont6loopBody1loopBody1cont1when1(__when_expr_4.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 5;
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 4, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6loopBody1loopBody1cont1(Reference<IBackupFile> && range,int loopDepth) 
	{
															#line 1742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		++nRangeFiles;
															#line 1743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		v = nextVersion(v);
															#line 1744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshots.rbegin()->second.push_back(range->getFileName());
															#line 1745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotBeginEndKeys.rbegin()->second.emplace_back(begin, end);
															#line 1747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int size = chooseFileSize(fileSizes);
															#line 1748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		snapshotSizes.rbegin()->second += size;
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_4 = testWriteSnapshotFile(range, begin, end, blockSize);
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 10128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 2)); else return a_body1cont6loopBody1loopBody1cont1when1(__when_expr_4.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 5;
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 4, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (deterministicRandom()->random01() < .2)
															#line 10205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			writes.push_back(c->writeKeyspaceSnapshotFile(snapshots.rbegin()->second, snapshotBeginEndKeys.rbegin()->second, snapshotSizes.rbegin()->second, IncludeKeyRangeMap(BUGGIFY)));
															#line 1758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshots[v] = {};
															#line 1759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshotBeginEndKeys[v] = {};
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshotSizes[v] = 0;
															#line 10215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			return a_body1cont6loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		--kvfiles;
															#line 10220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont6loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont6loopBody1loopBody1cont5(Void && _,int loopDepth) 
	{
															#line 1753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (deterministicRandom()->random01() < .2)
															#line 10229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			writes.push_back(c->writeKeyspaceSnapshotFile(snapshots.rbegin()->second, snapshotBeginEndKeys.rbegin()->second, snapshotSizes.rbegin()->second, IncludeKeyRangeMap(BUGGIFY)));
															#line 1758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshots[v] = {};
															#line 1759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshotBeginEndKeys[v] = {};
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshotSizes[v] = 0;
															#line 10239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			return a_body1cont6loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		--kvfiles;
															#line 10244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		logs[logStart] = log->getFileName();
															#line 1772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		int size = chooseFileSize(fileSizes);
															#line 1773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		writes.push_back(writeAndVerifyFile(c, log, size, &lock));
															#line 1776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (fileSizes.empty() && !snapshots.empty() && snapshots.rbegin()->second.empty() && deterministicRandom()->random01() < .2)
															#line 10322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			snapshots.erase(snapshots.rbegin()->first);
															#line 10326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			return a_body1cont6break1(loopDepth==0?0:loopDepth-1); // break
		}
		if (loopDepth == 0) return a_body1cont6loopHead1(0);

		return loopDepth;
	}
	int a_body1cont6loopBody1cont1when1(Reference<IBackupFile> const& __log,int loopDepth) 
	{
															#line 1770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		log = __log;
															#line 10337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<BackupFileList> __when_expr_7 = c->dumpFileList();
															#line 1785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1Catch1(__when_expr_7.getError(), loopDepth); else return a_body1cont9when1(__when_expr_7.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 8;
															#line 1785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 7, BackupFileList >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont9(Void && _,int loopDepth) 
	{
															#line 1785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<BackupFileList> __when_expr_7 = c->dumpFileList();
															#line 1785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1Catch1(__when_expr_7.getError(), loopDepth); else return a_body1cont9when1(__when_expr_7.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 8;
															#line 1785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 7, BackupFileList >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(listing.ranges.size(), nRangeFiles);
															#line 1787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(listing.logs.size(), logs.size());
															#line 1788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(listing.snapshots.size(), snapshots.size());
															#line 1790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<BackupDescription> __when_expr_8 = c->describeBackup();
															#line 1790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_8.isReady()) { if (__when_expr_8.isError()) return a_body1Catch1(__when_expr_8.getError(), loopDepth); else return a_body1cont10when1(__when_expr_8.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 9;
															#line 1790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_8.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 8, BackupDescription >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont9when1(BackupFileList const& __listing,int loopDepth) 
	{
															#line 1785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		listing = __listing;
															#line 10521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("\n%s\n", desc.toString().c_str());
															#line 1794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		i = 0;
															#line 1795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		;
															#line 10592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont10cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont10when1(BackupDescription const& __desc,int loopDepth) 
	{
															#line 1790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		desc = __desc;
															#line 10601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("DELETING\n");
															#line 1828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_14 = c->deleteContainer();
															#line 1828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_14.isReady()) { if (__when_expr_14.isError()) return a_body1Catch1(__when_expr_14.getError(), loopDepth); else return a_body1cont10cont2when1(__when_expr_14.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 15;
															#line 1828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_14.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 14, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!(i < listing.snapshots.size()))
															#line 10693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
			return a_body1cont10cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
		{
															#line 1798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Optional<RestorableFileSet>> __when_expr_9 = c->getRestoreSet(desc.maxRestorableVersion.get());
															#line 1798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_9.isReady()) { if (__when_expr_9.isError()) return a_body1Catch1(__when_expr_9.getError(), std::max(0, loopDepth - 1)); else return a_body1cont10cont1loopBody1when1(__when_expr_9.get(), loopDepth); };
			static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 10;
															#line 1798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_9.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 9, Optional<RestorableFileSet> >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Optional<RestorableFileSet>> __when_expr_10 = c->getRestoreSet(listing.snapshots[i].endVersion);
															#line 1804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_10.isReady()) { if (__when_expr_10.isError()) return a_body1Catch1(__when_expr_10.getError(), std::max(0, loopDepth - 1)); else return a_body1cont10cont1loopBody1cont1when1(__when_expr_10.get(), loopDepth); };
			static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 11;
															#line 1804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_10.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 10, Optional<RestorableFileSet> >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont3(Optional<RestorableFileSet> const& rest,int loopDepth) 
	{
															#line 1799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(rest.present());
															#line 10748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont10cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont3(Optional<RestorableFileSet> && rest,int loopDepth) 
	{
															#line 1799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(rest.present());
															#line 10757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		expireVersion = listing.snapshots[i].endVersion;
															#line 1812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		fmt::print("EXPIRE TO {}\n", expireVersion);
															#line 1813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		f = c->expireData(expireVersion);
															#line 1814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_11 = ready(f);
															#line 1814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_11.isReady()) { if (__when_expr_11.isError()) return a_body1Catch1(__when_expr_11.getError(), std::max(0, loopDepth - 1)); else return a_body1cont10cont1loopBody1cont5when1(__when_expr_11.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 12;
															#line 1814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_11.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 11, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont6(Optional<RestorableFileSet> const& rest,int loopDepth) 
	{
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(rest.present());
															#line 10851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = a_body1cont10cont1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont6(Optional<RestorableFileSet> && rest,int loopDepth) 
	{
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(rest.present());
															#line 10860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (f.isError())
															#line 10932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT_EQ(f.getError().code(), error_code_backup_cannot_expire);
															#line 1819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT_EQ(i, listing.snapshots.size() - 1);
															#line 1820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_12 = c->expireData(expireVersion, true);
															#line 1820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_12.isReady()) { if (__when_expr_12.isError()) return a_body1Catch1(__when_expr_12.getError(), std::max(0, loopDepth - 1)); else return a_body1cont10cont1loopBody1cont8when1(__when_expr_12.get(), loopDepth); };
			static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 13;
															#line 1820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_12.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 12, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (f.isError())
															#line 10961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		{
															#line 1818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT_EQ(f.getError().code(), error_code_backup_cannot_expire);
															#line 1819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT_EQ(i, listing.snapshots.size() - 1);
															#line 1820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_12 = c->expireData(expireVersion, true);
															#line 1820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_12.isReady()) { if (__when_expr_12.isError()) return a_body1Catch1(__when_expr_12.getError(), std::max(0, loopDepth - 1)); else return a_body1cont10cont1loopBody1cont8when1(__when_expr_12.get(), loopDepth); };
			static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 13;
															#line 1820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_12.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 12, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 10976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<BackupDescription> __when_expr_13 = c->describeBackup();
															#line 1823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 11055 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_13.isReady()) { if (__when_expr_13.isError()) return a_body1Catch1(__when_expr_13.getError(), std::max(0, loopDepth - 1)); else return a_body1cont10cont1loopBody1cont9when1(__when_expr_13.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 14;
															#line 1823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_13.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 13, BackupDescription >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 11060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("\n%s\n", d.toString().c_str());
															#line 1795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		++i;
															#line 11146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont10cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont10cont1loopBody1cont11(BackupDescription && d,int loopDepth) 
	{
															#line 1824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("\n%s\n", d.toString().c_str());
															#line 1795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		++i;
															#line 11157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		d = c->describeBackup();
															#line 1831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_15 = ready(d);
															#line 1831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_15.isReady()) { if (__when_expr_15.isError()) return a_body1Catch1(__when_expr_15.getError(), loopDepth); else return a_body1cont10cont3when1(__when_expr_15.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 16;
															#line 1831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_15.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 15, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 11238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont10cont3(Void && _,int loopDepth) 
	{
															#line 1830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		d = c->describeBackup();
															#line 1831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<Void> __when_expr_15 = ready(d);
															#line 1831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_15.isReady()) { if (__when_expr_15.isError()) return a_body1Catch1(__when_expr_15.getError(), loopDepth); else return a_body1cont10cont3when1(__when_expr_15.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 16;
															#line 1831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_15.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 15, Void >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 11256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(d.isError() && d.getError().code() == error_code_backup_does_not_exist);
															#line 1834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<BackupFileList> __when_expr_16 = c->dumpFileList();
															#line 1834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_16.isReady()) { if (__when_expr_16.isError()) return a_body1Catch1(__when_expr_16.getError(), loopDepth); else return a_body1cont10cont4when1(__when_expr_16.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 17;
															#line 1834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_16.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 16, BackupFileList >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 11337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont10cont4(Void && _,int loopDepth) 
	{
															#line 1832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT(d.isError() && d.getError().code() == error_code_backup_does_not_exist);
															#line 1834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		StrictFuture<BackupFileList> __when_expr_16 = c->dumpFileList();
															#line 1834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (static_cast<TestBackupContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		if (__when_expr_16.isReady()) { if (__when_expr_16.isError()) return a_body1Catch1(__when_expr_16.getError(), loopDepth); else return a_body1cont10cont4when1(__when_expr_16.get(), loopDepth); };
		static_cast<TestBackupContainerActor*>(this)->actor_wait_state = 17;
															#line 1834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		__when_expr_16.addCallbackAndClear(static_cast<ActorCallback< TestBackupContainerActor, 16, BackupFileList >*>(static_cast<TestBackupContainerActor*>(this)));
															#line 11355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(empty.ranges.size(), 0);
															#line 1836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(empty.logs.size(), 0);
															#line 1837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(empty.snapshots.size(), 0);
															#line 1839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("BackupContainerTest URL=%s PASSED.\n", url.c_str());
															#line 1841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<TestBackupContainerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestBackupContainerActorState(); static_cast<TestBackupContainerActor*>(this)->destroy(); return 0; }
															#line 11435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<TestBackupContainerActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TestBackupContainerActorState();
		static_cast<TestBackupContainerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10cont5(BackupFileList && empty,int loopDepth) 
	{
															#line 1835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(empty.ranges.size(), 0);
															#line 1836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(empty.logs.size(), 0);
															#line 1837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		ASSERT_EQ(empty.snapshots.size(), 0);
															#line 1839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		printf("BackupContainerTest URL=%s PASSED.\n", url.c_str());
															#line 1841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<TestBackupContainerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TestBackupContainerActorState(); static_cast<TestBackupContainerActor*>(this)->destroy(); return 0; }
															#line 11455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::string url;
															#line 1687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<std::string> proxy;
															#line 1687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Optional<std::string> encryptionKeyFileName;
															#line 1690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowLock lock;
															#line 1698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IBackupContainer> c;
															#line 1710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<Future<Void>> writes;
															#line 1711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::map<Version, std::vector<std::string>> snapshots;
															#line 1712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::map<Version, int64_t> snapshotSizes;
															#line 1713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::map<Version, std::vector<std::pair<Key, Key>>> snapshotBeginEndKeys;
															#line 1714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int nRangeFiles;
															#line 1715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::map<Version, std::string> logs;
															#line 1716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version v;
															#line 1719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	std::vector<int> fileSizes;
															#line 1726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version logStart;
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int kvfiles;
															#line 1728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Key begin;
															#line 1729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Key end;
															#line 1730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int blockSize;
															#line 1770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Reference<IBackupFile> log;
															#line 1785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	BackupFileList listing;
															#line 1790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	BackupDescription desc;
															#line 1794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	int i;
															#line 1809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Version expireVersion;
															#line 1813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Future<Void> f;
															#line 1830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	Future<BackupDescription> d;
															#line 11576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via testBackupContainer()
															#line 1687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class TestBackupContainerActor final : public Actor<Void>, public ActorCallback< TestBackupContainerActor, 0, Void >, public ActorCallback< TestBackupContainerActor, 1, Void >, public ActorCallback< TestBackupContainerActor, 2, Void >, public ActorCallback< TestBackupContainerActor, 3, Reference<IBackupFile> >, public ActorCallback< TestBackupContainerActor, 4, Void >, public ActorCallback< TestBackupContainerActor, 5, Reference<IBackupFile> >, public ActorCallback< TestBackupContainerActor, 6, Void >, public ActorCallback< TestBackupContainerActor, 7, BackupFileList >, public ActorCallback< TestBackupContainerActor, 8, BackupDescription >, public ActorCallback< TestBackupContainerActor, 9, Optional<RestorableFileSet> >, public ActorCallback< TestBackupContainerActor, 10, Optional<RestorableFileSet> >, public ActorCallback< TestBackupContainerActor, 11, Void >, public ActorCallback< TestBackupContainerActor, 12, Void >, public ActorCallback< TestBackupContainerActor, 13, BackupDescription >, public ActorCallback< TestBackupContainerActor, 14, Void >, public ActorCallback< TestBackupContainerActor, 15, Void >, public ActorCallback< TestBackupContainerActor, 16, BackupFileList >, public FastAllocated<TestBackupContainerActor>, public TestBackupContainerActorState<TestBackupContainerActor> {
															#line 11581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	TestBackupContainerActor(std::string const& url,Optional<std::string> const& proxy,Optional<std::string> const& encryptionKeyFileName) 
															#line 11608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
															#line 1687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
[[nodiscard]] Future<Void> testBackupContainer( std::string const& url, Optional<std::string> const& proxy, Optional<std::string> const& encryptionKeyFileName ) {
															#line 1687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new TestBackupContainerActor(url, proxy, encryptionKeyFileName));
															#line 11651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}

#line 1843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 11656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via flowTestCase1844()
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class FlowTestCase1844Actor>
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1844ActorState {
															#line 11662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1844ActorState(UnitTestParameters const& params) 
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : params(params)
															#line 11669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1844", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1844ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1844", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_0 = testBackupContainer(format("file://%s/fdb_backups/%llx", params.getDataDir().c_str(), timer_int()), {}, {});
															#line 1845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<FlowTestCase1844Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase1844Actor*>(this)->actor_wait_state = 1;
															#line 1845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1844Actor, 0, Void >*>(static_cast<FlowTestCase1844Actor*>(this)));
															#line 11691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~FlowTestCase1844ActorState();
		static_cast<FlowTestCase1844Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<FlowTestCase1844Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1844ActorState(); static_cast<FlowTestCase1844Actor*>(this)->destroy(); return 0; }
															#line 11714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<FlowTestCase1844Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1844ActorState();
		static_cast<FlowTestCase1844Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<FlowTestCase1844Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1844ActorState(); static_cast<FlowTestCase1844Actor*>(this)->destroy(); return 0; }
															#line 11726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<FlowTestCase1844Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1844ActorState();
		static_cast<FlowTestCase1844Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase1844Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1844Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1844Actor*>(this)->ActorCallback< FlowTestCase1844Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1844Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1844", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1844", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1844Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1844", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1844", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1844Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1844", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1844", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	UnitTestParameters params;
															#line 11799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1844()
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1844Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1844Actor, 0, Void >, public FastAllocated<FlowTestCase1844Actor>, public FlowTestCase1844ActorState<FlowTestCase1844Actor> {
															#line 11804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1844Actor>::operator new;
	using FastAllocated<FlowTestCase1844Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1844Actor, 0, Void >;
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1844Actor(UnitTestParameters const& params) 
															#line 11815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1844ActorState<FlowTestCase1844Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1844", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1844");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1844", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1844Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
static Future<Void> flowTestCase1844( UnitTestParameters const& params ) {
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new FlowTestCase1844Actor(params));
															#line 11842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1844, "/backup/containers/localdir/unencrypted")

#line 1848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 11848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via flowTestCase1849()
															#line 1849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class FlowTestCase1849Actor>
															#line 1849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1849ActorState {
															#line 11854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1849ActorState(UnitTestParameters const& params) 
															#line 1849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : params(params)
															#line 11861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1849", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1849ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1849", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			StrictFuture<Void> __when_expr_0 = testBackupContainer(format("file://%s/fdb_backups/%llx", params.getDataDir().c_str(), timer_int()), {}, format("%s/test_encryption_key", params.getDataDir().c_str()));
															#line 1850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (static_cast<FlowTestCase1849Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase1849Actor*>(this)->actor_wait_state = 1;
															#line 1850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1849Actor, 0, Void >*>(static_cast<FlowTestCase1849Actor*>(this)));
															#line 11883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~FlowTestCase1849ActorState();
		static_cast<FlowTestCase1849Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<FlowTestCase1849Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1849ActorState(); static_cast<FlowTestCase1849Actor*>(this)->destroy(); return 0; }
															#line 11906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<FlowTestCase1849Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1849ActorState();
		static_cast<FlowTestCase1849Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<FlowTestCase1849Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1849ActorState(); static_cast<FlowTestCase1849Actor*>(this)->destroy(); return 0; }
															#line 11918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<FlowTestCase1849Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1849ActorState();
		static_cast<FlowTestCase1849Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase1849Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1849Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1849Actor*>(this)->ActorCallback< FlowTestCase1849Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1849Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1849", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1849", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1849Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1849", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1849", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1849Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1849", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1849", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	UnitTestParameters params;
															#line 11991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1849()
															#line 1849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1849Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1849Actor, 0, Void >, public FastAllocated<FlowTestCase1849Actor>, public FlowTestCase1849ActorState<FlowTestCase1849Actor> {
															#line 11996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1849Actor>::operator new;
	using FastAllocated<FlowTestCase1849Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1849Actor, 0, Void >;
															#line 1849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1849Actor(UnitTestParameters const& params) 
															#line 12007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1849ActorState<FlowTestCase1849Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1849", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1849");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1849", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1849Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
static Future<Void> flowTestCase1849( UnitTestParameters const& params ) {
															#line 1849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new FlowTestCase1849Actor(params));
															#line 12034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1849, "/backup/containers/localdir/encrypted")

#line 1855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 12040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via flowTestCase1856()
															#line 1856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class FlowTestCase1856Actor>
															#line 1856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1856ActorState {
															#line 12046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1856ActorState(UnitTestParameters const& params) 
															#line 1856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : params(params)
															#line 12053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1856", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1856ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1856", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!g_network->isSimulated())
															#line 12068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				const char* url = getenv("FDB_TEST_BACKUP_URL");
															#line 1859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				ASSERT(url != nullptr);
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<Void> __when_expr_0 = testBackupContainer(url, {}, {});
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<FlowTestCase1856Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12078 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<FlowTestCase1856Actor*>(this)->actor_wait_state = 1;
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1856Actor, 0, Void >*>(static_cast<FlowTestCase1856Actor*>(this)));
															#line 12083 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~FlowTestCase1856ActorState();
		static_cast<FlowTestCase1856Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<FlowTestCase1856Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1856ActorState(); static_cast<FlowTestCase1856Actor*>(this)->destroy(); return 0; }
															#line 12111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<FlowTestCase1856Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1856ActorState();
		static_cast<FlowTestCase1856Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase1856Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1856Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1856Actor*>(this)->ActorCallback< FlowTestCase1856Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1856Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1856", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1856", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1856Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase1856", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1856", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1856Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1856", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1856", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	UnitTestParameters params;
															#line 12196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1856()
															#line 1856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1856Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1856Actor, 0, Void >, public FastAllocated<FlowTestCase1856Actor>, public FlowTestCase1856ActorState<FlowTestCase1856Actor> {
															#line 12201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1856Actor>::operator new;
	using FastAllocated<FlowTestCase1856Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1856Actor, 0, Void >;
															#line 1856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1856Actor(UnitTestParameters const& params) 
															#line 12212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1856ActorState<FlowTestCase1856Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1856", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1856");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1856", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1856Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
static Future<Void> flowTestCase1856( UnitTestParameters const& params ) {
															#line 1856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new FlowTestCase1856Actor(params));
															#line 12239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1856, "/backup/containers/url")

#line 1864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 12245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via flowTestCase1865()
															#line 1865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class FlowTestCase1865Actor>
															#line 1865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1865ActorState {
															#line 12251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1865ActorState(UnitTestParameters const& params) 
															#line 1865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : params(params)
															#line 12258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1865", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1865ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1865", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!g_network->isSimulated())
															#line 12273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			{
															#line 1867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				url = getenv("FDB_TEST_BACKUP_URL");
															#line 1868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				ASSERT(url != nullptr);
															#line 1869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				printf("Listing %s\n", url);
															#line 1870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				StrictFuture<std::vector<std::string>> __when_expr_0 = IBackupContainer::listContainers(url, {});
															#line 1870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				if (static_cast<FlowTestCase1865Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<FlowTestCase1865Actor*>(this)->actor_wait_state = 1;
															#line 1870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase1865Actor, 0, std::vector<std::string> >*>(static_cast<FlowTestCase1865Actor*>(this)));
															#line 12290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		this->~FlowTestCase1865ActorState();
		static_cast<FlowTestCase1865Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		if (!static_cast<FlowTestCase1865Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1865ActorState(); static_cast<FlowTestCase1865Actor*>(this)->destroy(); return 0; }
															#line 12318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		new (&static_cast<FlowTestCase1865Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase1865ActorState();
		static_cast<FlowTestCase1865Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(std::vector<std::string> const& urls,int loopDepth) 
	{
															#line 1871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto& u : urls ) {
															#line 1872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			printf("%s\n", u.c_str());
															#line 12332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(std::vector<std::string> && urls,int loopDepth) 
	{
															#line 1871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		for( auto& u : urls ) {
															#line 1872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			printf("%s\n", u.c_str());
															#line 12344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
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
		if (static_cast<FlowTestCase1865Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase1865Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase1865Actor*>(this)->ActorCallback< FlowTestCase1865Actor, 0, std::vector<std::string> >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase1865Actor, 0, std::vector<std::string> >*,std::vector<std::string> const& value) 
	{
		fdb_probe_actor_enter("flowTestCase1865", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1865", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase1865Actor, 0, std::vector<std::string> >*,std::vector<std::string> && value) 
	{
		fdb_probe_actor_enter("flowTestCase1865", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1865", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase1865Actor, 0, std::vector<std::string> >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase1865", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase1865", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	UnitTestParameters params;
															#line 1867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	const char* url;
															#line 12417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1865()
															#line 1865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1865Actor final : public Actor<Void>, public ActorCallback< FlowTestCase1865Actor, 0, std::vector<std::string> >, public FastAllocated<FlowTestCase1865Actor>, public FlowTestCase1865ActorState<FlowTestCase1865Actor> {
															#line 12422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1865Actor>::operator new;
	using FastAllocated<FlowTestCase1865Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase1865Actor, 0, std::vector<std::string> >;
															#line 1865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1865Actor(UnitTestParameters const& params) 
															#line 12433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1865ActorState<FlowTestCase1865Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1865", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1865");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1865", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase1865Actor, 0, std::vector<std::string> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
static Future<Void> flowTestCase1865( UnitTestParameters const& params ) {
															#line 1865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new FlowTestCase1865Actor(params));
															#line 12460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1865, "/backup/containers_list")

#line 1877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 12466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via flowTestCase1878()
															#line 1878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class FlowTestCase1878Actor>
															#line 1878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1878ActorState {
															#line 12472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1878ActorState(UnitTestParameters const& params) 
															#line 1878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : params(params)
															#line 12479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1878", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1878ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1878", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			for(int i = 0;i < 1000;++i) {
															#line 1881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				int64_t ts = deterministicRandom()->randomInt64(0, std::numeric_limits<int32_t>::max());
															#line 1882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
				ASSERT(BackupAgentBase::parseTime(BackupAgentBase::formatTime(ts)) == ts);
															#line 12498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			}
															#line 1885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupAgentBase::parseTime("2019/03/18.17:51:11-0600") == BackupAgentBase::parseTime("2019/03/18.16:51:11-0700"));
															#line 1887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupAgentBase::parseTime("2019/03/31.22:45:07-0700") == BackupAgentBase::parseTime("2019/04/01.03:45:07-0200"));
															#line 1889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupAgentBase::parseTime("2019/03/31.22:45:07+0000") == BackupAgentBase::parseTime("2019/04/01.03:45:07+0500"));
															#line 1891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupAgentBase::parseTime("2019/03/31.22:45:07+0030") == BackupAgentBase::parseTime("2019/04/01.03:45:07+0530"));
															#line 1893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupAgentBase::parseTime("2019/03/31.22:45:07+0030") == BackupAgentBase::parseTime("2019/04/01.04:00:07+0545"));
															#line 1896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<FlowTestCase1878Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1878ActorState(); static_cast<FlowTestCase1878Actor*>(this)->destroy(); return 0; }
															#line 12512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<FlowTestCase1878Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase1878ActorState();
			static_cast<FlowTestCase1878Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase1878ActorState();
		static_cast<FlowTestCase1878Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 1878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	UnitTestParameters params;
															#line 12536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1878()
															#line 1878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1878Actor final : public Actor<Void>, public FastAllocated<FlowTestCase1878Actor>, public FlowTestCase1878ActorState<FlowTestCase1878Actor> {
															#line 12541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1878Actor>::operator new;
	using FastAllocated<FlowTestCase1878Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 1878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1878Actor(UnitTestParameters const& params) 
															#line 12551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1878ActorState<FlowTestCase1878Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1878", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1878");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1878", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
															#line 1878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
static Future<Void> flowTestCase1878( UnitTestParameters const& params ) {
															#line 1878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new FlowTestCase1878Actor(params));
															#line 12577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1878, "/backup/time")

#line 1898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

															#line 12583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
// This generated class is to be used only via flowTestCase1899()
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
template <class FlowTestCase1899Actor>
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1899ActorState {
															#line 12589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1899ActorState(UnitTestParameters const& params) 
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
		 : params(params)
															#line 12596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase1899", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase1899ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase1899", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::vector<LogFile> files;
															#line 1903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 0, 100, 10, "file1", 100, 0, 2 });
															#line 1904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 0, 99));
															#line 1905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 0) == 0);
															#line 1907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 0, 100, 10, "file2", 200, 1, 2 });
															#line 1908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(files.begin(), files.end());
															#line 1909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 0, 99));
															#line 1910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 0, 100));
															#line 1911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 0) == 99);
															#line 1914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 100, 200, 10, "file3", 200, 0, 3 });
															#line 1915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 100, 250, 10, "file4", 200, 1, 3 });
															#line 1916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(files.begin(), files.end());
															#line 1917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 0, 99));
															#line 1918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 0, 100));
															#line 1919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 50, 150));
															#line 1920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 0) == 99);
															#line 1922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 100, 300, 10, "file5", 200, 2, 3 });
															#line 1923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(files.begin(), files.end());
															#line 1924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 50, 150));
															#line 1925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 50, 200));
															#line 1926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 10, 199));
															#line 1927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 0) == 199);
															#line 1928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 100) == 199);
															#line 1930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 250, 300, 10, "file6", 200, 0, 3 });
															#line 1931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(files.begin(), files.end());
															#line 1932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 50, 240));
															#line 1933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 100, 280));
															#line 1934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 99) == 199);
															#line 1936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 250, 300, 10, "file7", 200, 1, 3 });
															#line 1937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(files.begin(), files.end());
															#line 1938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(!BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 100, 280));
															#line 1940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 200, 250, 10, "file8", 200, 0, 3 });
															#line 1941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(files.begin(), files.end());
															#line 1942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 0, 299));
															#line 1943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 100, 280));
															#line 1944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 150) == 299);
															#line 1948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			files.push_back({ 300, 400, 10, "file10", 200, 0, 1 });
															#line 1949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			std::sort(files.begin(), files.end());
															#line 1950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 0, 399));
															#line 1951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 100, 399));
															#line 1952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 150, 399));
															#line 1953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::isPartitionedLogsContinuous(files, 250, 399));
															#line 1954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 0) == 399);
															#line 1955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 99) == 399);
															#line 1956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			ASSERT(BackupContainerFileSystemImpl::getPartitionedLogsContinuousEndVersion(files, 250) == 399);
															#line 1958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
			if (!static_cast<FlowTestCase1899Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase1899ActorState(); static_cast<FlowTestCase1899Actor*>(this)->destroy(); return 0; }
															#line 12701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
			new (&static_cast<FlowTestCase1899Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase1899ActorState();
			static_cast<FlowTestCase1899Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase1899ActorState();
		static_cast<FlowTestCase1899Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	UnitTestParameters params;
															#line 12725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase1899()
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
class FlowTestCase1899Actor final : public Actor<Void>, public FastAllocated<FlowTestCase1899Actor>, public FlowTestCase1899ActorState<FlowTestCase1899Actor> {
															#line 12730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase1899Actor>::operator new;
	using FastAllocated<FlowTestCase1899Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	FlowTestCase1899Actor(UnitTestParameters const& params) 
															#line 12740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase1899ActorState<FlowTestCase1899Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase1899", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase1899");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase1899", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
static Future<Void> flowTestCase1899( UnitTestParameters const& params ) {
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"
	return Future<Void>(new FlowTestCase1899Actor(params));
															#line 12766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase1899, "/backup/continuous")

#line 1960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerFileSystem.actor.cpp"

} // namespace backup_test
