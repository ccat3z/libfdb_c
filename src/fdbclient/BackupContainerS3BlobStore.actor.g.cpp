#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
/*
 * BackupContainerS3BlobStore.actor.cpp
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

#include "fdbclient/AsyncFileS3BlobStore.actor.h"
#include "fdbclient/BackupContainerS3BlobStore.h"
#include "fdbrpc/AsyncFileEncrypted.h"
#include "fdbrpc/AsyncFileReadAhead.actor.h"
#include "flow/actorcompiler.h" // This must be the last #include.

class BackupContainerS3BlobStoreImpl {
public:
	// Backup files to under a single folder prefix with subfolders for each named backup
	static const std::string DATAFOLDER;

	// Indexfolder contains keys for which user-named backups exist.  Backup names can contain an arbitrary
	// number of slashes so the backup names are kept in a separate folder tree from their actual data.
	static const std::string INDEXFOLDER;

																#line 38 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
// This generated class is to be used only via listURLs()
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
template <class ListURLsActor>
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
class ListURLsActorState {
															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
public:
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	ListURLsActorState(Reference<S3BlobStoreEndpoint> const& bstore,std::string const& bucket) 
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		 : bstore(bstore),
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		   bucket(bucket),
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		   basePath(INDEXFOLDER + '/')
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
	{
		fdb_probe_actor_create("listURLs", reinterpret_cast<unsigned long>(this));

	}
	~ListURLsActorState() 
	{
		fdb_probe_actor_destroy("listURLs", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 38 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			StrictFuture<S3BlobStoreEndpoint::ListResult> __when_expr_0 = bstore->listObjects(bucket, basePath);
															#line 38 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			if (static_cast<ListURLsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ListURLsActor*>(this)->actor_wait_state = 1;
															#line 38 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ListURLsActor, 0, S3BlobStoreEndpoint::ListResult >*>(static_cast<ListURLsActor*>(this)));
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
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
		this->~ListURLsActorState();
		static_cast<ListURLsActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(S3BlobStoreEndpoint::ListResult const& contents,int loopDepth) 
	{
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		std::vector<std::string> results;
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		for( const auto& f : contents.objects ) {
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			results.push_back( bstore->getResourceURL(f.name.substr(basePath.size()), format("bucket=%s", bucket.c_str())));
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		}
															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (!static_cast<ListURLsActor*>(this)->SAV<std::vector<std::string>>::futures) { (void)(results); this->~ListURLsActorState(); static_cast<ListURLsActor*>(this)->destroy(); return 0; }
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		new (&static_cast<ListURLsActor*>(this)->SAV< std::vector<std::string> >::value()) std::vector<std::string>(results);
		this->~ListURLsActorState();
		static_cast<ListURLsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(S3BlobStoreEndpoint::ListResult && contents,int loopDepth) 
	{
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		std::vector<std::string> results;
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		for( const auto& f : contents.objects ) {
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			results.push_back( bstore->getResourceURL(f.name.substr(basePath.size()), format("bucket=%s", bucket.c_str())));
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		}
															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (!static_cast<ListURLsActor*>(this)->SAV<std::vector<std::string>>::futures) { (void)(results); this->~ListURLsActorState(); static_cast<ListURLsActor*>(this)->destroy(); return 0; }
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		new (&static_cast<ListURLsActor*>(this)->SAV< std::vector<std::string> >::value()) std::vector<std::string>(results);
		this->~ListURLsActorState();
		static_cast<ListURLsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(S3BlobStoreEndpoint::ListResult const& contents,int loopDepth) 
	{
		loopDepth = a_body1cont1(contents, loopDepth);

		return loopDepth;
	}
	int a_body1when1(S3BlobStoreEndpoint::ListResult && contents,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(contents), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ListURLsActor*>(this)->actor_wait_state > 0) static_cast<ListURLsActor*>(this)->actor_wait_state = 0;
		static_cast<ListURLsActor*>(this)->ActorCallback< ListURLsActor, 0, S3BlobStoreEndpoint::ListResult >::remove();

	}
	void a_callback_fire(ActorCallback< ListURLsActor, 0, S3BlobStoreEndpoint::ListResult >*,S3BlobStoreEndpoint::ListResult const& value) 
	{
		fdb_probe_actor_enter("listURLs", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listURLs", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ListURLsActor, 0, S3BlobStoreEndpoint::ListResult >*,S3BlobStoreEndpoint::ListResult && value) 
	{
		fdb_probe_actor_enter("listURLs", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listURLs", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ListURLsActor, 0, S3BlobStoreEndpoint::ListResult >*,Error err) 
	{
		fdb_probe_actor_enter("listURLs", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listURLs", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	Reference<S3BlobStoreEndpoint> bstore;
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	std::string bucket;
															#line 37 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	std::string basePath;
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
};
// This generated class is to be used only via listURLs()
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
class ListURLsActor final : public Actor<std::vector<std::string>>, public ActorCallback< ListURLsActor, 0, S3BlobStoreEndpoint::ListResult >, public FastAllocated<ListURLsActor>, public ListURLsActorState<ListURLsActor> {
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
public:
	using FastAllocated<ListURLsActor>::operator new;
	using FastAllocated<ListURLsActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ListURLsActor, 0, S3BlobStoreEndpoint::ListResult >;
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	ListURLsActor(Reference<S3BlobStoreEndpoint> const& bstore,std::string const& bucket) 
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		 : Actor<std::vector<std::string>>(),
		   ListURLsActorState<ListURLsActor>(bstore, bucket)
	{
		fdb_probe_actor_enter("listURLs", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("listURLs");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("listURLs", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ListURLsActor, 0, S3BlobStoreEndpoint::ListResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
[[nodiscard]] static Future<std::vector<std::string>> listURLs( Reference<S3BlobStoreEndpoint> const& bstore, std::string const& bucket ) {
															#line 36 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	return Future<std::vector<std::string>>(new ListURLsActor(bstore, bucket));
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
}

#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"

	class BackupFile : public IBackupFile, ReferenceCounted<BackupFile> {
	public:
		BackupFile(std::string fileName, Reference<IAsyncFile> file)
		  : IBackupFile(fileName), m_file(file), m_offset(0) {}

		Future<Void> append(const void* data, int len) override {
			Future<Void> r = m_file->write(data, len, m_offset);
			m_offset += len;
			return r;
		}

		Future<Void> finish() override {
			Reference<BackupFile> self = Reference<BackupFile>::addRef(this);
			return map(m_file->sync(), [=](Void _) {
				self->m_file.clear();
				return Void();
			});
		}

		int64_t size() const override { return m_offset; }

		void addref() final { return ReferenceCounted<BackupFile>::addref(); }
		void delref() final { return ReferenceCounted<BackupFile>::delref(); }

	private:
		Reference<IAsyncFile> m_file;
		int64_t m_offset;
	};

																#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
// This generated class is to be used only via listFiles()
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
template <class ListFilesActor>
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
class ListFilesActorState {
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
public:
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	ListFilesActorState(Reference<BackupContainerS3BlobStore> const& bc,std::string const& path,std::function<bool(std::string const&)> const& pathFilter) 
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		 : bc(bc),
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		   path(path),
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		   pathFilter(pathFilter),
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		   prefixTrim(bc->dataPath("").size())
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
	{
		fdb_probe_actor_create("listFiles", reinterpret_cast<unsigned long>(this));

	}
	~ListFilesActorState() 
	{
		fdb_probe_actor_destroy("listFiles", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			std::function<bool(std::string const&)> rawPathFilter = [=](const std::string& folderPath) { ASSERT(folderPath.size() >= prefixTrim); return pathFilter(folderPath.substr(prefixTrim)); };
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			StrictFuture<S3BlobStoreEndpoint::ListResult> __when_expr_0 = bc->m_bstore->listObjects( bc->m_bucket, bc->dataPath(path), '/', std::numeric_limits<int>::max(), rawPathFilter);
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			if (static_cast<ListFilesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ListFilesActor*>(this)->actor_wait_state = 1;
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ListFilesActor, 0, S3BlobStoreEndpoint::ListResult >*>(static_cast<ListFilesActor*>(this)));
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
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
		this->~ListFilesActorState();
		static_cast<ListFilesActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		BackupContainerFileSystem::FilesAndSizesT files;
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		for( const auto& o : result.objects ) {
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			ASSERT(o.name.size() >= prefixTrim);
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			files.push_back({ o.name.substr(prefixTrim), o.size });
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		}
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (!static_cast<ListFilesActor*>(this)->SAV<BackupContainerFileSystem::FilesAndSizesT>::futures) { (void)(files); this->~ListFilesActorState(); static_cast<ListFilesActor*>(this)->destroy(); return 0; }
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		new (&static_cast<ListFilesActor*>(this)->SAV< BackupContainerFileSystem::FilesAndSizesT >::value()) BackupContainerFileSystem::FilesAndSizesT(files);
		this->~ListFilesActorState();
		static_cast<ListFilesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(S3BlobStoreEndpoint::ListResult const& __result,int loopDepth) 
	{
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		result = __result;
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(S3BlobStoreEndpoint::ListResult && __result,int loopDepth) 
	{
		result = std::move(__result);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ListFilesActor*>(this)->actor_wait_state > 0) static_cast<ListFilesActor*>(this)->actor_wait_state = 0;
		static_cast<ListFilesActor*>(this)->ActorCallback< ListFilesActor, 0, S3BlobStoreEndpoint::ListResult >::remove();

	}
	void a_callback_fire(ActorCallback< ListFilesActor, 0, S3BlobStoreEndpoint::ListResult >*,S3BlobStoreEndpoint::ListResult const& value) 
	{
		fdb_probe_actor_enter("listFiles", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listFiles", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ListFilesActor, 0, S3BlobStoreEndpoint::ListResult >*,S3BlobStoreEndpoint::ListResult && value) 
	{
		fdb_probe_actor_enter("listFiles", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listFiles", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ListFilesActor, 0, S3BlobStoreEndpoint::ListResult >*,Error err) 
	{
		fdb_probe_actor_enter("listFiles", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listFiles", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	Reference<BackupContainerS3BlobStore> bc;
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	std::string path;
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	std::function<bool(std::string const&)> pathFilter;
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	int prefixTrim;
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	S3BlobStoreEndpoint::ListResult result;
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
};
// This generated class is to be used only via listFiles()
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
class ListFilesActor final : public Actor<BackupContainerFileSystem::FilesAndSizesT>, public ActorCallback< ListFilesActor, 0, S3BlobStoreEndpoint::ListResult >, public FastAllocated<ListFilesActor>, public ListFilesActorState<ListFilesActor> {
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
public:
	using FastAllocated<ListFilesActor>::operator new;
	using FastAllocated<ListFilesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<BackupContainerFileSystem::FilesAndSizesT>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ListFilesActor, 0, S3BlobStoreEndpoint::ListResult >;
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	ListFilesActor(Reference<BackupContainerS3BlobStore> const& bc,std::string const& path,std::function<bool(std::string const&)> const& pathFilter) 
															#line 459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		 : Actor<BackupContainerFileSystem::FilesAndSizesT>(),
		   ListFilesActorState<ListFilesActor>(bc, path, pathFilter)
	{
		fdb_probe_actor_enter("listFiles", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("listFiles");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("listFiles", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ListFilesActor, 0, S3BlobStoreEndpoint::ListResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
[[nodiscard]] static Future<BackupContainerFileSystem::FilesAndSizesT> listFiles( Reference<BackupContainerS3BlobStore> const& bc, std::string const& path, std::function<bool(std::string const&)> const& pathFilter ) {
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	return Future<BackupContainerFileSystem::FilesAndSizesT>(new ListFilesActor(bc, path, pathFilter));
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
}

#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"

																#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
// This generated class is to be used only via create()
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
template <class CreateActor>
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
class CreateActorState {
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
public:
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	CreateActorState(Reference<BackupContainerS3BlobStore> const& bc) 
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		 : bc(bc)
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
	{
		fdb_probe_actor_create("create", reinterpret_cast<unsigned long>(this));

	}
	~CreateActorState() 
	{
		fdb_probe_actor_destroy("create", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			StrictFuture<Void> __when_expr_0 = bc->m_bstore->createBucket(bc->m_bucket);
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			if (static_cast<CreateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CreateActor*>(this)->actor_wait_state = 1;
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CreateActor, 0, Void >*>(static_cast<CreateActor*>(this)));
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
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
		this->~CreateActorState();
		static_cast<CreateActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		StrictFuture<bool> __when_expr_1 = bc->m_bstore->objectExists(bc->m_bucket, bc->indexEntry());
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (static_cast<CreateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CreateActor*>(this)->actor_wait_state = 2;
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CreateActor, 1, bool >*>(static_cast<CreateActor*>(this)));
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		StrictFuture<bool> __when_expr_1 = bc->m_bstore->objectExists(bc->m_bucket, bc->indexEntry());
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (static_cast<CreateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CreateActor*>(this)->actor_wait_state = 2;
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CreateActor, 1, bool >*>(static_cast<CreateActor*>(this)));
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
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
		if (static_cast<CreateActor*>(this)->actor_wait_state > 0) static_cast<CreateActor*>(this)->actor_wait_state = 0;
		static_cast<CreateActor*>(this)->ActorCallback< CreateActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CreateActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("create", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("create", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CreateActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("create", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("create", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CreateActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("create", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("create", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(bool const& exists,int loopDepth) 
	{
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (!exists)
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		{
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			StrictFuture<Void> __when_expr_2 = bc->m_bstore->writeEntireFile(bc->m_bucket, bc->indexEntry(), "");
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			if (static_cast<CreateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<CreateActor*>(this)->actor_wait_state = 3;
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CreateActor, 2, Void >*>(static_cast<CreateActor*>(this)));
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(bool && exists,int loopDepth) 
	{
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (!exists)
															#line 669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		{
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			StrictFuture<Void> __when_expr_2 = bc->m_bstore->writeEntireFile(bc->m_bucket, bc->indexEntry(), "");
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			if (static_cast<CreateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<CreateActor*>(this)->actor_wait_state = 3;
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CreateActor, 2, Void >*>(static_cast<CreateActor*>(this)));
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1when1(bool const& exists,int loopDepth) 
	{
		loopDepth = a_body1cont2(exists, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(bool && exists,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(exists), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CreateActor*>(this)->actor_wait_state > 0) static_cast<CreateActor*>(this)->actor_wait_state = 0;
		static_cast<CreateActor*>(this)->ActorCallback< CreateActor, 1, bool >::remove();

	}
	void a_callback_fire(ActorCallback< CreateActor, 1, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("create", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("create", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CreateActor, 1, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("create", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("create", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CreateActor, 1, bool >*,Error err) 
	{
		fdb_probe_actor_enter("create", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("create", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (bc->usesEncryption())
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		{
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			StrictFuture<Void> __when_expr_3 = bc->encryptionSetupComplete();
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			if (static_cast<CreateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
			static_cast<CreateActor*>(this)->actor_wait_state = 4;
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CreateActor, 3, Void >*>(static_cast<CreateActor*>(this)));
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont5(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<CreateActor*>(this)->actor_wait_state > 0) static_cast<CreateActor*>(this)->actor_wait_state = 0;
		static_cast<CreateActor*>(this)->ActorCallback< CreateActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CreateActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("create", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("create", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CreateActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("create", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("create", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CreateActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("create", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("create", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont5(int loopDepth) 
	{
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (!static_cast<CreateActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CreateActorState(); static_cast<CreateActor*>(this)->destroy(); return 0; }
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		new (&static_cast<CreateActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CreateActorState();
		static_cast<CreateActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

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
	void a_exitChoose4() 
	{
		if (static_cast<CreateActor*>(this)->actor_wait_state > 0) static_cast<CreateActor*>(this)->actor_wait_state = 0;
		static_cast<CreateActor*>(this)->ActorCallback< CreateActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CreateActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("create", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("create", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< CreateActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("create", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("create", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< CreateActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("create", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("create", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	Reference<BackupContainerS3BlobStore> bc;
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
};
// This generated class is to be used only via create()
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
class CreateActor final : public Actor<Void>, public ActorCallback< CreateActor, 0, Void >, public ActorCallback< CreateActor, 1, bool >, public ActorCallback< CreateActor, 2, Void >, public ActorCallback< CreateActor, 3, Void >, public FastAllocated<CreateActor>, public CreateActorState<CreateActor> {
															#line 947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
public:
	using FastAllocated<CreateActor>::operator new;
	using FastAllocated<CreateActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CreateActor, 0, Void >;
friend struct ActorCallback< CreateActor, 1, bool >;
friend struct ActorCallback< CreateActor, 2, Void >;
friend struct ActorCallback< CreateActor, 3, Void >;
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	CreateActor(Reference<BackupContainerS3BlobStore> const& bc) 
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		 : Actor<Void>(),
		   CreateActorState<CreateActor>(bc)
	{
		fdb_probe_actor_enter("create", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("create");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("create", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CreateActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CreateActor, 1, bool >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CreateActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< CreateActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
[[nodiscard]] static Future<Void> create( Reference<BackupContainerS3BlobStore> const& bc ) {
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	return Future<Void>(new CreateActor(bc));
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
}

#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"

																#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
// This generated class is to be used only via deleteContainer()
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
template <class DeleteContainerActor>
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
class DeleteContainerActorState {
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
public:
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	DeleteContainerActorState(Reference<BackupContainerS3BlobStore> const& bc,int* const& pNumDeleted) 
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		 : bc(bc),
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		   pNumDeleted(pNumDeleted)
															#line 1011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
	{
		fdb_probe_actor_create("deleteContainer", reinterpret_cast<unsigned long>(this));

	}
	~DeleteContainerActorState() 
	{
		fdb_probe_actor_destroy("deleteContainer", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			StrictFuture<bool> __when_expr_0 = bc->exists();
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			if (static_cast<DeleteContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DeleteContainerActor*>(this)->actor_wait_state = 1;
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DeleteContainerActor, 0, bool >*>(static_cast<DeleteContainerActor*>(this)));
															#line 1033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
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
		this->~DeleteContainerActorState();
		static_cast<DeleteContainerActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(bool const& e,int loopDepth) 
	{
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (!e)
															#line 1056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		{
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			TraceEvent(SevWarnAlways, "BackupContainerDoesNotExist").detail("URL", bc->getURL());
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			return a_body1Catch1(backup_does_not_exist(), loopDepth);
															#line 1062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		}
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		StrictFuture<Void> __when_expr_1 = bc->m_bstore->deleteRecursively(bc->m_bucket, bc->dataPath(""), pNumDeleted);
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (static_cast<DeleteContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DeleteContainerActor*>(this)->actor_wait_state = 2;
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DeleteContainerActor, 1, Void >*>(static_cast<DeleteContainerActor*>(this)));
															#line 1073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(bool && e,int loopDepth) 
	{
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (!e)
															#line 1082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		{
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			TraceEvent(SevWarnAlways, "BackupContainerDoesNotExist").detail("URL", bc->getURL());
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
			return a_body1Catch1(backup_does_not_exist(), loopDepth);
															#line 1088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		}
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		StrictFuture<Void> __when_expr_1 = bc->m_bstore->deleteRecursively(bc->m_bucket, bc->dataPath(""), pNumDeleted);
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (static_cast<DeleteContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DeleteContainerActor*>(this)->actor_wait_state = 2;
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DeleteContainerActor, 1, Void >*>(static_cast<DeleteContainerActor*>(this)));
															#line 1099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<DeleteContainerActor*>(this)->actor_wait_state > 0) static_cast<DeleteContainerActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteContainerActor*>(this)->ActorCallback< DeleteContainerActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteContainerActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("deleteContainer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteContainer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DeleteContainerActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("deleteContainer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteContainer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DeleteContainerActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("deleteContainer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteContainer", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		StrictFuture<Void> __when_expr_2 = bc->m_bstore->deleteObject(bc->m_bucket, bc->indexEntry());
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (static_cast<DeleteContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<DeleteContainerActor*>(this)->actor_wait_state = 3;
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DeleteContainerActor, 2, Void >*>(static_cast<DeleteContainerActor*>(this)));
															#line 1178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		StrictFuture<Void> __when_expr_2 = bc->m_bstore->deleteObject(bc->m_bucket, bc->indexEntry());
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (static_cast<DeleteContainerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<DeleteContainerActor*>(this)->actor_wait_state = 3;
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DeleteContainerActor, 2, Void >*>(static_cast<DeleteContainerActor*>(this)));
															#line 1194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
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
		if (static_cast<DeleteContainerActor*>(this)->actor_wait_state > 0) static_cast<DeleteContainerActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteContainerActor*>(this)->ActorCallback< DeleteContainerActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteContainerActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteContainer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteContainer", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DeleteContainerActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteContainer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteContainer", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DeleteContainerActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteContainer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteContainer", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (!static_cast<DeleteContainerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteContainerActorState(); static_cast<DeleteContainerActor*>(this)->destroy(); return 0; }
															#line 1266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		new (&static_cast<DeleteContainerActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteContainerActorState();
		static_cast<DeleteContainerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
		if (!static_cast<DeleteContainerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteContainerActorState(); static_cast<DeleteContainerActor*>(this)->destroy(); return 0; }
															#line 1278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		new (&static_cast<DeleteContainerActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteContainerActorState();
		static_cast<DeleteContainerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<DeleteContainerActor*>(this)->actor_wait_state > 0) static_cast<DeleteContainerActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteContainerActor*>(this)->ActorCallback< DeleteContainerActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteContainerActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteContainer", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteContainer", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< DeleteContainerActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteContainer", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteContainer", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< DeleteContainerActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteContainer", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteContainer", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	Reference<BackupContainerS3BlobStore> bc;
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	int* pNumDeleted;
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
};
// This generated class is to be used only via deleteContainer()
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
class DeleteContainerActor final : public Actor<Void>, public ActorCallback< DeleteContainerActor, 0, bool >, public ActorCallback< DeleteContainerActor, 1, Void >, public ActorCallback< DeleteContainerActor, 2, Void >, public FastAllocated<DeleteContainerActor>, public DeleteContainerActorState<DeleteContainerActor> {
															#line 1358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
public:
	using FastAllocated<DeleteContainerActor>::operator new;
	using FastAllocated<DeleteContainerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DeleteContainerActor, 0, bool >;
friend struct ActorCallback< DeleteContainerActor, 1, Void >;
friend struct ActorCallback< DeleteContainerActor, 2, Void >;
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	DeleteContainerActor(Reference<BackupContainerS3BlobStore> const& bc,int* const& pNumDeleted) 
															#line 1371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
		 : Actor<Void>(),
		   DeleteContainerActorState<DeleteContainerActor>(bc, pNumDeleted)
	{
		fdb_probe_actor_enter("deleteContainer", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("deleteContainer");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("deleteContainer", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DeleteContainerActor, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DeleteContainerActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< DeleteContainerActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
[[nodiscard]] static Future<Void> deleteContainer( Reference<BackupContainerS3BlobStore> const& bc, int* const& pNumDeleted ) {
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
	return Future<Void>(new DeleteContainerActor(bc, pNumDeleted));
															#line 1400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.g.cpp"
}

#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerS3BlobStore.actor.cpp"
};

const std::string BackupContainerS3BlobStoreImpl::DATAFOLDER = "data";
const std::string BackupContainerS3BlobStoreImpl::INDEXFOLDER = "backups";

std::string BackupContainerS3BlobStore::dataPath(const std::string& path) {
	// if backup, include the backup data prefix.
	// if m_name ends in a trailing slash, don't add another
	std::string dataPath = "";
	if (isBackup) {
		dataPath = BackupContainerS3BlobStoreImpl::DATAFOLDER + "/";
	}
	if (!m_name.empty() && m_name.back() == '/') {
		dataPath += m_name + path;
	} else {
		dataPath += m_name + "/" + path;
	}
	return dataPath;
}

// Get the path of the backups's index entry
std::string BackupContainerS3BlobStore::indexEntry() {
	ASSERT(isBackup);
	return BackupContainerS3BlobStoreImpl::INDEXFOLDER + "/" + m_name;
}

BackupContainerS3BlobStore::BackupContainerS3BlobStore(Reference<S3BlobStoreEndpoint> bstore,
                                                       const std::string& name,
                                                       const S3BlobStoreEndpoint::ParametersT& params,
                                                       const Optional<std::string>& encryptionKeyFileName,
                                                       bool isBackup)
  : m_bstore(bstore), m_name(name), m_bucket("FDB_BACKUPS_V2"), isBackup(isBackup) {
	setEncryptionKey(encryptionKeyFileName);
	// Currently only one parameter is supported, "bucket"
	for (const auto& [name, value] : params) {
		if (name == "bucket") {
			m_bucket = value;
			continue;
		}
		TraceEvent(SevWarn, "BackupContainerS3BlobStoreInvalidParameter").detail("Name", name).detail("Value", value);
		IBackupContainer::lastOpenError = format("Unknown URL parameter: '%s'", name.c_str());
		throw backup_invalid_url();
	}
}

void BackupContainerS3BlobStore::addref() {
	return ReferenceCounted<BackupContainerS3BlobStore>::addref();
}
void BackupContainerS3BlobStore::delref() {
	return ReferenceCounted<BackupContainerS3BlobStore>::delref();
}

std::string BackupContainerS3BlobStore::getURLFormat() {
	return S3BlobStoreEndpoint::getURLFormat(true) + " (Note: The 'bucket' parameter is required.)";
}

Future<Reference<IAsyncFile>> BackupContainerS3BlobStore::readFile(const std::string& path) {
	Reference<IAsyncFile> f = makeReference<AsyncFileS3BlobStoreRead>(m_bstore, m_bucket, dataPath(path));

	if (usesEncryption()) {
		f = makeReference<AsyncFileEncrypted>(f, AsyncFileEncrypted::Mode::READ_ONLY);
	}
	if (m_bstore->knobs.enable_read_cache) {
		f = makeReference<AsyncFileReadAheadCache>(f,
		                                           m_bstore->knobs.read_block_size,
		                                           m_bstore->knobs.read_ahead_blocks,
		                                           m_bstore->knobs.concurrent_reads_per_file,
		                                           m_bstore->knobs.read_cache_blocks_per_file);
	}
	return f;
}

Future<std::vector<std::string>> BackupContainerS3BlobStore::listURLs(Reference<S3BlobStoreEndpoint> bstore,
                                                                      const std::string& bucket) {
	return BackupContainerS3BlobStoreImpl::listURLs(bstore, bucket);
}

Future<Reference<IBackupFile>> BackupContainerS3BlobStore::writeFile(const std::string& path) {
	Reference<IAsyncFile> f = makeReference<AsyncFileS3BlobStoreWrite>(m_bstore, m_bucket, dataPath(path));
	if (usesEncryption()) {
		f = makeReference<AsyncFileEncrypted>(f, AsyncFileEncrypted::Mode::APPEND_ONLY);
	}
	return Future<Reference<IBackupFile>>(makeReference<BackupContainerS3BlobStoreImpl::BackupFile>(path, f));
}

Future<Void> BackupContainerS3BlobStore::writeEntireFile(const std::string& path, const std::string& fileContents) {
	return m_bstore->writeEntireFile(m_bucket, dataPath(path), fileContents);
}

Future<Void> BackupContainerS3BlobStore::deleteFile(const std::string& path) {
	return m_bstore->deleteObject(m_bucket, dataPath(path));
}

Future<BackupContainerFileSystem::FilesAndSizesT> BackupContainerS3BlobStore::listFiles(
    const std::string& path,
    std::function<bool(std::string const&)> pathFilter) {
	return BackupContainerS3BlobStoreImpl::listFiles(
	    Reference<BackupContainerS3BlobStore>::addRef(this), path, pathFilter);
}

Future<Void> BackupContainerS3BlobStore::create() {
	return BackupContainerS3BlobStoreImpl::create(Reference<BackupContainerS3BlobStore>::addRef(this));
}

Future<bool> BackupContainerS3BlobStore::exists() {
	return m_bstore->objectExists(m_bucket, indexEntry());
}

Future<Void> BackupContainerS3BlobStore::deleteContainer(int* pNumDeleted) {
	return BackupContainerS3BlobStoreImpl::deleteContainer(Reference<BackupContainerS3BlobStore>::addRef(this),
	                                                       pNumDeleted);
}

std::string BackupContainerS3BlobStore::getBucket() const {
	return m_bucket;
}
