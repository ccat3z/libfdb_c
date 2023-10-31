#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
/*
 * GenericManagementAPI.actor.h
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
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_GENERIC_MANAGEMENT_API_ACTOR_G_H)
#define FDBCLIENT_GENERIC_MANAGEMENT_API_ACTOR_G_H
#include "fdbclient/GenericManagementAPI.actor.g.h"
#elif !defined(FDBCLIENT_GENERIC_MANAGEMENT_API_ACTOR_H)
#define FDBCLIENT_GENERIC_MANAGEMENT_API_ACTOR_H

/* This file defines "management" interfaces that have been templated to support both IClientAPI
and Native version of databases, transactions, etc., and includes functions for performing cluster
managment tasks. It isn't exposed to C clients or anywhere outside our code base and doesn't need
to be versioned. It doesn't do anything you can't do with the standard API and some knowledge of
the contents of the system key space.
*/

#include <string>
#include <map>
#include "fdbclient/ClientBooleanParams.h"
#include "fdbclient/DatabaseConfiguration.h"
#include "fdbclient/Status.h"
#include "fdbclient/Subspace.h"
#include "fdbclient/DatabaseConfiguration.h"
#include "fdbclient/MetaclusterRegistration.h"
#include "fdbclient/Status.h"
#include "fdbclient/SystemData.h"
#include "fdbclient/StorageWiggleMetrics.actor.h"
#include "flow/actorcompiler.h" // has to be last include

// ConfigurationResult enumerates normal outcomes of changeConfig() and various error
// conditions specific to it.  changeConfig may also throw an Error to report other problems.
enum class ConfigurationResult {
	NO_OPTIONS_PROVIDED,
	CONFLICTING_OPTIONS,
	UNKNOWN_OPTION,
	INCOMPLETE_CONFIGURATION,
	INVALID_CONFIGURATION,
	STORAGE_MIGRATION_DISABLED,
	DATABASE_ALREADY_CREATED,
	DATABASE_CREATED,
	DATABASE_UNAVAILABLE,
	STORAGE_IN_UNKNOWN_DCID,
	REGION_NOT_FULLY_REPLICATED,
	MULTIPLE_ACTIVE_REGIONS,
	REGIONS_CHANGED,
	NOT_ENOUGH_WORKERS,
	REGION_REPLICATION_MISMATCH,
	DCID_MISSING,
	LOCKED_NOT_NEW,
	SUCCESS_WARN_PPW_GRADUAL,
	SUCCESS,
	SUCCESS_WARN_ROCKSDB_EXPERIMENTAL,
	SUCCESS_WARN_SHARDED_ROCKSDB_EXPERIMENTAL,
	DATABASE_CREATED_WARN_ROCKSDB_EXPERIMENTAL,
	DATABASE_CREATED_WARN_SHARDED_ROCKSDB_EXPERIMENTAL,
	DATABASE_IS_REGISTERED,
	ENCRYPTION_AT_REST_MODE_ALREADY_SET
};

enum class CoordinatorsResult {
	INVALID_NETWORK_ADDRESSES,
	SAME_NETWORK_ADDRESSES,
	NOT_COORDINATORS, // FIXME: not detected
	DATABASE_UNREACHABLE, // FIXME: not detected
	BAD_DATABASE_STATE,
	COORDINATOR_UNREACHABLE,
	NOT_ENOUGH_MACHINES,
	SUCCESS
};

struct ConfigureAutoResult {
	std::map<NetworkAddress, ProcessClass> address_class;
	int32_t processes;
	int32_t machines;

	std::string old_replication;
	int32_t old_commit_proxies;
	int32_t old_grv_proxies;
	int32_t old_resolvers;
	int32_t old_logs;
	int32_t old_processes_with_transaction;
	int32_t old_machines_with_transaction;

	std::string auto_replication;
	int32_t auto_commit_proxies;
	int32_t auto_grv_proxies;
	int32_t auto_resolvers;
	int32_t auto_logs;
	int32_t auto_processes_with_transaction;
	int32_t auto_machines_with_transaction;

	int32_t desired_commit_proxies;
	int32_t desired_grv_proxies;
	int32_t desired_resolvers;
	int32_t desired_logs;

	ConfigureAutoResult()
	  : processes(-1), machines(-1), old_commit_proxies(-1), old_grv_proxies(-1), old_resolvers(-1), old_logs(-1),
	    old_processes_with_transaction(-1), old_machines_with_transaction(-1), auto_commit_proxies(-1),
	    auto_grv_proxies(-1), auto_resolvers(-1), auto_logs(-1), auto_processes_with_transaction(-1),
	    auto_machines_with_transaction(-1), desired_commit_proxies(-1), desired_grv_proxies(-1), desired_resolvers(-1),
	    desired_logs(-1) {}

	bool isValid() const { return processes != -1; }
};

ConfigurationResult buildConfiguration(
    std::vector<StringRef> const& modeTokens,
    std::map<std::string, std::string>& outConf); // Accepts a vector of configuration tokens
ConfigurationResult buildConfiguration(
    std::string const& modeString,
    std::map<std::string, std::string>& outConf); // Accepts tokens separated by spaces in a single string

bool isCompleteConfiguration(std::map<std::string, std::string> const& options);

ConfigureAutoResult parseConfig(StatusObject const& status);

bool isEncryptionAtRestModeConfigValid(Optional<DatabaseConfiguration> oldConfiguration,
                                       std::map<std::string, std::string> newConfig,
                                       bool creating);
bool isTenantModeModeConfigValid(DatabaseConfiguration oldConfiguration, DatabaseConfiguration newConfiguration);

