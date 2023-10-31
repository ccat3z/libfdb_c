#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
/*
 * TenantData.actor.h
 *
 * This source file is part of the FoundationDB open source project
 *
 * Copyright 2013-2023 Apple Inc. and the FoundationDB project authors
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
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_TENANTDATA_ACTOR_G_H)
#define FDBCLIENT_TENANTDATA_ACTOR_G_H
#include "fdbclient/TenantData.actor.g.h"
#elif !defined(FDBCLIENT_TENANTDATA_ACTOR_H)
#define FDBCLIENT_TENANTDATA_ACTOR_H

#include "fdbclient/FDBOptions.g.h"
#include "fdbclient/KeyBackedTypes.actor.h"
#include "fdbclient/MetaclusterRegistration.h"
#include "fdbclient/Tenant.h"
#include "fdbclient/TenantManagement.actor.h"
#include "flow/BooleanParam.h"

#include "flow/actorcompiler.h" // This must be the last #include.

template <class DB, class TenantTypes>
class TenantData {
public:
	Reference<DB> db;
	TenantMetadataSpecification<TenantTypes>* tenantMetadata;

	Optional<MetaclusterRegistrationEntry> metaclusterRegistration;
	ClusterType clusterType;

	std::map<int64_t, typename TenantTypes::TenantMapEntryT> tenantMap;
	std::map<TenantName, int64_t> tenantNameIndex;
	int64_t lastTenantId;
	int64_t tenantCount;
	std::set<int64_t> tenantTombstones;
	Optional<TenantTombstoneCleanupData> tombstoneCleanupData;
	std::map<TenantGroupName, typename TenantTypes::TenantGroupEntryT> tenantGroupMap;
	std::map<TenantGroupName, std::set<int64_t>> tenantGroupIndex;
	std::map<TenantGroupName, int64_t> storageQuotas;

private:
	// Note: this check can only be run on metaclusters with a reasonable number of tenants, as should be
	// the case with the current metacluster simulation workloads
	static inline const int metaclusterMaxTenants = 10e6;

																#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
// This generated class is to be used only via loadTenantMetadata()
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
template <class Transaction, class LoadTenantMetadataActor>
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
class LoadTenantMetadataActorState {
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
public:
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
	LoadTenantMetadataActorState(TenantData* const& self,Transaction const& tr) 
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		 : self(self),
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		   tr(tr),
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		   tenantList(),
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		   tenantNameIndexList(),
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		   tenantTombstoneList(),
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		   tenantGroupList(),
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		   tenantGroupTenantTuples(),
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		   storageQuotaList()
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
	{
		fdb_probe_actor_create("loadTenantMetadata", reinterpret_cast<unsigned long>(this));

	}
	~LoadTenantMetadataActorState() 
	{
		fdb_probe_actor_destroy("loadTenantMetadata", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
			StrictFuture<Void> __when_expr_0 = store(self->metaclusterRegistration, metacluster::metadata::metaclusterRegistration().get(tr));
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
			if (static_cast<LoadTenantMetadataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<LoadTenantMetadataActor*>(this)->actor_wait_state = 1;
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< LoadTenantMetadataActor, 0, Void >*>(static_cast<LoadTenantMetadataActor*>(this)));
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
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
		this->~LoadTenantMetadataActorState();
		static_cast<LoadTenantMetadataActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		self->clusterType = self->metaclusterRegistration.present() ? self->metaclusterRegistration.get().clusterType : ClusterType::STANDALONE;
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		StrictFuture<Void> __when_expr_1 = store(tenantList, self->tenantMetadata->tenantMap.getRange(tr, {}, {}, metaclusterMaxTenants)) && store(tenantNameIndexList, self->tenantMetadata->tenantNameIndex.getRange(tr, {}, {}, metaclusterMaxTenants)) && store(self->lastTenantId, self->tenantMetadata->lastTenantId.getD(tr, Snapshot::False, -1)) && store(self->tenantCount, self->tenantMetadata->tenantCount.getD(tr, Snapshot::False, 0)) && store(tenantTombstoneList, self->tenantMetadata->tenantTombstones.getRange(tr, {}, {}, metaclusterMaxTenants)) && store(self->tombstoneCleanupData, self->tenantMetadata->tombstoneCleanupData.get(tr)) && store(tenantGroupTenantTuples, self->tenantMetadata->tenantGroupTenantIndex.getRange(tr, {}, {}, metaclusterMaxTenants)) && store(tenantGroupList, self->tenantMetadata->tenantGroupMap.getRange(tr, {}, {}, metaclusterMaxTenants)) && store(storageQuotaList, self->tenantMetadata->storageQuota.getRange(tr, {}, {}, metaclusterMaxTenants));
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		if (static_cast<LoadTenantMetadataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<LoadTenantMetadataActor*>(this)->actor_wait_state = 2;
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< LoadTenantMetadataActor, 1, Void >*>(static_cast<LoadTenantMetadataActor*>(this)));
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		self->clusterType = self->metaclusterRegistration.present() ? self->metaclusterRegistration.get().clusterType : ClusterType::STANDALONE;
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		StrictFuture<Void> __when_expr_1 = store(tenantList, self->tenantMetadata->tenantMap.getRange(tr, {}, {}, metaclusterMaxTenants)) && store(tenantNameIndexList, self->tenantMetadata->tenantNameIndex.getRange(tr, {}, {}, metaclusterMaxTenants)) && store(self->lastTenantId, self->tenantMetadata->lastTenantId.getD(tr, Snapshot::False, -1)) && store(self->tenantCount, self->tenantMetadata->tenantCount.getD(tr, Snapshot::False, 0)) && store(tenantTombstoneList, self->tenantMetadata->tenantTombstones.getRange(tr, {}, {}, metaclusterMaxTenants)) && store(self->tombstoneCleanupData, self->tenantMetadata->tombstoneCleanupData.get(tr)) && store(tenantGroupTenantTuples, self->tenantMetadata->tenantGroupTenantIndex.getRange(tr, {}, {}, metaclusterMaxTenants)) && store(tenantGroupList, self->tenantMetadata->tenantGroupMap.getRange(tr, {}, {}, metaclusterMaxTenants)) && store(storageQuotaList, self->tenantMetadata->storageQuota.getRange(tr, {}, {}, metaclusterMaxTenants));
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		if (static_cast<LoadTenantMetadataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<LoadTenantMetadataActor*>(this)->actor_wait_state = 2;
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< LoadTenantMetadataActor, 1, Void >*>(static_cast<LoadTenantMetadataActor*>(this)));
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
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
		if (static_cast<LoadTenantMetadataActor*>(this)->actor_wait_state > 0) static_cast<LoadTenantMetadataActor*>(this)->actor_wait_state = 0;
		static_cast<LoadTenantMetadataActor*>(this)->ActorCallback< LoadTenantMetadataActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LoadTenantMetadataActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("loadTenantMetadata", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadTenantMetadata", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< LoadTenantMetadataActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("loadTenantMetadata", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadTenantMetadata", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< LoadTenantMetadataActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("loadTenantMetadata", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadTenantMetadata", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		ASSERT(!tenantList.more);
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		self->tenantMap = std::map<int64_t, typename TenantTypes::TenantMapEntryT>(tenantList.results.begin(), tenantList.results.end());
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		ASSERT(!tenantNameIndexList.more);
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		self->tenantNameIndex = std::map<TenantName, int64_t>(tenantNameIndexList.results.begin(), tenantNameIndexList.results.end());
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		ASSERT(!tenantTombstoneList.more);
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		self->tenantTombstones = std::set<int64_t>(tenantTombstoneList.results.begin(), tenantTombstoneList.results.end());
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		ASSERT(!tenantGroupList.more);
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		self->tenantGroupMap = std::map<TenantGroupName, typename TenantTypes::TenantGroupEntryT>( tenantGroupList.results.begin(), tenantGroupList.results.end());
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		ASSERT(!storageQuotaList.more);
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		self->storageQuotas = std::map<TenantGroupName, int64_t>(storageQuotaList.results.begin(), storageQuotaList.results.end());
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		self->tenantGroupIndex.clear();
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		for( auto t : tenantGroupTenantTuples.results ) {
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
			ASSERT_EQ(t.size(), 2);
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
			TenantGroupName tenantGroupName = t.getString(0);
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
			int64_t tenantId = t.getInt(1);
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
			ASSERT(self->tenantGroupMap.count(tenantGroupName));
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
			ASSERT(self->tenantMap.count(tenantId));
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
			self->tenantGroupIndex[tenantGroupName].insert(tenantId);
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
		}
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		ASSERT_EQ(self->tenantGroupIndex.size(), self->tenantGroupMap.size());
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		if (!static_cast<LoadTenantMetadataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~LoadTenantMetadataActorState(); static_cast<LoadTenantMetadataActor*>(this)->destroy(); return 0; }
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
		new (&static_cast<LoadTenantMetadataActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~LoadTenantMetadataActorState();
		static_cast<LoadTenantMetadataActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		ASSERT(!tenantList.more);
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		self->tenantMap = std::map<int64_t, typename TenantTypes::TenantMapEntryT>(tenantList.results.begin(), tenantList.results.end());
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		ASSERT(!tenantNameIndexList.more);
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		self->tenantNameIndex = std::map<TenantName, int64_t>(tenantNameIndexList.results.begin(), tenantNameIndexList.results.end());
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		ASSERT(!tenantTombstoneList.more);
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		self->tenantTombstones = std::set<int64_t>(tenantTombstoneList.results.begin(), tenantTombstoneList.results.end());
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		ASSERT(!tenantGroupList.more);
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		self->tenantGroupMap = std::map<TenantGroupName, typename TenantTypes::TenantGroupEntryT>( tenantGroupList.results.begin(), tenantGroupList.results.end());
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		ASSERT(!storageQuotaList.more);
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		self->storageQuotas = std::map<TenantGroupName, int64_t>(storageQuotaList.results.begin(), storageQuotaList.results.end());
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		self->tenantGroupIndex.clear();
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		for( auto t : tenantGroupTenantTuples.results ) {
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
			ASSERT_EQ(t.size(), 2);
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
			TenantGroupName tenantGroupName = t.getString(0);
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
			int64_t tenantId = t.getInt(1);
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
			ASSERT(self->tenantGroupMap.count(tenantGroupName));
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
			ASSERT(self->tenantMap.count(tenantId));
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
			self->tenantGroupIndex[tenantGroupName].insert(tenantId);
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
		}
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		ASSERT_EQ(self->tenantGroupIndex.size(), self->tenantGroupMap.size());
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
		if (!static_cast<LoadTenantMetadataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~LoadTenantMetadataActorState(); static_cast<LoadTenantMetadataActor*>(this)->destroy(); return 0; }
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
		new (&static_cast<LoadTenantMetadataActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~LoadTenantMetadataActorState();
		static_cast<LoadTenantMetadataActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<LoadTenantMetadataActor*>(this)->actor_wait_state > 0) static_cast<LoadTenantMetadataActor*>(this)->actor_wait_state = 0;
		static_cast<LoadTenantMetadataActor*>(this)->ActorCallback< LoadTenantMetadataActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LoadTenantMetadataActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("loadTenantMetadata", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadTenantMetadata", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< LoadTenantMetadataActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("loadTenantMetadata", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadTenantMetadata", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< LoadTenantMetadataActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("loadTenantMetadata", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadTenantMetadata", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
	TenantData* self;
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
	Transaction tr;
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
	KeyBackedRangeResult<std::pair<int64_t, typename TenantTypes::TenantMapEntryT>> tenantList;
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
	KeyBackedRangeResult<std::pair<TenantName, int64_t>> tenantNameIndexList;
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
	KeyBackedRangeResult<int64_t> tenantTombstoneList;
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
	KeyBackedRangeResult<std::pair<TenantGroupName, typename TenantTypes::TenantGroupEntryT>> tenantGroupList;
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
	KeyBackedRangeResult<Tuple> tenantGroupTenantTuples;
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
	KeyBackedRangeResult<std::pair<TenantGroupName, int64_t>> storageQuotaList;
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
};
// This generated class is to be used only via loadTenantMetadata()
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
template <class Transaction>
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
class LoadTenantMetadataActor final : public Actor<Void>, public ActorCallback< LoadTenantMetadataActor<Transaction>, 0, Void >, public ActorCallback< LoadTenantMetadataActor<Transaction>, 1, Void >, public FastAllocated<LoadTenantMetadataActor<Transaction>>, public LoadTenantMetadataActorState<Transaction, LoadTenantMetadataActor<Transaction>> {
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
public:
	using FastAllocated<LoadTenantMetadataActor<Transaction>>::operator new;
	using FastAllocated<LoadTenantMetadataActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< LoadTenantMetadataActor<Transaction>, 0, Void >;
friend struct ActorCallback< LoadTenantMetadataActor<Transaction>, 1, Void >;
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
	LoadTenantMetadataActor(TenantData* const& self,Transaction const& tr) 
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
		 : Actor<Void>(),
		   LoadTenantMetadataActorState<Transaction, LoadTenantMetadataActor<Transaction>>(self, tr)
	{
		fdb_probe_actor_enter("loadTenantMetadata", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("loadTenantMetadata");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("loadTenantMetadata", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< LoadTenantMetadataActor<Transaction>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< LoadTenantMetadataActor<Transaction>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
template <class Transaction>
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
[[nodiscard]] static Future<Void> loadTenantMetadata( TenantData* const& self, Transaction const& tr ) {
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"
	return Future<Void>(new LoadTenantMetadataActor<Transaction>(self, tr));
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.g.h"
}

#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantData.actor.h"

public:
	TenantData() {}
	TenantData(Reference<DB> db, TenantMetadataSpecification<TenantTypes>* tenantMetadata)
	  : db(db), tenantMetadata(tenantMetadata) {}

	Future<Void> load() {
		return runTransactionVoid(db, [this](Reference<typename DB::TransactionT> tr) {
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
			return loadTenantMetadata(this, tr);
		});
	}

	template <class Transaction>
	Future<Void> load(Transaction tr) {
		return loadTenantMetadata(this, tr);
	}

	// Similar to operator==, but useful in assertions for identifying which member is different
	void assertEquals(TenantData const& other) const {
		ASSERT(metaclusterRegistration == other.metaclusterRegistration);
		ASSERT_EQ(clusterType, other.clusterType);
		ASSERT(tenantMap == other.tenantMap);
		ASSERT(tenantNameIndex == other.tenantNameIndex);
		ASSERT_EQ(lastTenantId, other.lastTenantId);
		ASSERT_EQ(tenantCount, other.tenantCount);
		ASSERT(tenantTombstones == other.tenantTombstones);
		ASSERT(tombstoneCleanupData == other.tombstoneCleanupData);
		ASSERT(tenantGroupMap == other.tenantGroupMap);
		ASSERT(tenantGroupIndex == other.tenantGroupIndex);
		ASSERT(storageQuotas == other.storageQuotas);
	}

	bool operator==(TenantData const& other) const {
		return metaclusterRegistration == other.metaclusterRegistration && clusterType == other.clusterType &&
		       tenantMap == other.tenantMap && tenantNameIndex == other.tenantNameIndex &&
		       lastTenantId == other.lastTenantId && tenantCount == other.tenantCount &&
		       tenantTombstones == other.tenantTombstones && tombstoneCleanupData == other.tombstoneCleanupData &&
		       tenantGroupMap == other.tenantGroupMap && tenantGroupIndex == other.tenantGroupIndex &&
		       storageQuotas == other.storageQuotas;
	}

	bool operator!=(TenantData const& other) const { return !(*this == other); }
};

#include "flow/unactorcompiler.h"

#endif
