#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
/*
 * BlobGranuleReader.actor.cpp
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

#include <map>
#include <vector>

#include "fmt/format.h"
#include "fdbclient/AsyncFileS3BlobStore.actor.h"
#include "fdbclient/BlobGranuleCommon.h"
#include "fdbclient/BlobGranuleFiles.h"
#include "fdbclient/BlobGranuleReader.actor.h"
#include "fdbclient/BlobWorkerCommon.h"
#include "fdbclient/BlobWorkerInterface.h"
#include "fdbclient/FDBTypes.h"
#include "flow/actorcompiler.h" // This must be the last #include.

															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
namespace {
// This generated class is to be used only via readFile()
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
template <class ReadFileActor>
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
class ReadFileActorState {
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
public:
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	ReadFileActorState(Reference<BlobConnectionProvider> const& bstoreProvider,BlobFilePointerRef const& f) 
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		 : bstoreProvider(bstoreProvider),
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		   f(f)
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
	{
		fdb_probe_actor_create("readFile", reinterpret_cast<unsigned long>(this));

	}
	~ReadFileActorState() 
	{
		fdb_probe_actor_destroy("readFile", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				arena = Arena();
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				std::string fname = f.filename.toString();
															#line 38 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				bstore = bstoreProvider->getForRead(fname);
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				StrictFuture<Reference<IAsyncFile>> __when_expr_0 = bstore->readFile(fname);
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				if (static_cast<ReadFileActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ReadFileActor*>(this)->actor_wait_state = 1;
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadFileActor, 0, Reference<IAsyncFile> >*>(static_cast<ReadFileActor*>(this)));
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
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
		this->~ReadFileActorState();
		static_cast<ReadFileActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		data = new (arena) uint8_t[f.length];
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		StrictFuture<int> __when_expr_1 = reader->read(data, f.length, f.offset);
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		if (static_cast<ReadFileActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadFileActor*>(this)->actor_wait_state = 2;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadFileActor, 1, int >*>(static_cast<ReadFileActor*>(this)));
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> const& __reader,int loopDepth) 
	{
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		reader = __reader;
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> && __reader,int loopDepth) 
	{
		reader = std::move(__reader);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadFileActor*>(this)->actor_wait_state > 0) static_cast<ReadFileActor*>(this)->actor_wait_state = 0;
		static_cast<ReadFileActor*>(this)->ActorCallback< ReadFileActor, 0, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< ReadFileActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("readFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadFileActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("readFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadFileActor, 0, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("readFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readFile", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int const& readSize,int loopDepth) 
	{
															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		ASSERT(f.length == readSize);
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		StringRef dataRef(data, f.length);
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		if (!static_cast<ReadFileActor*>(this)->SAV<Standalone<StringRef>>::futures) { (void)(Standalone<StringRef>(dataRef, arena)); this->~ReadFileActorState(); static_cast<ReadFileActor*>(this)->destroy(); return 0; }
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		new (&static_cast<ReadFileActor*>(this)->SAV< Standalone<StringRef> >::value()) Standalone<StringRef>(Standalone<StringRef>(dataRef, arena));
		this->~ReadFileActorState();
		static_cast<ReadFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(int && readSize,int loopDepth) 
	{
															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		ASSERT(f.length == readSize);
															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		StringRef dataRef(data, f.length);
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		if (!static_cast<ReadFileActor*>(this)->SAV<Standalone<StringRef>>::futures) { (void)(Standalone<StringRef>(dataRef, arena)); this->~ReadFileActorState(); static_cast<ReadFileActor*>(this)->destroy(); return 0; }
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		new (&static_cast<ReadFileActor*>(this)->SAV< Standalone<StringRef> >::value()) Standalone<StringRef>(Standalone<StringRef>(dataRef, arena));
		this->~ReadFileActorState();
		static_cast<ReadFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(int const& readSize,int loopDepth) 
	{
		loopDepth = a_body1cont3(readSize, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(int && readSize,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(readSize), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReadFileActor*>(this)->actor_wait_state > 0) static_cast<ReadFileActor*>(this)->actor_wait_state = 0;
		static_cast<ReadFileActor*>(this)->ActorCallback< ReadFileActor, 1, int >::remove();

	}
	void a_callback_fire(ActorCallback< ReadFileActor, 1, int >*,int const& value) 
	{
		fdb_probe_actor_enter("readFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadFileActor, 1, int >*,int && value) 
	{
		fdb_probe_actor_enter("readFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadFileActor, 1, int >*,Error err) 
	{
		fdb_probe_actor_enter("readFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readFile", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	Reference<BlobConnectionProvider> bstoreProvider;
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	BlobFilePointerRef f;
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	Arena arena;
															#line 38 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	Reference<BackupContainerFileSystem> bstore;
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	Reference<IAsyncFile> reader;
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	uint8_t* data;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
};
// This generated class is to be used only via readFile()
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
class ReadFileActor final : public Actor<Standalone<StringRef>>, public ActorCallback< ReadFileActor, 0, Reference<IAsyncFile> >, public ActorCallback< ReadFileActor, 1, int >, public FastAllocated<ReadFileActor>, public ReadFileActorState<ReadFileActor> {
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
public:
	using FastAllocated<ReadFileActor>::operator new;
	using FastAllocated<ReadFileActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Standalone<StringRef>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadFileActor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< ReadFileActor, 1, int >;
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	ReadFileActor(Reference<BlobConnectionProvider> const& bstoreProvider,BlobFilePointerRef const& f) 
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		 : Actor<Standalone<StringRef>>(),
		   ReadFileActorState<ReadFileActor>(bstoreProvider, f)
	{
		fdb_probe_actor_enter("readFile", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readFile");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readFile", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadFileActor, 0, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReadFileActor, 1, int >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
[[nodiscard]] Future<Standalone<StringRef>> readFile( Reference<BlobConnectionProvider> const& bstoreProvider, BlobFilePointerRef const& f ) {
															#line 34 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	return Future<Standalone<StringRef>>(new ReadFileActor(bstoreProvider, f));
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
}

#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"

// TODO: improve the interface of this function so that it doesn't need
//       to be passed the entire BlobWorkerStats object

// FIXME: probably want to chunk this up with yields to avoid slow task for blob worker re-snapshotting by calling the
// sub-functions that BlobGranuleFiles actually exposes?
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
namespace {
// This generated class is to be used only via readBlobGranule()
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
template <class ReadBlobGranuleActor>
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
class ReadBlobGranuleActorState {
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
public:
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	ReadBlobGranuleActorState(BlobGranuleChunkRef const& chunk,KeyRangeRef const& keyRange,Version const& beginVersion,Version const& readVersion,Reference<BlobConnectionProvider> const& bstore,Optional<BlobWorkerStats*> const& stats) 
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		 : chunk(chunk),
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		   keyRange(keyRange),
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		   beginVersion(beginVersion),
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		   readVersion(readVersion),
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		   bstore(bstore),
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		   stats(stats)
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
	{
		fdb_probe_actor_create("readBlobGranule", reinterpret_cast<unsigned long>(this));

	}
	~ReadBlobGranuleActorState() 
	{
		fdb_probe_actor_destroy("readBlobGranule", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
			ASSERT(readVersion == chunk.includedVersion);
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
			arena = Arena();
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
			try {
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				Future<Standalone<StringRef>> readSnapshotFuture;
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				if (chunk.snapshotFile.present())
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
				{
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
					readSnapshotFuture = readFile(bstore, chunk.snapshotFile.get());
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
					if (stats.present())
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
					{
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
						++stats.get()->s3GetReqs;
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
					}
				}
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				readDeltaFutures = std::vector<Future<Standalone<StringRef>>>();
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				readDeltaFutures.reserve(chunk.deltaFiles.size());
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				for( BlobFilePointerRef deltaFile : chunk.deltaFiles ) {
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
					readDeltaFutures.push_back(readFile(bstore, deltaFile));
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
					if (stats.present())
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
					{
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
						++stats.get()->s3GetReqs;
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
					}
				}
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				snapshotData = Optional<StringRef>();
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				if (chunk.snapshotFile.present())
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
				{
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
					StrictFuture<Standalone<StringRef>> __when_expr_0 = readSnapshotFuture;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
					if (static_cast<ReadBlobGranuleActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<ReadBlobGranuleActor*>(this)->actor_wait_state = 1;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadBlobGranuleActor, 0, Standalone<StringRef> >*>(static_cast<ReadBlobGranuleActor*>(this)));
															#line 461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
					loopDepth = 0;
				}
				else
				{
					loopDepth = a_body1cont2(loopDepth);
				}
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
		this->~ReadBlobGranuleActorState();
		static_cast<ReadBlobGranuleActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		numDeltaFiles = chunk.deltaFiles.size();
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		deltaData = std::vector<StringRef>();
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		deltaIdx = int();
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		deltaData.reserve(numDeltaFiles);
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		deltaIdx = 0;
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		loopDepth = a_body1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(int loopDepth) 
	{
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		arena.dependsOn(s.arena());
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		snapshotData = s;
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Standalone<StringRef> const& __s,int loopDepth) 
	{
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		s = __s;
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Standalone<StringRef> && __s,int loopDepth) 
	{
		s = std::move(__s);
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadBlobGranuleActor*>(this)->actor_wait_state > 0) static_cast<ReadBlobGranuleActor*>(this)->actor_wait_state = 0;
		static_cast<ReadBlobGranuleActor*>(this)->ActorCallback< ReadBlobGranuleActor, 0, Standalone<StringRef> >::remove();

	}
	void a_callback_fire(ActorCallback< ReadBlobGranuleActor, 0, Standalone<StringRef> >*,Standalone<StringRef> const& value) 
	{
		fdb_probe_actor_enter("readBlobGranule", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readBlobGranule", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadBlobGranuleActor, 0, Standalone<StringRef> >*,Standalone<StringRef> && value) 
	{
		fdb_probe_actor_enter("readBlobGranule", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readBlobGranule", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadBlobGranuleActor, 0, Standalone<StringRef> >*,Error err) 
	{
		fdb_probe_actor_enter("readBlobGranule", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readBlobGranule", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont9(int loopDepth) 
	{
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		GranuleMaterializeStats stats;
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		if (!static_cast<ReadBlobGranuleActor*>(this)->SAV<RangeResult>::futures) { (void)(materializeBlobGranule(chunk, keyRange, beginVersion, readVersion, snapshotData, deltaData, stats)); this->~ReadBlobGranuleActorState(); static_cast<ReadBlobGranuleActor*>(this)->destroy(); return 0; }
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		new (&static_cast<ReadBlobGranuleActor*>(this)->SAV< RangeResult >::value()) RangeResult(materializeBlobGranule(chunk, keyRange, beginVersion, readVersion, snapshotData, deltaData, stats));
		this->~ReadBlobGranuleActorState();
		static_cast<ReadBlobGranuleActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		if (!(deltaIdx < numDeltaFiles))
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		{
			return a_body1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		StrictFuture<Standalone<StringRef>> __when_expr_1 = readDeltaFutures[deltaIdx];
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		if (static_cast<ReadBlobGranuleActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadBlobGranuleActor*>(this)->actor_wait_state = 2;
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadBlobGranuleActor, 1, Standalone<StringRef> >*>(static_cast<ReadBlobGranuleActor*>(this)));
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2break1(int loopDepth) 
	{
		try {
			return a_body1cont9(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2loopBody1cont1(Standalone<StringRef> const& data,int loopDepth) 
	{
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		deltaData.push_back(data);
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		arena.dependsOn(data.arena());
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		deltaIdx++;
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont1(Standalone<StringRef> && data,int loopDepth) 
	{
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		deltaData.push_back(data);
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		arena.dependsOn(data.arena());
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		deltaIdx++;
															#line 678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1cont2loopBody1when1(Standalone<StringRef> const& data,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont1(data, loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1when1(Standalone<StringRef> && data,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont1(std::move(data), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReadBlobGranuleActor*>(this)->actor_wait_state > 0) static_cast<ReadBlobGranuleActor*>(this)->actor_wait_state = 0;
		static_cast<ReadBlobGranuleActor*>(this)->ActorCallback< ReadBlobGranuleActor, 1, Standalone<StringRef> >::remove();

	}
	void a_callback_fire(ActorCallback< ReadBlobGranuleActor, 1, Standalone<StringRef> >*,Standalone<StringRef> const& value) 
	{
		fdb_probe_actor_enter("readBlobGranule", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readBlobGranule", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadBlobGranuleActor, 1, Standalone<StringRef> >*,Standalone<StringRef> && value) 
	{
		fdb_probe_actor_enter("readBlobGranule", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readBlobGranule", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadBlobGranuleActor, 1, Standalone<StringRef> >*,Error err) 
	{
		fdb_probe_actor_enter("readBlobGranule", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readBlobGranule", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	BlobGranuleChunkRef chunk;
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	KeyRangeRef keyRange;
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	Version beginVersion;
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	Version readVersion;
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	Reference<BlobConnectionProvider> bstore;
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	Optional<BlobWorkerStats*> stats;
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	Arena arena;
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	std::vector<Future<Standalone<StringRef>>> readDeltaFutures;
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	Optional<StringRef> snapshotData;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	Standalone<StringRef> s;
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	int numDeltaFiles;
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	std::vector<StringRef> deltaData;
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	int deltaIdx;
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
};
// This generated class is to be used only via readBlobGranule()
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
class ReadBlobGranuleActor final : public Actor<RangeResult>, public ActorCallback< ReadBlobGranuleActor, 0, Standalone<StringRef> >, public ActorCallback< ReadBlobGranuleActor, 1, Standalone<StringRef> >, public FastAllocated<ReadBlobGranuleActor>, public ReadBlobGranuleActorState<ReadBlobGranuleActor> {
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
public:
	using FastAllocated<ReadBlobGranuleActor>::operator new;
	using FastAllocated<ReadBlobGranuleActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadBlobGranuleActor, 0, Standalone<StringRef> >;
friend struct ActorCallback< ReadBlobGranuleActor, 1, Standalone<StringRef> >;
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	ReadBlobGranuleActor(BlobGranuleChunkRef const& chunk,KeyRangeRef const& keyRange,Version const& beginVersion,Version const& readVersion,Reference<BlobConnectionProvider> const& bstore,Optional<BlobWorkerStats*> const& stats) 
															#line 789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		 : Actor<RangeResult>(),
		   ReadBlobGranuleActorState<ReadBlobGranuleActor>(chunk, keyRange, beginVersion, readVersion, bstore, stats)
	{
		fdb_probe_actor_enter("readBlobGranule", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readBlobGranule");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readBlobGranule", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadBlobGranuleActor, 0, Standalone<StringRef> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReadBlobGranuleActor, 1, Standalone<StringRef> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
[[nodiscard]] Future<RangeResult> readBlobGranule( BlobGranuleChunkRef const& chunk, KeyRangeRef const& keyRange, Version const& beginVersion, Version const& readVersion, Reference<BlobConnectionProvider> const& bstore, Optional<BlobWorkerStats*> const& stats ) {
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	return Future<RangeResult>(new ReadBlobGranuleActor(chunk, keyRange, beginVersion, readVersion, bstore, stats));
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
}

#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"

// TODO probably should add things like limit/bytelimit at some point?
															#line 824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
namespace {
// This generated class is to be used only via readBlobGranules()
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
template <class ReadBlobGranulesActor>
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
class ReadBlobGranulesActorState {
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
public:
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	ReadBlobGranulesActorState(BlobGranuleFileRequest const& request,BlobGranuleFileReply const& reply,Reference<BlobConnectionProvider> const& bstore,PromiseStream<RangeResult> const& results) 
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		 : request(request),
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		   reply(reply),
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		   bstore(bstore),
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		   results(results)
															#line 844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
	{
		fdb_probe_actor_create("readBlobGranules", reinterpret_cast<unsigned long>(this));

	}
	~ReadBlobGranulesActorState() 
	{
		fdb_probe_actor_destroy("readBlobGranules", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				i = int();
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				i = 0;
															#line 862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
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
		this->~ReadBlobGranulesActorState();
		static_cast<ReadBlobGranulesActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		if (!static_cast<ReadBlobGranulesActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadBlobGranulesActorState(); static_cast<ReadBlobGranulesActor*>(this)->destroy(); return 0; }
															#line 891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		new (&static_cast<ReadBlobGranulesActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ReadBlobGranulesActorState();
		static_cast<ReadBlobGranulesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
			results.sendError(e);
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
			loopDepth = a_body1cont1(loopDepth);
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
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		results.sendError(end_of_stream());
															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		loopDepth = a_body1cont4(loopDepth);

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
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		if (!(i < reply.chunks.size()))
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		StrictFuture<RangeResult> __when_expr_0 = readBlobGranule(reply.chunks[i], request.keyRange, request.beginVersion, request.readVersion, bstore);
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		if (static_cast<ReadBlobGranulesActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ReadBlobGranulesActor*>(this)->actor_wait_state = 1;
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadBlobGranulesActor, 0, RangeResult >*>(static_cast<ReadBlobGranulesActor*>(this)));
															#line 948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
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
	int a_body1loopBody1cont1(RangeResult const& chunkResult,int loopDepth) 
	{
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		results.send(std::move(chunkResult));
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		i++;
															#line 972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(RangeResult && chunkResult,int loopDepth) 
	{
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		results.send(std::move(chunkResult));
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		i++;
															#line 983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& chunkResult,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(chunkResult, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && chunkResult,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(chunkResult), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadBlobGranulesActor*>(this)->actor_wait_state > 0) static_cast<ReadBlobGranulesActor*>(this)->actor_wait_state = 0;
		static_cast<ReadBlobGranulesActor*>(this)->ActorCallback< ReadBlobGranulesActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ReadBlobGranulesActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("readBlobGranules", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readBlobGranules", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadBlobGranulesActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("readBlobGranules", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readBlobGranules", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadBlobGranulesActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("readBlobGranules", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readBlobGranules", reinterpret_cast<unsigned long>(this), 0);

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
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	BlobGranuleFileRequest request;
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	BlobGranuleFileReply reply;
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	Reference<BlobConnectionProvider> bstore;
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	PromiseStream<RangeResult> results;
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	int i;
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
};
// This generated class is to be used only via readBlobGranules()
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
class ReadBlobGranulesActor final : public Actor<Void>, public ActorCallback< ReadBlobGranulesActor, 0, RangeResult >, public FastAllocated<ReadBlobGranulesActor>, public ReadBlobGranulesActorState<ReadBlobGranulesActor> {
															#line 1079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
public:
	using FastAllocated<ReadBlobGranulesActor>::operator new;
	using FastAllocated<ReadBlobGranulesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadBlobGranulesActor, 0, RangeResult >;
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	ReadBlobGranulesActor(BlobGranuleFileRequest const& request,BlobGranuleFileReply const& reply,Reference<BlobConnectionProvider> const& bstore,PromiseStream<RangeResult> const& results) 
															#line 1090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		 : Actor<Void>(),
		   ReadBlobGranulesActorState<ReadBlobGranulesActor>(request, reply, bstore, results)
	{
		fdb_probe_actor_enter("readBlobGranules", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readBlobGranules");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readBlobGranules", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadBlobGranulesActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
[[nodiscard]] Future<Void> readBlobGranules( BlobGranuleFileRequest const& request, BlobGranuleFileReply const& reply, Reference<BlobConnectionProvider> const& bstore, PromiseStream<RangeResult> const& results ) {
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	return Future<Void>(new ReadBlobGranulesActor(request, reply, bstore, results));
															#line 1118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
}

#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"

// Return true if a given range is fully covered by blob chunks
bool isRangeFullyCovered(KeyRange range, Standalone<VectorRef<BlobGranuleChunkRef>> blobChunks) {
	std::vector<KeyRangeRef> blobRanges;
	for (const BlobGranuleChunkRef& chunk : blobChunks) {
		blobRanges.push_back(chunk.keyRange);
	}
	return range.isCovered(blobRanges);
}

void testAddChunkRange(KeyRef begin, KeyRef end, Standalone<VectorRef<BlobGranuleChunkRef>>& chunks) {
	BlobGranuleChunkRef chunk;
	chunk.keyRange = KeyRangeRef(begin, end);
	chunks.push_back(chunks.arena(), chunk);
}

															#line 1138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase152()
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
template <class FlowTestCase152Actor>
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
class FlowTestCase152ActorState {
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
public:
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	FlowTestCase152ActorState(UnitTestParameters const& params) 
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
		 : params(params)
															#line 1152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase152", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase152ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase152", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
			Standalone<VectorRef<BlobGranuleChunkRef>> chunks;
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
			testAddChunkRange("key_a1"_sr, "key_a9"_sr, chunks);
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
			testAddChunkRange("key_b1"_sr, "key_b9"_sr, chunks);
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
			{
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(isRangeFullyCovered(KeyRangeRef(), chunks) == false);
															#line 1175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
			}
			{
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				Standalone<VectorRef<BlobGranuleChunkRef>> empyChunks;
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(isRangeFullyCovered(KeyRangeRef(), empyChunks) == false);
															#line 1182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
			}
			{
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(isRangeFullyCovered(KeyRangeRef(""_sr, "\xff"_sr), chunks) == false);
															#line 1187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
			}
			{
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(isRangeFullyCovered(KeyRangeRef("key_a1"_sr, "key_a9"_sr), chunks));
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
			}
			{
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(isRangeFullyCovered(KeyRangeRef("key_a1"_sr, "key_a3"_sr), chunks));
															#line 1197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
			}
			{
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(isRangeFullyCovered(KeyRangeRef("key_a0"_sr, "key_a3"_sr), chunks) == false);
															#line 1202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
			}
			{
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				auto range = KeyRangeRef("key_a5"_sr, "key_b5"_sr);
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(isRangeFullyCovered(range, chunks) == false);
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(range.begin == "key_a5"_sr);
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(range.end == "key_b5"_sr);
															#line 1213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
			}
			{
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				Standalone<VectorRef<BlobGranuleChunkRef>> continuedChunks;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				testAddChunkRange("key_a1"_sr, "key_a9"_sr, continuedChunks);
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				testAddChunkRange("key_a9"_sr, "key_b1"_sr, continuedChunks);
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				testAddChunkRange("key_b1"_sr, "key_b9"_sr, continuedChunks);
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(isRangeFullyCovered(KeyRangeRef("key_a1"_sr, "key_b9"_sr), continuedChunks));
															#line 1226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
			}
			{
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				std::vector<KeyRangeRef> ranges;
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_a"_sr, "key_b"_sr));
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_x"_sr, "key_y"_sr));
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(KeyRangeRef("key_x"_sr, "key_y"_sr).isCovered(ranges));
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.clear();
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_a"_sr, "key_b"_sr));
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_v"_sr, "key_y"_sr));
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(KeyRangeRef("key_x"_sr, "key_y"_sr).isCovered(ranges));
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.clear();
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_a"_sr, "key_b"_sr));
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_x"_sr, "key_xa"_sr));
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_xa"_sr, "key_ya"_sr));
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(KeyRangeRef("key_x"_sr, "key_y"_sr).isCovered(ranges));
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.clear();
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_a"_sr, "key_b"_sr));
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_x"_sr, "key_xa"_sr));
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_xa"_sr, "key_xb"_sr));
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(!KeyRangeRef("key_x"_sr, "key_y"_sr).isCovered(ranges));
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.clear();
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_a"_sr, "key_b"_sr));
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_x"_sr, "key_xa"_sr));
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(!KeyRangeRef("key_x"_sr, "key_y"_sr).isCovered(ranges));
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.clear();
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_a"_sr, "key_b"_sr));
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_xa"_sr, "key_y"_sr));
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(!KeyRangeRef("key_x"_sr, "key_y"_sr).isCovered(ranges));
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.clear();
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_a"_sr, "key_b"_sr));
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ranges.push_back(KeyRangeRef("key_x"_sr, "key_y"_sr));
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
				ASSERT(!KeyRangeRef("key_a"_sr, "key_y"_sr).isCovered(ranges));
															#line 1289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
			}
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
			if (!static_cast<FlowTestCase152Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase152ActorState(); static_cast<FlowTestCase152Actor*>(this)->destroy(); return 0; }
															#line 1293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
			new (&static_cast<FlowTestCase152Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase152ActorState();
			static_cast<FlowTestCase152Actor*>(this)->finishSendAndDelPromiseRef();
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
		this->~FlowTestCase152ActorState();
		static_cast<FlowTestCase152Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	UnitTestParameters params;
															#line 1317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase152()
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
class FlowTestCase152Actor final : public Actor<Void>, public FastAllocated<FlowTestCase152Actor>, public FlowTestCase152ActorState<FlowTestCase152Actor> {
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase152Actor>::operator new;
	using FastAllocated<FlowTestCase152Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	FlowTestCase152Actor(UnitTestParameters const& params) 
															#line 1332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase152ActorState<FlowTestCase152Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase152", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase152");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase152", reinterpret_cast<unsigned long>(this), -1);

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
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
static Future<Void> flowTestCase152( UnitTestParameters const& params ) {
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
	return Future<Void>(new FlowTestCase152Actor(params));
															#line 1359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase152, "/fdbserver/blobgranule/isRangeCoveredByBlob")

#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BlobGranuleReader.actor.cpp"
