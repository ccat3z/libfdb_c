#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
/*
 * BackupContainer.actor.cpp
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

#include <cstdlib>
#include <ostream>

// FIXME: Trim this down
#include "fmt/format.h"
#include "flow/Platform.actor.h"
#include "fdbclient/AsyncTaskThread.h"
#include "fdbclient/BackupContainer.h"
#include "fdbclient/BackupAgent.actor.h"
#include "fdbclient/FDBTypes.h"
#include "fdbclient/JsonBuilder.h"
#include "flow/Arena.h"
#include "flow/Trace.h"
#include "flow/UnitTest.h"
#include "flow/Hash3.h"
#include "fdbrpc/AsyncFileReadAhead.actor.h"
#include "fdbrpc/simulator.h"
#include "flow/Platform.h"
#include "fdbclient/AsyncFileS3BlobStore.actor.h"
#ifdef BUILD_AZURE_BACKUP
#include "fdbclient/BackupContainerAzureBlobStore.h"
#endif
#include "fdbclient/BackupContainerFileSystem.h"
#include "fdbclient/BackupContainerLocalDirectory.h"
#include "fdbclient/BackupContainerS3BlobStore.h"
#include "fdbclient/Status.h"
#include "fdbclient/SystemData.h"
#include "fdbclient/ReadYourWrites.h"
#include "fdbclient/KeyBackedTypes.actor.h"
#include "fdbclient/RunRYWTransaction.actor.h"
#include <algorithm>
#include <cinttypes>
#include <time.h>
#include "flow/actorcompiler.h" // has to be last include

namespace IBackupFile_impl {

															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
// This generated class is to be used only via appendStringRefWithLen()
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
template <class AppendStringRefWithLenActor>
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
class AppendStringRefWithLenActorState {
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
public:
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	AppendStringRefWithLenActorState(Reference<IBackupFile> const& file,Standalone<StringRef> const& s) 
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		 : file(file),
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		   s(s),
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		   lenBuf(bigEndian32((uint32_t)s.size()))
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
	{
		fdb_probe_actor_create("appendStringRefWithLen", reinterpret_cast<unsigned long>(this));

	}
	~AppendStringRefWithLenActorState() 
	{
		fdb_probe_actor_destroy("appendStringRefWithLen", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			StrictFuture<Void> __when_expr_0 = file->append(&lenBuf, sizeof(lenBuf));
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			if (static_cast<AppendStringRefWithLenActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AppendStringRefWithLenActor*>(this)->actor_wait_state = 1;
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AppendStringRefWithLenActor, 0, Void >*>(static_cast<AppendStringRefWithLenActor*>(this)));
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
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
		this->~AppendStringRefWithLenActorState();
		static_cast<AppendStringRefWithLenActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		StrictFuture<Void> __when_expr_1 = file->append(s.begin(), s.size());
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (static_cast<AppendStringRefWithLenActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<AppendStringRefWithLenActor*>(this)->actor_wait_state = 2;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AppendStringRefWithLenActor, 1, Void >*>(static_cast<AppendStringRefWithLenActor*>(this)));
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		StrictFuture<Void> __when_expr_1 = file->append(s.begin(), s.size());
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (static_cast<AppendStringRefWithLenActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<AppendStringRefWithLenActor*>(this)->actor_wait_state = 2;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AppendStringRefWithLenActor, 1, Void >*>(static_cast<AppendStringRefWithLenActor*>(this)));
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
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
		if (static_cast<AppendStringRefWithLenActor*>(this)->actor_wait_state > 0) static_cast<AppendStringRefWithLenActor*>(this)->actor_wait_state = 0;
		static_cast<AppendStringRefWithLenActor*>(this)->ActorCallback< AppendStringRefWithLenActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AppendStringRefWithLenActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("appendStringRefWithLen", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("appendStringRefWithLen", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AppendStringRefWithLenActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("appendStringRefWithLen", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("appendStringRefWithLen", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AppendStringRefWithLenActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("appendStringRefWithLen", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("appendStringRefWithLen", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (!static_cast<AppendStringRefWithLenActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AppendStringRefWithLenActorState(); static_cast<AppendStringRefWithLenActor*>(this)->destroy(); return 0; }
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		new (&static_cast<AppendStringRefWithLenActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~AppendStringRefWithLenActorState();
		static_cast<AppendStringRefWithLenActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (!static_cast<AppendStringRefWithLenActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AppendStringRefWithLenActorState(); static_cast<AppendStringRefWithLenActor*>(this)->destroy(); return 0; }
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		new (&static_cast<AppendStringRefWithLenActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~AppendStringRefWithLenActorState();
		static_cast<AppendStringRefWithLenActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<AppendStringRefWithLenActor*>(this)->actor_wait_state > 0) static_cast<AppendStringRefWithLenActor*>(this)->actor_wait_state = 0;
		static_cast<AppendStringRefWithLenActor*>(this)->ActorCallback< AppendStringRefWithLenActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AppendStringRefWithLenActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("appendStringRefWithLen", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("appendStringRefWithLen", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< AppendStringRefWithLenActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("appendStringRefWithLen", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("appendStringRefWithLen", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< AppendStringRefWithLenActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("appendStringRefWithLen", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("appendStringRefWithLen", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	Reference<IBackupFile> file;
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	Standalone<StringRef> s;
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	uint32_t lenBuf;
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
};
// This generated class is to be used only via appendStringRefWithLen()
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
class AppendStringRefWithLenActor final : public Actor<Void>, public ActorCallback< AppendStringRefWithLenActor, 0, Void >, public ActorCallback< AppendStringRefWithLenActor, 1, Void >, public FastAllocated<AppendStringRefWithLenActor>, public AppendStringRefWithLenActorState<AppendStringRefWithLenActor> {
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
public:
	using FastAllocated<AppendStringRefWithLenActor>::operator new;
	using FastAllocated<AppendStringRefWithLenActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AppendStringRefWithLenActor, 0, Void >;
friend struct ActorCallback< AppendStringRefWithLenActor, 1, Void >;
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	AppendStringRefWithLenActor(Reference<IBackupFile> const& file,Standalone<StringRef> const& s) 
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		 : Actor<Void>(),
		   AppendStringRefWithLenActorState<AppendStringRefWithLenActor>(file, s)
	{
		fdb_probe_actor_enter("appendStringRefWithLen", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("appendStringRefWithLen");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("appendStringRefWithLen", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AppendStringRefWithLenActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< AppendStringRefWithLenActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
[[nodiscard]] Future<Void> appendStringRefWithLen( Reference<IBackupFile> const& file, Standalone<StringRef> const& s ) {
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	return Future<Void>(new AppendStringRefWithLenActor(file, s));
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
}

#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"

} // namespace IBackupFile_impl

Future<Void> IBackupFile::appendStringRefWithLen(Standalone<StringRef> s) {
	return IBackupFile_impl::appendStringRefWithLen(Reference<IBackupFile>::addRef(this), s);
}

std::string IBackupContainer::ExpireProgress::toString() const {
	std::string s = step + "...";
	if (total > 0) {
		s += format("%d/%d (%.2f%%)", done, total, double(done) / total * 100);
	}
	return s;
}

void BackupFileList::toStream(FILE* fout) const {
	for (const RangeFile& f : ranges) {
		fmt::print(fout, "range {0} {1}\n", f.fileSize, f.fileName);
	}
	for (const LogFile& f : logs) {
		fmt::print(fout, "log {0} {1}\n", f.fileSize, f.fileName);
	}
	for (const KeyspaceSnapshotFile& f : snapshots) {
		fmt::print(fout, "snapshotManifest {0} {1}\n", f.totalSize, f.fileName);
	}
}

Future<Void> fetchTimes(Reference<ReadYourWritesTransaction> tr, std::map<Version, int64_t>* pVersionTimeMap) {
	std::vector<Future<Void>> futures;

	// Resolve each version in the map,
	for (auto& p : *pVersionTimeMap) {
		futures.push_back(map(timeKeeperEpochsFromVersion(p.first, tr), [=](Optional<int64_t> t) {
			if (t.present())
				pVersionTimeMap->at(p.first) = t.get();
			else
				pVersionTimeMap->erase(p.first);
			return Void();
		}));
	}

	return waitForAll(futures);
}

Future<Void> BackupDescription::resolveVersionTimes(Database cx) {
	// Populate map with versions needed
	versionTimeMap.clear();

	for (const KeyspaceSnapshotFile& m : snapshots) {
		versionTimeMap[m.beginVersion];
		versionTimeMap[m.endVersion];
	}
	if (minLogBegin.present())
		versionTimeMap[minLogBegin.get()];
	if (maxLogEnd.present())
		versionTimeMap[maxLogEnd.get()];
	if (contiguousLogEnd.present())
		versionTimeMap[contiguousLogEnd.get()];
	if (minRestorableVersion.present())
		versionTimeMap[minRestorableVersion.get()];
	if (maxRestorableVersion.present())
		versionTimeMap[maxRestorableVersion.get()];

	return runRYWTransaction(cx,
	                         [=](Reference<ReadYourWritesTransaction> tr) { return fetchTimes(tr, &versionTimeMap); });
};

std::string BackupDescription::toString() const {
	std::string info;

	info.append(format("URL: %s\n", url.c_str()));
	info.append(format("Restorable: %s\n", maxRestorableVersion.present() ? "true" : "false"));
	info.append(format("Partitioned logs: %s\n", partitioned ? "true" : "false"));

	auto formatVersion = [&](Version v) {
		std::string s;
		if (!versionTimeMap.empty()) {
			auto i = versionTimeMap.find(v);
			if (i != versionTimeMap.end())
				s = format("%lld (%s)", v, BackupAgentBase::formatTime(i->second).c_str());
			else
				s = format("%lld (unknown)", v);
		} else if (maxLogEnd.present()) {
			double days = double(maxLogEnd.get() - v) / (CLIENT_KNOBS->CORE_VERSIONSPERSECOND * 24 * 60 * 60);
			s = format("%lld (maxLogEnd %s%.2f days)", v, days < 0 ? "+" : "-", days);
		} else {
			s = format("%lld", v);
		}
		return s;
	};

	for (const KeyspaceSnapshotFile& m : snapshots) {
		info.append(
		    format("Snapshot:  startVersion=%s  endVersion=%s  totalBytes=%lld  restorable=%s  expiredPct=%.2f\n",
		           formatVersion(m.beginVersion).c_str(),
		           formatVersion(m.endVersion).c_str(),
		           m.totalSize,
		           m.restorable.orDefault(false) ? "true" : "false",
		           m.expiredPct(expiredEndVersion)));
	}

	info.append(format("SnapshotBytes: %lld\n", snapshotBytes));

	if (expiredEndVersion.present())
		info.append(format("ExpiredEndVersion:       %s\n", formatVersion(expiredEndVersion.get()).c_str()));
	if (unreliableEndVersion.present())
		info.append(format("UnreliableEndVersion:    %s\n", formatVersion(unreliableEndVersion.get()).c_str()));
	if (minLogBegin.present())
		info.append(format("MinLogBeginVersion:      %s\n", formatVersion(minLogBegin.get()).c_str()));
	if (contiguousLogEnd.present())
		info.append(format("ContiguousLogEndVersion: %s\n", formatVersion(contiguousLogEnd.get()).c_str()));
	if (maxLogEnd.present())
		info.append(format("MaxLogEndVersion:        %s\n", formatVersion(maxLogEnd.get()).c_str()));
	if (minRestorableVersion.present())
		info.append(format("MinRestorableVersion:    %s\n", formatVersion(minRestorableVersion.get()).c_str()));
	if (maxRestorableVersion.present())
		info.append(format("MaxRestorableVersion:    %s\n", formatVersion(maxRestorableVersion.get()).c_str()));

	if (!extendedDetail.empty())
		info.append("ExtendedDetail: ").append(extendedDetail);

	return info;
}

std::string BackupDescription::toJSON() const {
	JsonBuilderObject doc;

	doc.setKey("SchemaVersion", "1.0.0");
	doc.setKey("URL", url.c_str());
	doc.setKey("Restorable", maxRestorableVersion.present());
	doc.setKey("Partitioned", partitioned);

	auto formatVersion = [&](Version v) {
		JsonBuilderObject doc;
		doc.setKey("Version", v);
		if (!versionTimeMap.empty()) {
			auto i = versionTimeMap.find(v);
			if (i != versionTimeMap.end()) {
				doc.setKey("Timestamp", BackupAgentBase::formatTime(i->second));
				doc.setKey("EpochSeconds", i->second);
			}
		} else if (maxLogEnd.present()) {
			double days = double(v - maxLogEnd.get()) / (CLIENT_KNOBS->CORE_VERSIONSPERSECOND * 24 * 60 * 60);
			doc.setKey("RelativeDays", days);
		}
		return doc;
	};

	JsonBuilderArray snapshotsArray;
	for (const KeyspaceSnapshotFile& m : snapshots) {
		JsonBuilderObject snapshotDoc;
		snapshotDoc.setKey("Start", formatVersion(m.beginVersion));
		snapshotDoc.setKey("End", formatVersion(m.endVersion));
		snapshotDoc.setKey("Restorable", m.restorable.orDefault(false));
		snapshotDoc.setKey("TotalBytes", m.totalSize);
		snapshotDoc.setKey("PercentageExpired", m.expiredPct(expiredEndVersion));
		snapshotsArray.push_back(snapshotDoc);
	}
	doc.setKey("Snapshots", snapshotsArray);

	doc.setKey("TotalSnapshotBytes", snapshotBytes);

	if (expiredEndVersion.present())
		doc.setKey("ExpiredEnd", formatVersion(expiredEndVersion.get()));
	if (unreliableEndVersion.present())
		doc.setKey("UnreliableEnd", formatVersion(unreliableEndVersion.get()));
	if (minLogBegin.present())
		doc.setKey("MinLogBegin", formatVersion(minLogBegin.get()));
	if (contiguousLogEnd.present())
		doc.setKey("ContiguousLogEnd", formatVersion(contiguousLogEnd.get()));
	if (maxLogEnd.present())
		doc.setKey("MaxLogEnd", formatVersion(maxLogEnd.get()));
	if (minRestorableVersion.present())
		doc.setKey("MinRestorablePoint", formatVersion(minRestorableVersion.get()));
	if (maxRestorableVersion.present())
		doc.setKey("MaxRestorablePoint", formatVersion(maxRestorableVersion.get()));

	if (!extendedDetail.empty())
		doc.setKey("ExtendedDetail", extendedDetail);

	return doc.getJson();
}

std::string IBackupContainer::lastOpenError;

std::vector<std::string> IBackupContainer::getURLFormats() {
	return {
#ifdef BUILD_AZURE_BACKUP
		BackupContainerAzureBlobStore::getURLFormat(),
#endif
		BackupContainerLocalDirectory::getURLFormat(),
		BackupContainerS3BlobStore::getURLFormat(),
	};
}

// Get an IBackupContainer based on a container URL string
Reference<IBackupContainer> IBackupContainer::openContainer(const std::string& url,
                                                            const Optional<std::string>& proxy,
                                                            const Optional<std::string>& encryptionKeyFileName) {
	static std::map<std::string, Reference<IBackupContainer>> m_cache;

	Reference<IBackupContainer>& r = m_cache[url];
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
			r = makeReference<BackupContainerS3BlobStore>(bstore, resource, backupParams, encryptionKeyFileName, true);
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

// Get a list of URLS to backup containers based on some a shorter URL.  This function knows about some set of supported
// URL types which support this sort of backup discovery.
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
namespace {
// This generated class is to be used only via listContainers_impl()
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
template <class ListContainers_implActor>
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
class ListContainers_implActorState {
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
public:
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	ListContainers_implActorState(std::string const& baseURL,Optional<std::string> const& proxy) 
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		 : baseURL(baseURL),
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		   proxy(proxy)
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
	{
		fdb_probe_actor_create("listContainers_impl", reinterpret_cast<unsigned long>(this));

	}
	~ListContainers_implActorState() 
	{
		fdb_probe_actor_destroy("listContainers_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
				StringRef u(baseURL);
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
				if (u.startsWith("file://"_sr))
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
				{
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
					StrictFuture<std::vector<std::string>> __when_expr_0 = BackupContainerLocalDirectory::listURLs(baseURL);
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
					if (static_cast<ListContainers_implActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<ListContainers_implActor*>(this)->actor_wait_state = 1;
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ListContainers_implActor, 0, std::vector<std::string> >*>(static_cast<ListContainers_implActor*>(this)));
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
					loopDepth = 0;
				}
				else
				{
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
					if (u.startsWith("blobstore://"_sr))
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
					{
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
						std::string resource;
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
						S3BlobStoreEndpoint::ParametersT backupParams;
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
						Reference<S3BlobStoreEndpoint> bstore = S3BlobStoreEndpoint::fromString( baseURL, proxy, &resource, &IBackupContainer::lastOpenError, &backupParams);
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
						if (!resource.empty())
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
						{
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
							TraceEvent(SevWarn, "BackupContainer") .detail("Description", "Invalid backup container base URL, resource aka path should be blank.") .detail("URL", baseURL);
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
							return a_body1Catch2(backup_invalid_url(), loopDepth);
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
						}
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
						BackupContainerS3BlobStore dummy(bstore, "dummy", backupParams, {}, true);
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
						StrictFuture<std::vector<std::string>> __when_expr_1 = BackupContainerS3BlobStore::listURLs(bstore, dummy.getBucket());
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
						if (static_cast<ListContainers_implActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
						if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
						static_cast<ListContainers_implActor*>(this)->actor_wait_state = 2;
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
						__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ListContainers_implActor, 1, std::vector<std::string> >*>(static_cast<ListContainers_implActor*>(this)));
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
						loopDepth = 0;
					}
					else
					{
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
						IBackupContainer::lastOpenError = "invalid URL prefix";
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
						return a_body1Catch2(backup_invalid_url(), loopDepth);
															#line 748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
					}
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
		this->~ListContainers_implActorState();
		static_cast<ListContainers_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			{
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			}
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			TraceEvent m(SevWarn, "BackupContainer");
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			m.error(e);
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			m.detail("Description", "Invalid backup container URL prefix.  See help.");
															#line 412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			m.detail("URL", baseURL);
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			if (e.code() == error_code_backup_invalid_url)
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			{
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
				m.detail("LastOpenError", IBackupContainer::lastOpenError);
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			}
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(std::vector<std::string> const& results,int loopDepth) 
	{
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (!static_cast<ListContainers_implActor*>(this)->SAV<std::vector<std::string>>::futures) { (void)(results); this->~ListContainers_implActorState(); static_cast<ListContainers_implActor*>(this)->destroy(); return 0; }
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		new (&static_cast<ListContainers_implActor*>(this)->SAV< std::vector<std::string> >::value()) std::vector<std::string>(results);
		this->~ListContainers_implActorState();
		static_cast<ListContainers_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(std::vector<std::string> && results,int loopDepth) 
	{
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (!static_cast<ListContainers_implActor*>(this)->SAV<std::vector<std::string>>::futures) { (void)(results); this->~ListContainers_implActorState(); static_cast<ListContainers_implActor*>(this)->destroy(); return 0; }
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		new (&static_cast<ListContainers_implActor*>(this)->SAV< std::vector<std::string> >::value()) std::vector<std::string>(results);
		this->~ListContainers_implActorState();
		static_cast<ListContainers_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<std::string> const& results,int loopDepth) 
	{
		loopDepth = a_body1cont3(results, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<std::string> && results,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(results), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ListContainers_implActor*>(this)->actor_wait_state > 0) static_cast<ListContainers_implActor*>(this)->actor_wait_state = 0;
		static_cast<ListContainers_implActor*>(this)->ActorCallback< ListContainers_implActor, 0, std::vector<std::string> >::remove();

	}
	void a_callback_fire(ActorCallback< ListContainers_implActor, 0, std::vector<std::string> >*,std::vector<std::string> const& value) 
	{
		fdb_probe_actor_enter("listContainers_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listContainers_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ListContainers_implActor, 0, std::vector<std::string> >*,std::vector<std::string> && value) 
	{
		fdb_probe_actor_enter("listContainers_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listContainers_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ListContainers_implActor, 0, std::vector<std::string> >*,Error err) 
	{
		fdb_probe_actor_enter("listContainers_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listContainers_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont6(std::vector<std::string> const& results,int loopDepth) 
	{
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (!static_cast<ListContainers_implActor*>(this)->SAV<std::vector<std::string>>::futures) { (void)(results); this->~ListContainers_implActorState(); static_cast<ListContainers_implActor*>(this)->destroy(); return 0; }
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		new (&static_cast<ListContainers_implActor*>(this)->SAV< std::vector<std::string> >::value()) std::vector<std::string>(results);
		this->~ListContainers_implActorState();
		static_cast<ListContainers_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(std::vector<std::string> && results,int loopDepth) 
	{
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (!static_cast<ListContainers_implActor*>(this)->SAV<std::vector<std::string>>::futures) { (void)(results); this->~ListContainers_implActorState(); static_cast<ListContainers_implActor*>(this)->destroy(); return 0; }
															#line 916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		new (&static_cast<ListContainers_implActor*>(this)->SAV< std::vector<std::string> >::value()) std::vector<std::string>(results);
		this->~ListContainers_implActorState();
		static_cast<ListContainers_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(std::vector<std::string> const& results,int loopDepth) 
	{
		loopDepth = a_body1cont6(results, loopDepth);

		return loopDepth;
	}
	int a_body1when2(std::vector<std::string> && results,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(results), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ListContainers_implActor*>(this)->actor_wait_state > 0) static_cast<ListContainers_implActor*>(this)->actor_wait_state = 0;
		static_cast<ListContainers_implActor*>(this)->ActorCallback< ListContainers_implActor, 1, std::vector<std::string> >::remove();

	}
	void a_callback_fire(ActorCallback< ListContainers_implActor, 1, std::vector<std::string> >*,std::vector<std::string> const& value) 
	{
		fdb_probe_actor_enter("listContainers_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listContainers_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ListContainers_implActor, 1, std::vector<std::string> >*,std::vector<std::string> && value) 
	{
		fdb_probe_actor_enter("listContainers_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listContainers_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ListContainers_implActor, 1, std::vector<std::string> >*,Error err) 
	{
		fdb_probe_actor_enter("listContainers_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listContainers_impl", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	std::string baseURL;
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	Optional<std::string> proxy;
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
};
// This generated class is to be used only via listContainers_impl()
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
class ListContainers_implActor final : public Actor<std::vector<std::string>>, public ActorCallback< ListContainers_implActor, 0, std::vector<std::string> >, public ActorCallback< ListContainers_implActor, 1, std::vector<std::string> >, public FastAllocated<ListContainers_implActor>, public ListContainers_implActorState<ListContainers_implActor> {
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
public:
	using FastAllocated<ListContainers_implActor>::operator new;
	using FastAllocated<ListContainers_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ListContainers_implActor, 0, std::vector<std::string> >;
friend struct ActorCallback< ListContainers_implActor, 1, std::vector<std::string> >;
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	ListContainers_implActor(std::string const& baseURL,Optional<std::string> const& proxy) 
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		 : Actor<std::vector<std::string>>(),
		   ListContainers_implActorState<ListContainers_implActor>(baseURL, proxy)
	{
		fdb_probe_actor_enter("listContainers_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("listContainers_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("listContainers_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ListContainers_implActor, 0, std::vector<std::string> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ListContainers_implActor, 1, std::vector<std::string> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
[[nodiscard]] Future<std::vector<std::string>> listContainers_impl( std::string const& baseURL, Optional<std::string> const& proxy ) {
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	return Future<std::vector<std::string>>(new ListContainers_implActor(baseURL, proxy));
															#line 1037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
}

#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"

Future<std::vector<std::string>> IBackupContainer::listContainers(const std::string& baseURL,
                                                                  const Optional<std::string>& proxy) {
	return listContainers_impl(baseURL, proxy);
}

															#line 1047 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
namespace {
// This generated class is to be used only via timeKeeperVersionFromDatetime()
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
template <class TimeKeeperVersionFromDatetimeActor>
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
class TimeKeeperVersionFromDatetimeActorState {
															#line 1054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
public:
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	TimeKeeperVersionFromDatetimeActorState(std::string const& datetime,Database const& db) 
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		 : datetime(datetime),
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		   db(db),
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		   versionMap(timeKeeperPrefixRange.begin),
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		   tr(makeReference<ReadYourWritesTransaction>(db)),
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		   time(BackupAgentBase::parseTime(datetime))
															#line 1069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
	{
		fdb_probe_actor_create("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this));

	}
	~TimeKeeperVersionFromDatetimeActorState() 
	{
		fdb_probe_actor_destroy("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			if (time < 0)
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			{
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
				fprintf( stderr, "ERROR: Incorrect date/time or format.  Format is %s.\n", BackupAgentBase::timeFormat().c_str());
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
				return a_body1Catch1(backup_error(), loopDepth);
															#line 1090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			}
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			;
															#line 1094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
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
		this->~TimeKeeperVersionFromDatetimeActorState();
		static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			StrictFuture<KeyBackedRangeResult<std::pair<int64_t, Version>>> __when_expr_0 = versionMap.getRange(tr, 0, time, 1, Snapshot::False, Reverse::True);
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			if (static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->actor_wait_state = 1;
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TimeKeeperVersionFromDatetimeActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >*>(static_cast<TimeKeeperVersionFromDatetimeActor*>(this)));
															#line 1136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
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
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			StrictFuture<Void> __when_expr_2 = tr->onError(e);
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			if (static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->actor_wait_state = 3;
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TimeKeeperVersionFromDatetimeActor, 2, Void >*>(static_cast<TimeKeeperVersionFromDatetimeActor*>(this)));
															#line 1165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			loopDepth = 0;
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
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (rangeResult.results.size() != 1)
															#line 1180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		{
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			StrictFuture<Void> __when_expr_1 = store(rangeResult, versionMap.getRange(tr, time, std::numeric_limits<int64_t>::max(), 1));
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			if (static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->actor_wait_state = 2;
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TimeKeeperVersionFromDatetimeActor, 1, Void >*>(static_cast<TimeKeeperVersionFromDatetimeActor*>(this)));
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(KeyBackedRangeResult<std::pair<int64_t, Version>> const& __rangeResult,int loopDepth) 
	{
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		rangeResult = __rangeResult;
															#line 1205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(KeyBackedRangeResult<std::pair<int64_t, Version>> && __rangeResult,int loopDepth) 
	{
		rangeResult = std::move(__rangeResult);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->actor_wait_state > 0) static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->actor_wait_state = 0;
		static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->ActorCallback< TimeKeeperVersionFromDatetimeActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >::remove();

	}
	void a_callback_fire(ActorCallback< TimeKeeperVersionFromDatetimeActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >*,KeyBackedRangeResult<std::pair<int64_t, Version>> const& value) 
	{
		fdb_probe_actor_enter("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TimeKeeperVersionFromDatetimeActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >*,KeyBackedRangeResult<std::pair<int64_t, Version>> && value) 
	{
		fdb_probe_actor_enter("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TimeKeeperVersionFromDatetimeActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >*,Error err) 
	{
		fdb_probe_actor_enter("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		auto& result = rangeResult.results[0];
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (!static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->SAV<Version>::futures) { (void)(std::max<Version>(0, result.second + (time - result.first) * CLIENT_KNOBS->CORE_VERSIONSPERSECOND)); this->~TimeKeeperVersionFromDatetimeActorState(); static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->destroy(); return 0; }
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		new (&static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->SAV< Version >::value()) Version(std::max<Version>(0, result.second + (time - result.first) * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
		this->~TimeKeeperVersionFromDatetimeActorState();
		static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (rangeResult.results.size() != 1)
															#line 1286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		{
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			fprintf(stderr, "ERROR: Unable to calculate a version for given date/time.\n");
															#line 449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			return a_body1loopBody1Catch1(backup_error(), loopDepth);
															#line 1292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (rangeResult.results.size() != 1)
															#line 1302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		{
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			fprintf(stderr, "ERROR: Unable to calculate a version for given date/time.\n");
															#line 449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			return a_body1loopBody1Catch1(backup_error(), loopDepth);
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->actor_wait_state > 0) static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->actor_wait_state = 0;
		static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->ActorCallback< TimeKeeperVersionFromDatetimeActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TimeKeeperVersionFromDatetimeActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TimeKeeperVersionFromDatetimeActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TimeKeeperVersionFromDatetimeActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->actor_wait_state > 0) static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->actor_wait_state = 0;
		static_cast<TimeKeeperVersionFromDatetimeActor*>(this)->ActorCallback< TimeKeeperVersionFromDatetimeActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TimeKeeperVersionFromDatetimeActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< TimeKeeperVersionFromDatetimeActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< TimeKeeperVersionFromDatetimeActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	std::string datetime;
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	Database db;
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	KeyBackedMap<int64_t, Version> versionMap;
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	int64_t time;
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	KeyBackedRangeResult<std::pair<int64_t, Version>> rangeResult;
															#line 1464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
};
// This generated class is to be used only via timeKeeperVersionFromDatetime()
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
class TimeKeeperVersionFromDatetimeActor final : public Actor<Version>, public ActorCallback< TimeKeeperVersionFromDatetimeActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >, public ActorCallback< TimeKeeperVersionFromDatetimeActor, 1, Void >, public ActorCallback< TimeKeeperVersionFromDatetimeActor, 2, Void >, public FastAllocated<TimeKeeperVersionFromDatetimeActor>, public TimeKeeperVersionFromDatetimeActorState<TimeKeeperVersionFromDatetimeActor> {
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
public:
	using FastAllocated<TimeKeeperVersionFromDatetimeActor>::operator new;
	using FastAllocated<TimeKeeperVersionFromDatetimeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Version>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TimeKeeperVersionFromDatetimeActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >;
friend struct ActorCallback< TimeKeeperVersionFromDatetimeActor, 1, Void >;
friend struct ActorCallback< TimeKeeperVersionFromDatetimeActor, 2, Void >;
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	TimeKeeperVersionFromDatetimeActor(std::string const& datetime,Database const& db) 
															#line 1482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		 : Actor<Version>(),
		   TimeKeeperVersionFromDatetimeActorState<TimeKeeperVersionFromDatetimeActor>(datetime, db)
	{
		fdb_probe_actor_enter("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("timeKeeperVersionFromDatetime");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("timeKeeperVersionFromDatetime", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TimeKeeperVersionFromDatetimeActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TimeKeeperVersionFromDatetimeActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< TimeKeeperVersionFromDatetimeActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
[[nodiscard]] Future<Version> timeKeeperVersionFromDatetime( std::string const& datetime, Database const& db ) {
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	return Future<Version>(new TimeKeeperVersionFromDatetimeActor(datetime, db));
															#line 1512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
}

#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"

															#line 1517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
namespace {
// This generated class is to be used only via timeKeeperEpochsFromVersion()
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
template <class TimeKeeperEpochsFromVersionActor>
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
class TimeKeeperEpochsFromVersionActorState {
															#line 1524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
public:
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	TimeKeeperEpochsFromVersionActorState(Version const& v,Reference<ReadYourWritesTransaction> const& tr) 
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		 : v(v),
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		   tr(tr),
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		   versionMap(timeKeeperPrefixRange.begin),
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		   min(0),
															#line 468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		   max((int64_t)now()),
															#line 469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		   mid(),
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		   found()
															#line 1543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
	{
		fdb_probe_actor_create("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this));

	}
	~TimeKeeperEpochsFromVersionActorState() 
	{
		fdb_probe_actor_destroy("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			;
															#line 1562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
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
		this->~TimeKeeperEpochsFromVersionActorState();
		static_cast<TimeKeeperEpochsFromVersionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (!static_cast<TimeKeeperEpochsFromVersionActor*>(this)->SAV<Optional<int64_t>>::futures) { (void)(found.first + (v - found.second) / CLIENT_KNOBS->CORE_VERSIONSPERSECOND); this->~TimeKeeperEpochsFromVersionActorState(); static_cast<TimeKeeperEpochsFromVersionActor*>(this)->destroy(); return 0; }
															#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		new (&static_cast<TimeKeeperEpochsFromVersionActor*>(this)->SAV< Optional<int64_t> >::value()) Optional<int64_t>(found.first + (v - found.second) / CLIENT_KNOBS->CORE_VERSIONSPERSECOND);
		this->~TimeKeeperEpochsFromVersionActorState();
		static_cast<TimeKeeperEpochsFromVersionActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		mid = (min + max + 1) / 2;
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		StrictFuture<KeyBackedRangeResult<std::pair<int64_t, Version>>> __when_expr_0 = versionMap.getRange(tr, min, mid, 1, Snapshot::False, Reverse::True);
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (static_cast<TimeKeeperEpochsFromVersionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<TimeKeeperEpochsFromVersionActor*>(this)->actor_wait_state = 1;
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TimeKeeperEpochsFromVersionActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >*>(static_cast<TimeKeeperEpochsFromVersionActor*>(this)));
															#line 1613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
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
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (rangeResult.results.size() != 1)
															#line 1635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		{
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			if (mid == min)
															#line 1639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			{
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
				StrictFuture<Void> __when_expr_1 = store(rangeResult, versionMap.getRange(tr, 0, (int64_t)now(), 1));
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
				if (static_cast<TimeKeeperEpochsFromVersionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
				static_cast<TimeKeeperEpochsFromVersionActor*>(this)->actor_wait_state = 2;
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TimeKeeperEpochsFromVersionActor, 1, Void >*>(static_cast<TimeKeeperEpochsFromVersionActor*>(this)));
															#line 1650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont3(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1loopBody1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(KeyBackedRangeResult<std::pair<int64_t, Version>> const& __rangeResult,int loopDepth) 
	{
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		rangeResult = __rangeResult;
															#line 1669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(KeyBackedRangeResult<std::pair<int64_t, Version>> && __rangeResult,int loopDepth) 
	{
		rangeResult = std::move(__rangeResult);
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TimeKeeperEpochsFromVersionActor*>(this)->actor_wait_state > 0) static_cast<TimeKeeperEpochsFromVersionActor*>(this)->actor_wait_state = 0;
		static_cast<TimeKeeperEpochsFromVersionActor*>(this)->ActorCallback< TimeKeeperEpochsFromVersionActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >::remove();

	}
	void a_callback_fire(ActorCallback< TimeKeeperEpochsFromVersionActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >*,KeyBackedRangeResult<std::pair<int64_t, Version>> const& value) 
	{
		fdb_probe_actor_enter("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TimeKeeperEpochsFromVersionActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >*,KeyBackedRangeResult<std::pair<int64_t, Version>> && value) 
	{
		fdb_probe_actor_enter("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TimeKeeperEpochsFromVersionActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >*,Error err) 
	{
		fdb_probe_actor_enter("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		found = rangeResult.results[0];
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (v < found.second)
															#line 1738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		{
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			max = found.first;
															#line 1742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		}
		else
		{
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			if (found.first == min)
															#line 1748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			{
				return a_body1break1(loopDepth==0?0:loopDepth-1); // break
			}
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			min = found.first;
															#line 1754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		min = mid;
															#line 1764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		return a_body1loopHead1(loopDepth); // continue

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (rangeResult.results.size() != 1)
															#line 1773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		{
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			if (!static_cast<TimeKeeperEpochsFromVersionActor*>(this)->SAV<Optional<int64_t>>::futures) { (void)(Optional<int64_t>()); this->~TimeKeeperEpochsFromVersionActorState(); static_cast<TimeKeeperEpochsFromVersionActor*>(this)->destroy(); return 0; }
															#line 1777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			new (&static_cast<TimeKeeperEpochsFromVersionActor*>(this)->SAV< Optional<int64_t> >::value()) Optional<int64_t>(Optional<int64_t>());
			this->~TimeKeeperEpochsFromVersionActorState();
			static_cast<TimeKeeperEpochsFromVersionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		found = rangeResult.results[0];
															#line 1785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		if (rangeResult.results.size() != 1)
															#line 1794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		{
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
			if (!static_cast<TimeKeeperEpochsFromVersionActor*>(this)->SAV<Optional<int64_t>>::futures) { (void)(Optional<int64_t>()); this->~TimeKeeperEpochsFromVersionActorState(); static_cast<TimeKeeperEpochsFromVersionActor*>(this)->destroy(); return 0; }
															#line 1798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
			new (&static_cast<TimeKeeperEpochsFromVersionActor*>(this)->SAV< Optional<int64_t> >::value()) Optional<int64_t>(Optional<int64_t>());
			this->~TimeKeeperEpochsFromVersionActorState();
			static_cast<TimeKeeperEpochsFromVersionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
		found = rangeResult.results[0];
															#line 1806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<TimeKeeperEpochsFromVersionActor*>(this)->actor_wait_state > 0) static_cast<TimeKeeperEpochsFromVersionActor*>(this)->actor_wait_state = 0;
		static_cast<TimeKeeperEpochsFromVersionActor*>(this)->ActorCallback< TimeKeeperEpochsFromVersionActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TimeKeeperEpochsFromVersionActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TimeKeeperEpochsFromVersionActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TimeKeeperEpochsFromVersionActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	Version v;
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	KeyBackedMap<int64_t, Version> versionMap;
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	int64_t min;
															#line 468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	int64_t max;
															#line 469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	int64_t mid;
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	std::pair<int64_t, Version> found;
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	KeyBackedRangeResult<std::pair<int64_t, Version>> rangeResult;
															#line 1890 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
};
// This generated class is to be used only via timeKeeperEpochsFromVersion()
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
class TimeKeeperEpochsFromVersionActor final : public Actor<Optional<int64_t>>, public ActorCallback< TimeKeeperEpochsFromVersionActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >, public ActorCallback< TimeKeeperEpochsFromVersionActor, 1, Void >, public FastAllocated<TimeKeeperEpochsFromVersionActor>, public TimeKeeperEpochsFromVersionActorState<TimeKeeperEpochsFromVersionActor> {
															#line 1895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
public:
	using FastAllocated<TimeKeeperEpochsFromVersionActor>::operator new;
	using FastAllocated<TimeKeeperEpochsFromVersionActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<int64_t>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TimeKeeperEpochsFromVersionActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >;
friend struct ActorCallback< TimeKeeperEpochsFromVersionActor, 1, Void >;
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	TimeKeeperEpochsFromVersionActor(Version const& v,Reference<ReadYourWritesTransaction> const& tr) 
															#line 1907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
		 : Actor<Optional<int64_t>>(),
		   TimeKeeperEpochsFromVersionActorState<TimeKeeperEpochsFromVersionActor>(v, tr)
	{
		fdb_probe_actor_enter("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("timeKeeperEpochsFromVersion");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("timeKeeperEpochsFromVersion", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TimeKeeperEpochsFromVersionActor, 0, KeyBackedRangeResult<std::pair<int64_t, Version>> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TimeKeeperEpochsFromVersionActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
[[nodiscard]] Future<Optional<int64_t>> timeKeeperEpochsFromVersion( Version const& v, Reference<ReadYourWritesTransaction> const& tr ) {
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
	return Future<Optional<int64_t>>(new TimeKeeperEpochsFromVersionActor(v, tr));
															#line 1936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.g.cpp"
}

#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainer.actor.cpp"
