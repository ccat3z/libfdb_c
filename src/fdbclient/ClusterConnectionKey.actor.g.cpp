#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
/*
 * ClusterConnectionKey.actor.cpp
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

#include "fdbclient/ClusterConnectionKey.actor.h"
#include "flow/actorcompiler.h" // has to be last include

// Creates a cluster connection record with a given connection string and saves it to the specified key. Needs to be
// persisted should be set to true unless this ClusterConnectionKey is being created with the value read from the
// key.
ClusterConnectionKey::ClusterConnectionKey(Database db,
                                           Key connectionStringKey,
                                           ClusterConnectionString const& contents,
                                           ConnectionStringNeedsPersisted needsToBePersisted)
  : IClusterConnectionRecord(needsToBePersisted), db(db), connectionStringKey(connectionStringKey) {
	if (!needsToBePersisted) {
		lastPersistedConnectionString = ValueRef(contents.toString());
	}
	cs = contents;
}

// Loads and parses the connection string at the specified key, throwing errors if the file cannot be read or the
// format is invalid.
															#line 42 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
// This generated class is to be used only via loadClusterConnectionKey()
															#line 40 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
template <class ClusterConnectionKey_LoadClusterConnectionKeyActor>
															#line 40 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
class ClusterConnectionKey_LoadClusterConnectionKeyActorState {
															#line 48 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
public:
															#line 40 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	ClusterConnectionKey_LoadClusterConnectionKeyActorState(Database const& db,Key const& connectionStringKey) 
															#line 40 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
															#line 40 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		 : db(db),
															#line 40 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		   connectionStringKey(connectionStringKey),
															#line 42 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		   tr(db)
															#line 59 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
	{
		fdb_probe_actor_create("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this));

	}
	~ClusterConnectionKey_LoadClusterConnectionKeyActorState() 
	{
		fdb_probe_actor_destroy("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 43 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			;
															#line 74 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
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
		this->~ClusterConnectionKey_LoadClusterConnectionKeyActorState();
		static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 45 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(connectionStringKey);
															#line 45 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			if (static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 107 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->actor_wait_state = 1;
															#line 45 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 0, Optional<Value> >*>(static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)));
															#line 112 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
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
															#line 54 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.onError(e);
															#line 54 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			if (static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 136 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->actor_wait_state = 2;
															#line 54 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 1, Void >*>(static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)));
															#line 141 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& v,int loopDepth) 
	{
															#line 46 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		if (!v.present())
															#line 156 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		{
															#line 47 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			return a_body1loopBody1Catch1(connection_string_invalid(), loopDepth);
															#line 160 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		}
															#line 49 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		if (!static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->SAV<Reference<ClusterConnectionKey>>::futures) { (void)(makeReference<ClusterConnectionKey>(db, connectionStringKey, ClusterConnectionString(v.get().toString()), ConnectionStringNeedsPersisted::False)); this->~ClusterConnectionKey_LoadClusterConnectionKeyActorState(); static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->destroy(); return 0; }
															#line 164 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		new (&static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->SAV< Reference<ClusterConnectionKey> >::value()) Reference<ClusterConnectionKey>(makeReference<ClusterConnectionKey>(db, connectionStringKey, ClusterConnectionString(v.get().toString()), ConnectionStringNeedsPersisted::False));
		this->~ClusterConnectionKey_LoadClusterConnectionKeyActorState();
		static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && v,int loopDepth) 
	{
															#line 46 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		if (!v.present())
															#line 176 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		{
															#line 47 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			return a_body1loopBody1Catch1(connection_string_invalid(), loopDepth);
															#line 180 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		}
															#line 49 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		if (!static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->SAV<Reference<ClusterConnectionKey>>::futures) { (void)(makeReference<ClusterConnectionKey>(db, connectionStringKey, ClusterConnectionString(v.get().toString()), ConnectionStringNeedsPersisted::False)); this->~ClusterConnectionKey_LoadClusterConnectionKeyActorState(); static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->destroy(); return 0; }
															#line 184 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		new (&static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->SAV< Reference<ClusterConnectionKey> >::value()) Reference<ClusterConnectionKey>(makeReference<ClusterConnectionKey>(db, connectionStringKey, ClusterConnectionString(v.get().toString()), ConnectionStringNeedsPersisted::False));
		this->~ClusterConnectionKey_LoadClusterConnectionKeyActorState();
		static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& v,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(v, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && v,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(v), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->actor_wait_state > 0) static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->actor_wait_state = 0;
		static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this), 0);

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
	void a_exitChoose2() 
	{
		if (static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->actor_wait_state > 0) static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->actor_wait_state = 0;
		static_cast<ClusterConnectionKey_LoadClusterConnectionKeyActor*>(this)->ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 40 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	Database db;
															#line 40 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	Key connectionStringKey;
															#line 42 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	Transaction tr;
															#line 336 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
};
// This generated class is to be used only via loadClusterConnectionKey()
															#line 40 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
class ClusterConnectionKey_LoadClusterConnectionKeyActor final : public Actor<Reference<ClusterConnectionKey>>, public ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 0, Optional<Value> >, public ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 1, Void >, public FastAllocated<ClusterConnectionKey_LoadClusterConnectionKeyActor>, public ClusterConnectionKey_LoadClusterConnectionKeyActorState<ClusterConnectionKey_LoadClusterConnectionKeyActor> {
															#line 341 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
public:
	using FastAllocated<ClusterConnectionKey_LoadClusterConnectionKeyActor>::operator new;
	using FastAllocated<ClusterConnectionKey_LoadClusterConnectionKeyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<ClusterConnectionKey>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 0, Optional<Value> >;
friend struct ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 1, Void >;
															#line 40 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	ClusterConnectionKey_LoadClusterConnectionKeyActor(Database const& db,Key const& connectionStringKey) 
															#line 353 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		 : Actor<Reference<ClusterConnectionKey>>(),
		   ClusterConnectionKey_LoadClusterConnectionKeyActorState<ClusterConnectionKey_LoadClusterConnectionKeyActor>(db, connectionStringKey)
	{
		fdb_probe_actor_enter("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("loadClusterConnectionKey");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("loadClusterConnectionKey", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ClusterConnectionKey_LoadClusterConnectionKeyActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 40 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
[[nodiscard]] Future<Reference<ClusterConnectionKey>> ClusterConnectionKey::loadClusterConnectionKey( Database const& db, Key const& connectionStringKey ) {
															#line 40 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	return Future<Reference<ClusterConnectionKey>>(new ClusterConnectionKey_LoadClusterConnectionKeyActor(db, connectionStringKey));
															#line 381 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
}

#line 58 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"

// Sets the connections string held by this object and persists it.
Future<Void> ClusterConnectionKey::setAndPersistConnectionString(ClusterConnectionString const& connectionString) {
	cs = connectionString;
	return success(persist());
}

// Get the connection string stored in the database.
															#line 393 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
// This generated class is to be used only via getStoredConnectionStringImpl()
															#line 66 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
template <class ClusterConnectionKey_GetStoredConnectionStringImplActor>
															#line 66 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
class ClusterConnectionKey_GetStoredConnectionStringImplActorState {
															#line 399 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
public:
															#line 66 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	ClusterConnectionKey_GetStoredConnectionStringImplActorState(Reference<ClusterConnectionKey> const& self) 
															#line 66 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
															#line 66 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		 : self(self)
															#line 406 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
	{
		fdb_probe_actor_create("getStoredConnectionStringImpl", reinterpret_cast<unsigned long>(this));

	}
	~ClusterConnectionKey_GetStoredConnectionStringImplActorState() 
	{
		fdb_probe_actor_destroy("getStoredConnectionStringImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 68 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			StrictFuture<Reference<ClusterConnectionKey>> __when_expr_0 = ClusterConnectionKey::loadClusterConnectionKey(self->db, self->connectionStringKey);
															#line 68 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			if (static_cast<ClusterConnectionKey_GetStoredConnectionStringImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 423 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ClusterConnectionKey_GetStoredConnectionStringImplActor*>(this)->actor_wait_state = 1;
															#line 68 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ClusterConnectionKey_GetStoredConnectionStringImplActor, 0, Reference<ClusterConnectionKey> >*>(static_cast<ClusterConnectionKey_GetStoredConnectionStringImplActor*>(this)));
															#line 428 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
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
		this->~ClusterConnectionKey_GetStoredConnectionStringImplActorState();
		static_cast<ClusterConnectionKey_GetStoredConnectionStringImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<ClusterConnectionKey> const& cck,int loopDepth) 
	{
															#line 70 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		if (!static_cast<ClusterConnectionKey_GetStoredConnectionStringImplActor*>(this)->SAV<ClusterConnectionString>::futures) { (void)(cck->cs); this->~ClusterConnectionKey_GetStoredConnectionStringImplActorState(); static_cast<ClusterConnectionKey_GetStoredConnectionStringImplActor*>(this)->destroy(); return 0; }
															#line 451 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		new (&static_cast<ClusterConnectionKey_GetStoredConnectionStringImplActor*>(this)->SAV< ClusterConnectionString >::value()) ClusterConnectionString(cck->cs);
		this->~ClusterConnectionKey_GetStoredConnectionStringImplActorState();
		static_cast<ClusterConnectionKey_GetStoredConnectionStringImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Reference<ClusterConnectionKey> && cck,int loopDepth) 
	{
															#line 70 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		if (!static_cast<ClusterConnectionKey_GetStoredConnectionStringImplActor*>(this)->SAV<ClusterConnectionString>::futures) { (void)(cck->cs); this->~ClusterConnectionKey_GetStoredConnectionStringImplActorState(); static_cast<ClusterConnectionKey_GetStoredConnectionStringImplActor*>(this)->destroy(); return 0; }
															#line 463 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		new (&static_cast<ClusterConnectionKey_GetStoredConnectionStringImplActor*>(this)->SAV< ClusterConnectionString >::value()) ClusterConnectionString(cck->cs);
		this->~ClusterConnectionKey_GetStoredConnectionStringImplActorState();
		static_cast<ClusterConnectionKey_GetStoredConnectionStringImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Reference<ClusterConnectionKey> const& cck,int loopDepth) 
	{
		loopDepth = a_body1cont1(cck, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<ClusterConnectionKey> && cck,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(cck), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ClusterConnectionKey_GetStoredConnectionStringImplActor*>(this)->actor_wait_state > 0) static_cast<ClusterConnectionKey_GetStoredConnectionStringImplActor*>(this)->actor_wait_state = 0;
		static_cast<ClusterConnectionKey_GetStoredConnectionStringImplActor*>(this)->ActorCallback< ClusterConnectionKey_GetStoredConnectionStringImplActor, 0, Reference<ClusterConnectionKey> >::remove();

	}
	void a_callback_fire(ActorCallback< ClusterConnectionKey_GetStoredConnectionStringImplActor, 0, Reference<ClusterConnectionKey> >*,Reference<ClusterConnectionKey> const& value) 
	{
		fdb_probe_actor_enter("getStoredConnectionStringImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getStoredConnectionStringImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ClusterConnectionKey_GetStoredConnectionStringImplActor, 0, Reference<ClusterConnectionKey> >*,Reference<ClusterConnectionKey> && value) 
	{
		fdb_probe_actor_enter("getStoredConnectionStringImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getStoredConnectionStringImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ClusterConnectionKey_GetStoredConnectionStringImplActor, 0, Reference<ClusterConnectionKey> >*,Error err) 
	{
		fdb_probe_actor_enter("getStoredConnectionStringImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getStoredConnectionStringImpl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 66 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	Reference<ClusterConnectionKey> self;
															#line 536 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
};
// This generated class is to be used only via getStoredConnectionStringImpl()
															#line 66 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
class ClusterConnectionKey_GetStoredConnectionStringImplActor final : public Actor<ClusterConnectionString>, public ActorCallback< ClusterConnectionKey_GetStoredConnectionStringImplActor, 0, Reference<ClusterConnectionKey> >, public FastAllocated<ClusterConnectionKey_GetStoredConnectionStringImplActor>, public ClusterConnectionKey_GetStoredConnectionStringImplActorState<ClusterConnectionKey_GetStoredConnectionStringImplActor> {
															#line 541 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
public:
	using FastAllocated<ClusterConnectionKey_GetStoredConnectionStringImplActor>::operator new;
	using FastAllocated<ClusterConnectionKey_GetStoredConnectionStringImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<ClusterConnectionString>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ClusterConnectionKey_GetStoredConnectionStringImplActor, 0, Reference<ClusterConnectionKey> >;
															#line 66 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	ClusterConnectionKey_GetStoredConnectionStringImplActor(Reference<ClusterConnectionKey> const& self) 
															#line 552 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		 : Actor<ClusterConnectionString>(),
		   ClusterConnectionKey_GetStoredConnectionStringImplActorState<ClusterConnectionKey_GetStoredConnectionStringImplActor>(self)
	{
		fdb_probe_actor_enter("getStoredConnectionStringImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getStoredConnectionStringImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getStoredConnectionStringImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ClusterConnectionKey_GetStoredConnectionStringImplActor, 0, Reference<ClusterConnectionKey> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 66 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
[[nodiscard]] Future<ClusterConnectionString> ClusterConnectionKey::getStoredConnectionStringImpl( Reference<ClusterConnectionKey> const& self ) {
															#line 66 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	return Future<ClusterConnectionString>(new ClusterConnectionKey_GetStoredConnectionStringImplActor(self));
															#line 579 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
}

#line 72 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"

Future<ClusterConnectionString> ClusterConnectionKey::getStoredConnectionString() {
	return getStoredConnectionStringImpl(Reference<ClusterConnectionKey>::addRef(this));
}

															#line 588 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
// This generated class is to be used only via upToDateImpl()
															#line 77 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
template <class ClusterConnectionKey_UpToDateImplActor>
															#line 77 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
class ClusterConnectionKey_UpToDateImplActorState {
															#line 594 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
public:
															#line 77 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	ClusterConnectionKey_UpToDateImplActorState(Reference<ClusterConnectionKey> const& self,ClusterConnectionString* const& connectionString) 
															#line 77 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
															#line 77 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		 : self(self),
															#line 77 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		   connectionString(connectionString)
															#line 603 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
	{
		fdb_probe_actor_create("upToDateImpl", reinterpret_cast<unsigned long>(this));

	}
	~ClusterConnectionKey_UpToDateImplActorState() 
	{
		fdb_probe_actor_destroy("upToDateImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 81 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
				if (self->needsToBePersisted())
															#line 619 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
				{
															#line 82 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
					if (!static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ClusterConnectionKey_UpToDateImplActorState(); static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->destroy(); return 0; }
															#line 623 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
					new (&static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->SAV< bool >::value()) bool(true);
					this->~ClusterConnectionKey_UpToDateImplActorState();
					static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 84 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
				StrictFuture<Reference<ClusterConnectionKey>> __when_expr_0 = ClusterConnectionKey::loadClusterConnectionKey(self->db, self->connectionStringKey);
															#line 84 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
				if (static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 633 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->actor_wait_state = 1;
															#line 84 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ClusterConnectionKey_UpToDateImplActor, 0, Reference<ClusterConnectionKey> >*>(static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)));
															#line 638 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
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
		this->~ClusterConnectionKey_UpToDateImplActorState();
		static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 89 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			TraceEvent(SevWarnAlways, "ClusterKeyError").error(e).detail("Key", self->connectionStringKey);
															#line 90 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			if (!static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->SAV<bool>::futures) { (void)(false); this->~ClusterConnectionKey_UpToDateImplActorState(); static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->destroy(); return 0; }
															#line 670 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
			new (&static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->SAV< bool >::value()) bool(false);
			this->~ClusterConnectionKey_UpToDateImplActorState();
			static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Reference<ClusterConnectionKey> const& temp,int loopDepth) 
	{
															#line 86 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		*connectionString = temp->getConnectionString();
															#line 87 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		if (!static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->SAV<bool>::futures) { (void)(connectionString->toString() == self->cs.toString()); this->~ClusterConnectionKey_UpToDateImplActorState(); static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->destroy(); return 0; }
															#line 690 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		new (&static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->SAV< bool >::value()) bool(connectionString->toString() == self->cs.toString());
		this->~ClusterConnectionKey_UpToDateImplActorState();
		static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Reference<ClusterConnectionKey> && temp,int loopDepth) 
	{
															#line 86 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		*connectionString = temp->getConnectionString();
															#line 87 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		if (!static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->SAV<bool>::futures) { (void)(connectionString->toString() == self->cs.toString()); this->~ClusterConnectionKey_UpToDateImplActorState(); static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->destroy(); return 0; }
															#line 704 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		new (&static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->SAV< bool >::value()) bool(connectionString->toString() == self->cs.toString());
		this->~ClusterConnectionKey_UpToDateImplActorState();
		static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Reference<ClusterConnectionKey> const& temp,int loopDepth) 
	{
		loopDepth = a_body1cont2(temp, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<ClusterConnectionKey> && temp,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(temp), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->actor_wait_state > 0) static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->actor_wait_state = 0;
		static_cast<ClusterConnectionKey_UpToDateImplActor*>(this)->ActorCallback< ClusterConnectionKey_UpToDateImplActor, 0, Reference<ClusterConnectionKey> >::remove();

	}
	void a_callback_fire(ActorCallback< ClusterConnectionKey_UpToDateImplActor, 0, Reference<ClusterConnectionKey> >*,Reference<ClusterConnectionKey> const& value) 
	{
		fdb_probe_actor_enter("upToDateImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("upToDateImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ClusterConnectionKey_UpToDateImplActor, 0, Reference<ClusterConnectionKey> >*,Reference<ClusterConnectionKey> && value) 
	{
		fdb_probe_actor_enter("upToDateImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("upToDateImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ClusterConnectionKey_UpToDateImplActor, 0, Reference<ClusterConnectionKey> >*,Error err) 
	{
		fdb_probe_actor_enter("upToDateImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("upToDateImpl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 77 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	Reference<ClusterConnectionKey> self;
															#line 77 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	ClusterConnectionString* connectionString;
															#line 779 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
};
// This generated class is to be used only via upToDateImpl()
															#line 77 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
class ClusterConnectionKey_UpToDateImplActor final : public Actor<bool>, public ActorCallback< ClusterConnectionKey_UpToDateImplActor, 0, Reference<ClusterConnectionKey> >, public FastAllocated<ClusterConnectionKey_UpToDateImplActor>, public ClusterConnectionKey_UpToDateImplActorState<ClusterConnectionKey_UpToDateImplActor> {
															#line 784 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
public:
	using FastAllocated<ClusterConnectionKey_UpToDateImplActor>::operator new;
	using FastAllocated<ClusterConnectionKey_UpToDateImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ClusterConnectionKey_UpToDateImplActor, 0, Reference<ClusterConnectionKey> >;
															#line 77 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	ClusterConnectionKey_UpToDateImplActor(Reference<ClusterConnectionKey> const& self,ClusterConnectionString* const& connectionString) 
															#line 795 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		 : Actor<bool>(),
		   ClusterConnectionKey_UpToDateImplActorState<ClusterConnectionKey_UpToDateImplActor>(self, connectionString)
	{
		fdb_probe_actor_enter("upToDateImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("upToDateImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("upToDateImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ClusterConnectionKey_UpToDateImplActor, 0, Reference<ClusterConnectionKey> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 77 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
[[nodiscard]] Future<bool> ClusterConnectionKey::upToDateImpl( Reference<ClusterConnectionKey> const& self, ClusterConnectionString* const& connectionString ) {
															#line 77 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	return Future<bool>(new ClusterConnectionKey_UpToDateImplActor(self, connectionString));
															#line 822 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
}

#line 93 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"

// Checks whether the connection string in the database matches the connection string stored in memory. The cluster
// string stored in the database is returned via the reference parameter connectionString.
Future<bool> ClusterConnectionKey::upToDate(ClusterConnectionString& connectionString) {
	return upToDateImpl(Reference<ClusterConnectionKey>::addRef(this), &connectionString);
}

// Returns the key where the connection string is stored.
std::string ClusterConnectionKey::getLocation() const {
	return printable(connectionStringKey);
}

// Creates a copy of this object with a modified connection string but that isn't persisted.
Reference<IClusterConnectionRecord> ClusterConnectionKey::makeIntermediateRecord(
    ClusterConnectionString const& connectionString) const {
	return makeReference<ClusterConnectionKey>(db, connectionStringKey, connectionString);
}

// Returns a string representation of this cluster connection record. This will include the type of record and the
// key where the record is stored.
std::string ClusterConnectionKey::toString() const {
	return "fdbkey://" + printable(connectionStringKey);
}

															#line 850 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
// This generated class is to be used only via persistImpl()
															#line 117 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
template <class ClusterConnectionKey_PersistImplActor>
															#line 117 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
class ClusterConnectionKey_PersistImplActorState {
															#line 856 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
public:
															#line 117 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	ClusterConnectionKey_PersistImplActorState(Reference<ClusterConnectionKey> const& self) 
															#line 117 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
															#line 117 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		 : self(self)
															#line 863 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
	{
		fdb_probe_actor_create("persistImpl", reinterpret_cast<unsigned long>(this));

	}
	~ClusterConnectionKey_PersistImplActorState() 
	{
		fdb_probe_actor_destroy("persistImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 118 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			self->setPersisted();
															#line 119 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			newConnectionString = ValueRef(self->cs.toString());
															#line 880 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
			try {
															#line 122 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
				tr = Transaction(self->db);
															#line 123 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
				;
															#line 886 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
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
		this->~ClusterConnectionKey_PersistImplActorState();
		static_cast<ClusterConnectionKey_PersistImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 161 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		if (!static_cast<ClusterConnectionKey_PersistImplActor*>(this)->SAV<bool>::futures) { (void)(false); this->~ClusterConnectionKey_PersistImplActorState(); static_cast<ClusterConnectionKey_PersistImplActor*>(this)->destroy(); return 0; }
															#line 915 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		new (&static_cast<ClusterConnectionKey_PersistImplActor*>(this)->SAV< bool >::value()) bool(false);
		this->~ClusterConnectionKey_PersistImplActorState();
		static_cast<ClusterConnectionKey_PersistImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 155 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			TraceEvent(SevWarnAlways, "UnableToChangeConnectionKey") .error(e) .detail("ConnectionKey", self->connectionStringKey) .detail("ConnectionString", self->cs.toString());
															#line 928 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

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
															#line 125 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(self->connectionStringKey);
															#line 125 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			if (static_cast<ClusterConnectionKey_PersistImplActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 953 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ClusterConnectionKey_PersistImplActor*>(this)->actor_wait_state = 1;
															#line 125 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ClusterConnectionKey_PersistImplActor, 0, Optional<Value> >*>(static_cast<ClusterConnectionKey_PersistImplActor*>(this)));
															#line 958 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
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
															#line 151 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			StrictFuture<Void> __when_expr_2 = tr.onError(e);
															#line 151 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			if (static_cast<ClusterConnectionKey_PersistImplActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 982 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<ClusterConnectionKey_PersistImplActor*>(this)->actor_wait_state = 3;
															#line 151 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ClusterConnectionKey_PersistImplActor, 2, Void >*>(static_cast<ClusterConnectionKey_PersistImplActor*>(this)));
															#line 987 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& existingConnectionString,int loopDepth) 
	{
															#line 127 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		if (existingConnectionString.present() && existingConnectionString.get() == newConnectionString)
															#line 1002 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		{
															#line 128 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			self->lastPersistedConnectionString = newConnectionString;
															#line 129 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			if (!static_cast<ClusterConnectionKey_PersistImplActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ClusterConnectionKey_PersistImplActorState(); static_cast<ClusterConnectionKey_PersistImplActor*>(this)->destroy(); return 0; }
															#line 1008 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
			new (&static_cast<ClusterConnectionKey_PersistImplActor*>(this)->SAV< bool >::value()) bool(true);
			this->~ClusterConnectionKey_PersistImplActorState();
			static_cast<ClusterConnectionKey_PersistImplActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 136 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			if (existingConnectionString.present() && existingConnectionString != self->lastPersistedConnectionString)
															#line 1018 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
			{
															#line 138 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
				TraceEvent(SevWarnAlways, "UnableToChangeConnectionKeyDueToMismatch") .detail("ConnectionKey", self->connectionStringKey) .detail("NewConnectionString", newConnectionString) .detail("ExpectedStoredConnectionString", self->lastPersistedConnectionString) .detail("ActualStoredConnectionString", existingConnectionString);
															#line 143 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
				if (!static_cast<ClusterConnectionKey_PersistImplActor*>(this)->SAV<bool>::futures) { (void)(false); this->~ClusterConnectionKey_PersistImplActorState(); static_cast<ClusterConnectionKey_PersistImplActor*>(this)->destroy(); return 0; }
															#line 1024 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
				new (&static_cast<ClusterConnectionKey_PersistImplActor*>(this)->SAV< bool >::value()) bool(false);
				this->~ClusterConnectionKey_PersistImplActorState();
				static_cast<ClusterConnectionKey_PersistImplActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 145 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		tr.set(self->connectionStringKey, newConnectionString);
															#line 146 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 146 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		if (static_cast<ClusterConnectionKey_PersistImplActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1037 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ClusterConnectionKey_PersistImplActor*>(this)->actor_wait_state = 2;
															#line 146 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ClusterConnectionKey_PersistImplActor, 1, Void >*>(static_cast<ClusterConnectionKey_PersistImplActor*>(this)));
															#line 1042 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && existingConnectionString,int loopDepth) 
	{
															#line 127 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		if (existingConnectionString.present() && existingConnectionString.get() == newConnectionString)
															#line 1051 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		{
															#line 128 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			self->lastPersistedConnectionString = newConnectionString;
															#line 129 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			if (!static_cast<ClusterConnectionKey_PersistImplActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ClusterConnectionKey_PersistImplActorState(); static_cast<ClusterConnectionKey_PersistImplActor*>(this)->destroy(); return 0; }
															#line 1057 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
			new (&static_cast<ClusterConnectionKey_PersistImplActor*>(this)->SAV< bool >::value()) bool(true);
			this->~ClusterConnectionKey_PersistImplActorState();
			static_cast<ClusterConnectionKey_PersistImplActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 136 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
			if (existingConnectionString.present() && existingConnectionString != self->lastPersistedConnectionString)
															#line 1067 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
			{
															#line 138 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
				TraceEvent(SevWarnAlways, "UnableToChangeConnectionKeyDueToMismatch") .detail("ConnectionKey", self->connectionStringKey) .detail("NewConnectionString", newConnectionString) .detail("ExpectedStoredConnectionString", self->lastPersistedConnectionString) .detail("ActualStoredConnectionString", existingConnectionString);
															#line 143 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
				if (!static_cast<ClusterConnectionKey_PersistImplActor*>(this)->SAV<bool>::futures) { (void)(false); this->~ClusterConnectionKey_PersistImplActorState(); static_cast<ClusterConnectionKey_PersistImplActor*>(this)->destroy(); return 0; }
															#line 1073 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
				new (&static_cast<ClusterConnectionKey_PersistImplActor*>(this)->SAV< bool >::value()) bool(false);
				this->~ClusterConnectionKey_PersistImplActorState();
				static_cast<ClusterConnectionKey_PersistImplActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 145 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		tr.set(self->connectionStringKey, newConnectionString);
															#line 146 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 146 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		if (static_cast<ClusterConnectionKey_PersistImplActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1086 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ClusterConnectionKey_PersistImplActor*>(this)->actor_wait_state = 2;
															#line 146 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ClusterConnectionKey_PersistImplActor, 1, Void >*>(static_cast<ClusterConnectionKey_PersistImplActor*>(this)));
															#line 1091 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& existingConnectionString,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(existingConnectionString, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && existingConnectionString,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(existingConnectionString), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ClusterConnectionKey_PersistImplActor*>(this)->actor_wait_state > 0) static_cast<ClusterConnectionKey_PersistImplActor*>(this)->actor_wait_state = 0;
		static_cast<ClusterConnectionKey_PersistImplActor*>(this)->ActorCallback< ClusterConnectionKey_PersistImplActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< ClusterConnectionKey_PersistImplActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("persistImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ClusterConnectionKey_PersistImplActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("persistImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ClusterConnectionKey_PersistImplActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("persistImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 148 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		self->lastPersistedConnectionString = newConnectionString;
															#line 149 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		if (!static_cast<ClusterConnectionKey_PersistImplActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ClusterConnectionKey_PersistImplActorState(); static_cast<ClusterConnectionKey_PersistImplActor*>(this)->destroy(); return 0; }
															#line 1165 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		new (&static_cast<ClusterConnectionKey_PersistImplActor*>(this)->SAV< bool >::value()) bool(true);
		this->~ClusterConnectionKey_PersistImplActorState();
		static_cast<ClusterConnectionKey_PersistImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 148 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		self->lastPersistedConnectionString = newConnectionString;
															#line 149 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
		if (!static_cast<ClusterConnectionKey_PersistImplActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ClusterConnectionKey_PersistImplActorState(); static_cast<ClusterConnectionKey_PersistImplActor*>(this)->destroy(); return 0; }
															#line 1179 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		new (&static_cast<ClusterConnectionKey_PersistImplActor*>(this)->SAV< bool >::value()) bool(true);
		this->~ClusterConnectionKey_PersistImplActorState();
		static_cast<ClusterConnectionKey_PersistImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ClusterConnectionKey_PersistImplActor*>(this)->actor_wait_state > 0) static_cast<ClusterConnectionKey_PersistImplActor*>(this)->actor_wait_state = 0;
		static_cast<ClusterConnectionKey_PersistImplActor*>(this)->ActorCallback< ClusterConnectionKey_PersistImplActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ClusterConnectionKey_PersistImplActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("persistImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ClusterConnectionKey_PersistImplActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("persistImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ClusterConnectionKey_PersistImplActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("persistImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistImpl", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<ClusterConnectionKey_PersistImplActor*>(this)->actor_wait_state > 0) static_cast<ClusterConnectionKey_PersistImplActor*>(this)->actor_wait_state = 0;
		static_cast<ClusterConnectionKey_PersistImplActor*>(this)->ActorCallback< ClusterConnectionKey_PersistImplActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ClusterConnectionKey_PersistImplActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("persistImpl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistImpl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ClusterConnectionKey_PersistImplActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("persistImpl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistImpl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ClusterConnectionKey_PersistImplActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("persistImpl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistImpl", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 117 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	Reference<ClusterConnectionKey> self;
															#line 119 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	Value newConnectionString;
															#line 122 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	Transaction tr;
															#line 1331 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
};
// This generated class is to be used only via persistImpl()
															#line 117 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
class ClusterConnectionKey_PersistImplActor final : public Actor<bool>, public ActorCallback< ClusterConnectionKey_PersistImplActor, 0, Optional<Value> >, public ActorCallback< ClusterConnectionKey_PersistImplActor, 1, Void >, public ActorCallback< ClusterConnectionKey_PersistImplActor, 2, Void >, public FastAllocated<ClusterConnectionKey_PersistImplActor>, public ClusterConnectionKey_PersistImplActorState<ClusterConnectionKey_PersistImplActor> {
															#line 1336 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
public:
	using FastAllocated<ClusterConnectionKey_PersistImplActor>::operator new;
	using FastAllocated<ClusterConnectionKey_PersistImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ClusterConnectionKey_PersistImplActor, 0, Optional<Value> >;
friend struct ActorCallback< ClusterConnectionKey_PersistImplActor, 1, Void >;
friend struct ActorCallback< ClusterConnectionKey_PersistImplActor, 2, Void >;
															#line 117 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	ClusterConnectionKey_PersistImplActor(Reference<ClusterConnectionKey> const& self) 
															#line 1349 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
		 : Actor<bool>(),
		   ClusterConnectionKey_PersistImplActorState<ClusterConnectionKey_PersistImplActor>(self)
	{
		fdb_probe_actor_enter("persistImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("persistImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("persistImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ClusterConnectionKey_PersistImplActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ClusterConnectionKey_PersistImplActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ClusterConnectionKey_PersistImplActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 117 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
[[nodiscard]] Future<bool> ClusterConnectionKey::persistImpl( Reference<ClusterConnectionKey> const& self ) {
															#line 117 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"
	return Future<bool>(new ClusterConnectionKey_PersistImplActor(self));
															#line 1378 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.g.cpp"
}

#line 162 "/usr/src/libfdb_c/fdbclient/ClusterConnectionKey.actor.cpp"


// Writes the connection string to the database
Future<bool> ClusterConnectionKey::persist() {
	return persistImpl(Reference<ClusterConnectionKey>::addRef(this));
}