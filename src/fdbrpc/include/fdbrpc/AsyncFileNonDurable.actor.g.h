#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
/*
 * AsyncFileNonDurable.actor.h
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
#if defined(NO_INTELLISENSE) && !defined(FLOW_ASYNCFILENONDURABLE_ACTOR_G_H)
#define FLOW_ASYNCFILENONDURABLE_ACTOR_G_H
#include "fdbrpc/AsyncFileNonDurable.actor.g.h"
#elif !defined(FLOW_ASYNCFILENONDURABLE_ACTOR_H)
#define FLOW_ASYNCFILENONDURABLE_ACTOR_H

#include "flow/flow.h"
#include "flow/IAsyncFile.h"
#include "flow/ActorCollection.h"
#include "fdbrpc/simulator.h"
#include "fdbrpc/TraceFileIO.h"
#include "fdbrpc/RangeMap.h"
#include "flow/actorcompiler.h" // This must be the last #include.

#undef max
#undef min

															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
[[nodiscard]] Future<Void> sendOnProcess( ISimulator::ProcessInfo* const& process, Promise<Void> const& promise, TaskPriority const& taskID );

#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 48 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
[[nodiscard]] Future<Void> sendErrorOnProcess( ISimulator::ProcessInfo* const& process, Promise<Void> const& promise, Error const& e, TaskPriority const& taskID );

#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"

extern Future<Void> waitShutdownSignal();

															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
namespace {
// This generated class is to be used only via sendErrorOnShutdown()
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
template <class T, class SendErrorOnShutdownActor>
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class SendErrorOnShutdownActorState {
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	SendErrorOnShutdownActorState(Future<T> const& in,bool const& assertOnCancel = false) 
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		 : in(in),
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   assertOnCancel(assertOnCancel)
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this));

	}
	~SendErrorOnShutdownActorState() 
	{
		fdb_probe_actor_destroy("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				StrictFuture<Void> __when_expr_0 = waitShutdownSignal();
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				if (static_cast<SendErrorOnShutdownActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				StrictFuture<T> __when_expr_1 = in;
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
				static_cast<SendErrorOnShutdownActor*>(this)->actor_wait_state = 1;
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SendErrorOnShutdownActor, 0, Void >*>(static_cast<SendErrorOnShutdownActor*>(this)));
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SendErrorOnShutdownActor, 1, T >*>(static_cast<SendErrorOnShutdownActor*>(this)));
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~SendErrorOnShutdownActorState();
		static_cast<SendErrorOnShutdownActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			ASSERT(e.code() != error_code_actor_cancelled || !assertOnCancel);
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch2(io_error().asInjectedFault(), loopDepth);
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch2(io_error().asInjectedFault(), loopDepth);
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1when2(T const& rep,int loopDepth) 
	{
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<SendErrorOnShutdownActor*>(this)->SAV<T>::futures) { (void)(rep); this->~SendErrorOnShutdownActorState(); static_cast<SendErrorOnShutdownActor*>(this)->destroy(); return 0; }
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<SendErrorOnShutdownActor*>(this)->SAV< T >::value()) T(rep);
		this->~SendErrorOnShutdownActorState();
		static_cast<SendErrorOnShutdownActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(T && rep,int loopDepth) 
	{
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<SendErrorOnShutdownActor*>(this)->SAV<T>::futures) { (void)(rep); this->~SendErrorOnShutdownActorState(); static_cast<SendErrorOnShutdownActor*>(this)->destroy(); return 0; }
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<SendErrorOnShutdownActor*>(this)->SAV< T >::value()) T(rep);
		this->~SendErrorOnShutdownActorState();
		static_cast<SendErrorOnShutdownActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SendErrorOnShutdownActor*>(this)->actor_wait_state > 0) static_cast<SendErrorOnShutdownActor*>(this)->actor_wait_state = 0;
		static_cast<SendErrorOnShutdownActor*>(this)->ActorCallback< SendErrorOnShutdownActor, 0, Void >::remove();
		static_cast<SendErrorOnShutdownActor*>(this)->ActorCallback< SendErrorOnShutdownActor, 1, T >::remove();

	}
	void a_callback_fire(ActorCallback< SendErrorOnShutdownActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SendErrorOnShutdownActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SendErrorOnShutdownActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SendErrorOnShutdownActor, 1, T >*,T const& value) 
	{
		fdb_probe_actor_enter("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SendErrorOnShutdownActor, 1, T >*,T && value) 
	{
		fdb_probe_actor_enter("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SendErrorOnShutdownActor, 1, T >*,Error err) 
	{
		fdb_probe_actor_enter("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<T> in;
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	bool assertOnCancel;
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via sendErrorOnShutdown()
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
template <class T>
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class SendErrorOnShutdownActor final : public Actor<T>, public ActorCallback< SendErrorOnShutdownActor<T>, 0, Void >, public ActorCallback< SendErrorOnShutdownActor<T>, 1, T >, public FastAllocated<SendErrorOnShutdownActor<T>>, public SendErrorOnShutdownActorState<T, SendErrorOnShutdownActor<T>> {
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<SendErrorOnShutdownActor<T>>::operator new;
	using FastAllocated<SendErrorOnShutdownActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<T>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SendErrorOnShutdownActor<T>, 0, Void >;
friend struct ActorCallback< SendErrorOnShutdownActor<T>, 1, T >;
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	SendErrorOnShutdownActor(Future<T> const& in,bool const& assertOnCancel = false) 
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<T>(),
		   SendErrorOnShutdownActorState<T, SendErrorOnShutdownActor<T>>(in, assertOnCancel)
	{
		fdb_probe_actor_enter("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("sendErrorOnShutdown");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("sendErrorOnShutdown", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SendErrorOnShutdownActor<T>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
template <class T>
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] Future<T> sendErrorOnShutdown( Future<T> const& in, bool const& assertOnCancel = false ) {
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<T>(new SendErrorOnShutdownActor<T>(in, assertOnCancel));
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"

class AsyncFileDetachable final : public IAsyncFile, public ReferenceCounted<AsyncFileDetachable> {
private:
	Reference<IAsyncFile> file;
	Future<Void> shutdown;
	bool assertOnReadWriteCancel;

public:
	explicit AsyncFileDetachable(Reference<IAsyncFile> file) : file(file), assertOnReadWriteCancel(true) {
		shutdown = doShutdown(this);
	}

																#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
[[nodiscard]] Future<Void> doShutdown( AsyncFileDetachable* const& self );
template <class> friend class AsyncFileDetachable_DoShutdownActorState;

#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
																#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
[[nodiscard]] static Future<Reference<IAsyncFile>> open( Future<Reference<IAsyncFile>> const& wrappedFile );
template <class> friend class AsyncFileDetachable_OpenActorState;

#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"

	void addref() override { ReferenceCounted<AsyncFileDetachable>::addref(); }
	void delref() override { ReferenceCounted<AsyncFileDetachable>::delref(); }

	Future<int> read(void* data, int length, int64_t offset) override;
	Future<Void> write(void const* data, int length, int64_t offset) override;
	Future<Void> truncate(int64_t size) override;
	Future<Void> sync() override;
	Future<int64_t> size() const override;

	int64_t debugFD() const override {
		if (!file.getPtr())
			throw io_error().asInjectedFault();
		return file->debugFD();
	}
	std::string getFilename() const override {
		if (!file.getPtr())
			throw io_error().asInjectedFault();
		return file->getFilename();
	}
};

// An async file implementation which wraps another async file and will randomly destroy sectors that it is writing when
// killed This is used to simulate a power failure which prevents all written data from being persisted to disk
class AsyncFileNonDurable final : public IAsyncFile, public ReferenceCounted<AsyncFileNonDurable> {
public:
	UID id;
	std::string filename;

	// For files that use atomic write and create, they are initially created with an extra suffix
	std::string initialFilename;

	// An approximation of the size of the file; .size() should be used instead of this variable in most cases
	mutable int64_t approximateSize;

	// The address of the machine that opened the file
	NetworkAddress openedAddress;

	bool aio;

private:
	// The wrapped IAsyncFile
	Reference<IAsyncFile> file;

	// The maximum amount of time a write is delayed before being passed along to the underlying file
	double maxWriteDelay;

	// Modifications which haven't been pushed to file, mapped by the location in the file that is being modified.
	// Be sure to update minSizeAfterPendingModifications when modifying pendingModifications.
	RangeMap<uint64_t, Future<Void>> pendingModifications;
	// The size of the file after the set of pendingModifications completes,
	// (the set pending at the time of reading this member). Must be updated in
	// lockstep with any inserts into the pendingModifications map. Tracking
	// this variable is necessary so that we can know the range of the file a
	// truncate is modifying, so we can insert it into the pendingModifications
	// map. Until minSizeAfterPendingModificationsIsExact is true, this is only a lower bound.
	mutable int64_t minSizeAfterPendingModifications = 0;
	mutable bool minSizeAfterPendingModificationsIsExact = false;

	// Will be blocked whenever kill is running
	Promise<Void> killed;
	Promise<Void> killComplete;

	// Used by sync (and kill) to force writes which have not yet been passed along.
	// If true is sent, then writes will be durable.  If false, then they may not be durable.
	Promise<bool> startSyncPromise;

	// The performance parameters of the simulated disk
	Reference<DiskParameters> diskParameters;

	// Set to true the first time sync is called on the file
	bool hasBeenSynced;

	// Used to describe what corruption is allowed by the file as well as the type of corruption being used on a
	// particular page
	enum KillMode { NO_CORRUPTION = 0, DROP_ONLY = 1, FULL_CORRUPTION = 2 };

	// Limits what types of corruption are applied to writes from this file
	KillMode killMode;

	ActorCollection
	    reponses; // cannot call getResult on this actor collection, since the actors will be on different processes

	AsyncFileNonDurable(const std::string& filename,
	                    const std::string& initialFilename,
	                    Reference<IAsyncFile> file,
	                    Reference<DiskParameters> diskParameters,
	                    NetworkAddress openedAddress,
	                    bool aio)
	  : filename(filename), initialFilename(initialFilename), approximateSize(0), openedAddress(openedAddress),
	    aio(aio), file(file), pendingModifications(uint64_t(-1)), diskParameters(diskParameters), reponses(false) {

		// This is only designed to work in simulation
		ASSERT(g_network->isSimulated());
		this->id = deterministicRandom()->randomUniqueID();

		//TraceEvent("AsyncFileNonDurable_Create", id).detail("Filename", filename);
		maxWriteDelay = FLOW_KNOBS->NON_DURABLE_MAX_WRITE_DELAY;
		hasBeenSynced = false;

		killMode = (KillMode)deterministicRandom()->randomInt(1, 3);
		//TraceEvent("AsyncFileNonDurable_CreateEnd", id).detail("Filename", filename).backtrace();
	}

public:
	static std::map<std::string, Future<Void>> filesBeingDeleted;

	// Creates a new AsyncFileNonDurable which wraps the provided IAsyncFile
																#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
[[nodiscard]] static Future<Reference<IAsyncFile>> open( std::string const& filename, std::string const& actualFilename, Future<Reference<IAsyncFile>> const& wrappedFile, Reference<DiskParameters> const& diskParameters, bool const& aio );
template <class> friend class AsyncFileNonDurable_OpenActorState;

#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"

	~AsyncFileNonDurable() override {
		//TraceEvent("AsyncFileNonDurable_Destroy", id).detail("Filename", filename);
	}

	void addref() override { ReferenceCounted<AsyncFileNonDurable>::addref(); }

	void delref() override {
		if (delref_no_destroy()) {
			if (filesBeingDeleted.count(filename) == 0) {
				//TraceEvent("AsyncFileNonDurable_StartDelete", id).detail("Filename", filename);
				Future<Void> deleteFuture = closeFile(this);
				if (!deleteFuture.isReady())
					filesBeingDeleted[filename] = deleteFuture;
			}

			removeOpenFile(filename, this);
			if (initialFilename != filename) {
				removeOpenFile(initialFilename, this);
			}
		}
	}

	// Removes a file from the openFiles map
	static void removeOpenFile(std::string filename, AsyncFileNonDurable* file);

	// Passes along reads straight to the underlying file, waiting for any outstanding changes that could affect the
	// results
	Future<int> read(void* data, int length, int64_t offset) override { return read(this, data, length, offset); }

	// Writes data to the file.  Writes are delayed a random amount of time before being
	// passed to the underlying file
	Future<Void> write(void const* data, int length, int64_t offset) override {
		//TraceEvent("AsyncFileNonDurable_Write", id).detail("Filename", filename).detail("Offset", offset).detail("Length", length);
		if (length == 0) {
			TraceEvent(SevWarnAlways, "AsyncFileNonDurable_EmptyModification", id).detail("Filename", filename);
			return Void();
		}

		debugFileSet("AsyncFileNonDurableWrite", filename, data, offset, length);

		Promise<Void> writeStarted;
		Promise<Future<Void>> writeEnded;
		writeEnded.send(write(this, writeStarted, writeEnded.getFuture(), data, length, offset));
		return writeStarted.getFuture();
	}

	// Truncates the file.  Truncates are delayed a random amount of time before being
	// passed to the underlying file
	Future<Void> truncate(int64_t size) override {
		//TraceEvent("AsyncFileNonDurable_Truncate", id).detail("Filename", filename).detail("Offset", size);
		debugFileTruncate("AsyncFileNonDurableTruncate", filename, size);

		Promise<Void> truncateStarted;
		Promise<Future<Void>> truncateEnded;
		truncateEnded.send(truncate(this, truncateStarted, truncateEnded.getFuture(), size));
		return truncateStarted.getFuture();
	}

	// Fsyncs the file.  This allows all delayed modifications to the file to complete before
	// syncing the underlying file
	Future<Void> sync() override {
		//TraceEvent("AsyncFileNonDurable_Sync", id).detail("Filename", filename);
		Future<Void> syncFuture = sync(this, true);
		reponses.add(syncFuture);
		return syncFuture;
	}

	// Passes along size requests to the underlying file, augmenting with any writes past the end of the file
	Future<int64_t> size() const override { return size(this); }

	int64_t debugFD() const override { return file->debugFD(); }

	std::string getFilename() const override { return file->getFilename(); }

	// Forces a non-durable sync (some writes are not made or made incorrectly)
	// This is used when the file should 'die' without first completing its operations
	//(e.g. to simulate power failure)
	Future<Void> kill() {
		TraceEvent("AsyncFileNonDurable_Kill", id).detail("Filename", filename);
		CODE_PROBE(true, "AsyncFileNonDurable was killed", probe::decoration::rare);
		return sync(this, false);
	}

private:
	// Returns a future that is used to ensure the waiter ends up on the main thread
	Future<Void> returnToMainThread() {
		Promise<Void> p;
		Future<Void> f = p.getFuture();
		g_network->onMainThread(std::move(p), g_network->getCurrentTask());
		return f;
	}

	// Gets existing modifications that overlap the specified range.  Optionally inserts a new modification into the map
	std::vector<Future<Void>> getModificationsAndInsert(int64_t offset,
	                                                    int64_t length,
	                                                    bool insertModification = false,
	                                                    Future<Void> value = Void()) {
		auto modification = RangeMapRange<uint64_t>(offset, length >= 0 ? offset + length : uint64_t(-1));
		auto priorModifications = pendingModifications.intersectingRanges(modification);

		// Aggregate existing modifications in this range
		std::vector<Future<Void>> modificationFutures;
		for (auto itr = priorModifications.begin(); itr != priorModifications.end(); ++itr) {
			if (itr.value().isValid() && (!itr.value().isReady() || itr.value().isError())) {
				modificationFutures.push_back(itr.value());
			}
		}

		// Add the modification if we are doing a write or truncate
		if (insertModification)
			pendingModifications.insert(modification, value);

		return modificationFutures;
	}

	// Checks if the file is killed.  If so, then the current sync is completed if running and then an error is thrown
																#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via checkKilled()
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
template <class CheckKilledActor>
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class CheckKilledActorState {
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	CheckKilledActorState(AsyncFileNonDurable const* const& self,std::string const& context) 
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   context(context)
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("checkKilled", reinterpret_cast<unsigned long>(this));

	}
	~CheckKilledActorState() 
	{
		fdb_probe_actor_destroy("checkKilled", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (self->killed.isSet())
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			{
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				StrictFuture<Void> __when_expr_0 = self->killComplete.getFuture();
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				if (static_cast<CheckKilledActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<CheckKilledActor*>(this)->actor_wait_state = 1;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckKilledActor, 0, Void >*>(static_cast<CheckKilledActor*>(this)));
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~CheckKilledActorState();
		static_cast<CheckKilledActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<CheckKilledActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckKilledActorState(); static_cast<CheckKilledActor*>(this)->destroy(); return 0; }
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<CheckKilledActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheckKilledActorState();
		static_cast<CheckKilledActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		TraceEvent("AsyncFileNonDurable_KilledFileOperation", self->id) .detail("In", context) .detail("Filename", self->filename);
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		CODE_PROBE(true, "AsyncFileNonDurable operation killed", probe::decoration::rare);
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		TraceEvent("AsyncFileNonDurable_KilledFileOperation", self->id) .detail("In", context) .detail("Filename", self->filename);
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		CODE_PROBE(true, "AsyncFileNonDurable operation killed", probe::decoration::rare);
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(io_error().asInjectedFault(), loopDepth);
															#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"

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
		if (static_cast<CheckKilledActor*>(this)->actor_wait_state > 0) static_cast<CheckKilledActor*>(this)->actor_wait_state = 0;
		static_cast<CheckKilledActor*>(this)->ActorCallback< CheckKilledActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CheckKilledActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("checkKilled", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkKilled", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckKilledActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("checkKilled", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkKilled", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckKilledActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("checkKilled", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkKilled", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable const* self;
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	std::string context;
															#line 756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via checkKilled()
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class CheckKilledActor final : public Actor<Void>, public ActorCallback< CheckKilledActor, 0, Void >, public FastAllocated<CheckKilledActor>, public CheckKilledActorState<CheckKilledActor> {
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<CheckKilledActor>::operator new;
	using FastAllocated<CheckKilledActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckKilledActor, 0, Void >;
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	CheckKilledActor(AsyncFileNonDurable const* const& self,std::string const& context) 
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<Void>(),
		   CheckKilledActorState<CheckKilledActor>(self, context)
	{
		fdb_probe_actor_enter("checkKilled", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkKilled");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkKilled", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckKilledActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] static Future<Void> checkKilled( AsyncFileNonDurable const* const& self, std::string const& context ) {
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<Void>(new CheckKilledActor(self, context));
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"

	// Passes along reads straight to the underlying file, waiting for any outstanding changes that could affect the
	// results
																#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via onRead()
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
template <class OnReadActor>
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class OnReadActorState {
															#line 812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	OnReadActorState(AsyncFileNonDurable* const& self,void* const& data,int const& length,int64_t const& offset) 
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   data(data),
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   length(length),
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   offset(offset)
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("onRead", reinterpret_cast<unsigned long>(this));

	}
	~OnReadActorState() 
	{
		fdb_probe_actor_destroy("onRead", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = checkKilled(self, "Read");
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OnReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OnReadActor*>(this)->actor_wait_state = 1;
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnReadActor, 0, Void >*>(static_cast<OnReadActor*>(this)));
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~OnReadActorState();
		static_cast<OnReadActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> priorModifications = self->getModificationsAndInsert(offset, length);
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_1 = waitForAll(priorModifications);
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnReadActor*>(this)->actor_wait_state = 2;
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnReadActor, 1, Void >*>(static_cast<OnReadActor*>(this)));
															#line 879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> priorModifications = self->getModificationsAndInsert(offset, length);
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_1 = waitForAll(priorModifications);
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnReadActor*>(this)->actor_wait_state = 2;
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnReadActor, 1, Void >*>(static_cast<OnReadActor*>(this)));
															#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		if (static_cast<OnReadActor*>(this)->actor_wait_state > 0) static_cast<OnReadActor*>(this)->actor_wait_state = 0;
		static_cast<OnReadActor*>(this)->ActorCallback< OnReadActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnReadActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OnReadActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OnReadActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		readFuture = self->file->read(data, length, offset);
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_2 = success(readFuture) || self->killed.getFuture();
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<OnReadActor*>(this)->actor_wait_state = 3;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OnReadActor, 2, Void >*>(static_cast<OnReadActor*>(this)));
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		readFuture = self->file->read(data, length, offset);
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_2 = success(readFuture) || self->killed.getFuture();
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<OnReadActor*>(this)->actor_wait_state = 3;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OnReadActor, 2, Void >*>(static_cast<OnReadActor*>(this)));
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		if (static_cast<OnReadActor*>(this)->actor_wait_state > 0) static_cast<OnReadActor*>(this)->actor_wait_state = 0;
		static_cast<OnReadActor*>(this)->ActorCallback< OnReadActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnReadActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< OnReadActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< OnReadActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_3 = checkKilled(self, "ReadEnd");
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<OnReadActor*>(this)->actor_wait_state = 4;
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< OnReadActor, 3, Void >*>(static_cast<OnReadActor*>(this)));
															#line 1075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_3 = checkKilled(self, "ReadEnd");
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when1(__when_expr_3.get(), loopDepth); };
		static_cast<OnReadActor*>(this)->actor_wait_state = 4;
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< OnReadActor, 3, Void >*>(static_cast<OnReadActor*>(this)));
															#line 1091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		if (static_cast<OnReadActor*>(this)->actor_wait_state > 0) static_cast<OnReadActor*>(this)->actor_wait_state = 0;
		static_cast<OnReadActor*>(this)->ActorCallback< OnReadActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnReadActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< OnReadActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< OnReadActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		debugFileCheck("AsyncFileNonDurableRead", self->filename, data, offset, length);
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<OnReadActor*>(this)->SAV<int>::futures) { (void)(readFuture.get()); this->~OnReadActorState(); static_cast<OnReadActor*>(this)->destroy(); return 0; }
															#line 1165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<OnReadActor*>(this)->SAV< int >::value()) int(readFuture.get());
		this->~OnReadActorState();
		static_cast<OnReadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		debugFileCheck("AsyncFileNonDurableRead", self->filename, data, offset, length);
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<OnReadActor*>(this)->SAV<int>::futures) { (void)(readFuture.get()); this->~OnReadActorState(); static_cast<OnReadActor*>(this)->destroy(); return 0; }
															#line 1179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<OnReadActor*>(this)->SAV< int >::value()) int(readFuture.get());
		this->~OnReadActorState();
		static_cast<OnReadActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<OnReadActor*>(this)->actor_wait_state > 0) static_cast<OnReadActor*>(this)->actor_wait_state = 0;
		static_cast<OnReadActor*>(this)->ActorCallback< OnReadActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnReadActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< OnReadActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< OnReadActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable* self;
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	void* data;
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	int length;
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	int64_t offset;
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<int> readFuture;
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via onRead()
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class OnReadActor final : public Actor<int>, public ActorCallback< OnReadActor, 0, Void >, public ActorCallback< OnReadActor, 1, Void >, public ActorCallback< OnReadActor, 2, Void >, public ActorCallback< OnReadActor, 3, Void >, public FastAllocated<OnReadActor>, public OnReadActorState<OnReadActor> {
															#line 1265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<OnReadActor>::operator new;
	using FastAllocated<OnReadActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnReadActor, 0, Void >;
friend struct ActorCallback< OnReadActor, 1, Void >;
friend struct ActorCallback< OnReadActor, 2, Void >;
friend struct ActorCallback< OnReadActor, 3, Void >;
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	OnReadActor(AsyncFileNonDurable* const& self,void* const& data,int const& length,int64_t const& offset) 
															#line 1279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<int>(),
		   OnReadActorState<OnReadActor>(self, data, length, offset)
	{
		fdb_probe_actor_enter("onRead", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("onRead");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("onRead", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OnReadActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< OnReadActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< OnReadActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< OnReadActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] Future<int> onRead( AsyncFileNonDurable* const& self, void* const& data, int const& length, int64_t const& offset ) {
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<int>(new OnReadActor(self, data, length, offset));
															#line 1309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"

																#line 1314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
[[nodiscard]] Future<int> read( AsyncFileNonDurable* const& self, void* const& data, int const& length, int64_t const& offset );
template <class> friend class AsyncFileNonDurable_ReadActorState;

#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"

	// Delays writes a random amount of time before passing them through to the underlying file.
	// If a kill interrupts the delay, then the output could be the correct write, part of the write,
	// or none of the write.  It may also corrupt parts of sectors which have not been written correctly
																#line 1323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via write()
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
template <class WriteActor>
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class WriteActorState {
															#line 1329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	WriteActorState(AsyncFileNonDurable* const& self,Promise<Void> const& writeStarted,Future<Future<Void>> const& ownFuture,void const* const& data,int const& length,int64_t const& offset) 
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   writeStarted(writeStarted),
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   ownFuture(ownFuture),
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   data(data),
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   length(length),
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   offset(offset),
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   dataCopy(StringRef((uint8_t*)data, length)),
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentProcess(g_simulator->getCurrentProcess()),
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentTaskID(g_network->getCurrentTask())
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("write", reinterpret_cast<unsigned long>(this));

	}
	~WriteActorState() 
	{
		fdb_probe_actor_destroy("write", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = g_simulator->onMachine(currentProcess);
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WriteActor*>(this)->actor_wait_state = 1;
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 0, Void >*>(static_cast<WriteActor*>(this)));
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~WriteActorState();
		static_cast<WriteActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		delayDuration = g_simulator->speedUpSimulation ? 0.0001 : (deterministicRandom()->random01() * self->maxWriteDelay);
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		startSyncFuture = self->startSyncPromise.getFuture();
															#line 1399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		try {
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = checkKilled(self, "Write");
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<WriteActor*>(this)->actor_wait_state = 2;
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 1, Void >*>(static_cast<WriteActor*>(this)));
															#line 1410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		delayDuration = g_simulator->speedUpSimulation ? 0.0001 : (deterministicRandom()->random01() * self->maxWriteDelay);
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		startSyncFuture = self->startSyncPromise.getFuture();
															#line 1427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		try {
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = checkKilled(self, "Write");
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<WriteActor*>(this)->actor_wait_state = 2;
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 1, Void >*>(static_cast<WriteActor*>(this)));
															#line 1438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

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
		if (static_cast<WriteActor*>(this)->actor_wait_state > 0) static_cast<WriteActor*>(this)->actor_wait_state = 0;
		static_cast<WriteActor*>(this)->ActorCallback< WriteActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WriteActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WriteActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		saveDurable = true;
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_4 = delay(delayDuration);
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont2when1(__when_expr_4.get(), loopDepth); };
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<bool> __when_expr_5 = startSyncFuture;
															#line 1524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont2when2(__when_expr_5.get(), loopDepth); };
		static_cast<WriteActor*>(this)->actor_wait_state = 5;
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 4, Void >*>(static_cast<WriteActor*>(this)));
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 5, bool >*>(static_cast<WriteActor*>(this)));
															#line 1531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			self->reponses.add(sendErrorOnProcess(currentProcess, writeStarted, e, currentTaskID));
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 1543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Future<Void>> __when_expr_2 = ownFuture;
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteActor*>(this)->actor_wait_state = 3;
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 2, Future<Void> >*>(static_cast<WriteActor*>(this)));
															#line 1564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Future<Void>> __when_expr_2 = ownFuture;
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WriteActor*>(this)->actor_wait_state = 3;
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 2, Future<Void> >*>(static_cast<WriteActor*>(this)));
															#line 1580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		if (static_cast<WriteActor*>(this)->actor_wait_state > 0) static_cast<WriteActor*>(this)->actor_wait_state = 0;
		static_cast<WriteActor*>(this)->ActorCallback< WriteActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WriteActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WriteActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(Future<Void> const& writeEnded,int loopDepth) 
	{
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> priorModifications = self->getModificationsAndInsert(offset, length, true, writeEnded);
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->minSizeAfterPendingModifications = std::max(self->minSizeAfterPendingModifications, offset + length);
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (BUGGIFY_WITH_PROB(0.001) && !g_simulator->speedUpSimulation)
															#line 1656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			priorModifications.push_back( delay(deterministicRandom()->random01() * FLOW_KNOBS->MAX_PRIOR_MODIFICATION_DELAY) || self->killed.getFuture());
															#line 1660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
		else
		{
															#line 379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			priorModifications.push_back(waitUntilDiskReady(self->diskParameters, length) || self->killed.getFuture());
															#line 1666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_3 = waitForAll(priorModifications);
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<WriteActor*>(this)->actor_wait_state = 4;
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 3, Void >*>(static_cast<WriteActor*>(this)));
															#line 1677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(Future<Void> && writeEnded,int loopDepth) 
	{
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> priorModifications = self->getModificationsAndInsert(offset, length, true, writeEnded);
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->minSizeAfterPendingModifications = std::max(self->minSizeAfterPendingModifications, offset + length);
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (BUGGIFY_WITH_PROB(0.001) && !g_simulator->speedUpSimulation)
															#line 1690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			priorModifications.push_back( delay(deterministicRandom()->random01() * FLOW_KNOBS->MAX_PRIOR_MODIFICATION_DELAY) || self->killed.getFuture());
															#line 1694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
		else
		{
															#line 379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			priorModifications.push_back(waitUntilDiskReady(self->diskParameters, length) || self->killed.getFuture());
															#line 1700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_3 = waitForAll(priorModifications);
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 1706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
		static_cast<WriteActor*>(this)->actor_wait_state = 4;
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 3, Void >*>(static_cast<WriteActor*>(this)));
															#line 1711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3when1(Future<Void> const& writeEnded,int loopDepth) 
	{
		loopDepth = a_body1cont4(writeEnded, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Future<Void> && writeEnded,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(writeEnded), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<WriteActor*>(this)->actor_wait_state > 0) static_cast<WriteActor*>(this)->actor_wait_state = 0;
		static_cast<WriteActor*>(this)->ActorCallback< WriteActor, 2, Future<Void> >::remove();

	}
	void a_callback_fire(ActorCallback< WriteActor, 2, Future<Void> >*,Future<Void> const& value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< WriteActor, 2, Future<Void> >*,Future<Void> && value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< WriteActor, 2, Future<Void> >*,Error err) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->approximateSize = std::max(self->approximateSize, length + offset);
															#line 386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->reponses.add(sendOnProcess(currentProcess, writeStarted, currentTaskID));
															#line 1785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->approximateSize = std::max(self->approximateSize, length + offset);
															#line 386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->reponses.add(sendOnProcess(currentProcess, writeStarted, currentTaskID));
															#line 1796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont9(loopDepth);

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
		if (static_cast<WriteActor*>(this)->actor_wait_state > 0) static_cast<WriteActor*>(this)->actor_wait_state = 0;
		static_cast<WriteActor*>(this)->ActorCallback< WriteActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< WriteActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< WriteActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont9(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont10(int loopDepth) 
	{
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		debugFileCheck("AsyncFileNonDurableWriteAfterWait", self->filename, dataCopy.begin(), offset, length);
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		ASSERT(!self->aio || (offset % 4096 == 0 && length % 4096 == 0));
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		int diskPageLength = saveDurable ? length : 4096;
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		int diskSectorLength = saveDurable ? length : 512;
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> writeFutures;
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		for(int writeOffset = 0;writeOffset < length;) {
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			int pageLength = diskPageLength;
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (!self->aio && !saveDurable)
															#line 1895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			{
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				pageLength = std::min<int64_t>((int64_t)length - writeOffset, diskPageLength - ((offset + writeOffset) % diskPageLength));
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			}
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			KillMode pageKillMode = (KillMode)deterministicRandom()->randomInt(0, self->killMode + 1);
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			for(int pageOffset = 0;pageOffset < pageLength;) {
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				int sectorLength = diskSectorLength;
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				if (!self->aio && !saveDurable)
															#line 1909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
				{
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
					sectorLength = std::min<int64_t>((int64_t)length - (writeOffset + pageOffset), diskSectorLength - ((offset + writeOffset + pageOffset) % diskSectorLength));
															#line 1913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
				}
															#line 441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				if (saveDurable || pageKillMode == NO_CORRUPTION || (pageKillMode == FULL_CORRUPTION && deterministicRandom()->random01() < 0.25))
															#line 1917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
				{
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
					writeFutures.push_back(self->file->write( dataCopy.begin() + writeOffset + pageOffset, sectorLength, offset + writeOffset + pageOffset));
															#line 1921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
				}
				else
				{
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
					if (pageKillMode == FULL_CORRUPTION && deterministicRandom()->random01() < 0.66667)
															#line 1927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
					{
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
						int side = deterministicRandom()->randomInt(0, 3);
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
						bool garbage = side == 2 || deterministicRandom()->random01() < 0.5;
															#line 460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
						int64_t goodStart = 0;
															#line 461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
						int64_t goodEnd = sectorLength;
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
						int64_t badStart = 0;
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
						int64_t badEnd = sectorLength;
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
						if (side == 0)
															#line 1943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
						{
															#line 466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
							goodEnd = deterministicRandom()->randomInt(0, sectorLength);
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
							badStart = goodEnd;
															#line 1949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
						}
						else
						{
															#line 468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
							if (side == 1)
															#line 1955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
							{
															#line 469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
								badEnd = deterministicRandom()->randomInt(0, sectorLength);
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
								goodStart = badEnd;
															#line 1961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
							}
							else
							{
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
								goodEnd = 0;
															#line 1967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
							}
						}
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
						if (garbage && badStart != badEnd)
															#line 1972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
						{
															#line 476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
							uint8_t* badData = const_cast<uint8_t*>(&dataCopy.begin()[badStart + writeOffset + pageOffset]);
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
							for(int i = 0;i < badEnd - badStart;i += sizeof(uint32_t)) {
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
								uint32_t val = deterministicRandom()->randomUInt32();
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
								memcpy(&badData[i], &val, std::min(badEnd - badStart - i, (int64_t)sizeof(uint32_t)));
															#line 1982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
							}
															#line 482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
							writeFutures.push_back(self->file->write(dataCopy.begin() + writeOffset + pageOffset, sectorLength, offset + writeOffset + pageOffset));
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
							debugFileSet("AsyncFileNonDurableBadWrite", self->filename, dataCopy.begin() + writeOffset + pageOffset, offset + writeOffset + pageOffset, sectorLength);
															#line 1988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
						}
						else
						{
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
							if (goodStart != goodEnd)
															#line 1994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
							{
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
								writeFutures.push_back( self->file->write(dataCopy.begin() + goodStart + writeOffset + pageOffset, goodEnd - goodStart, goodStart + offset + writeOffset + pageOffset));
															#line 1998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
							}
						}
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
						TraceEvent("AsyncFileNonDurable_BadWrite", self->id) .detail("Offset", offset + writeOffset + pageOffset) .detail("Length", sectorLength) .detail("GoodStart", goodStart) .detail("GoodEnd", goodEnd) .detail("HasGarbage", garbage) .detail("Side", side) .detail("Filename", self->filename);
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
						CODE_PROBE(true, "AsyncFileNonDurable bad write", probe::decoration::rare);
															#line 2005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
					}
					else
					{
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
						TraceEvent("AsyncFileNonDurable_DroppedWrite", self->id) .detail("Offset", offset + writeOffset + pageOffset) .detail("Length", sectorLength) .detail("Filename", self->filename);
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
						CODE_PROBE(true, "AsyncFileNonDurable dropped write", probe::decoration::rare);
															#line 2013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
					}
				}
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				pageOffset += sectorLength;
															#line 2018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			}
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			writeOffset += pageLength;
															#line 2022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_6 = waitForAll(writeFutures);
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<WriteActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont10when1(__when_expr_6.get(), loopDepth); };
		static_cast<WriteActor*>(this)->actor_wait_state = 6;
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< WriteActor, 6, Void >*>(static_cast<WriteActor*>(this)));
															#line 2033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when2(bool const& durable,int loopDepth) 
	{
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		saveDurable = durable;
															#line 2054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when2(bool && durable,int loopDepth) 
	{
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		saveDurable = durable;
															#line 2063 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<WriteActor*>(this)->actor_wait_state > 0) static_cast<WriteActor*>(this)->actor_wait_state = 0;
		static_cast<WriteActor*>(this)->ActorCallback< WriteActor, 4, Void >::remove();
		static_cast<WriteActor*>(this)->ActorCallback< WriteActor, 5, bool >::remove();

	}
	void a_callback_fire(ActorCallback< WriteActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< WriteActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< WriteActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< WriteActor, 5, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose5();
		try {
			a_body1cont2when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< WriteActor, 5, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose5();
		try {
			a_body1cont2when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< WriteActor, 5, bool >*,Error err) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1cont10cont1(Void const& _,int loopDepth) 
	{
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<WriteActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteActorState(); static_cast<WriteActor*>(this)->destroy(); return 0; }
															#line 2169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<WriteActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteActorState();
		static_cast<WriteActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10cont1(Void && _,int loopDepth) 
	{
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<WriteActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WriteActorState(); static_cast<WriteActor*>(this)->destroy(); return 0; }
															#line 2181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<WriteActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WriteActorState();
		static_cast<WriteActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont10when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<WriteActor*>(this)->actor_wait_state > 0) static_cast<WriteActor*>(this)->actor_wait_state = 0;
		static_cast<WriteActor*>(this)->ActorCallback< WriteActor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WriteActor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose6();
		try {
			a_body1cont10when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< WriteActor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose6();
		try {
			a_body1cont10when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< WriteActor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), 6);

	}
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable* self;
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	Promise<Void> writeStarted;
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<Future<Void>> ownFuture;
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	void const* data;
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	int length;
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	int64_t offset;
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	Standalone<StringRef> dataCopy;
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	ISimulator::ProcessInfo* currentProcess;
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	TaskPriority currentTaskID;
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	double delayDuration;
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<bool> startSyncFuture;
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	bool saveDurable;
															#line 2276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via write()
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class WriteActor final : public Actor<Void>, public ActorCallback< WriteActor, 0, Void >, public ActorCallback< WriteActor, 1, Void >, public ActorCallback< WriteActor, 2, Future<Void> >, public ActorCallback< WriteActor, 3, Void >, public ActorCallback< WriteActor, 4, Void >, public ActorCallback< WriteActor, 5, bool >, public ActorCallback< WriteActor, 6, Void >, public FastAllocated<WriteActor>, public WriteActorState<WriteActor> {
															#line 2281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<WriteActor>::operator new;
	using FastAllocated<WriteActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WriteActor, 0, Void >;
friend struct ActorCallback< WriteActor, 1, Void >;
friend struct ActorCallback< WriteActor, 2, Future<Void> >;
friend struct ActorCallback< WriteActor, 3, Void >;
friend struct ActorCallback< WriteActor, 4, Void >;
friend struct ActorCallback< WriteActor, 5, bool >;
friend struct ActorCallback< WriteActor, 6, Void >;
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	WriteActor(AsyncFileNonDurable* const& self,Promise<Void> const& writeStarted,Future<Future<Void>> const& ownFuture,void const* const& data,int const& length,int64_t const& offset) 
															#line 2298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<Void>(),
		   WriteActorState<WriteActor>(self, writeStarted, ownFuture, data, length, offset)
	{
		fdb_probe_actor_enter("write", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("write");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("write", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WriteActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WriteActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< WriteActor, 2, Future<Void> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< WriteActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< WriteActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< WriteActor, 6, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] Future<Void> write( AsyncFileNonDurable* const& self, Promise<Void> const& writeStarted, Future<Future<Void>> const& ownFuture, void const* const& data, int const& length, int64_t const& offset ) {
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<Void>(new WriteActor(self, writeStarted, ownFuture, data, length, offset));
															#line 2330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"

	// Delays truncates a random amount of time before passing them through to the underlying file.
	// If a kill interrupts the delay, then the truncate may or may not be performed
																#line 2337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via truncate()
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
template <class TruncateActor>
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class TruncateActorState {
															#line 2343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	TruncateActorState(AsyncFileNonDurable* const& self,Promise<Void> const& truncateStarted,Future<Future<Void>> const& ownFuture,int64_t const& size) 
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   truncateStarted(truncateStarted),
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   ownFuture(ownFuture),
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   size(size),
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentProcess(g_simulator->getCurrentProcess()),
															#line 531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentTaskID(g_network->getCurrentTask())
															#line 2360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("truncate", reinterpret_cast<unsigned long>(this));

	}
	~TruncateActorState() 
	{
		fdb_probe_actor_destroy("truncate", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = g_simulator->onMachine(currentProcess);
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TruncateActor*>(this)->actor_wait_state = 1;
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 0, Void >*>(static_cast<TruncateActor*>(this)));
															#line 2382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~TruncateActorState();
		static_cast<TruncateActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		delayDuration = g_simulator->speedUpSimulation ? 0.0001 : (deterministicRandom()->random01() * self->maxWriteDelay);
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		startSyncFuture = self->startSyncPromise.getFuture();
															#line 2407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		try {
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = checkKilled(self, "Truncate");
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TruncateActor*>(this)->actor_wait_state = 2;
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 1, Void >*>(static_cast<TruncateActor*>(this)));
															#line 2418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		delayDuration = g_simulator->speedUpSimulation ? 0.0001 : (deterministicRandom()->random01() * self->maxWriteDelay);
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		startSyncFuture = self->startSyncPromise.getFuture();
															#line 2435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		try {
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = checkKilled(self, "Truncate");
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TruncateActor*>(this)->actor_wait_state = 2;
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 1, Void >*>(static_cast<TruncateActor*>(this)));
															#line 2446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

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
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TruncateActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		saveDurable = true;
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_5 = delay(delayDuration);
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont2when1(__when_expr_5.get(), loopDepth); };
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<bool> __when_expr_6 = startSyncFuture;
															#line 2532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont2when2(__when_expr_6.get(), loopDepth); };
		static_cast<TruncateActor*>(this)->actor_wait_state = 6;
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 5, Void >*>(static_cast<TruncateActor*>(this)));
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 6, bool >*>(static_cast<TruncateActor*>(this)));
															#line 2539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			self->reponses.add(sendErrorOnProcess(currentProcess, truncateStarted, e, currentTaskID));
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 2551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Future<Void>> __when_expr_2 = ownFuture;
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<TruncateActor*>(this)->actor_wait_state = 3;
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 2, Future<Void> >*>(static_cast<TruncateActor*>(this)));
															#line 2572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Future<Void>> __when_expr_2 = ownFuture;
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<TruncateActor*>(this)->actor_wait_state = 3;
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 2, Future<Void> >*>(static_cast<TruncateActor*>(this)));
															#line 2588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TruncateActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(int loopDepth) 
	{
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!self->minSizeAfterPendingModificationsIsExact)
															#line 2660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_3 = success(self->size());
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
			static_cast<TruncateActor*>(this)->actor_wait_state = 4;
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 3, Void >*>(static_cast<TruncateActor*>(this)));
															#line 2671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont5(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3when1(Future<Void> const& __truncateEnded,int loopDepth) 
	{
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		truncateEnded = __truncateEnded;
															#line 2685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Future<Void> && __truncateEnded,int loopDepth) 
	{
		truncateEnded = std::move(__truncateEnded);
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 2, Future<Void> >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 2, Future<Void> >*,Future<Void> const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 2, Future<Void> >*,Future<Void> && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< TruncateActor, 2, Future<Void> >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont5(int loopDepth) 
	{
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		ASSERT(self->minSizeAfterPendingModificationsIsExact);
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		int64_t beginModifiedRange = std::min(size, self->minSizeAfterPendingModifications);
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->minSizeAfterPendingModifications = size;
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> priorModifications = self->getModificationsAndInsert(beginModifiedRange, -1, true, truncateEnded);
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (BUGGIFY_WITH_PROB(0.001))
															#line 2760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			priorModifications.push_back( delay(deterministicRandom()->random01() * FLOW_KNOBS->MAX_PRIOR_MODIFICATION_DELAY) || self->killed.getFuture());
															#line 2764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
		else
		{
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			priorModifications.push_back(waitUntilDiskReady(self->diskParameters, 0) || self->killed.getFuture());
															#line 2770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_4 = waitForAll(priorModifications);
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 2776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1cont1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont5when1(__when_expr_4.get(), loopDepth); };
		static_cast<TruncateActor*>(this)->actor_wait_state = 5;
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 4, Void >*>(static_cast<TruncateActor*>(this)));
															#line 2781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

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
	void a_exitChoose4() 
	{
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< TruncateActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->approximateSize = size;
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->reponses.add(sendOnProcess(currentProcess, truncateStarted, currentTaskID));
															#line 2867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->approximateSize = size;
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->reponses.add(sendOnProcess(currentProcess, truncateStarted, currentTaskID));
															#line 2878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< TruncateActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont11(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont12(int loopDepth) 
	{
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (g_network->check_yield(TaskPriority::DefaultYield))
															#line 2963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_7 = delay(0, TaskPriority::DefaultYield);
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1Catch1(__when_expr_7.getError(), loopDepth); else return a_body1cont12when1(__when_expr_7.get(), loopDepth); };
			static_cast<TruncateActor*>(this)->actor_wait_state = 7;
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 7, Void >*>(static_cast<TruncateActor*>(this)));
															#line 2974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont12cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when2(bool const& durable,int loopDepth) 
	{
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		saveDurable = durable;
															#line 3000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when2(bool && durable,int loopDepth) 
	{
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		saveDurable = durable;
															#line 3009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 5, Void >::remove();
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 6, bool >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< TruncateActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 6, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose6();
		try {
			a_body1cont2when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 6, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose6();
		try {
			a_body1cont2when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< TruncateActor, 6, bool >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 6);

	}
	int a_body1cont12cont1(int loopDepth) 
	{
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (saveDurable || self->killMode == NO_CORRUPTION || deterministicRandom()->random01() < 0.5)
															#line 3115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_8 = self->file->truncate(size);
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<TruncateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_8.isReady()) { if (__when_expr_8.isError()) return a_body1Catch1(__when_expr_8.getError(), loopDepth); else return a_body1cont12cont1when1(__when_expr_8.get(), loopDepth); };
			static_cast<TruncateActor*>(this)->actor_wait_state = 8;
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_8.addCallbackAndClear(static_cast<ActorCallback< TruncateActor, 8, Void >*>(static_cast<TruncateActor*>(this)));
															#line 3126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		else
		{
															#line 591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			TraceEvent("AsyncFileNonDurable_DroppedTruncate", self->id).detail("Size", size);
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			CODE_PROBE(true, "AsyncFileNonDurable dropped truncate", probe::decoration::rare);
															#line 3135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = a_body1cont12cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont12cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont12cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont12when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont12when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 7, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 7, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose7();
		try {
			a_body1cont12when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 7, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose7();
		try {
			a_body1cont12when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_error(ActorCallback< TruncateActor, 7, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose7();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 7);

	}
	int a_body1cont12cont3(int loopDepth) 
	{
															#line 595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<TruncateActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TruncateActorState(); static_cast<TruncateActor*>(this)->destroy(); return 0; }
															#line 3220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<TruncateActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TruncateActorState();
		static_cast<TruncateActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont12cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont12cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont12cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont12cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont12cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose8() 
	{
		if (static_cast<TruncateActor*>(this)->actor_wait_state > 0) static_cast<TruncateActor*>(this)->actor_wait_state = 0;
		static_cast<TruncateActor*>(this)->ActorCallback< TruncateActor, 8, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TruncateActor, 8, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose8();
		try {
			a_body1cont12cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_fire(ActorCallback< TruncateActor, 8, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose8();
		try {
			a_body1cont12cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_error(ActorCallback< TruncateActor, 8, Void >*,Error err) 
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose8();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), 8);

	}
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable* self;
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	Promise<Void> truncateStarted;
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<Future<Void>> ownFuture;
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	int64_t size;
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	ISimulator::ProcessInfo* currentProcess;
															#line 531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	TaskPriority currentTaskID;
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	double delayDuration;
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<bool> startSyncFuture;
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<Void> truncateEnded;
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	bool saveDurable;
															#line 3323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via truncate()
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class TruncateActor final : public Actor<Void>, public ActorCallback< TruncateActor, 0, Void >, public ActorCallback< TruncateActor, 1, Void >, public ActorCallback< TruncateActor, 2, Future<Void> >, public ActorCallback< TruncateActor, 3, Void >, public ActorCallback< TruncateActor, 4, Void >, public ActorCallback< TruncateActor, 5, Void >, public ActorCallback< TruncateActor, 6, bool >, public ActorCallback< TruncateActor, 7, Void >, public ActorCallback< TruncateActor, 8, Void >, public FastAllocated<TruncateActor>, public TruncateActorState<TruncateActor> {
															#line 3328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<TruncateActor>::operator new;
	using FastAllocated<TruncateActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TruncateActor, 0, Void >;
friend struct ActorCallback< TruncateActor, 1, Void >;
friend struct ActorCallback< TruncateActor, 2, Future<Void> >;
friend struct ActorCallback< TruncateActor, 3, Void >;
friend struct ActorCallback< TruncateActor, 4, Void >;
friend struct ActorCallback< TruncateActor, 5, Void >;
friend struct ActorCallback< TruncateActor, 6, bool >;
friend struct ActorCallback< TruncateActor, 7, Void >;
friend struct ActorCallback< TruncateActor, 8, Void >;
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	TruncateActor(AsyncFileNonDurable* const& self,Promise<Void> const& truncateStarted,Future<Future<Void>> const& ownFuture,int64_t const& size) 
															#line 3347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<Void>(),
		   TruncateActorState<TruncateActor>(self, truncateStarted, ownFuture, size)
	{
		fdb_probe_actor_enter("truncate", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("truncate");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("truncate", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TruncateActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TruncateActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< TruncateActor, 2, Future<Void> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< TruncateActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< TruncateActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< TruncateActor, 5, Void >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< TruncateActor, 7, Void >*)0, actor_cancelled()); break;
		case 8: this->a_callback_error((ActorCallback< TruncateActor, 8, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] Future<Void> truncate( AsyncFileNonDurable* const& self, Promise<Void> const& truncateStarted, Future<Future<Void>> const& ownFuture, int64_t const& size ) {
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<Void>(new TruncateActor(self, truncateStarted, ownFuture, size));
															#line 3381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"

	// Waits for delayed modifications to the file to complete and then syncs the underlying file
	// If durable is false, then some of the delayed modifications will not be applied or will be
	// applied incorrectly
																#line 3389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via onSync()
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
template <class OnSyncActor>
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class OnSyncActorState {
															#line 3395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	OnSyncActorState(AsyncFileNonDurable* const& self,bool const& durable) 
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   durable(durable)
															#line 3404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("onSync", reinterpret_cast<unsigned long>(this));

	}
	~OnSyncActorState() 
	{
		fdb_probe_actor_destroy("onSync", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			ASSERT(durable || !self->killed.isSet());
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (durable)
															#line 3421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			{
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				self->hasBeenSynced = true;
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				StrictFuture<Void> __when_expr_0 = waitUntilDiskReady(self->diskParameters, 0, true) || self->killed.getFuture();
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<OnSyncActor*>(this)->actor_wait_state = 1;
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 0, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 3434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~OnSyncActorState();
		static_cast<OnSyncActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_1 = checkKilled(self, durable ? "Sync" : "Kill");
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnSyncActor*>(this)->actor_wait_state = 2;
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 1, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 3469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

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
		if (static_cast<OnSyncActor*>(this)->actor_wait_state > 0) static_cast<OnSyncActor*>(this)->actor_wait_state = 0;
		static_cast<OnSyncActor*>(this)->ActorCallback< OnSyncActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OnSyncActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!durable)
															#line 3553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			self->killed.send(Void());
															#line 3557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> outstandingModifications;
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<RangeMapRange<uint64_t>> stillPendingModifications;
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		auto rangeItr = self->pendingModifications.ranges();
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		for(auto itr = rangeItr.begin();itr != rangeItr.end();++itr) {
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (itr.value().isValid() && (!itr->value().isReady() || itr->value().isError()))
															#line 3569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			{
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				outstandingModifications.push_back(itr->value());
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				if (!itr.value().isReady())
															#line 3575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
				{
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
					stillPendingModifications.push_back(itr->range());
															#line 3579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
				}
			}
		}
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		Future<Void> allModifications = waitForAll(outstandingModifications);
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->pendingModifications.insert(RangeMapRange<uint64_t>(0, -1), Void());
															#line 632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		for(auto itr = stillPendingModifications.begin();itr != stillPendingModifications.end();++itr) {
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			self->pendingModifications.insert( *itr, success(allModifications));
															#line 3591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		Promise<bool> startSyncPromise = self->startSyncPromise;
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->startSyncPromise = Promise<bool>();
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		writeDurable = durable || deterministicRandom()->random01() < 0.1;
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		startSyncPromise.send(writeDurable);
															#line 646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (durable)
															#line 3603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_2 = allModifications;
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
			static_cast<OnSyncActor*>(this)->actor_wait_state = 3;
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 2, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 3614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		else
		{
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_3 = success(errorOr(allModifications));
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when2(__when_expr_3.get(), loopDepth); };
			static_cast<OnSyncActor*>(this)->actor_wait_state = 4;
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 3, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 3628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!durable)
															#line 3638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			self->killed.send(Void());
															#line 3642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<Future<Void>> outstandingModifications;
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		std::vector<RangeMapRange<uint64_t>> stillPendingModifications;
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		auto rangeItr = self->pendingModifications.ranges();
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		for(auto itr = rangeItr.begin();itr != rangeItr.end();++itr) {
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (itr.value().isValid() && (!itr->value().isReady() || itr->value().isError()))
															#line 3654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			{
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				outstandingModifications.push_back(itr->value());
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				if (!itr.value().isReady())
															#line 3660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
				{
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
					stillPendingModifications.push_back(itr->range());
															#line 3664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
				}
			}
		}
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		Future<Void> allModifications = waitForAll(outstandingModifications);
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->pendingModifications.insert(RangeMapRange<uint64_t>(0, -1), Void());
															#line 632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		for(auto itr = stillPendingModifications.begin();itr != stillPendingModifications.end();++itr) {
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			self->pendingModifications.insert( *itr, success(allModifications));
															#line 3676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		}
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		Promise<bool> startSyncPromise = self->startSyncPromise;
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->startSyncPromise = Promise<bool>();
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		writeDurable = durable || deterministicRandom()->random01() < 0.1;
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		startSyncPromise.send(writeDurable);
															#line 646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (durable)
															#line 3688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_2 = allModifications;
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
			static_cast<OnSyncActor*>(this)->actor_wait_state = 3;
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 2, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 3699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		else
		{
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_3 = success(errorOr(allModifications));
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont3when2(__when_expr_3.get(), loopDepth); };
			static_cast<OnSyncActor*>(this)->actor_wait_state = 4;
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 3, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 3713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}

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
		if (static_cast<OnSyncActor*>(this)->actor_wait_state > 0) static_cast<OnSyncActor*>(this)->actor_wait_state = 0;
		static_cast<OnSyncActor*>(this)->ActorCallback< OnSyncActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< OnSyncActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(int loopDepth) 
	{
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!durable)
															#line 3786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		{
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (self->hasBeenSynced && writeDurable && deterministicRandom()->random01() < 0.5)
															#line 3790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			{
															#line 656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				CODE_PROBE(true, "AsyncFileNonDurable kill was durable and synced", probe::decoration::rare);
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				StrictFuture<Void> __when_expr_4 = success(errorOr(self->file->sync()));
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
				if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont4when1(__when_expr_4.get(), loopDepth); };
				static_cast<OnSyncActor*>(this)->actor_wait_state = 5;
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
				__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 4, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 3803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1cont13(loopDepth);
			}
		}
		else
		{
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_5 = checkKilled(self, "SyncEnd");
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont4when2(__when_expr_5.get(), loopDepth); };
			static_cast<OnSyncActor*>(this)->actor_wait_state = 6;
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 5, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 3822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont10(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont10(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<OnSyncActor*>(this)->actor_wait_state > 0) static_cast<OnSyncActor*>(this)->actor_wait_state = 0;
		static_cast<OnSyncActor*>(this)->ActorCallback< OnSyncActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< OnSyncActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont11(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont11(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont3when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont11(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont11(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<OnSyncActor*>(this)->actor_wait_state > 0) static_cast<OnSyncActor*>(this)->actor_wait_state = 0;
		static_cast<OnSyncActor*>(this)->ActorCallback< OnSyncActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont3when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< OnSyncActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont12(int loopDepth) 
	{
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<OnSyncActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnSyncActorState(); static_cast<OnSyncActor*>(this)->destroy(); return 0; }
															#line 3982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<OnSyncActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnSyncActorState();
		static_cast<OnSyncActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont13(int loopDepth) 
	{
															#line 663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->killComplete.send(Void());
															#line 3994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont14(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont13(loopDepth);

		return loopDepth;
	}
	int a_body1cont14(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont13(loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont14(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont14(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<OnSyncActor*>(this)->actor_wait_state > 0) static_cast<OnSyncActor*>(this)->actor_wait_state = 0;
		static_cast<OnSyncActor*>(this)->ActorCallback< OnSyncActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< OnSyncActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont15(Void const& _,int loopDepth) 
	{
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_6 = self->file->sync();
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont15when1(__when_expr_6.get(), loopDepth); };
		static_cast<OnSyncActor*>(this)->actor_wait_state = 7;
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 6, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 4085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont15(Void && _,int loopDepth) 
	{
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_6 = self->file->sync();
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnSyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1cont15when1(__when_expr_6.get(), loopDepth); };
		static_cast<OnSyncActor*>(this)->actor_wait_state = 7;
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< OnSyncActor, 6, Void >*>(static_cast<OnSyncActor*>(this)));
															#line 4101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont15(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont15(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<OnSyncActor*>(this)->actor_wait_state > 0) static_cast<OnSyncActor*>(this)->actor_wait_state = 0;
		static_cast<OnSyncActor*>(this)->ActorCallback< OnSyncActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont4when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont4when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< OnSyncActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1cont15cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont15cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont12(loopDepth);

		return loopDepth;
	}
	int a_body1cont15when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont15cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont15when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont15cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<OnSyncActor*>(this)->actor_wait_state > 0) static_cast<OnSyncActor*>(this)->actor_wait_state = 0;
		static_cast<OnSyncActor*>(this)->ActorCallback< OnSyncActor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont15when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< OnSyncActor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1cont15when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< OnSyncActor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), 6);

	}
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable* self;
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	bool durable;
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	bool writeDurable;
															#line 4250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via onSync()
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class OnSyncActor final : public Actor<Void>, public ActorCallback< OnSyncActor, 0, Void >, public ActorCallback< OnSyncActor, 1, Void >, public ActorCallback< OnSyncActor, 2, Void >, public ActorCallback< OnSyncActor, 3, Void >, public ActorCallback< OnSyncActor, 4, Void >, public ActorCallback< OnSyncActor, 5, Void >, public ActorCallback< OnSyncActor, 6, Void >, public FastAllocated<OnSyncActor>, public OnSyncActorState<OnSyncActor> {
															#line 4255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<OnSyncActor>::operator new;
	using FastAllocated<OnSyncActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnSyncActor, 0, Void >;
friend struct ActorCallback< OnSyncActor, 1, Void >;
friend struct ActorCallback< OnSyncActor, 2, Void >;
friend struct ActorCallback< OnSyncActor, 3, Void >;
friend struct ActorCallback< OnSyncActor, 4, Void >;
friend struct ActorCallback< OnSyncActor, 5, Void >;
friend struct ActorCallback< OnSyncActor, 6, Void >;
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	OnSyncActor(AsyncFileNonDurable* const& self,bool const& durable) 
															#line 4272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<Void>(),
		   OnSyncActorState<OnSyncActor>(self, durable)
	{
		fdb_probe_actor_enter("onSync", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("onSync");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("onSync", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OnSyncActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< OnSyncActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< OnSyncActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< OnSyncActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< OnSyncActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< OnSyncActor, 5, Void >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< OnSyncActor, 6, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] Future<Void> onSync( AsyncFileNonDurable* const& self, bool const& durable ) {
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<Void>(new OnSyncActor(self, durable));
															#line 4305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"

																#line 4310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via sync()
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
template <class SyncActor>
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class SyncActorState {
															#line 4316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	SyncActorState(AsyncFileNonDurable* const& self,bool const& durable) 
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   durable(durable),
															#line 676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentProcess(g_simulator->getCurrentProcess()),
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentTaskID(g_network->getCurrentTask())
															#line 4329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("sync", reinterpret_cast<unsigned long>(this));

	}
	~SyncActorState() 
	{
		fdb_probe_actor_destroy("sync", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = g_simulator->onMachine(currentProcess);
															#line 678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SyncActor*>(this)->actor_wait_state = 1;
															#line 678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SyncActor, 0, Void >*>(static_cast<SyncActor*>(this)));
															#line 4351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~SyncActorState();
		static_cast<SyncActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
		try {
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = self->onSync(self, durable);
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SyncActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<SyncActor*>(this)->actor_wait_state = 2;
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SyncActor, 1, Void >*>(static_cast<SyncActor*>(this)));
															#line 4382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
		try {
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_1 = self->onSync(self, durable);
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SyncActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<SyncActor*>(this)->actor_wait_state = 2;
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SyncActor, 1, Void >*>(static_cast<SyncActor*>(this)));
															#line 4405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

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
		if (static_cast<SyncActor*>(this)->actor_wait_state > 0) static_cast<SyncActor*>(this)->actor_wait_state = 0;
		static_cast<SyncActor*>(this)->ActorCallback< SyncActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SyncActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SyncActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SyncActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			err = e;
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_3 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SyncActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<SyncActor*>(this)->actor_wait_state = 4;
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< SyncActor, 3, Void >*>(static_cast<SyncActor*>(this)));
															#line 4493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
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
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_2 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<SyncActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<SyncActor*>(this)->actor_wait_state = 3;
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SyncActor, 2, Void >*>(static_cast<SyncActor*>(this)));
															#line 4515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_2 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<SyncActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 4526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<SyncActor*>(this)->actor_wait_state = 3;
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SyncActor, 2, Void >*>(static_cast<SyncActor*>(this)));
															#line 4531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		if (static_cast<SyncActor*>(this)->actor_wait_state > 0) static_cast<SyncActor*>(this)->actor_wait_state = 0;
		static_cast<SyncActor*>(this)->ActorCallback< SyncActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SyncActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SyncActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SyncActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<SyncActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SyncActorState(); static_cast<SyncActor*>(this)->destroy(); return 0; }
															#line 4603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<SyncActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SyncActorState();
		static_cast<SyncActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<SyncActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SyncActorState(); static_cast<SyncActor*>(this)->destroy(); return 0; }
															#line 4615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<SyncActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SyncActorState();
		static_cast<SyncActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<SyncActor*>(this)->actor_wait_state > 0) static_cast<SyncActor*>(this)->actor_wait_state = 0;
		static_cast<SyncActor*>(this)->ActorCallback< SyncActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SyncActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< SyncActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< SyncActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(err, loopDepth);
															#line 4690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(err, loopDepth);
															#line 4698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"

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
	void a_exitChoose4() 
	{
		if (static_cast<SyncActor*>(this)->actor_wait_state > 0) static_cast<SyncActor*>(this)->actor_wait_state = 0;
		static_cast<SyncActor*>(this)->ActorCallback< SyncActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SyncActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< SyncActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< SyncActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable* self;
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	bool durable;
															#line 676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	ISimulator::ProcessInfo* currentProcess;
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	TaskPriority currentTaskID;
															#line 686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	Error err;
															#line 4775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via sync()
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class SyncActor final : public Actor<Void>, public ActorCallback< SyncActor, 0, Void >, public ActorCallback< SyncActor, 1, Void >, public ActorCallback< SyncActor, 2, Void >, public ActorCallback< SyncActor, 3, Void >, public FastAllocated<SyncActor>, public SyncActorState<SyncActor> {
															#line 4780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<SyncActor>::operator new;
	using FastAllocated<SyncActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SyncActor, 0, Void >;
friend struct ActorCallback< SyncActor, 1, Void >;
friend struct ActorCallback< SyncActor, 2, Void >;
friend struct ActorCallback< SyncActor, 3, Void >;
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	SyncActor(AsyncFileNonDurable* const& self,bool const& durable) 
															#line 4794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<Void>(),
		   SyncActorState<SyncActor>(self, durable)
	{
		fdb_probe_actor_enter("sync", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("sync");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("sync", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SyncActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SyncActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< SyncActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< SyncActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] Future<Void> sync( AsyncFileNonDurable* const& self, bool const& durable ) {
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<Void>(new SyncActor(self, durable));
															#line 4824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"

	// Passes along size requests to the underlying file, augmenting with any writes past the end of the file
																#line 4830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via onSize()
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
template <class OnSizeActor>
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class OnSizeActorState {
															#line 4836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	OnSizeActorState(AsyncFileNonDurable const* const& self) 
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self)
															#line 4843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("onSize", reinterpret_cast<unsigned long>(this));

	}
	~OnSizeActorState() 
	{
		fdb_probe_actor_destroy("onSize", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = checkKilled(self, "Size");
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<OnSizeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<OnSizeActor*>(this)->actor_wait_state = 1;
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnSizeActor, 0, Void >*>(static_cast<OnSizeActor*>(this)));
															#line 4865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~OnSizeActorState();
		static_cast<OnSizeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		sizeFuture = self->file->size();
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_1 = success(sizeFuture) || self->killed.getFuture();
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnSizeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnSizeActor*>(this)->actor_wait_state = 2;
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnSizeActor, 1, Void >*>(static_cast<OnSizeActor*>(this)));
															#line 4897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		sizeFuture = self->file->size();
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_1 = success(sizeFuture) || self->killed.getFuture();
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnSizeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<OnSizeActor*>(this)->actor_wait_state = 2;
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< OnSizeActor, 1, Void >*>(static_cast<OnSizeActor*>(this)));
															#line 4915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		if (static_cast<OnSizeActor*>(this)->actor_wait_state > 0) static_cast<OnSizeActor*>(this)->actor_wait_state = 0;
		static_cast<OnSizeActor*>(this)->ActorCallback< OnSizeActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSizeActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OnSizeActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OnSizeActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_2 = checkKilled(self, "SizeEnd");
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnSizeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<OnSizeActor*>(this)->actor_wait_state = 3;
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OnSizeActor, 2, Void >*>(static_cast<OnSizeActor*>(this)));
															#line 4994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_2 = checkKilled(self, "SizeEnd");
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<OnSizeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<OnSizeActor*>(this)->actor_wait_state = 3;
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< OnSizeActor, 2, Void >*>(static_cast<OnSizeActor*>(this)));
															#line 5010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		if (static_cast<OnSizeActor*>(this)->actor_wait_state > 0) static_cast<OnSizeActor*>(this)->actor_wait_state = 0;
		static_cast<OnSizeActor*>(this)->ActorCallback< OnSizeActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSizeActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< OnSizeActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< OnSizeActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->approximateSize = self->minSizeAfterPendingModifications = std::max<int64_t>(sizeFuture.get(), self->minSizeAfterPendingModifications);
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->minSizeAfterPendingModificationsIsExact = true;
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<OnSizeActor*>(this)->SAV<int64_t>::futures) { (void)(self->approximateSize); this->~OnSizeActorState(); static_cast<OnSizeActor*>(this)->destroy(); return 0; }
															#line 5086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<OnSizeActor*>(this)->SAV< int64_t >::value()) int64_t(self->approximateSize);
		this->~OnSizeActorState();
		static_cast<OnSizeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->approximateSize = self->minSizeAfterPendingModifications = std::max<int64_t>(sizeFuture.get(), self->minSizeAfterPendingModifications);
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		self->minSizeAfterPendingModificationsIsExact = true;
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<OnSizeActor*>(this)->SAV<int64_t>::futures) { (void)(self->approximateSize); this->~OnSizeActorState(); static_cast<OnSizeActor*>(this)->destroy(); return 0; }
															#line 5102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<OnSizeActor*>(this)->SAV< int64_t >::value()) int64_t(self->approximateSize);
		this->~OnSizeActorState();
		static_cast<OnSizeActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<OnSizeActor*>(this)->actor_wait_state > 0) static_cast<OnSizeActor*>(this)->actor_wait_state = 0;
		static_cast<OnSizeActor*>(this)->ActorCallback< OnSizeActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnSizeActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< OnSizeActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< OnSizeActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable const* self;
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	Future<int64_t> sizeFuture;
															#line 5177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via onSize()
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class OnSizeActor final : public Actor<int64_t>, public ActorCallback< OnSizeActor, 0, Void >, public ActorCallback< OnSizeActor, 1, Void >, public ActorCallback< OnSizeActor, 2, Void >, public FastAllocated<OnSizeActor>, public OnSizeActorState<OnSizeActor> {
															#line 5182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<OnSizeActor>::operator new;
	using FastAllocated<OnSizeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int64_t>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnSizeActor, 0, Void >;
friend struct ActorCallback< OnSizeActor, 1, Void >;
friend struct ActorCallback< OnSizeActor, 2, Void >;
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	OnSizeActor(AsyncFileNonDurable const* const& self) 
															#line 5195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<int64_t>(),
		   OnSizeActorState<OnSizeActor>(self)
	{
		fdb_probe_actor_enter("onSize", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("onSize");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("onSize", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OnSizeActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< OnSizeActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< OnSizeActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] static Future<int64_t> onSize( AsyncFileNonDurable const* const& self ) {
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<int64_t>(new OnSizeActor(self));
															#line 5224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"

																#line 5229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
// This generated class is to be used only via size()
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
template <class SizeActor>
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class SizeActorState {
															#line 5235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	SizeActorState(AsyncFileNonDurable const* const& self) 
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		 : self(self),
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentProcess(g_simulator->getCurrentProcess()),
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		   currentTaskID(g_network->getCurrentTask())
															#line 5246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
	{
		fdb_probe_actor_create("size", reinterpret_cast<unsigned long>(this));

	}
	~SizeActorState() 
	{
		fdb_probe_actor_destroy("size", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_0 = g_simulator->onMachine(currentProcess);
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SizeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SizeActor*>(this)->actor_wait_state = 1;
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SizeActor, 0, Void >*>(static_cast<SizeActor*>(this)));
															#line 5268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
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
		this->~SizeActorState();
		static_cast<SizeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
		try {
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<int64_t> __when_expr_1 = onSize(self);
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SizeActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 5294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<SizeActor*>(this)->actor_wait_state = 2;
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SizeActor, 1, int64_t >*>(static_cast<SizeActor*>(this)));
															#line 5299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
		try {
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<int64_t> __when_expr_1 = onSize(self);
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SizeActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 5317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<SizeActor*>(this)->actor_wait_state = 2;
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SizeActor, 1, int64_t >*>(static_cast<SizeActor*>(this)));
															#line 5322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

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
		if (static_cast<SizeActor*>(this)->actor_wait_state > 0) static_cast<SizeActor*>(this)->actor_wait_state = 0;
		static_cast<SizeActor*>(this)->ActorCallback< SizeActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SizeActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SizeActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SizeActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			err = e;
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			StrictFuture<Void> __when_expr_3 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			if (static_cast<SizeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<SizeActor*>(this)->actor_wait_state = 4;
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< SizeActor, 3, Void >*>(static_cast<SizeActor*>(this)));
															#line 5410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(int loopDepth) 
	{
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		StrictFuture<Void> __when_expr_2 = g_simulator->onProcess(currentProcess, currentTaskID);
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (static_cast<SizeActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), loopDepth);
															#line 5427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<SizeActor*>(this)->actor_wait_state = 3;
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SizeActor, 2, Void >*>(static_cast<SizeActor*>(this)));
															#line 5432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(int64_t const& __rep,int loopDepth) 
	{
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		rep = __rep;
															#line 5441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(int64_t && __rep,int loopDepth) 
	{
		rep = std::move(__rep);
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<SizeActor*>(this)->actor_wait_state > 0) static_cast<SizeActor*>(this)->actor_wait_state = 0;
		static_cast<SizeActor*>(this)->ActorCallback< SizeActor, 1, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< SizeActor, 1, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SizeActor, 1, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SizeActor, 1, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<SizeActor*>(this)->SAV<int64_t>::futures) { (void)(rep); this->~SizeActorState(); static_cast<SizeActor*>(this)->destroy(); return 0; }
															#line 5508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<SizeActor*>(this)->SAV< int64_t >::value()) int64_t(std::move(rep)); // state_var_RVO
		this->~SizeActorState();
		static_cast<SizeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		if (!static_cast<SizeActor*>(this)->SAV<int64_t>::futures) { (void)(rep); this->~SizeActorState(); static_cast<SizeActor*>(this)->destroy(); return 0; }
															#line 5520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		new (&static_cast<SizeActor*>(this)->SAV< int64_t >::value()) int64_t(std::move(rep)); // state_var_RVO
		this->~SizeActorState();
		static_cast<SizeActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<SizeActor*>(this)->actor_wait_state > 0) static_cast<SizeActor*>(this)->actor_wait_state = 0;
		static_cast<SizeActor*>(this)->ActorCallback< SizeActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SizeActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< SizeActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< SizeActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(err, loopDepth);
															#line 5595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"

		return loopDepth;
	}
	int a_body1cont1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
		return a_body1Catch1(err, loopDepth);
															#line 5603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"

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
	void a_exitChoose4() 
	{
		if (static_cast<SizeActor*>(this)->actor_wait_state > 0) static_cast<SizeActor*>(this)->actor_wait_state = 0;
		static_cast<SizeActor*>(this)->ActorCallback< SizeActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SizeActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< SizeActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< SizeActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	AsyncFileNonDurable const* self;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	ISimulator::ProcessInfo* currentProcess;
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	TaskPriority currentTaskID;
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	int64_t rep;
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	Error err;
															#line 5680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
};
// This generated class is to be used only via size()
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
class SizeActor final : public Actor<int64_t>, public ActorCallback< SizeActor, 0, Void >, public ActorCallback< SizeActor, 1, int64_t >, public ActorCallback< SizeActor, 2, Void >, public ActorCallback< SizeActor, 3, Void >, public FastAllocated<SizeActor>, public SizeActorState<SizeActor> {
															#line 5685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
public:
	using FastAllocated<SizeActor>::operator new;
	using FastAllocated<SizeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int64_t>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SizeActor, 0, Void >;
friend struct ActorCallback< SizeActor, 1, int64_t >;
friend struct ActorCallback< SizeActor, 2, Void >;
friend struct ActorCallback< SizeActor, 3, Void >;
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	SizeActor(AsyncFileNonDurable const* const& self) 
															#line 5699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
		 : Actor<int64_t>(),
		   SizeActorState<SizeActor>(self)
	{
		fdb_probe_actor_enter("size", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("size");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("size", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SizeActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SizeActor, 1, int64_t >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< SizeActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< SizeActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
[[nodiscard]] static Future<int64_t> size( AsyncFileNonDurable const* const& self ) {
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
	return Future<int64_t>(new SizeActor(self));
															#line 5729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
}

#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"

	// Finishes all outstanding actors on an AsyncFileNonDurable and then deletes it
																#line 5735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.g.h"
[[nodiscard]] Future<Void> closeFile( AsyncFileNonDurable* const& self );
template <class> friend class AsyncFileNonDurable_CloseFileActorState;

#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/AsyncFileNonDurable.actor.h"
};

#include "flow/unactorcompiler.h"
#endif
