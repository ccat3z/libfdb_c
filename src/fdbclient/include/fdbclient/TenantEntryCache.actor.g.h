#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
/*
 * TenantEntryCache.h
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

#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_TENANTENTRYCACHE_ACTOR_G_H)
#define FDBCLIENT_TENANTENTRYCACHE_ACTOR_G_H
#include "fdbclient/TenantEntryCache.actor.g.h"
#elif !defined(FDBCLIENT_TENANTENTRYCACHE_ACTOR_H)
#define FDBCLIENT_TENANTENTRYCACHE_ACTOR_H

#pragma once

#include "fdbclient/DatabaseContext.h"
#include "fdbclient/FDBOptions.g.h"
#include "fdbclient/FDBTypes.h"
#include "fdbclient/RunRYWTransaction.actor.h"
#include "fdbclient/Tenant.h"
#include "fdbclient/TenantManagement.actor.h"
#include "fdbclient/Knobs.h"
#include "fdbrpc/TenantName.h"
#include "flow/IndexedSet.h"

#include <functional>
#include <unordered_map>

#include "flow/actorcompiler.h" // has to be last include

using TenantNameEntryPair = std::pair<TenantName, TenantMapEntry>;
using TenantNameEntryPairVec = std::vector<TenantNameEntryPair>;

enum class TenantEntryCacheRefreshReason {
	INIT = 1,
	PERIODIC_TASK = 2,
	CACHE_MISS = 3,
	REMOVE_ENTRY = 4,
	WATCH_TRIGGER = 5
};
enum class TenantEntryCacheRefreshMode { PERIODIC_TASK = 1, WATCH = 2, NONE = 3 };

template <class T>
struct TenantEntryCachePayload {
	TenantMapEntry entry;
	// Custom client payload
	T payload;
};

template <class T>
using TenantEntryCachePayloadFunc = std::function<TenantEntryCachePayload<T>(const TenantMapEntry&)>;

// In-memory cache for TenantEntryMap objects. It supports three indices:
// 1. Lookup by 'TenantId'
// 2. Lookup by 'TenantPrefix'
// 3. Lookup by 'TenantName'
// TODO: Currently this cache performs poorly if there are tenant access happening to unknown tenants which happens most
// frequently in optional tenant mode but can also happen in required mode if there are alot of tenants created. Further
// as a consequence of the design we cannot be sure that the state of a given tenant is accurate even if its present in
// the cache.

template <class T>
class TenantEntryCache : public ReferenceCounted<TenantEntryCache<T>>, NonCopyable {
private:
	UID uid;
	Database db;
	TenantEntryCachePayloadFunc<T> createPayloadFunc;
	TenantEntryCacheRefreshMode refreshMode;

	Future<Void> refresher;
	Future<Void> watchRefresher;
	Future<Void> lastTenantIdRefresher;
	Promise<Void> setInitialWatch;
	Optional<int64_t> lastTenantId;
	Map<int64_t, TenantEntryCachePayload<T>> mapByTenantId;
	Map<TenantName, TenantEntryCachePayload<T>> mapByTenantName;

	CounterCollection metrics;
	Counter hits;
	Counter misses;
	Counter refreshByCacheInit;
	Counter refreshByCacheMiss;
	Counter numRefreshes;
	Counter refreshByWatchTrigger;

																#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
// This generated class is to be used only via getTenantList()
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
template <class GetTenantListActor>
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class GetTenantListActorState {
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	GetTenantListActorState(Reference<ReadYourWritesTransaction> const& tr) 
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		 : tr(tr)
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
	{
		fdb_probe_actor_create("getTenantList", reinterpret_cast<unsigned long>(this));

	}
	~GetTenantListActorState() 
	{
		fdb_probe_actor_destroy("getTenantList", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr->setOption(FDBTransactionOptions::READ_LOCK_AWARE);
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			StrictFuture<KeyBackedRangeResult<std::pair<int64_t, TenantMapEntry>>> __when_expr_0 = TenantMetadata::tenantMap().getRange(tr, {}, {}, CLIENT_KNOBS->MAX_TENANTS_PER_CLUSTER + 1);
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (static_cast<GetTenantListActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetTenantListActor*>(this)->actor_wait_state = 1;
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetTenantListActor, 0, KeyBackedRangeResult<std::pair<int64_t, TenantMapEntry>> >*>(static_cast<GetTenantListActor*>(this)));
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
		this->~GetTenantListActorState();
		static_cast<GetTenantListActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(KeyBackedRangeResult<std::pair<int64_t, TenantMapEntry>> const& tenantList,int loopDepth) 
	{
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		ASSERT(tenantList.results.size() <= CLIENT_KNOBS->MAX_TENANTS_PER_CLUSTER && !tenantList.more);
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		TraceEvent(SevDebug, "TenantEntryCacheGetTenantList").detail("Count", tenantList.results.size());
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (!static_cast<GetTenantListActor*>(this)->SAV<std::vector<std::pair<int64_t, TenantMapEntry>>>::futures) { (void)(tenantList.results); this->~GetTenantListActorState(); static_cast<GetTenantListActor*>(this)->destroy(); return 0; }
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		new (&static_cast<GetTenantListActor*>(this)->SAV< std::vector<std::pair<int64_t, TenantMapEntry>> >::value()) std::vector<std::pair<int64_t, TenantMapEntry>>(tenantList.results);
		this->~GetTenantListActorState();
		static_cast<GetTenantListActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(KeyBackedRangeResult<std::pair<int64_t, TenantMapEntry>> && tenantList,int loopDepth) 
	{
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		ASSERT(tenantList.results.size() <= CLIENT_KNOBS->MAX_TENANTS_PER_CLUSTER && !tenantList.more);
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		TraceEvent(SevDebug, "TenantEntryCacheGetTenantList").detail("Count", tenantList.results.size());
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (!static_cast<GetTenantListActor*>(this)->SAV<std::vector<std::pair<int64_t, TenantMapEntry>>>::futures) { (void)(tenantList.results); this->~GetTenantListActorState(); static_cast<GetTenantListActor*>(this)->destroy(); return 0; }
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		new (&static_cast<GetTenantListActor*>(this)->SAV< std::vector<std::pair<int64_t, TenantMapEntry>> >::value()) std::vector<std::pair<int64_t, TenantMapEntry>>(tenantList.results);
		this->~GetTenantListActorState();
		static_cast<GetTenantListActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(KeyBackedRangeResult<std::pair<int64_t, TenantMapEntry>> const& tenantList,int loopDepth) 
	{
		loopDepth = a_body1cont1(tenantList, loopDepth);

		return loopDepth;
	}
	int a_body1when1(KeyBackedRangeResult<std::pair<int64_t, TenantMapEntry>> && tenantList,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(tenantList), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetTenantListActor*>(this)->actor_wait_state > 0) static_cast<GetTenantListActor*>(this)->actor_wait_state = 0;
		static_cast<GetTenantListActor*>(this)->ActorCallback< GetTenantListActor, 0, KeyBackedRangeResult<std::pair<int64_t, TenantMapEntry>> >::remove();

	}
	void a_callback_fire(ActorCallback< GetTenantListActor, 0, KeyBackedRangeResult<std::pair<int64_t, TenantMapEntry>> >*,KeyBackedRangeResult<std::pair<int64_t, TenantMapEntry>> const& value) 
	{
		fdb_probe_actor_enter("getTenantList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenantList", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetTenantListActor, 0, KeyBackedRangeResult<std::pair<int64_t, TenantMapEntry>> >*,KeyBackedRangeResult<std::pair<int64_t, TenantMapEntry>> && value) 
	{
		fdb_probe_actor_enter("getTenantList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenantList", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetTenantListActor, 0, KeyBackedRangeResult<std::pair<int64_t, TenantMapEntry>> >*,Error err) 
	{
		fdb_probe_actor_enter("getTenantList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenantList", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	Reference<ReadYourWritesTransaction> tr;
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
};
// This generated class is to be used only via getTenantList()
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class GetTenantListActor final : public Actor<std::vector<std::pair<int64_t, TenantMapEntry>>>, public ActorCallback< GetTenantListActor, 0, KeyBackedRangeResult<std::pair<int64_t, TenantMapEntry>> >, public FastAllocated<GetTenantListActor>, public GetTenantListActorState<GetTenantListActor> {
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
	using FastAllocated<GetTenantListActor>::operator new;
	using FastAllocated<GetTenantListActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<std::pair<int64_t, TenantMapEntry>>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetTenantListActor, 0, KeyBackedRangeResult<std::pair<int64_t, TenantMapEntry>> >;
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	GetTenantListActor(Reference<ReadYourWritesTransaction> const& tr) 
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		 : Actor<std::vector<std::pair<int64_t, TenantMapEntry>>>(),
		   GetTenantListActorState<GetTenantListActor>(tr)
	{
		fdb_probe_actor_enter("getTenantList", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getTenantList");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getTenantList", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetTenantListActor, 0, KeyBackedRangeResult<std::pair<int64_t, TenantMapEntry>> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
[[nodiscard]] static Future<std::vector<std::pair<int64_t, TenantMapEntry>>> getTenantList( Reference<ReadYourWritesTransaction> const& tr ) {
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	return Future<std::vector<std::pair<int64_t, TenantMapEntry>>>(new GetTenantListActor(tr));
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
}

#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"

																#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
// This generated class is to be used only via refreshCacheById()
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
template <class RefreshCacheByIdActor>
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class RefreshCacheByIdActorState {
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	RefreshCacheByIdActorState(int64_t const& tenantId,TenantEntryCache<T>* const& cache,TenantEntryCacheRefreshReason const& reason) 
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		 : tenantId(tenantId),
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		   cache(cache),
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		   reason(reason)
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
	{
		fdb_probe_actor_create("refreshCacheById", reinterpret_cast<unsigned long>(this));

	}
	~RefreshCacheByIdActorState() 
	{
		fdb_probe_actor_destroy("refreshCacheById", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			TraceEvent(SevDebug, "TenantEntryCacheIDRefreshStart", cache->id()).detail("Reason", static_cast<int>(reason));
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr = cache->getDatabase()->createTransaction();
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			;
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
		this->~RefreshCacheByIdActorState();
		static_cast<RefreshCacheByIdActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		TraceEvent(SevDebug, "TenantEntryCacheIDRefreshEnd", cache->id()).detail("Reason", static_cast<int>(reason));
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (!static_cast<RefreshCacheByIdActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RefreshCacheByIdActorState(); static_cast<RefreshCacheByIdActor*>(this)->destroy(); return 0; }
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		new (&static_cast<RefreshCacheByIdActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RefreshCacheByIdActorState();
		static_cast<RefreshCacheByIdActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr->setOption(FDBTransactionOptions::READ_LOCK_AWARE);
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			StrictFuture<Optional<TenantMapEntry>> __when_expr_0 = TenantMetadata::tenantMap().get(tr, tenantId);
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (static_cast<RefreshCacheByIdActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RefreshCacheByIdActor*>(this)->actor_wait_state = 1;
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RefreshCacheByIdActor, 0, Optional<TenantMapEntry> >*>(static_cast<RefreshCacheByIdActor*>(this)));
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			StrictFuture<Void> __when_expr_1 = tr->onError(e);
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (static_cast<RefreshCacheByIdActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<RefreshCacheByIdActor*>(this)->actor_wait_state = 2;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RefreshCacheByIdActor, 1, Void >*>(static_cast<RefreshCacheByIdActor*>(this)));
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (entry.present())
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		{
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			cache->put(entry.get());
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			updateCacheRefreshMetrics(cache, reason);
															#line 459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		}
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<TenantMapEntry> const& __entry,int loopDepth) 
	{
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		entry = __entry;
															#line 469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<TenantMapEntry> && __entry,int loopDepth) 
	{
		entry = std::move(__entry);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RefreshCacheByIdActor*>(this)->actor_wait_state > 0) static_cast<RefreshCacheByIdActor*>(this)->actor_wait_state = 0;
		static_cast<RefreshCacheByIdActor*>(this)->ActorCallback< RefreshCacheByIdActor, 0, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< RefreshCacheByIdActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("refreshCacheById", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheById", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RefreshCacheByIdActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("refreshCacheById", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheById", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RefreshCacheByIdActor, 0, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("refreshCacheById", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheById", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<RefreshCacheByIdActor*>(this)->actor_wait_state > 0) static_cast<RefreshCacheByIdActor*>(this)->actor_wait_state = 0;
		static_cast<RefreshCacheByIdActor*>(this)->ActorCallback< RefreshCacheByIdActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RefreshCacheByIdActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("refreshCacheById", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheById", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RefreshCacheByIdActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("refreshCacheById", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheById", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RefreshCacheByIdActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("refreshCacheById", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheById", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	int64_t tenantId;
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	TenantEntryCache<T>* cache;
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	TenantEntryCacheRefreshReason reason;
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	Reference<ReadYourWritesTransaction> tr;
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	Optional<TenantMapEntry> entry;
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
};
// This generated class is to be used only via refreshCacheById()
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class RefreshCacheByIdActor final : public Actor<Void>, public ActorCallback< RefreshCacheByIdActor, 0, Optional<TenantMapEntry> >, public ActorCallback< RefreshCacheByIdActor, 1, Void >, public FastAllocated<RefreshCacheByIdActor>, public RefreshCacheByIdActorState<RefreshCacheByIdActor> {
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
	using FastAllocated<RefreshCacheByIdActor>::operator new;
	using FastAllocated<RefreshCacheByIdActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RefreshCacheByIdActor, 0, Optional<TenantMapEntry> >;
friend struct ActorCallback< RefreshCacheByIdActor, 1, Void >;
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	RefreshCacheByIdActor(int64_t const& tenantId,TenantEntryCache<T>* const& cache,TenantEntryCacheRefreshReason const& reason) 
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		 : Actor<Void>(),
		   RefreshCacheByIdActorState<RefreshCacheByIdActor>(tenantId, cache, reason)
	{
		fdb_probe_actor_enter("refreshCacheById", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("refreshCacheById");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("refreshCacheById", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RefreshCacheByIdActor, 0, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RefreshCacheByIdActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
[[nodiscard]] static Future<Void> refreshCacheById( int64_t const& tenantId, TenantEntryCache<T>* const& cache, TenantEntryCacheRefreshReason const& reason ) {
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	return Future<Void>(new RefreshCacheByIdActor(tenantId, cache, reason));
															#line 662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
}

#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"

																#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
// This generated class is to be used only via refreshCacheByName()
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
template <class RefreshCacheByNameActor>
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class RefreshCacheByNameActorState {
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	RefreshCacheByNameActorState(TenantName const& name,TenantEntryCache<T>* const& cache,TenantEntryCacheRefreshReason const& reason) 
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		 : name(name),
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		   cache(cache),
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		   reason(reason)
															#line 684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
	{
		fdb_probe_actor_create("refreshCacheByName", reinterpret_cast<unsigned long>(this));

	}
	~RefreshCacheByNameActorState() 
	{
		fdb_probe_actor_destroy("refreshCacheByName", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			TraceEvent(SevDebug, "TenantEntryCacheNameRefreshStart", cache->id()) .detail("Reason", static_cast<int>(reason));
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr = cache->getDatabase()->createTransaction();
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			;
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
		this->~RefreshCacheByNameActorState();
		static_cast<RefreshCacheByNameActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		TraceEvent(SevDebug, "TenantEntryCacheNameRefreshEnd", cache->id()).detail("Reason", static_cast<int>(reason));
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (!static_cast<RefreshCacheByNameActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RefreshCacheByNameActorState(); static_cast<RefreshCacheByNameActor*>(this)->destroy(); return 0; }
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		new (&static_cast<RefreshCacheByNameActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RefreshCacheByNameActorState();
		static_cast<RefreshCacheByNameActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr->setOption(FDBTransactionOptions::READ_LOCK_AWARE);
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			StrictFuture<Optional<int64_t>> __when_expr_0 = TenantMetadata::tenantNameIndex().get(tr, name);
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (static_cast<RefreshCacheByNameActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RefreshCacheByNameActor*>(this)->actor_wait_state = 1;
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RefreshCacheByNameActor, 0, Optional<int64_t> >*>(static_cast<RefreshCacheByNameActor*>(this)));
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			StrictFuture<Void> __when_expr_2 = tr->onError(e);
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (static_cast<RefreshCacheByNameActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<RefreshCacheByNameActor*>(this)->actor_wait_state = 3;
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RefreshCacheByNameActor, 2, Void >*>(static_cast<RefreshCacheByNameActor*>(this)));
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (tenantId.present())
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			StrictFuture<Optional<TenantMapEntry>> __when_expr_1 = TenantMetadata::tenantMap().get(tr, tenantId.get());
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (static_cast<RefreshCacheByNameActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<RefreshCacheByNameActor*>(this)->actor_wait_state = 2;
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RefreshCacheByNameActor, 1, Optional<TenantMapEntry> >*>(static_cast<RefreshCacheByNameActor*>(this)));
															#line 827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<int64_t> const& __tenantId,int loopDepth) 
	{
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		tenantId = __tenantId;
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<int64_t> && __tenantId,int loopDepth) 
	{
		tenantId = std::move(__tenantId);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RefreshCacheByNameActor*>(this)->actor_wait_state > 0) static_cast<RefreshCacheByNameActor*>(this)->actor_wait_state = 0;
		static_cast<RefreshCacheByNameActor*>(this)->ActorCallback< RefreshCacheByNameActor, 0, Optional<int64_t> >::remove();

	}
	void a_callback_fire(ActorCallback< RefreshCacheByNameActor, 0, Optional<int64_t> >*,Optional<int64_t> const& value) 
	{
		fdb_probe_actor_enter("refreshCacheByName", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheByName", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RefreshCacheByNameActor, 0, Optional<int64_t> >*,Optional<int64_t> && value) 
	{
		fdb_probe_actor_enter("refreshCacheByName", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheByName", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RefreshCacheByNameActor, 0, Optional<int64_t> >*,Error err) 
	{
		fdb_probe_actor_enter("refreshCacheByName", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheByName", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont4(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (entry.present())
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		{
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			cache->put(entry.get());
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			updateCacheRefreshMetrics(cache, reason);
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (entry.present())
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		{
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			cache->put(entry.get());
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			updateCacheRefreshMetrics(cache, reason);
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(entry, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(entry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<RefreshCacheByNameActor*>(this)->actor_wait_state > 0) static_cast<RefreshCacheByNameActor*>(this)->actor_wait_state = 0;
		static_cast<RefreshCacheByNameActor*>(this)->ActorCallback< RefreshCacheByNameActor, 1, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< RefreshCacheByNameActor, 1, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("refreshCacheByName", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheByName", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RefreshCacheByNameActor, 1, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("refreshCacheByName", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheByName", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RefreshCacheByNameActor, 1, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("refreshCacheByName", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheByName", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<RefreshCacheByNameActor*>(this)->actor_wait_state > 0) static_cast<RefreshCacheByNameActor*>(this)->actor_wait_state = 0;
		static_cast<RefreshCacheByNameActor*>(this)->ActorCallback< RefreshCacheByNameActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RefreshCacheByNameActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("refreshCacheByName", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheByName", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< RefreshCacheByNameActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("refreshCacheByName", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheByName", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< RefreshCacheByNameActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("refreshCacheByName", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheByName", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	TenantName name;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	TenantEntryCache<T>* cache;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	TenantEntryCacheRefreshReason reason;
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	Reference<ReadYourWritesTransaction> tr;
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	Optional<int64_t> tenantId;
															#line 1090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
};
// This generated class is to be used only via refreshCacheByName()
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class RefreshCacheByNameActor final : public Actor<Void>, public ActorCallback< RefreshCacheByNameActor, 0, Optional<int64_t> >, public ActorCallback< RefreshCacheByNameActor, 1, Optional<TenantMapEntry> >, public ActorCallback< RefreshCacheByNameActor, 2, Void >, public FastAllocated<RefreshCacheByNameActor>, public RefreshCacheByNameActorState<RefreshCacheByNameActor> {
															#line 1095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
	using FastAllocated<RefreshCacheByNameActor>::operator new;
	using FastAllocated<RefreshCacheByNameActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RefreshCacheByNameActor, 0, Optional<int64_t> >;
friend struct ActorCallback< RefreshCacheByNameActor, 1, Optional<TenantMapEntry> >;
friend struct ActorCallback< RefreshCacheByNameActor, 2, Void >;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	RefreshCacheByNameActor(TenantName const& name,TenantEntryCache<T>* const& cache,TenantEntryCacheRefreshReason const& reason) 
															#line 1108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		 : Actor<Void>(),
		   RefreshCacheByNameActorState<RefreshCacheByNameActor>(name, cache, reason)
	{
		fdb_probe_actor_enter("refreshCacheByName", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("refreshCacheByName");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("refreshCacheByName", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RefreshCacheByNameActor, 0, Optional<int64_t> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RefreshCacheByNameActor, 1, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< RefreshCacheByNameActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
[[nodiscard]] static Future<Void> refreshCacheByName( TenantName const& name, TenantEntryCache<T>* const& cache, TenantEntryCacheRefreshReason const& reason ) {
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	return Future<Void>(new RefreshCacheByNameActor(name, cache, reason));
															#line 1137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
}

#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"

	static void updateCacheRefreshMetrics(TenantEntryCache<T>* cache, TenantEntryCacheRefreshReason reason) {
		if (reason == TenantEntryCacheRefreshReason::INIT) {
			cache->refreshByCacheInit += 1;
		} else if (reason == TenantEntryCacheRefreshReason::CACHE_MISS) {
			cache->refreshByCacheMiss += 1;
		} else if (reason == TenantEntryCacheRefreshReason::WATCH_TRIGGER) {
			cache->refreshByWatchTrigger += 1;
		}

		cache->numRefreshes += 1;
	}

																#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
// This generated class is to be used only via refreshCacheUsingWatch()
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
template <class RefreshCacheUsingWatchActor>
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class RefreshCacheUsingWatchActorState {
															#line 1160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	RefreshCacheUsingWatchActorState(TenantEntryCache<T>* const& cache,TenantEntryCacheRefreshReason const& reason) 
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		 : cache(cache),
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		   reason(reason)
															#line 1169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
	{
		fdb_probe_actor_create("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this));

	}
	~RefreshCacheUsingWatchActorState() 
	{
		fdb_probe_actor_destroy("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			TraceEvent(SevDebug, "TenantEntryCacheRefreshUsingWatchStart", cache->id()) .detail("Reason", static_cast<int>(reason));
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr = cache->getDatabase()->createTransaction();
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			;
															#line 1188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
		this->~RefreshCacheUsingWatchActorState();
		static_cast<RefreshCacheUsingWatchActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tenantModifiedWatch = TenantMetadata::lastTenantModification().watch(tr);
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			StrictFuture<Void> __when_expr_0 = tr->commit();
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state = 1;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RefreshCacheUsingWatchActor, 0, Void >*>(static_cast<RefreshCacheUsingWatchActor*>(this)));
															#line 1234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (e.code() != error_code_actor_cancelled)
															#line 1256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			{
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				TraceEvent("TenantEntryCacheRefreshUsingWatchError", cache->id()) .errorUnsuppressed(e) .suppressFor(1.0);
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			}
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			StrictFuture<Void> __when_expr_3 = tr->onError(e);
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state = 4;
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< RefreshCacheUsingWatchActor, 3, Void >*>(static_cast<RefreshCacheUsingWatchActor*>(this)));
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		TraceEvent(SevDebug, "TenantEntryCacheRefreshWatchSet", cache->id());
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (cache->setInitialWatch.canBeSet())
															#line 1288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		{
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			cache->setInitialWatch.send(Void());
															#line 1292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		}
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		StrictFuture<Void> __when_expr_1 = tenantModifiedWatch;
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state = 2;
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RefreshCacheUsingWatchActor, 1, Void >*>(static_cast<RefreshCacheUsingWatchActor*>(this)));
															#line 1303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		TraceEvent(SevDebug, "TenantEntryCacheRefreshWatchSet", cache->id());
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (cache->setInitialWatch.canBeSet())
															#line 1314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		{
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			cache->setInitialWatch.send(Void());
															#line 1318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		}
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		StrictFuture<Void> __when_expr_1 = tenantModifiedWatch;
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state = 2;
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RefreshCacheUsingWatchActor, 1, Void >*>(static_cast<RefreshCacheUsingWatchActor*>(this)));
															#line 1329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state > 0) static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state = 0;
		static_cast<RefreshCacheUsingWatchActor*>(this)->ActorCallback< RefreshCacheUsingWatchActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RefreshCacheUsingWatchActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RefreshCacheUsingWatchActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RefreshCacheUsingWatchActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		TraceEvent(SevDebug, "TenantEntryCacheRefreshUsingWatchTriggered", cache->id()) .detail("Reason", static_cast<int>(reason));
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		StrictFuture<Void> __when_expr_2 = refreshImpl(cache, reason);
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state = 3;
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RefreshCacheUsingWatchActor, 2, Void >*>(static_cast<RefreshCacheUsingWatchActor*>(this)));
															#line 1410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		TraceEvent(SevDebug, "TenantEntryCacheRefreshUsingWatchTriggered", cache->id()) .detail("Reason", static_cast<int>(reason));
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		StrictFuture<Void> __when_expr_2 = refreshImpl(cache, reason);
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state = 3;
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RefreshCacheUsingWatchActor, 2, Void >*>(static_cast<RefreshCacheUsingWatchActor*>(this)));
															#line 1428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = 0;

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
		if (static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state > 0) static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state = 0;
		static_cast<RefreshCacheUsingWatchActor*>(this)->ActorCallback< RefreshCacheUsingWatchActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RefreshCacheUsingWatchActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RefreshCacheUsingWatchActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RefreshCacheUsingWatchActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont5(Void const& _,int loopDepth) 
	{
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		tr->reset();
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5(Void && _,int loopDepth) 
	{
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		tr->reset();
															#line 1509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state > 0) static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state = 0;
		static_cast<RefreshCacheUsingWatchActor*>(this)->ActorCallback< RefreshCacheUsingWatchActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RefreshCacheUsingWatchActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< RefreshCacheUsingWatchActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< RefreshCacheUsingWatchActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont7(int loopDepth) 
	{
		try {
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		StrictFuture<Void> __when_expr_4 = refreshImpl(cache, reason);
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1cont1when1(__when_expr_4.get(), loopDepth); };
		static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state = 5;
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< RefreshCacheUsingWatchActor, 4, Void >*>(static_cast<RefreshCacheUsingWatchActor*>(this)));
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		StrictFuture<Void> __when_expr_4 = refreshImpl(cache, reason);
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1cont1when1(__when_expr_4.get(), loopDepth); };
		static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state = 5;
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< RefreshCacheUsingWatchActor, 4, Void >*>(static_cast<RefreshCacheUsingWatchActor*>(this)));
															#line 1617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = 0;

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
	void a_exitChoose4() 
	{
		if (static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state > 0) static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state = 0;
		static_cast<RefreshCacheUsingWatchActor*>(this)->ActorCallback< RefreshCacheUsingWatchActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RefreshCacheUsingWatchActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< RefreshCacheUsingWatchActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< RefreshCacheUsingWatchActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1Catch1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state > 0) static_cast<RefreshCacheUsingWatchActor*>(this)->actor_wait_state = 0;
		static_cast<RefreshCacheUsingWatchActor*>(this)->ActorCallback< RefreshCacheUsingWatchActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RefreshCacheUsingWatchActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< RefreshCacheUsingWatchActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< RefreshCacheUsingWatchActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	TenantEntryCache<T>* cache;
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	TenantEntryCacheRefreshReason reason;
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	Reference<ReadYourWritesTransaction> tr;
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	Future<Void> tenantModifiedWatch;
															#line 1768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
};
// This generated class is to be used only via refreshCacheUsingWatch()
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class RefreshCacheUsingWatchActor final : public Actor<Void>, public ActorCallback< RefreshCacheUsingWatchActor, 0, Void >, public ActorCallback< RefreshCacheUsingWatchActor, 1, Void >, public ActorCallback< RefreshCacheUsingWatchActor, 2, Void >, public ActorCallback< RefreshCacheUsingWatchActor, 3, Void >, public ActorCallback< RefreshCacheUsingWatchActor, 4, Void >, public FastAllocated<RefreshCacheUsingWatchActor>, public RefreshCacheUsingWatchActorState<RefreshCacheUsingWatchActor> {
															#line 1773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
	using FastAllocated<RefreshCacheUsingWatchActor>::operator new;
	using FastAllocated<RefreshCacheUsingWatchActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RefreshCacheUsingWatchActor, 0, Void >;
friend struct ActorCallback< RefreshCacheUsingWatchActor, 1, Void >;
friend struct ActorCallback< RefreshCacheUsingWatchActor, 2, Void >;
friend struct ActorCallback< RefreshCacheUsingWatchActor, 3, Void >;
friend struct ActorCallback< RefreshCacheUsingWatchActor, 4, Void >;
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	RefreshCacheUsingWatchActor(TenantEntryCache<T>* const& cache,TenantEntryCacheRefreshReason const& reason) 
															#line 1788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		 : Actor<Void>(),
		   RefreshCacheUsingWatchActorState<RefreshCacheUsingWatchActor>(cache, reason)
	{
		fdb_probe_actor_enter("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("refreshCacheUsingWatch");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("refreshCacheUsingWatch", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RefreshCacheUsingWatchActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RefreshCacheUsingWatchActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< RefreshCacheUsingWatchActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< RefreshCacheUsingWatchActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< RefreshCacheUsingWatchActor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
[[nodiscard]] static Future<Void> refreshCacheUsingWatch( TenantEntryCache<T>* const& cache, TenantEntryCacheRefreshReason const& reason ) {
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	return Future<Void>(new RefreshCacheUsingWatchActor(cache, reason));
															#line 1819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
}

#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"

	static bool tenantsEnabled(TenantEntryCache<T>* cache) {
		// Avoid using the cache if the tenant mode is disabled. However since we use clientInfo, sometimes it may not
		// be fully up to date (i.e it may indicate the tenantMode is disabled when in fact it is required). Thus if
		// there is at least one tenant that has been created on the cluster then use the cache to avoid an incorrect
		// miss.
		if (cache->getDatabase()->clientInfo->get().tenantMode == TenantMode::DISABLED) {
			if (!cache->lastTenantId.present()) {
				return false;
			}
			return cache->lastTenantId.get() >= 0;
		}
		return true;
	}

																#line 1838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
// This generated class is to be used only via setLastTenantId()
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
template <class SetLastTenantIdActor>
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class SetLastTenantIdActorState {
															#line 1844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	SetLastTenantIdActorState(TenantEntryCache<T>* const& cache) 
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		 : cache(cache),
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		   tr(cache->getDatabase()->createTransaction())
															#line 1853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
	{
		fdb_probe_actor_create("setLastTenantId", reinterpret_cast<unsigned long>(this));

	}
	~SetLastTenantIdActorState() 
	{
		fdb_probe_actor_destroy("setLastTenantId", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			;
															#line 1868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
		this->~SetLastTenantIdActorState();
		static_cast<SetLastTenantIdActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr->setOption(FDBTransactionOptions::READ_LOCK_AWARE);
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			StrictFuture<Optional<int64_t>> __when_expr_0 = TenantMetadata::lastTenantId().get(tr);
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (static_cast<SetLastTenantIdActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SetLastTenantIdActor*>(this)->actor_wait_state = 1;
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SetLastTenantIdActor, 0, Optional<int64_t> >*>(static_cast<SetLastTenantIdActor*>(this)));
															#line 1912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			StrictFuture<Void> __when_expr_1 = tr->onError(e);
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (static_cast<SetLastTenantIdActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<SetLastTenantIdActor*>(this)->actor_wait_state = 2;
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SetLastTenantIdActor, 1, Void >*>(static_cast<SetLastTenantIdActor*>(this)));
															#line 1941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<int64_t> const& lastTenantId,int loopDepth) 
	{
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		cache->lastTenantId = lastTenantId;
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (!static_cast<SetLastTenantIdActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SetLastTenantIdActorState(); static_cast<SetLastTenantIdActor*>(this)->destroy(); return 0; }
															#line 1958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		new (&static_cast<SetLastTenantIdActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SetLastTenantIdActorState();
		static_cast<SetLastTenantIdActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<int64_t> && lastTenantId,int loopDepth) 
	{
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		cache->lastTenantId = lastTenantId;
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (!static_cast<SetLastTenantIdActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SetLastTenantIdActorState(); static_cast<SetLastTenantIdActor*>(this)->destroy(); return 0; }
															#line 1972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		new (&static_cast<SetLastTenantIdActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SetLastTenantIdActorState();
		static_cast<SetLastTenantIdActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<int64_t> const& lastTenantId,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(lastTenantId, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<int64_t> && lastTenantId,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(lastTenantId), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SetLastTenantIdActor*>(this)->actor_wait_state > 0) static_cast<SetLastTenantIdActor*>(this)->actor_wait_state = 0;
		static_cast<SetLastTenantIdActor*>(this)->ActorCallback< SetLastTenantIdActor, 0, Optional<int64_t> >::remove();

	}
	void a_callback_fire(ActorCallback< SetLastTenantIdActor, 0, Optional<int64_t> >*,Optional<int64_t> const& value) 
	{
		fdb_probe_actor_enter("setLastTenantId", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setLastTenantId", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SetLastTenantIdActor, 0, Optional<int64_t> >*,Optional<int64_t> && value) 
	{
		fdb_probe_actor_enter("setLastTenantId", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setLastTenantId", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SetLastTenantIdActor, 0, Optional<int64_t> >*,Error err) 
	{
		fdb_probe_actor_enter("setLastTenantId", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setLastTenantId", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<SetLastTenantIdActor*>(this)->actor_wait_state > 0) static_cast<SetLastTenantIdActor*>(this)->actor_wait_state = 0;
		static_cast<SetLastTenantIdActor*>(this)->ActorCallback< SetLastTenantIdActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SetLastTenantIdActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("setLastTenantId", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setLastTenantId", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SetLastTenantIdActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("setLastTenantId", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setLastTenantId", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SetLastTenantIdActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("setLastTenantId", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setLastTenantId", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	TenantEntryCache<T>* cache;
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	Reference<ReadYourWritesTransaction> tr;
															#line 2122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
};
// This generated class is to be used only via setLastTenantId()
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class SetLastTenantIdActor final : public Actor<Void>, public ActorCallback< SetLastTenantIdActor, 0, Optional<int64_t> >, public ActorCallback< SetLastTenantIdActor, 1, Void >, public FastAllocated<SetLastTenantIdActor>, public SetLastTenantIdActorState<SetLastTenantIdActor> {
															#line 2127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
	using FastAllocated<SetLastTenantIdActor>::operator new;
	using FastAllocated<SetLastTenantIdActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SetLastTenantIdActor, 0, Optional<int64_t> >;
friend struct ActorCallback< SetLastTenantIdActor, 1, Void >;
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	SetLastTenantIdActor(TenantEntryCache<T>* const& cache) 
															#line 2139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		 : Actor<Void>(),
		   SetLastTenantIdActorState<SetLastTenantIdActor>(cache)
	{
		fdb_probe_actor_enter("setLastTenantId", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("setLastTenantId");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("setLastTenantId", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SetLastTenantIdActor, 0, Optional<int64_t> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SetLastTenantIdActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
[[nodiscard]] static Future<Void> setLastTenantId( TenantEntryCache<T>* const& cache ) {
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	return Future<Void>(new SetLastTenantIdActor(cache));
															#line 2167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
}

#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"

																#line 2172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
// This generated class is to be used only via lastTenantIdWatch()
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
template <class LastTenantIdWatchActor>
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class LastTenantIdWatchActorState {
															#line 2178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	LastTenantIdWatchActorState(TenantEntryCache<T>* const& cache) 
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		 : cache(cache)
															#line 2185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
	{
		fdb_probe_actor_create("lastTenantIdWatch", reinterpret_cast<unsigned long>(this));

	}
	~LastTenantIdWatchActorState() 
	{
		fdb_probe_actor_destroy("lastTenantIdWatch", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			TraceEvent(SevDebug, "TenantEntryCacheLastTenantIdWatchStart", cache->id());
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr = cache->getDatabase()->createTransaction();
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			;
															#line 2204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
		this->~LastTenantIdWatchActorState();
		static_cast<LastTenantIdWatchActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			lastTenantIdWatch = tr->watch(TenantMetadata::lastTenantId().key);
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			StrictFuture<Void> __when_expr_0 = tr->commit();
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state = 1;
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< LastTenantIdWatchActor, 0, Void >*>(static_cast<LastTenantIdWatchActor*>(this)));
															#line 2250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			err = Error(e);
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (err.code() != error_code_actor_cancelled)
															#line 2274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			{
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				TraceEvent("TenantEntryCacheLastTenantIdWatchError", cache->id()) .errorUnsuppressed(err) .suppressFor(1.0);
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				StrictFuture<Void> __when_expr_3 = setLastTenantId(cache);
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				if (static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
				if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
				static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state = 4;
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< LastTenantIdWatchActor, 3, Void >*>(static_cast<LastTenantIdWatchActor*>(this)));
															#line 2287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1Catch1cont1(loopDepth);
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		StrictFuture<Void> __when_expr_1 = lastTenantIdWatch;
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state = 2;
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< LastTenantIdWatchActor, 1, Void >*>(static_cast<LastTenantIdWatchActor*>(this)));
															#line 2314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		StrictFuture<Void> __when_expr_1 = lastTenantIdWatch;
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state = 2;
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< LastTenantIdWatchActor, 1, Void >*>(static_cast<LastTenantIdWatchActor*>(this)));
															#line 2330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state > 0) static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state = 0;
		static_cast<LastTenantIdWatchActor*>(this)->ActorCallback< LastTenantIdWatchActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LastTenantIdWatchActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< LastTenantIdWatchActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< LastTenantIdWatchActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		StrictFuture<Void> __when_expr_2 = setLastTenantId(cache);
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state = 3;
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< LastTenantIdWatchActor, 2, Void >*>(static_cast<LastTenantIdWatchActor*>(this)));
															#line 2409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		StrictFuture<Void> __when_expr_2 = setLastTenantId(cache);
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state = 3;
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< LastTenantIdWatchActor, 2, Void >*>(static_cast<LastTenantIdWatchActor*>(this)));
															#line 2425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = 0;

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
		if (static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state > 0) static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state = 0;
		static_cast<LastTenantIdWatchActor*>(this)->ActorCallback< LastTenantIdWatchActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LastTenantIdWatchActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< LastTenantIdWatchActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< LastTenantIdWatchActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		tr->reset();
															#line 2497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		tr->reset();
															#line 2506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state > 0) static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state = 0;
		static_cast<LastTenantIdWatchActor*>(this)->ActorCallback< LastTenantIdWatchActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LastTenantIdWatchActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< LastTenantIdWatchActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< LastTenantIdWatchActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont6(int loopDepth) 
	{
		try {
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(int loopDepth) 
	{
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		StrictFuture<Void> __when_expr_4 = tr->onError(err);
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1cont1when1(__when_expr_4.get(), loopDepth); };
		static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state = 5;
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< LastTenantIdWatchActor, 4, Void >*>(static_cast<LastTenantIdWatchActor*>(this)));
															#line 2598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state > 0) static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state = 0;
		static_cast<LastTenantIdWatchActor*>(this)->ActorCallback< LastTenantIdWatchActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LastTenantIdWatchActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< LastTenantIdWatchActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< LastTenantIdWatchActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1Catch1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state > 0) static_cast<LastTenantIdWatchActor*>(this)->actor_wait_state = 0;
		static_cast<LastTenantIdWatchActor*>(this)->ActorCallback< LastTenantIdWatchActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LastTenantIdWatchActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< LastTenantIdWatchActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< LastTenantIdWatchActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	TenantEntryCache<T>* cache;
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	Reference<ReadYourWritesTransaction> tr;
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	Future<Void> lastTenantIdWatch;
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	Error err;
															#line 2761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
};
// This generated class is to be used only via lastTenantIdWatch()
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class LastTenantIdWatchActor final : public Actor<Void>, public ActorCallback< LastTenantIdWatchActor, 0, Void >, public ActorCallback< LastTenantIdWatchActor, 1, Void >, public ActorCallback< LastTenantIdWatchActor, 2, Void >, public ActorCallback< LastTenantIdWatchActor, 3, Void >, public ActorCallback< LastTenantIdWatchActor, 4, Void >, public FastAllocated<LastTenantIdWatchActor>, public LastTenantIdWatchActorState<LastTenantIdWatchActor> {
															#line 2766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
	using FastAllocated<LastTenantIdWatchActor>::operator new;
	using FastAllocated<LastTenantIdWatchActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< LastTenantIdWatchActor, 0, Void >;
friend struct ActorCallback< LastTenantIdWatchActor, 1, Void >;
friend struct ActorCallback< LastTenantIdWatchActor, 2, Void >;
friend struct ActorCallback< LastTenantIdWatchActor, 3, Void >;
friend struct ActorCallback< LastTenantIdWatchActor, 4, Void >;
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	LastTenantIdWatchActor(TenantEntryCache<T>* const& cache) 
															#line 2781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		 : Actor<Void>(),
		   LastTenantIdWatchActorState<LastTenantIdWatchActor>(cache)
	{
		fdb_probe_actor_enter("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("lastTenantIdWatch");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("lastTenantIdWatch", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< LastTenantIdWatchActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< LastTenantIdWatchActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< LastTenantIdWatchActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< LastTenantIdWatchActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< LastTenantIdWatchActor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
[[nodiscard]] static Future<Void> lastTenantIdWatch( TenantEntryCache<T>* const& cache ) {
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	return Future<Void>(new LastTenantIdWatchActor(cache));
															#line 2812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
}

#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"

																#line 2817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
// This generated class is to be used only via refreshImpl()
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
template <class RefreshImplActor>
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class RefreshImplActorState {
															#line 2823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	RefreshImplActorState(TenantEntryCache<T>* const& cache,TenantEntryCacheRefreshReason const& reason) 
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		 : cache(cache),
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		   reason(reason)
															#line 2832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
	{
		fdb_probe_actor_create("refreshImpl", reinterpret_cast<unsigned long>(this));

	}
	~RefreshImplActorState() 
	{
		fdb_probe_actor_destroy("refreshImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			TraceEvent(SevDebug, "TenantEntryCacheRefreshStart", cache->id()).detail("Reason", static_cast<int>(reason));
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			tr = cache->getDatabase()->createTransaction();
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			;
															#line 2851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
		this->~RefreshImplActorState();
		static_cast<RefreshImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		TraceEvent(SevDebug, "TenantEntryCacheRefreshEnd", cache->id()).detail("Reason", static_cast<int>(reason));
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (!static_cast<RefreshImplActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RefreshImplActorState(); static_cast<RefreshImplActor*>(this)->destroy(); return 0; }
															#line 2876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		new (&static_cast<RefreshImplActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RefreshImplActorState();
		static_cast<RefreshImplActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			StrictFuture<std::vector<std::pair<int64_t, TenantMapEntry>>> __when_expr_0 = getTenantList(tr);
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (static_cast<RefreshImplActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RefreshImplActor*>(this)->actor_wait_state = 1;
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RefreshImplActor, 0, std::vector<std::pair<int64_t, TenantMapEntry>> >*>(static_cast<RefreshImplActor*>(this)));
															#line 2903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (e.code() != error_code_actor_cancelled)
															#line 2938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			{
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				TraceEvent("TenantEntryCacheRefreshError", cache->id()).errorUnsuppressed(e).suppressFor(1.0);
															#line 2942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			}
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			StrictFuture<Void> __when_expr_1 = tr->onError(e);
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (static_cast<RefreshImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<RefreshImplActor*>(this)->actor_wait_state = 2;
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RefreshImplActor, 1, Void >*>(static_cast<RefreshImplActor*>(this)));
															#line 2953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
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
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		cache->clear();
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		for( auto& tenant : tenantList ) {
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			cache->put(tenant.second);
															#line 2972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		}
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		updateCacheRefreshMetrics(cache, reason);
															#line 2976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<std::pair<int64_t, TenantMapEntry>> const& __tenantList,int loopDepth) 
	{
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		tenantList = __tenantList;
															#line 2985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<std::pair<int64_t, TenantMapEntry>> && __tenantList,int loopDepth) 
	{
		tenantList = std::move(__tenantList);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RefreshImplActor*>(this)->actor_wait_state > 0) static_cast<RefreshImplActor*>(this)->actor_wait_state = 0;
		static_cast<RefreshImplActor*>(this)->ActorCallback< RefreshImplActor, 0, std::vector<std::pair<int64_t, TenantMapEntry>> >::remove();

	}
	void a_callback_fire(ActorCallback< RefreshImplActor, 0, std::vector<std::pair<int64_t, TenantMapEntry>> >*,std::vector<std::pair<int64_t, TenantMapEntry>> const& value) 
	{
		fdb_probe_actor_enter("refreshImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RefreshImplActor, 0, std::vector<std::pair<int64_t, TenantMapEntry>> >*,std::vector<std::pair<int64_t, TenantMapEntry>> && value) 
	{
		fdb_probe_actor_enter("refreshImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RefreshImplActor, 0, std::vector<std::pair<int64_t, TenantMapEntry>> >*,Error err) 
	{
		fdb_probe_actor_enter("refreshImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshImpl", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<RefreshImplActor*>(this)->actor_wait_state > 0) static_cast<RefreshImplActor*>(this)->actor_wait_state = 0;
		static_cast<RefreshImplActor*>(this)->ActorCallback< RefreshImplActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RefreshImplActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("refreshImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RefreshImplActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("refreshImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RefreshImplActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("refreshImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refreshImpl", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	TenantEntryCache<T>* cache;
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	TenantEntryCacheRefreshReason reason;
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	Reference<ReadYourWritesTransaction> tr;
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	std::vector<std::pair<int64_t, TenantMapEntry>> tenantList;
															#line 3131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
};
// This generated class is to be used only via refreshImpl()
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class RefreshImplActor final : public Actor<Void>, public ActorCallback< RefreshImplActor, 0, std::vector<std::pair<int64_t, TenantMapEntry>> >, public ActorCallback< RefreshImplActor, 1, Void >, public FastAllocated<RefreshImplActor>, public RefreshImplActorState<RefreshImplActor> {
															#line 3136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
	using FastAllocated<RefreshImplActor>::operator new;
	using FastAllocated<RefreshImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RefreshImplActor, 0, std::vector<std::pair<int64_t, TenantMapEntry>> >;
friend struct ActorCallback< RefreshImplActor, 1, Void >;
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	RefreshImplActor(TenantEntryCache<T>* const& cache,TenantEntryCacheRefreshReason const& reason) 
															#line 3148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		 : Actor<Void>(),
		   RefreshImplActorState<RefreshImplActor>(cache, reason)
	{
		fdb_probe_actor_enter("refreshImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("refreshImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("refreshImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RefreshImplActor, 0, std::vector<std::pair<int64_t, TenantMapEntry>> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RefreshImplActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
[[nodiscard]] static Future<Void> refreshImpl( TenantEntryCache<T>* const& cache, TenantEntryCacheRefreshReason const& reason ) {
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	return Future<Void>(new RefreshImplActor(cache, reason));
															#line 3176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
}

#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"

																#line 3181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
// This generated class is to be used only via getByIdImpl()
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
template <class GetByIdImplActor>
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class GetByIdImplActorState {
															#line 3187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	GetByIdImplActorState(TenantEntryCache<T>* const& cache,int64_t const& tenantId) 
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		 : cache(cache),
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		   tenantId(tenantId)
															#line 3196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
	{
		fdb_probe_actor_create("getByIdImpl", reinterpret_cast<unsigned long>(this));

	}
	~GetByIdImplActorState() 
	{
		fdb_probe_actor_destroy("getByIdImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			Optional<TenantEntryCachePayload<T>> ret = cache->lookupById(tenantId);
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (ret.present())
															#line 3213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			{
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				cache->hits += 1;
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				if (!static_cast<GetByIdImplActor*>(this)->SAV<Optional<TenantEntryCachePayload<T>>>::futures) { (void)(ret); this->~GetByIdImplActorState(); static_cast<GetByIdImplActor*>(this)->destroy(); return 0; }
															#line 3219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
				new (&static_cast<GetByIdImplActor*>(this)->SAV< Optional<TenantEntryCachePayload<T>> >::value()) Optional<TenantEntryCachePayload<T>>(ret);
				this->~GetByIdImplActorState();
				static_cast<GetByIdImplActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (!tenantsEnabled(cache))
															#line 3227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			{
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				if (!static_cast<GetByIdImplActor*>(this)->SAV<Optional<TenantEntryCachePayload<T>>>::futures) { (void)(Optional<TenantEntryCachePayload<T>>()); this->~GetByIdImplActorState(); static_cast<GetByIdImplActor*>(this)->destroy(); return 0; }
															#line 3231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
				new (&static_cast<GetByIdImplActor*>(this)->SAV< Optional<TenantEntryCachePayload<T>> >::value()) Optional<TenantEntryCachePayload<T>>(Optional<TenantEntryCachePayload<T>>());
				this->~GetByIdImplActorState();
				static_cast<GetByIdImplActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			TraceEvent("TenantEntryCacheGetByIdRefresh").detail("TenantId", tenantId);
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (cache->refreshMode == TenantEntryCacheRefreshMode::WATCH)
															#line 3241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			{
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				StrictFuture<Void> __when_expr_0 = refreshCacheById(tenantId, cache, TenantEntryCacheRefreshReason::CACHE_MISS);
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				if (static_cast<GetByIdImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetByIdImplActor*>(this)->actor_wait_state = 1;
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetByIdImplActor, 0, Void >*>(static_cast<GetByIdImplActor*>(this)));
															#line 3252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
				loopDepth = 0;
			}
			else
			{
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				StrictFuture<Void> __when_expr_1 = refreshImpl(cache, TenantEntryCacheRefreshReason::CACHE_MISS);
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				if (static_cast<GetByIdImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
				static_cast<GetByIdImplActor*>(this)->actor_wait_state = 2;
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetByIdImplActor, 1, Void >*>(static_cast<GetByIdImplActor*>(this)));
															#line 3266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
				loopDepth = 0;
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
		this->~GetByIdImplActorState();
		static_cast<GetByIdImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		cache->misses += 1;
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (!static_cast<GetByIdImplActor*>(this)->SAV<Optional<TenantEntryCachePayload<T>>>::futures) { (void)(cache->lookupById(tenantId)); this->~GetByIdImplActorState(); static_cast<GetByIdImplActor*>(this)->destroy(); return 0; }
															#line 3292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		new (&static_cast<GetByIdImplActor*>(this)->SAV< Optional<TenantEntryCachePayload<T>> >::value()) Optional<TenantEntryCachePayload<T>>(cache->lookupById(tenantId));
		this->~GetByIdImplActorState();
		static_cast<GetByIdImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetByIdImplActor*>(this)->actor_wait_state > 0) static_cast<GetByIdImplActor*>(this)->actor_wait_state = 0;
		static_cast<GetByIdImplActor*>(this)->ActorCallback< GetByIdImplActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetByIdImplActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getByIdImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getByIdImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetByIdImplActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getByIdImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getByIdImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetByIdImplActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getByIdImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getByIdImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetByIdImplActor*>(this)->actor_wait_state > 0) static_cast<GetByIdImplActor*>(this)->actor_wait_state = 0;
		static_cast<GetByIdImplActor*>(this)->ActorCallback< GetByIdImplActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetByIdImplActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getByIdImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getByIdImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetByIdImplActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getByIdImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getByIdImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetByIdImplActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getByIdImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getByIdImpl", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	TenantEntryCache<T>* cache;
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	int64_t tenantId;
															#line 3454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
};
// This generated class is to be used only via getByIdImpl()
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class GetByIdImplActor final : public Actor<Optional<TenantEntryCachePayload<T>>>, public ActorCallback< GetByIdImplActor, 0, Void >, public ActorCallback< GetByIdImplActor, 1, Void >, public FastAllocated<GetByIdImplActor>, public GetByIdImplActorState<GetByIdImplActor> {
															#line 3459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
	using FastAllocated<GetByIdImplActor>::operator new;
	using FastAllocated<GetByIdImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<TenantEntryCachePayload<T>>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetByIdImplActor, 0, Void >;
friend struct ActorCallback< GetByIdImplActor, 1, Void >;
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	GetByIdImplActor(TenantEntryCache<T>* const& cache,int64_t const& tenantId) 
															#line 3471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		 : Actor<Optional<TenantEntryCachePayload<T>>>(),
		   GetByIdImplActorState<GetByIdImplActor>(cache, tenantId)
	{
		fdb_probe_actor_enter("getByIdImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getByIdImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getByIdImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetByIdImplActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetByIdImplActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
[[nodiscard]] static Future<Optional<TenantEntryCachePayload<T>>> getByIdImpl( TenantEntryCache<T>* const& cache, int64_t const& tenantId ) {
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	return Future<Optional<TenantEntryCachePayload<T>>>(new GetByIdImplActor(cache, tenantId));
															#line 3499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
}

#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"

																#line 3504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
// This generated class is to be used only via getByNameImpl()
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
template <class GetByNameImplActor>
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class GetByNameImplActorState {
															#line 3510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	GetByNameImplActorState(TenantEntryCache<T>* const& cache,TenantName const& name) 
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		 : cache(cache),
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		   name(name)
															#line 3519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
	{
		fdb_probe_actor_create("getByNameImpl", reinterpret_cast<unsigned long>(this));

	}
	~GetByNameImplActorState() 
	{
		fdb_probe_actor_destroy("getByNameImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			Optional<TenantEntryCachePayload<T>> ret = cache->lookupByName(name);
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (ret.present())
															#line 3536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			{
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				cache->hits += 1;
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				if (!static_cast<GetByNameImplActor*>(this)->SAV<Optional<TenantEntryCachePayload<T>>>::futures) { (void)(ret); this->~GetByNameImplActorState(); static_cast<GetByNameImplActor*>(this)->destroy(); return 0; }
															#line 3542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
				new (&static_cast<GetByNameImplActor*>(this)->SAV< Optional<TenantEntryCachePayload<T>> >::value()) Optional<TenantEntryCachePayload<T>>(ret);
				this->~GetByNameImplActorState();
				static_cast<GetByNameImplActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (!tenantsEnabled(cache))
															#line 3550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			{
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				if (!static_cast<GetByNameImplActor*>(this)->SAV<Optional<TenantEntryCachePayload<T>>>::futures) { (void)(Optional<TenantEntryCachePayload<T>>()); this->~GetByNameImplActorState(); static_cast<GetByNameImplActor*>(this)->destroy(); return 0; }
															#line 3554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
				new (&static_cast<GetByNameImplActor*>(this)->SAV< Optional<TenantEntryCachePayload<T>> >::value()) Optional<TenantEntryCachePayload<T>>(Optional<TenantEntryCachePayload<T>>());
				this->~GetByNameImplActorState();
				static_cast<GetByNameImplActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			TraceEvent("TenantEntryCacheGetByNameRefresh").detail("TenantName", name);
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
			if (cache->refreshMode == TenantEntryCacheRefreshMode::WATCH)
															#line 3564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
			{
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				StrictFuture<Void> __when_expr_0 = refreshCacheByName(name, cache, TenantEntryCacheRefreshReason::CACHE_MISS);
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				if (static_cast<GetByNameImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetByNameImplActor*>(this)->actor_wait_state = 1;
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetByNameImplActor, 0, Void >*>(static_cast<GetByNameImplActor*>(this)));
															#line 3575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
				loopDepth = 0;
			}
			else
			{
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				StrictFuture<Void> __when_expr_1 = refreshImpl(cache, TenantEntryCacheRefreshReason::CACHE_MISS);
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				if (static_cast<GetByNameImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
				static_cast<GetByNameImplActor*>(this)->actor_wait_state = 2;
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetByNameImplActor, 1, Void >*>(static_cast<GetByNameImplActor*>(this)));
															#line 3589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
				loopDepth = 0;
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
		this->~GetByNameImplActorState();
		static_cast<GetByNameImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		cache->misses += 1;
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
		if (!static_cast<GetByNameImplActor*>(this)->SAV<Optional<TenantEntryCachePayload<T>>>::futures) { (void)(cache->lookupByName(name)); this->~GetByNameImplActorState(); static_cast<GetByNameImplActor*>(this)->destroy(); return 0; }
															#line 3615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		new (&static_cast<GetByNameImplActor*>(this)->SAV< Optional<TenantEntryCachePayload<T>> >::value()) Optional<TenantEntryCachePayload<T>>(cache->lookupByName(name));
		this->~GetByNameImplActorState();
		static_cast<GetByNameImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetByNameImplActor*>(this)->actor_wait_state > 0) static_cast<GetByNameImplActor*>(this)->actor_wait_state = 0;
		static_cast<GetByNameImplActor*>(this)->ActorCallback< GetByNameImplActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetByNameImplActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getByNameImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getByNameImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetByNameImplActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getByNameImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getByNameImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetByNameImplActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getByNameImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getByNameImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetByNameImplActor*>(this)->actor_wait_state > 0) static_cast<GetByNameImplActor*>(this)->actor_wait_state = 0;
		static_cast<GetByNameImplActor*>(this)->ActorCallback< GetByNameImplActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetByNameImplActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getByNameImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getByNameImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetByNameImplActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getByNameImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getByNameImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetByNameImplActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getByNameImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getByNameImpl", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	TenantEntryCache<T>* cache;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	TenantName name;
															#line 3777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
};
// This generated class is to be used only via getByNameImpl()
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
class GetByNameImplActor final : public Actor<Optional<TenantEntryCachePayload<T>>>, public ActorCallback< GetByNameImplActor, 0, Void >, public ActorCallback< GetByNameImplActor, 1, Void >, public FastAllocated<GetByNameImplActor>, public GetByNameImplActorState<GetByNameImplActor> {
															#line 3782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
public:
	using FastAllocated<GetByNameImplActor>::operator new;
	using FastAllocated<GetByNameImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<TenantEntryCachePayload<T>>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetByNameImplActor, 0, Void >;
friend struct ActorCallback< GetByNameImplActor, 1, Void >;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	GetByNameImplActor(TenantEntryCache<T>* const& cache,TenantName const& name) 
															#line 3794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
		 : Actor<Optional<TenantEntryCachePayload<T>>>(),
		   GetByNameImplActorState<GetByNameImplActor>(cache, name)
	{
		fdb_probe_actor_enter("getByNameImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getByNameImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getByNameImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetByNameImplActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetByNameImplActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
[[nodiscard]] static Future<Optional<TenantEntryCachePayload<T>>> getByNameImpl( TenantEntryCache<T>* const& cache, TenantName const& name ) {
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"
	return Future<Optional<TenantEntryCachePayload<T>>>(new GetByNameImplActor(cache, name));
															#line 3822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.g.h"
}

#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantEntryCache.actor.h"

	Optional<TenantEntryCachePayload<T>> lookupById(int64_t tenantId) {
		Optional<TenantEntryCachePayload<T>> ret;
		auto itr = mapByTenantId.find(tenantId);
		if (itr == mapByTenantId.end()) {
			return ret;
		}

		return itr->value;
	}

	Optional<TenantEntryCachePayload<T>> lookupByName(TenantName name) {
		Optional<TenantEntryCachePayload<T>> ret;
		auto itr = mapByTenantName.find(name);
		if (itr == mapByTenantName.end()) {
			return ret;
		}

		return itr->value;
	}

	Future<Void> refresh(TenantEntryCacheRefreshReason reason) { return refreshImpl(this, reason); }

	static TenantEntryCachePayload<Void> defaultCreatePayload(const TenantMapEntry& entry) {
		TenantEntryCachePayload<Void> payload;
		payload.entry = entry;

		return payload;
	}

	Future<Void> removeEntryInt(Optional<int64_t> tenantId,
	                            Optional<KeyRef> tenantPrefix,
	                            Optional<TenantName> tenantName,
	                            bool refreshCache) {
		typename Map<int64_t, TenantEntryCachePayload<T>>::iterator itrId;
		typename Map<TenantName, TenantEntryCachePayload<T>>::iterator itrName;

		if (tenantId.present() || tenantPrefix.present()) {
			// Ensure either tenantId OR tenantPrefix is valid (but not both)
			ASSERT(tenantId.present() != tenantPrefix.present());
			ASSERT(!tenantName.present());

			int64_t tId = tenantId.present() ? tenantId.get() : TenantAPI::prefixToId(tenantPrefix.get());
			TraceEvent("TenantEntryCacheRemoveEntry").detail("Id", tId);
			itrId = mapByTenantId.find(tId);
			if (itrId == mapByTenantId.end()) {
				return Void();
			}
			// Ensure byId and byName cache are in-sync
			itrName = mapByTenantName.find(itrId->value.entry.tenantName);
			ASSERT(itrName != mapByTenantName.end());
		} else if (tenantName.present()) {
			ASSERT(!tenantId.present() && !tenantPrefix.present());

			TraceEvent("TenantEntryCacheRemoveEntry").detail("Name", tenantName.get());
			itrName = mapByTenantName.find(tenantName.get());
			if (itrName == mapByTenantName.end()) {
				return Void();
			}
			// Ensure byId and byName cache are in-sync
			itrId = mapByTenantId.find(itrName->value.entry.id);
			ASSERT(itrId != mapByTenantId.end());
		} else {
			// Invalid input, one of: tenantId, tenantPrefix or tenantName needs to be valid.
			throw operation_failed();
		}

		ASSERT(itrId != mapByTenantId.end() && itrName != mapByTenantName.end());

		TraceEvent("TenantEntryCacheRemoveEntry")
		    .detail("Id", itrId->key)
		    .detail("Prefix", itrId->value.entry.prefix)
		    .detail("Name", itrName->key);

		mapByTenantId.erase(itrId);
		mapByTenantName.erase(itrName);

		if (refreshCache) {
			return refreshImpl(this, TenantEntryCacheRefreshReason::REMOVE_ENTRY);
		}

		return Void();
	}

public:
	TenantEntryCache(Database db)
	  : uid(deterministicRandom()->randomUniqueID()), db(db), createPayloadFunc(defaultCreatePayload),
	    refreshMode(TenantEntryCacheRefreshMode::PERIODIC_TASK), metrics("TenantEntryCacheMetrics", uid.toString()),
	    hits("TenantEntryCacheHits", metrics), misses("TenantEntryCacheMisses", metrics),
	    refreshByCacheInit("TenantEntryCacheRefreshInit", metrics),
	    refreshByCacheMiss("TenantEntryCacheRefreshMiss", metrics),
	    numRefreshes("TenantEntryCacheNumRefreshes", metrics),
	    refreshByWatchTrigger("TenantEntryCacheRefreshWatchTrigger", metrics) {
		TraceEvent("TenantEntryCacheCreatedDefaultFunc", uid);
	}

	TenantEntryCache(Database db, TenantEntryCacheRefreshMode mode)
	  : uid(deterministicRandom()->randomUniqueID()), db(db), createPayloadFunc(defaultCreatePayload),
	    refreshMode(mode), metrics("TenantEntryCacheMetrics", uid.toString()), hits("TenantEntryCacheHits", metrics),
	    misses("TenantEntryCacheMisses", metrics), refreshByCacheInit("TenantEntryCacheRefreshInit", metrics),
	    refreshByCacheMiss("TenantEntryCacheRefreshMiss", metrics),
	    numRefreshes("TenantEntryCacheNumRefreshes", metrics),
	    refreshByWatchTrigger("TenantEntryCacheRefreshWatchTrigger", metrics) {
		TraceEvent("TenantEntryCacheCreatedDefaultFunc", uid);
	}

	TenantEntryCache(Database db, TenantEntryCachePayloadFunc<T> fn)
	  : uid(deterministicRandom()->randomUniqueID()), db(db), createPayloadFunc(fn),
	    refreshMode(TenantEntryCacheRefreshMode::PERIODIC_TASK), metrics("TenantEntryCacheMetrics", uid.toString()),
	    hits("TenantEntryCacheHits", metrics), misses("TenantEntryCacheMisses", metrics),
	    refreshByCacheInit("TenantEntryCacheRefreshInit", metrics),
	    refreshByCacheMiss("TenantEntryCacheRefreshMiss", metrics),
	    numRefreshes("TenantEntryCacheNumRefreshes", metrics),
	    refreshByWatchTrigger("TenantEntryCacheRefreshWatchTrigger", metrics) {
		TraceEvent("TenantEntryCacheCreated", uid);
	}

	TenantEntryCache(Database db, UID id, TenantEntryCachePayloadFunc<T> fn)
	  : uid(id), db(db), createPayloadFunc(fn), refreshMode(TenantEntryCacheRefreshMode::PERIODIC_TASK),
	    metrics("TenantEntryCacheMetrics", uid.toString()), hits("TenantEntryCacheHits", metrics),
	    misses("TenantEntryCacheMisses", metrics), refreshByCacheInit("TenantEntryCacheRefreshInit", metrics),
	    refreshByCacheMiss("TenantEntryCacheRefreshMiss", metrics),
	    numRefreshes("TenantEntryCacheNumRefreshes", metrics),
	    refreshByWatchTrigger("TenantEntryCacheRefreshWatchTrigger", metrics) {
		TraceEvent("TenantEntryCacheCreated", uid);
	}

	TenantEntryCache(Database db, UID id, TenantEntryCachePayloadFunc<T> fn, TenantEntryCacheRefreshMode mode)
	  : uid(id), db(db), createPayloadFunc(fn), refreshMode(mode), metrics("TenantEntryCacheMetrics", uid.toString()),
	    hits("TenantEntryCacheHits", metrics), misses("TenantEntryCacheMisses", metrics),
	    refreshByCacheInit("TenantEntryCacheRefreshInit", metrics),
	    refreshByCacheMiss("TenantEntryCacheRefreshMiss", metrics),
	    numRefreshes("TenantEntryCacheNumRefreshes", metrics),
	    refreshByWatchTrigger("TenantEntryCacheRefreshWatchTrigger", metrics) {
		TraceEvent("TenantEntryCacheCreated", uid);
	}

	Future<Void> init(bool waitForInitalWatch = false) {
		TraceEvent("TenantEntryCacheInit", uid);

		Future<Void> f = refreshImpl(this, TenantEntryCacheRefreshReason::INIT);

		// Launch reaper task to periodically refresh cache by scanning database KeyRange
		TenantEntryCacheRefreshReason reason = TenantEntryCacheRefreshReason::PERIODIC_TASK;
		Future<Void> initalWatchFuture = Void();
		lastTenantIdRefresher = lastTenantIdWatch(this);
		if (refreshMode == TenantEntryCacheRefreshMode::PERIODIC_TASK) {
			refresher = recurringAsync([&, reason]() { return refresh(reason); },
			                           CLIENT_KNOBS->TENANT_ENTRY_CACHE_LIST_REFRESH_INTERVAL, /* interval */
			                           true, /* absoluteIntervalDelay */
			                           CLIENT_KNOBS->TENANT_ENTRY_CACHE_LIST_REFRESH_INTERVAL, /* intialDelay */
			                           TaskPriority::Worker);
		} else if (refreshMode == TenantEntryCacheRefreshMode::WATCH) {
			if (waitForInitalWatch) {
				initalWatchFuture = setInitialWatch.getFuture();
			}
			watchRefresher = refreshCacheUsingWatch(this, TenantEntryCacheRefreshReason::WATCH_TRIGGER);
		}

		Future<Void> setLastTenant = setLastTenantId(this);

		return f && initalWatchFuture && setLastTenant;
	}

	Database getDatabase() const { return db; }
	UID id() const { return uid; }

	void clear() {
		mapByTenantId.clear();
		mapByTenantName.clear();
	}

	Future<Void> removeEntryById(int64_t tenantId, bool refreshCache = false) {
		return removeEntryInt(tenantId, Optional<KeyRef>(), Optional<TenantName>(), refreshCache);
	}
	Future<Void> removeEntryByPrefix(KeyRef tenantPrefix, bool refreshCache = false) {
		return removeEntryInt(Optional<int64_t>(), tenantPrefix, Optional<TenantName>(), refreshCache);
	}
	Future<Void> removeEntryByName(TenantName tenantName, bool refreshCache = false) {
		return removeEntryInt(Optional<int64_t>(), Optional<KeyRef>(), tenantName, refreshCache);
	}

	void put(const TenantMapEntry& entry) {
		TenantEntryCachePayload<T> payload = createPayloadFunc(entry);
		auto idItr = mapByTenantId.find(entry.id);
		auto nameItr = mapByTenantName.find(entry.tenantName);

		Optional<TenantName> existingName;
		Optional<int64_t> existingId;
		if (nameItr != mapByTenantName.end()) {
			existingId = nameItr->value.entry.id;
		}
		if (idItr != mapByTenantId.end()) {
			existingName = idItr->value.entry.tenantName;
		}
		if (existingId.present()) {
			mapByTenantId.erase(existingId.get());
		}
		if (existingName.present()) {
			mapByTenantName.erase(existingName.get());
		}

		mapByTenantId[entry.id] = payload;
		mapByTenantName[entry.tenantName] = payload;

		TraceEvent("TenantEntryCachePut", uid)
		    .detail("TenantName", entry.tenantName)
		    .detail("TenantNameExisting", existingName)
		    .detail("TenantID", entry.id)
		    .detail("TenantIDExisting", existingId)
		    .detail("TenantPrefix", entry.prefix);

		CODE_PROBE(idItr == mapByTenantId.end() && nameItr == mapByTenantName.end(), "TenantCache new entry");
		CODE_PROBE(idItr != mapByTenantId.end() && nameItr == mapByTenantName.end(), "TenantCache entry name updated");
		CODE_PROBE(idItr == mapByTenantId.end() && nameItr != mapByTenantName.end(), "TenantCache entry id updated");
		CODE_PROBE(idItr != mapByTenantId.end() && nameItr != mapByTenantName.end(),
		           "TenantCache entry id and name updated");
	}

	Future<Optional<TenantEntryCachePayload<T>>> getById(int64_t tenantId) { return getByIdImpl(this, tenantId); }
	Future<Optional<TenantEntryCachePayload<T>>> getByPrefix(KeyRef prefix) {
		int64_t id = TenantAPI::prefixToId(prefix);
		return getByIdImpl(this, id);
	}
	Future<Optional<TenantEntryCachePayload<T>>> getByName(TenantName name) { return getByNameImpl(this, name); }

	// Counter access APIs
	Counter::Value numCacheRefreshes() const { return numRefreshes.getValue(); }
	Counter::Value numRefreshByMisses() const { return refreshByCacheMiss.getValue(); }
	Counter::Value numRefreshByInit() const { return refreshByCacheInit.getValue(); }
	Counter::Value numWatchRefreshes() const { return refreshByWatchTrigger.getValue(); }
};

#include "flow/unactorcompiler.h"
#endif // FDBCLIENT_TENANTENTRYCACHE_ACTOR_H