// Management API written in template code to support both IClientAPI and NativeAPI
namespace ManagementAPI {

															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via changeCachedRange()
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
template <class DB, class ChangeCachedRangeActor>
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
class ChangeCachedRangeActorState {
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	ChangeCachedRangeActorState(Reference<DB> const& db,KeyRangeRef const& range,bool const& add) 
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		 : db(db),
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   range(range),
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   add(add),
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   tr(db->createTransaction()),
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   sysRange(KeyRangeRef(storageCacheKey(range.begin), storageCacheKey(range.end))),
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   sysRangeClear(KeyRangeRef(storageCacheKey(range.begin), keyAfter(storageCacheKey(range.end)))),
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   privateRange(KeyRangeRef(cacheKeysKey(0, range.begin), cacheKeysKey(0, range.end))),
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   trueValue(storageCacheValue(std::vector<uint16_t>{ 0 })),
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   falseValue(storageCacheValue(std::vector<uint16_t>{}))
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("changeCachedRange", reinterpret_cast<unsigned long>(this));

	}
	~ChangeCachedRangeActorState() 
	{
		fdb_probe_actor_destroy("changeCachedRange", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			;
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
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
		this->~ChangeCachedRangeActorState();
		static_cast<ChangeCachedRangeActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		try {
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->clear(sysRangeClear);
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->clear(privateRange);
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->addReadConflictRange(privateRange);
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			previousFuture = tr->getRange(KeyRangeRef(storageCachePrefix, sysRange.begin), 1, Snapshot::False, Reverse::True);
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<RangeResult> __when_expr_0 = safeThreadFutureToFuture(previousFuture);
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 1;
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ChangeCachedRangeActor, 0, RangeResult >*>(static_cast<ChangeCachedRangeActor*>(this)));
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
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
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			err = e;
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Void> __when_expr_3 = safeThreadFutureToFuture(tr->onError(e));
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 4;
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ChangeCachedRangeActor, 3, Void >*>(static_cast<ChangeCachedRangeActor*>(this)));
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(RangeResult const& previous,int loopDepth) 
	{
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		bool prevIsCached = false;
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!previous.empty())
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			std::vector<uint16_t> prevVal;
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			decodeStorageCacheValue(previous[0].value, prevVal);
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			prevIsCached = !prevVal.empty();
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (prevIsCached && !add)
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(sysRange.begin, falseValue);
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(privateRange.begin, serverKeysFalse);
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
		else
		{
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!prevIsCached && add)
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->set(sysRange.begin, trueValue);
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->set(privateRange.begin, serverKeysTrue);
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		afterFuture = tr->getRange(KeyRangeRef(sysRange.end, storageCacheKeys.end), 1, Snapshot::False, Reverse::False);
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<RangeResult> __when_expr_1 = safeThreadFutureToFuture(afterFuture);
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 2;
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChangeCachedRangeActor, 1, RangeResult >*>(static_cast<ChangeCachedRangeActor*>(this)));
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(RangeResult && previous,int loopDepth) 
	{
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		bool prevIsCached = false;
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!previous.empty())
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			std::vector<uint16_t> prevVal;
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			decodeStorageCacheValue(previous[0].value, prevVal);
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			prevIsCached = !prevVal.empty();
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (prevIsCached && !add)
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(sysRange.begin, falseValue);
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(privateRange.begin, serverKeysFalse);
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
		else
		{
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!prevIsCached && add)
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->set(sysRange.begin, trueValue);
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->set(privateRange.begin, serverKeysTrue);
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		afterFuture = tr->getRange(KeyRangeRef(sysRange.end, storageCacheKeys.end), 1, Snapshot::False, Reverse::False);
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<RangeResult> __when_expr_1 = safeThreadFutureToFuture(afterFuture);
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 2;
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChangeCachedRangeActor, 1, RangeResult >*>(static_cast<ChangeCachedRangeActor*>(this)));
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& previous,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(previous, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && previous,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(previous), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state > 0) static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeCachedRangeActor*>(this)->ActorCallback< ChangeCachedRangeActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ChangeCachedRangeActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(RangeResult const& after,int loopDepth) 
	{
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		bool afterIsCached = false;
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!after.empty())
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			std::vector<uint16_t> afterVal;
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			decodeStorageCacheValue(after[0].value, afterVal);
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			afterIsCached = afterVal.empty();
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (afterIsCached && !add)
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(sysRange.end, trueValue);
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(privateRange.end, serverKeysTrue);
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
		else
		{
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!afterIsCached && add)
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->set(sysRange.end, falseValue);
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->set(privateRange.end, serverKeysFalse);
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->commit());
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 3;
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ChangeCachedRangeActor, 2, Void >*>(static_cast<ChangeCachedRangeActor*>(this)));
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(RangeResult && after,int loopDepth) 
	{
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		bool afterIsCached = false;
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!after.empty())
															#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			std::vector<uint16_t> afterVal;
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			decodeStorageCacheValue(after[0].value, afterVal);
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			afterIsCached = afterVal.empty();
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (afterIsCached && !add)
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(sysRange.end, trueValue);
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(privateRange.end, serverKeysTrue);
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
		else
		{
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!afterIsCached && add)
															#line 539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->set(sysRange.end, falseValue);
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->set(privateRange.end, serverKeysFalse);
															#line 545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->commit());
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 3;
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ChangeCachedRangeActor, 2, Void >*>(static_cast<ChangeCachedRangeActor*>(this)));
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(RangeResult const& after,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(after, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(RangeResult && after,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(after), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state > 0) static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeCachedRangeActor*>(this)->ActorCallback< ChangeCachedRangeActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 1, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 1, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ChangeCachedRangeActor, 1, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont8(Void const& _,int loopDepth) 
	{
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<ChangeCachedRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ChangeCachedRangeActorState(); static_cast<ChangeCachedRangeActor*>(this)->destroy(); return 0; }
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<ChangeCachedRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ChangeCachedRangeActorState();
		static_cast<ChangeCachedRangeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void && _,int loopDepth) 
	{
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<ChangeCachedRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ChangeCachedRangeActorState(); static_cast<ChangeCachedRangeActor*>(this)->destroy(); return 0; }
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<ChangeCachedRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ChangeCachedRangeActorState();
		static_cast<ChangeCachedRangeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state > 0) static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeCachedRangeActor*>(this)->ActorCallback< ChangeCachedRangeActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ChangeCachedRangeActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		TraceEvent(SevDebug, "ChangeCachedRangeError").error(err);
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		TraceEvent(SevDebug, "ChangeCachedRangeError").error(err);
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
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
	void a_exitChoose4() 
	{
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state > 0) static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeCachedRangeActor*>(this)->ActorCallback< ChangeCachedRangeActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ChangeCachedRangeActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Reference<DB> db;
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	KeyRangeRef range;
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	bool add;
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	KeyRange sysRange;
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	KeyRange sysRangeClear;
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	KeyRange privateRange;
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Value trueValue;
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Value falseValue;
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> previousFuture;
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> afterFuture;
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Error err;
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via changeCachedRange()
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
class ChangeCachedRangeActor final : public Actor<Void>, public ActorCallback< ChangeCachedRangeActor<DB>, 0, RangeResult >, public ActorCallback< ChangeCachedRangeActor<DB>, 1, RangeResult >, public ActorCallback< ChangeCachedRangeActor<DB>, 2, Void >, public ActorCallback< ChangeCachedRangeActor<DB>, 3, Void >, public FastAllocated<ChangeCachedRangeActor<DB>>, public ChangeCachedRangeActorState<DB, ChangeCachedRangeActor<DB>> {
															#line 824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<ChangeCachedRangeActor<DB>>::operator new;
	using FastAllocated<ChangeCachedRangeActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ChangeCachedRangeActor<DB>, 0, RangeResult >;
friend struct ActorCallback< ChangeCachedRangeActor<DB>, 1, RangeResult >;
friend struct ActorCallback< ChangeCachedRangeActor<DB>, 2, Void >;
friend struct ActorCallback< ChangeCachedRangeActor<DB>, 3, Void >;
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	ChangeCachedRangeActor(Reference<DB> const& db,KeyRangeRef const& range,bool const& add) 
															#line 838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<Void>(),
		   ChangeCachedRangeActorState<DB, ChangeCachedRangeActor<DB>>(db, range, add)
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("changeCachedRange");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ChangeCachedRangeActor<DB>, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ChangeCachedRangeActor<DB>, 1, RangeResult >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ChangeCachedRangeActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ChangeCachedRangeActor<DB>, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<Void> changeCachedRange( Reference<DB> const& db, KeyRangeRef const& range, bool const& add ) {
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	return Future<Void>(new ChangeCachedRangeActor<DB>(db, range, add));
															#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"

template <class DB>
Future<Void> addCachedRange(Reference<DB> db, KeyRangeRef range) {
	return changeCachedRange(db, range, true);
}

template <class DB>
Future<Void> removeCachedRange(Reference<DB> db, KeyRangeRef range) {
	return changeCachedRange(db, range, false);
}

															#line 885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via getWorkers()
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
template <class Tr, class GetWorkersActor>
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
class GetWorkersActorState {
															#line 891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	GetWorkersActorState(Reference<Tr> const& tr,typename Tr::template FutureT<RangeResult> const& processClassesF,typename Tr::template FutureT<RangeResult> const& processDataF) 
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		 : tr(tr),
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   processClassesF(processClassesF),
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   processDataF(processDataF)
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("getWorkers", reinterpret_cast<unsigned long>(this));

	}
	~GetWorkersActorState() 
	{
		fdb_probe_actor_destroy("getWorkers", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			processClassesF = tr->getRange(processClassKeys, CLIENT_KNOBS->TOO_MANY);
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			processDataF = tr->getRange(workerListKeys, CLIENT_KNOBS->TOO_MANY);
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			processClasses = safeThreadFutureToFuture(processClassesF);
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			processData = safeThreadFutureToFuture(processDataF);
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Void> __when_expr_0 = success(processClasses) && success(processData);
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<GetWorkersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetWorkersActor*>(this)->actor_wait_state = 1;
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetWorkersActor, 0, Void >*>(static_cast<GetWorkersActor*>(this)));
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
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
		this->~GetWorkersActorState();
		static_cast<GetWorkersActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!processClasses.get().more && processClasses.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!processData.get().more && processData.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		std::map<Optional<Standalone<StringRef>>, ProcessClass> id_class;
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for(int i = 0;i < processClasses.get().size();i++) {
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			id_class[decodeProcessClassKey(processClasses.get()[i].key)] = decodeProcessClassValue(processClasses.get()[i].value);
															#line 963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		std::vector<ProcessData> results;
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for(int i = 0;i < processData.get().size();i++) {
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			ProcessData data = decodeWorkerListValue(processData.get()[i].value);
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			ProcessClass processClass = id_class[data.locality.processId()];
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (processClass.classSource() == ProcessClass::DBSource || data.processClass.classType() == ProcessClass::UnsetClass)
															#line 975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				data.processClass = processClass;
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (data.processClass.classType() != ProcessClass::TesterClass)
															#line 983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				results.push_back(data);
															#line 987 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<GetWorkersActor*>(this)->SAV<std::vector<ProcessData>>::futures) { (void)(results); this->~GetWorkersActorState(); static_cast<GetWorkersActor*>(this)->destroy(); return 0; }
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<GetWorkersActor*>(this)->SAV< std::vector<ProcessData> >::value()) std::vector<ProcessData>(results);
		this->~GetWorkersActorState();
		static_cast<GetWorkersActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!processClasses.get().more && processClasses.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!processData.get().more && processData.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		std::map<Optional<Standalone<StringRef>>, ProcessClass> id_class;
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for(int i = 0;i < processClasses.get().size();i++) {
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			id_class[decodeProcessClassKey(processClasses.get()[i].key)] = decodeProcessClassValue(processClasses.get()[i].value);
															#line 1012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		std::vector<ProcessData> results;
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for(int i = 0;i < processData.get().size();i++) {
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			ProcessData data = decodeWorkerListValue(processData.get()[i].value);
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			ProcessClass processClass = id_class[data.locality.processId()];
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (processClass.classSource() == ProcessClass::DBSource || data.processClass.classType() == ProcessClass::UnsetClass)
															#line 1024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				data.processClass = processClass;
															#line 1028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (data.processClass.classType() != ProcessClass::TesterClass)
															#line 1032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				results.push_back(data);
															#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<GetWorkersActor*>(this)->SAV<std::vector<ProcessData>>::futures) { (void)(results); this->~GetWorkersActorState(); static_cast<GetWorkersActor*>(this)->destroy(); return 0; }
															#line 1041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<GetWorkersActor*>(this)->SAV< std::vector<ProcessData> >::value()) std::vector<ProcessData>(results);
		this->~GetWorkersActorState();
		static_cast<GetWorkersActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<GetWorkersActor*>(this)->actor_wait_state > 0) static_cast<GetWorkersActor*>(this)->actor_wait_state = 0;
		static_cast<GetWorkersActor*>(this)->ActorCallback< GetWorkersActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetWorkersActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getWorkers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetWorkersActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getWorkers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetWorkersActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getWorkers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkers", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Reference<Tr> tr;
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	typename Tr::template FutureT<RangeResult> processClassesF;
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	typename Tr::template FutureT<RangeResult> processDataF;
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Future<RangeResult> processClasses;
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Future<RangeResult> processData;
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via getWorkers()
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
template <class Tr>
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
class GetWorkersActor final : public Actor<std::vector<ProcessData>>, public ActorCallback< GetWorkersActor<Tr>, 0, Void >, public FastAllocated<GetWorkersActor<Tr>>, public GetWorkersActorState<Tr, GetWorkersActor<Tr>> {
															#line 1129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<GetWorkersActor<Tr>>::operator new;
	using FastAllocated<GetWorkersActor<Tr>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<ProcessData>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetWorkersActor<Tr>, 0, Void >;
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	GetWorkersActor(Reference<Tr> const& tr,typename Tr::template FutureT<RangeResult> const& processClassesF,typename Tr::template FutureT<RangeResult> const& processDataF) 
															#line 1140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<std::vector<ProcessData>>(),
		   GetWorkersActorState<Tr, GetWorkersActor<Tr>>(tr, processClassesF, processDataF)
	{
		fdb_probe_actor_enter("getWorkers", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getWorkers");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getWorkers", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetWorkersActor<Tr>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
template <class Tr>
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<std::vector<ProcessData>> getWorkers( Reference<Tr> const& tr, typename Tr::template FutureT<RangeResult> const& processClassesF, typename Tr::template FutureT<RangeResult> const& processDataF ) {
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	return Future<std::vector<ProcessData>>(new GetWorkersActor<Tr>(tr, processClassesF, processDataF));
															#line 1169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"

// All versions of changeConfig apply the given set of configuration tokens to the database, and return a
// ConfigurationResult (or error).

// Accepts a full configuration in key/value format (from buildConfiguration)
															#line 1178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via changeConfig()
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
template <class DB, class ChangeConfigActor>
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
class ChangeConfigActorState {
															#line 1184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	ChangeConfigActorState(Reference<DB> const& db,std::map<std::string, std::string> const& m,bool const& force) 
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		 : db(db),
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   m(m),
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   force(force),
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   initIdKey("\xff/init_id"_sr),
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   tr(db->createTransaction())
															#line 1199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("changeConfig", reinterpret_cast<unsigned long>(this));

	}
	~ChangeConfigActorState() 
	{
		fdb_probe_actor_destroy("changeConfig", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!m.size())
															#line 1214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::NO_OPTIONS_PROVIDED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::NO_OPTIONS_PROVIDED);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			std::string initKey = configKeysPrefix.toString() + "initialized";
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			creating = m.count(initKey) != 0;
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			locked = Optional<UID>();
															#line 1230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				auto iter = m.find(databaseLockedKey.toString());
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (iter != m.end())
															#line 1236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!creating)
															#line 1240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
						if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::LOCKED_NOT_NEW); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
						new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::LOCKED_NOT_NEW);
						this->~ChangeConfigActorState();
						static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
						return 0;
					}
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					locked = UID::fromString(iter->second);
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					m.erase(iter);
															#line 1254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				}
			}
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (creating)
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				m[initIdKey.toString()] = deterministicRandom()->randomUniqueID().toString();
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!isCompleteConfiguration(m))
															#line 1265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::INCOMPLETE_CONFIGURATION); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::INCOMPLETE_CONFIGURATION);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!isEncryptionAtRestModeConfigValid(Optional<DatabaseConfiguration>(), m, creating))
															#line 1277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::INVALID_CONFIGURATION); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::INVALID_CONFIGURATION);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
			else
			{
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (m.count(encryptionAtRestModeConfKey.toString()) != 0)
															#line 1292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::ENCRYPTION_AT_REST_MODE_ALREADY_SET); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::ENCRYPTION_AT_REST_MODE_ALREADY_SET);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tooLong = delay(60);
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			versionKey = BinaryWriter::toValue(deterministicRandom()->randomUniqueID(), Unversioned());
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			oldReplicationUsesDcId = false;
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			resetPPWStats = false;
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			warnPPWGradual = false;
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			warnRocksDBIsExperimental = false;
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			warnShardedRocksDBIsExperimental = false;
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			;
															#line 1319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
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
		this->~ChangeConfigActorState();
		static_cast<ChangeConfigActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (warnPPWGradual)
															#line 1342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::SUCCESS_WARN_PPW_GRADUAL); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::SUCCESS_WARN_PPW_GRADUAL);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (warnRocksDBIsExperimental)
															#line 1356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::SUCCESS_WARN_ROCKSDB_EXPERIMENTAL); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::SUCCESS_WARN_ROCKSDB_EXPERIMENTAL);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (warnShardedRocksDBIsExperimental)
															#line 1370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::SUCCESS_WARN_SHARDED_ROCKSDB_EXPERIMENTAL); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::SUCCESS_WARN_SHARDED_ROCKSDB_EXPERIMENTAL);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
				else
				{
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::SUCCESS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::SUCCESS);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
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
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!creating && !force)
															#line 1415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				fConfigF = tr->getRange(configKeys, CLIENT_KNOBS->TOO_MANY);
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				fConfig = safeThreadFutureToFuture(fConfigF);
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				processClassesF = typename DB::TransactionT::template FutureT<RangeResult>();
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				processDataF = typename DB::TransactionT::template FutureT<RangeResult>();
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				fWorkers = getWorkers(tr, processClassesF, processDataF);
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				StrictFuture<Void> __when_expr_0 = success(fConfig) || tooLong;
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ChangeConfigActor*>(this)->actor_wait_state = 1;
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 0, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 1436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont2(loopDepth);
			}
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
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			e1 = Error(e);
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if ((e.code() == error_code_not_committed || e.code() == error_code_transaction_too_old) && creating)
															#line 1478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->reset();
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				;
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				loopDepth = a_body1loopBody1Catch1loopHead1(loopDepth);
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
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (creating)
															#line 1504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::INITIALIZE_NEW_DATABASE);
															#line 509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->addReadConflictRange(singleKeyRange(initIdKey));
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
		else
		{
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (m.size())
															#line 1516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->setOption(FDBTransactionOptions::CAUSAL_WRITE_RISKY);
															#line 514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->addReadConflictRange(singleKeyRange(m.begin()->first));
															#line 1522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (locked.present())
															#line 1527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			ASSERT(creating);
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->atomicOp(databaseLockedKey, BinaryWriter::toValue(locked.get(), Unversioned()) .withPrefix("0123456789"_sr) .withSuffix("\x00\x00\x00\x00"_sr), MutationRef::SetVersionstampedValue);
															#line 1533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for(auto i = m.begin();i != m.end();++i) {
															#line 527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(StringRef(i->first), StringRef(i->second));
															#line 528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (i->first == perpetualStorageWiggleKey)
															#line 1541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (i->second == "0")
															#line 1545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					resetPPWStats = true;
															#line 1549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				}
				else
				{
															#line 531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (i->first == "1")
															#line 1555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
						resetPPWStats = false;
															#line 1559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					}
				}
			}
		}
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!creating && resetPPWStats)
															#line 1566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			wiggleData = StorageWiggleData();
															#line 539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Void> __when_expr_6 = wiggleData.resetStorageWiggleMetrics(tr, PrimaryRegion(true));
															#line 539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1loopBody1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_6.get(), loopDepth); };
			static_cast<ChangeConfigActor*>(this)->actor_wait_state = 7;
															#line 539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 6, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 1579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont24(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!fConfig.isReady())
															#line 1593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (fConfig.isReady())
															#line 1605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			ASSERT(fConfig.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			oldConfig = DatabaseConfiguration();
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			oldConfig.fromKeyValues((VectorRef<KeyValueRef>)fConfig.get());
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			newConfig = oldConfig;
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			for( auto kv : m ) {
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				newConfig.set(kv.first, kv.second);
															#line 1619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!newConfig.isValid() || !isEncryptionAtRestModeConfigValid(oldConfig, m, creating) || !isTenantModeModeConfigValid(oldConfig, newConfig))
															#line 1623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::INVALID_CONFIGURATION); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::INVALID_CONFIGURATION);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (newConfig.tLogPolicy->attributeKeys().count("dcid") && newConfig.regions.size() > 0)
															#line 1635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::REGION_REPLICATION_MISMATCH); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::REGION_REPLICATION_MISMATCH);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			oldReplicationUsesDcId = oldReplicationUsesDcId || oldConfig.tLogPolicy->attributeKeys().count("dcid");
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (oldConfig.usableRegions != newConfig.usableRegions)
															#line 1649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				std::map<Key, int32_t> dcId_priority;
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				for( auto& it : newConfig.regions ) {
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					dcId_priority[it.dcId] = it.priority;
															#line 1657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				}
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				for( auto& it : oldConfig.regions ) {
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!dcId_priority.count(it.dcId) || dcId_priority[it.dcId] != it.priority)
															#line 1663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
						if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::REGIONS_CHANGED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
						new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::REGIONS_CHANGED);
						this->~ChangeConfigActorState();
						static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
						return 0;
					}
				}
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				int activeRegionCount = 0;
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				for( auto& it : newConfig.regions ) {
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (it.priority >= 0)
															#line 1680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
						activeRegionCount++;
															#line 1684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					}
				}
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (activeRegionCount > 1)
															#line 1689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::MULTIPLE_ACTIVE_REGIONS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::MULTIPLE_ACTIVE_REGIONS);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			fServerListF = tr->getRange(serverListKeys, CLIENT_KNOBS->TOO_MANY);
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			fServerList = (newConfig.regions.size()) ? safeThreadFutureToFuture(fServerListF) : Future<RangeResult>();
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (newConfig.usableRegions == 2)
															#line 1706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (oldReplicationUsesDcId)
															#line 1710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					fLocalityListF = tr->getRange(tagLocalityListKeys, CLIENT_KNOBS->TOO_MANY);
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					fLocalityList = safeThreadFutureToFuture(fLocalityListF);
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					StrictFuture<Void> __when_expr_1 = success(fLocalityList) || tooLong;
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
					static_cast<ChangeConfigActor*>(this)->actor_wait_state = 2;
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 1, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 1725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					loopDepth = 0;
				}
				else
				{
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					replicasFuturesF = std::vector<typename DB::TransactionT::template FutureT<Optional<Value>>>();
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					replicasFutures = std::vector<Future<Optional<Value>>>();
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					for( auto& it : newConfig.regions ) {
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
						if (it.priority >= 0)
															#line 1738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
						{
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
							replicasFuturesF.push_back(tr->get(datacenterReplicasKeyFor(it.dcId)));
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
							replicasFutures.push_back(safeThreadFutureToFuture(replicasFuturesF.back()));
															#line 1744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
						}
					}
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					StrictFuture<Void> __when_expr_2 = waitForAll(replicasFutures) || tooLong;
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when2(__when_expr_2.get(), loopDepth); };
					static_cast<ChangeConfigActor*>(this)->actor_wait_state = 3;
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 2, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 1756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					loopDepth = 0;
				}
			}
			else
			{
				loopDepth = a_body1loopBody1cont6(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1loopBody1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!fConfig.isReady())
															#line 1776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (fConfig.isReady())
															#line 1788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			ASSERT(fConfig.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			oldConfig = DatabaseConfiguration();
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			oldConfig.fromKeyValues((VectorRef<KeyValueRef>)fConfig.get());
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			newConfig = oldConfig;
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			for( auto kv : m ) {
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				newConfig.set(kv.first, kv.second);
															#line 1802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!newConfig.isValid() || !isEncryptionAtRestModeConfigValid(oldConfig, m, creating) || !isTenantModeModeConfigValid(oldConfig, newConfig))
															#line 1806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::INVALID_CONFIGURATION); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::INVALID_CONFIGURATION);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (newConfig.tLogPolicy->attributeKeys().count("dcid") && newConfig.regions.size() > 0)
															#line 1818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::REGION_REPLICATION_MISMATCH); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::REGION_REPLICATION_MISMATCH);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			oldReplicationUsesDcId = oldReplicationUsesDcId || oldConfig.tLogPolicy->attributeKeys().count("dcid");
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (oldConfig.usableRegions != newConfig.usableRegions)
															#line 1832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				std::map<Key, int32_t> dcId_priority;
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				for( auto& it : newConfig.regions ) {
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					dcId_priority[it.dcId] = it.priority;
															#line 1840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				}
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				for( auto& it : oldConfig.regions ) {
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!dcId_priority.count(it.dcId) || dcId_priority[it.dcId] != it.priority)
															#line 1846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
						if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::REGIONS_CHANGED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
						new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::REGIONS_CHANGED);
						this->~ChangeConfigActorState();
						static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
						return 0;
					}
				}
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				int activeRegionCount = 0;
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				for( auto& it : newConfig.regions ) {
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (it.priority >= 0)
															#line 1863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
						activeRegionCount++;
															#line 1867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					}
				}
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (activeRegionCount > 1)
															#line 1872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::MULTIPLE_ACTIVE_REGIONS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::MULTIPLE_ACTIVE_REGIONS);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			fServerListF = tr->getRange(serverListKeys, CLIENT_KNOBS->TOO_MANY);
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			fServerList = (newConfig.regions.size()) ? safeThreadFutureToFuture(fServerListF) : Future<RangeResult>();
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (newConfig.usableRegions == 2)
															#line 1889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (oldReplicationUsesDcId)
															#line 1893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					fLocalityListF = tr->getRange(tagLocalityListKeys, CLIENT_KNOBS->TOO_MANY);
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					fLocalityList = safeThreadFutureToFuture(fLocalityListF);
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					StrictFuture<Void> __when_expr_1 = success(fLocalityList) || tooLong;
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
					static_cast<ChangeConfigActor*>(this)->actor_wait_state = 2;
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 1, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 1908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					loopDepth = 0;
				}
				else
				{
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					replicasFuturesF = std::vector<typename DB::TransactionT::template FutureT<Optional<Value>>>();
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					replicasFutures = std::vector<Future<Optional<Value>>>();
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					for( auto& it : newConfig.regions ) {
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
						if (it.priority >= 0)
															#line 1921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
						{
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
							replicasFuturesF.push_back(tr->get(datacenterReplicasKeyFor(it.dcId)));
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
							replicasFutures.push_back(safeThreadFutureToFuture(replicasFuturesF.back()));
															#line 1927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
						}
					}
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					StrictFuture<Void> __when_expr_2 = waitForAll(replicasFutures) || tooLong;
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when2(__when_expr_2.get(), loopDepth); };
					static_cast<ChangeConfigActor*>(this)->actor_wait_state = 3;
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 2, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 1939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					loopDepth = 0;
				}
			}
			else
			{
				loopDepth = a_body1loopBody1cont6(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1loopBody1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont4(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(int loopDepth) 
	{
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (newConfig.regions.size())
															#line 2028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Void> __when_expr_3 = success(fServerList) || tooLong;
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont6when1(__when_expr_3.get(), loopDepth); };
			static_cast<ChangeConfigActor*>(this)->actor_wait_state = 4;
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 3, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 2039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont22(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont17(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont18(Void const& _,int loopDepth) 
	{
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!fLocalityList.isReady())
															#line 2059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2063 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		RangeResult localityList = fLocalityList.get();
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!localityList.more && localityList.size() < CLIENT_KNOBS->TOO_MANY);
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		std::set<Key> localityDcIds;
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for( auto& s : localityList ) {
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			auto dc = decodeTagLocalityListKey(s.key);
															#line 386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (dc.present())
															#line 2081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				localityDcIds.insert(dc.get());
															#line 2085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : newConfig.regions ) {
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (localityDcIds.count(it.dcId) == 0)
															#line 2092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DCID_MISSING); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DCID_MISSING);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
		loopDepth = a_body1loopBody1cont17(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont18(Void && _,int loopDepth) 
	{
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!fLocalityList.isReady())
															#line 2111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		RangeResult localityList = fLocalityList.get();
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!localityList.more && localityList.size() < CLIENT_KNOBS->TOO_MANY);
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		std::set<Key> localityDcIds;
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for( auto& s : localityList ) {
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			auto dc = decodeTagLocalityListKey(s.key);
															#line 386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (dc.present())
															#line 2133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				localityDcIds.insert(dc.get());
															#line 2137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : newConfig.regions ) {
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (localityDcIds.count(it.dcId) == 0)
															#line 2144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DCID_MISSING); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DCID_MISSING);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
		loopDepth = a_body1loopBody1cont17(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont18(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont18(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont19(Void const& _,int loopDepth) 
	{
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : replicasFutures ) {
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!it.isReady())
															#line 2228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!it.get().present())
															#line 2240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::REGION_NOT_FULLY_REPLICATED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::REGION_NOT_FULLY_REPLICATED);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
		loopDepth = a_body1loopBody1cont17(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont19(Void && _,int loopDepth) 
	{
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : replicasFutures ) {
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!it.isReady())
															#line 2261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!it.get().present())
															#line 2273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::REGION_NOT_FULLY_REPLICATED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::REGION_NOT_FULLY_REPLICATED);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
		loopDepth = a_body1loopBody1cont17(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont19(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont19(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when2(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont22(int loopDepth) 
	{
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_4 = success(fWorkers) || tooLong;
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1loopBody1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1loopBody1cont22when1(__when_expr_4.get(), loopDepth); };
		static_cast<ChangeConfigActor*>(this)->actor_wait_state = 5;
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 4, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 2362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont23(Void const& _,int loopDepth) 
	{
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!fServerList.isReady())
															#line 2371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		RangeResult serverList = fServerList.get();
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!serverList.more && serverList.size() < CLIENT_KNOBS->TOO_MANY);
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		std::set<Key> newDcIds;
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : newConfig.regions ) {
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			newDcIds.insert(it.dcId);
															#line 2391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		std::set<Optional<Key>> missingDcIds;
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for( auto& s : serverList ) {
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			auto ssi = decodeServerListValue(s.value);
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!ssi.locality.dcId().present() || !newDcIds.count(ssi.locality.dcId().get()))
															#line 2401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				missingDcIds.insert(ssi.locality.dcId());
															#line 2405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (missingDcIds.size() > (oldReplicationUsesDcId ? 1 : 0))
															#line 2410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::STORAGE_IN_UNKNOWN_DCID); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::STORAGE_IN_UNKNOWN_DCID);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1loopBody1cont22(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont23(Void && _,int loopDepth) 
	{
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!fServerList.isReady())
															#line 2428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		RangeResult serverList = fServerList.get();
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!serverList.more && serverList.size() < CLIENT_KNOBS->TOO_MANY);
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		std::set<Key> newDcIds;
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : newConfig.regions ) {
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			newDcIds.insert(it.dcId);
															#line 2448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		std::set<Optional<Key>> missingDcIds;
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for( auto& s : serverList ) {
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			auto ssi = decodeServerListValue(s.value);
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!ssi.locality.dcId().present() || !newDcIds.count(ssi.locality.dcId().get()))
															#line 2458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				missingDcIds.insert(ssi.locality.dcId());
															#line 2462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (missingDcIds.size() > (oldReplicationUsesDcId ? 1 : 0))
															#line 2467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::STORAGE_IN_UNKNOWN_DCID); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::STORAGE_IN_UNKNOWN_DCID);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1loopBody1cont22(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont23(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont23(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1cont22cont1(Void const& _,int loopDepth) 
	{
															#line 446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!fWorkers.isReady())
															#line 2548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (newConfig.regions.size())
															#line 2560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			std::map<Optional<Key>, std::set<Optional<Key>>> dcId_zoneIds;
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			for( auto& it : fWorkers.get() ) {
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (it.processClass.machineClassFitness(ProcessClass::Storage) <= ProcessClass::WorstFit)
															#line 2568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					dcId_zoneIds[it.locality.dcId()].insert(it.locality.zoneId());
															#line 2572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				}
			}
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			for( auto& region : newConfig.regions ) {
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (dcId_zoneIds[region.dcId].size() < std::max(newConfig.storageTeamSize, newConfig.tLogReplicationFactor))
															#line 2579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::NOT_ENOUGH_WORKERS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::NOT_ENOUGH_WORKERS);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (region.satelliteTLogReplicationFactor > 0 && region.priority >= 0)
															#line 2591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					int totalSatelliteProcesses = 0;
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					for( auto& sat : region.satellites ) {
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
						totalSatelliteProcesses += dcId_zoneIds[sat.dcId].size();
															#line 2599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					}
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (totalSatelliteProcesses < region.satelliteTLogReplicationFactor)
															#line 2603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
						if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::NOT_ENOUGH_WORKERS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
						new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::NOT_ENOUGH_WORKERS);
						this->~ChangeConfigActorState();
						static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
						return 0;
					}
				}
			}
		}
		else
		{
															#line 473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			std::set<Optional<Key>> zoneIds;
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			for( auto& it : fWorkers.get() ) {
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (it.processClass.machineClassFitness(ProcessClass::Storage) <= ProcessClass::WorstFit)
															#line 2624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					zoneIds.insert(it.locality.zoneId());
															#line 2628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				}
			}
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (zoneIds.size() < std::max(newConfig.storageTeamSize, newConfig.tLogReplicationFactor))
															#line 2633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::NOT_ENOUGH_WORKERS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::NOT_ENOUGH_WORKERS);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (newConfig.storageServerStoreType != oldConfig.storageServerStoreType && newConfig.storageMigrationType == StorageMigrationType::DISABLED)
															#line 2646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::STORAGE_MIGRATION_DISABLED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::STORAGE_MIGRATION_DISABLED);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (newConfig.storageMigrationType == StorageMigrationType::GRADUAL && newConfig.perpetualStorageWiggleSpeed == 0)
															#line 2660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				warnPPWGradual = true;
															#line 2664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
			else
			{
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (newConfig.storageServerStoreType != oldConfig.storageServerStoreType && newConfig.storageServerStoreType == KeyValueStoreType::SSD_ROCKSDB_V1)
															#line 2670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					warnRocksDBIsExperimental = true;
															#line 2674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				}
				else
				{
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (newConfig.storageServerStoreType != oldConfig.storageServerStoreType && newConfig.storageServerStoreType == KeyValueStoreType::SSD_SHARDED_ROCKSDB)
															#line 2680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
						warnShardedRocksDBIsExperimental = true;
															#line 2684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					}
				}
			}
		}
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (newConfig.tenantMode != oldConfig.tenantMode)
															#line 2691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Optional<MetaclusterRegistrationEntry>> __when_expr_5 = metacluster::metadata::metaclusterRegistration().get(tr);
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1loopBody1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1loopBody1cont22cont1when1(__when_expr_5.get(), loopDepth); };
			static_cast<ChangeConfigActor*>(this)->actor_wait_state = 6;
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 5, Optional<MetaclusterRegistrationEntry> >*>(static_cast<ChangeConfigActor*>(this)));
															#line 2702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont22cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont22cont1(Void && _,int loopDepth) 
	{
															#line 446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!fWorkers.isReady())
															#line 2716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (newConfig.regions.size())
															#line 2728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			std::map<Optional<Key>, std::set<Optional<Key>>> dcId_zoneIds;
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			for( auto& it : fWorkers.get() ) {
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (it.processClass.machineClassFitness(ProcessClass::Storage) <= ProcessClass::WorstFit)
															#line 2736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					dcId_zoneIds[it.locality.dcId()].insert(it.locality.zoneId());
															#line 2740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				}
			}
															#line 457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			for( auto& region : newConfig.regions ) {
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (dcId_zoneIds[region.dcId].size() < std::max(newConfig.storageTeamSize, newConfig.tLogReplicationFactor))
															#line 2747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::NOT_ENOUGH_WORKERS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::NOT_ENOUGH_WORKERS);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (region.satelliteTLogReplicationFactor > 0 && region.priority >= 0)
															#line 2759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					int totalSatelliteProcesses = 0;
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					for( auto& sat : region.satellites ) {
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
						totalSatelliteProcesses += dcId_zoneIds[sat.dcId].size();
															#line 2767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					}
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (totalSatelliteProcesses < region.satelliteTLogReplicationFactor)
															#line 2771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
						if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::NOT_ENOUGH_WORKERS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
						new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::NOT_ENOUGH_WORKERS);
						this->~ChangeConfigActorState();
						static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
						return 0;
					}
				}
			}
		}
		else
		{
															#line 473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			std::set<Optional<Key>> zoneIds;
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			for( auto& it : fWorkers.get() ) {
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (it.processClass.machineClassFitness(ProcessClass::Storage) <= ProcessClass::WorstFit)
															#line 2792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					zoneIds.insert(it.locality.zoneId());
															#line 2796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				}
			}
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (zoneIds.size() < std::max(newConfig.storageTeamSize, newConfig.tLogReplicationFactor))
															#line 2801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::NOT_ENOUGH_WORKERS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::NOT_ENOUGH_WORKERS);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (newConfig.storageServerStoreType != oldConfig.storageServerStoreType && newConfig.storageMigrationType == StorageMigrationType::DISABLED)
															#line 2814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::STORAGE_MIGRATION_DISABLED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::STORAGE_MIGRATION_DISABLED);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (newConfig.storageMigrationType == StorageMigrationType::GRADUAL && newConfig.perpetualStorageWiggleSpeed == 0)
															#line 2828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				warnPPWGradual = true;
															#line 2832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
			else
			{
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (newConfig.storageServerStoreType != oldConfig.storageServerStoreType && newConfig.storageServerStoreType == KeyValueStoreType::SSD_ROCKSDB_V1)
															#line 2838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					warnRocksDBIsExperimental = true;
															#line 2842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				}
				else
				{
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (newConfig.storageServerStoreType != oldConfig.storageServerStoreType && newConfig.storageServerStoreType == KeyValueStoreType::SSD_SHARDED_ROCKSDB)
															#line 2848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
						warnShardedRocksDBIsExperimental = true;
															#line 2852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					}
				}
			}
		}
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (newConfig.tenantMode != oldConfig.tenantMode)
															#line 2859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Optional<MetaclusterRegistrationEntry>> __when_expr_5 = metacluster::metadata::metaclusterRegistration().get(tr);
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1loopBody1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1loopBody1cont22cont1when1(__when_expr_5.get(), loopDepth); };
			static_cast<ChangeConfigActor*>(this)->actor_wait_state = 6;
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 5, Optional<MetaclusterRegistrationEntry> >*>(static_cast<ChangeConfigActor*>(this)));
															#line 2870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont22cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont22when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont22cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont22when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont22cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont22when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont22when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1loopBody1cont22cont2(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont22cont23(Optional<MetaclusterRegistrationEntry> const& metaclusterRegistration,int loopDepth) 
	{
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (metaclusterRegistration.present())
															#line 2953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_IS_REGISTERED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_IS_REGISTERED);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1loopBody1cont22cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont22cont23(Optional<MetaclusterRegistrationEntry> && metaclusterRegistration,int loopDepth) 
	{
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (metaclusterRegistration.present())
															#line 2971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_IS_REGISTERED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_IS_REGISTERED);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1loopBody1cont22cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont22cont1when1(Optional<MetaclusterRegistrationEntry> const& metaclusterRegistration,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont22cont23(metaclusterRegistration, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont22cont1when1(Optional<MetaclusterRegistrationEntry> && metaclusterRegistration,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont22cont23(std::move(metaclusterRegistration), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 5, Optional<MetaclusterRegistrationEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 5, Optional<MetaclusterRegistrationEntry> >*,Optional<MetaclusterRegistrationEntry> const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1cont22cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 5, Optional<MetaclusterRegistrationEntry> >*,Optional<MetaclusterRegistrationEntry> && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1cont22cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 5, Optional<MetaclusterRegistrationEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1loopBody1cont24(int loopDepth) 
	{
															#line 543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		tr->addReadConflictRange(singleKeyRange(moveKeysLockOwnerKey));
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		tr->set(moveKeysLockOwnerKey, versionKey);
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_8 = safeThreadFutureToFuture(tr->commit());
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_8.isReady()) { if (__when_expr_8.isError()) return a_body1loopBody1Catch1(__when_expr_8.getError(), loopDepth); else return a_body1loopBody1cont24when1(__when_expr_8.get(), loopDepth); };
		static_cast<ChangeConfigActor*>(this)->actor_wait_state = 9;
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_8.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 8, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 3063 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont34(Void const& _,int loopDepth) 
	{
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_7 = wiggleData.resetStorageWiggleMetrics(tr, PrimaryRegion(false));
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1loopBody1Catch1(__when_expr_7.getError(), loopDepth); else return a_body1loopBody1cont34when1(__when_expr_7.get(), loopDepth); };
		static_cast<ChangeConfigActor*>(this)->actor_wait_state = 8;
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 7, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 3079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont34(Void && _,int loopDepth) 
	{
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_7 = wiggleData.resetStorageWiggleMetrics(tr, PrimaryRegion(false));
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1loopBody1Catch1(__when_expr_7.getError(), loopDepth); else return a_body1loopBody1cont34when1(__when_expr_7.get(), loopDepth); };
		static_cast<ChangeConfigActor*>(this)->actor_wait_state = 8;
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 7, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 3095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont34(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont34(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 6);

	}
	int a_body1loopBody1cont34cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont24(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont34cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont24(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont34when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont34cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont34when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont34cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose8() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 7, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 7, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1loopBody1cont34when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 7, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1loopBody1cont34when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 7, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 7);

	}
	int a_body1loopBody1cont24cont1(Void const& _,int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont24cont1(Void && _,int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont24when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont24cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont24when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont24cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose9() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 8, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 8, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose9();
		try {
			a_body1loopBody1cont24when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 8, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose9();
		try {
			a_body1loopBody1cont24when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 8, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose9();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 8);

	}
	int a_body1loopBody1Catch1cont1(int loopDepth) 
	{
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_11 = safeThreadFutureToFuture(tr->onError(e1));
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_11.isReady()) { if (__when_expr_11.isError()) return a_body1Catch1(__when_expr_11.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1cont1when1(__when_expr_11.get(), loopDepth); };
		static_cast<ChangeConfigActor*>(this)->actor_wait_state = 12;
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_11.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 11, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 3324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1Catch1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1(int loopDepth) 
	{
		try {
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			vF = tr->get(initIdKey);
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Optional<Value>> __when_expr_9 = safeThreadFutureToFuture(vF);
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_9.isReady()) { if (__when_expr_9.isError()) return a_body1loopBody1Catch1loopBody1Catch1(__when_expr_9.getError(), loopDepth); else return a_body1loopBody1Catch1loopBody1when1(__when_expr_9.get(), loopDepth); };
			static_cast<ChangeConfigActor*>(this)->actor_wait_state = 10;
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_9.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 9, Optional<Value> >*>(static_cast<ChangeConfigActor*>(this)));
															#line 3358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1Catch1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1Catch1(const Error& e2,int loopDepth=0) 
	{
		try {
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Void> __when_expr_10 = safeThreadFutureToFuture(tr->onError(e2));
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 3382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_10.isReady()) { if (__when_expr_10.isError()) return a_body1Catch1(__when_expr_10.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1Catch1loopBody1Catch1when1(__when_expr_10.get(), loopDepth); };
			static_cast<ChangeConfigActor*>(this)->actor_wait_state = 11;
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_10.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 10, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 3387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 2));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 2));
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1cont2(Optional<Value> const& v,int loopDepth) 
	{
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (v != m[initIdKey.toString()])
															#line 3402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_ALREADY_CREATED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 3406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_ALREADY_CREATED);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (m[configKeysPrefix.toString() + "storage_engine"] == std::to_string(KeyValueStoreType::SSD_ROCKSDB_V1))
															#line 3416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_CREATED_WARN_ROCKSDB_EXPERIMENTAL); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 3420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_CREATED_WARN_ROCKSDB_EXPERIMENTAL);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (m[configKeysPrefix.toString() + "storage_engine"] == std::to_string(KeyValueStoreType::SSD_SHARDED_ROCKSDB))
															#line 3430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_CREATED_WARN_SHARDED_ROCKSDB_EXPERIMENTAL); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 3434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_CREATED_WARN_SHARDED_ROCKSDB_EXPERIMENTAL);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
				else
				{
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_CREATED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 3444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_CREATED);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1cont2(Optional<Value> && v,int loopDepth) 
	{
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (v != m[initIdKey.toString()])
															#line 3459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_ALREADY_CREATED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 3463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_ALREADY_CREATED);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (m[configKeysPrefix.toString() + "storage_engine"] == std::to_string(KeyValueStoreType::SSD_ROCKSDB_V1))
															#line 3473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_CREATED_WARN_ROCKSDB_EXPERIMENTAL); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 3477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_CREATED_WARN_ROCKSDB_EXPERIMENTAL);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (m[configKeysPrefix.toString() + "storage_engine"] == std::to_string(KeyValueStoreType::SSD_SHARDED_ROCKSDB))
															#line 3487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_CREATED_WARN_SHARDED_ROCKSDB_EXPERIMENTAL); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 3491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_CREATED_WARN_SHARDED_ROCKSDB_EXPERIMENTAL);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
				else
				{
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_CREATED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 3501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_CREATED);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1when1(Optional<Value> const& v,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1loopBody1cont2(v, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1when1(Optional<Value> && v,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1loopBody1cont2(std::move(v), loopDepth);

		return loopDepth;
	}
	void a_exitChoose10() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 9, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 9, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 9);
		a_exitChoose10();
		try {
			a_body1loopBody1Catch1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 9);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 9, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 9);
		a_exitChoose10();
		try {
			a_body1loopBody1Catch1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 9);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 9, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 9);
		a_exitChoose10();
		try {
			a_body1loopBody1Catch1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 9);

	}
	int a_body1loopBody1Catch1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose11() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 10, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 10, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 10);
		a_exitChoose11();
		try {
			a_body1loopBody1Catch1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 10);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 10, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 10);
		a_exitChoose11();
		try {
			a_body1loopBody1Catch1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 10);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 10, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 10);
		a_exitChoose11();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 10);

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
	void a_exitChoose12() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 11, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 11, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 11);
		a_exitChoose12();
		try {
			a_body1loopBody1Catch1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 11);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 11, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 11);
		a_exitChoose12();
		try {
			a_body1loopBody1Catch1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 11);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 11, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 11);
		a_exitChoose12();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 11);

	}
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Reference<DB> db;
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	std::map<std::string, std::string> m;
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	bool force;
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	StringRef initIdKey;
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	bool creating;
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Optional<UID> locked;
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Future<Void> tooLong;
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Key versionKey;
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	bool oldReplicationUsesDcId;
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	bool resetPPWStats;
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	bool warnPPWGradual;
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	bool warnRocksDBIsExperimental;
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	bool warnShardedRocksDBIsExperimental;
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> fConfigF;
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Future<RangeResult> fConfig;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> processClassesF;
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> processDataF;
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Future<std::vector<ProcessData>> fWorkers;
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	DatabaseConfiguration oldConfig;
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	DatabaseConfiguration newConfig;
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> fServerListF;
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Future<RangeResult> fServerList;
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> fLocalityListF;
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Future<RangeResult> fLocalityList;
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	std::vector<typename DB::TransactionT::template FutureT<Optional<Value>>> replicasFuturesF;
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	std::vector<Future<Optional<Value>>> replicasFutures;
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	StorageWiggleData wiggleData;
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Error e1;
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<Optional<Value>> vF;
															#line 3785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via changeConfig()
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
class ChangeConfigActor final : public Actor<ConfigurationResult>, public ActorCallback< ChangeConfigActor<DB>, 0, Void >, public ActorCallback< ChangeConfigActor<DB>, 1, Void >, public ActorCallback< ChangeConfigActor<DB>, 2, Void >, public ActorCallback< ChangeConfigActor<DB>, 3, Void >, public ActorCallback< ChangeConfigActor<DB>, 4, Void >, public ActorCallback< ChangeConfigActor<DB>, 5, Optional<MetaclusterRegistrationEntry> >, public ActorCallback< ChangeConfigActor<DB>, 6, Void >, public ActorCallback< ChangeConfigActor<DB>, 7, Void >, public ActorCallback< ChangeConfigActor<DB>, 8, Void >, public ActorCallback< ChangeConfigActor<DB>, 9, Optional<Value> >, public ActorCallback< ChangeConfigActor<DB>, 10, Void >, public ActorCallback< ChangeConfigActor<DB>, 11, Void >, public FastAllocated<ChangeConfigActor<DB>>, public ChangeConfigActorState<DB, ChangeConfigActor<DB>> {
															#line 3792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<ChangeConfigActor<DB>>::operator new;
	using FastAllocated<ChangeConfigActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<ConfigurationResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ChangeConfigActor<DB>, 0, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 1, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 2, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 3, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 4, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 5, Optional<MetaclusterRegistrationEntry> >;
friend struct ActorCallback< ChangeConfigActor<DB>, 6, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 7, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 8, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 9, Optional<Value> >;
friend struct ActorCallback< ChangeConfigActor<DB>, 10, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 11, Void >;
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	ChangeConfigActor(Reference<DB> const& db,std::map<std::string, std::string> const& m,bool const& force) 
															#line 3814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<ConfigurationResult>(),
		   ChangeConfigActorState<DB, ChangeConfigActor<DB>>(db, m, force)
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("changeConfig");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 5, Optional<MetaclusterRegistrationEntry> >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 6, Void >*)0, actor_cancelled()); break;
		case 8: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 7, Void >*)0, actor_cancelled()); break;
		case 9: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 8, Void >*)0, actor_cancelled()); break;
		case 10: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 9, Optional<Value> >*)0, actor_cancelled()); break;
		case 11: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 10, Void >*)0, actor_cancelled()); break;
		case 12: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 11, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<ConfigurationResult> changeConfig( Reference<DB> const& db, std::map<std::string, std::string> const& m, bool const& force ) {
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	return Future<ConfigurationResult>(new ChangeConfigActor<DB>(db, m, force));
															#line 3854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"

															#line 3859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via autoConfig()
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
template <class DB, class AutoConfigActor>
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
class AutoConfigActorState {
															#line 3865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	AutoConfigActorState(Reference<DB> const& db,ConfigureAutoResult const& conf) 
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		 : db(db),
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   conf(conf),
															#line 595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   tr(db->createTransaction()),
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		   versionKey(BinaryWriter::toValue(deterministicRandom()->randomUniqueID(), Unversioned()))
															#line 3878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("autoConfig", reinterpret_cast<unsigned long>(this));

	}
	~AutoConfigActorState() 
	{
		fdb_probe_actor_destroy("autoConfig", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (!conf.address_class.size())
															#line 3893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<AutoConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::INCOMPLETE_CONFIGURATION); this->~AutoConfigActorState(); static_cast<AutoConfigActor*>(this)->destroy(); return 0; }
															#line 3897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<AutoConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::INCOMPLETE_CONFIGURATION);
				this->~AutoConfigActorState();
				static_cast<AutoConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			;
															#line 3905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
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
		this->~AutoConfigActorState();
		static_cast<AutoConfigActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			processClassesF = typename DB::TransactionT::template FutureT<RangeResult>();
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			processDataF = typename DB::TransactionT::template FutureT<RangeResult>();
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<std::vector<ProcessData>> __when_expr_0 = getWorkers(tr, processClassesF, processDataF);
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<AutoConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AutoConfigActor*>(this)->actor_wait_state = 1;
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AutoConfigActor, 0, std::vector<ProcessData> >*>(static_cast<AutoConfigActor*>(this)));
															#line 3955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
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
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->onError(e));
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<AutoConfigActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<AutoConfigActor*>(this)->actor_wait_state = 3;
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< AutoConfigActor, 2, Void >*>(static_cast<AutoConfigActor*>(this)));
															#line 3984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::vector<ProcessData> const& workers,int loopDepth) 
	{
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		std::map<NetworkAddress, Optional<Standalone<StringRef>>> address_processId;
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for( auto& w : workers ) {
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			address_processId[w.address] = w.locality.processId();
															#line 4003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : conf.address_class ) {
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (it.second.classSource() == ProcessClass::CommandLineSource)
															#line 4009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->clear(processClassKeyFor(address_processId[it.first].get()));
															#line 4013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
			else
			{
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->set(processClassKeyFor(address_processId[it.first].get()), processClassValue(it.second));
															#line 4019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (conf.address_class.size())
															#line 4024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(processClassChangeKey, deterministicRandom()->randomUniqueID().toString());
															#line 4028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_logs != conf.old_logs)
															#line 4032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_logs", format("%d", conf.auto_logs));
															#line 4036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_commit_proxies != conf.old_commit_proxies)
															#line 4040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_commit_proxies", format("%d", conf.auto_commit_proxies));
															#line 4044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_grv_proxies != conf.old_grv_proxies)
															#line 4048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_grv_proxies", format("%d", conf.auto_grv_proxies));
															#line 4052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_resolvers != conf.old_resolvers)
															#line 4056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_resolvers", format("%d", conf.auto_resolvers));
															#line 4060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_replication != conf.old_replication)
															#line 4064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			std::vector<StringRef> modes;
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			modes.push_back(conf.auto_replication);
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			std::map<std::string, std::string> m;
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			auto r = buildConfiguration(modes, m);
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (r != ConfigurationResult::SUCCESS)
															#line 4076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<AutoConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(r); this->~AutoConfigActorState(); static_cast<AutoConfigActor*>(this)->destroy(); return 0; }
															#line 4080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<AutoConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(r);
				this->~AutoConfigActorState();
				static_cast<AutoConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			for( auto& kv : m ) {
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->set(kv.first, kv.second);
															#line 4090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		tr->addReadConflictRange(singleKeyRange(moveKeysLockOwnerKey));
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		tr->set(moveKeysLockOwnerKey, versionKey);
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->commit());
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<AutoConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<AutoConfigActor*>(this)->actor_wait_state = 2;
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AutoConfigActor, 1, Void >*>(static_cast<AutoConfigActor*>(this)));
															#line 4106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::vector<ProcessData> && workers,int loopDepth) 
	{
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		std::map<NetworkAddress, Optional<Standalone<StringRef>>> address_processId;
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for( auto& w : workers ) {
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			address_processId[w.address] = w.locality.processId();
															#line 4119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : conf.address_class ) {
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (it.second.classSource() == ProcessClass::CommandLineSource)
															#line 4125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->clear(processClassKeyFor(address_processId[it.first].get()));
															#line 4129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
			else
			{
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->set(processClassKeyFor(address_processId[it.first].get()), processClassValue(it.second));
															#line 4135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (conf.address_class.size())
															#line 4140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(processClassChangeKey, deterministicRandom()->randomUniqueID().toString());
															#line 4144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_logs != conf.old_logs)
															#line 4148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_logs", format("%d", conf.auto_logs));
															#line 4152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_commit_proxies != conf.old_commit_proxies)
															#line 4156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_commit_proxies", format("%d", conf.auto_commit_proxies));
															#line 4160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_grv_proxies != conf.old_grv_proxies)
															#line 4164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_grv_proxies", format("%d", conf.auto_grv_proxies));
															#line 4168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_resolvers != conf.old_resolvers)
															#line 4172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_resolvers", format("%d", conf.auto_resolvers));
															#line 4176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_replication != conf.old_replication)
															#line 4180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			std::vector<StringRef> modes;
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			modes.push_back(conf.auto_replication);
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			std::map<std::string, std::string> m;
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			auto r = buildConfiguration(modes, m);
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			if (r != ConfigurationResult::SUCCESS)
															#line 4192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<AutoConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(r); this->~AutoConfigActorState(); static_cast<AutoConfigActor*>(this)->destroy(); return 0; }
															#line 4196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<AutoConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(r);
				this->~AutoConfigActorState();
				static_cast<AutoConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
			for( auto& kv : m ) {
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
				tr->set(kv.first, kv.second);
															#line 4206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		tr->addReadConflictRange(singleKeyRange(moveKeysLockOwnerKey));
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		tr->set(moveKeysLockOwnerKey, versionKey);
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->commit());
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<AutoConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<AutoConfigActor*>(this)->actor_wait_state = 2;
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AutoConfigActor, 1, Void >*>(static_cast<AutoConfigActor*>(this)));
															#line 4222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<ProcessData> const& workers,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(workers, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<ProcessData> && workers,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(workers), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<AutoConfigActor*>(this)->actor_wait_state > 0) static_cast<AutoConfigActor*>(this)->actor_wait_state = 0;
		static_cast<AutoConfigActor*>(this)->ActorCallback< AutoConfigActor, 0, std::vector<ProcessData> >::remove();

	}
	void a_callback_fire(ActorCallback< AutoConfigActor, 0, std::vector<ProcessData> >*,std::vector<ProcessData> const& value) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AutoConfigActor, 0, std::vector<ProcessData> >*,std::vector<ProcessData> && value) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AutoConfigActor, 0, std::vector<ProcessData> >*,Error err) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<AutoConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::SUCCESS); this->~AutoConfigActorState(); static_cast<AutoConfigActor*>(this)->destroy(); return 0; }
															#line 4294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<AutoConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::SUCCESS);
		this->~AutoConfigActorState();
		static_cast<AutoConfigActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<AutoConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::SUCCESS); this->~AutoConfigActorState(); static_cast<AutoConfigActor*>(this)->destroy(); return 0; }
															#line 4306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<AutoConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::SUCCESS);
		this->~AutoConfigActorState();
		static_cast<AutoConfigActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<AutoConfigActor*>(this)->actor_wait_state > 0) static_cast<AutoConfigActor*>(this)->actor_wait_state = 0;
		static_cast<AutoConfigActor*>(this)->ActorCallback< AutoConfigActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AutoConfigActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< AutoConfigActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< AutoConfigActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<AutoConfigActor*>(this)->actor_wait_state > 0) static_cast<AutoConfigActor*>(this)->actor_wait_state = 0;
		static_cast<AutoConfigActor*>(this)->ActorCallback< AutoConfigActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AutoConfigActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< AutoConfigActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< AutoConfigActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Reference<DB> db;
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	ConfigureAutoResult conf;
															#line 595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	Key versionKey;
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> processClassesF;
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> processDataF;
															#line 4464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via autoConfig()
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
class AutoConfigActor final : public Actor<ConfigurationResult>, public ActorCallback< AutoConfigActor<DB>, 0, std::vector<ProcessData> >, public ActorCallback< AutoConfigActor<DB>, 1, Void >, public ActorCallback< AutoConfigActor<DB>, 2, Void >, public FastAllocated<AutoConfigActor<DB>>, public AutoConfigActorState<DB, AutoConfigActor<DB>> {
															#line 4471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<AutoConfigActor<DB>>::operator new;
	using FastAllocated<AutoConfigActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<ConfigurationResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AutoConfigActor<DB>, 0, std::vector<ProcessData> >;
friend struct ActorCallback< AutoConfigActor<DB>, 1, Void >;
friend struct ActorCallback< AutoConfigActor<DB>, 2, Void >;
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	AutoConfigActor(Reference<DB> const& db,ConfigureAutoResult const& conf) 
															#line 4484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<ConfigurationResult>(),
		   AutoConfigActorState<DB, AutoConfigActor<DB>>(db, conf)
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("autoConfig");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AutoConfigActor<DB>, 0, std::vector<ProcessData> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< AutoConfigActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< AutoConfigActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<ConfigurationResult> autoConfig( Reference<DB> const& db, ConfigureAutoResult const& conf ) {
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"
	return Future<ConfigurationResult>(new AutoConfigActor<DB>(db, conf));
															#line 4515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/GenericManagementAPI.actor.h"

// Accepts tokens separated by spaces in a single string
template <class DB>
Future<ConfigurationResult> changeConfig(Reference<DB> db, std::string const& modes, bool force) {
	TraceEvent("ChangeConfig").detail("Mode", modes);
	std::map<std::string, std::string> m;
	auto r = buildConfiguration(modes, m);
	if (r != ConfigurationResult::SUCCESS)
		return r;
	return changeConfig(db, m, force);
}

// Accepts a vector of configuration tokens
template <class DB>
Future<ConfigurationResult> changeConfig(Reference<DB> db,
                                         std::vector<StringRef> const& modes,
                                         Optional<ConfigureAutoResult> const& conf,
                                         bool force) {
	if (modes.size() && modes[0] == "auto"_sr && conf.present()) {
		return autoConfig(db, conf.get());
	}

	std::map<std::string, std::string> m;
	auto r = buildConfiguration(modes, m);
	if (r != ConfigurationResult::SUCCESS)
		return r;
	return changeConfig(db, m, force);
}

// return the corresponding error message for the CoordinatorsResult
// used by special keys and fdbcli
std::string generateErrorMessage(const CoordinatorsResult& res);

} // namespace ManagementAPI

#include "flow/unactorcompiler.h"
#endif
