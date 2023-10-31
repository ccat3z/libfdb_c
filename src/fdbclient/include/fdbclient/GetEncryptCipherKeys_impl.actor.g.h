#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
/*
 * GetEncryptCipherKeys_impl.actor.h
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
#include "flow/EncryptUtils.h"
#include "flow/genericactors.actor.h"
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_GETCIPHERKEYS_IMPL_ACTOR_G_H)
#define FDBCLIENT_GETCIPHERKEYS_IMPL_ACTOR_G_H
#include "fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
#elif !defined(FDBCLIENT_GETCIPHERKEYS_IMPL_ACTOR_H)
#define FDBCLIENT_GETCIPHERKEYS_IMPL_ACTOR_H

#include "fdbclient/CommitProxyInterface.h"
#include "fdbclient/GlobalConfig.actor.h"
#include "fdbclient/SpecialKeySpace.actor.h"
#include "fdbclient/SystemData.h"
#include "fdbclient/Tuple.h"
#include "flow/flow.h"
#include "flow/genericactors.actor.h"

#include "flow/actorcompiler.h" // This must be the last #include.

template <class T>
Optional<EncryptKeyProxyInterface> getEncryptKeyProxyInterface(const Reference<AsyncVar<T> const>& db) {
	if constexpr (std::is_same_v<T, ClientDBInfo>) {
		return db->get().encryptKeyProxy;
	} else {
		return db->get().client.encryptKeyProxy;
	}
}

template <class T>
Optional<UID> getEncryptKeyProxyId(const Reference<AsyncVar<T> const>& db) {
	return getEncryptKeyProxyInterface(db).map(&EncryptKeyProxyInterface::id);
}

															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
namespace {
// This generated class is to be used only via _onEncryptKeyProxyChange()
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T, class _onEncryptKeyProxyChangeActor>
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _onEncryptKeyProxyChangeActorState {
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_onEncryptKeyProxyChangeActorState(Reference<AsyncVar<T> const> const& db) 
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		 : db(db),
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   previousProxyId(getEncryptKeyProxyId(db)),
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   currentProxyId()
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
	{
		fdb_probe_actor_create("_onEncryptKeyProxyChange", reinterpret_cast<unsigned long>(this));

	}
	~_onEncryptKeyProxyChangeActorState() 
	{
		fdb_probe_actor_destroy("_onEncryptKeyProxyChange", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			;
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
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
		this->~_onEncryptKeyProxyChangeActorState();
		static_cast<_onEncryptKeyProxyChangeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		TraceEvent("GetEncryptCipherKeysEncryptKeyProxyChanged") .detail("PreviousProxyId", previousProxyId.orDefault(UID())) .detail("CurrentProxyId", currentProxyId.orDefault(UID()));
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (!static_cast<_onEncryptKeyProxyChangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_onEncryptKeyProxyChangeActorState(); static_cast<_onEncryptKeyProxyChangeActor*>(this)->destroy(); return 0; }
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		new (&static_cast<_onEncryptKeyProxyChangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~_onEncryptKeyProxyChangeActorState();
		static_cast<_onEncryptKeyProxyChangeActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		StrictFuture<Void> __when_expr_0 = db->onChange();
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (static_cast<_onEncryptKeyProxyChangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<_onEncryptKeyProxyChangeActor*>(this)->actor_wait_state = 1;
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< _onEncryptKeyProxyChangeActor, 0, Void >*>(static_cast<_onEncryptKeyProxyChangeActor*>(this)));
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
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
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		currentProxyId = getEncryptKeyProxyId(db);
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (currentProxyId != previousProxyId)
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		currentProxyId = getEncryptKeyProxyId(db);
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (currentProxyId != previousProxyId)
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<_onEncryptKeyProxyChangeActor*>(this)->actor_wait_state > 0) static_cast<_onEncryptKeyProxyChangeActor*>(this)->actor_wait_state = 0;
		static_cast<_onEncryptKeyProxyChangeActor*>(this)->ActorCallback< _onEncryptKeyProxyChangeActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< _onEncryptKeyProxyChangeActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("_onEncryptKeyProxyChange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_onEncryptKeyProxyChange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< _onEncryptKeyProxyChangeActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("_onEncryptKeyProxyChange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_onEncryptKeyProxyChange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< _onEncryptKeyProxyChangeActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("_onEncryptKeyProxyChange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_onEncryptKeyProxyChange", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	Reference<AsyncVar<T> const> db;
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	Optional<UID> previousProxyId;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	Optional<UID> currentProxyId;
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
};
// This generated class is to be used only via _onEncryptKeyProxyChange()
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _onEncryptKeyProxyChangeActor final : public Actor<Void>, public ActorCallback< _onEncryptKeyProxyChangeActor<T>, 0, Void >, public FastAllocated<_onEncryptKeyProxyChangeActor<T>>, public _onEncryptKeyProxyChangeActorState<T, _onEncryptKeyProxyChangeActor<T>> {
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
	using FastAllocated<_onEncryptKeyProxyChangeActor<T>>::operator new;
	using FastAllocated<_onEncryptKeyProxyChangeActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< _onEncryptKeyProxyChangeActor<T>, 0, Void >;
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_onEncryptKeyProxyChangeActor(Reference<AsyncVar<T> const> const& db) 
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		 : Actor<Void>(),
		   _onEncryptKeyProxyChangeActorState<T, _onEncryptKeyProxyChangeActor<T>>(db)
	{
		fdb_probe_actor_enter("_onEncryptKeyProxyChange", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("_onEncryptKeyProxyChange");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("_onEncryptKeyProxyChange", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< _onEncryptKeyProxyChangeActor<T>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
[[nodiscard]] Future<Void> _onEncryptKeyProxyChange( Reference<AsyncVar<T> const> const& db ) {
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	return Future<Void>(new _onEncryptKeyProxyChangeActor<T>(db));
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
}

#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"

															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
namespace {
// This generated class is to be used only via _getUncachedLatestEncryptCipherKeys()
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T, class _getUncachedLatestEncryptCipherKeysActor>
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getUncachedLatestEncryptCipherKeysActorState {
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getUncachedLatestEncryptCipherKeysActorState(Reference<AsyncVar<T> const> const& db,EKPGetLatestBaseCipherKeysRequest const& request,BlobCipherMetrics::UsageType const& usageType) 
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		 : db(db),
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   request(request),
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   usageType(usageType)
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
	{
		fdb_probe_actor_create("_getUncachedLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this));

	}
	~_getUncachedLatestEncryptCipherKeysActorState() 
	{
		fdb_probe_actor_destroy("_getUncachedLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			Optional<EncryptKeyProxyInterface> proxy = getEncryptKeyProxyInterface(db);
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (!proxy.present())
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				TraceEvent("GetLatestEncryptCipherKeysEncryptKeyProxyNotPresent").detail("UsageType", toString(usageType));
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				this->~_getUncachedLatestEncryptCipherKeysActorState();
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
				static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->sendAndDelPromiseRef(Never());
				return 0;
			}
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			request.reply.reset();
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			try {
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				StrictFuture<EKPGetLatestBaseCipherKeysReply> __when_expr_0 = proxy.get().getLatestBaseCipherKeys.getReply(request);
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				if (static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->actor_wait_state = 1;
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< _getUncachedLatestEncryptCipherKeysActor, 0, EKPGetLatestBaseCipherKeysReply >*>(static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)));
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
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
		this->~_getUncachedLatestEncryptCipherKeysActorState();
		static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			TraceEvent("GetLatestEncryptCipherKeysCaughtError").error(e);
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (e.code() == error_code_broken_promise)
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				this->~_getUncachedLatestEncryptCipherKeysActorState();
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
				static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->sendAndDelPromiseRef(Never());
				return 0;
			}
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(EKPGetLatestBaseCipherKeysReply const& reply,int loopDepth) 
	{
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (reply.error.present())
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		{
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			TraceEvent(SevWarn, "GetLatestEncryptCipherKeysRequestFailed").error(reply.error.get());
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			return a_body1Catch2(reply.error.get(), loopDepth);
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		}
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (!static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->SAV<EKPGetLatestBaseCipherKeysReply>::futures) { (void)(reply); this->~_getUncachedLatestEncryptCipherKeysActorState(); static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->destroy(); return 0; }
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		new (&static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->SAV< EKPGetLatestBaseCipherKeysReply >::value()) EKPGetLatestBaseCipherKeysReply(reply);
		this->~_getUncachedLatestEncryptCipherKeysActorState();
		static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(EKPGetLatestBaseCipherKeysReply && reply,int loopDepth) 
	{
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (reply.error.present())
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		{
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			TraceEvent(SevWarn, "GetLatestEncryptCipherKeysRequestFailed").error(reply.error.get());
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			return a_body1Catch2(reply.error.get(), loopDepth);
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		}
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (!static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->SAV<EKPGetLatestBaseCipherKeysReply>::futures) { (void)(reply); this->~_getUncachedLatestEncryptCipherKeysActorState(); static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->destroy(); return 0; }
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		new (&static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->SAV< EKPGetLatestBaseCipherKeysReply >::value()) EKPGetLatestBaseCipherKeysReply(reply);
		this->~_getUncachedLatestEncryptCipherKeysActorState();
		static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(EKPGetLatestBaseCipherKeysReply const& reply,int loopDepth) 
	{
		loopDepth = a_body1cont3(reply, loopDepth);

		return loopDepth;
	}
	int a_body1when1(EKPGetLatestBaseCipherKeysReply && reply,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(reply), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->actor_wait_state > 0) static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->actor_wait_state = 0;
		static_cast<_getUncachedLatestEncryptCipherKeysActor*>(this)->ActorCallback< _getUncachedLatestEncryptCipherKeysActor, 0, EKPGetLatestBaseCipherKeysReply >::remove();

	}
	void a_callback_fire(ActorCallback< _getUncachedLatestEncryptCipherKeysActor, 0, EKPGetLatestBaseCipherKeysReply >*,EKPGetLatestBaseCipherKeysReply const& value) 
	{
		fdb_probe_actor_enter("_getUncachedLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getUncachedLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< _getUncachedLatestEncryptCipherKeysActor, 0, EKPGetLatestBaseCipherKeysReply >*,EKPGetLatestBaseCipherKeysReply && value) 
	{
		fdb_probe_actor_enter("_getUncachedLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getUncachedLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< _getUncachedLatestEncryptCipherKeysActor, 0, EKPGetLatestBaseCipherKeysReply >*,Error err) 
	{
		fdb_probe_actor_enter("_getUncachedLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getUncachedLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	Reference<AsyncVar<T> const> db;
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	EKPGetLatestBaseCipherKeysRequest request;
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	BlobCipherMetrics::UsageType usageType;
															#line 531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
};
// This generated class is to be used only via _getUncachedLatestEncryptCipherKeys()
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getUncachedLatestEncryptCipherKeysActor final : public Actor<EKPGetLatestBaseCipherKeysReply>, public ActorCallback< _getUncachedLatestEncryptCipherKeysActor<T>, 0, EKPGetLatestBaseCipherKeysReply >, public FastAllocated<_getUncachedLatestEncryptCipherKeysActor<T>>, public _getUncachedLatestEncryptCipherKeysActorState<T, _getUncachedLatestEncryptCipherKeysActor<T>> {
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
	using FastAllocated<_getUncachedLatestEncryptCipherKeysActor<T>>::operator new;
	using FastAllocated<_getUncachedLatestEncryptCipherKeysActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<EKPGetLatestBaseCipherKeysReply>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< _getUncachedLatestEncryptCipherKeysActor<T>, 0, EKPGetLatestBaseCipherKeysReply >;
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getUncachedLatestEncryptCipherKeysActor(Reference<AsyncVar<T> const> const& db,EKPGetLatestBaseCipherKeysRequest const& request,BlobCipherMetrics::UsageType const& usageType) 
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		 : Actor<EKPGetLatestBaseCipherKeysReply>(),
		   _getUncachedLatestEncryptCipherKeysActorState<T, _getUncachedLatestEncryptCipherKeysActor<T>>(db, request, usageType)
	{
		fdb_probe_actor_enter("_getUncachedLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("_getUncachedLatestEncryptCipherKeys");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("_getUncachedLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< _getUncachedLatestEncryptCipherKeysActor<T>, 0, EKPGetLatestBaseCipherKeysReply >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
[[nodiscard]] Future<EKPGetLatestBaseCipherKeysReply> _getUncachedLatestEncryptCipherKeys( Reference<AsyncVar<T> const> const& db, EKPGetLatestBaseCipherKeysRequest const& request, BlobCipherMetrics::UsageType const& usageType ) {
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	return Future<EKPGetLatestBaseCipherKeysReply>(new _getUncachedLatestEncryptCipherKeysActor<T>(db, request, usageType));
															#line 579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
}

#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"

															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
namespace {
// This generated class is to be used only via _getLatestEncryptCipherKeys()
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T, class _getLatestEncryptCipherKeysActor>
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getLatestEncryptCipherKeysActorState {
															#line 591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getLatestEncryptCipherKeysActorState(Reference<AsyncVar<T> const> const& db,std::unordered_set<EncryptCipherDomainId> const& domainIds,BlobCipherMetrics::UsageType const& usageType) 
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		 : db(db),
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   domainIds(domainIds),
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   usageType(usageType),
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   cipherKeyCache(BlobCipherKeyCache::getInstance()),
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   cipherKeys(),
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   request()
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
	{
		fdb_probe_actor_create("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this));

	}
	~_getLatestEncryptCipherKeysActorState() 
	{
		fdb_probe_actor_destroy("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (!db.isValid())
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				TraceEvent(SevError, "GetLatestEncryptCipherKeysServerDBInfoNotAvailable");
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				return a_body1Catch1(encrypt_ops_error(), loopDepth);
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			}
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			for( auto& domainId : domainIds ) {
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				Reference<BlobCipherKey> cachedCipherKey = cipherKeyCache->getLatestCipherKey(domainId);
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				if (cachedCipherKey.isValid())
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
				{
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
					cipherKeys[domainId] = cachedCipherKey;
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
				}
				else
				{
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
					request.encryptDomainIds.emplace_back(domainId);
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
				}
			}
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (request.encryptDomainIds.empty())
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				if (!static_cast<_getLatestEncryptCipherKeysActor*>(this)->SAV<std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>>>::futures) { (void)(cipherKeys); this->~_getLatestEncryptCipherKeysActorState(); static_cast<_getLatestEncryptCipherKeysActor*>(this)->destroy(); return 0; }
															#line 656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
				new (&static_cast<_getLatestEncryptCipherKeysActor*>(this)->SAV< std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >::value()) std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>>(std::move(cipherKeys)); // state_var_RVO
				this->~_getLatestEncryptCipherKeysActorState();
				static_cast<_getLatestEncryptCipherKeysActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			startTime = now();
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			;
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
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
		this->~_getLatestEncryptCipherKeysActorState();
		static_cast<_getLatestEncryptCipherKeysActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		double elapsed = now() - startTime;
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		BlobCipherMetrics::getInstance()->getLatestCipherKeysLatency.addMeasurement(elapsed);
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		BlobCipherMetrics::counters(usageType).getLatestCipherKeysLatency.addMeasurement(elapsed);
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (!static_cast<_getLatestEncryptCipherKeysActor*>(this)->SAV<std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>>>::futures) { (void)(cipherKeys); this->~_getLatestEncryptCipherKeysActorState(); static_cast<_getLatestEncryptCipherKeysActor*>(this)->destroy(); return 0; }
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		new (&static_cast<_getLatestEncryptCipherKeysActor*>(this)->SAV< std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >::value()) std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>>(std::move(cipherKeys)); // state_var_RVO
		this->~_getLatestEncryptCipherKeysActorState();
		static_cast<_getLatestEncryptCipherKeysActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		StrictFuture<EKPGetLatestBaseCipherKeysReply> __when_expr_0 = _getUncachedLatestEncryptCipherKeys(db, request, usageType);
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (static_cast<_getLatestEncryptCipherKeysActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		StrictFuture<Void> __when_expr_1 = _onEncryptKeyProxyChange(db);
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<_getLatestEncryptCipherKeysActor*>(this)->actor_wait_state = 1;
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< _getLatestEncryptCipherKeysActor, 0, EKPGetLatestBaseCipherKeysReply >*>(static_cast<_getLatestEncryptCipherKeysActor*>(this)));
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< _getLatestEncryptCipherKeysActor, 1, Void >*>(static_cast<_getLatestEncryptCipherKeysActor*>(this)));
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
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
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(EKPGetLatestBaseCipherKeysReply const& reply,int loopDepth) 
	{
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		for( const EKPBaseCipherDetails& details : reply.baseCipherDetails ) {
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			EncryptCipherDomainId domainId = details.encryptDomainId;
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (domainIds.count(domainId) > 0 && cipherKeys.count(domainId) == 0)
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				Reference<BlobCipherKey> cipherKey = cipherKeyCache->insertCipherKey(domainId, details.baseCipherId, details.baseCipherKey.begin(), details.baseCipherKey.size(), details.baseCipherKCV, details.refreshAt, details.expireAt);
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				ASSERT(cipherKey.isValid());
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				cipherKeys[domainId] = cipherKey;
															#line 767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			}
		}
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		for( auto domainId : request.encryptDomainIds ) {
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (cipherKeys.count(domainId) == 0)
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				TraceEvent(SevWarn, "GetLatestEncryptCipherKeysKeyMissing").detail("DomainId", domainId);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				return a_body1Catch1(encrypt_key_not_found(), std::max(0, loopDepth - 1));
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			}
		}
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1when1(EKPGetLatestBaseCipherKeysReply && reply,int loopDepth) 
	{
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		for( const EKPBaseCipherDetails& details : reply.baseCipherDetails ) {
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			EncryptCipherDomainId domainId = details.encryptDomainId;
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (domainIds.count(domainId) > 0 && cipherKeys.count(domainId) == 0)
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				Reference<BlobCipherKey> cipherKey = cipherKeyCache->insertCipherKey(domainId, details.baseCipherId, details.baseCipherKey.begin(), details.baseCipherKey.size(), details.baseCipherKCV, details.refreshAt, details.expireAt);
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				ASSERT(cipherKey.isValid());
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				cipherKeys[domainId] = cipherKey;
															#line 803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			}
		}
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		for( auto domainId : request.encryptDomainIds ) {
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (cipherKeys.count(domainId) == 0)
															#line 810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				TraceEvent(SevWarn, "GetLatestEncryptCipherKeysKeyMissing").detail("DomainId", domainId);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				return a_body1Catch1(encrypt_key_not_found(), std::max(0, loopDepth - 1));
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			}
		}
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<_getLatestEncryptCipherKeysActor*>(this)->actor_wait_state > 0) static_cast<_getLatestEncryptCipherKeysActor*>(this)->actor_wait_state = 0;
		static_cast<_getLatestEncryptCipherKeysActor*>(this)->ActorCallback< _getLatestEncryptCipherKeysActor, 0, EKPGetLatestBaseCipherKeysReply >::remove();
		static_cast<_getLatestEncryptCipherKeysActor*>(this)->ActorCallback< _getLatestEncryptCipherKeysActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< _getLatestEncryptCipherKeysActor, 0, EKPGetLatestBaseCipherKeysReply >*,EKPGetLatestBaseCipherKeysReply const& value) 
	{
		fdb_probe_actor_enter("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< _getLatestEncryptCipherKeysActor, 0, EKPGetLatestBaseCipherKeysReply >*,EKPGetLatestBaseCipherKeysReply && value) 
	{
		fdb_probe_actor_enter("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< _getLatestEncryptCipherKeysActor, 0, EKPGetLatestBaseCipherKeysReply >*,Error err) 
	{
		fdb_probe_actor_enter("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< _getLatestEncryptCipherKeysActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< _getLatestEncryptCipherKeysActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< _getLatestEncryptCipherKeysActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	Reference<AsyncVar<T> const> db;
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	std::unordered_set<EncryptCipherDomainId> domainIds;
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	BlobCipherMetrics::UsageType usageType;
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	Reference<BlobCipherKeyCache> cipherKeyCache;
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> cipherKeys;
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	EKPGetLatestBaseCipherKeysRequest request;
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	double startTime;
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
};
// This generated class is to be used only via _getLatestEncryptCipherKeys()
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getLatestEncryptCipherKeysActor final : public Actor<std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>>>, public ActorCallback< _getLatestEncryptCipherKeysActor<T>, 0, EKPGetLatestBaseCipherKeysReply >, public ActorCallback< _getLatestEncryptCipherKeysActor<T>, 1, Void >, public FastAllocated<_getLatestEncryptCipherKeysActor<T>>, public _getLatestEncryptCipherKeysActorState<T, _getLatestEncryptCipherKeysActor<T>> {
															#line 953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
	using FastAllocated<_getLatestEncryptCipherKeysActor<T>>::operator new;
	using FastAllocated<_getLatestEncryptCipherKeysActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< _getLatestEncryptCipherKeysActor<T>, 0, EKPGetLatestBaseCipherKeysReply >;
friend struct ActorCallback< _getLatestEncryptCipherKeysActor<T>, 1, Void >;
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getLatestEncryptCipherKeysActor(Reference<AsyncVar<T> const> const& db,std::unordered_set<EncryptCipherDomainId> const& domainIds,BlobCipherMetrics::UsageType const& usageType) 
															#line 965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		 : Actor<std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>>>(),
		   _getLatestEncryptCipherKeysActorState<T, _getLatestEncryptCipherKeysActor<T>>(db, domainIds, usageType)
	{
		fdb_probe_actor_enter("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("_getLatestEncryptCipherKeys");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("_getLatestEncryptCipherKeys", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< _getLatestEncryptCipherKeysActor<T>, 0, EKPGetLatestBaseCipherKeysReply >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
[[nodiscard]] Future<std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>>> _getLatestEncryptCipherKeys( Reference<AsyncVar<T> const> const& db, std::unordered_set<EncryptCipherDomainId> const& domainIds, BlobCipherMetrics::UsageType const& usageType ) {
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	return Future<std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>>>(new _getLatestEncryptCipherKeysActor<T>(db, domainIds, usageType));
															#line 995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
}

#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"

															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
namespace {
// This generated class is to be used only via _getLatestEncryptCipherKey()
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T, class _getLatestEncryptCipherKeyActor>
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getLatestEncryptCipherKeyActorState {
															#line 1007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getLatestEncryptCipherKeyActorState(Reference<AsyncVar<T> const> const& db,EncryptCipherDomainId const& domainId,BlobCipherMetrics::UsageType const& usageType) 
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		 : db(db),
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   domainId(domainId),
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   usageType(usageType)
															#line 1018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
	{
		fdb_probe_actor_create("_getLatestEncryptCipherKey", reinterpret_cast<unsigned long>(this));

	}
	~_getLatestEncryptCipherKeyActorState() 
	{
		fdb_probe_actor_destroy("_getLatestEncryptCipherKey", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			std::unordered_set<EncryptCipherDomainId> domainIds{ domainId };
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			StrictFuture<std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>>> __when_expr_0 = _getLatestEncryptCipherKeys(db, domainIds, usageType);
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (static_cast<_getLatestEncryptCipherKeyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<_getLatestEncryptCipherKeyActor*>(this)->actor_wait_state = 1;
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< _getLatestEncryptCipherKeyActor, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >*>(static_cast<_getLatestEncryptCipherKeyActor*>(this)));
															#line 1042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
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
		this->~_getLatestEncryptCipherKeyActorState();
		static_cast<_getLatestEncryptCipherKeyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> const& cipherKey,int loopDepth) 
	{
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (!static_cast<_getLatestEncryptCipherKeyActor*>(this)->SAV<Reference<BlobCipherKey>>::futures) { (void)(cipherKey.at(domainId)); this->~_getLatestEncryptCipherKeyActorState(); static_cast<_getLatestEncryptCipherKeyActor*>(this)->destroy(); return 0; }
															#line 1065 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		new (&static_cast<_getLatestEncryptCipherKeyActor*>(this)->SAV< Reference<BlobCipherKey> >::value()) Reference<BlobCipherKey>(cipherKey.at(domainId));
		this->~_getLatestEncryptCipherKeyActorState();
		static_cast<_getLatestEncryptCipherKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> && cipherKey,int loopDepth) 
	{
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (!static_cast<_getLatestEncryptCipherKeyActor*>(this)->SAV<Reference<BlobCipherKey>>::futures) { (void)(cipherKey.at(domainId)); this->~_getLatestEncryptCipherKeyActorState(); static_cast<_getLatestEncryptCipherKeyActor*>(this)->destroy(); return 0; }
															#line 1077 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		new (&static_cast<_getLatestEncryptCipherKeyActor*>(this)->SAV< Reference<BlobCipherKey> >::value()) Reference<BlobCipherKey>(cipherKey.at(domainId));
		this->~_getLatestEncryptCipherKeyActorState();
		static_cast<_getLatestEncryptCipherKeyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> const& cipherKey,int loopDepth) 
	{
		loopDepth = a_body1cont1(cipherKey, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> && cipherKey,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(cipherKey), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<_getLatestEncryptCipherKeyActor*>(this)->actor_wait_state > 0) static_cast<_getLatestEncryptCipherKeyActor*>(this)->actor_wait_state = 0;
		static_cast<_getLatestEncryptCipherKeyActor*>(this)->ActorCallback< _getLatestEncryptCipherKeyActor, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >::remove();

	}
	void a_callback_fire(ActorCallback< _getLatestEncryptCipherKeyActor, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >*,std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> const& value) 
	{
		fdb_probe_actor_enter("_getLatestEncryptCipherKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getLatestEncryptCipherKey", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< _getLatestEncryptCipherKeyActor, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >*,std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> && value) 
	{
		fdb_probe_actor_enter("_getLatestEncryptCipherKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getLatestEncryptCipherKey", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< _getLatestEncryptCipherKeyActor, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >*,Error err) 
	{
		fdb_probe_actor_enter("_getLatestEncryptCipherKey", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getLatestEncryptCipherKey", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	Reference<AsyncVar<T> const> db;
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	EncryptCipherDomainId domainId;
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	BlobCipherMetrics::UsageType usageType;
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
};
// This generated class is to be used only via _getLatestEncryptCipherKey()
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getLatestEncryptCipherKeyActor final : public Actor<Reference<BlobCipherKey>>, public ActorCallback< _getLatestEncryptCipherKeyActor<T>, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >, public FastAllocated<_getLatestEncryptCipherKeyActor<T>>, public _getLatestEncryptCipherKeyActorState<T, _getLatestEncryptCipherKeyActor<T>> {
															#line 1161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
	using FastAllocated<_getLatestEncryptCipherKeyActor<T>>::operator new;
	using FastAllocated<_getLatestEncryptCipherKeyActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<BlobCipherKey>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< _getLatestEncryptCipherKeyActor<T>, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >;
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getLatestEncryptCipherKeyActor(Reference<AsyncVar<T> const> const& db,EncryptCipherDomainId const& domainId,BlobCipherMetrics::UsageType const& usageType) 
															#line 1172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		 : Actor<Reference<BlobCipherKey>>(),
		   _getLatestEncryptCipherKeyActorState<T, _getLatestEncryptCipherKeyActor<T>>(db, domainId, usageType)
	{
		fdb_probe_actor_enter("_getLatestEncryptCipherKey", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("_getLatestEncryptCipherKey");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("_getLatestEncryptCipherKey", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< _getLatestEncryptCipherKeyActor<T>, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
[[nodiscard]] Future<Reference<BlobCipherKey>> _getLatestEncryptCipherKey( Reference<AsyncVar<T> const> const& db, EncryptCipherDomainId const& domainId, BlobCipherMetrics::UsageType const& usageType ) {
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	return Future<Reference<BlobCipherKey>>(new _getLatestEncryptCipherKeyActor<T>(db, domainId, usageType));
															#line 1202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
}

#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"

															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
namespace {
// This generated class is to be used only via _getUncachedEncryptCipherKeys()
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T, class _getUncachedEncryptCipherKeysActor>
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getUncachedEncryptCipherKeysActorState {
															#line 1214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getUncachedEncryptCipherKeysActorState(Reference<AsyncVar<T> const> const& db,EKPGetBaseCipherKeysByIdsRequest const& request,BlobCipherMetrics::UsageType const& usageType) 
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		 : db(db),
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   request(request),
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   usageType(usageType)
															#line 1225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
	{
		fdb_probe_actor_create("_getUncachedEncryptCipherKeys", reinterpret_cast<unsigned long>(this));

	}
	~_getUncachedEncryptCipherKeysActorState() 
	{
		fdb_probe_actor_destroy("_getUncachedEncryptCipherKeys", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			Optional<EncryptKeyProxyInterface> proxy = getEncryptKeyProxyInterface(db);
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (!proxy.present())
															#line 1242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				TraceEvent("GetEncryptCipherKeysEncryptKeyProxyNotPresent").detail("UsageType", toString(usageType));
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				this->~_getUncachedEncryptCipherKeysActorState();
															#line 1248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
				static_cast<_getUncachedEncryptCipherKeysActor*>(this)->sendAndDelPromiseRef(Never());
				return 0;
			}
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			request.reply.reset();
															#line 1254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			try {
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				StrictFuture<EKPGetBaseCipherKeysByIdsReply> __when_expr_0 = proxy.get().getBaseCipherKeysByIds.getReply(request);
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				if (static_cast<_getUncachedEncryptCipherKeysActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<_getUncachedEncryptCipherKeysActor*>(this)->actor_wait_state = 1;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< _getUncachedEncryptCipherKeysActor, 0, EKPGetBaseCipherKeysByIdsReply >*>(static_cast<_getUncachedEncryptCipherKeysActor*>(this)));
															#line 1265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
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
		this->~_getUncachedEncryptCipherKeysActorState();
		static_cast<_getUncachedEncryptCipherKeysActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			TraceEvent("GetEncryptCipherKeysCaughtError").error(e);
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (e.code() == error_code_broken_promise)
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				this->~_getUncachedEncryptCipherKeysActorState();
															#line 1301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
				static_cast<_getUncachedEncryptCipherKeysActor*>(this)->sendAndDelPromiseRef(Never());
				return 0;
			}
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			return a_body1Catch1(e, loopDepth);
															#line 1307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(EKPGetBaseCipherKeysByIdsReply const& reply,int loopDepth) 
	{
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (reply.error.present())
															#line 1321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		{
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			TraceEvent(SevWarn, "GetEncryptCipherKeysRequestFailed").error(reply.error.get());
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			return a_body1Catch2(reply.error.get(), loopDepth);
															#line 1327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		}
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (g_network && g_network->isSimulated() && usageType == BlobCipherMetrics::RESTORE)
															#line 1331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		{
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			std::unordered_set<int64_t> tenantIdsToDrop = parseStringToUnorderedSet<int64_t>(CLIENT_KNOBS->SIMULATION_EKP_TENANT_IDS_TO_DROP, ',');
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (!tenantIdsToDrop.count(TenantInfo::INVALID_TENANT))
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				for( auto& baseCipherInfo : request.baseCipherInfos ) {
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
					if (tenantIdsToDrop.count(baseCipherInfo.domainId))
															#line 1343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
					{
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
						TraceEvent("GetEncryptCipherKeysSimulatedError").detail("DomainId", baseCipherInfo.domainId);
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
						if (deterministicRandom()->coinflip())
															#line 1349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
						{
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
							return a_body1Catch2(encrypt_keys_fetch_failed(), loopDepth);
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
						}
						else
						{
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
							return a_body1Catch2(encrypt_key_not_found(), loopDepth);
															#line 1359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
						}
					}
				}
			}
		}
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (!static_cast<_getUncachedEncryptCipherKeysActor*>(this)->SAV<EKPGetBaseCipherKeysByIdsReply>::futures) { (void)(reply); this->~_getUncachedEncryptCipherKeysActorState(); static_cast<_getUncachedEncryptCipherKeysActor*>(this)->destroy(); return 0; }
															#line 1367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		new (&static_cast<_getUncachedEncryptCipherKeysActor*>(this)->SAV< EKPGetBaseCipherKeysByIdsReply >::value()) EKPGetBaseCipherKeysByIdsReply(reply);
		this->~_getUncachedEncryptCipherKeysActorState();
		static_cast<_getUncachedEncryptCipherKeysActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(EKPGetBaseCipherKeysByIdsReply && reply,int loopDepth) 
	{
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (reply.error.present())
															#line 1379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		{
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			TraceEvent(SevWarn, "GetEncryptCipherKeysRequestFailed").error(reply.error.get());
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			return a_body1Catch2(reply.error.get(), loopDepth);
															#line 1385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		}
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (g_network && g_network->isSimulated() && usageType == BlobCipherMetrics::RESTORE)
															#line 1389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		{
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			std::unordered_set<int64_t> tenantIdsToDrop = parseStringToUnorderedSet<int64_t>(CLIENT_KNOBS->SIMULATION_EKP_TENANT_IDS_TO_DROP, ',');
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (!tenantIdsToDrop.count(TenantInfo::INVALID_TENANT))
															#line 1395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				for( auto& baseCipherInfo : request.baseCipherInfos ) {
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
					if (tenantIdsToDrop.count(baseCipherInfo.domainId))
															#line 1401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
					{
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
						TraceEvent("GetEncryptCipherKeysSimulatedError").detail("DomainId", baseCipherInfo.domainId);
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
						if (deterministicRandom()->coinflip())
															#line 1407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
						{
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
							return a_body1Catch2(encrypt_keys_fetch_failed(), loopDepth);
															#line 1411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
						}
						else
						{
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
							return a_body1Catch2(encrypt_key_not_found(), loopDepth);
															#line 1417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
						}
					}
				}
			}
		}
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (!static_cast<_getUncachedEncryptCipherKeysActor*>(this)->SAV<EKPGetBaseCipherKeysByIdsReply>::futures) { (void)(reply); this->~_getUncachedEncryptCipherKeysActorState(); static_cast<_getUncachedEncryptCipherKeysActor*>(this)->destroy(); return 0; }
															#line 1425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		new (&static_cast<_getUncachedEncryptCipherKeysActor*>(this)->SAV< EKPGetBaseCipherKeysByIdsReply >::value()) EKPGetBaseCipherKeysByIdsReply(reply);
		this->~_getUncachedEncryptCipherKeysActorState();
		static_cast<_getUncachedEncryptCipherKeysActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(EKPGetBaseCipherKeysByIdsReply const& reply,int loopDepth) 
	{
		loopDepth = a_body1cont3(reply, loopDepth);

		return loopDepth;
	}
	int a_body1when1(EKPGetBaseCipherKeysByIdsReply && reply,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(reply), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<_getUncachedEncryptCipherKeysActor*>(this)->actor_wait_state > 0) static_cast<_getUncachedEncryptCipherKeysActor*>(this)->actor_wait_state = 0;
		static_cast<_getUncachedEncryptCipherKeysActor*>(this)->ActorCallback< _getUncachedEncryptCipherKeysActor, 0, EKPGetBaseCipherKeysByIdsReply >::remove();

	}
	void a_callback_fire(ActorCallback< _getUncachedEncryptCipherKeysActor, 0, EKPGetBaseCipherKeysByIdsReply >*,EKPGetBaseCipherKeysByIdsReply const& value) 
	{
		fdb_probe_actor_enter("_getUncachedEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getUncachedEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< _getUncachedEncryptCipherKeysActor, 0, EKPGetBaseCipherKeysByIdsReply >*,EKPGetBaseCipherKeysByIdsReply && value) 
	{
		fdb_probe_actor_enter("_getUncachedEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getUncachedEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< _getUncachedEncryptCipherKeysActor, 0, EKPGetBaseCipherKeysByIdsReply >*,Error err) 
	{
		fdb_probe_actor_enter("_getUncachedEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getUncachedEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	Reference<AsyncVar<T> const> db;
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	EKPGetBaseCipherKeysByIdsRequest request;
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	BlobCipherMetrics::UsageType usageType;
															#line 1502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
};
// This generated class is to be used only via _getUncachedEncryptCipherKeys()
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getUncachedEncryptCipherKeysActor final : public Actor<EKPGetBaseCipherKeysByIdsReply>, public ActorCallback< _getUncachedEncryptCipherKeysActor<T>, 0, EKPGetBaseCipherKeysByIdsReply >, public FastAllocated<_getUncachedEncryptCipherKeysActor<T>>, public _getUncachedEncryptCipherKeysActorState<T, _getUncachedEncryptCipherKeysActor<T>> {
															#line 1509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
	using FastAllocated<_getUncachedEncryptCipherKeysActor<T>>::operator new;
	using FastAllocated<_getUncachedEncryptCipherKeysActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<EKPGetBaseCipherKeysByIdsReply>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< _getUncachedEncryptCipherKeysActor<T>, 0, EKPGetBaseCipherKeysByIdsReply >;
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getUncachedEncryptCipherKeysActor(Reference<AsyncVar<T> const> const& db,EKPGetBaseCipherKeysByIdsRequest const& request,BlobCipherMetrics::UsageType const& usageType) 
															#line 1520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		 : Actor<EKPGetBaseCipherKeysByIdsReply>(),
		   _getUncachedEncryptCipherKeysActorState<T, _getUncachedEncryptCipherKeysActor<T>>(db, request, usageType)
	{
		fdb_probe_actor_enter("_getUncachedEncryptCipherKeys", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("_getUncachedEncryptCipherKeys");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("_getUncachedEncryptCipherKeys", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< _getUncachedEncryptCipherKeysActor<T>, 0, EKPGetBaseCipherKeysByIdsReply >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
[[nodiscard]] Future<EKPGetBaseCipherKeysByIdsReply> _getUncachedEncryptCipherKeys( Reference<AsyncVar<T> const> const& db, EKPGetBaseCipherKeysByIdsRequest const& request, BlobCipherMetrics::UsageType const& usageType ) {
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	return Future<EKPGetBaseCipherKeysByIdsReply>(new _getUncachedEncryptCipherKeysActor<T>(db, request, usageType));
															#line 1550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
}

#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"

// Get cipher keys specified by the list of cipher details. It tries to get the cipher keys from local cache.
// In case of cache miss, it fetches the cipher keys from EncryptKeyProxy and put the result in the local cache
// before return.
															#line 1558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
namespace {
// This generated class is to be used only via _getEncryptCipherKeys()
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T, class _getEncryptCipherKeysActor>
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getEncryptCipherKeysActorState {
															#line 1565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getEncryptCipherKeysActorState(Reference<AsyncVar<T> const> const& db,std::unordered_set<BlobCipherDetails> const& cipherDetails,BlobCipherMetrics::UsageType const& usageType) 
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		 : db(db),
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   cipherDetails(cipherDetails),
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   usageType(usageType),
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   cipherKeyCache(BlobCipherKeyCache::getInstance()),
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   cipherKeys(),
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   uncachedBaseCipherIds(),
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   request()
															#line 1584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
	{
		fdb_probe_actor_create("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this));

	}
	~_getEncryptCipherKeysActorState() 
	{
		fdb_probe_actor_destroy("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (!db.isValid())
															#line 1599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				TraceEvent(SevError, "GetEncryptCipherKeysServerDBInfoNotAvailable");
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				return a_body1Catch1(encrypt_ops_error(), loopDepth);
															#line 1605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			}
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			for( const BlobCipherDetails& details : cipherDetails ) {
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				Reference<BlobCipherKey> cachedCipherKey = cipherKeyCache->getCipherKey(details.encryptDomainId, details.baseCipherId, details.salt);
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				if (cachedCipherKey.isValid())
															#line 1613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
				{
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
					cipherKeys.emplace(details, cachedCipherKey);
															#line 1617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
				}
				else
				{
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
					uncachedBaseCipherIds.insert(std::make_pair(details.encryptDomainId, details.baseCipherId));
															#line 1623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
				}
			}
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (uncachedBaseCipherIds.empty())
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				if (!static_cast<_getEncryptCipherKeysActor*>(this)->SAV<std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>>>::futures) { (void)(cipherKeys); this->~_getEncryptCipherKeysActorState(); static_cast<_getEncryptCipherKeysActor*>(this)->destroy(); return 0; }
															#line 1632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
				new (&static_cast<_getEncryptCipherKeysActor*>(this)->SAV< std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >::value()) std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>>(std::move(cipherKeys)); // state_var_RVO
				this->~_getEncryptCipherKeysActorState();
				static_cast<_getEncryptCipherKeysActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			for( const BaseCipherIndex& id : uncachedBaseCipherIds ) {
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				request.baseCipherInfos.emplace_back(id.first , id.second );
															#line 1642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			}
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			startTime = now();
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			;
															#line 1648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
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
		this->~_getEncryptCipherKeysActorState();
		static_cast<_getEncryptCipherKeysActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		double elapsed = now() - startTime;
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		BlobCipherMetrics::getInstance()->getCipherKeysLatency.addMeasurement(elapsed);
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		BlobCipherMetrics::counters(usageType).getCipherKeysLatency.addMeasurement(elapsed);
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (!static_cast<_getEncryptCipherKeysActor*>(this)->SAV<std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>>>::futures) { (void)(cipherKeys); this->~_getEncryptCipherKeysActorState(); static_cast<_getEncryptCipherKeysActor*>(this)->destroy(); return 0; }
															#line 1677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		new (&static_cast<_getEncryptCipherKeysActor*>(this)->SAV< std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >::value()) std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>>(std::move(cipherKeys)); // state_var_RVO
		this->~_getEncryptCipherKeysActorState();
		static_cast<_getEncryptCipherKeysActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		StrictFuture<EKPGetBaseCipherKeysByIdsReply> __when_expr_0 = _getUncachedEncryptCipherKeys(db, request, usageType);
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (static_cast<_getEncryptCipherKeysActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		StrictFuture<Void> __when_expr_1 = _onEncryptKeyProxyChange(db);
															#line 1702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<_getEncryptCipherKeysActor*>(this)->actor_wait_state = 1;
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< _getEncryptCipherKeysActor, 0, EKPGetBaseCipherKeysByIdsReply >*>(static_cast<_getEncryptCipherKeysActor*>(this)));
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< _getEncryptCipherKeysActor, 1, Void >*>(static_cast<_getEncryptCipherKeysActor*>(this)));
															#line 1709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
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
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(EKPGetBaseCipherKeysByIdsReply const& reply,int loopDepth) 
	{
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		std::unordered_map<BaseCipherIndex, EKPBaseCipherDetails, boost::hash<BaseCipherIndex>> baseCipherKeys;
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		for( const EKPBaseCipherDetails& baseDetails : reply.baseCipherDetails ) {
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			BaseCipherIndex baseIdx = std::make_pair(baseDetails.encryptDomainId, baseDetails.baseCipherId);
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			baseCipherKeys[baseIdx] = baseDetails;
															#line 1743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		}
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		for( const BlobCipherDetails& details : cipherDetails ) {
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (cipherKeys.count(details) > 0)
															#line 1749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
				continue;
			}
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			BaseCipherIndex baseIdx = std::make_pair(details.encryptDomainId, details.baseCipherId);
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			const auto& itr = baseCipherKeys.find(baseIdx);
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (itr == baseCipherKeys.end())
															#line 1759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				TraceEvent(SevError, "GetEncryptCipherKeysKeyMissing") .detail("DomainId", details.encryptDomainId) .detail("BaseCipherId", details.baseCipherId);
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				return a_body1Catch1(encrypt_key_not_found(), std::max(0, loopDepth - 1));
															#line 1765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			}
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			Reference<BlobCipherKey> cipherKey = cipherKeyCache->insertCipherKey(details.encryptDomainId, details.baseCipherId, itr->second.baseCipherKey.begin(), itr->second.baseCipherKey.size(), itr->second.baseCipherKCV, details.salt, itr->second.refreshAt, itr->second.expireAt);
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			ASSERT(cipherKey.isValid());
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			cipherKeys[details] = cipherKey;
															#line 1773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		}
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1when1(EKPGetBaseCipherKeysByIdsReply && reply,int loopDepth) 
	{
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		std::unordered_map<BaseCipherIndex, EKPBaseCipherDetails, boost::hash<BaseCipherIndex>> baseCipherKeys;
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		for( const EKPBaseCipherDetails& baseDetails : reply.baseCipherDetails ) {
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			BaseCipherIndex baseIdx = std::make_pair(baseDetails.encryptDomainId, baseDetails.baseCipherId);
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			baseCipherKeys[baseIdx] = baseDetails;
															#line 1789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		}
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		for( const BlobCipherDetails& details : cipherDetails ) {
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (cipherKeys.count(details) > 0)
															#line 1795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
				continue;
			}
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			BaseCipherIndex baseIdx = std::make_pair(details.encryptDomainId, details.baseCipherId);
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			const auto& itr = baseCipherKeys.find(baseIdx);
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (itr == baseCipherKeys.end())
															#line 1805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				TraceEvent(SevError, "GetEncryptCipherKeysKeyMissing") .detail("DomainId", details.encryptDomainId) .detail("BaseCipherId", details.baseCipherId);
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				return a_body1Catch1(encrypt_key_not_found(), std::max(0, loopDepth - 1));
															#line 1811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			}
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			Reference<BlobCipherKey> cipherKey = cipherKeyCache->insertCipherKey(details.encryptDomainId, details.baseCipherId, itr->second.baseCipherKey.begin(), itr->second.baseCipherKey.size(), itr->second.baseCipherKCV, details.salt, itr->second.refreshAt, itr->second.expireAt);
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			ASSERT(cipherKey.isValid());
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			cipherKeys[details] = cipherKey;
															#line 1819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		}
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<_getEncryptCipherKeysActor*>(this)->actor_wait_state > 0) static_cast<_getEncryptCipherKeysActor*>(this)->actor_wait_state = 0;
		static_cast<_getEncryptCipherKeysActor*>(this)->ActorCallback< _getEncryptCipherKeysActor, 0, EKPGetBaseCipherKeysByIdsReply >::remove();
		static_cast<_getEncryptCipherKeysActor*>(this)->ActorCallback< _getEncryptCipherKeysActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< _getEncryptCipherKeysActor, 0, EKPGetBaseCipherKeysByIdsReply >*,EKPGetBaseCipherKeysByIdsReply const& value) 
	{
		fdb_probe_actor_enter("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< _getEncryptCipherKeysActor, 0, EKPGetBaseCipherKeysByIdsReply >*,EKPGetBaseCipherKeysByIdsReply && value) 
	{
		fdb_probe_actor_enter("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< _getEncryptCipherKeysActor, 0, EKPGetBaseCipherKeysByIdsReply >*,Error err) 
	{
		fdb_probe_actor_enter("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< _getEncryptCipherKeysActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< _getEncryptCipherKeysActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< _getEncryptCipherKeysActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	Reference<AsyncVar<T> const> db;
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	std::unordered_set<BlobCipherDetails> cipherDetails;
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	BlobCipherMetrics::UsageType usageType;
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	Reference<BlobCipherKeyCache> cipherKeyCache;
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> cipherKeys;
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	std::unordered_set<BaseCipherIndex, boost::hash<BaseCipherIndex>> uncachedBaseCipherIds;
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	EKPGetBaseCipherKeysByIdsRequest request;
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	double startTime;
															#line 1950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
};
// This generated class is to be used only via _getEncryptCipherKeys()
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getEncryptCipherKeysActor final : public Actor<std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>>>, public ActorCallback< _getEncryptCipherKeysActor<T>, 0, EKPGetBaseCipherKeysByIdsReply >, public ActorCallback< _getEncryptCipherKeysActor<T>, 1, Void >, public FastAllocated<_getEncryptCipherKeysActor<T>>, public _getEncryptCipherKeysActorState<T, _getEncryptCipherKeysActor<T>> {
															#line 1957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
	using FastAllocated<_getEncryptCipherKeysActor<T>>::operator new;
	using FastAllocated<_getEncryptCipherKeysActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< _getEncryptCipherKeysActor<T>, 0, EKPGetBaseCipherKeysByIdsReply >;
friend struct ActorCallback< _getEncryptCipherKeysActor<T>, 1, Void >;
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getEncryptCipherKeysActor(Reference<AsyncVar<T> const> const& db,std::unordered_set<BlobCipherDetails> const& cipherDetails,BlobCipherMetrics::UsageType const& usageType) 
															#line 1969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		 : Actor<std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>>>(),
		   _getEncryptCipherKeysActorState<T, _getEncryptCipherKeysActor<T>>(db, cipherDetails, usageType)
	{
		fdb_probe_actor_enter("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("_getEncryptCipherKeys");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< _getEncryptCipherKeysActor<T>, 0, EKPGetBaseCipherKeysByIdsReply >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
[[nodiscard]] Future<std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>>> _getEncryptCipherKeys( Reference<AsyncVar<T> const> const& db, std::unordered_set<BlobCipherDetails> const& cipherDetails, BlobCipherMetrics::UsageType const& usageType ) {
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	return Future<std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>>>(new _getEncryptCipherKeysActor<T>(db, cipherDetails, usageType));
															#line 1999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
}

#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"

															#line 2004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
namespace {
// This generated class is to be used only via _getLatestEncryptCipherKeysForDomain()
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T, class _getLatestEncryptCipherKeysForDomainActor>
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getLatestEncryptCipherKeysForDomainActorState {
															#line 2011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getLatestEncryptCipherKeysForDomainActorState(Reference<AsyncVar<T> const> const& db,EncryptCipherDomainId const& domainId,BlobCipherMetrics::UsageType const& usageType) 
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		 : db(db),
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   domainId(domainId),
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   usageType(usageType)
															#line 2022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
	{
		fdb_probe_actor_create("_getLatestEncryptCipherKeysForDomain", reinterpret_cast<unsigned long>(this));

	}
	~_getLatestEncryptCipherKeysForDomainActorState() 
	{
		fdb_probe_actor_destroy("_getLatestEncryptCipherKeysForDomain", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			std::unordered_set<EncryptCipherDomainId> domainIds = { domainId, ENCRYPT_HEADER_DOMAIN_ID };
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			StrictFuture<std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>>> __when_expr_0 = _getLatestEncryptCipherKeys(db, domainIds, usageType);
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (static_cast<_getLatestEncryptCipherKeysForDomainActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<_getLatestEncryptCipherKeysForDomainActor*>(this)->actor_wait_state = 1;
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< _getLatestEncryptCipherKeysForDomainActor, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >*>(static_cast<_getLatestEncryptCipherKeysForDomainActor*>(this)));
															#line 2046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
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
		this->~_getLatestEncryptCipherKeysForDomainActorState();
		static_cast<_getLatestEncryptCipherKeysForDomainActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> const& cipherKeys,int loopDepth) 
	{
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		ASSERT(cipherKeys.count(domainId) > 0);
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		ASSERT(cipherKeys.count(ENCRYPT_HEADER_DOMAIN_ID) > 0);
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		TextAndHeaderCipherKeys result{ cipherKeys.at(domainId), cipherKeys.at(ENCRYPT_HEADER_DOMAIN_ID) };
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		ASSERT(result.cipherTextKey.isValid());
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		ASSERT(result.cipherHeaderKey.isValid());
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (!static_cast<_getLatestEncryptCipherKeysForDomainActor*>(this)->SAV<TextAndHeaderCipherKeys>::futures) { (void)(result); this->~_getLatestEncryptCipherKeysForDomainActorState(); static_cast<_getLatestEncryptCipherKeysForDomainActor*>(this)->destroy(); return 0; }
															#line 2079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		new (&static_cast<_getLatestEncryptCipherKeysForDomainActor*>(this)->SAV< TextAndHeaderCipherKeys >::value()) TextAndHeaderCipherKeys(result);
		this->~_getLatestEncryptCipherKeysForDomainActorState();
		static_cast<_getLatestEncryptCipherKeysForDomainActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> && cipherKeys,int loopDepth) 
	{
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		ASSERT(cipherKeys.count(domainId) > 0);
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		ASSERT(cipherKeys.count(ENCRYPT_HEADER_DOMAIN_ID) > 0);
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		TextAndHeaderCipherKeys result{ cipherKeys.at(domainId), cipherKeys.at(ENCRYPT_HEADER_DOMAIN_ID) };
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		ASSERT(result.cipherTextKey.isValid());
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		ASSERT(result.cipherHeaderKey.isValid());
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (!static_cast<_getLatestEncryptCipherKeysForDomainActor*>(this)->SAV<TextAndHeaderCipherKeys>::futures) { (void)(result); this->~_getLatestEncryptCipherKeysForDomainActorState(); static_cast<_getLatestEncryptCipherKeysForDomainActor*>(this)->destroy(); return 0; }
															#line 2101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		new (&static_cast<_getLatestEncryptCipherKeysForDomainActor*>(this)->SAV< TextAndHeaderCipherKeys >::value()) TextAndHeaderCipherKeys(result);
		this->~_getLatestEncryptCipherKeysForDomainActorState();
		static_cast<_getLatestEncryptCipherKeysForDomainActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> const& cipherKeys,int loopDepth) 
	{
		loopDepth = a_body1cont1(cipherKeys, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> && cipherKeys,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(cipherKeys), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<_getLatestEncryptCipherKeysForDomainActor*>(this)->actor_wait_state > 0) static_cast<_getLatestEncryptCipherKeysForDomainActor*>(this)->actor_wait_state = 0;
		static_cast<_getLatestEncryptCipherKeysForDomainActor*>(this)->ActorCallback< _getLatestEncryptCipherKeysForDomainActor, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >::remove();

	}
	void a_callback_fire(ActorCallback< _getLatestEncryptCipherKeysForDomainActor, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >*,std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> const& value) 
	{
		fdb_probe_actor_enter("_getLatestEncryptCipherKeysForDomain", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getLatestEncryptCipherKeysForDomain", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< _getLatestEncryptCipherKeysForDomainActor, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >*,std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> && value) 
	{
		fdb_probe_actor_enter("_getLatestEncryptCipherKeysForDomain", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getLatestEncryptCipherKeysForDomain", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< _getLatestEncryptCipherKeysForDomainActor, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >*,Error err) 
	{
		fdb_probe_actor_enter("_getLatestEncryptCipherKeysForDomain", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getLatestEncryptCipherKeysForDomain", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	Reference<AsyncVar<T> const> db;
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	EncryptCipherDomainId domainId;
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	BlobCipherMetrics::UsageType usageType;
															#line 2178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
};
// This generated class is to be used only via _getLatestEncryptCipherKeysForDomain()
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getLatestEncryptCipherKeysForDomainActor final : public Actor<TextAndHeaderCipherKeys>, public ActorCallback< _getLatestEncryptCipherKeysForDomainActor<T>, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >, public FastAllocated<_getLatestEncryptCipherKeysForDomainActor<T>>, public _getLatestEncryptCipherKeysForDomainActorState<T, _getLatestEncryptCipherKeysForDomainActor<T>> {
															#line 2185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
	using FastAllocated<_getLatestEncryptCipherKeysForDomainActor<T>>::operator new;
	using FastAllocated<_getLatestEncryptCipherKeysForDomainActor<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<TextAndHeaderCipherKeys>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< _getLatestEncryptCipherKeysForDomainActor<T>, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >;
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getLatestEncryptCipherKeysForDomainActor(Reference<AsyncVar<T> const> const& db,EncryptCipherDomainId const& domainId,BlobCipherMetrics::UsageType const& usageType) 
															#line 2196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		 : Actor<TextAndHeaderCipherKeys>(),
		   _getLatestEncryptCipherKeysForDomainActorState<T, _getLatestEncryptCipherKeysForDomainActor<T>>(db, domainId, usageType)
	{
		fdb_probe_actor_enter("_getLatestEncryptCipherKeysForDomain", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("_getLatestEncryptCipherKeysForDomain");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("_getLatestEncryptCipherKeysForDomain", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< _getLatestEncryptCipherKeysForDomainActor<T>, 0, std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
[[nodiscard]] Future<TextAndHeaderCipherKeys> _getLatestEncryptCipherKeysForDomain( Reference<AsyncVar<T> const> const& db, EncryptCipherDomainId const& domainId, BlobCipherMetrics::UsageType const& usageType ) {
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	return Future<TextAndHeaderCipherKeys>(new _getLatestEncryptCipherKeysForDomainActor<T>(db, domainId, usageType));
															#line 2226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
}

#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"

template <class T>
Future<TextAndHeaderCipherKeys> _getLatestSystemEncryptCipherKeys(const Reference<AsyncVar<T> const>& db,
                                                                  BlobCipherMetrics::UsageType usageType) {
	return _getLatestEncryptCipherKeysForDomain(db, SYSTEM_KEYSPACE_ENCRYPT_DOMAIN_ID, usageType);
}

															#line 2237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
namespace {
// This generated class is to be used only via _getEncryptCipherKeys()
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T, class _getEncryptCipherKeysActor1>
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getEncryptCipherKeysActor1State {
															#line 2244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getEncryptCipherKeysActor1State(Reference<AsyncVar<T> const> const& db,BlobCipherEncryptHeader const& header,BlobCipherMetrics::UsageType const& usageType) 
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		 : db(db),
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   header(header),
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   usageType(usageType),
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   authenticatedEncryption(header.flags.authTokenMode != ENCRYPT_HEADER_AUTH_TOKEN_MODE_NONE)
															#line 2257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
	{
		fdb_probe_actor_create("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this));

	}
	~_getEncryptCipherKeysActor1State() 
	{
		fdb_probe_actor_destroy("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			ASSERT(header.cipherTextDetails.isValid());
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			ASSERT(!authenticatedEncryption || header.cipherHeaderDetails.isValid());
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			std::unordered_set<BlobCipherDetails> cipherDetails{ header.cipherTextDetails };
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (authenticatedEncryption)
															#line 2278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				cipherDetails.insert(header.cipherHeaderDetails);
															#line 2282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			}
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			StrictFuture<std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>>> __when_expr_0 = _getEncryptCipherKeys(db, cipherDetails, usageType);
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (static_cast<_getEncryptCipherKeysActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<_getEncryptCipherKeysActor1*>(this)->actor_wait_state = 1;
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< _getEncryptCipherKeysActor1, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >*>(static_cast<_getEncryptCipherKeysActor1*>(this)));
															#line 2293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
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
		this->~_getEncryptCipherKeysActor1State();
		static_cast<_getEncryptCipherKeysActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> const& cipherKeys,int loopDepth) 
	{
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		TextAndHeaderCipherKeys result;
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		auto setCipherKey = [&](const BlobCipherDetails& details, TextAndHeaderCipherKeys& result) { ASSERT(details.isValid()); auto iter = cipherKeys.find(details); ASSERT(iter != cipherKeys.end() && iter->second.isValid()); isEncryptHeaderDomain(details.encryptDomainId) ? result.cipherHeaderKey = iter->second : result.cipherTextKey = iter->second; };
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		setCipherKey(header.cipherTextDetails, result);
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (authenticatedEncryption)
															#line 2322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		{
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			setCipherKey(header.cipherHeaderDetails, result);
															#line 2326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		}
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		ASSERT(result.cipherTextKey.isValid() && (!authenticatedEncryption || result.cipherHeaderKey.isValid()));
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (!static_cast<_getEncryptCipherKeysActor1*>(this)->SAV<TextAndHeaderCipherKeys>::futures) { (void)(result); this->~_getEncryptCipherKeysActor1State(); static_cast<_getEncryptCipherKeysActor1*>(this)->destroy(); return 0; }
															#line 2332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		new (&static_cast<_getEncryptCipherKeysActor1*>(this)->SAV< TextAndHeaderCipherKeys >::value()) TextAndHeaderCipherKeys(result);
		this->~_getEncryptCipherKeysActor1State();
		static_cast<_getEncryptCipherKeysActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> && cipherKeys,int loopDepth) 
	{
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		TextAndHeaderCipherKeys result;
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		auto setCipherKey = [&](const BlobCipherDetails& details, TextAndHeaderCipherKeys& result) { ASSERT(details.isValid()); auto iter = cipherKeys.find(details); ASSERT(iter != cipherKeys.end() && iter->second.isValid()); isEncryptHeaderDomain(details.encryptDomainId) ? result.cipherHeaderKey = iter->second : result.cipherTextKey = iter->second; };
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		setCipherKey(header.cipherTextDetails, result);
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (authenticatedEncryption)
															#line 2350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		{
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			setCipherKey(header.cipherHeaderDetails, result);
															#line 2354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		}
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		ASSERT(result.cipherTextKey.isValid() && (!authenticatedEncryption || result.cipherHeaderKey.isValid()));
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (!static_cast<_getEncryptCipherKeysActor1*>(this)->SAV<TextAndHeaderCipherKeys>::futures) { (void)(result); this->~_getEncryptCipherKeysActor1State(); static_cast<_getEncryptCipherKeysActor1*>(this)->destroy(); return 0; }
															#line 2360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		new (&static_cast<_getEncryptCipherKeysActor1*>(this)->SAV< TextAndHeaderCipherKeys >::value()) TextAndHeaderCipherKeys(result);
		this->~_getEncryptCipherKeysActor1State();
		static_cast<_getEncryptCipherKeysActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> const& cipherKeys,int loopDepth) 
	{
		loopDepth = a_body1cont1(cipherKeys, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> && cipherKeys,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(cipherKeys), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<_getEncryptCipherKeysActor1*>(this)->actor_wait_state > 0) static_cast<_getEncryptCipherKeysActor1*>(this)->actor_wait_state = 0;
		static_cast<_getEncryptCipherKeysActor1*>(this)->ActorCallback< _getEncryptCipherKeysActor1, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >::remove();

	}
	void a_callback_fire(ActorCallback< _getEncryptCipherKeysActor1, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >*,std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> const& value) 
	{
		fdb_probe_actor_enter("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< _getEncryptCipherKeysActor1, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >*,std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> && value) 
	{
		fdb_probe_actor_enter("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< _getEncryptCipherKeysActor1, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >*,Error err) 
	{
		fdb_probe_actor_enter("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	Reference<AsyncVar<T> const> db;
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	BlobCipherEncryptHeader header;
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	BlobCipherMetrics::UsageType usageType;
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	bool authenticatedEncryption;
															#line 2439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
};
// This generated class is to be used only via _getEncryptCipherKeys()
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getEncryptCipherKeysActor1 final : public Actor<TextAndHeaderCipherKeys>, public ActorCallback< _getEncryptCipherKeysActor1<T>, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >, public FastAllocated<_getEncryptCipherKeysActor1<T>>, public _getEncryptCipherKeysActor1State<T, _getEncryptCipherKeysActor1<T>> {
															#line 2446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
	using FastAllocated<_getEncryptCipherKeysActor1<T>>::operator new;
	using FastAllocated<_getEncryptCipherKeysActor1<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<TextAndHeaderCipherKeys>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< _getEncryptCipherKeysActor1<T>, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >;
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getEncryptCipherKeysActor1(Reference<AsyncVar<T> const> const& db,BlobCipherEncryptHeader const& header,BlobCipherMetrics::UsageType const& usageType) 
															#line 2457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		 : Actor<TextAndHeaderCipherKeys>(),
		   _getEncryptCipherKeysActor1State<T, _getEncryptCipherKeysActor1<T>>(db, header, usageType)
	{
		fdb_probe_actor_enter("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("_getEncryptCipherKeys");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< _getEncryptCipherKeysActor1<T>, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
[[nodiscard]] Future<TextAndHeaderCipherKeys> _getEncryptCipherKeys( Reference<AsyncVar<T> const> const& db, BlobCipherEncryptHeader const& header, BlobCipherMetrics::UsageType const& usageType ) {
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	return Future<TextAndHeaderCipherKeys>(new _getEncryptCipherKeysActor1<T>(db, header, usageType));
															#line 2487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
}

#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"

															#line 2492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
namespace {
// This generated class is to be used only via _getEncryptCipherKeys()
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T, class _getEncryptCipherKeysActor2>
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getEncryptCipherKeysActor2State {
															#line 2499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getEncryptCipherKeysActor2State(Reference<AsyncVar<T> const> const& db,BlobCipherEncryptHeaderRef const& header,BlobCipherMetrics::UsageType const& usageType) 
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		 : db(db),
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   header(header),
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		   usageType(usageType)
															#line 2510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
	{
		fdb_probe_actor_create("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this));

	}
	~_getEncryptCipherKeysActor2State() 
	{
		fdb_probe_actor_destroy("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			ASSERT(CLIENT_KNOBS->ENABLE_CONFIGURABLE_ENCRYPTION);
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			authenticatedEncryption = header.getAuthTokenMode() != ENCRYPT_HEADER_AUTH_TOKEN_MODE_NONE;
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			details = header.getCipherDetails();
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			ASSERT(details.textCipherDetails.isValid());
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			ASSERT(!authenticatedEncryption || (details.headerCipherDetails.present() && details.headerCipherDetails.get().isValid()));
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			std::unordered_set<BlobCipherDetails> cipherDetails{ details.textCipherDetails };
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (authenticatedEncryption)
															#line 2537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			{
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
				cipherDetails.insert(details.headerCipherDetails.get());
															#line 2541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			}
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			StrictFuture<std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>>> __when_expr_0 = _getEncryptCipherKeys(db, cipherDetails, usageType);
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			if (static_cast<_getEncryptCipherKeysActor2*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<_getEncryptCipherKeysActor2*>(this)->actor_wait_state = 1;
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< _getEncryptCipherKeysActor2, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >*>(static_cast<_getEncryptCipherKeysActor2*>(this)));
															#line 2552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
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
		this->~_getEncryptCipherKeysActor2State();
		static_cast<_getEncryptCipherKeysActor2*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> const& cipherKeys,int loopDepth) 
	{
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		TextAndHeaderCipherKeys result;
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		auto setCipherKey = [&](const BlobCipherDetails& details, TextAndHeaderCipherKeys& result) { ASSERT(details.isValid()); auto iter = cipherKeys.find(details); ASSERT(iter != cipherKeys.end() && iter->second.isValid()); isEncryptHeaderDomain(details.encryptDomainId) ? result.cipherHeaderKey = iter->second : result.cipherTextKey = iter->second; };
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		setCipherKey(details.textCipherDetails, result);
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (authenticatedEncryption)
															#line 2581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		{
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			setCipherKey(details.headerCipherDetails.get(), result);
															#line 2585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		}
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		ASSERT(result.cipherTextKey.isValid() && (!authenticatedEncryption || result.cipherHeaderKey.isValid()));
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (!static_cast<_getEncryptCipherKeysActor2*>(this)->SAV<TextAndHeaderCipherKeys>::futures) { (void)(result); this->~_getEncryptCipherKeysActor2State(); static_cast<_getEncryptCipherKeysActor2*>(this)->destroy(); return 0; }
															#line 2591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		new (&static_cast<_getEncryptCipherKeysActor2*>(this)->SAV< TextAndHeaderCipherKeys >::value()) TextAndHeaderCipherKeys(result);
		this->~_getEncryptCipherKeysActor2State();
		static_cast<_getEncryptCipherKeysActor2*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> && cipherKeys,int loopDepth) 
	{
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		TextAndHeaderCipherKeys result;
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		auto setCipherKey = [&](const BlobCipherDetails& details, TextAndHeaderCipherKeys& result) { ASSERT(details.isValid()); auto iter = cipherKeys.find(details); ASSERT(iter != cipherKeys.end() && iter->second.isValid()); isEncryptHeaderDomain(details.encryptDomainId) ? result.cipherHeaderKey = iter->second : result.cipherTextKey = iter->second; };
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		setCipherKey(details.textCipherDetails, result);
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (authenticatedEncryption)
															#line 2609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		{
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
			setCipherKey(details.headerCipherDetails.get(), result);
															#line 2613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		}
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		ASSERT(result.cipherTextKey.isValid() && (!authenticatedEncryption || result.cipherHeaderKey.isValid()));
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
		if (!static_cast<_getEncryptCipherKeysActor2*>(this)->SAV<TextAndHeaderCipherKeys>::futures) { (void)(result); this->~_getEncryptCipherKeysActor2State(); static_cast<_getEncryptCipherKeysActor2*>(this)->destroy(); return 0; }
															#line 2619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		new (&static_cast<_getEncryptCipherKeysActor2*>(this)->SAV< TextAndHeaderCipherKeys >::value()) TextAndHeaderCipherKeys(result);
		this->~_getEncryptCipherKeysActor2State();
		static_cast<_getEncryptCipherKeysActor2*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> const& cipherKeys,int loopDepth) 
	{
		loopDepth = a_body1cont1(cipherKeys, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> && cipherKeys,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(cipherKeys), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<_getEncryptCipherKeysActor2*>(this)->actor_wait_state > 0) static_cast<_getEncryptCipherKeysActor2*>(this)->actor_wait_state = 0;
		static_cast<_getEncryptCipherKeysActor2*>(this)->ActorCallback< _getEncryptCipherKeysActor2, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >::remove();

	}
	void a_callback_fire(ActorCallback< _getEncryptCipherKeysActor2, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >*,std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> const& value) 
	{
		fdb_probe_actor_enter("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< _getEncryptCipherKeysActor2, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >*,std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> && value) 
	{
		fdb_probe_actor_enter("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< _getEncryptCipherKeysActor2, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >*,Error err) 
	{
		fdb_probe_actor_enter("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	Reference<AsyncVar<T> const> db;
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	BlobCipherEncryptHeaderRef header;
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	BlobCipherMetrics::UsageType usageType;
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	bool authenticatedEncryption;
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	EncryptHeaderCipherDetails details;
															#line 2700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
};
// This generated class is to be used only via _getEncryptCipherKeys()
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
class _getEncryptCipherKeysActor2 final : public Actor<TextAndHeaderCipherKeys>, public ActorCallback< _getEncryptCipherKeysActor2<T>, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >, public FastAllocated<_getEncryptCipherKeysActor2<T>>, public _getEncryptCipherKeysActor2State<T, _getEncryptCipherKeysActor2<T>> {
															#line 2707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
public:
	using FastAllocated<_getEncryptCipherKeysActor2<T>>::operator new;
	using FastAllocated<_getEncryptCipherKeysActor2<T>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<TextAndHeaderCipherKeys>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< _getEncryptCipherKeysActor2<T>, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >;
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	_getEncryptCipherKeysActor2(Reference<AsyncVar<T> const> const& db,BlobCipherEncryptHeaderRef const& header,BlobCipherMetrics::UsageType const& usageType) 
															#line 2718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
		 : Actor<TextAndHeaderCipherKeys>(),
		   _getEncryptCipherKeysActor2State<T, _getEncryptCipherKeysActor2<T>>(db, header, usageType)
	{
		fdb_probe_actor_enter("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("_getEncryptCipherKeys");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("_getEncryptCipherKeys", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< _getEncryptCipherKeysActor2<T>, 0, std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
template <class T>
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
[[nodiscard]] Future<TextAndHeaderCipherKeys> _getEncryptCipherKeys( Reference<AsyncVar<T> const> const& db, BlobCipherEncryptHeaderRef const& header, BlobCipherMetrics::UsageType const& usageType ) {
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"
	return Future<TextAndHeaderCipherKeys>(new _getEncryptCipherKeysActor2<T>(db, header, usageType));
															#line 2748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.g.h"
}

#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GetEncryptCipherKeys_impl.actor.h"

template <class T>
Future<std::unordered_map<EncryptCipherDomainId, Reference<BlobCipherKey>>>
GetEncryptCipherKeys<T>::getLatestEncryptCipherKeys(Reference<AsyncVar<T> const> db,
                                                    std::unordered_set<EncryptCipherDomainId> domainIds,
                                                    BlobCipherMetrics::UsageType usageType) {
	return _getLatestEncryptCipherKeys(db, domainIds, usageType);
}

template <class T>
Future<Reference<BlobCipherKey>> GetEncryptCipherKeys<T>::getLatestEncryptCipherKey(
    Reference<AsyncVar<T> const> db,
    EncryptCipherDomainId domainId,
    BlobCipherMetrics::UsageType usageType) {
	return _getLatestEncryptCipherKey(db, domainId, usageType);
}

template <class T>
Future<std::unordered_map<BlobCipherDetails, Reference<BlobCipherKey>>> GetEncryptCipherKeys<T>::getEncryptCipherKeys(
    Reference<AsyncVar<T> const> db,
    std::unordered_set<BlobCipherDetails> cipherDetails,
    BlobCipherMetrics::UsageType usageType) {
	return _getEncryptCipherKeys(db, cipherDetails, usageType);
}

template <class T>
Future<TextAndHeaderCipherKeys> GetEncryptCipherKeys<T>::getLatestEncryptCipherKeysForDomain(
    Reference<AsyncVar<T> const> db,
    EncryptCipherDomainId domainId,
    BlobCipherMetrics::UsageType usageType) {
	return _getLatestEncryptCipherKeysForDomain(db, domainId, usageType);
}

template <class T>
Future<TextAndHeaderCipherKeys> GetEncryptCipherKeys<T>::getLatestSystemEncryptCipherKeys(
    const Reference<AsyncVar<T> const>& db,
    BlobCipherMetrics::UsageType usageType) {
	return _getLatestSystemEncryptCipherKeys(db, usageType);
}

template <class T>
Future<TextAndHeaderCipherKeys> GetEncryptCipherKeys<T>::getEncryptCipherKeys(Reference<AsyncVar<T> const> db,
                                                                              BlobCipherEncryptHeader header,
                                                                              BlobCipherMetrics::UsageType usageType) {
	return _getEncryptCipherKeys(db, header, usageType);
}

template <class T>
Future<TextAndHeaderCipherKeys> GetEncryptCipherKeys<T>::getEncryptCipherKeys(Reference<AsyncVar<T> const> db,
                                                                              BlobCipherEncryptHeaderRef header,
                                                                              BlobCipherMetrics::UsageType usageType) {
	return _getEncryptCipherKeys(db, header, usageType);
}

#include "flow/unactorcompiler.h"
#endif