#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
/*
 * BackupContainerLocalDirectory.actor.cpp
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

#include "fdbclient/BackupContainerLocalDirectory.h"
#include "fdbrpc/AsyncFileReadAhead.actor.h"
#include "fdbrpc/IAsyncFile.h"
#include "flow/Platform.actor.h"
#include "flow/Platform.h"
#include "fdbrpc/simulator.h"
#include "flow/actorcompiler.h" // This must be the last #include.

namespace {

class BackupFile : public IBackupFile, ReferenceCounted<BackupFile> {
public:
	BackupFile(const std::string& fileName, Reference<IAsyncFile> file, const std::string& finalFullPath)
	  : IBackupFile(fileName), m_file(file), m_writeOffset(0), m_finalFullPath(finalFullPath),
	    m_blockSize(CLIENT_KNOBS->BACKUP_LOCAL_FILE_WRITE_BLOCK) {
		if (BUGGIFY) {
			m_blockSize = deterministicRandom()->randomInt(100, 20000);
		}
		m_buffer.reserve(m_buffer.arena(), m_blockSize);
	}

	Future<Void> append(const void* data, int len) override {
		m_buffer.append(m_buffer.arena(), (const uint8_t*)data, len);

		if (m_buffer.size() >= m_blockSize) {
			return flush(m_blockSize);
		}

		return Void();
	}

	Future<Void> flush(int size) {
		// Avoid empty write
		if (size == 0) {
			return Void();
		}

		ASSERT(size <= m_buffer.size());

		// Keep a reference to the old buffer
		Standalone<VectorRef<uint8_t>> old = m_buffer;
		// Make a new buffer, initialized with the excess bytes over the block size from the old buffer
		m_buffer = Standalone<VectorRef<uint8_t>>(old.slice(size, old.size()));

		// Write the old buffer to the underlying file and update the write offset
		Future<Void> r = uncancellable(holdWhile(old, m_file->write(old.begin(), size, m_writeOffset)));
		m_writeOffset += size;

		return r;
	}

																#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
// This generated class is to be used only via finish_impl()
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
template <class Finish_implActor>
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
class Finish_implActorState {
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
public:
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
	Finish_implActorState(Reference<BackupFile> const& f) 
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		 : f(f)
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
	{
		fdb_probe_actor_create("finish_impl", reinterpret_cast<unsigned long>(this));

	}
	~Finish_implActorState() 
	{
		fdb_probe_actor_destroy("finish_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
			StrictFuture<Void> __when_expr_0 = f->flush(f->m_buffer.size());
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
			if (static_cast<Finish_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<Finish_implActor*>(this)->actor_wait_state = 1;
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< Finish_implActor, 0, Void >*>(static_cast<Finish_implActor*>(this)));
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
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
		this->~Finish_implActorState();
		static_cast<Finish_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		StrictFuture<Void> __when_expr_1 = f->m_file->truncate(f->size());
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		if (static_cast<Finish_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Finish_implActor*>(this)->actor_wait_state = 2;
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Finish_implActor, 1, Void >*>(static_cast<Finish_implActor*>(this)));
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		StrictFuture<Void> __when_expr_1 = f->m_file->truncate(f->size());
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		if (static_cast<Finish_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<Finish_implActor*>(this)->actor_wait_state = 2;
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< Finish_implActor, 1, Void >*>(static_cast<Finish_implActor*>(this)));
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
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
		if (static_cast<Finish_implActor*>(this)->actor_wait_state > 0) static_cast<Finish_implActor*>(this)->actor_wait_state = 0;
		static_cast<Finish_implActor*>(this)->ActorCallback< Finish_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Finish_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("finish_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finish_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< Finish_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("finish_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finish_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< Finish_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("finish_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finish_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->m_file->sync();
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		if (static_cast<Finish_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<Finish_implActor*>(this)->actor_wait_state = 3;
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< Finish_implActor, 2, Void >*>(static_cast<Finish_implActor*>(this)));
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		StrictFuture<Void> __when_expr_2 = f->m_file->sync();
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		if (static_cast<Finish_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<Finish_implActor*>(this)->actor_wait_state = 3;
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< Finish_implActor, 2, Void >*>(static_cast<Finish_implActor*>(this)));
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
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
		if (static_cast<Finish_implActor*>(this)->actor_wait_state > 0) static_cast<Finish_implActor*>(this)->actor_wait_state = 0;
		static_cast<Finish_implActor*>(this)->ActorCallback< Finish_implActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Finish_implActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("finish_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finish_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< Finish_implActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("finish_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finish_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< Finish_implActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("finish_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finish_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		std::string name = f->m_file->getFilename();
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		f->m_file.clear();
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		StrictFuture<Void> __when_expr_3 = IAsyncFileSystem::filesystem()->renameFile(name, f->m_finalFullPath);
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		if (static_cast<Finish_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<Finish_implActor*>(this)->actor_wait_state = 4;
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< Finish_implActor, 3, Void >*>(static_cast<Finish_implActor*>(this)));
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		std::string name = f->m_file->getFilename();
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		f->m_file.clear();
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		StrictFuture<Void> __when_expr_3 = IAsyncFileSystem::filesystem()->renameFile(name, f->m_finalFullPath);
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		if (static_cast<Finish_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<Finish_implActor*>(this)->actor_wait_state = 4;
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< Finish_implActor, 3, Void >*>(static_cast<Finish_implActor*>(this)));
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
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
		if (static_cast<Finish_implActor*>(this)->actor_wait_state > 0) static_cast<Finish_implActor*>(this)->actor_wait_state = 0;
		static_cast<Finish_implActor*>(this)->ActorCallback< Finish_implActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Finish_implActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("finish_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finish_impl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< Finish_implActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("finish_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finish_impl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< Finish_implActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("finish_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finish_impl", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		if (!static_cast<Finish_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Finish_implActorState(); static_cast<Finish_implActor*>(this)->destroy(); return 0; }
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		new (&static_cast<Finish_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Finish_implActorState();
		static_cast<Finish_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		if (!static_cast<Finish_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~Finish_implActorState(); static_cast<Finish_implActor*>(this)->destroy(); return 0; }
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		new (&static_cast<Finish_implActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~Finish_implActorState();
		static_cast<Finish_implActor*>(this)->finishSendAndDelPromiseRef();
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
	void a_exitChoose4() 
	{
		if (static_cast<Finish_implActor*>(this)->actor_wait_state > 0) static_cast<Finish_implActor*>(this)->actor_wait_state = 0;
		static_cast<Finish_implActor*>(this)->ActorCallback< Finish_implActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< Finish_implActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("finish_impl", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finish_impl", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< Finish_implActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("finish_impl", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finish_impl", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< Finish_implActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("finish_impl", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("finish_impl", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
	Reference<BackupFile> f;
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
};
// This generated class is to be used only via finish_impl()
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
class Finish_implActor final : public Actor<Void>, public ActorCallback< Finish_implActor, 0, Void >, public ActorCallback< Finish_implActor, 1, Void >, public ActorCallback< Finish_implActor, 2, Void >, public ActorCallback< Finish_implActor, 3, Void >, public FastAllocated<Finish_implActor>, public Finish_implActorState<Finish_implActor> {
															#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
public:
	using FastAllocated<Finish_implActor>::operator new;
	using FastAllocated<Finish_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< Finish_implActor, 0, Void >;
friend struct ActorCallback< Finish_implActor, 1, Void >;
friend struct ActorCallback< Finish_implActor, 2, Void >;
friend struct ActorCallback< Finish_implActor, 3, Void >;
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
	Finish_implActor(Reference<BackupFile> const& f) 
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		 : Actor<Void>(),
		   Finish_implActorState<Finish_implActor>(f)
	{
		fdb_probe_actor_enter("finish_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("finish_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("finish_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< Finish_implActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< Finish_implActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< Finish_implActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< Finish_implActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
[[nodiscard]] static Future<Void> finish_impl( Reference<BackupFile> const& f ) {
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
	return Future<Void>(new Finish_implActor(f));
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
}

#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"

	int64_t size() const override { return m_buffer.size() + m_writeOffset; }

	Future<Void> finish() override { return finish_impl(Reference<BackupFile>::addRef(this)); }

	void addref() override { return ReferenceCounted<BackupFile>::addref(); }
	void delref() override { return ReferenceCounted<BackupFile>::delref(); }

private:
	Reference<IAsyncFile> m_file;
	Standalone<VectorRef<uint8_t>> m_buffer;
	int64_t m_writeOffset;
	std::string m_finalFullPath;
	int m_blockSize;
};

															#line 579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
// This generated class is to be used only via listFiles_impl()
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
template <class ListFiles_implActor>
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
class ListFiles_implActorState {
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
public:
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
	ListFiles_implActorState(std::string const& path,std::string const& m_path) 
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		 : path(path),
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		   m_path(m_path),
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		   files()
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
	{
		fdb_probe_actor_create("listFiles_impl", reinterpret_cast<unsigned long>(this));

	}
	~ListFiles_implActorState() 
	{
		fdb_probe_actor_destroy("listFiles_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
			StrictFuture<Void> __when_expr_0 = platform::findFilesRecursivelyAsync(joinPath(m_path, path), &files);
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
			if (static_cast<ListFiles_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ListFiles_implActor*>(this)->actor_wait_state = 1;
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ListFiles_implActor, 0, Void >*>(static_cast<ListFiles_implActor*>(this)));
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
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
		this->~ListFiles_implActorState();
		static_cast<ListFiles_implActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		BackupContainerFileSystem::FilesAndSizesT results;
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		if (g_network->isSimulated())
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		{
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
			files.erase( std::remove_if(files.begin(), files.end(), [](std::string const& f) { return StringRef(f).endsWith(LiteralStringRef(".lnk")); }), files.end());
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		}
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		for( const auto& f : files ) {
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
			StringRef s(f);
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
			if (!s.endsWith(LiteralStringRef(".part")) && !s.endsWith(LiteralStringRef(".temp")))
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
			{
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
				results.push_back({ f.substr(m_path.size() + 1), ::fileSize(f) });
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
			}
		}
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		if (!static_cast<ListFiles_implActor*>(this)->SAV<BackupContainerFileSystem::FilesAndSizesT>::futures) { (void)(results); this->~ListFiles_implActorState(); static_cast<ListFiles_implActor*>(this)->destroy(); return 0; }
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		new (&static_cast<ListFiles_implActor*>(this)->SAV< BackupContainerFileSystem::FilesAndSizesT >::value()) BackupContainerFileSystem::FilesAndSizesT(results);
		this->~ListFiles_implActorState();
		static_cast<ListFiles_implActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		BackupContainerFileSystem::FilesAndSizesT results;
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		if (g_network->isSimulated())
															#line 678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		{
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
			files.erase( std::remove_if(files.begin(), files.end(), [](std::string const& f) { return StringRef(f).endsWith(LiteralStringRef(".lnk")); }), files.end());
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		}
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		for( const auto& f : files ) {
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
			StringRef s(f);
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
			if (!s.endsWith(LiteralStringRef(".part")) && !s.endsWith(LiteralStringRef(".temp")))
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
			{
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
				results.push_back({ f.substr(m_path.size() + 1), ::fileSize(f) });
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
			}
		}
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
		if (!static_cast<ListFiles_implActor*>(this)->SAV<BackupContainerFileSystem::FilesAndSizesT>::futures) { (void)(results); this->~ListFiles_implActorState(); static_cast<ListFiles_implActor*>(this)->destroy(); return 0; }
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		new (&static_cast<ListFiles_implActor*>(this)->SAV< BackupContainerFileSystem::FilesAndSizesT >::value()) BackupContainerFileSystem::FilesAndSizesT(results);
		this->~ListFiles_implActorState();
		static_cast<ListFiles_implActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<ListFiles_implActor*>(this)->actor_wait_state > 0) static_cast<ListFiles_implActor*>(this)->actor_wait_state = 0;
		static_cast<ListFiles_implActor*>(this)->ActorCallback< ListFiles_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ListFiles_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("listFiles_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listFiles_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ListFiles_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("listFiles_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listFiles_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ListFiles_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("listFiles_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listFiles_impl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
	std::string path;
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
	std::string m_path;
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
	std::vector<std::string> files;
															#line 776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
};
// This generated class is to be used only via listFiles_impl()
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
class ListFiles_implActor final : public Actor<BackupContainerFileSystem::FilesAndSizesT>, public ActorCallback< ListFiles_implActor, 0, Void >, public FastAllocated<ListFiles_implActor>, public ListFiles_implActorState<ListFiles_implActor> {
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
public:
	using FastAllocated<ListFiles_implActor>::operator new;
	using FastAllocated<ListFiles_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<BackupContainerFileSystem::FilesAndSizesT>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ListFiles_implActor, 0, Void >;
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
	ListFiles_implActor(std::string const& path,std::string const& m_path) 
															#line 792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
		 : Actor<BackupContainerFileSystem::FilesAndSizesT>(),
		   ListFiles_implActorState<ListFiles_implActor>(path, m_path)
	{
		fdb_probe_actor_enter("listFiles_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("listFiles_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("listFiles_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ListFiles_implActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
[[nodiscard]] static Future<BackupContainerFileSystem::FilesAndSizesT> listFiles_impl( std::string const& path, std::string const& m_path ) {
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"
	return Future<BackupContainerFileSystem::FilesAndSizesT>(new ListFiles_implActor(path, m_path));
															#line 819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.g.cpp"
}

#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupContainerLocalDirectory.actor.cpp"

} // namespace

void BackupContainerLocalDirectory::addref() {
	return ReferenceCounted<BackupContainerLocalDirectory>::addref();
}
void BackupContainerLocalDirectory::delref() {
	return ReferenceCounted<BackupContainerLocalDirectory>::delref();
}

std::string BackupContainerLocalDirectory::getURLFormat() {
	return "file://</path/to/base/dir/>";
}

BackupContainerLocalDirectory::BackupContainerLocalDirectory(const std::string& url,
                                                             const Optional<std::string>& encryptionKeyFileName) {
	setEncryptionKey(encryptionKeyFileName);

	std::string path;
	if (url.find("file://") != 0) {
		TraceEvent(SevWarn, "BackupContainerLocalDirectory")
		    .detail("Description", "Invalid URL for BackupContainerLocalDirectory")
		    .detail("URL", url);
	}

	path = url.substr(7);
	// Remove trailing slashes on path
	path.erase(path.find_last_not_of("\\/") + 1);

	std::string absolutePath = abspath(path);

	if (!g_network->isSimulated() && path != absolutePath) {
		TraceEvent(SevWarn, "BackupContainerLocalDirectory")
		    .detail("Description", "Backup path must be absolute (e.g. file:///some/path)")
		    .detail("URL", url)
		    .detail("Path", path)
		    .detail("AbsolutePath", absolutePath);
		// throw io_error();
		IBackupContainer::lastOpenError =
		    format("Backup path '%s' must be the absolute path '%s'", path.c_str(), absolutePath.c_str());
		throw backup_invalid_url();
	}

	// Finalized path written to will be will be <path>/backup-<uid>
	m_path = path;
}

Future<std::vector<std::string>> BackupContainerLocalDirectory::listURLs(const std::string& url) {
	std::string path;
	if (url.find("file://") != 0) {
		TraceEvent(SevWarn, "BackupContainerLocalDirectory")
		    .detail("Description", "Invalid URL for BackupContainerLocalDirectory")
		    .detail("URL", url);
	}

	path = url.substr(7);
	// Remove trailing slashes on path
	path.erase(path.find_last_not_of("\\/") + 1);

	if (!g_network->isSimulated() && path != abspath(path)) {
		TraceEvent(SevWarn, "BackupContainerLocalDirectory")
		    .detail("Description", "Backup path must be absolute (e.g. file:///some/path)")
		    .detail("URL", url)
		    .detail("Path", path);
		throw io_error();
	}
	std::vector<std::string> dirs = platform::listDirectories(path);
	std::vector<std::string> results;

	for (const auto& r : dirs) {
		if (r == "." || r == "..")
			continue;
		results.push_back(std::string("file://") + joinPath(path, r));
	}

	return results;
}

Future<Void> BackupContainerLocalDirectory::create() {
	if (usesEncryption()) {
		return encryptionSetupComplete();
	}
	// No directory should be created here because create() can be called by any process working with the container URL,
	// such as fdbbackup. Since "local directory" containers are by definition local to the machine they are
	// accessed from, the container's creation (in this case the creation of a directory) must be ensured prior to
	// every file creation, which is done in openFile(). Creating the directory here will result in unnecessary
	// directories being created on machines that run fdbbackup but not agents.
	return Void();
}

Future<bool> BackupContainerLocalDirectory::exists() {
	return directoryExists(m_path);
}

Future<Reference<IAsyncFile>> BackupContainerLocalDirectory::readFile(const std::string& path) {
	int flags = IAsyncFile::OPEN_NO_AIO | IAsyncFile::OPEN_READONLY | IAsyncFile::OPEN_UNCACHED;
	if (usesEncryption()) {
		flags |= IAsyncFile::OPEN_ENCRYPTED;
	}
	// Simulation does not properly handle opening the same file from multiple machines using a shared filesystem,
	// so create a symbolic link to make each file opening appear to be unique.  This could also work in production
	// but only if the source directory is writeable which shouldn't be required for a restore.
	std::string fullPath = joinPath(m_path, path);
#ifndef _WIN32
	if (g_network->isSimulated()) {
		if (!fileExists(fullPath)) {
			throw file_not_found();
		}

		if (g_simulator.getCurrentProcess()->uid == UID()) {
			TraceEvent(SevError, "BackupContainerReadFileOnUnsetProcessID").log();
		}
		std::string uniquePath = fullPath + "." + g_simulator.getCurrentProcess()->uid.toString() + ".lnk";
		unlink(uniquePath.c_str());
		ASSERT(symlink(basename(path).c_str(), uniquePath.c_str()) == 0);
		fullPath = uniquePath;
	}
// Opening cached mode forces read/write mode at a lower level, overriding the readonly request.  So cached mode
// can't be used because backup files are read-only.  Cached mode can only help during restore task retries handled
// by the same process that failed the first task execution anyway, which is a very rare case.
#endif
	Future<Reference<IAsyncFile>> f = IAsyncFileSystem::filesystem()->open(fullPath, flags, 0644);

	if (g_network->isSimulated()) {
		int blockSize = 0;
		// Extract block size from the filename, if present
		size_t lastComma = path.find_last_of(',');
		if (lastComma != path.npos) {
			blockSize = atoi(path.substr(lastComma + 1).c_str());
		}
		if (blockSize <= 0) {
			blockSize = deterministicRandom()->randomInt(1e4, 1e6);
		}
		if (deterministicRandom()->random01() < .01) {
			blockSize /= deterministicRandom()->randomInt(1, 3);
		}
		ASSERT(blockSize > 0);

		return map(f, [=](Reference<IAsyncFile> fr) {
			int readAhead = deterministicRandom()->randomInt(0, 3);
			int reads = deterministicRandom()->randomInt(1, 3);
			int cacheSize = deterministicRandom()->randomInt(0, 3);
			return Reference<IAsyncFile>(new AsyncFileReadAheadCache(fr, blockSize, readAhead, reads, cacheSize));
		});
	}

	return f;
}

Future<Reference<IBackupFile>> BackupContainerLocalDirectory::writeFile(const std::string& path) {
	int flags = IAsyncFile::OPEN_NO_AIO | IAsyncFile::OPEN_UNCACHED | IAsyncFile::OPEN_CREATE |
	            IAsyncFile::OPEN_ATOMIC_WRITE_AND_CREATE | IAsyncFile::OPEN_READWRITE;
	if (usesEncryption()) {
		flags |= IAsyncFile::OPEN_ENCRYPTED;
	}
	std::string fullPath = joinPath(m_path, path);
	platform::createDirectory(parentDirectory(fullPath));
	std::string temp = fullPath + "." + deterministicRandom()->randomUniqueID().toString() + ".temp";
	Future<Reference<IAsyncFile>> f = IAsyncFileSystem::filesystem()->open(temp, flags, 0644);
	return map(f, [=](Reference<IAsyncFile> f) { return Reference<IBackupFile>(new BackupFile(path, f, fullPath)); });
}

Future<Void> BackupContainerLocalDirectory::deleteFile(const std::string& path) {
	::deleteFile(joinPath(m_path, path));
	return Void();
}

Future<BackupContainerFileSystem::FilesAndSizesT> BackupContainerLocalDirectory::listFiles(
    const std::string& path,
    std::function<bool(std::string const&)>) {
	return listFiles_impl(path, m_path);
}

Future<Void> BackupContainerLocalDirectory::deleteContainer(int* pNumDeleted) {
	// In order to avoid deleting some random directory due to user error, first describe the backup
	// and make sure it has something in it.
	return map(describeBackup(false, invalidVersion), [=](BackupDescription const& desc) {
		// If the backup has no snapshots and no logs then it's probably not a valid backup
		if (desc.snapshots.size() == 0 && !desc.minLogBegin.present())
			throw backup_invalid_url();

		int count = platform::eraseDirectoryRecursive(m_path);
		if (pNumDeleted != nullptr)
			*pNumDeleted = count;

		return Void();
	});
}
