#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
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

// TODO more efficient data structure besides std::map? PTree is unnecessary since this isn't versioned, but some other
// sorted thing could work. And if it used arenas it'd probably be more efficient with allocations, since everything
// else is in 1 arena and discarded at the end.

// TODO could refactor the file reading code from here and the delta file function into another actor,
// then this part would also be testable? but meh

															#line 43 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
namespace {
// This generated class is to be used only via readFile()
															#line 41 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
template <class ReadFileActor>
															#line 41 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
class ReadFileActorState {
															#line 50 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
public:
															#line 41 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	ReadFileActorState(Reference<BackupContainerFileSystem> const& bstore,BlobFilePointerRef const& f) 
															#line 41 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
															#line 41 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		 : bstore(bstore),
															#line 41 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		   f(f)
															#line 59 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 43 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
				arena = Arena();
															#line 45 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
				StrictFuture<Reference<IAsyncFile>> __when_expr_0 = bstore->readFile(f.filename.toString());
															#line 45 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
				if (static_cast<ReadFileActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 79 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ReadFileActor*>(this)->actor_wait_state = 1;
															#line 45 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadFileActor, 0, Reference<IAsyncFile> >*>(static_cast<ReadFileActor*>(this)));
															#line 84 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 56 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 114 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 47 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		data = new (arena) uint8_t[f.length];
															#line 49 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		StrictFuture<int> __when_expr_1 = reader->read(data, f.length, f.offset);
															#line 49 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		if (static_cast<ReadFileActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 132 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadFileActor*>(this)->actor_wait_state = 2;
															#line 49 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadFileActor, 1, int >*>(static_cast<ReadFileActor*>(this)));
															#line 137 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> const& __reader,int loopDepth) 
	{
															#line 45 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		reader = __reader;
															#line 146 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 51 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		ASSERT(f.length == readSize);
															#line 53 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		StringRef dataRef(data, f.length);
															#line 54 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		if (!static_cast<ReadFileActor*>(this)->SAV<Standalone<StringRef>>::futures) { (void)(Standalone<StringRef>(dataRef, arena)); this->~ReadFileActorState(); static_cast<ReadFileActor*>(this)->destroy(); return 0; }
															#line 217 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
		new (&static_cast<ReadFileActor*>(this)->SAV< Standalone<StringRef> >::value()) Standalone<StringRef>(Standalone<StringRef>(dataRef, arena));
		this->~ReadFileActorState();
		static_cast<ReadFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(int && readSize,int loopDepth) 
	{
															#line 51 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		ASSERT(f.length == readSize);
															#line 53 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		StringRef dataRef(data, f.length);
															#line 54 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		if (!static_cast<ReadFileActor*>(this)->SAV<Standalone<StringRef>>::futures) { (void)(Standalone<StringRef>(dataRef, arena)); this->~ReadFileActorState(); static_cast<ReadFileActor*>(this)->destroy(); return 0; }
															#line 233 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 41 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	Reference<BackupContainerFileSystem> bstore;
															#line 41 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	BlobFilePointerRef f;
															#line 43 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	Arena arena;
															#line 45 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	Reference<IAsyncFile> reader;
															#line 47 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	uint8_t* data;
															#line 314 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
};
// This generated class is to be used only via readFile()
															#line 41 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
class ReadFileActor final : public Actor<Standalone<StringRef>>, public ActorCallback< ReadFileActor, 0, Reference<IAsyncFile> >, public ActorCallback< ReadFileActor, 1, int >, public FastAllocated<ReadFileActor>, public ReadFileActorState<ReadFileActor> {
															#line 319 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
public:
	using FastAllocated<ReadFileActor>::operator new;
	using FastAllocated<ReadFileActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Standalone<StringRef>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadFileActor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< ReadFileActor, 1, int >;
															#line 41 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	ReadFileActor(Reference<BackupContainerFileSystem> const& bstore,BlobFilePointerRef const& f) 
															#line 331 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
		 : Actor<Standalone<StringRef>>(),
		   ReadFileActorState<ReadFileActor>(bstore, f)
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
															#line 41 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
[[nodiscard]] Future<Standalone<StringRef>> readFile( Reference<BackupContainerFileSystem> const& bstore, BlobFilePointerRef const& f ) {
															#line 41 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	return Future<Standalone<StringRef>>(new ReadFileActor(bstore, f));
															#line 360 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
}

#line 59 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"

// TODO: improve the interface of this function so that it doesn't need
//       to be passed the entire BlobWorkerStats object

// FIXME: probably want to chunk this up with yields to avoid slow task for blob worker re-snapshotting by calling the
// sub-functions that BlobGranuleFiles actually exposes?
															#line 370 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
namespace {
// This generated class is to be used only via readBlobGranule()
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
template <class ReadBlobGranuleActor>
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
class ReadBlobGranuleActorState {
															#line 377 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
public:
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	ReadBlobGranuleActorState(BlobGranuleChunkRef const& chunk,KeyRangeRef const& keyRange,Version const& beginVersion,Version const& readVersion,Reference<BackupContainerFileSystem> const& bstore,Optional<BlobWorkerStats*> const& stats) 
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		 : chunk(chunk),
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		   keyRange(keyRange),
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		   beginVersion(beginVersion),
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		   readVersion(readVersion),
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		   bstore(bstore),
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		   stats(stats)
															#line 394 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 73 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
			ASSERT(readVersion == chunk.includedVersion);
															#line 75 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
			arena = Arena();
															#line 411 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
			try {
															#line 78 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
				Future<Standalone<StringRef>> readSnapshotFuture;
															#line 79 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
				if (chunk.snapshotFile.present())
															#line 417 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
				{
															#line 80 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
					readSnapshotFuture = readFile(bstore, chunk.snapshotFile.get());
															#line 81 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
					if (stats.present())
															#line 423 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
					{
															#line 82 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
						++stats.get()->s3GetReqs;
															#line 427 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
					}
				}
															#line 85 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
				readDeltaFutures = std::vector<Future<Standalone<StringRef>>>();
															#line 87 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
				readDeltaFutures.reserve(chunk.deltaFiles.size());
															#line 88 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
				for( BlobFilePointerRef deltaFile : chunk.deltaFiles ) {
															#line 89 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
					readDeltaFutures.push_back(readFile(bstore, deltaFile));
															#line 90 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
					if (stats.present())
															#line 440 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
					{
															#line 91 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
						++stats.get()->s3GetReqs;
															#line 444 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
					}
				}
															#line 95 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
				snapshotData = Optional<StringRef>();
															#line 96 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
				if (chunk.snapshotFile.present())
															#line 451 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
				{
															#line 97 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
					StrictFuture<Standalone<StringRef>> __when_expr_0 = readSnapshotFuture;
															#line 97 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
					if (static_cast<ReadBlobGranuleActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 457 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<ReadBlobGranuleActor*>(this)->actor_wait_state = 1;
															#line 97 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadBlobGranuleActor, 0, Standalone<StringRef> >*>(static_cast<ReadBlobGranuleActor*>(this)));
															#line 462 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 116 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 497 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 102 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		numDeltaFiles = chunk.deltaFiles.size();
															#line 103 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		deltaData = new (arena) StringRef[numDeltaFiles];
															#line 104 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		deltaIdx = int();
															#line 107 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		deltaIdx = 0;
															#line 517 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
		loopDepth = a_body1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(int loopDepth) 
	{
															#line 98 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		arena.dependsOn(s.arena());
															#line 99 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		snapshotData = s;
															#line 528 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Standalone<StringRef> const& __s,int loopDepth) 
	{
															#line 97 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		s = __s;
															#line 537 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 113 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		if (!static_cast<ReadBlobGranuleActor*>(this)->SAV<RangeResult>::futures) { (void)(materializeBlobGranule(chunk, keyRange, beginVersion, readVersion, snapshotData, deltaData)); this->~ReadBlobGranuleActorState(); static_cast<ReadBlobGranuleActor*>(this)->destroy(); return 0; }
															#line 604 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
		new (&static_cast<ReadBlobGranuleActor*>(this)->SAV< RangeResult >::value()) RangeResult(materializeBlobGranule(chunk, keyRange, beginVersion, readVersion, snapshotData, deltaData));
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
															#line 107 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		if (!(deltaIdx < numDeltaFiles))
															#line 623 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
		{
			return a_body1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 108 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		StrictFuture<Standalone<StringRef>> __when_expr_1 = readDeltaFutures[deltaIdx];
															#line 108 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		if (static_cast<ReadBlobGranuleActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 631 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont2loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadBlobGranuleActor*>(this)->actor_wait_state = 2;
															#line 108 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadBlobGranuleActor, 1, Standalone<StringRef> >*>(static_cast<ReadBlobGranuleActor*>(this)));
															#line 636 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 109 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		deltaData[deltaIdx] = data;
															#line 110 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		arena.dependsOn(data.arena());
															#line 107 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		deltaIdx++;
															#line 662 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont1(Standalone<StringRef> && data,int loopDepth) 
	{
															#line 109 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		deltaData[deltaIdx] = data;
															#line 110 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		arena.dependsOn(data.arena());
															#line 107 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		deltaIdx++;
															#line 675 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	BlobGranuleChunkRef chunk;
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	KeyRangeRef keyRange;
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	Version beginVersion;
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	Version readVersion;
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	Reference<BackupContainerFileSystem> bstore;
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	Optional<BlobWorkerStats*> stats;
															#line 75 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	Arena arena;
															#line 85 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	std::vector<Future<Standalone<StringRef>>> readDeltaFutures;
															#line 95 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	Optional<StringRef> snapshotData;
															#line 97 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	Standalone<StringRef> s;
															#line 102 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	int numDeltaFiles;
															#line 103 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	StringRef* deltaData;
															#line 104 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	int deltaIdx;
															#line 769 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
};
// This generated class is to be used only via readBlobGranule()
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
class ReadBlobGranuleActor final : public Actor<RangeResult>, public ActorCallback< ReadBlobGranuleActor, 0, Standalone<StringRef> >, public ActorCallback< ReadBlobGranuleActor, 1, Standalone<StringRef> >, public FastAllocated<ReadBlobGranuleActor>, public ReadBlobGranuleActorState<ReadBlobGranuleActor> {
															#line 774 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
public:
	using FastAllocated<ReadBlobGranuleActor>::operator new;
	using FastAllocated<ReadBlobGranuleActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadBlobGranuleActor, 0, Standalone<StringRef> >;
friend struct ActorCallback< ReadBlobGranuleActor, 1, Standalone<StringRef> >;
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	ReadBlobGranuleActor(BlobGranuleChunkRef const& chunk,KeyRangeRef const& keyRange,Version const& beginVersion,Version const& readVersion,Reference<BackupContainerFileSystem> const& bstore,Optional<BlobWorkerStats*> const& stats) 
															#line 786 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
[[nodiscard]] Future<RangeResult> readBlobGranule( BlobGranuleChunkRef const& chunk, KeyRangeRef const& keyRange, Version const& beginVersion, Version const& readVersion, Reference<BackupContainerFileSystem> const& bstore, Optional<BlobWorkerStats*> const& stats ) {
															#line 65 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	return Future<RangeResult>(new ReadBlobGranuleActor(chunk, keyRange, beginVersion, readVersion, bstore, stats));
															#line 815 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
}

#line 119 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"

// TODO probably should add things like limit/bytelimit at some point?
															#line 821 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
namespace {
// This generated class is to be used only via readBlobGranules()
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
template <class ReadBlobGranulesActor>
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
class ReadBlobGranulesActorState {
															#line 828 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
public:
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	ReadBlobGranulesActorState(BlobGranuleFileRequest const& request,BlobGranuleFileReply const& reply,Reference<BackupContainerFileSystem> const& bstore,PromiseStream<RangeResult> const& results) 
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		 : request(request),
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		   reply(reply),
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		   bstore(bstore),
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		   results(results)
															#line 841 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 128 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
				i = int();
															#line 129 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
				i = 0;
															#line 859 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 139 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		if (!static_cast<ReadBlobGranulesActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadBlobGranulesActorState(); static_cast<ReadBlobGranulesActor*>(this)->destroy(); return 0; }
															#line 888 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
		new (&static_cast<ReadBlobGranulesActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ReadBlobGranulesActorState();
		static_cast<ReadBlobGranulesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 136 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
			results.sendError(e);
															#line 901 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 134 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		results.sendError(end_of_stream());
															#line 916 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 129 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		if (!(i < reply.chunks.size()))
															#line 932 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 130 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		StrictFuture<RangeResult> __when_expr_0 = readBlobGranule(reply.chunks[i], request.keyRange, request.beginVersion, request.readVersion, bstore);
															#line 130 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		if (static_cast<ReadBlobGranulesActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 940 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ReadBlobGranulesActor*>(this)->actor_wait_state = 1;
															#line 130 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadBlobGranulesActor, 0, RangeResult >*>(static_cast<ReadBlobGranulesActor*>(this)));
															#line 945 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 132 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		results.send(std::move(chunkResult));
															#line 129 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		i++;
															#line 969 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(RangeResult && chunkResult,int loopDepth) 
	{
															#line 132 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		results.send(std::move(chunkResult));
															#line 129 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
		i++;
															#line 980 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	BlobGranuleFileRequest request;
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	BlobGranuleFileReply reply;
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	Reference<BackupContainerFileSystem> bstore;
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	PromiseStream<RangeResult> results;
															#line 128 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	int i;
															#line 1071 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
};
// This generated class is to be used only via readBlobGranules()
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
class ReadBlobGranulesActor final : public Actor<Void>, public ActorCallback< ReadBlobGranulesActor, 0, RangeResult >, public FastAllocated<ReadBlobGranulesActor>, public ReadBlobGranulesActorState<ReadBlobGranulesActor> {
															#line 1076 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
public:
	using FastAllocated<ReadBlobGranulesActor>::operator new;
	using FastAllocated<ReadBlobGranulesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadBlobGranulesActor, 0, RangeResult >;
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	ReadBlobGranulesActor(BlobGranuleFileRequest const& request,BlobGranuleFileReply const& reply,Reference<BackupContainerFileSystem> const& bstore,PromiseStream<RangeResult> const& results) 
															#line 1087 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
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
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
[[nodiscard]] Future<Void> readBlobGranules( BlobGranuleFileRequest const& request, BlobGranuleFileReply const& reply, Reference<BackupContainerFileSystem> const& bstore, PromiseStream<RangeResult> const& results ) {
															#line 121 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
	return Future<Void>(new ReadBlobGranulesActor(request, reply, bstore, results));
															#line 1115 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.g.cpp"
}

#line 141 "/usr/src/libfdb_c/fdbclient/BlobGranuleReader.actor.cpp"
