#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
/*
 * ManagementAPI.actor.cpp
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

#include <cinttypes>
#include <string>
#include <vector>

#include "fmt/format.h"
#include "fdbclient/Knobs.h"
#include "flow/Arena.h"
#include "fdbclient/ClusterConnectionMemoryRecord.h"
#include "fdbclient/FDBOptions.g.h"
#include "fdbclient/FDBTypes.h"
#include "fdbclient/ReadYourWrites.h"
#include "fdbclient/ManagementAPI.actor.h"

#include "fdbclient/SystemData.h"
#include "fdbclient/NativeAPI.actor.h"
#include "fdbclient/CoordinationInterface.h"
#include "fdbclient/DatabaseContext.h"
#include "fdbrpc/simulator.h"
#include "fdbclient/StatusClient.h"
#include "flow/Trace.h"
#include "flow/UnitTest.h"
#include "fdbrpc/ReplicationPolicy.h"
#include "fdbrpc/Replication.h"
#include "fdbclient/Schemas.h"

#include "flow/actorcompiler.h" // This must be the last #include.

bool isInteger(const std::string& s) {
	if (s.empty())
		return false;
	char* p;
	strtol(s.c_str(), &p, 10);
	return (*p == 0);
}

// Defines the mapping between configuration names (as exposed by fdbcli, buildConfiguration()) and actual configuration
// parameters
std::map<std::string, std::string> configForToken(std::string const& mode) {
	std::map<std::string, std::string> out;
	std::string p = configKeysPrefix.toString();

	if (mode == "new") {
		out[p + "initialized"] = "1";
		return out;
	}

	if (mode == "tss") {
		// Set temporary marker in config map to mark that this is a tss configuration and not a normal storage/log
		// configuration. A bit of a hack but reuses the parsing code nicely.
		out[p + "istss"] = "1";
		return out;
	}

	if (mode == "locked") {
		// Setting this key is interpreted as an instruction to use the normal version-stamp-based mechanism for locking
		// the database.
		out[databaseLockedKey.toString()] = deterministicRandom()->randomUniqueID().toString();
		return out;
	}

	size_t pos;

	// key:=value is unvalidated and unchecked
	pos = mode.find(":=");
	if (pos != std::string::npos) {
		out[p + mode.substr(0, pos)] = mode.substr(pos + 2);
		return out;
	}

	// key=value is constrained to a limited set of options and basic validation is performed
	pos = mode.find("=");
	if (pos != std::string::npos) {
		std::string key = mode.substr(0, pos);
		std::string value = mode.substr(pos + 1);

		if (key == "proxies" && isInteger(value)) {
			printf("Warning: Proxy role is being split into GRV Proxy and Commit Proxy, now prefer configuring "
			       "'grv_proxies' and 'commit_proxies' separately. Generally we should follow that 'commit_proxies'"
			       " is three times of 'grv_proxies' count and 'grv_proxies' should be not more than 4.\n");
			int proxiesCount = atoi(value.c_str());
			if (proxiesCount == -1) {
				proxiesCount = CLIENT_KNOBS->DEFAULT_AUTO_GRV_PROXIES + CLIENT_KNOBS->DEFAULT_AUTO_COMMIT_PROXIES;
				ASSERT_WE_THINK(proxiesCount >= 2);
			}

			if (proxiesCount < 2) {
				printf("Error: At least 2 proxies (1 GRV proxy and 1 Commit proxy) are required.\n");
				return out;
			}

			int grvProxyCount = std::max(1,
			                             std::min(CLIENT_KNOBS->DEFAULT_MAX_GRV_PROXIES,
			                                      proxiesCount / (CLIENT_KNOBS->DEFAULT_COMMIT_GRV_PROXIES_RATIO + 1)));
			int commitProxyCount = proxiesCount - grvProxyCount;
			ASSERT_WE_THINK(grvProxyCount >= 1 && commitProxyCount >= 1);

			out[p + "grv_proxies"] = std::to_string(grvProxyCount);
			out[p + "commit_proxies"] = std::to_string(commitProxyCount);
			printf("%d proxies are automatically converted into %d GRV proxies and %d Commit proxies.\n",
			       proxiesCount,
			       grvProxyCount,
			       commitProxyCount);

			TraceEvent("DatabaseConfigurationProxiesSpecified")
			    .detail("SpecifiedProxies", atoi(value.c_str()))
			    .detail("EffectiveSpecifiedProxies", proxiesCount)
			    .detail("ConvertedGrvProxies", grvProxyCount)
			    .detail("ConvertedCommitProxies", commitProxyCount);
		}

		if ((key == "logs" || key == "commit_proxies" || key == "grv_proxies" || key == "resolvers" ||
		     key == "remote_logs" || key == "log_routers" || key == "usable_regions" ||
		     key == "repopulate_anti_quorum" || key == "count") &&
		    isInteger(value)) {
			out[p + key] = value;
		}

		if (key == "regions") {
			json_spirit::mValue mv;
			json_spirit::read_string(value, mv);

			StatusObject regionObj;
			regionObj["regions"] = mv;
			out[p + key] =
			    BinaryWriter::toValue(regionObj, IncludeVersion(ProtocolVersion::withRegionConfiguration())).toString();
		}

		if (key == "perpetual_storage_wiggle" && isInteger(value)) {
			int ppWiggle = std::stoi(value);
			if (ppWiggle >= 2 || ppWiggle < 0) {
				printf("Error: Only 0 and 1 are valid values of perpetual_storage_wiggle at present.\n");
				return out;
			}
			out[p + key] = value;
		}
		if (key == "perpetual_storage_wiggle_locality") {
			if (!isValidPerpetualStorageWiggleLocality(value)) {
				printf("Error: perpetual_storage_wiggle_locality should be in <locality_key>:<locality_value> "
				       "format or enter 0 to disable the locality match for wiggling.\n");
				return out;
			}
			out[p + key] = value;
		}
		if (key == "storage_migration_type") {
			StorageMigrationType type;
			if (value == "disabled") {
				type = StorageMigrationType::DISABLED;
			} else if (value == "aggressive") {
				type = StorageMigrationType::AGGRESSIVE;
			} else if (value == "gradual") {
				type = StorageMigrationType::GRADUAL;
			} else {
				printf("Error: Only disabled|aggressive|gradual are valid for storage_migration_type.\n");
				return out;
			}
			out[p + key] = format("%d", type);
		}

		if (key == "blob_granules_enabled") {
			int enabled = std::stoi(value);
			if (enabled != 0 && enabled != 1) {
				printf("Error: Only 0 or 1 are valid values for blob_granules_enabled. "
				       "1 enables blob granules and 0 disables them.\n");
				return out;
			}
			out[p + key] = value;
		}

		if (key == "tenant_mode") {
			TenantMode tenantMode;
			if (value == "disabled") {
				tenantMode = TenantMode::DISABLED;
			} else if (value == "optional_experimental") {
				tenantMode = TenantMode::OPTIONAL_TENANT;
			} else if (value == "required_experimental") {
				tenantMode = TenantMode::REQUIRED;
			} else {
				printf("Error: Only disabled|optional_experimental|required_experimental are valid for tenant_mode.\n");
				return out;
			}
			out[p + key] = format("%d", tenantMode);
		}
		if (key == "exclude") {
			int p = 0;
			while (p < value.size()) {
				int end = value.find_first_of(',', p);
				if (end == value.npos) {
					end = value.size();
				}
				auto addrRef = StringRef(value).substr(p, end - p);
				AddressExclusion addr = AddressExclusion::parse(addrRef);
				if (addr.isValid()) {
					out[encodeExcludedServersKey(addr)] = "";
				} else {
					printf("Error: invalid address format: %s\n", addrRef.toString().c_str());
				}
				p = end + 1;
			}
		}

		if (key == "perpetual_storage_wiggle_engine") {
			StringRef s = value;

			// Parse as engine_name[:p=v]... to handle future storage engine params
			Value engine = s.eat(":");
			std::map<Key, Value> params;
			while (!s.empty()) {
				params[s.eat("=")] = s.eat(":");
			}

			try {
				out[p + key] = format("%d", KeyValueStoreType::fromString(engine.toString()).storeType());
			} catch (Error& e) {
				printf("Error: Invalid value for %s (%s): %s\n", key.c_str(), value.c_str(), e.what());
			}
			return out;
		}
		return out;
	}

	Optional<KeyValueStoreType> logType;
	Optional<KeyValueStoreType> storeType;
	if (mode == "ssd-1") {
		logType = KeyValueStoreType::SSD_BTREE_V1;
		storeType = KeyValueStoreType::SSD_BTREE_V1;
	} else if (mode == "ssd" || mode == "ssd-2") {
		logType = KeyValueStoreType::SSD_BTREE_V2;
		storeType = KeyValueStoreType::SSD_BTREE_V2;
	} else if (mode == "ssd-redwood-1-experimental") {
		logType = KeyValueStoreType::SSD_BTREE_V2;
		storeType = KeyValueStoreType::SSD_REDWOOD_V1;
	} else if (mode == "ssd-rocksdb-v1") {
		logType = KeyValueStoreType::SSD_BTREE_V2;
		storeType = KeyValueStoreType::SSD_ROCKSDB_V1;
	} else if (mode == "memory" || mode == "memory-2") {
		logType = KeyValueStoreType::SSD_BTREE_V2;
		storeType = KeyValueStoreType::MEMORY;
	} else if (mode == "memory-1") {
		logType = KeyValueStoreType::MEMORY;
		storeType = KeyValueStoreType::MEMORY;
	} else if (mode == "memory-radixtree-beta") {
		logType = KeyValueStoreType::SSD_BTREE_V2;
		storeType = KeyValueStoreType::MEMORY_RADIXTREE;
	}
	// Add any new store types to fdbserver/workloads/ConfigureDatabase, too

	if (storeType.present()) {
		out[p + "log_engine"] = format("%d", logType.get().storeType());
		out[p + "storage_engine"] = format("%d", KeyValueStoreType::StoreType(storeType.get()));
		return out;
	}

	std::string redundancy, log_replicas;
	Reference<IReplicationPolicy> storagePolicy;
	Reference<IReplicationPolicy> tLogPolicy;

	bool redundancySpecified = true;
	if (mode == "single") {
		redundancy = "1";
		log_replicas = "1";
		storagePolicy = tLogPolicy = Reference<IReplicationPolicy>(new PolicyOne());

	} else if (mode == "double" || mode == "fast_recovery_double") {
		redundancy = "2";
		log_replicas = "2";
		storagePolicy = tLogPolicy = Reference<IReplicationPolicy>(
		    new PolicyAcross(2, "zoneid", Reference<IReplicationPolicy>(new PolicyOne())));
	} else if (mode == "triple" || mode == "fast_recovery_triple") {
		redundancy = "3";
		log_replicas = "3";
		storagePolicy = tLogPolicy = Reference<IReplicationPolicy>(
		    new PolicyAcross(3, "zoneid", Reference<IReplicationPolicy>(new PolicyOne())));
	} else if (mode == "three_datacenter" || mode == "multi_dc") {
		redundancy = "6";
		log_replicas = "4";
		storagePolicy = Reference<IReplicationPolicy>(
		    new PolicyAcross(3,
		                     "dcid",
		                     Reference<IReplicationPolicy>(
		                         new PolicyAcross(2, "zoneid", Reference<IReplicationPolicy>(new PolicyOne())))));
		tLogPolicy = Reference<IReplicationPolicy>(
		    new PolicyAcross(2,
		                     "dcid",
		                     Reference<IReplicationPolicy>(
		                         new PolicyAcross(2, "zoneid", Reference<IReplicationPolicy>(new PolicyOne())))));
	} else if (mode == "three_datacenter_fallback") {
		redundancy = "4";
		log_replicas = "4";
		storagePolicy = tLogPolicy = Reference<IReplicationPolicy>(
		    new PolicyAcross(2,
		                     "dcid",
		                     Reference<IReplicationPolicy>(
		                         new PolicyAcross(2, "zoneid", Reference<IReplicationPolicy>(new PolicyOne())))));
	} else if (mode == "three_data_hall") {
		redundancy = "3";
		log_replicas = "4";
		storagePolicy = Reference<IReplicationPolicy>(
		    new PolicyAcross(3, "data_hall", Reference<IReplicationPolicy>(new PolicyOne())));
		tLogPolicy = Reference<IReplicationPolicy>(
		    new PolicyAcross(2,
		                     "data_hall",
		                     Reference<IReplicationPolicy>(
		                         new PolicyAcross(2, "zoneid", Reference<IReplicationPolicy>(new PolicyOne())))));
	} else if (mode == "three_data_hall_fallback") {
		redundancy = "2";
		log_replicas = "4";
		storagePolicy = Reference<IReplicationPolicy>(
		    new PolicyAcross(2, "data_hall", Reference<IReplicationPolicy>(new PolicyOne())));
		tLogPolicy = Reference<IReplicationPolicy>(
		    new PolicyAcross(2,
		                     "data_hall",
		                     Reference<IReplicationPolicy>(
		                         new PolicyAcross(2, "zoneid", Reference<IReplicationPolicy>(new PolicyOne())))));
	} else
		redundancySpecified = false;
	if (redundancySpecified) {
		out[p + "storage_replicas"] = redundancy;
		out[p + "log_replicas"] = log_replicas;
		out[p + "log_anti_quorum"] = "0";

		BinaryWriter policyWriter(IncludeVersion(ProtocolVersion::withReplicationPolicy()));
		serializeReplicationPolicy(policyWriter, storagePolicy);
		out[p + "storage_replication_policy"] = policyWriter.toValue().toString();

		policyWriter = BinaryWriter(IncludeVersion(ProtocolVersion::withReplicationPolicy()));
		serializeReplicationPolicy(policyWriter, tLogPolicy);
		out[p + "log_replication_policy"] = policyWriter.toValue().toString();
		return out;
	}

	std::string remote_redundancy, remote_log_replicas;
	Reference<IReplicationPolicy> remoteTLogPolicy;
	bool remoteRedundancySpecified = true;
	if (mode == "remote_default") {
		remote_redundancy = "0";
		remote_log_replicas = "0";
		remoteTLogPolicy = Reference<IReplicationPolicy>();
	} else if (mode == "remote_single") {
		remote_redundancy = "1";
		remote_log_replicas = "1";
		remoteTLogPolicy = Reference<IReplicationPolicy>(new PolicyOne());
	} else if (mode == "remote_double") {
		remote_redundancy = "2";
		remote_log_replicas = "2";
		remoteTLogPolicy = Reference<IReplicationPolicy>(
		    new PolicyAcross(2, "zoneid", Reference<IReplicationPolicy>(new PolicyOne())));
	} else if (mode == "remote_triple") {
		remote_redundancy = "3";
		remote_log_replicas = "3";
		remoteTLogPolicy = Reference<IReplicationPolicy>(
		    new PolicyAcross(3, "zoneid", Reference<IReplicationPolicy>(new PolicyOne())));
	} else if (mode == "remote_three_data_hall") { // FIXME: not tested in simulation
		remote_redundancy = "3";
		remote_log_replicas = "4";
		remoteTLogPolicy = Reference<IReplicationPolicy>(
		    new PolicyAcross(2,
		                     "data_hall",
		                     Reference<IReplicationPolicy>(
		                         new PolicyAcross(2, "zoneid", Reference<IReplicationPolicy>(new PolicyOne())))));
	} else
		remoteRedundancySpecified = false;
	if (remoteRedundancySpecified) {
		out[p + "remote_log_replicas"] = remote_log_replicas;

		BinaryWriter policyWriter(IncludeVersion(ProtocolVersion::withReplicationPolicy()));
		serializeReplicationPolicy(policyWriter, remoteTLogPolicy);
		out[p + "remote_log_policy"] = policyWriter.toValue().toString();
		return out;
	}

	return out;
}

ConfigurationResult buildConfiguration(std::vector<StringRef> const& modeTokens,
                                       std::map<std::string, std::string>& outConf) {
	for (auto it : modeTokens) {
		std::string mode = it.toString();
		auto m = configForToken(mode);
		if (!m.size()) {
			TraceEvent(SevWarnAlways, "UnknownOption").detail("Option", mode);
			return ConfigurationResult::UNKNOWN_OPTION;
		}

		for (auto t = m.begin(); t != m.end(); ++t) {
			if (outConf.count(t->first)) {
				TraceEvent(SevWarnAlways, "ConflictingOption").detail("Option", t->first);
				return ConfigurationResult::CONFLICTING_OPTIONS;
			}
			outConf[t->first] = t->second;
		}
	}
	auto p = configKeysPrefix.toString();
	if (!outConf.count(p + "storage_replication_policy") && outConf.count(p + "storage_replicas")) {
		int storageCount = stoi(outConf[p + "storage_replicas"]);
		Reference<IReplicationPolicy> storagePolicy = Reference<IReplicationPolicy>(
		    new PolicyAcross(storageCount, "zoneid", Reference<IReplicationPolicy>(new PolicyOne())));
		BinaryWriter policyWriter(IncludeVersion(ProtocolVersion::withReplicationPolicy()));
		serializeReplicationPolicy(policyWriter, storagePolicy);
		outConf[p + "storage_replication_policy"] = policyWriter.toValue().toString();
	}

	if (!outConf.count(p + "log_replication_policy") && outConf.count(p + "log_replicas")) {
		int logCount = stoi(outConf[p + "log_replicas"]);
		Reference<IReplicationPolicy> logPolicy = Reference<IReplicationPolicy>(
		    new PolicyAcross(logCount, "zoneid", Reference<IReplicationPolicy>(new PolicyOne())));
		BinaryWriter policyWriter(IncludeVersion(ProtocolVersion::withReplicationPolicy()));
		serializeReplicationPolicy(policyWriter, logPolicy);
		outConf[p + "log_replication_policy"] = policyWriter.toValue().toString();
	}
	if (outConf.count(p + "istss")) {
		// redo config parameters to be tss config instead of normal config

		// save param values from parsing as a normal config
		bool isNew = outConf.count(p + "initialized");
		Optional<std::string> count;
		Optional<std::string> storageEngine;
		if (outConf.count(p + "count")) {
			count = Optional<std::string>(outConf[p + "count"]);
		}
		if (outConf.count(p + "storage_engine")) {
			storageEngine = Optional<std::string>(outConf[p + "storage_engine"]);
		}

		// A new tss setup must have count + storage engine. An adjustment must have at least one.
		if ((isNew && (!count.present() || !storageEngine.present())) ||
		    (!isNew && !count.present() && !storageEngine.present())) {
			return ConfigurationResult::INCOMPLETE_CONFIGURATION;
		}

		// clear map and only reset tss parameters
		outConf.clear();
		if (count.present()) {
			outConf[p + "tss_count"] = count.get();
		}
		if (storageEngine.present()) {
			outConf[p + "tss_storage_engine"] = storageEngine.get();
		}
	}
	return ConfigurationResult::SUCCESS;
}

ConfigurationResult buildConfiguration(std::string const& configMode, std::map<std::string, std::string>& outConf) {
	std::vector<StringRef> modes;

	int p = 0;
	while (p < configMode.size()) {
		int end = configMode.find_first_of(' ', p);
		if (end == configMode.npos)
			end = configMode.size();
		modes.push_back(StringRef(configMode).substr(p, end - p));
		p = end + 1;
	}

	return buildConfiguration(modes, outConf);
}

bool isCompleteConfiguration(std::map<std::string, std::string> const& options) {
	std::string p = configKeysPrefix.toString();

	return options.count(p + "log_replicas") == 1 && options.count(p + "log_anti_quorum") == 1 &&
	       options.count(p + "storage_replicas") == 1 && options.count(p + "log_engine") == 1 &&
	       options.count(p + "storage_engine") == 1;
}

															#line 487 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via getDatabaseConfiguration()
															#line 485 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class GetDatabaseConfigurationActor>
															#line 485 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetDatabaseConfigurationActorState {
															#line 494 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 485 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetDatabaseConfigurationActorState(Database const& cx) 
															#line 485 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 485 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 486 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 503 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this));

	}
	~GetDatabaseConfigurationActorState() 
	{
		fdb_probe_actor_destroy("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 487 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 518 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~GetDatabaseConfigurationActorState();
		static_cast<GetDatabaseConfigurationActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 489 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::READ_LOCK_AWARE);
															#line 490 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 491 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr.getRange(configKeys, CLIENT_KNOBS->TOO_MANY);
															#line 491 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetDatabaseConfigurationActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 555 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetDatabaseConfigurationActor*>(this)->actor_wait_state = 1;
															#line 491 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetDatabaseConfigurationActor, 0, RangeResult >*>(static_cast<GetDatabaseConfigurationActor*>(this)));
															#line 560 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 497 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.onError(e);
															#line 497 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetDatabaseConfigurationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 584 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetDatabaseConfigurationActor*>(this)->actor_wait_state = 2;
															#line 497 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetDatabaseConfigurationActor, 1, Void >*>(static_cast<GetDatabaseConfigurationActor*>(this)));
															#line 589 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(RangeResult const& res,int loopDepth) 
	{
															#line 492 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(res.size() < CLIENT_KNOBS->TOO_MANY);
															#line 493 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		DatabaseConfiguration config;
															#line 494 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		config.fromKeyValues((VectorRef<KeyValueRef>)res);
															#line 495 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetDatabaseConfigurationActor*>(this)->SAV<DatabaseConfiguration>::futures) { (void)(config); this->~GetDatabaseConfigurationActorState(); static_cast<GetDatabaseConfigurationActor*>(this)->destroy(); return 0; }
															#line 610 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetDatabaseConfigurationActor*>(this)->SAV< DatabaseConfiguration >::value()) DatabaseConfiguration(config);
		this->~GetDatabaseConfigurationActorState();
		static_cast<GetDatabaseConfigurationActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(RangeResult && res,int loopDepth) 
	{
															#line 492 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(res.size() < CLIENT_KNOBS->TOO_MANY);
															#line 493 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		DatabaseConfiguration config;
															#line 494 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		config.fromKeyValues((VectorRef<KeyValueRef>)res);
															#line 495 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetDatabaseConfigurationActor*>(this)->SAV<DatabaseConfiguration>::futures) { (void)(config); this->~GetDatabaseConfigurationActorState(); static_cast<GetDatabaseConfigurationActor*>(this)->destroy(); return 0; }
															#line 628 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetDatabaseConfigurationActor*>(this)->SAV< DatabaseConfiguration >::value()) DatabaseConfiguration(config);
		this->~GetDatabaseConfigurationActorState();
		static_cast<GetDatabaseConfigurationActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& res,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(res, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && res,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(res), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetDatabaseConfigurationActor*>(this)->actor_wait_state > 0) static_cast<GetDatabaseConfigurationActor*>(this)->actor_wait_state = 0;
		static_cast<GetDatabaseConfigurationActor*>(this)->ActorCallback< GetDatabaseConfigurationActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetDatabaseConfigurationActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetDatabaseConfigurationActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetDatabaseConfigurationActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<GetDatabaseConfigurationActor*>(this)->actor_wait_state > 0) static_cast<GetDatabaseConfigurationActor*>(this)->actor_wait_state = 0;
		static_cast<GetDatabaseConfigurationActor*>(this)->ActorCallback< GetDatabaseConfigurationActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetDatabaseConfigurationActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetDatabaseConfigurationActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetDatabaseConfigurationActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 485 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 486 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 778 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via getDatabaseConfiguration()
															#line 485 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetDatabaseConfigurationActor final : public Actor<DatabaseConfiguration>, public ActorCallback< GetDatabaseConfigurationActor, 0, RangeResult >, public ActorCallback< GetDatabaseConfigurationActor, 1, Void >, public FastAllocated<GetDatabaseConfigurationActor>, public GetDatabaseConfigurationActorState<GetDatabaseConfigurationActor> {
															#line 783 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<GetDatabaseConfigurationActor>::operator new;
	using FastAllocated<GetDatabaseConfigurationActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<DatabaseConfiguration>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetDatabaseConfigurationActor, 0, RangeResult >;
friend struct ActorCallback< GetDatabaseConfigurationActor, 1, Void >;
															#line 485 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetDatabaseConfigurationActor(Database const& cx) 
															#line 795 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<DatabaseConfiguration>(),
		   GetDatabaseConfigurationActorState<GetDatabaseConfigurationActor>(cx)
	{
		fdb_probe_actor_enter("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getDatabaseConfiguration");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getDatabaseConfiguration", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetDatabaseConfigurationActor, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetDatabaseConfigurationActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 485 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<DatabaseConfiguration> getDatabaseConfiguration( Database const& cx ) {
															#line 485 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<DatabaseConfiguration>(new GetDatabaseConfigurationActor(cx));
															#line 824 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 501 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

ConfigureAutoResult parseConfig(StatusObject const& status) {
	ConfigureAutoResult result;
	StatusObjectReader statusObj(status);

	StatusObjectReader statusObjCluster;
	if (!statusObj.get("cluster", statusObjCluster))
		return ConfigureAutoResult();

	StatusObjectReader statusObjConfig;
	if (!statusObjCluster.get("configuration", statusObjConfig))
		return ConfigureAutoResult();

	if (!statusObjConfig.get("redundancy.factor", result.old_replication))
		return ConfigureAutoResult();

	result.auto_replication = result.old_replication;

	[[maybe_unused]] int storage_replication;
	int log_replication;
	if (result.old_replication == "single") {
		result.auto_replication = "double";
		storage_replication = 2;
		log_replication = 2;
	} else if (result.old_replication == "double" || result.old_replication == "fast_recovery_double") {
		storage_replication = 2;
		log_replication = 2;
	} else if (result.old_replication == "triple" || result.old_replication == "fast_recovery_triple") {
		storage_replication = 3;
		log_replication = 3;
	} else if (result.old_replication == "three_datacenter") {
		storage_replication = 6;
		log_replication = 4;
	} else if (result.old_replication == "three_datacenter_fallback") {
		storage_replication = 4;
		log_replication = 4;
	} else if (result.old_replication == "three_data_hall") {
		storage_replication = 3;
		log_replication = 4;
	} else if (result.old_replication == "three_data_hall_fallback") {
		storage_replication = 2;
		log_replication = 4;
	} else
		return ConfigureAutoResult();

	StatusObjectReader machinesMap;
	if (!statusObjCluster.get("machines", machinesMap))
		return ConfigureAutoResult();

	std::map<std::string, std::string> machineid_dcid;
	std::set<std::string> datacenters;
	int machineCount = 0;
	for (auto mach : machinesMap.obj()) {
		StatusObjectReader machine(mach.second);
		std::string dcId;
		if (machine.get("datacenter_id", dcId)) {
			machineid_dcid[mach.first] = dcId;
			datacenters.insert(dcId);
		}
		machineCount++;
	}

	result.machines = machineCount;

	if (datacenters.size() > 1)
		return ConfigureAutoResult();

	StatusObjectReader processesMap;
	if (!statusObjCluster.get("processes", processesMap))
		return ConfigureAutoResult();

	std::set<std::string> oldMachinesWithTransaction;
	int oldTransactionProcesses = 0;
	std::map<std::string, std::vector<std::pair<NetworkAddress, ProcessClass>>> machine_processes;
	int processCount = 0;
	for (auto proc : processesMap.obj()) {
		StatusObjectReader process(proc.second);
		if (!process.has("excluded") || !process.last().get_bool()) {
			std::string addrStr;
			if (!process.get("address", addrStr))
				return ConfigureAutoResult();
			std::string class_source;
			if (!process.get("class_source", class_source))
				return ConfigureAutoResult();
			std::string class_type;
			if (!process.get("class_type", class_type))
				return ConfigureAutoResult();
			std::string machineId;
			if (!process.get("machine_id", machineId))
				return ConfigureAutoResult();

			NetworkAddress addr = NetworkAddress::parse(addrStr);
			ProcessClass processClass(class_type, class_source);

			if (processClass.classType() == ProcessClass::TransactionClass ||
			    processClass.classType() == ProcessClass::LogClass) {
				oldMachinesWithTransaction.insert(machineId);
			}

			if (processClass.classType() == ProcessClass::TransactionClass ||
			    processClass.classType() == ProcessClass::CommitProxyClass ||
			    processClass.classType() == ProcessClass::GrvProxyClass ||
			    processClass.classType() == ProcessClass::ResolutionClass ||
			    processClass.classType() == ProcessClass::StatelessClass ||
			    processClass.classType() == ProcessClass::LogClass) {
				oldTransactionProcesses++;
			}

			if (processClass.classSource() == ProcessClass::AutoSource) {
				processClass = ProcessClass(ProcessClass::UnsetClass, ProcessClass::CommandLineSource);
				result.address_class[addr] = processClass;
			}

			if (processClass.classType() != ProcessClass::TesterClass) {
				machine_processes[machineId].emplace_back(addr, processClass);
				processCount++;
			}
		}
	}

	result.processes = processCount;
	result.old_processes_with_transaction = oldTransactionProcesses;
	result.old_machines_with_transaction = oldMachinesWithTransaction.size();

	std::map<std::pair<int, std::string>, std::vector<std::pair<NetworkAddress, ProcessClass>>> count_processes;
	for (auto& it : machine_processes) {
		count_processes[std::make_pair(it.second.size(), it.first)] = it.second;
	}

	std::set<std::string> machinesWithTransaction;
	std::set<std::string> machinesWithStorage;
	int totalTransactionProcesses = 0;
	int existingProxyCount = 0;
	int existingGrvProxyCount = 0;
	int existingResolverCount = 0;
	int existingStatelessCount = 0;
	for (auto& it : machine_processes) {
		for (auto& proc : it.second) {
			if (proc.second == ProcessClass::TransactionClass || proc.second == ProcessClass::LogClass) {
				totalTransactionProcesses++;
				machinesWithTransaction.insert(it.first);
			}
			if (proc.second == ProcessClass::StatelessClass) {
				existingStatelessCount++;
			}
			if (proc.second == ProcessClass::CommitProxyClass) {
				existingProxyCount++;
			}
			if (proc.second == ProcessClass::GrvProxyClass) {
				existingGrvProxyCount++;
			}
			if (proc.second == ProcessClass::ResolutionClass) {
				existingResolverCount++;
			}
			if (proc.second == ProcessClass::StorageClass) {
				machinesWithStorage.insert(it.first);
			}
			if (proc.second == ProcessClass::UnsetClass && proc.second.classSource() == ProcessClass::DBSource) {
				machinesWithStorage.insert(it.first);
			}
		}
	}

	if (processCount < 10)
		return ConfigureAutoResult();

	result.desired_resolvers = 1;
	int resolverCount;
	if (!statusObjConfig.get("resolvers", result.old_resolvers)) {
		result.old_resolvers = CLIENT_KNOBS->DEFAULT_AUTO_RESOLVERS;
		statusObjConfig.get("auto_resolvers", result.old_resolvers);
		result.auto_resolvers = result.desired_resolvers;
		resolverCount = result.auto_resolvers;
	} else {
		result.auto_resolvers = result.old_resolvers;
		resolverCount = result.old_resolvers;
	}

	result.desired_commit_proxies = std::max(std::min(12, processCount / 15), 1);
	int proxyCount;
	if (!statusObjConfig.get("commit_proxies", result.old_commit_proxies)) {
		result.old_commit_proxies = CLIENT_KNOBS->DEFAULT_AUTO_COMMIT_PROXIES;
		statusObjConfig.get("auto_commit_proxies", result.old_commit_proxies);
		result.auto_commit_proxies = result.desired_commit_proxies;
		proxyCount = result.auto_commit_proxies;
	} else {
		result.auto_commit_proxies = result.old_commit_proxies;
		proxyCount = result.old_commit_proxies;
	}

	result.desired_grv_proxies = std::max(std::min(4, processCount / 20), 1);
	int grvProxyCount;
	if (!statusObjConfig.get("grv_proxies", result.old_grv_proxies)) {
		result.old_grv_proxies = CLIENT_KNOBS->DEFAULT_AUTO_GRV_PROXIES;
		statusObjConfig.get("auto_grv_proxies", result.old_grv_proxies);
		result.auto_grv_proxies = result.desired_grv_proxies;
		grvProxyCount = result.auto_grv_proxies;
	} else {
		result.auto_grv_proxies = result.old_grv_proxies;
		grvProxyCount = result.old_grv_proxies;
	}

	result.desired_logs = std::min(12, processCount / 20);
	result.desired_logs = std::max(result.desired_logs, log_replication + 1);
	result.desired_logs = std::min<int>(result.desired_logs, machine_processes.size());
	int logCount;
	if (!statusObjConfig.get("logs", result.old_logs)) {
		result.old_logs = CLIENT_KNOBS->DEFAULT_AUTO_LOGS;
		statusObjConfig.get("auto_logs", result.old_logs);
		result.auto_logs = result.desired_logs;
		logCount = result.auto_logs;
	} else {
		result.auto_logs = result.old_logs;
		logCount = result.old_logs;
	}

	logCount = std::max(logCount, log_replication);

	totalTransactionProcesses += std::min(existingProxyCount, proxyCount);
	totalTransactionProcesses += std::min(existingGrvProxyCount, grvProxyCount);
	totalTransactionProcesses += std::min(existingResolverCount, resolverCount);
	totalTransactionProcesses += existingStatelessCount;

	// if one process on a machine is transaction class, make them all transaction class
	for (auto& it : count_processes) {
		if (machinesWithTransaction.count(it.first.second) && !machinesWithStorage.count(it.first.second)) {
			for (auto& proc : it.second) {
				if (proc.second == ProcessClass::UnsetClass &&
				    proc.second.classSource() == ProcessClass::CommandLineSource) {
					result.address_class[proc.first] =
					    ProcessClass(ProcessClass::TransactionClass, ProcessClass::AutoSource);
					totalTransactionProcesses++;
				}
			}
		}
	}

	int desiredTotalTransactionProcesses = logCount + resolverCount + proxyCount + grvProxyCount;

	// add machines with all transaction class until we have enough processes and enough machines
	for (auto& it : count_processes) {
		if (machinesWithTransaction.size() >= logCount && totalTransactionProcesses >= desiredTotalTransactionProcesses)
			break;

		if (!machinesWithTransaction.count(it.first.second) && !machinesWithStorage.count(it.first.second)) {
			for (auto& proc : it.second) {
				if (proc.second == ProcessClass::UnsetClass &&
				    proc.second.classSource() == ProcessClass::CommandLineSource) {
					ASSERT(proc.second != ProcessClass::TransactionClass);
					result.address_class[proc.first] =
					    ProcessClass(ProcessClass::TransactionClass, ProcessClass::AutoSource);
					totalTransactionProcesses++;
					machinesWithTransaction.insert(it.first.second);
				}
			}
		}
	}

	if (machinesWithTransaction.size() < logCount || totalTransactionProcesses < desiredTotalTransactionProcesses)
		return ConfigureAutoResult();

	result.auto_processes_with_transaction = totalTransactionProcesses;
	result.auto_machines_with_transaction = machinesWithTransaction.size();

	if (3 * totalTransactionProcesses > processCount)
		return ConfigureAutoResult();

	return result;
}

															#line 1098 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via getWorkers()
															#line 771 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class GetWorkersActor>
															#line 771 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetWorkersActorState {
															#line 1105 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 771 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetWorkersActorState(Transaction* const& tr) 
															#line 771 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 771 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr),
															#line 772 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   processClasses(tr->getRange(processClassKeys, CLIENT_KNOBS->TOO_MANY)),
															#line 773 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   processData(tr->getRange(workerListKeys, CLIENT_KNOBS->TOO_MANY))
															#line 1116 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 775 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_0 = success(processClasses) && success(processData);
															#line 775 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetWorkersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetWorkersActor*>(this)->actor_wait_state = 1;
															#line 775 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetWorkersActor, 0, Void >*>(static_cast<GetWorkersActor*>(this)));
															#line 1138 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 776 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(!processClasses.get().more && processClasses.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 777 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(!processData.get().more && processData.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 779 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::map<Optional<Standalone<StringRef>>, ProcessClass> id_class;
															#line 780 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(int i = 0;i < processClasses.get().size();i++) {
															#line 781 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			id_class[decodeProcessClassKey(processClasses.get()[i].key)] = decodeProcessClassValue(processClasses.get()[i].value);
															#line 1169 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 785 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::vector<ProcessData> results;
															#line 787 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(int i = 0;i < processData.get().size();i++) {
															#line 788 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ProcessData data = decodeWorkerListValue(processData.get()[i].value);
															#line 789 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ProcessClass processClass = id_class[data.locality.processId()];
															#line 791 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (processClass.classSource() == ProcessClass::DBSource || data.processClass.classType() == ProcessClass::UnsetClass)
															#line 1181 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 793 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				data.processClass = processClass;
															#line 1185 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 795 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (data.processClass.classType() != ProcessClass::TesterClass)
															#line 1189 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 796 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				results.push_back(data);
															#line 1193 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 799 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetWorkersActor*>(this)->SAV<std::vector<ProcessData>>::futures) { (void)(results); this->~GetWorkersActorState(); static_cast<GetWorkersActor*>(this)->destroy(); return 0; }
															#line 1198 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetWorkersActor*>(this)->SAV< std::vector<ProcessData> >::value()) std::vector<ProcessData>(results);
		this->~GetWorkersActorState();
		static_cast<GetWorkersActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 776 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(!processClasses.get().more && processClasses.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 777 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(!processData.get().more && processData.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 779 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::map<Optional<Standalone<StringRef>>, ProcessClass> id_class;
															#line 780 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(int i = 0;i < processClasses.get().size();i++) {
															#line 781 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			id_class[decodeProcessClassKey(processClasses.get()[i].key)] = decodeProcessClassValue(processClasses.get()[i].value);
															#line 1218 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 785 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::vector<ProcessData> results;
															#line 787 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(int i = 0;i < processData.get().size();i++) {
															#line 788 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ProcessData data = decodeWorkerListValue(processData.get()[i].value);
															#line 789 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ProcessClass processClass = id_class[data.locality.processId()];
															#line 791 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (processClass.classSource() == ProcessClass::DBSource || data.processClass.classType() == ProcessClass::UnsetClass)
															#line 1230 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 793 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				data.processClass = processClass;
															#line 1234 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 795 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (data.processClass.classType() != ProcessClass::TesterClass)
															#line 1238 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 796 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				results.push_back(data);
															#line 1242 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 799 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetWorkersActor*>(this)->SAV<std::vector<ProcessData>>::futures) { (void)(results); this->~GetWorkersActorState(); static_cast<GetWorkersActor*>(this)->destroy(); return 0; }
															#line 1247 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 771 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 772 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Future<RangeResult> processClasses;
															#line 773 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Future<RangeResult> processData;
															#line 1324 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via getWorkers()
															#line 771 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetWorkersActor final : public Actor<std::vector<ProcessData>>, public ActorCallback< GetWorkersActor, 0, Void >, public FastAllocated<GetWorkersActor>, public GetWorkersActorState<GetWorkersActor> {
															#line 1329 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<GetWorkersActor>::operator new;
	using FastAllocated<GetWorkersActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<ProcessData>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetWorkersActor, 0, Void >;
															#line 771 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetWorkersActor(Transaction* const& tr) 
															#line 1340 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<std::vector<ProcessData>>(),
		   GetWorkersActorState<GetWorkersActor>(tr)
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
		case 1: this->a_callback_error((ActorCallback< GetWorkersActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 771 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<std::vector<ProcessData>> getWorkers( Transaction* const& tr ) {
															#line 771 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<std::vector<ProcessData>>(new GetWorkersActor(tr));
															#line 1368 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 801 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 1373 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via getWorkers()
															#line 802 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class GetWorkersActor1>
															#line 802 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetWorkersActor1State {
															#line 1380 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 802 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetWorkersActor1State(Database const& cx) 
															#line 802 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 802 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 803 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 1389 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("getWorkers", reinterpret_cast<unsigned long>(this));

	}
	~GetWorkersActor1State() 
	{
		fdb_probe_actor_destroy("getWorkers", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 804 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 1404 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~GetWorkersActor1State();
		static_cast<GetWorkersActor1*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 806 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 807 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 808 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 809 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<std::vector<ProcessData>> __when_expr_0 = getWorkers(&tr);
															#line 809 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetWorkersActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1443 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetWorkersActor1*>(this)->actor_wait_state = 1;
															#line 809 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetWorkersActor1, 0, std::vector<ProcessData> >*>(static_cast<GetWorkersActor1*>(this)));
															#line 1448 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 812 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.onError(e);
															#line 812 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetWorkersActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1472 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetWorkersActor1*>(this)->actor_wait_state = 2;
															#line 812 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetWorkersActor1, 1, Void >*>(static_cast<GetWorkersActor1*>(this)));
															#line 1477 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 810 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetWorkersActor1*>(this)->SAV<std::vector<ProcessData>>::futures) { (void)(workers); this->~GetWorkersActor1State(); static_cast<GetWorkersActor1*>(this)->destroy(); return 0; }
															#line 1492 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetWorkersActor1*>(this)->SAV< std::vector<ProcessData> >::value()) std::vector<ProcessData>(workers);
		this->~GetWorkersActor1State();
		static_cast<GetWorkersActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::vector<ProcessData> && workers,int loopDepth) 
	{
															#line 810 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetWorkersActor1*>(this)->SAV<std::vector<ProcessData>>::futures) { (void)(workers); this->~GetWorkersActor1State(); static_cast<GetWorkersActor1*>(this)->destroy(); return 0; }
															#line 1504 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetWorkersActor1*>(this)->SAV< std::vector<ProcessData> >::value()) std::vector<ProcessData>(workers);
		this->~GetWorkersActor1State();
		static_cast<GetWorkersActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<GetWorkersActor1*>(this)->actor_wait_state > 0) static_cast<GetWorkersActor1*>(this)->actor_wait_state = 0;
		static_cast<GetWorkersActor1*>(this)->ActorCallback< GetWorkersActor1, 0, std::vector<ProcessData> >::remove();

	}
	void a_callback_fire(ActorCallback< GetWorkersActor1, 0, std::vector<ProcessData> >*,std::vector<ProcessData> const& value) 
	{
		fdb_probe_actor_enter("getWorkers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetWorkersActor1, 0, std::vector<ProcessData> >*,std::vector<ProcessData> && value) 
	{
		fdb_probe_actor_enter("getWorkers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetWorkersActor1, 0, std::vector<ProcessData> >*,Error err) 
	{
		fdb_probe_actor_enter("getWorkers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkers", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<GetWorkersActor1*>(this)->actor_wait_state > 0) static_cast<GetWorkersActor1*>(this)->actor_wait_state = 0;
		static_cast<GetWorkersActor1*>(this)->ActorCallback< GetWorkersActor1, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetWorkersActor1, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getWorkers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkers", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetWorkersActor1, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getWorkers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkers", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetWorkersActor1, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getWorkers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkers", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 802 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 803 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 1654 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via getWorkers()
															#line 802 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetWorkersActor1 final : public Actor<std::vector<ProcessData>>, public ActorCallback< GetWorkersActor1, 0, std::vector<ProcessData> >, public ActorCallback< GetWorkersActor1, 1, Void >, public FastAllocated<GetWorkersActor1>, public GetWorkersActor1State<GetWorkersActor1> {
															#line 1659 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<GetWorkersActor1>::operator new;
	using FastAllocated<GetWorkersActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<ProcessData>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetWorkersActor1, 0, std::vector<ProcessData> >;
friend struct ActorCallback< GetWorkersActor1, 1, Void >;
															#line 802 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetWorkersActor1(Database const& cx) 
															#line 1671 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<std::vector<ProcessData>>(),
		   GetWorkersActor1State<GetWorkersActor1>(cx)
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
		case 1: this->a_callback_error((ActorCallback< GetWorkersActor1, 0, std::vector<ProcessData> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetWorkersActor1, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 802 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<std::vector<ProcessData>> getWorkers( Database const& cx ) {
															#line 802 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<std::vector<ProcessData>>(new GetWorkersActor1(cx));
															#line 1700 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 816 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 1705 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via getConnectionString()
															#line 817 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class GetConnectionStringActor>
															#line 817 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetConnectionStringActorState {
															#line 1712 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 817 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetConnectionStringActorState(Database const& cx) 
															#line 817 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 817 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 818 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 1721 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("getConnectionString", reinterpret_cast<unsigned long>(this));

	}
	~GetConnectionStringActorState() 
	{
		fdb_probe_actor_destroy("getConnectionString", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 819 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 1736 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~GetConnectionStringActorState();
		static_cast<GetConnectionStringActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 821 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::READ_LOCK_AWARE);
															#line 822 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 823 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(coordinatorsKey);
															#line 823 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetConnectionStringActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1773 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetConnectionStringActor*>(this)->actor_wait_state = 1;
															#line 823 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetConnectionStringActor, 0, Optional<Value> >*>(static_cast<GetConnectionStringActor*>(this)));
															#line 1778 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 828 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.onError(e);
															#line 828 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetConnectionStringActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1802 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetConnectionStringActor*>(this)->actor_wait_state = 2;
															#line 828 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetConnectionStringActor, 1, Void >*>(static_cast<GetConnectionStringActor*>(this)));
															#line 1807 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& currentKey,int loopDepth) 
	{
															#line 824 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!currentKey.present())
															#line 1822 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 825 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<GetConnectionStringActor*>(this)->SAV<Optional<ClusterConnectionString>>::futures) { (void)(Optional<ClusterConnectionString>()); this->~GetConnectionStringActorState(); static_cast<GetConnectionStringActor*>(this)->destroy(); return 0; }
															#line 1826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<GetConnectionStringActor*>(this)->SAV< Optional<ClusterConnectionString> >::value()) Optional<ClusterConnectionString>(Optional<ClusterConnectionString>());
			this->~GetConnectionStringActorState();
			static_cast<GetConnectionStringActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetConnectionStringActor*>(this)->SAV<Optional<ClusterConnectionString>>::futures) { (void)(ClusterConnectionString(currentKey.get().toString())); this->~GetConnectionStringActorState(); static_cast<GetConnectionStringActor*>(this)->destroy(); return 0; }
															#line 1834 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetConnectionStringActor*>(this)->SAV< Optional<ClusterConnectionString> >::value()) Optional<ClusterConnectionString>(ClusterConnectionString(currentKey.get().toString()));
		this->~GetConnectionStringActorState();
		static_cast<GetConnectionStringActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && currentKey,int loopDepth) 
	{
															#line 824 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!currentKey.present())
															#line 1846 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 825 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<GetConnectionStringActor*>(this)->SAV<Optional<ClusterConnectionString>>::futures) { (void)(Optional<ClusterConnectionString>()); this->~GetConnectionStringActorState(); static_cast<GetConnectionStringActor*>(this)->destroy(); return 0; }
															#line 1850 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<GetConnectionStringActor*>(this)->SAV< Optional<ClusterConnectionString> >::value()) Optional<ClusterConnectionString>(Optional<ClusterConnectionString>());
			this->~GetConnectionStringActorState();
			static_cast<GetConnectionStringActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetConnectionStringActor*>(this)->SAV<Optional<ClusterConnectionString>>::futures) { (void)(ClusterConnectionString(currentKey.get().toString())); this->~GetConnectionStringActorState(); static_cast<GetConnectionStringActor*>(this)->destroy(); return 0; }
															#line 1858 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetConnectionStringActor*>(this)->SAV< Optional<ClusterConnectionString> >::value()) Optional<ClusterConnectionString>(ClusterConnectionString(currentKey.get().toString()));
		this->~GetConnectionStringActorState();
		static_cast<GetConnectionStringActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& currentKey,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(currentKey, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && currentKey,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(currentKey), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetConnectionStringActor*>(this)->actor_wait_state > 0) static_cast<GetConnectionStringActor*>(this)->actor_wait_state = 0;
		static_cast<GetConnectionStringActor*>(this)->ActorCallback< GetConnectionStringActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< GetConnectionStringActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("getConnectionString", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConnectionString", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetConnectionStringActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("getConnectionString", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConnectionString", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetConnectionStringActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("getConnectionString", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConnectionString", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<GetConnectionStringActor*>(this)->actor_wait_state > 0) static_cast<GetConnectionStringActor*>(this)->actor_wait_state = 0;
		static_cast<GetConnectionStringActor*>(this)->ActorCallback< GetConnectionStringActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetConnectionStringActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getConnectionString", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConnectionString", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetConnectionStringActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getConnectionString", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConnectionString", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetConnectionStringActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getConnectionString", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConnectionString", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 817 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 818 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 2008 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via getConnectionString()
															#line 817 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetConnectionStringActor final : public Actor<Optional<ClusterConnectionString>>, public ActorCallback< GetConnectionStringActor, 0, Optional<Value> >, public ActorCallback< GetConnectionStringActor, 1, Void >, public FastAllocated<GetConnectionStringActor>, public GetConnectionStringActorState<GetConnectionStringActor> {
															#line 2013 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<GetConnectionStringActor>::operator new;
	using FastAllocated<GetConnectionStringActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<ClusterConnectionString>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetConnectionStringActor, 0, Optional<Value> >;
friend struct ActorCallback< GetConnectionStringActor, 1, Void >;
															#line 817 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetConnectionStringActor(Database const& cx) 
															#line 2025 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Optional<ClusterConnectionString>>(),
		   GetConnectionStringActorState<GetConnectionStringActor>(cx)
	{
		fdb_probe_actor_enter("getConnectionString", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getConnectionString");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getConnectionString", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetConnectionStringActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetConnectionStringActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 817 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Optional<ClusterConnectionString>> getConnectionString( Database const& cx ) {
															#line 817 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Optional<ClusterConnectionString>>(new GetConnectionStringActor(cx));
															#line 2054 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 832 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 2059 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via changeQuorumChecker()
															#line 833 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class ChangeQuorumCheckerActor>
															#line 833 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ChangeQuorumCheckerActorState {
															#line 2066 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 833 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ChangeQuorumCheckerActorState(Transaction* const& tr,ClusterConnectionString* const& conn,std::string const& newName) 
															#line 833 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 833 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr),
															#line 833 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   conn(conn),
															#line 833 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   newName(newName)
															#line 2077 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("changeQuorumChecker", reinterpret_cast<unsigned long>(this));

	}
	~ChangeQuorumCheckerActorState() 
	{
		fdb_probe_actor_destroy("changeQuorumChecker", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 836 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 837 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 838 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 839 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 840 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(coordinatorsKey);
															#line 840 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2102 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state = 1;
															#line 840 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumCheckerActor, 0, Optional<Value> >*>(static_cast<ChangeQuorumCheckerActor*>(this)));
															#line 2107 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~ChangeQuorumCheckerActorState();
		static_cast<ChangeQuorumCheckerActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& currentKey,int loopDepth) 
	{
															#line 842 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!currentKey.present())
															#line 2130 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 843 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumCheckerActor*>(this)->SAV<Optional<CoordinatorsResult>>::futures) { (void)(CoordinatorsResult::BAD_DATABASE_STATE); this->~ChangeQuorumCheckerActorState(); static_cast<ChangeQuorumCheckerActor*>(this)->destroy(); return 0; }
															#line 2134 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumCheckerActor*>(this)->SAV< Optional<CoordinatorsResult> >::value()) Optional<CoordinatorsResult>(CoordinatorsResult::BAD_DATABASE_STATE);
			this->~ChangeQuorumCheckerActorState();
			static_cast<ChangeQuorumCheckerActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 845 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		old = ClusterConnectionString(currentKey.get().toString());
															#line 846 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (tr->getDatabase()->getConnectionRecord() && old.clusterKeyName().toString() != tr->getDatabase()->getConnectionRecord()->getConnectionString().clusterKeyName())
															#line 2144 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 849 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumCheckerActor*>(this)->SAV<Optional<CoordinatorsResult>>::futures) { (void)(CoordinatorsResult::BAD_DATABASE_STATE); this->~ChangeQuorumCheckerActorState(); static_cast<ChangeQuorumCheckerActor*>(this)->destroy(); return 0; }
															#line 2148 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumCheckerActor*>(this)->SAV< Optional<CoordinatorsResult> >::value()) Optional<CoordinatorsResult>(CoordinatorsResult::BAD_DATABASE_STATE);
			this->~ChangeQuorumCheckerActorState();
			static_cast<ChangeQuorumCheckerActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 851 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (conn->hostnames.size() + conn->coords.size() == 0)
															#line 2156 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 852 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			conn->hostnames = old.hostnames;
															#line 853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			conn->coords = old.coords;
															#line 2162 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 855 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<std::vector<NetworkAddress>> __when_expr_1 = conn->tryResolveHostnames();
															#line 855 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2168 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state = 2;
															#line 855 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumCheckerActor, 1, std::vector<NetworkAddress> >*>(static_cast<ChangeQuorumCheckerActor*>(this)));
															#line 2173 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && currentKey,int loopDepth) 
	{
															#line 842 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!currentKey.present())
															#line 2182 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 843 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumCheckerActor*>(this)->SAV<Optional<CoordinatorsResult>>::futures) { (void)(CoordinatorsResult::BAD_DATABASE_STATE); this->~ChangeQuorumCheckerActorState(); static_cast<ChangeQuorumCheckerActor*>(this)->destroy(); return 0; }
															#line 2186 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumCheckerActor*>(this)->SAV< Optional<CoordinatorsResult> >::value()) Optional<CoordinatorsResult>(CoordinatorsResult::BAD_DATABASE_STATE);
			this->~ChangeQuorumCheckerActorState();
			static_cast<ChangeQuorumCheckerActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 845 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		old = ClusterConnectionString(currentKey.get().toString());
															#line 846 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (tr->getDatabase()->getConnectionRecord() && old.clusterKeyName().toString() != tr->getDatabase()->getConnectionRecord()->getConnectionString().clusterKeyName())
															#line 2196 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 849 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumCheckerActor*>(this)->SAV<Optional<CoordinatorsResult>>::futures) { (void)(CoordinatorsResult::BAD_DATABASE_STATE); this->~ChangeQuorumCheckerActorState(); static_cast<ChangeQuorumCheckerActor*>(this)->destroy(); return 0; }
															#line 2200 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumCheckerActor*>(this)->SAV< Optional<CoordinatorsResult> >::value()) Optional<CoordinatorsResult>(CoordinatorsResult::BAD_DATABASE_STATE);
			this->~ChangeQuorumCheckerActorState();
			static_cast<ChangeQuorumCheckerActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 851 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (conn->hostnames.size() + conn->coords.size() == 0)
															#line 2208 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 852 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			conn->hostnames = old.hostnames;
															#line 853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			conn->coords = old.coords;
															#line 2214 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 855 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<std::vector<NetworkAddress>> __when_expr_1 = conn->tryResolveHostnames();
															#line 855 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2220 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state = 2;
															#line 855 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumCheckerActor, 1, std::vector<NetworkAddress> >*>(static_cast<ChangeQuorumCheckerActor*>(this)));
															#line 2225 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& currentKey,int loopDepth) 
	{
		loopDepth = a_body1cont1(currentKey, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && currentKey,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(currentKey), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state > 0) static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeQuorumCheckerActor*>(this)->ActorCallback< ChangeQuorumCheckerActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeQuorumCheckerActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ChangeQuorumCheckerActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ChangeQuorumCheckerActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(std::vector<NetworkAddress> const& desiredCoordinators,int loopDepth) 
	{
															#line 856 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (desiredCoordinators.size() != conn->hostnames.size() + conn->coords.size())
															#line 2297 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 857 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("ChangeQuorumCheckerEarlyTermination") .detail("Reason", "One or more hostnames are unresolvable") .backtrace();
															#line 860 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumCheckerActor*>(this)->SAV<Optional<CoordinatorsResult>>::futures) { (void)(CoordinatorsResult::COORDINATOR_UNREACHABLE); this->~ChangeQuorumCheckerActorState(); static_cast<ChangeQuorumCheckerActor*>(this)->destroy(); return 0; }
															#line 2303 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumCheckerActor*>(this)->SAV< Optional<CoordinatorsResult> >::value()) Optional<CoordinatorsResult>(CoordinatorsResult::COORDINATOR_UNREACHABLE);
			this->~ChangeQuorumCheckerActorState();
			static_cast<ChangeQuorumCheckerActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 863 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (newName.empty())
															#line 2311 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 864 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			newName = old.clusterKeyName().toString();
															#line 2315 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 866 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::sort(conn->hostnames.begin(), conn->hostnames.end());
															#line 867 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::sort(conn->coords.begin(), conn->coords.end());
															#line 868 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::sort(old.hostnames.begin(), old.hostnames.end());
															#line 869 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::sort(old.coords.begin(), old.coords.end());
															#line 870 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (conn->hostnames == old.hostnames && conn->coords == old.coords && old.clusterKeyName() == newName)
															#line 2327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 871 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumCheckerActor*>(this)->SAV<Optional<CoordinatorsResult>>::futures) { (void)(CoordinatorsResult::SAME_NETWORK_ADDRESSES); this->~ChangeQuorumCheckerActorState(); static_cast<ChangeQuorumCheckerActor*>(this)->destroy(); return 0; }
															#line 2331 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumCheckerActor*>(this)->SAV< Optional<CoordinatorsResult> >::value()) Optional<CoordinatorsResult>(CoordinatorsResult::SAME_NETWORK_ADDRESSES);
			this->~ChangeQuorumCheckerActorState();
			static_cast<ChangeQuorumCheckerActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 874 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		conn->parseKey(newName + ':' + deterministicRandom()->randomAlphaNumeric(32));
															#line 876 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (g_network->isSimulated())
															#line 2341 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 877 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			int i = 0;
															#line 878 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			int protectedCount = 0;
															#line 879 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			for(;(protectedCount < ((desiredCoordinators.size() / 2) + 1)) && (i < desiredCoordinators.size());) {
															#line 880 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				auto process = g_simulator.getProcessByAddress(desiredCoordinators[i]);
															#line 881 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				auto addresses = process->addresses;
															#line 883 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (!process->isReliable())
															#line 2355 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 884 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					i++;
															#line 2359 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					continue;
				}
															#line 888 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				g_simulator.protectedAddresses.insert(process->addresses.address);
															#line 889 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (addresses.secondaryAddress.present())
															#line 2366 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 890 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					g_simulator.protectedAddresses.insert(process->addresses.secondaryAddress.get());
															#line 2370 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
															#line 892 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				TraceEvent("ProtectCoordinator").detail("Address", desiredCoordinators[i]).backtrace();
															#line 893 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				protectedCount++;
															#line 894 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				i++;
															#line 2378 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 898 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::vector<Future<Optional<LeaderInfo>>> leaderServers;
															#line 899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ClientCoordinators coord(Reference<ClusterConnectionMemoryRecord>(new ClusterConnectionMemoryRecord(*conn)));
															#line 901 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		leaderServers.reserve(coord.clientLeaderServers.size());
															#line 902 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(int i = 0;i < coord.clientLeaderServers.size();i++) {
															#line 903 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (coord.clientLeaderServers[i].hostname.present())
															#line 2391 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 904 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				leaderServers.push_back(retryGetReplyFromHostname(GetLeaderRequest(coord.clusterKey, UID()), coord.clientLeaderServers[i].hostname.get(), WLTOKEN_CLIENTLEADERREG_GETLEADER, TaskPriority::CoordinationReply));
															#line 2395 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
			else
			{
															#line 909 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				leaderServers.push_back(retryBrokenPromise(coord.clientLeaderServers[i].getLeader, GetLeaderRequest(coord.clusterKey, UID()), TaskPriority::CoordinationReply));
															#line 2401 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 916 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_2 = waitForAll(leaderServers);
															#line 915 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2408 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
															#line 917 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_3 = delay(5.0);
															#line 2412 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont2when2(__when_expr_3.get(), loopDepth); };
		static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state = 3;
															#line 916 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumCheckerActor, 2, Void >*>(static_cast<ChangeQuorumCheckerActor*>(this)));
															#line 917 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumCheckerActor, 3, Void >*>(static_cast<ChangeQuorumCheckerActor*>(this)));
															#line 2419 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(std::vector<NetworkAddress> && desiredCoordinators,int loopDepth) 
	{
															#line 856 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (desiredCoordinators.size() != conn->hostnames.size() + conn->coords.size())
															#line 2428 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 857 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("ChangeQuorumCheckerEarlyTermination") .detail("Reason", "One or more hostnames are unresolvable") .backtrace();
															#line 860 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumCheckerActor*>(this)->SAV<Optional<CoordinatorsResult>>::futures) { (void)(CoordinatorsResult::COORDINATOR_UNREACHABLE); this->~ChangeQuorumCheckerActorState(); static_cast<ChangeQuorumCheckerActor*>(this)->destroy(); return 0; }
															#line 2434 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumCheckerActor*>(this)->SAV< Optional<CoordinatorsResult> >::value()) Optional<CoordinatorsResult>(CoordinatorsResult::COORDINATOR_UNREACHABLE);
			this->~ChangeQuorumCheckerActorState();
			static_cast<ChangeQuorumCheckerActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 863 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (newName.empty())
															#line 2442 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 864 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			newName = old.clusterKeyName().toString();
															#line 2446 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 866 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::sort(conn->hostnames.begin(), conn->hostnames.end());
															#line 867 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::sort(conn->coords.begin(), conn->coords.end());
															#line 868 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::sort(old.hostnames.begin(), old.hostnames.end());
															#line 869 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::sort(old.coords.begin(), old.coords.end());
															#line 870 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (conn->hostnames == old.hostnames && conn->coords == old.coords && old.clusterKeyName() == newName)
															#line 2458 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 871 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumCheckerActor*>(this)->SAV<Optional<CoordinatorsResult>>::futures) { (void)(CoordinatorsResult::SAME_NETWORK_ADDRESSES); this->~ChangeQuorumCheckerActorState(); static_cast<ChangeQuorumCheckerActor*>(this)->destroy(); return 0; }
															#line 2462 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumCheckerActor*>(this)->SAV< Optional<CoordinatorsResult> >::value()) Optional<CoordinatorsResult>(CoordinatorsResult::SAME_NETWORK_ADDRESSES);
			this->~ChangeQuorumCheckerActorState();
			static_cast<ChangeQuorumCheckerActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 874 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		conn->parseKey(newName + ':' + deterministicRandom()->randomAlphaNumeric(32));
															#line 876 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (g_network->isSimulated())
															#line 2472 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 877 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			int i = 0;
															#line 878 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			int protectedCount = 0;
															#line 879 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			for(;(protectedCount < ((desiredCoordinators.size() / 2) + 1)) && (i < desiredCoordinators.size());) {
															#line 880 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				auto process = g_simulator.getProcessByAddress(desiredCoordinators[i]);
															#line 881 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				auto addresses = process->addresses;
															#line 883 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (!process->isReliable())
															#line 2486 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 884 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					i++;
															#line 2490 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					continue;
				}
															#line 888 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				g_simulator.protectedAddresses.insert(process->addresses.address);
															#line 889 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (addresses.secondaryAddress.present())
															#line 2497 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 890 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					g_simulator.protectedAddresses.insert(process->addresses.secondaryAddress.get());
															#line 2501 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
															#line 892 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				TraceEvent("ProtectCoordinator").detail("Address", desiredCoordinators[i]).backtrace();
															#line 893 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				protectedCount++;
															#line 894 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				i++;
															#line 2509 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 898 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::vector<Future<Optional<LeaderInfo>>> leaderServers;
															#line 899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ClientCoordinators coord(Reference<ClusterConnectionMemoryRecord>(new ClusterConnectionMemoryRecord(*conn)));
															#line 901 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		leaderServers.reserve(coord.clientLeaderServers.size());
															#line 902 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(int i = 0;i < coord.clientLeaderServers.size();i++) {
															#line 903 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (coord.clientLeaderServers[i].hostname.present())
															#line 2522 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 904 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				leaderServers.push_back(retryGetReplyFromHostname(GetLeaderRequest(coord.clusterKey, UID()), coord.clientLeaderServers[i].hostname.get(), WLTOKEN_CLIENTLEADERREG_GETLEADER, TaskPriority::CoordinationReply));
															#line 2526 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
			else
			{
															#line 909 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				leaderServers.push_back(retryBrokenPromise(coord.clientLeaderServers[i].getLeader, GetLeaderRequest(coord.clusterKey, UID()), TaskPriority::CoordinationReply));
															#line 2532 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 916 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_2 = waitForAll(leaderServers);
															#line 915 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2539 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
															#line 917 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_3 = delay(5.0);
															#line 2543 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont2when2(__when_expr_3.get(), loopDepth); };
		static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state = 3;
															#line 916 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumCheckerActor, 2, Void >*>(static_cast<ChangeQuorumCheckerActor*>(this)));
															#line 917 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumCheckerActor, 3, Void >*>(static_cast<ChangeQuorumCheckerActor*>(this)));
															#line 2550 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(std::vector<NetworkAddress> const& desiredCoordinators,int loopDepth) 
	{
		loopDepth = a_body1cont2(desiredCoordinators, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(std::vector<NetworkAddress> && desiredCoordinators,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(desiredCoordinators), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state > 0) static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeQuorumCheckerActor*>(this)->ActorCallback< ChangeQuorumCheckerActor, 1, std::vector<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeQuorumCheckerActor, 1, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> const& value) 
	{
		fdb_probe_actor_enter("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ChangeQuorumCheckerActor, 1, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ChangeQuorumCheckerActor, 1, std::vector<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont6(int loopDepth) 
	{
															#line 921 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr->set(coordinatorsKey, conn->toString());
															#line 922 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ChangeQuorumCheckerActor*>(this)->SAV<Optional<CoordinatorsResult>>::futures) { (void)(Optional<CoordinatorsResult>()); this->~ChangeQuorumCheckerActorState(); static_cast<ChangeQuorumCheckerActor*>(this)->destroy(); return 0; }
															#line 2624 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ChangeQuorumCheckerActor*>(this)->SAV< Optional<CoordinatorsResult> >::value()) Optional<CoordinatorsResult>(Optional<CoordinatorsResult>());
		this->~ChangeQuorumCheckerActorState();
		static_cast<ChangeQuorumCheckerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when2(Void const& _,int loopDepth) 
	{
															#line 918 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ChangeQuorumCheckerActor*>(this)->SAV<Optional<CoordinatorsResult>>::futures) { (void)(CoordinatorsResult::COORDINATOR_UNREACHABLE); this->~ChangeQuorumCheckerActorState(); static_cast<ChangeQuorumCheckerActor*>(this)->destroy(); return 0; }
															#line 2648 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ChangeQuorumCheckerActor*>(this)->SAV< Optional<CoordinatorsResult> >::value()) Optional<CoordinatorsResult>(CoordinatorsResult::COORDINATOR_UNREACHABLE);
		this->~ChangeQuorumCheckerActorState();
		static_cast<ChangeQuorumCheckerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when2(Void && _,int loopDepth) 
	{
															#line 918 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ChangeQuorumCheckerActor*>(this)->SAV<Optional<CoordinatorsResult>>::futures) { (void)(CoordinatorsResult::COORDINATOR_UNREACHABLE); this->~ChangeQuorumCheckerActorState(); static_cast<ChangeQuorumCheckerActor*>(this)->destroy(); return 0; }
															#line 2660 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ChangeQuorumCheckerActor*>(this)->SAV< Optional<CoordinatorsResult> >::value()) Optional<CoordinatorsResult>(CoordinatorsResult::COORDINATOR_UNREACHABLE);
		this->~ChangeQuorumCheckerActorState();
		static_cast<ChangeQuorumCheckerActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state > 0) static_cast<ChangeQuorumCheckerActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeQuorumCheckerActor*>(this)->ActorCallback< ChangeQuorumCheckerActor, 2, Void >::remove();
		static_cast<ChangeQuorumCheckerActor*>(this)->ActorCallback< ChangeQuorumCheckerActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeQuorumCheckerActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ChangeQuorumCheckerActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ChangeQuorumCheckerActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ChangeQuorumCheckerActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose3();
		try {
			a_body1cont2when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ChangeQuorumCheckerActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose3();
		try {
			a_body1cont2when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ChangeQuorumCheckerActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorumChecker", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 833 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 833 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ClusterConnectionString* conn;
															#line 833 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::string newName;
															#line 845 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ClusterConnectionString old;
															#line 2773 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via changeQuorumChecker()
															#line 833 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ChangeQuorumCheckerActor final : public Actor<Optional<CoordinatorsResult>>, public ActorCallback< ChangeQuorumCheckerActor, 0, Optional<Value> >, public ActorCallback< ChangeQuorumCheckerActor, 1, std::vector<NetworkAddress> >, public ActorCallback< ChangeQuorumCheckerActor, 2, Void >, public ActorCallback< ChangeQuorumCheckerActor, 3, Void >, public FastAllocated<ChangeQuorumCheckerActor>, public ChangeQuorumCheckerActorState<ChangeQuorumCheckerActor> {
															#line 2778 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<ChangeQuorumCheckerActor>::operator new;
	using FastAllocated<ChangeQuorumCheckerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<CoordinatorsResult>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ChangeQuorumCheckerActor, 0, Optional<Value> >;
friend struct ActorCallback< ChangeQuorumCheckerActor, 1, std::vector<NetworkAddress> >;
friend struct ActorCallback< ChangeQuorumCheckerActor, 2, Void >;
friend struct ActorCallback< ChangeQuorumCheckerActor, 3, Void >;
															#line 833 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ChangeQuorumCheckerActor(Transaction* const& tr,ClusterConnectionString* const& conn,std::string const& newName) 
															#line 2792 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Optional<CoordinatorsResult>>(),
		   ChangeQuorumCheckerActorState<ChangeQuorumCheckerActor>(tr, conn, newName)
	{
		fdb_probe_actor_enter("changeQuorumChecker", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("changeQuorumChecker");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("changeQuorumChecker", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ChangeQuorumCheckerActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ChangeQuorumCheckerActor, 1, std::vector<NetworkAddress> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ChangeQuorumCheckerActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 833 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Optional<CoordinatorsResult>> changeQuorumChecker( Transaction* const& tr, ClusterConnectionString* const& conn, std::string const& newName ) {
															#line 833 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Optional<CoordinatorsResult>>(new ChangeQuorumCheckerActor(tr, conn, newName));
															#line 2822 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 924 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 2827 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via changeQuorum()
															#line 925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class ChangeQuorumActor>
															#line 925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ChangeQuorumActorState {
															#line 2834 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ChangeQuorumActorState(Database const& cx,Reference<IQuorumChange> const& change) 
															#line 925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   change(change),
															#line 926 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx),
															#line 927 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   retries(0),
															#line 928 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   desiredCoordinators(),
															#line 929 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   notEnoughMachineResults(0)
															#line 2851 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("changeQuorum", reinterpret_cast<unsigned long>(this));

	}
	~ChangeQuorumActorState() 
	{
		fdb_probe_actor_destroy("changeQuorum", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 931 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 2866 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~ChangeQuorumActorState();
		static_cast<ChangeQuorumActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 933 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 934 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 935 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 936 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 937 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(coordinatorsKey);
															#line 937 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2907 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 1;
															#line 937 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumActor, 0, Optional<Value> >*>(static_cast<ChangeQuorumActor*>(this)));
															#line 2912 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1026 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("RetryQuorumChange").error(e).detail("Retries", retries);
															#line 1027 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_8 = tr.onError(e);
															#line 1027 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2938 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_8.isReady()) { if (__when_expr_8.isError()) return a_body1Catch1(__when_expr_8.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_8.get(), loopDepth); };
			static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 8;
															#line 1027 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_8.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumActor, 8, Void >*>(static_cast<ChangeQuorumActor*>(this)));
															#line 2943 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& currentKey,int loopDepth) 
	{
															#line 939 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!currentKey.present())
															#line 2958 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 940 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumActor*>(this)->SAV<CoordinatorsResult>::futures) { (void)(CoordinatorsResult::BAD_DATABASE_STATE); this->~ChangeQuorumActorState(); static_cast<ChangeQuorumActor*>(this)->destroy(); return 0; }
															#line 2962 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumActor*>(this)->SAV< CoordinatorsResult >::value()) CoordinatorsResult(CoordinatorsResult::BAD_DATABASE_STATE);
			this->~ChangeQuorumActorState();
			static_cast<ChangeQuorumActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 942 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		old = ClusterConnectionString(currentKey.get().toString());
															#line 943 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (cx->getConnectionRecord() && old.clusterKeyName().toString() != cx->getConnectionRecord()->getConnectionString().clusterKeyName())
															#line 2972 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 945 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumActor*>(this)->SAV<CoordinatorsResult>::futures) { (void)(CoordinatorsResult::BAD_DATABASE_STATE); this->~ChangeQuorumActorState(); static_cast<ChangeQuorumActor*>(this)->destroy(); return 0; }
															#line 2976 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumActor*>(this)->SAV< CoordinatorsResult >::value()) CoordinatorsResult(CoordinatorsResult::BAD_DATABASE_STATE);
			this->~ChangeQuorumActorState();
			static_cast<ChangeQuorumActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 947 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<std::vector<NetworkAddress>> __when_expr_1 = old.tryResolveHostnames();
															#line 947 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2986 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 2;
															#line 947 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumActor, 1, std::vector<NetworkAddress> >*>(static_cast<ChangeQuorumActor*>(this)));
															#line 2991 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && currentKey,int loopDepth) 
	{
															#line 939 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!currentKey.present())
															#line 3000 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 940 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumActor*>(this)->SAV<CoordinatorsResult>::futures) { (void)(CoordinatorsResult::BAD_DATABASE_STATE); this->~ChangeQuorumActorState(); static_cast<ChangeQuorumActor*>(this)->destroy(); return 0; }
															#line 3004 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumActor*>(this)->SAV< CoordinatorsResult >::value()) CoordinatorsResult(CoordinatorsResult::BAD_DATABASE_STATE);
			this->~ChangeQuorumActorState();
			static_cast<ChangeQuorumActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 942 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		old = ClusterConnectionString(currentKey.get().toString());
															#line 943 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (cx->getConnectionRecord() && old.clusterKeyName().toString() != cx->getConnectionRecord()->getConnectionString().clusterKeyName())
															#line 3014 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 945 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumActor*>(this)->SAV<CoordinatorsResult>::futures) { (void)(CoordinatorsResult::BAD_DATABASE_STATE); this->~ChangeQuorumActorState(); static_cast<ChangeQuorumActor*>(this)->destroy(); return 0; }
															#line 3018 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumActor*>(this)->SAV< CoordinatorsResult >::value()) CoordinatorsResult(CoordinatorsResult::BAD_DATABASE_STATE);
			this->~ChangeQuorumActorState();
			static_cast<ChangeQuorumActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 947 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<std::vector<NetworkAddress>> __when_expr_1 = old.tryResolveHostnames();
															#line 947 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3028 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 2;
															#line 947 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumActor, 1, std::vector<NetworkAddress> >*>(static_cast<ChangeQuorumActor*>(this)));
															#line 3033 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& currentKey,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(currentKey, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && currentKey,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(currentKey), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state > 0) static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeQuorumActor*>(this)->ActorCallback< ChangeQuorumActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ChangeQuorumActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 948 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		result = CoordinatorsResult::SUCCESS;
															#line 949 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!desiredCoordinators.size())
															#line 3107 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 950 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<std::vector<NetworkAddress>> __when_expr_2 = change->getDesiredCoordinators( &tr, oldCoordinators, Reference<ClusterConnectionMemoryRecord>(new ClusterConnectionMemoryRecord(old)), result);
															#line 950 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3113 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
			static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 3;
															#line 950 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumActor, 2, std::vector<NetworkAddress> >*>(static_cast<ChangeQuorumActor*>(this)));
															#line 3118 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont6(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(std::vector<NetworkAddress> const& __oldCoordinators,int loopDepth) 
	{
															#line 947 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		oldCoordinators = __oldCoordinators;
															#line 3132 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(std::vector<NetworkAddress> && __oldCoordinators,int loopDepth) 
	{
		oldCoordinators = std::move(__oldCoordinators);
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state > 0) static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeQuorumActor*>(this)->ActorCallback< ChangeQuorumActor, 1, std::vector<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 1, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> const& value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 1, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ChangeQuorumActor, 1, std::vector<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont6(int loopDepth) 
	{
															#line 958 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (result == CoordinatorsResult::NOT_ENOUGH_MACHINES && notEnoughMachineResults < 1)
															#line 3199 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 961 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			notEnoughMachineResults++;
															#line 962 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_3 = delay(1.0);
															#line 962 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3207 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont6when1(__when_expr_3.get(), loopDepth); };
			static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 4;
															#line 962 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumActor, 3, Void >*>(static_cast<ChangeQuorumActor*>(this)));
															#line 3212 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont9(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont7(std::vector<NetworkAddress> const& _desiredCoordinators,int loopDepth) 
	{
															#line 955 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		desiredCoordinators = _desiredCoordinators;
															#line 3226 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7(std::vector<NetworkAddress> && _desiredCoordinators,int loopDepth) 
	{
															#line 955 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		desiredCoordinators = _desiredCoordinators;
															#line 3235 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(std::vector<NetworkAddress> const& _desiredCoordinators,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(_desiredCoordinators, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(std::vector<NetworkAddress> && _desiredCoordinators,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(std::move(_desiredCoordinators), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state > 0) static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeQuorumActor*>(this)->ActorCallback< ChangeQuorumActor, 2, std::vector<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 2, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> const& value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 2, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ChangeQuorumActor, 2, std::vector<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont9(int loopDepth) 
	{
															#line 966 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (result != CoordinatorsResult::SUCCESS)
															#line 3307 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 967 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumActor*>(this)->SAV<CoordinatorsResult>::futures) { (void)(result); this->~ChangeQuorumActorState(); static_cast<ChangeQuorumActor*>(this)->destroy(); return 0; }
															#line 3311 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumActor*>(this)->SAV< CoordinatorsResult >::value()) CoordinatorsResult(std::move(result)); // state_var_RVO
			this->~ChangeQuorumActorState();
			static_cast<ChangeQuorumActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 968 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!desiredCoordinators.size())
															#line 3319 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 969 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumActor*>(this)->SAV<CoordinatorsResult>::futures) { (void)(CoordinatorsResult::INVALID_NETWORK_ADDRESSES); this->~ChangeQuorumActorState(); static_cast<ChangeQuorumActor*>(this)->destroy(); return 0; }
															#line 3323 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumActor*>(this)->SAV< CoordinatorsResult >::value()) CoordinatorsResult(CoordinatorsResult::INVALID_NETWORK_ADDRESSES);
			this->~ChangeQuorumActorState();
			static_cast<ChangeQuorumActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 970 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::sort(desiredCoordinators.begin(), desiredCoordinators.end());
															#line 972 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::string newName = change->getDesiredClusterKeyName();
															#line 973 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (newName.empty())
															#line 3335 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 974 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			newName = old.clusterKeyName().toString();
															#line 3339 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 976 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (oldCoordinators == desiredCoordinators && old.clusterKeyName() == newName)
															#line 3343 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 977 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumActor*>(this)->SAV<CoordinatorsResult>::futures) { (void)(retries ? CoordinatorsResult::SUCCESS : CoordinatorsResult::SAME_NETWORK_ADDRESSES); this->~ChangeQuorumActorState(); static_cast<ChangeQuorumActor*>(this)->destroy(); return 0; }
															#line 3347 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumActor*>(this)->SAV< CoordinatorsResult >::value()) CoordinatorsResult(retries ? CoordinatorsResult::SUCCESS : CoordinatorsResult::SAME_NETWORK_ADDRESSES);
			this->~ChangeQuorumActorState();
			static_cast<ChangeQuorumActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 979 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		conn = ClusterConnectionString(desiredCoordinators, StringRef(newName + ':' + deterministicRandom()->randomAlphaNumeric(32)));
															#line 982 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (g_network->isSimulated())
															#line 3357 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 983 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			for(int i = 0;i < (desiredCoordinators.size() / 2) + 1;i++) {
															#line 984 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				auto process = g_simulator.getProcessByAddress(desiredCoordinators[i]);
															#line 985 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				ASSERT(process->isReliable() || process->rebooting);
															#line 987 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				g_simulator.protectedAddresses.insert(process->addresses.address);
															#line 988 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (process->addresses.secondaryAddress.present())
															#line 3369 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 989 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					g_simulator.protectedAddresses.insert(process->addresses.secondaryAddress.get());
															#line 3373 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
															#line 991 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				TraceEvent("ProtectCoordinator").detail("Address", desiredCoordinators[i]).backtrace();
															#line 3377 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 995 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		TraceEvent("AttemptingQuorumChange").detail("FromCS", old.toString()).detail("ToCS", conn.toString());
															#line 996 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		TEST(old.clusterKeyName() != conn.clusterKeyName());
															#line 997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		TEST(old.clusterKeyName() == conn.clusterKeyName());
															#line 999 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		leaderServers = std::vector<Future<Optional<LeaderInfo>>>();
															#line 1000 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		coord = ClientCoordinators(Reference<ClusterConnectionMemoryRecord>(new ClusterConnectionMemoryRecord(conn)));
															#line 1003 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!change->getDesiredClusterKeyName().empty())
															#line 3392 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1004 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<CheckDescriptorMutableReply> __when_expr_4 = coord.clientLeaderServers[0].checkDescriptorMutable.getReply(CheckDescriptorMutableRequest());
															#line 1004 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3398 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1loopBody1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1loopBody1cont9when1(__when_expr_4.get(), loopDepth); };
			static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 5;
															#line 1004 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumActor, 4, CheckDescriptorMutableReply >*>(static_cast<ChangeQuorumActor*>(this)));
															#line 3403 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont11(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont10(Void const& _,int loopDepth) 
	{
															#line 963 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.reset();
															#line 3417 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		return a_body1loopHead1(loopDepth); // continue

		return loopDepth;
	}
	int a_body1loopBody1cont10(Void && _,int loopDepth) 
	{
															#line 963 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.reset();
															#line 3426 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		return a_body1loopHead1(loopDepth); // continue

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state > 0) static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeQuorumActor*>(this)->ActorCallback< ChangeQuorumActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ChangeQuorumActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1cont11(int loopDepth) 
	{
															#line 1009 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		leaderServers.reserve(coord.clientLeaderServers.size());
															#line 1010 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(int i = 0;i < coord.clientLeaderServers.size();i++) {
															#line 1011 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			leaderServers.push_back(retryBrokenPromise(coord.clientLeaderServers[i].getLeader, GetLeaderRequest(coord.clusterKey, UID()), TaskPriority::CoordinationReply));
															#line 3502 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1015 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_5 = waitForAll(leaderServers);
															#line 1014 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3508 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1loopBody1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1loopBody1cont11when1(__when_expr_5.get(), loopDepth); };
															#line 1016 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_6 = delay(5.0);
															#line 3512 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1loopBody1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1loopBody1cont11when2(__when_expr_6.get(), loopDepth); };
		static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 6;
															#line 1015 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumActor, 5, Void >*>(static_cast<ChangeQuorumActor*>(this)));
															#line 1016 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumActor, 6, Void >*>(static_cast<ChangeQuorumActor*>(this)));
															#line 3519 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont19(CheckDescriptorMutableReply const& mutabilityReply,int loopDepth) 
	{
															#line 1006 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!mutabilityReply.isMutable)
															#line 3528 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1007 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumActor*>(this)->SAV<CoordinatorsResult>::futures) { (void)(CoordinatorsResult::BAD_DATABASE_STATE); this->~ChangeQuorumActorState(); static_cast<ChangeQuorumActor*>(this)->destroy(); return 0; }
															#line 3532 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumActor*>(this)->SAV< CoordinatorsResult >::value()) CoordinatorsResult(CoordinatorsResult::BAD_DATABASE_STATE);
			this->~ChangeQuorumActorState();
			static_cast<ChangeQuorumActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont19(CheckDescriptorMutableReply && mutabilityReply,int loopDepth) 
	{
															#line 1006 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!mutabilityReply.isMutable)
															#line 3546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1007 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ChangeQuorumActor*>(this)->SAV<CoordinatorsResult>::futures) { (void)(CoordinatorsResult::BAD_DATABASE_STATE); this->~ChangeQuorumActorState(); static_cast<ChangeQuorumActor*>(this)->destroy(); return 0; }
															#line 3550 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ChangeQuorumActor*>(this)->SAV< CoordinatorsResult >::value()) CoordinatorsResult(CoordinatorsResult::BAD_DATABASE_STATE);
			this->~ChangeQuorumActorState();
			static_cast<ChangeQuorumActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont9when1(CheckDescriptorMutableReply const& mutabilityReply,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont19(mutabilityReply, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont9when1(CheckDescriptorMutableReply && mutabilityReply,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont19(std::move(mutabilityReply), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state > 0) static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeQuorumActor*>(this)->ActorCallback< ChangeQuorumActor, 4, CheckDescriptorMutableReply >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 4, CheckDescriptorMutableReply >*,CheckDescriptorMutableReply const& value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont9when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 4, CheckDescriptorMutableReply >*,CheckDescriptorMutableReply && value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont9when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< ChangeQuorumActor, 4, CheckDescriptorMutableReply >*,Error err) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1loopBody1cont11cont1(int loopDepth) 
	{
															#line 1021 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.set(coordinatorsKey, conn.toString());
															#line 1023 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_7 = tr.commit();
															#line 1023 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3631 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1loopBody1Catch1(__when_expr_7.getError(), loopDepth); else return a_body1loopBody1cont11cont1when1(__when_expr_7.get(), loopDepth); };
		static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 7;
															#line 1023 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< ChangeQuorumActor, 7, Void >*>(static_cast<ChangeQuorumActor*>(this)));
															#line 3636 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont11when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11when2(Void const& _,int loopDepth) 
	{
															#line 1017 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ChangeQuorumActor*>(this)->SAV<CoordinatorsResult>::futures) { (void)(CoordinatorsResult::COORDINATOR_UNREACHABLE); this->~ChangeQuorumActorState(); static_cast<ChangeQuorumActor*>(this)->destroy(); return 0; }
															#line 3657 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ChangeQuorumActor*>(this)->SAV< CoordinatorsResult >::value()) CoordinatorsResult(CoordinatorsResult::COORDINATOR_UNREACHABLE);
		this->~ChangeQuorumActorState();
		static_cast<ChangeQuorumActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont11when2(Void && _,int loopDepth) 
	{
															#line 1017 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ChangeQuorumActor*>(this)->SAV<CoordinatorsResult>::futures) { (void)(CoordinatorsResult::COORDINATOR_UNREACHABLE); this->~ChangeQuorumActorState(); static_cast<ChangeQuorumActor*>(this)->destroy(); return 0; }
															#line 3669 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ChangeQuorumActor*>(this)->SAV< CoordinatorsResult >::value()) CoordinatorsResult(CoordinatorsResult::COORDINATOR_UNREACHABLE);
		this->~ChangeQuorumActorState();
		static_cast<ChangeQuorumActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state > 0) static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeQuorumActor*>(this)->ActorCallback< ChangeQuorumActor, 5, Void >::remove();
		static_cast<ChangeQuorumActor*>(this)->ActorCallback< ChangeQuorumActor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1cont11when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1cont11when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< ChangeQuorumActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose6();
		try {
			a_body1loopBody1cont11when2(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose6();
		try {
			a_body1loopBody1cont11when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< ChangeQuorumActor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose6();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 6);

	}
	int a_body1loopBody1cont11cont3(Void const& _,int loopDepth) 
	{
															#line 1024 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(false);
															#line 3778 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1loopBody1cont11cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11cont3(Void && _,int loopDepth) 
	{
															#line 1024 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(false);
															#line 3787 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1loopBody1cont11cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state > 0) static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeQuorumActor*>(this)->ActorCallback< ChangeQuorumActor, 7, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 7, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose7();
		try {
			a_body1loopBody1cont11cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 7, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose7();
		try {
			a_body1loopBody1cont11cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_error(ActorCallback< ChangeQuorumActor, 7, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose7();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 7);

	}
	int a_body1loopBody1cont11cont5(int loopDepth) 
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
															#line 1028 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		++retries;
															#line 3872 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 1028 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		++retries;
															#line 3881 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
	void a_exitChoose8() 
	{
		if (static_cast<ChangeQuorumActor*>(this)->actor_wait_state > 0) static_cast<ChangeQuorumActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeQuorumActor*>(this)->ActorCallback< ChangeQuorumActor, 8, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 8, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose8();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_fire(ActorCallback< ChangeQuorumActor, 8, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose8();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_error(ActorCallback< ChangeQuorumActor, 8, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose8();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), 8);

	}
															#line 925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Reference<IQuorumChange> change;
															#line 926 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 927 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	int retries;
															#line 928 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::vector<NetworkAddress> desiredCoordinators;
															#line 929 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	int notEnoughMachineResults;
															#line 942 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ClusterConnectionString old;
															#line 947 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::vector<NetworkAddress> oldCoordinators;
															#line 948 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	CoordinatorsResult result;
															#line 979 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ClusterConnectionString conn;
															#line 999 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::vector<Future<Optional<LeaderInfo>>> leaderServers;
															#line 1000 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ClientCoordinators coord;
															#line 3973 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via changeQuorum()
															#line 925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ChangeQuorumActor final : public Actor<CoordinatorsResult>, public ActorCallback< ChangeQuorumActor, 0, Optional<Value> >, public ActorCallback< ChangeQuorumActor, 1, std::vector<NetworkAddress> >, public ActorCallback< ChangeQuorumActor, 2, std::vector<NetworkAddress> >, public ActorCallback< ChangeQuorumActor, 3, Void >, public ActorCallback< ChangeQuorumActor, 4, CheckDescriptorMutableReply >, public ActorCallback< ChangeQuorumActor, 5, Void >, public ActorCallback< ChangeQuorumActor, 6, Void >, public ActorCallback< ChangeQuorumActor, 7, Void >, public ActorCallback< ChangeQuorumActor, 8, Void >, public FastAllocated<ChangeQuorumActor>, public ChangeQuorumActorState<ChangeQuorumActor> {
															#line 3978 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<ChangeQuorumActor>::operator new;
	using FastAllocated<ChangeQuorumActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<CoordinatorsResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ChangeQuorumActor, 0, Optional<Value> >;
friend struct ActorCallback< ChangeQuorumActor, 1, std::vector<NetworkAddress> >;
friend struct ActorCallback< ChangeQuorumActor, 2, std::vector<NetworkAddress> >;
friend struct ActorCallback< ChangeQuorumActor, 3, Void >;
friend struct ActorCallback< ChangeQuorumActor, 4, CheckDescriptorMutableReply >;
friend struct ActorCallback< ChangeQuorumActor, 5, Void >;
friend struct ActorCallback< ChangeQuorumActor, 6, Void >;
friend struct ActorCallback< ChangeQuorumActor, 7, Void >;
friend struct ActorCallback< ChangeQuorumActor, 8, Void >;
															#line 925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ChangeQuorumActor(Database const& cx,Reference<IQuorumChange> const& change) 
															#line 3997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<CoordinatorsResult>(),
		   ChangeQuorumActorState<ChangeQuorumActor>(cx, change)
	{
		fdb_probe_actor_enter("changeQuorum", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("changeQuorum");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("changeQuorum", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ChangeQuorumActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ChangeQuorumActor, 1, std::vector<NetworkAddress> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ChangeQuorumActor, 2, std::vector<NetworkAddress> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ChangeQuorumActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< ChangeQuorumActor, 4, CheckDescriptorMutableReply >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< ChangeQuorumActor, 5, Void >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< ChangeQuorumActor, 7, Void >*)0, actor_cancelled()); break;
		case 8: this->a_callback_error((ActorCallback< ChangeQuorumActor, 8, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<CoordinatorsResult> changeQuorum( Database const& cx, Reference<IQuorumChange> const& change ) {
															#line 925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<CoordinatorsResult>(new ChangeQuorumActor(cx, change));
															#line 4032 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1032 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

struct NameQuorumChange final : IQuorumChange {
	std::string newName;
	Reference<IQuorumChange> otherChange;
	explicit NameQuorumChange(std::string const& newName, Reference<IQuorumChange> const& otherChange)
	  : newName(newName), otherChange(otherChange) {}
	Future<std::vector<NetworkAddress>> getDesiredCoordinators(Transaction* tr,
	                                                           std::vector<NetworkAddress> oldCoordinators,
	                                                           Reference<IClusterConnectionRecord> ccr,
	                                                           CoordinatorsResult& t) override {
		return otherChange->getDesiredCoordinators(tr, oldCoordinators, ccr, t);
	}
	std::string getDesiredClusterKeyName() const override { return newName; }
};
Reference<IQuorumChange> nameQuorumChange(std::string const& name, Reference<IQuorumChange> const& other) {
	return Reference<IQuorumChange>(new NameQuorumChange(name, other));
}

struct AutoQuorumChange final : IQuorumChange {
	int desired;
	explicit AutoQuorumChange(int desired) : desired(desired) {}

	Future<std::vector<NetworkAddress>> getDesiredCoordinators(Transaction* tr,
	                                                           std::vector<NetworkAddress> oldCoordinators,
	                                                           Reference<IClusterConnectionRecord> ccr,
	                                                           CoordinatorsResult& err) override {
		return getDesired(Reference<AutoQuorumChange>::addRef(this), tr, oldCoordinators, ccr, &err);
	}

																#line 4065 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
// This generated class is to be used only via getRedundancy()
															#line 1061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class GetRedundancyActor>
															#line 1061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetRedundancyActorState {
															#line 4071 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetRedundancyActorState(AutoQuorumChange* const& self,Transaction* const& tr) 
															#line 1061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : self(self),
															#line 1061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(tr),
															#line 1062 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   fStorageReplicas(tr->get(LiteralStringRef("storage_replicas").withPrefix(configKeysPrefix))),
															#line 1064 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   fLogReplicas(tr->get(LiteralStringRef("log_replicas").withPrefix(configKeysPrefix)))
															#line 4084 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("getRedundancy", reinterpret_cast<unsigned long>(this));

	}
	~GetRedundancyActorState() 
	{
		fdb_probe_actor_destroy("getRedundancy", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1066 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_0 = success(fStorageReplicas) && success(fLogReplicas);
															#line 1066 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetRedundancyActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4101 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetRedundancyActor*>(this)->actor_wait_state = 1;
															#line 1066 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetRedundancyActor, 0, Void >*>(static_cast<GetRedundancyActor*>(this)));
															#line 4106 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~GetRedundancyActorState();
		static_cast<GetRedundancyActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1067 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		int redundancy = std::min(atoi(fStorageReplicas.get().get().toString().c_str()), atoi(fLogReplicas.get().get().toString().c_str()));
															#line 1070 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetRedundancyActor*>(this)->SAV<int>::futures) { (void)(redundancy); this->~GetRedundancyActorState(); static_cast<GetRedundancyActor*>(this)->destroy(); return 0; }
															#line 4131 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetRedundancyActor*>(this)->SAV< int >::value()) int(redundancy);
		this->~GetRedundancyActorState();
		static_cast<GetRedundancyActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1067 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		int redundancy = std::min(atoi(fStorageReplicas.get().get().toString().c_str()), atoi(fLogReplicas.get().get().toString().c_str()));
															#line 1070 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetRedundancyActor*>(this)->SAV<int>::futures) { (void)(redundancy); this->~GetRedundancyActorState(); static_cast<GetRedundancyActor*>(this)->destroy(); return 0; }
															#line 4145 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetRedundancyActor*>(this)->SAV< int >::value()) int(redundancy);
		this->~GetRedundancyActorState();
		static_cast<GetRedundancyActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<GetRedundancyActor*>(this)->actor_wait_state > 0) static_cast<GetRedundancyActor*>(this)->actor_wait_state = 0;
		static_cast<GetRedundancyActor*>(this)->ActorCallback< GetRedundancyActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetRedundancyActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getRedundancy", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRedundancy", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetRedundancyActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getRedundancy", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRedundancy", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetRedundancyActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getRedundancy", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRedundancy", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	AutoQuorumChange* self;
															#line 1061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 1062 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Future<Optional<Value>> fStorageReplicas;
															#line 1064 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Future<Optional<Value>> fLogReplicas;
															#line 4224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via getRedundancy()
															#line 1061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetRedundancyActor final : public Actor<int>, public ActorCallback< GetRedundancyActor, 0, Void >, public FastAllocated<GetRedundancyActor>, public GetRedundancyActorState<GetRedundancyActor> {
															#line 4229 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<GetRedundancyActor>::operator new;
	using FastAllocated<GetRedundancyActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetRedundancyActor, 0, Void >;
															#line 1061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetRedundancyActor(AutoQuorumChange* const& self,Transaction* const& tr) 
															#line 4240 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<int>(),
		   GetRedundancyActorState<GetRedundancyActor>(self, tr)
	{
		fdb_probe_actor_enter("getRedundancy", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getRedundancy");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getRedundancy", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetRedundancyActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] static Future<int> getRedundancy( AutoQuorumChange* const& self, Transaction* const& tr ) {
															#line 1061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<int>(new GetRedundancyActor(self, tr));
															#line 4267 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1072 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

																#line 4272 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
// This generated class is to be used only via isAcceptable()
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class IsAcceptableActor>
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class IsAcceptableActorState {
															#line 4278 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	IsAcceptableActorState(AutoQuorumChange* const& self,Transaction* const& tr,std::vector<NetworkAddress> const& oldCoordinators,Reference<IClusterConnectionRecord> const& ccr,int const& desiredCount,std::set<AddressExclusion>* const& excluded) 
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : self(self),
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(tr),
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   oldCoordinators(oldCoordinators),
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   ccr(ccr),
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   desiredCount(desiredCount),
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   excluded(excluded)
															#line 4295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("isAcceptable", reinterpret_cast<unsigned long>(this));

	}
	~IsAcceptableActorState() 
	{
		fdb_probe_actor_destroy("isAcceptable", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1079 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ClusterConnectionString cs = ccr->getConnectionString();
															#line 1080 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (oldCoordinators.size() != cs.hostnames.size() + cs.coords.size())
															#line 4312 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1081 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (!static_cast<IsAcceptableActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsAcceptableActorState(); static_cast<IsAcceptableActor*>(this)->destroy(); return 0; }
															#line 4316 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				new (&static_cast<IsAcceptableActor*>(this)->SAV< bool >::value()) bool(false);
				this->~IsAcceptableActorState();
				static_cast<IsAcceptableActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1085 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (oldCoordinators.size() < desiredCount)
															#line 4324 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1086 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (!static_cast<IsAcceptableActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsAcceptableActorState(); static_cast<IsAcceptableActor*>(this)->destroy(); return 0; }
															#line 4328 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				new (&static_cast<IsAcceptableActor*>(this)->SAV< bool >::value()) bool(false);
				this->~IsAcceptableActorState();
				static_cast<IsAcceptableActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1087 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (oldCoordinators.size() % 2 != 1)
															#line 4336 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1088 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (!static_cast<IsAcceptableActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsAcceptableActorState(); static_cast<IsAcceptableActor*>(this)->destroy(); return 0; }
															#line 4340 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				new (&static_cast<IsAcceptableActor*>(this)->SAV< bool >::value()) bool(false);
				this->~IsAcceptableActorState();
				static_cast<IsAcceptableActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1091 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			for( auto& c : oldCoordinators ) {
															#line 1092 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (addressExcluded(*excluded, c))
															#line 4350 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 1093 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					if (!static_cast<IsAcceptableActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsAcceptableActorState(); static_cast<IsAcceptableActor*>(this)->destroy(); return 0; }
															#line 4354 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					new (&static_cast<IsAcceptableActor*>(this)->SAV< bool >::value()) bool(false);
					this->~IsAcceptableActorState();
					static_cast<IsAcceptableActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
															#line 1098 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			std::sort(oldCoordinators.begin(), oldCoordinators.end());
															#line 1099 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			for(int i = 1;i < oldCoordinators.size();i++) {
															#line 1100 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (oldCoordinators[i - 1].ip == oldCoordinators[i].ip)
															#line 4367 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 1101 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					if (!static_cast<IsAcceptableActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsAcceptableActorState(); static_cast<IsAcceptableActor*>(this)->destroy(); return 0; }
															#line 4371 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					new (&static_cast<IsAcceptableActor*>(this)->SAV< bool >::value()) bool(false);
					this->~IsAcceptableActorState();
					static_cast<IsAcceptableActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
															#line 1104 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ClientCoordinators coord(ccr);
															#line 1105 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			std::vector<Future<Optional<LeaderInfo>>> leaderServers;
															#line 1106 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			leaderServers.reserve(coord.clientLeaderServers.size());
															#line 1107 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			for(int i = 0;i < coord.clientLeaderServers.size();i++) {
															#line 1108 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (coord.clientLeaderServers[i].hostname.present())
															#line 4388 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 1109 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					leaderServers.push_back(retryGetReplyFromHostname(GetLeaderRequest(coord.clusterKey, UID()), coord.clientLeaderServers[i].hostname.get(), WLTOKEN_CLIENTLEADERREG_GETLEADER, TaskPriority::CoordinationReply));
															#line 4392 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
				else
				{
															#line 1114 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					leaderServers.push_back(retryBrokenPromise(coord.clientLeaderServers[i].getLeader, GetLeaderRequest(coord.clusterKey, UID()), TaskPriority::CoordinationReply));
															#line 4398 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
			}
															#line 1119 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<std::vector<Optional<LeaderInfo>>>> __when_expr_0 = timeout(getAll(leaderServers), CLIENT_KNOBS->IS_ACCEPTABLE_DELAY);
															#line 1119 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<IsAcceptableActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4405 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<IsAcceptableActor*>(this)->actor_wait_state = 1;
															#line 1119 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IsAcceptableActor, 0, Optional<std::vector<Optional<LeaderInfo>>> >*>(static_cast<IsAcceptableActor*>(this)));
															#line 4410 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~IsAcceptableActorState();
		static_cast<IsAcceptableActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<std::vector<Optional<LeaderInfo>>> const& results,int loopDepth) 
	{
															#line 1121 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!results.present())
															#line 4433 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1122 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<IsAcceptableActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsAcceptableActorState(); static_cast<IsAcceptableActor*>(this)->destroy(); return 0; }
															#line 4437 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<IsAcceptableActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsAcceptableActorState();
			static_cast<IsAcceptableActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1124 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto& r : results.get() ) {
															#line 1125 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!r.present())
															#line 4447 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1126 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (!static_cast<IsAcceptableActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsAcceptableActorState(); static_cast<IsAcceptableActor*>(this)->destroy(); return 0; }
															#line 4451 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				new (&static_cast<IsAcceptableActor*>(this)->SAV< bool >::value()) bool(false);
				this->~IsAcceptableActorState();
				static_cast<IsAcceptableActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 1130 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<IsAcceptableActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsAcceptableActorState(); static_cast<IsAcceptableActor*>(this)->destroy(); return 0; }
															#line 4460 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<IsAcceptableActor*>(this)->SAV< bool >::value()) bool(true);
		this->~IsAcceptableActorState();
		static_cast<IsAcceptableActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<std::vector<Optional<LeaderInfo>>> && results,int loopDepth) 
	{
															#line 1121 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!results.present())
															#line 4472 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1122 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<IsAcceptableActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsAcceptableActorState(); static_cast<IsAcceptableActor*>(this)->destroy(); return 0; }
															#line 4476 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<IsAcceptableActor*>(this)->SAV< bool >::value()) bool(false);
			this->~IsAcceptableActorState();
			static_cast<IsAcceptableActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1124 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto& r : results.get() ) {
															#line 1125 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!r.present())
															#line 4486 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1126 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (!static_cast<IsAcceptableActor*>(this)->SAV<bool>::futures) { (void)(false); this->~IsAcceptableActorState(); static_cast<IsAcceptableActor*>(this)->destroy(); return 0; }
															#line 4490 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				new (&static_cast<IsAcceptableActor*>(this)->SAV< bool >::value()) bool(false);
				this->~IsAcceptableActorState();
				static_cast<IsAcceptableActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 1130 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<IsAcceptableActor*>(this)->SAV<bool>::futures) { (void)(true); this->~IsAcceptableActorState(); static_cast<IsAcceptableActor*>(this)->destroy(); return 0; }
															#line 4499 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<IsAcceptableActor*>(this)->SAV< bool >::value()) bool(true);
		this->~IsAcceptableActorState();
		static_cast<IsAcceptableActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<std::vector<Optional<LeaderInfo>>> const& results,int loopDepth) 
	{
		loopDepth = a_body1cont1(results, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<std::vector<Optional<LeaderInfo>>> && results,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(results), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<IsAcceptableActor*>(this)->actor_wait_state > 0) static_cast<IsAcceptableActor*>(this)->actor_wait_state = 0;
		static_cast<IsAcceptableActor*>(this)->ActorCallback< IsAcceptableActor, 0, Optional<std::vector<Optional<LeaderInfo>>> >::remove();

	}
	void a_callback_fire(ActorCallback< IsAcceptableActor, 0, Optional<std::vector<Optional<LeaderInfo>>> >*,Optional<std::vector<Optional<LeaderInfo>>> const& value) 
	{
		fdb_probe_actor_enter("isAcceptable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isAcceptable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IsAcceptableActor, 0, Optional<std::vector<Optional<LeaderInfo>>> >*,Optional<std::vector<Optional<LeaderInfo>>> && value) 
	{
		fdb_probe_actor_enter("isAcceptable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isAcceptable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IsAcceptableActor, 0, Optional<std::vector<Optional<LeaderInfo>>> >*,Error err) 
	{
		fdb_probe_actor_enter("isAcceptable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("isAcceptable", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	AutoQuorumChange* self;
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::vector<NetworkAddress> oldCoordinators;
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Reference<IClusterConnectionRecord> ccr;
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	int desiredCount;
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::set<AddressExclusion>* excluded;
															#line 4582 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via isAcceptable()
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class IsAcceptableActor final : public Actor<bool>, public ActorCallback< IsAcceptableActor, 0, Optional<std::vector<Optional<LeaderInfo>>> >, public FastAllocated<IsAcceptableActor>, public IsAcceptableActorState<IsAcceptableActor> {
															#line 4587 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<IsAcceptableActor>::operator new;
	using FastAllocated<IsAcceptableActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IsAcceptableActor, 0, Optional<std::vector<Optional<LeaderInfo>>> >;
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	IsAcceptableActor(AutoQuorumChange* const& self,Transaction* const& tr,std::vector<NetworkAddress> const& oldCoordinators,Reference<IClusterConnectionRecord> const& ccr,int const& desiredCount,std::set<AddressExclusion>* const& excluded) 
															#line 4598 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<bool>(),
		   IsAcceptableActorState<IsAcceptableActor>(self, tr, oldCoordinators, ccr, desiredCount, excluded)
	{
		fdb_probe_actor_enter("isAcceptable", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("isAcceptable");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("isAcceptable", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< IsAcceptableActor, 0, Optional<std::vector<Optional<LeaderInfo>>> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] static Future<bool> isAcceptable( AutoQuorumChange* const& self, Transaction* const& tr, std::vector<NetworkAddress> const& oldCoordinators, Reference<IClusterConnectionRecord> const& ccr, int const& desiredCount, std::set<AddressExclusion>* const& excluded ) {
															#line 1073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<bool>(new IsAcceptableActor(self, tr, oldCoordinators, ccr, desiredCount, excluded));
															#line 4625 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1132 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

																#line 4630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
// This generated class is to be used only via getDesired()
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class GetDesiredActor>
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetDesiredActorState {
															#line 4636 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetDesiredActorState(Reference<AutoQuorumChange> const& self,Transaction* const& tr,std::vector<NetworkAddress> const& oldCoordinators,Reference<IClusterConnectionRecord> const& ccr,CoordinatorsResult* const& err) 
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : self(self),
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(tr),
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   oldCoordinators(oldCoordinators),
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   ccr(ccr),
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   err(err),
															#line 1138 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   desiredCount(self->desired)
															#line 4653 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("getDesired", reinterpret_cast<unsigned long>(this));

	}
	~GetDesiredActorState() 
	{
		fdb_probe_actor_destroy("getDesired", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1140 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (desiredCount == -1)
															#line 4668 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1141 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				StrictFuture<int> __when_expr_0 = getRedundancy(self.getPtr(), tr);
															#line 1141 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (static_cast<GetDesiredActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4674 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetDesiredActor*>(this)->actor_wait_state = 1;
															#line 1141 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetDesiredActor, 0, int >*>(static_cast<GetDesiredActor*>(this)));
															#line 4679 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~GetDesiredActorState();
		static_cast<GetDesiredActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1145 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<std::vector<AddressExclusion>> __when_expr_1 = getAllExcludedServers(tr);
															#line 1145 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<GetDesiredActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4709 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetDesiredActor*>(this)->actor_wait_state = 2;
															#line 1145 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetDesiredActor, 1, std::vector<AddressExclusion> >*>(static_cast<GetDesiredActor*>(this)));
															#line 4714 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(int const& redundancy,int loopDepth) 
	{
															#line 1142 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		desiredCount = redundancy * 2 - 1;
															#line 4723 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(int && redundancy,int loopDepth) 
	{
															#line 1142 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		desiredCount = redundancy * 2 - 1;
															#line 4732 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(int const& redundancy,int loopDepth) 
	{
		loopDepth = a_body1cont2(redundancy, loopDepth);

		return loopDepth;
	}
	int a_body1when1(int && redundancy,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(redundancy), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetDesiredActor*>(this)->actor_wait_state > 0) static_cast<GetDesiredActor*>(this)->actor_wait_state = 0;
		static_cast<GetDesiredActor*>(this)->ActorCallback< GetDesiredActor, 0, int >::remove();

	}
	void a_callback_fire(ActorCallback< GetDesiredActor, 0, int >*,int const& value) 
	{
		fdb_probe_actor_enter("getDesired", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDesired", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetDesiredActor, 0, int >*,int && value) 
	{
		fdb_probe_actor_enter("getDesired", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDesired", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetDesiredActor, 0, int >*,Error err) 
	{
		fdb_probe_actor_enter("getDesired", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDesired", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(std::vector<AddressExclusion> const& excl,int loopDepth) 
	{
															#line 1146 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		excluded = std::set<AddressExclusion>(excl.begin(), excl.end());
															#line 1148 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<std::vector<ProcessData>> __when_expr_2 = getWorkers(tr);
															#line 1148 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<GetDesiredActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4808 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont4when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetDesiredActor*>(this)->actor_wait_state = 3;
															#line 1148 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetDesiredActor, 2, std::vector<ProcessData> >*>(static_cast<GetDesiredActor*>(this)));
															#line 4813 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(std::vector<AddressExclusion> && excl,int loopDepth) 
	{
															#line 1146 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		excluded = std::set<AddressExclusion>(excl.begin(), excl.end());
															#line 1148 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<std::vector<ProcessData>> __when_expr_2 = getWorkers(tr);
															#line 1148 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<GetDesiredActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont4when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetDesiredActor*>(this)->actor_wait_state = 3;
															#line 1148 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetDesiredActor, 2, std::vector<ProcessData> >*>(static_cast<GetDesiredActor*>(this)));
															#line 4831 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(std::vector<AddressExclusion> const& excl,int loopDepth) 
	{
		loopDepth = a_body1cont4(excl, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(std::vector<AddressExclusion> && excl,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(excl), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetDesiredActor*>(this)->actor_wait_state > 0) static_cast<GetDesiredActor*>(this)->actor_wait_state = 0;
		static_cast<GetDesiredActor*>(this)->ActorCallback< GetDesiredActor, 1, std::vector<AddressExclusion> >::remove();

	}
	void a_callback_fire(ActorCallback< GetDesiredActor, 1, std::vector<AddressExclusion> >*,std::vector<AddressExclusion> const& value) 
	{
		fdb_probe_actor_enter("getDesired", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDesired", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetDesiredActor, 1, std::vector<AddressExclusion> >*,std::vector<AddressExclusion> && value) 
	{
		fdb_probe_actor_enter("getDesired", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDesired", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetDesiredActor, 1, std::vector<AddressExclusion> >*,Error err) 
	{
		fdb_probe_actor_enter("getDesired", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDesired", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont5(std::vector<ProcessData> const& _workers,int loopDepth) 
	{
															#line 1149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		workers = _workers;
															#line 1151 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::map<NetworkAddress, LocalityData> addr_locality;
															#line 1152 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto w : workers ) {
															#line 1153 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			addr_locality[w.address] = w.locality;
															#line 4909 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1158 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		bool checkAcceptable = true;
															#line 1159 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::set<Optional<Standalone<StringRef>>> checkDuplicates;
															#line 1160 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto addr : oldCoordinators ) {
															#line 1161 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto findResult = addr_locality.find(addr);
															#line 1162 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (findResult == addr_locality.end() || checkDuplicates.count(findResult->second.zoneId()))
															#line 4921 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1163 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				checkAcceptable = false;
															#line 4925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				break;
			}
															#line 1166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			checkDuplicates.insert(findResult->second.zoneId());
															#line 4930 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1169 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (checkAcceptable)
															#line 4934 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1170 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<bool> __when_expr_3 = isAcceptable(self.getPtr(), tr, oldCoordinators, ccr, desiredCount, &excluded);
															#line 1170 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetDesiredActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4940 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont5when1(__when_expr_3.get(), loopDepth); };
			static_cast<GetDesiredActor*>(this)->actor_wait_state = 4;
															#line 1170 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GetDesiredActor, 3, bool >*>(static_cast<GetDesiredActor*>(this)));
															#line 4945 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont6(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont5(std::vector<ProcessData> && _workers,int loopDepth) 
	{
															#line 1149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		workers = _workers;
															#line 1151 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::map<NetworkAddress, LocalityData> addr_locality;
															#line 1152 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto w : workers ) {
															#line 1153 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			addr_locality[w.address] = w.locality;
															#line 4965 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1158 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		bool checkAcceptable = true;
															#line 1159 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::set<Optional<Standalone<StringRef>>> checkDuplicates;
															#line 1160 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto addr : oldCoordinators ) {
															#line 1161 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto findResult = addr_locality.find(addr);
															#line 1162 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (findResult == addr_locality.end() || checkDuplicates.count(findResult->second.zoneId()))
															#line 4977 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1163 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				checkAcceptable = false;
															#line 4981 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				break;
			}
															#line 1166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			checkDuplicates.insert(findResult->second.zoneId());
															#line 4986 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1169 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (checkAcceptable)
															#line 4990 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1170 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<bool> __when_expr_3 = isAcceptable(self.getPtr(), tr, oldCoordinators, ccr, desiredCount, &excluded);
															#line 1170 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetDesiredActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4996 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont5when1(__when_expr_3.get(), loopDepth); };
			static_cast<GetDesiredActor*>(this)->actor_wait_state = 4;
															#line 1170 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GetDesiredActor, 3, bool >*>(static_cast<GetDesiredActor*>(this)));
															#line 5001 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont6(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont4when1(std::vector<ProcessData> const& _workers,int loopDepth) 
	{
		loopDepth = a_body1cont5(_workers, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(std::vector<ProcessData> && _workers,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_workers), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetDesiredActor*>(this)->actor_wait_state > 0) static_cast<GetDesiredActor*>(this)->actor_wait_state = 0;
		static_cast<GetDesiredActor*>(this)->ActorCallback< GetDesiredActor, 2, std::vector<ProcessData> >::remove();

	}
	void a_callback_fire(ActorCallback< GetDesiredActor, 2, std::vector<ProcessData> >*,std::vector<ProcessData> const& value) 
	{
		fdb_probe_actor_enter("getDesired", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDesired", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetDesiredActor, 2, std::vector<ProcessData> >*,std::vector<ProcessData> && value) 
	{
		fdb_probe_actor_enter("getDesired", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDesired", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetDesiredActor, 2, std::vector<ProcessData> >*,Error err) 
	{
		fdb_probe_actor_enter("getDesired", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDesired", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont6(int loopDepth) 
	{
															#line 1177 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::vector<NetworkAddress> chosen;
															#line 1178 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		self->addDesiredWorkers(chosen, workers, desiredCount, excluded);
															#line 1180 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (chosen.size() < desiredCount)
															#line 5082 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1181 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (chosen.empty() || chosen.size() < oldCoordinators.size())
															#line 5086 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1182 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				TraceEvent("NotEnoughMachinesForCoordinators") .detail("EligibleWorkers", workers.size()) .detail("ChosenWorkers", chosen.size()) .detail("DesiredCoordinators", desiredCount) .detail("CurrentCoordinators", oldCoordinators.size());
															#line 1187 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				*err = CoordinatorsResult::NOT_ENOUGH_MACHINES;
															#line 1188 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (!static_cast<GetDesiredActor*>(this)->SAV<std::vector<NetworkAddress>>::futures) { (void)(std::vector<NetworkAddress>()); this->~GetDesiredActorState(); static_cast<GetDesiredActor*>(this)->destroy(); return 0; }
															#line 5094 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				new (&static_cast<GetDesiredActor*>(this)->SAV< std::vector<NetworkAddress> >::value()) std::vector<NetworkAddress>(std::vector<NetworkAddress>());
				this->~GetDesiredActorState();
				static_cast<GetDesiredActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1190 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			chosen.resize((chosen.size() - 1) | 1);
															#line 5102 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1193 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetDesiredActor*>(this)->SAV<std::vector<NetworkAddress>>::futures) { (void)(chosen); this->~GetDesiredActorState(); static_cast<GetDesiredActor*>(this)->destroy(); return 0; }
															#line 5106 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetDesiredActor*>(this)->SAV< std::vector<NetworkAddress> >::value()) std::vector<NetworkAddress>(chosen);
		this->~GetDesiredActorState();
		static_cast<GetDesiredActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10(bool const& ok,int loopDepth) 
	{
															#line 1171 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (ok)
															#line 5118 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1172 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			*err = CoordinatorsResult::SAME_NETWORK_ADDRESSES;
															#line 1173 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<GetDesiredActor*>(this)->SAV<std::vector<NetworkAddress>>::futures) { (void)(oldCoordinators); this->~GetDesiredActorState(); static_cast<GetDesiredActor*>(this)->destroy(); return 0; }
															#line 5124 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<GetDesiredActor*>(this)->SAV< std::vector<NetworkAddress> >::value()) std::vector<NetworkAddress>(std::move(oldCoordinators)); // state_var_RVO
			this->~GetDesiredActorState();
			static_cast<GetDesiredActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont10(bool && ok,int loopDepth) 
	{
															#line 1171 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (ok)
															#line 5138 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1172 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			*err = CoordinatorsResult::SAME_NETWORK_ADDRESSES;
															#line 1173 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<GetDesiredActor*>(this)->SAV<std::vector<NetworkAddress>>::futures) { (void)(oldCoordinators); this->~GetDesiredActorState(); static_cast<GetDesiredActor*>(this)->destroy(); return 0; }
															#line 5144 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<GetDesiredActor*>(this)->SAV< std::vector<NetworkAddress> >::value()) std::vector<NetworkAddress>(std::move(oldCoordinators)); // state_var_RVO
			this->~GetDesiredActorState();
			static_cast<GetDesiredActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(bool const& ok,int loopDepth) 
	{
		loopDepth = a_body1cont10(ok, loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(bool && ok,int loopDepth) 
	{
		loopDepth = a_body1cont10(std::move(ok), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<GetDesiredActor*>(this)->actor_wait_state > 0) static_cast<GetDesiredActor*>(this)->actor_wait_state = 0;
		static_cast<GetDesiredActor*>(this)->ActorCallback< GetDesiredActor, 3, bool >::remove();

	}
	void a_callback_fire(ActorCallback< GetDesiredActor, 3, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("getDesired", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDesired", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< GetDesiredActor, 3, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("getDesired", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDesired", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< GetDesiredActor, 3, bool >*,Error err) 
	{
		fdb_probe_actor_enter("getDesired", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getDesired", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Reference<AutoQuorumChange> self;
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::vector<NetworkAddress> oldCoordinators;
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Reference<IClusterConnectionRecord> ccr;
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	CoordinatorsResult* err;
															#line 1138 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	int desiredCount;
															#line 1146 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::set<AddressExclusion> excluded;
															#line 1149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::vector<ProcessData> workers;
															#line 5233 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via getDesired()
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetDesiredActor final : public Actor<std::vector<NetworkAddress>>, public ActorCallback< GetDesiredActor, 0, int >, public ActorCallback< GetDesiredActor, 1, std::vector<AddressExclusion> >, public ActorCallback< GetDesiredActor, 2, std::vector<ProcessData> >, public ActorCallback< GetDesiredActor, 3, bool >, public FastAllocated<GetDesiredActor>, public GetDesiredActorState<GetDesiredActor> {
															#line 5238 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<GetDesiredActor>::operator new;
	using FastAllocated<GetDesiredActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<NetworkAddress>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetDesiredActor, 0, int >;
friend struct ActorCallback< GetDesiredActor, 1, std::vector<AddressExclusion> >;
friend struct ActorCallback< GetDesiredActor, 2, std::vector<ProcessData> >;
friend struct ActorCallback< GetDesiredActor, 3, bool >;
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetDesiredActor(Reference<AutoQuorumChange> const& self,Transaction* const& tr,std::vector<NetworkAddress> const& oldCoordinators,Reference<IClusterConnectionRecord> const& ccr,CoordinatorsResult* const& err) 
															#line 5252 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<std::vector<NetworkAddress>>(),
		   GetDesiredActorState<GetDesiredActor>(self, tr, oldCoordinators, ccr, err)
	{
		fdb_probe_actor_enter("getDesired", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getDesired");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getDesired", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetDesiredActor, 0, int >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetDesiredActor, 1, std::vector<AddressExclusion> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GetDesiredActor, 2, std::vector<ProcessData> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< GetDesiredActor, 3, bool >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] static Future<std::vector<NetworkAddress>> getDesired( Reference<AutoQuorumChange> const& self, Transaction* const& tr, std::vector<NetworkAddress> const& oldCoordinators, Reference<IClusterConnectionRecord> const& ccr, CoordinatorsResult* const& err ) {
															#line 1133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<std::vector<NetworkAddress>>(new GetDesiredActor(self, tr, oldCoordinators, ccr, err));
															#line 5282 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1195 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

	// Select a desired set of workers such that
	// (1) the number of workers at each locality type (e.g., dcid) <= desiredCount; and
	// (2) prefer workers at a locality where less workers has been chosen than other localities: evenly distribute
	// workers.
	void addDesiredWorkers(std::vector<NetworkAddress>& chosen,
	                       const std::vector<ProcessData>& workers,
	                       int desiredCount,
	                       const std::set<AddressExclusion>& excluded) {
		std::vector<ProcessData> remainingWorkers(workers);
		deterministicRandom()->randomShuffle(remainingWorkers);

		std::partition(remainingWorkers.begin(), remainingWorkers.end(), [](const ProcessData& data) {
			return (data.processClass == ProcessClass::CoordinatorClass);
		});

		TraceEvent(SevDebug, "AutoSelectCoordinators").detail("CandidateWorkers", remainingWorkers.size());
		for (auto worker = remainingWorkers.begin(); worker != remainingWorkers.end(); worker++) {
			TraceEvent(SevDebug, "AutoSelectCoordinators")
			    .detail("Worker", worker->processClass.toString())
			    .detail("Address", worker->address.toString())
			    .detail("Locality", worker->locality.toString());
		}
		TraceEvent(SevDebug, "AutoSelectCoordinators").detail("ExcludedAddress", excluded.size());
		for (auto& excludedAddr : excluded) {
			TraceEvent(SevDebug, "AutoSelectCoordinators").detail("ExcludedAddress", excludedAddr.toString());
		}

		std::map<StringRef, int> maxCounts;
		std::map<StringRef, std::map<StringRef, int>> currentCounts;
		std::map<StringRef, int> hardLimits;

		std::vector<StringRef> fields({ LiteralStringRef("dcid"),
		                                LiteralStringRef("data_hall"),
		                                LiteralStringRef("zoneid"),
		                                LiteralStringRef("machineid") });

		for (auto field = fields.begin(); field != fields.end(); field++) {
			if (field->toString() == "zoneid") {
				hardLimits[*field] = 1;
			} else {
				hardLimits[*field] = desiredCount;
			}
		}

		while (chosen.size() < desiredCount) {
			bool found = false;
			for (auto worker = remainingWorkers.begin(); worker != remainingWorkers.end(); worker++) {
				if (addressExcluded(excluded, worker->address)) {
					continue;
				}
				// Exclude faulty node due to machine assassination
				if (g_network->isSimulated() && !g_simulator.getProcessByAddress(worker->address)->isReliable()) {
					TraceEvent("AutoSelectCoordinators").detail("SkipUnreliableWorker", worker->address.toString());
					continue;
				}
				bool valid = true;
				for (auto field = fields.begin(); field != fields.end(); field++) {
					if (maxCounts[*field] == 0) {
						maxCounts[*field] = 1;
					}
					auto value = worker->locality.get(*field).orDefault(LiteralStringRef(""));
					auto currentCount = currentCounts[*field][value];
					if (currentCount >= maxCounts[*field]) {
						valid = false;
						break;
					}
				}
				if (valid) {
					for (auto field = fields.begin(); field != fields.end(); field++) {
						auto value = worker->locality.get(*field).orDefault(LiteralStringRef(""));
						currentCounts[*field][value] += 1;
					}
					chosen.push_back(worker->address);
					remainingWorkers.erase(worker);
					found = true;
					break;
				}
			}
			if (!found) {
				bool canIncrement = false;
				for (auto field = fields.begin(); field != fields.end(); field++) {
					if (maxCounts[*field] < hardLimits[*field]) {
						maxCounts[*field] += 1;
						canIncrement = true;
						break;
					}
				}
				if (!canIncrement) {
					break;
				}
			}
		}
	}
};
Reference<IQuorumChange> autoQuorumChange(int desired) {
	return Reference<IQuorumChange>(new AutoQuorumChange(desired));
}

															#line 5385 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via excludeServers()
															#line 1294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class ExcludeServersActor>
															#line 1294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ExcludeServersActorState {
															#line 5392 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ExcludeServersActorState(Transaction* const& tr,std::vector<AddressExclusion> const& servers,bool const& failed) 
															#line 1294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr),
															#line 1294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   servers(servers),
															#line 1294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   failed(failed)
															#line 5403 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("excludeServers", reinterpret_cast<unsigned long>(this));

	}
	~ExcludeServersActorState() 
	{
		fdb_probe_actor_destroy("excludeServers", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1296 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1297 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1298 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 1299 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<std::vector<AddressExclusion>> __when_expr_0 = failed ? getExcludedFailedServerList(tr) : getExcludedServerList(tr);
															#line 1299 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ExcludeServersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5428 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ExcludeServersActor*>(this)->actor_wait_state = 1;
															#line 1299 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExcludeServersActor, 0, std::vector<AddressExclusion> >*>(static_cast<ExcludeServersActor*>(this)));
															#line 5433 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~ExcludeServersActorState();
		static_cast<ExcludeServersActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<AddressExclusion> const& excl,int loopDepth) 
	{
															#line 1300 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::set<AddressExclusion> exclusions(excl.begin(), excl.end());
															#line 1301 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		bool containNewExclusion = false;
															#line 1302 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto& s : servers ) {
															#line 1303 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (exclusions.find(s) != exclusions.end())
															#line 5462 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
				continue;
			}
															#line 1306 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			containNewExclusion = true;
															#line 1307 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (failed)
															#line 5470 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1308 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr->set(encodeFailedServersKey(s), StringRef());
															#line 5474 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
			else
			{
															#line 1310 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr->set(encodeExcludedServersKey(s), StringRef());
															#line 5480 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1314 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (containNewExclusion)
															#line 5485 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1315 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			std::string excludeVersionKey = deterministicRandom()->randomUniqueID().toString();
															#line 1316 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto serversVersionKey = failed ? failedServersVersionKey : excludedServersVersionKey;
															#line 1317 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->addReadConflictRange(singleKeyRange(serversVersionKey));
															#line 1318 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->set(serversVersionKey, excludeVersionKey);
															#line 5495 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1320 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		TraceEvent("ExcludeServersCommit") .detail("Servers", describe(servers)) .detail("ExcludeFailed", failed) .detail("ExclusionUpdated", containNewExclusion);
															#line 1324 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ExcludeServersActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExcludeServersActorState(); static_cast<ExcludeServersActor*>(this)->destroy(); return 0; }
															#line 5501 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ExcludeServersActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ExcludeServersActorState();
		static_cast<ExcludeServersActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<AddressExclusion> && excl,int loopDepth) 
	{
															#line 1300 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::set<AddressExclusion> exclusions(excl.begin(), excl.end());
															#line 1301 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		bool containNewExclusion = false;
															#line 1302 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto& s : servers ) {
															#line 1303 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (exclusions.find(s) != exclusions.end())
															#line 5519 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
				continue;
			}
															#line 1306 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			containNewExclusion = true;
															#line 1307 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (failed)
															#line 5527 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1308 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr->set(encodeFailedServersKey(s), StringRef());
															#line 5531 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
			else
			{
															#line 1310 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr->set(encodeExcludedServersKey(s), StringRef());
															#line 5537 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1314 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (containNewExclusion)
															#line 5542 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1315 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			std::string excludeVersionKey = deterministicRandom()->randomUniqueID().toString();
															#line 1316 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto serversVersionKey = failed ? failedServersVersionKey : excludedServersVersionKey;
															#line 1317 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->addReadConflictRange(singleKeyRange(serversVersionKey));
															#line 1318 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->set(serversVersionKey, excludeVersionKey);
															#line 5552 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1320 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		TraceEvent("ExcludeServersCommit") .detail("Servers", describe(servers)) .detail("ExcludeFailed", failed) .detail("ExclusionUpdated", containNewExclusion);
															#line 1324 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ExcludeServersActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExcludeServersActorState(); static_cast<ExcludeServersActor*>(this)->destroy(); return 0; }
															#line 5558 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ExcludeServersActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ExcludeServersActorState();
		static_cast<ExcludeServersActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<AddressExclusion> const& excl,int loopDepth) 
	{
		loopDepth = a_body1cont1(excl, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<AddressExclusion> && excl,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(excl), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ExcludeServersActor*>(this)->actor_wait_state > 0) static_cast<ExcludeServersActor*>(this)->actor_wait_state = 0;
		static_cast<ExcludeServersActor*>(this)->ActorCallback< ExcludeServersActor, 0, std::vector<AddressExclusion> >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeServersActor, 0, std::vector<AddressExclusion> >*,std::vector<AddressExclusion> const& value) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ExcludeServersActor, 0, std::vector<AddressExclusion> >*,std::vector<AddressExclusion> && value) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ExcludeServersActor, 0, std::vector<AddressExclusion> >*,Error err) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 1294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::vector<AddressExclusion> servers;
															#line 1294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	bool failed;
															#line 5635 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via excludeServers()
															#line 1294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ExcludeServersActor final : public Actor<Void>, public ActorCallback< ExcludeServersActor, 0, std::vector<AddressExclusion> >, public FastAllocated<ExcludeServersActor>, public ExcludeServersActorState<ExcludeServersActor> {
															#line 5640 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<ExcludeServersActor>::operator new;
	using FastAllocated<ExcludeServersActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ExcludeServersActor, 0, std::vector<AddressExclusion> >;
															#line 1294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ExcludeServersActor(Transaction* const& tr,std::vector<AddressExclusion> const& servers,bool const& failed) 
															#line 5651 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   ExcludeServersActorState<ExcludeServersActor>(tr, servers, failed)
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("excludeServers");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ExcludeServersActor, 0, std::vector<AddressExclusion> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> excludeServers( Transaction* const& tr, std::vector<AddressExclusion> const& servers, bool const& failed ) {
															#line 1294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new ExcludeServersActor(tr, servers, failed));
															#line 5679 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1326 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 5684 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via excludeServers()
															#line 1327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class ExcludeServersActor1>
															#line 1327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ExcludeServersActor1State {
															#line 5691 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ExcludeServersActor1State(Database const& cx,std::vector<AddressExclusion> const& servers,bool const& failed) 
															#line 1327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 1327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   servers(servers),
															#line 1327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   failed(failed)
															#line 5702 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("excludeServers", reinterpret_cast<unsigned long>(this));

	}
	~ExcludeServersActor1State() 
	{
		fdb_probe_actor_destroy("excludeServers", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1328 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (cx->apiVersionAtLeast(700))
															#line 5717 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1329 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				ryw = ReadYourWritesTransaction(cx);
															#line 1330 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				;
															#line 5723 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				loopDepth = a_body1loopHead1(loopDepth);
			}
			else
			{
															#line 1353 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr = Transaction(cx);
															#line 1354 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				;
															#line 5732 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				loopDepth = a_body1loopHead2(loopDepth);
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
		this->~ExcludeServersActor1State();
		static_cast<ExcludeServersActor1*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1332 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ryw.setOption(FDBTransactionOptions::SPECIAL_KEY_SPACE_ENABLE_WRITES);
															#line 1333 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ryw.set( SpecialKeySpace::getManagementApiCommandOptionSpecialKey(failed ? "failed" : "excluded", "force"), ValueRef());
															#line 1336 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			for( auto& s : servers ) {
															#line 1337 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				Key addr = failed ? SpecialKeySpace::getManagementApiCommandPrefix("failed").withSuffix(s.toString()) : SpecialKeySpace::getManagementApiCommandPrefix("exclude").withSuffix(s.toString());
															#line 1340 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				ryw.set(addr, ValueRef());
															#line 5772 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 1342 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("ExcludeServersSpecialKeySpaceCommit") .detail("Servers", describe(servers)) .detail("ExcludeFailed", failed);
															#line 1345 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_0 = ryw.commit();
															#line 1345 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ExcludeServersActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5780 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ExcludeServersActor1*>(this)->actor_wait_state = 1;
															#line 1345 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExcludeServersActor1, 0, Void >*>(static_cast<ExcludeServersActor1*>(this)));
															#line 5785 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1348 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("ExcludeServersError").errorUnsuppressed(e);
															#line 1349 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ryw.onError(e);
															#line 1349 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ExcludeServersActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5811 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ExcludeServersActor1*>(this)->actor_wait_state = 2;
															#line 1349 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExcludeServersActor1, 1, Void >*>(static_cast<ExcludeServersActor1*>(this)));
															#line 5816 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1346 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ExcludeServersActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExcludeServersActor1State(); static_cast<ExcludeServersActor1*>(this)->destroy(); return 0; }
															#line 5831 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ExcludeServersActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~ExcludeServersActor1State();
		static_cast<ExcludeServersActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 1346 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ExcludeServersActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExcludeServersActor1State(); static_cast<ExcludeServersActor1*>(this)->destroy(); return 0; }
															#line 5843 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ExcludeServersActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~ExcludeServersActor1State();
		static_cast<ExcludeServersActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<ExcludeServersActor1*>(this)->actor_wait_state > 0) static_cast<ExcludeServersActor1*>(this)->actor_wait_state = 0;
		static_cast<ExcludeServersActor1*>(this)->ActorCallback< ExcludeServersActor1, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeServersActor1, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ExcludeServersActor1, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ExcludeServersActor1, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<ExcludeServersActor1*>(this)->actor_wait_state > 0) static_cast<ExcludeServersActor1*>(this)->actor_wait_state = 0;
		static_cast<ExcludeServersActor1*>(this)->ActorCallback< ExcludeServersActor1, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeServersActor1, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ExcludeServersActor1, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ExcludeServersActor1, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopHead2(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2(int loopDepth) 
	{
		try {
															#line 1356 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_2 = excludeServers(&tr, servers, failed);
															#line 1356 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ExcludeServersActor1*>(this)->actor_wait_state < 0) return a_body1loopBody2Catch1(actor_cancelled(), loopDepth);
															#line 6003 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody2Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody2when1(__when_expr_2.get(), loopDepth); };
			static_cast<ExcludeServersActor1*>(this)->actor_wait_state = 3;
															#line 1356 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExcludeServersActor1, 2, Void >*>(static_cast<ExcludeServersActor1*>(this)));
															#line 6008 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody2Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody2Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody2cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead2(0);

		return loopDepth;
	}
	int a_body1loopBody2Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1360 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("ExcludeServersError").errorUnsuppressed(e);
															#line 1361 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_4 = tr.onError(e);
															#line 1361 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ExcludeServersActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6034 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody2Catch1when1(__when_expr_4.get(), loopDepth); };
			static_cast<ExcludeServersActor1*>(this)->actor_wait_state = 5;
															#line 1361 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ExcludeServersActor1, 4, Void >*>(static_cast<ExcludeServersActor1*>(this)));
															#line 6039 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody2cont2(Void const& _,int loopDepth) 
	{
															#line 1357 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_3 = tr.commit();
															#line 1357 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<ExcludeServersActor1*>(this)->actor_wait_state < 0) return a_body1loopBody2Catch1(actor_cancelled(), loopDepth);
															#line 6056 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody2Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody2cont2when1(__when_expr_3.get(), loopDepth); };
		static_cast<ExcludeServersActor1*>(this)->actor_wait_state = 4;
															#line 1357 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ExcludeServersActor1, 3, Void >*>(static_cast<ExcludeServersActor1*>(this)));
															#line 6061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody2cont2(Void && _,int loopDepth) 
	{
															#line 1357 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_3 = tr.commit();
															#line 1357 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<ExcludeServersActor1*>(this)->actor_wait_state < 0) return a_body1loopBody2Catch1(actor_cancelled(), loopDepth);
															#line 6072 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody2Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody2cont2when1(__when_expr_3.get(), loopDepth); };
		static_cast<ExcludeServersActor1*>(this)->actor_wait_state = 4;
															#line 1357 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ExcludeServersActor1, 3, Void >*>(static_cast<ExcludeServersActor1*>(this)));
															#line 6077 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ExcludeServersActor1*>(this)->actor_wait_state > 0) static_cast<ExcludeServersActor1*>(this)->actor_wait_state = 0;
		static_cast<ExcludeServersActor1*>(this)->ActorCallback< ExcludeServersActor1, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeServersActor1, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ExcludeServersActor1, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ExcludeServersActor1, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody2Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody2cont3(Void const& _,int loopDepth) 
	{
															#line 1358 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ExcludeServersActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExcludeServersActor1State(); static_cast<ExcludeServersActor1*>(this)->destroy(); return 0; }
															#line 6149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ExcludeServersActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~ExcludeServersActor1State();
		static_cast<ExcludeServersActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody2cont3(Void && _,int loopDepth) 
	{
															#line 1358 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ExcludeServersActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExcludeServersActor1State(); static_cast<ExcludeServersActor1*>(this)->destroy(); return 0; }
															#line 6161 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ExcludeServersActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~ExcludeServersActor1State();
		static_cast<ExcludeServersActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody2cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ExcludeServersActor1*>(this)->actor_wait_state > 0) static_cast<ExcludeServersActor1*>(this)->actor_wait_state = 0;
		static_cast<ExcludeServersActor1*>(this)->ActorCallback< ExcludeServersActor1, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeServersActor1, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody2cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ExcludeServersActor1, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody2cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ExcludeServersActor1, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody2Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody2Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2Catch1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<ExcludeServersActor1*>(this)->actor_wait_state > 0) static_cast<ExcludeServersActor1*>(this)->actor_wait_state = 0;
		static_cast<ExcludeServersActor1*>(this)->ActorCallback< ExcludeServersActor1, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeServersActor1, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody2Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< ExcludeServersActor1, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody2Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< ExcludeServersActor1, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 1327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 1327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::vector<AddressExclusion> servers;
															#line 1327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	bool failed;
															#line 1329 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ReadYourWritesTransaction ryw;
															#line 1353 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 6317 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via excludeServers()
															#line 1327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ExcludeServersActor1 final : public Actor<Void>, public ActorCallback< ExcludeServersActor1, 0, Void >, public ActorCallback< ExcludeServersActor1, 1, Void >, public ActorCallback< ExcludeServersActor1, 2, Void >, public ActorCallback< ExcludeServersActor1, 3, Void >, public ActorCallback< ExcludeServersActor1, 4, Void >, public FastAllocated<ExcludeServersActor1>, public ExcludeServersActor1State<ExcludeServersActor1> {
															#line 6322 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<ExcludeServersActor1>::operator new;
	using FastAllocated<ExcludeServersActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ExcludeServersActor1, 0, Void >;
friend struct ActorCallback< ExcludeServersActor1, 1, Void >;
friend struct ActorCallback< ExcludeServersActor1, 2, Void >;
friend struct ActorCallback< ExcludeServersActor1, 3, Void >;
friend struct ActorCallback< ExcludeServersActor1, 4, Void >;
															#line 1327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ExcludeServersActor1(Database const& cx,std::vector<AddressExclusion> const& servers,bool const& failed) 
															#line 6337 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   ExcludeServersActor1State<ExcludeServersActor1>(cx, servers, failed)
	{
		fdb_probe_actor_enter("excludeServers", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("excludeServers");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("excludeServers", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ExcludeServersActor1, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ExcludeServersActor1, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ExcludeServersActor1, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ExcludeServersActor1, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< ExcludeServersActor1, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> excludeServers( Database const& cx, std::vector<AddressExclusion> const& servers, bool const& failed ) {
															#line 1327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new ExcludeServersActor1(cx, servers, failed));
															#line 6369 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1366 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

// excludes localities by setting the keys in api version below 7.0
															#line 6375 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via excludeLocalities()
															#line 1368 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class ExcludeLocalitiesActor>
															#line 1368 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ExcludeLocalitiesActorState {
															#line 6382 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1368 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ExcludeLocalitiesActorState(Transaction* const& tr,std::unordered_set<std::string> const& localities,bool const& failed) 
															#line 1368 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1368 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr),
															#line 1368 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   localities(localities),
															#line 1368 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   failed(failed)
															#line 6393 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("excludeLocalities", reinterpret_cast<unsigned long>(this));

	}
	~ExcludeLocalitiesActorState() 
	{
		fdb_probe_actor_destroy("excludeLocalities", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1369 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1370 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1371 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1372 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 1373 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<std::vector<std::string>> __when_expr_0 = failed ? getExcludedFailedLocalityList(tr) : getExcludedLocalityList(tr);
															#line 1373 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ExcludeLocalitiesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6418 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ExcludeLocalitiesActor*>(this)->actor_wait_state = 1;
															#line 1373 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExcludeLocalitiesActor, 0, std::vector<std::string> >*>(static_cast<ExcludeLocalitiesActor*>(this)));
															#line 6423 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~ExcludeLocalitiesActorState();
		static_cast<ExcludeLocalitiesActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<std::string> const& excl,int loopDepth) 
	{
															#line 1374 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::set<std::string> exclusion(excl.begin(), excl.end());
															#line 1375 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		bool containNewExclusion = false;
															#line 1376 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( const auto& l : localities ) {
															#line 1377 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (exclusion.find(l) != exclusion.end())
															#line 6452 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
				continue;
			}
															#line 1380 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			containNewExclusion = true;
															#line 1381 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (failed)
															#line 6460 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1382 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr->set(encodeFailedLocalityKey(l), StringRef());
															#line 6464 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
			else
			{
															#line 1384 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr->set(encodeExcludedLocalityKey(l), StringRef());
															#line 6470 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1387 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (containNewExclusion)
															#line 6475 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1388 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			std::string excludeVersionKey = deterministicRandom()->randomUniqueID().toString();
															#line 1389 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto localityVersionKey = failed ? failedLocalityVersionKey : excludedLocalityVersionKey;
															#line 1390 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->addReadConflictRange(singleKeyRange(localityVersionKey));
															#line 1391 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->set(localityVersionKey, excludeVersionKey);
															#line 6485 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1393 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		TraceEvent("ExcludeLocalitiesCommit") .detail("Localities", describe(localities)) .detail("ExcludeFailed", failed) .detail("ExclusionUpdated", containNewExclusion);
															#line 1397 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ExcludeLocalitiesActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExcludeLocalitiesActorState(); static_cast<ExcludeLocalitiesActor*>(this)->destroy(); return 0; }
															#line 6491 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ExcludeLocalitiesActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ExcludeLocalitiesActorState();
		static_cast<ExcludeLocalitiesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<std::string> && excl,int loopDepth) 
	{
															#line 1374 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::set<std::string> exclusion(excl.begin(), excl.end());
															#line 1375 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		bool containNewExclusion = false;
															#line 1376 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( const auto& l : localities ) {
															#line 1377 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (exclusion.find(l) != exclusion.end())
															#line 6509 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
				continue;
			}
															#line 1380 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			containNewExclusion = true;
															#line 1381 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (failed)
															#line 6517 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1382 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr->set(encodeFailedLocalityKey(l), StringRef());
															#line 6521 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
			else
			{
															#line 1384 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr->set(encodeExcludedLocalityKey(l), StringRef());
															#line 6527 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1387 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (containNewExclusion)
															#line 6532 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1388 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			std::string excludeVersionKey = deterministicRandom()->randomUniqueID().toString();
															#line 1389 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto localityVersionKey = failed ? failedLocalityVersionKey : excludedLocalityVersionKey;
															#line 1390 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->addReadConflictRange(singleKeyRange(localityVersionKey));
															#line 1391 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->set(localityVersionKey, excludeVersionKey);
															#line 6542 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1393 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		TraceEvent("ExcludeLocalitiesCommit") .detail("Localities", describe(localities)) .detail("ExcludeFailed", failed) .detail("ExclusionUpdated", containNewExclusion);
															#line 1397 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ExcludeLocalitiesActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExcludeLocalitiesActorState(); static_cast<ExcludeLocalitiesActor*>(this)->destroy(); return 0; }
															#line 6548 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ExcludeLocalitiesActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ExcludeLocalitiesActorState();
		static_cast<ExcludeLocalitiesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<std::string> const& excl,int loopDepth) 
	{
		loopDepth = a_body1cont1(excl, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<std::string> && excl,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(excl), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ExcludeLocalitiesActor*>(this)->actor_wait_state > 0) static_cast<ExcludeLocalitiesActor*>(this)->actor_wait_state = 0;
		static_cast<ExcludeLocalitiesActor*>(this)->ActorCallback< ExcludeLocalitiesActor, 0, std::vector<std::string> >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeLocalitiesActor, 0, std::vector<std::string> >*,std::vector<std::string> const& value) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ExcludeLocalitiesActor, 0, std::vector<std::string> >*,std::vector<std::string> && value) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ExcludeLocalitiesActor, 0, std::vector<std::string> >*,Error err) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1368 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 1368 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::unordered_set<std::string> localities;
															#line 1368 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	bool failed;
															#line 6625 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via excludeLocalities()
															#line 1368 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ExcludeLocalitiesActor final : public Actor<Void>, public ActorCallback< ExcludeLocalitiesActor, 0, std::vector<std::string> >, public FastAllocated<ExcludeLocalitiesActor>, public ExcludeLocalitiesActorState<ExcludeLocalitiesActor> {
															#line 6630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<ExcludeLocalitiesActor>::operator new;
	using FastAllocated<ExcludeLocalitiesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ExcludeLocalitiesActor, 0, std::vector<std::string> >;
															#line 1368 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ExcludeLocalitiesActor(Transaction* const& tr,std::unordered_set<std::string> const& localities,bool const& failed) 
															#line 6641 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   ExcludeLocalitiesActorState<ExcludeLocalitiesActor>(tr, localities, failed)
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("excludeLocalities");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ExcludeLocalitiesActor, 0, std::vector<std::string> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1368 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> excludeLocalities( Transaction* const& tr, std::unordered_set<std::string> const& localities, bool const& failed ) {
															#line 1368 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new ExcludeLocalitiesActor(tr, localities, failed));
															#line 6669 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1399 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

// Exclude the servers matching the given set of localities from use as state servers.
// excludes localities by setting the keys.
															#line 6676 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via excludeLocalities()
															#line 1402 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class ExcludeLocalitiesActor1>
															#line 1402 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ExcludeLocalitiesActor1State {
															#line 6683 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1402 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ExcludeLocalitiesActor1State(Database const& cx,std::unordered_set<std::string> const& localities,bool const& failed) 
															#line 1402 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1402 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 1402 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   localities(localities),
															#line 1402 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   failed(failed)
															#line 6694 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("excludeLocalities", reinterpret_cast<unsigned long>(this));

	}
	~ExcludeLocalitiesActor1State() 
	{
		fdb_probe_actor_destroy("excludeLocalities", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1403 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (cx->apiVersionAtLeast(700))
															#line 6709 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1404 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				ryw = ReadYourWritesTransaction(cx);
															#line 1405 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				;
															#line 6715 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				loopDepth = a_body1loopHead1(loopDepth);
			}
			else
			{
															#line 1429 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr = Transaction(cx);
															#line 1430 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				;
															#line 6724 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				loopDepth = a_body1loopHead2(loopDepth);
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
		this->~ExcludeLocalitiesActor1State();
		static_cast<ExcludeLocalitiesActor1*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1407 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ryw.setOption(FDBTransactionOptions::SPECIAL_KEY_SPACE_ENABLE_WRITES);
															#line 1408 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ryw.set(SpecialKeySpace::getManagementApiCommandOptionSpecialKey( failed ? "failed_locality" : "excluded_locality", "force"), ValueRef());
															#line 1411 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			for( const auto& l : localities ) {
															#line 1412 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				Key addr = failed ? SpecialKeySpace::getManagementApiCommandPrefix("failedlocality").withSuffix(l) : SpecialKeySpace::getManagementApiCommandPrefix("excludedlocality").withSuffix(l);
															#line 1415 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				ryw.set(addr, ValueRef());
															#line 6764 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 1417 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("ExcludeLocalitiesSpecialKeySpaceCommit") .detail("Localities", describe(localities)) .detail("ExcludeFailed", failed);
															#line 1421 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_0 = ryw.commit();
															#line 1421 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 6772 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state = 1;
															#line 1421 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExcludeLocalitiesActor1, 0, Void >*>(static_cast<ExcludeLocalitiesActor1*>(this)));
															#line 6777 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1424 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("ExcludeLocalitiesError").errorUnsuppressed(e);
															#line 1425 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ryw.onError(e);
															#line 1425 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6803 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state = 2;
															#line 1425 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExcludeLocalitiesActor1, 1, Void >*>(static_cast<ExcludeLocalitiesActor1*>(this)));
															#line 6808 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1422 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ExcludeLocalitiesActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExcludeLocalitiesActor1State(); static_cast<ExcludeLocalitiesActor1*>(this)->destroy(); return 0; }
															#line 6823 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ExcludeLocalitiesActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~ExcludeLocalitiesActor1State();
		static_cast<ExcludeLocalitiesActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 1422 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ExcludeLocalitiesActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExcludeLocalitiesActor1State(); static_cast<ExcludeLocalitiesActor1*>(this)->destroy(); return 0; }
															#line 6835 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ExcludeLocalitiesActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~ExcludeLocalitiesActor1State();
		static_cast<ExcludeLocalitiesActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state > 0) static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state = 0;
		static_cast<ExcludeLocalitiesActor1*>(this)->ActorCallback< ExcludeLocalitiesActor1, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeLocalitiesActor1, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ExcludeLocalitiesActor1, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ExcludeLocalitiesActor1, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state > 0) static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state = 0;
		static_cast<ExcludeLocalitiesActor1*>(this)->ActorCallback< ExcludeLocalitiesActor1, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeLocalitiesActor1, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ExcludeLocalitiesActor1, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ExcludeLocalitiesActor1, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopHead2(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2(int loopDepth) 
	{
		try {
															#line 1432 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_2 = excludeLocalities(&tr, localities, failed);
															#line 1432 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state < 0) return a_body1loopBody2Catch1(actor_cancelled(), loopDepth);
															#line 6995 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody2Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody2when1(__when_expr_2.get(), loopDepth); };
			static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state = 3;
															#line 1432 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExcludeLocalitiesActor1, 2, Void >*>(static_cast<ExcludeLocalitiesActor1*>(this)));
															#line 7000 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody2Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody2Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody2cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead2(0);

		return loopDepth;
	}
	int a_body1loopBody2Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1436 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("ExcludeLocalitiesError").errorUnsuppressed(e);
															#line 1437 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_4 = tr.onError(e);
															#line 1437 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 7026 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody2Catch1when1(__when_expr_4.get(), loopDepth); };
			static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state = 5;
															#line 1437 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ExcludeLocalitiesActor1, 4, Void >*>(static_cast<ExcludeLocalitiesActor1*>(this)));
															#line 7031 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody2cont2(Void const& _,int loopDepth) 
	{
															#line 1433 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_3 = tr.commit();
															#line 1433 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state < 0) return a_body1loopBody2Catch1(actor_cancelled(), loopDepth);
															#line 7048 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody2Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody2cont2when1(__when_expr_3.get(), loopDepth); };
		static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state = 4;
															#line 1433 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ExcludeLocalitiesActor1, 3, Void >*>(static_cast<ExcludeLocalitiesActor1*>(this)));
															#line 7053 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody2cont2(Void && _,int loopDepth) 
	{
															#line 1433 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_3 = tr.commit();
															#line 1433 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state < 0) return a_body1loopBody2Catch1(actor_cancelled(), loopDepth);
															#line 7064 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody2Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody2cont2when1(__when_expr_3.get(), loopDepth); };
		static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state = 4;
															#line 1433 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ExcludeLocalitiesActor1, 3, Void >*>(static_cast<ExcludeLocalitiesActor1*>(this)));
															#line 7069 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state > 0) static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state = 0;
		static_cast<ExcludeLocalitiesActor1*>(this)->ActorCallback< ExcludeLocalitiesActor1, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeLocalitiesActor1, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ExcludeLocalitiesActor1, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ExcludeLocalitiesActor1, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody2Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody2cont3(Void const& _,int loopDepth) 
	{
															#line 1434 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ExcludeLocalitiesActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExcludeLocalitiesActor1State(); static_cast<ExcludeLocalitiesActor1*>(this)->destroy(); return 0; }
															#line 7141 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ExcludeLocalitiesActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~ExcludeLocalitiesActor1State();
		static_cast<ExcludeLocalitiesActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody2cont3(Void && _,int loopDepth) 
	{
															#line 1434 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ExcludeLocalitiesActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ExcludeLocalitiesActor1State(); static_cast<ExcludeLocalitiesActor1*>(this)->destroy(); return 0; }
															#line 7153 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ExcludeLocalitiesActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~ExcludeLocalitiesActor1State();
		static_cast<ExcludeLocalitiesActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody2cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state > 0) static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state = 0;
		static_cast<ExcludeLocalitiesActor1*>(this)->ActorCallback< ExcludeLocalitiesActor1, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeLocalitiesActor1, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody2cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ExcludeLocalitiesActor1, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody2cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ExcludeLocalitiesActor1, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody2Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody2Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2Catch1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state > 0) static_cast<ExcludeLocalitiesActor1*>(this)->actor_wait_state = 0;
		static_cast<ExcludeLocalitiesActor1*>(this)->ActorCallback< ExcludeLocalitiesActor1, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeLocalitiesActor1, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody2Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< ExcludeLocalitiesActor1, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody2Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< ExcludeLocalitiesActor1, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 1402 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 1402 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::unordered_set<std::string> localities;
															#line 1402 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	bool failed;
															#line 1404 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ReadYourWritesTransaction ryw;
															#line 1429 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 7309 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via excludeLocalities()
															#line 1402 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ExcludeLocalitiesActor1 final : public Actor<Void>, public ActorCallback< ExcludeLocalitiesActor1, 0, Void >, public ActorCallback< ExcludeLocalitiesActor1, 1, Void >, public ActorCallback< ExcludeLocalitiesActor1, 2, Void >, public ActorCallback< ExcludeLocalitiesActor1, 3, Void >, public ActorCallback< ExcludeLocalitiesActor1, 4, Void >, public FastAllocated<ExcludeLocalitiesActor1>, public ExcludeLocalitiesActor1State<ExcludeLocalitiesActor1> {
															#line 7314 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<ExcludeLocalitiesActor1>::operator new;
	using FastAllocated<ExcludeLocalitiesActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ExcludeLocalitiesActor1, 0, Void >;
friend struct ActorCallback< ExcludeLocalitiesActor1, 1, Void >;
friend struct ActorCallback< ExcludeLocalitiesActor1, 2, Void >;
friend struct ActorCallback< ExcludeLocalitiesActor1, 3, Void >;
friend struct ActorCallback< ExcludeLocalitiesActor1, 4, Void >;
															#line 1402 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ExcludeLocalitiesActor1(Database const& cx,std::unordered_set<std::string> const& localities,bool const& failed) 
															#line 7329 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   ExcludeLocalitiesActor1State<ExcludeLocalitiesActor1>(cx, localities, failed)
	{
		fdb_probe_actor_enter("excludeLocalities", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("excludeLocalities");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("excludeLocalities", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ExcludeLocalitiesActor1, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ExcludeLocalitiesActor1, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ExcludeLocalitiesActor1, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ExcludeLocalitiesActor1, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< ExcludeLocalitiesActor1, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1402 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> excludeLocalities( Database const& cx, std::unordered_set<std::string> const& localities, bool const& failed ) {
															#line 1402 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new ExcludeLocalitiesActor1(cx, localities, failed));
															#line 7361 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1442 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 7366 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via includeServers()
															#line 1443 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class IncludeServersActor>
															#line 1443 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class IncludeServersActorState {
															#line 7373 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1443 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	IncludeServersActorState(Database const& cx,std::vector<AddressExclusion> const& servers,bool const& failed) 
															#line 1443 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1443 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 1443 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   servers(servers),
															#line 1443 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   failed(failed),
															#line 1444 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   versionKey(deterministicRandom()->randomUniqueID().toString())
															#line 7386 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("includeServers", reinterpret_cast<unsigned long>(this));

	}
	~IncludeServersActorState() 
	{
		fdb_probe_actor_destroy("includeServers", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1445 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (cx->apiVersionAtLeast(700))
															#line 7401 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1446 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				ryw = ReadYourWritesTransaction(cx);
															#line 1447 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				;
															#line 7407 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				loopDepth = a_body1loopHead1(loopDepth);
			}
			else
			{
															#line 1485 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr = Transaction(cx);
															#line 1486 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				;
															#line 7416 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				loopDepth = a_body1loopHead2(loopDepth);
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
		this->~IncludeServersActorState();
		static_cast<IncludeServersActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1449 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ryw.setOption(FDBTransactionOptions::SPECIAL_KEY_SPACE_ENABLE_WRITES);
															#line 1450 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			for( auto& s : servers ) {
															#line 1451 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (!s.isValid())
															#line 7452 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 1452 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					if (failed)
															#line 7456 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					{
															#line 1453 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						ryw.clear(SpecialKeySpace::getManagementApiCommandRange("failed"));
															#line 7460 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
					else
					{
															#line 1455 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						ryw.clear(SpecialKeySpace::getManagementApiCommandRange("exclude"));
															#line 7466 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
				}
				else
				{
															#line 1458 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					Key addr = failed ? SpecialKeySpace::getManagementApiCommandPrefix("failed").withSuffix(s.toString()) : SpecialKeySpace::getManagementApiCommandPrefix("exclude").withSuffix(s.toString());
															#line 1461 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					ryw.clear(addr);
															#line 1470 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					if (s.isWholeMachine())
															#line 7477 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					{
															#line 1471 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						ryw.clear(KeyRangeRef(addr.withSuffix(LiteralStringRef(":")), addr.withSuffix(LiteralStringRef(";"))));
															#line 7481 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
				}
			}
															#line 1475 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("IncludeServersCommit").detail("Servers", describe(servers)).detail("Failed", failed);
															#line 1477 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_0 = ryw.commit();
															#line 1477 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<IncludeServersActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7491 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<IncludeServersActor*>(this)->actor_wait_state = 1;
															#line 1477 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IncludeServersActor, 0, Void >*>(static_cast<IncludeServersActor*>(this)));
															#line 7496 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1480 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("IncludeServersError").errorUnsuppressed(e);
															#line 1481 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ryw.onError(e);
															#line 1481 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<IncludeServersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 7522 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<IncludeServersActor*>(this)->actor_wait_state = 2;
															#line 1481 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IncludeServersActor, 1, Void >*>(static_cast<IncludeServersActor*>(this)));
															#line 7527 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1478 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<IncludeServersActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~IncludeServersActorState(); static_cast<IncludeServersActor*>(this)->destroy(); return 0; }
															#line 7542 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<IncludeServersActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~IncludeServersActorState();
		static_cast<IncludeServersActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 1478 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<IncludeServersActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~IncludeServersActorState(); static_cast<IncludeServersActor*>(this)->destroy(); return 0; }
															#line 7554 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<IncludeServersActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~IncludeServersActorState();
		static_cast<IncludeServersActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<IncludeServersActor*>(this)->actor_wait_state > 0) static_cast<IncludeServersActor*>(this)->actor_wait_state = 0;
		static_cast<IncludeServersActor*>(this)->ActorCallback< IncludeServersActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncludeServersActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("includeServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeServers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IncludeServersActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("includeServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeServers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IncludeServersActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("includeServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeServers", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<IncludeServersActor*>(this)->actor_wait_state > 0) static_cast<IncludeServersActor*>(this)->actor_wait_state = 0;
		static_cast<IncludeServersActor*>(this)->ActorCallback< IncludeServersActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncludeServersActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("includeServers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeServers", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< IncludeServersActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("includeServers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeServers", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< IncludeServersActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("includeServers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeServers", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopHead2(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2(int loopDepth) 
	{
		try {
															#line 1488 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1489 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1490 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1491 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 1495 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::CAUSAL_WRITE_RISKY);
															#line 1496 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (failed)
															#line 7722 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1497 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr.addReadConflictRange(singleKeyRange(failedServersVersionKey));
															#line 1498 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr.set(failedServersVersionKey, versionKey);
															#line 7728 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
			else
			{
															#line 1500 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr.addReadConflictRange(singleKeyRange(excludedServersVersionKey));
															#line 1501 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr.set(excludedServersVersionKey, versionKey);
															#line 7736 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 1504 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			for( auto& s : servers ) {
															#line 1505 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (!s.isValid())
															#line 7742 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 1506 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					if (failed)
															#line 7746 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					{
															#line 1507 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						tr.clear(failedServersKeys);
															#line 7750 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
					else
					{
															#line 1509 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						tr.clear(excludedServersKeys);
															#line 7756 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
				}
				else
				{
															#line 1511 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					if (s.isWholeMachine())
															#line 7763 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					{
															#line 1520 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						auto addr = failed ? encodeFailedServersKey(s) : encodeExcludedServersKey(s);
															#line 1521 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						tr.clear(singleKeyRange(addr));
															#line 1522 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						tr.clear(KeyRangeRef(addr + ':', addr + char(':' + 1)));
															#line 7771 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
					else
					{
															#line 1524 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						if (failed)
															#line 7777 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
						{
															#line 1525 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
							tr.clear(encodeFailedServersKey(s));
															#line 7781 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
						}
						else
						{
															#line 1527 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
							tr.clear(encodeExcludedServersKey(s));
															#line 7787 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
						}
					}
				}
			}
															#line 1532 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("IncludeServersCommit").detail("Servers", describe(servers)).detail("Failed", failed);
															#line 1534 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 1534 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<IncludeServersActor*>(this)->actor_wait_state < 0) return a_body1loopBody2Catch1(actor_cancelled(), loopDepth);
															#line 7798 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody2Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody2when1(__when_expr_2.get(), loopDepth); };
			static_cast<IncludeServersActor*>(this)->actor_wait_state = 3;
															#line 1534 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< IncludeServersActor, 2, Void >*>(static_cast<IncludeServersActor*>(this)));
															#line 7803 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody2Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody2Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody2cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead2(0);

		return loopDepth;
	}
	int a_body1loopBody2Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1537 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("IncludeServersError").errorUnsuppressed(e);
															#line 1538 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_3 = tr.onError(e);
															#line 1538 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<IncludeServersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 7829 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody2Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<IncludeServersActor*>(this)->actor_wait_state = 4;
															#line 1538 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< IncludeServersActor, 3, Void >*>(static_cast<IncludeServersActor*>(this)));
															#line 7834 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody2cont2(Void const& _,int loopDepth) 
	{
															#line 1535 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<IncludeServersActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~IncludeServersActorState(); static_cast<IncludeServersActor*>(this)->destroy(); return 0; }
															#line 7849 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<IncludeServersActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~IncludeServersActorState();
		static_cast<IncludeServersActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody2cont2(Void && _,int loopDepth) 
	{
															#line 1535 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<IncludeServersActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~IncludeServersActorState(); static_cast<IncludeServersActor*>(this)->destroy(); return 0; }
															#line 7861 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<IncludeServersActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~IncludeServersActorState();
		static_cast<IncludeServersActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<IncludeServersActor*>(this)->actor_wait_state > 0) static_cast<IncludeServersActor*>(this)->actor_wait_state = 0;
		static_cast<IncludeServersActor*>(this)->ActorCallback< IncludeServersActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncludeServersActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("includeServers", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeServers", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< IncludeServersActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("includeServers", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeServers", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< IncludeServersActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("includeServers", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody2Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeServers", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody2Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2Catch1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<IncludeServersActor*>(this)->actor_wait_state > 0) static_cast<IncludeServersActor*>(this)->actor_wait_state = 0;
		static_cast<IncludeServersActor*>(this)->ActorCallback< IncludeServersActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncludeServersActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("includeServers", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody2Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeServers", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< IncludeServersActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("includeServers", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody2Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeServers", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< IncludeServersActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("includeServers", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeServers", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 1443 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 1443 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::vector<AddressExclusion> servers;
															#line 1443 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	bool failed;
															#line 1444 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::string versionKey;
															#line 1446 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ReadYourWritesTransaction ryw;
															#line 1485 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 8019 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via includeServers()
															#line 1443 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class IncludeServersActor final : public Actor<Void>, public ActorCallback< IncludeServersActor, 0, Void >, public ActorCallback< IncludeServersActor, 1, Void >, public ActorCallback< IncludeServersActor, 2, Void >, public ActorCallback< IncludeServersActor, 3, Void >, public FastAllocated<IncludeServersActor>, public IncludeServersActorState<IncludeServersActor> {
															#line 8024 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<IncludeServersActor>::operator new;
	using FastAllocated<IncludeServersActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IncludeServersActor, 0, Void >;
friend struct ActorCallback< IncludeServersActor, 1, Void >;
friend struct ActorCallback< IncludeServersActor, 2, Void >;
friend struct ActorCallback< IncludeServersActor, 3, Void >;
															#line 1443 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	IncludeServersActor(Database const& cx,std::vector<AddressExclusion> const& servers,bool const& failed) 
															#line 8038 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   IncludeServersActorState<IncludeServersActor>(cx, servers, failed)
	{
		fdb_probe_actor_enter("includeServers", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("includeServers");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("includeServers", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< IncludeServersActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< IncludeServersActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< IncludeServersActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< IncludeServersActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1443 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> includeServers( Database const& cx, std::vector<AddressExclusion> const& servers, bool const& failed ) {
															#line 1443 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new IncludeServersActor(cx, servers, failed));
															#line 8069 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1543 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

// Remove the given localities from the exclusion list.
// include localities by clearing the keys.
															#line 8076 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via includeLocalities()
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class IncludeLocalitiesActor>
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class IncludeLocalitiesActorState {
															#line 8083 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	IncludeLocalitiesActorState(Database const& cx,std::vector<std::string> const& localities,bool const& failed,bool const& includeAll) 
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   localities(localities),
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   failed(failed),
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   includeAll(includeAll),
															#line 1547 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   versionKey(deterministicRandom()->randomUniqueID().toString())
															#line 8098 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("includeLocalities", reinterpret_cast<unsigned long>(this));

	}
	~IncludeLocalitiesActorState() 
	{
		fdb_probe_actor_destroy("includeLocalities", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1548 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (cx->apiVersionAtLeast(700))
															#line 8113 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1549 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				ryw = ReadYourWritesTransaction(cx);
															#line 1550 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				;
															#line 8119 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				loopDepth = a_body1loopHead1(loopDepth);
			}
			else
			{
															#line 1580 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr = Transaction(cx);
															#line 1581 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				;
															#line 8128 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				loopDepth = a_body1loopHead2(loopDepth);
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
		this->~IncludeLocalitiesActorState();
		static_cast<IncludeLocalitiesActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1552 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ryw.setOption(FDBTransactionOptions::SPECIAL_KEY_SPACE_ENABLE_WRITES);
															#line 1553 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (includeAll)
															#line 8162 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1554 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (failed)
															#line 8166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 1555 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					ryw.clear(SpecialKeySpace::getManagementApiCommandRange("failedlocality"));
															#line 8170 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
				else
				{
															#line 1557 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					ryw.clear(SpecialKeySpace::getManagementApiCommandRange("excludedlocality"));
															#line 8176 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
			}
			else
			{
															#line 1560 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				for( const auto& l : localities ) {
															#line 1561 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					Key locality = failed ? SpecialKeySpace::getManagementApiCommandPrefix("failedlocality").withSuffix(l) : SpecialKeySpace::getManagementApiCommandPrefix("excludedlocality").withSuffix(l);
															#line 1564 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					ryw.clear(locality);
															#line 8187 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
			}
															#line 1567 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("IncludeLocalitiesCommit") .detail("Localities", describe(localities)) .detail("Failed", failed) .detail("IncludeAll", includeAll);
															#line 1572 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_0 = ryw.commit();
															#line 1572 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8196 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state = 1;
															#line 1572 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< IncludeLocalitiesActor, 0, Void >*>(static_cast<IncludeLocalitiesActor*>(this)));
															#line 8201 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1575 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("IncludeLocalitiesError").errorUnsuppressed(e);
															#line 1576 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ryw.onError(e);
															#line 1576 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 8227 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state = 2;
															#line 1576 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< IncludeLocalitiesActor, 1, Void >*>(static_cast<IncludeLocalitiesActor*>(this)));
															#line 8232 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1573 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<IncludeLocalitiesActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~IncludeLocalitiesActorState(); static_cast<IncludeLocalitiesActor*>(this)->destroy(); return 0; }
															#line 8247 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<IncludeLocalitiesActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~IncludeLocalitiesActorState();
		static_cast<IncludeLocalitiesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 1573 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<IncludeLocalitiesActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~IncludeLocalitiesActorState(); static_cast<IncludeLocalitiesActor*>(this)->destroy(); return 0; }
															#line 8259 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<IncludeLocalitiesActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~IncludeLocalitiesActorState();
		static_cast<IncludeLocalitiesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state > 0) static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state = 0;
		static_cast<IncludeLocalitiesActor*>(this)->ActorCallback< IncludeLocalitiesActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncludeLocalitiesActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("includeLocalities", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeLocalities", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< IncludeLocalitiesActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("includeLocalities", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeLocalities", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< IncludeLocalitiesActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("includeLocalities", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeLocalities", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state > 0) static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state = 0;
		static_cast<IncludeLocalitiesActor*>(this)->ActorCallback< IncludeLocalitiesActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncludeLocalitiesActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("includeLocalities", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeLocalities", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< IncludeLocalitiesActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("includeLocalities", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeLocalities", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< IncludeLocalitiesActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("includeLocalities", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeLocalities", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopHead2(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2(int loopDepth) 
	{
		try {
															#line 1583 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1584 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1585 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1586 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 1590 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::CAUSAL_WRITE_RISKY);
															#line 1591 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (failed)
															#line 8427 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1592 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr.addReadConflictRange(singleKeyRange(failedLocalityVersionKey));
															#line 1593 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr.set(failedLocalityVersionKey, versionKey);
															#line 8433 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
			else
			{
															#line 1595 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr.addReadConflictRange(singleKeyRange(excludedLocalityVersionKey));
															#line 1596 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr.set(excludedLocalityVersionKey, versionKey);
															#line 8441 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 1599 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (includeAll)
															#line 8445 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1600 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (failed)
															#line 8449 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 1601 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					tr.clear(failedLocalityKeys);
															#line 8453 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
				else
				{
															#line 1603 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					tr.clear(excludedLocalityKeys);
															#line 8459 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
			}
			else
			{
															#line 1606 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				for( const auto& l : localities ) {
															#line 1607 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					if (failed)
															#line 8468 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					{
															#line 1608 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						tr.clear(encodeFailedLocalityKey(l));
															#line 8472 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
					else
					{
															#line 1610 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						tr.clear(encodeExcludedLocalityKey(l));
															#line 8478 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
				}
			}
															#line 1615 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("IncludeLocalitiesCommit") .detail("Localities", describe(localities)) .detail("Failed", failed) .detail("IncludeAll", includeAll);
															#line 1620 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 1620 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state < 0) return a_body1loopBody2Catch1(actor_cancelled(), loopDepth);
															#line 8488 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody2Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody2when1(__when_expr_2.get(), loopDepth); };
			static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state = 3;
															#line 1620 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< IncludeLocalitiesActor, 2, Void >*>(static_cast<IncludeLocalitiesActor*>(this)));
															#line 8493 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody2Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody2Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody2cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead2(0);

		return loopDepth;
	}
	int a_body1loopBody2Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1623 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("IncludeLocalitiesError").errorUnsuppressed(e);
															#line 1624 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_3 = tr.onError(e);
															#line 1624 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 8519 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody2Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state = 4;
															#line 1624 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< IncludeLocalitiesActor, 3, Void >*>(static_cast<IncludeLocalitiesActor*>(this)));
															#line 8524 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody2cont2(Void const& _,int loopDepth) 
	{
															#line 1621 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<IncludeLocalitiesActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~IncludeLocalitiesActorState(); static_cast<IncludeLocalitiesActor*>(this)->destroy(); return 0; }
															#line 8539 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<IncludeLocalitiesActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~IncludeLocalitiesActorState();
		static_cast<IncludeLocalitiesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody2cont2(Void && _,int loopDepth) 
	{
															#line 1621 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<IncludeLocalitiesActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~IncludeLocalitiesActorState(); static_cast<IncludeLocalitiesActor*>(this)->destroy(); return 0; }
															#line 8551 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<IncludeLocalitiesActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~IncludeLocalitiesActorState();
		static_cast<IncludeLocalitiesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state > 0) static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state = 0;
		static_cast<IncludeLocalitiesActor*>(this)->ActorCallback< IncludeLocalitiesActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncludeLocalitiesActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("includeLocalities", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeLocalities", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< IncludeLocalitiesActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("includeLocalities", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeLocalities", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< IncludeLocalitiesActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("includeLocalities", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody2Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody2Catch1(error, 0);
		} catch (...) {
			a_body1loopBody2Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeLocalities", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody2Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2Catch1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody2Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody2Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state > 0) static_cast<IncludeLocalitiesActor*>(this)->actor_wait_state = 0;
		static_cast<IncludeLocalitiesActor*>(this)->ActorCallback< IncludeLocalitiesActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< IncludeLocalitiesActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("includeLocalities", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody2Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeLocalities", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< IncludeLocalitiesActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("includeLocalities", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody2Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeLocalities", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< IncludeLocalitiesActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("includeLocalities", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("includeLocalities", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::vector<std::string> localities;
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	bool failed;
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	bool includeAll;
															#line 1547 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::string versionKey;
															#line 1549 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ReadYourWritesTransaction ryw;
															#line 1580 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 8711 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via includeLocalities()
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class IncludeLocalitiesActor final : public Actor<Void>, public ActorCallback< IncludeLocalitiesActor, 0, Void >, public ActorCallback< IncludeLocalitiesActor, 1, Void >, public ActorCallback< IncludeLocalitiesActor, 2, Void >, public ActorCallback< IncludeLocalitiesActor, 3, Void >, public FastAllocated<IncludeLocalitiesActor>, public IncludeLocalitiesActorState<IncludeLocalitiesActor> {
															#line 8716 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<IncludeLocalitiesActor>::operator new;
	using FastAllocated<IncludeLocalitiesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< IncludeLocalitiesActor, 0, Void >;
friend struct ActorCallback< IncludeLocalitiesActor, 1, Void >;
friend struct ActorCallback< IncludeLocalitiesActor, 2, Void >;
friend struct ActorCallback< IncludeLocalitiesActor, 3, Void >;
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	IncludeLocalitiesActor(Database const& cx,std::vector<std::string> const& localities,bool const& failed,bool const& includeAll) 
															#line 8730 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   IncludeLocalitiesActorState<IncludeLocalitiesActor>(cx, localities, failed, includeAll)
	{
		fdb_probe_actor_enter("includeLocalities", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("includeLocalities");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("includeLocalities", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< IncludeLocalitiesActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< IncludeLocalitiesActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< IncludeLocalitiesActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< IncludeLocalitiesActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> includeLocalities( Database const& cx, std::vector<std::string> const& localities, bool const& failed, bool const& includeAll ) {
															#line 1546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new IncludeLocalitiesActor(cx, localities, failed, includeAll));
															#line 8761 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1629 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 8766 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via setClass()
															#line 1630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class SetClassActor>
															#line 1630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class SetClassActorState {
															#line 8773 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	SetClassActorState(Database const& cx,AddressExclusion const& server,ProcessClass const& processClass) 
															#line 1630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 1630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   server(server),
															#line 1630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   processClass(processClass),
															#line 1631 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 8786 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("setClass", reinterpret_cast<unsigned long>(this));

	}
	~SetClassActorState() 
	{
		fdb_probe_actor_destroy("setClass", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1633 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 8801 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~SetClassActorState();
		static_cast<SetClassActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1635 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1636 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1637 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1638 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 1640 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<std::vector<ProcessData>> __when_expr_0 = getWorkers(&tr);
															#line 1640 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<SetClassActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8842 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SetClassActor*>(this)->actor_wait_state = 1;
															#line 1640 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SetClassActor, 0, std::vector<ProcessData> >*>(static_cast<SetClassActor*>(this)));
															#line 8847 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1660 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_2 = tr.onError(e);
															#line 1660 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<SetClassActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 8871 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<SetClassActor*>(this)->actor_wait_state = 3;
															#line 1660 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SetClassActor, 2, Void >*>(static_cast<SetClassActor*>(this)));
															#line 8876 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1642 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		bool foundChange = false;
															#line 1643 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(int i = 0;i < workers.size();i++) {
															#line 1644 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (server.excludes(workers[i].address))
															#line 8895 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1645 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (processClass.classType() != ProcessClass::InvalidClass)
															#line 8899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 1646 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					tr.set(processClassKeyFor(workers[i].locality.processId().get()), processClassValue(processClass));
															#line 8903 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
				else
				{
															#line 1649 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					tr.clear(processClassKeyFor(workers[i].locality.processId().get()));
															#line 8909 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
															#line 1650 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				foundChange = true;
															#line 8913 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1654 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (foundChange)
															#line 8918 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1655 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.set(processClassChangeKey, deterministicRandom()->randomUniqueID().toString());
															#line 8922 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1657 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 1657 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<SetClassActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8928 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<SetClassActor*>(this)->actor_wait_state = 2;
															#line 1657 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SetClassActor, 1, Void >*>(static_cast<SetClassActor*>(this)));
															#line 8933 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::vector<ProcessData> && workers,int loopDepth) 
	{
															#line 1642 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		bool foundChange = false;
															#line 1643 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(int i = 0;i < workers.size();i++) {
															#line 1644 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (server.excludes(workers[i].address))
															#line 8946 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1645 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (processClass.classType() != ProcessClass::InvalidClass)
															#line 8950 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 1646 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					tr.set(processClassKeyFor(workers[i].locality.processId().get()), processClassValue(processClass));
															#line 8954 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
				else
				{
															#line 1649 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					tr.clear(processClassKeyFor(workers[i].locality.processId().get()));
															#line 8960 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
															#line 1650 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				foundChange = true;
															#line 8964 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1654 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (foundChange)
															#line 8969 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1655 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.set(processClassChangeKey, deterministicRandom()->randomUniqueID().toString());
															#line 8973 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1657 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 1657 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<SetClassActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8979 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<SetClassActor*>(this)->actor_wait_state = 2;
															#line 1657 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SetClassActor, 1, Void >*>(static_cast<SetClassActor*>(this)));
															#line 8984 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		if (static_cast<SetClassActor*>(this)->actor_wait_state > 0) static_cast<SetClassActor*>(this)->actor_wait_state = 0;
		static_cast<SetClassActor*>(this)->ActorCallback< SetClassActor, 0, std::vector<ProcessData> >::remove();

	}
	void a_callback_fire(ActorCallback< SetClassActor, 0, std::vector<ProcessData> >*,std::vector<ProcessData> const& value) 
	{
		fdb_probe_actor_enter("setClass", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setClass", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SetClassActor, 0, std::vector<ProcessData> >*,std::vector<ProcessData> && value) 
	{
		fdb_probe_actor_enter("setClass", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setClass", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SetClassActor, 0, std::vector<ProcessData> >*,Error err) 
	{
		fdb_probe_actor_enter("setClass", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setClass", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 1658 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<SetClassActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SetClassActorState(); static_cast<SetClassActor*>(this)->destroy(); return 0; }
															#line 9056 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<SetClassActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SetClassActorState();
		static_cast<SetClassActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 1658 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<SetClassActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SetClassActorState(); static_cast<SetClassActor*>(this)->destroy(); return 0; }
															#line 9068 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<SetClassActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SetClassActorState();
		static_cast<SetClassActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<SetClassActor*>(this)->actor_wait_state > 0) static_cast<SetClassActor*>(this)->actor_wait_state = 0;
		static_cast<SetClassActor*>(this)->ActorCallback< SetClassActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SetClassActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("setClass", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setClass", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SetClassActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("setClass", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setClass", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SetClassActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("setClass", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setClass", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<SetClassActor*>(this)->actor_wait_state > 0) static_cast<SetClassActor*>(this)->actor_wait_state = 0;
		static_cast<SetClassActor*>(this)->ActorCallback< SetClassActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SetClassActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("setClass", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setClass", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< SetClassActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("setClass", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setClass", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< SetClassActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("setClass", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setClass", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 1630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 1630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	AddressExclusion server;
															#line 1630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ProcessClass processClass;
															#line 1631 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 9222 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via setClass()
															#line 1630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class SetClassActor final : public Actor<Void>, public ActorCallback< SetClassActor, 0, std::vector<ProcessData> >, public ActorCallback< SetClassActor, 1, Void >, public ActorCallback< SetClassActor, 2, Void >, public FastAllocated<SetClassActor>, public SetClassActorState<SetClassActor> {
															#line 9227 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<SetClassActor>::operator new;
	using FastAllocated<SetClassActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SetClassActor, 0, std::vector<ProcessData> >;
friend struct ActorCallback< SetClassActor, 1, Void >;
friend struct ActorCallback< SetClassActor, 2, Void >;
															#line 1630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	SetClassActor(Database const& cx,AddressExclusion const& server,ProcessClass const& processClass) 
															#line 9240 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   SetClassActorState<SetClassActor>(cx, server, processClass)
	{
		fdb_probe_actor_enter("setClass", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("setClass");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("setClass", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SetClassActor, 0, std::vector<ProcessData> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SetClassActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< SetClassActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> setClass( Database const& cx, AddressExclusion const& server, ProcessClass const& processClass ) {
															#line 1630 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new SetClassActor(cx, server, processClass));
															#line 9270 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1664 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 9275 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via getExcludedServerList()
															#line 1665 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class GetExcludedServerListActor>
															#line 1665 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetExcludedServerListActorState {
															#line 9282 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1665 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetExcludedServerListActorState(Transaction* const& tr) 
															#line 1665 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1665 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr)
															#line 9289 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("getExcludedServerList", reinterpret_cast<unsigned long>(this));

	}
	~GetExcludedServerListActorState() 
	{
		fdb_probe_actor_destroy("getExcludedServerList", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1666 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(excludedServersKeys, CLIENT_KNOBS->TOO_MANY);
															#line 1666 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetExcludedServerListActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9306 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetExcludedServerListActor*>(this)->actor_wait_state = 1;
															#line 1666 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetExcludedServerListActor, 0, RangeResult >*>(static_cast<GetExcludedServerListActor*>(this)));
															#line 9311 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~GetExcludedServerListActorState();
		static_cast<GetExcludedServerListActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1667 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(!r.more && r.size() < CLIENT_KNOBS->TOO_MANY);
															#line 1669 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::vector<AddressExclusion> exclusions;
															#line 1670 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(auto i = r.begin();i != r.end();++i) {
															#line 1671 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto a = decodeExcludedServersKey(i->key);
															#line 1672 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (a.isValid())
															#line 9342 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1673 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				exclusions.push_back(a);
															#line 9346 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1675 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		uniquify(exclusions);
															#line 1676 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetExcludedServerListActor*>(this)->SAV<std::vector<AddressExclusion>>::futures) { (void)(exclusions); this->~GetExcludedServerListActorState(); static_cast<GetExcludedServerListActor*>(this)->destroy(); return 0; }
															#line 9353 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetExcludedServerListActor*>(this)->SAV< std::vector<AddressExclusion> >::value()) std::vector<AddressExclusion>(exclusions);
		this->~GetExcludedServerListActorState();
		static_cast<GetExcludedServerListActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& __r,int loopDepth) 
	{
															#line 1666 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		r = __r;
															#line 9365 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && __r,int loopDepth) 
	{
		r = std::move(__r);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetExcludedServerListActor*>(this)->actor_wait_state > 0) static_cast<GetExcludedServerListActor*>(this)->actor_wait_state = 0;
		static_cast<GetExcludedServerListActor*>(this)->ActorCallback< GetExcludedServerListActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetExcludedServerListActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getExcludedServerList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getExcludedServerList", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetExcludedServerListActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getExcludedServerList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getExcludedServerList", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetExcludedServerListActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getExcludedServerList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getExcludedServerList", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1665 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 1666 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	RangeResult r;
															#line 9432 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via getExcludedServerList()
															#line 1665 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetExcludedServerListActor final : public Actor<std::vector<AddressExclusion>>, public ActorCallback< GetExcludedServerListActor, 0, RangeResult >, public FastAllocated<GetExcludedServerListActor>, public GetExcludedServerListActorState<GetExcludedServerListActor> {
															#line 9437 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<GetExcludedServerListActor>::operator new;
	using FastAllocated<GetExcludedServerListActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<AddressExclusion>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetExcludedServerListActor, 0, RangeResult >;
															#line 1665 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetExcludedServerListActor(Transaction* const& tr) 
															#line 9448 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<std::vector<AddressExclusion>>(),
		   GetExcludedServerListActorState<GetExcludedServerListActor>(tr)
	{
		fdb_probe_actor_enter("getExcludedServerList", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getExcludedServerList");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getExcludedServerList", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetExcludedServerListActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1665 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<std::vector<AddressExclusion>> getExcludedServerList( Transaction* const& tr ) {
															#line 1665 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<std::vector<AddressExclusion>>(new GetExcludedServerListActor(tr));
															#line 9476 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1678 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 9481 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via getExcludedFailedServerList()
															#line 1679 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class GetExcludedFailedServerListActor>
															#line 1679 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetExcludedFailedServerListActorState {
															#line 9488 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1679 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetExcludedFailedServerListActorState(Transaction* const& tr) 
															#line 1679 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1679 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr)
															#line 9495 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("getExcludedFailedServerList", reinterpret_cast<unsigned long>(this));

	}
	~GetExcludedFailedServerListActorState() 
	{
		fdb_probe_actor_destroy("getExcludedFailedServerList", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1680 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(failedServersKeys, CLIENT_KNOBS->TOO_MANY);
															#line 1680 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetExcludedFailedServerListActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9512 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetExcludedFailedServerListActor*>(this)->actor_wait_state = 1;
															#line 1680 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetExcludedFailedServerListActor, 0, RangeResult >*>(static_cast<GetExcludedFailedServerListActor*>(this)));
															#line 9517 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~GetExcludedFailedServerListActorState();
		static_cast<GetExcludedFailedServerListActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1681 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(!r.more && r.size() < CLIENT_KNOBS->TOO_MANY);
															#line 1683 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::vector<AddressExclusion> exclusions;
															#line 1684 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(auto i = r.begin();i != r.end();++i) {
															#line 1685 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto a = decodeFailedServersKey(i->key);
															#line 1686 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (a.isValid())
															#line 9548 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1687 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				exclusions.push_back(a);
															#line 9552 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1689 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		uniquify(exclusions);
															#line 1690 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetExcludedFailedServerListActor*>(this)->SAV<std::vector<AddressExclusion>>::futures) { (void)(exclusions); this->~GetExcludedFailedServerListActorState(); static_cast<GetExcludedFailedServerListActor*>(this)->destroy(); return 0; }
															#line 9559 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetExcludedFailedServerListActor*>(this)->SAV< std::vector<AddressExclusion> >::value()) std::vector<AddressExclusion>(exclusions);
		this->~GetExcludedFailedServerListActorState();
		static_cast<GetExcludedFailedServerListActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& __r,int loopDepth) 
	{
															#line 1680 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		r = __r;
															#line 9571 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && __r,int loopDepth) 
	{
		r = std::move(__r);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetExcludedFailedServerListActor*>(this)->actor_wait_state > 0) static_cast<GetExcludedFailedServerListActor*>(this)->actor_wait_state = 0;
		static_cast<GetExcludedFailedServerListActor*>(this)->ActorCallback< GetExcludedFailedServerListActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetExcludedFailedServerListActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getExcludedFailedServerList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getExcludedFailedServerList", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetExcludedFailedServerListActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getExcludedFailedServerList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getExcludedFailedServerList", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetExcludedFailedServerListActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getExcludedFailedServerList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getExcludedFailedServerList", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1679 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 1680 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	RangeResult r;
															#line 9638 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via getExcludedFailedServerList()
															#line 1679 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetExcludedFailedServerListActor final : public Actor<std::vector<AddressExclusion>>, public ActorCallback< GetExcludedFailedServerListActor, 0, RangeResult >, public FastAllocated<GetExcludedFailedServerListActor>, public GetExcludedFailedServerListActorState<GetExcludedFailedServerListActor> {
															#line 9643 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<GetExcludedFailedServerListActor>::operator new;
	using FastAllocated<GetExcludedFailedServerListActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<AddressExclusion>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetExcludedFailedServerListActor, 0, RangeResult >;
															#line 1679 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetExcludedFailedServerListActor(Transaction* const& tr) 
															#line 9654 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<std::vector<AddressExclusion>>(),
		   GetExcludedFailedServerListActorState<GetExcludedFailedServerListActor>(tr)
	{
		fdb_probe_actor_enter("getExcludedFailedServerList", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getExcludedFailedServerList");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getExcludedFailedServerList", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetExcludedFailedServerListActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1679 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<std::vector<AddressExclusion>> getExcludedFailedServerList( Transaction* const& tr ) {
															#line 1679 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<std::vector<AddressExclusion>>(new GetExcludedFailedServerListActor(tr));
															#line 9682 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1692 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 9687 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via getAllExcludedServers()
															#line 1693 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class GetAllExcludedServersActor>
															#line 1693 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetAllExcludedServersActorState {
															#line 9694 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1693 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetAllExcludedServersActorState(Transaction* const& tr) 
															#line 1693 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1693 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr),
															#line 1694 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   exclusions()
															#line 9703 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("getAllExcludedServers", reinterpret_cast<unsigned long>(this));

	}
	~GetAllExcludedServersActorState() 
	{
		fdb_probe_actor_destroy("getAllExcludedServers", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1695 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<std::vector<AddressExclusion>> __when_expr_0 = getExcludedServerList(tr);
															#line 1695 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetAllExcludedServersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9720 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetAllExcludedServersActor*>(this)->actor_wait_state = 1;
															#line 1695 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetAllExcludedServersActor, 0, std::vector<AddressExclusion> >*>(static_cast<GetAllExcludedServersActor*>(this)));
															#line 9725 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~GetAllExcludedServersActorState();
		static_cast<GetAllExcludedServersActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<AddressExclusion> const& excludedServers,int loopDepth) 
	{
															#line 1696 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		exclusions.insert(exclusions.end(), excludedServers.begin(), excludedServers.end());
															#line 1697 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<std::vector<AddressExclusion>> __when_expr_1 = getExcludedFailedServerList(tr);
															#line 1697 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<GetAllExcludedServersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9752 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetAllExcludedServersActor*>(this)->actor_wait_state = 2;
															#line 1697 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetAllExcludedServersActor, 1, std::vector<AddressExclusion> >*>(static_cast<GetAllExcludedServersActor*>(this)));
															#line 9757 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<AddressExclusion> && excludedServers,int loopDepth) 
	{
															#line 1696 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		exclusions.insert(exclusions.end(), excludedServers.begin(), excludedServers.end());
															#line 1697 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<std::vector<AddressExclusion>> __when_expr_1 = getExcludedFailedServerList(tr);
															#line 1697 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<GetAllExcludedServersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9770 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetAllExcludedServersActor*>(this)->actor_wait_state = 2;
															#line 1697 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetAllExcludedServersActor, 1, std::vector<AddressExclusion> >*>(static_cast<GetAllExcludedServersActor*>(this)));
															#line 9775 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<AddressExclusion> const& excludedServers,int loopDepth) 
	{
		loopDepth = a_body1cont1(excludedServers, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<AddressExclusion> && excludedServers,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(excludedServers), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetAllExcludedServersActor*>(this)->actor_wait_state > 0) static_cast<GetAllExcludedServersActor*>(this)->actor_wait_state = 0;
		static_cast<GetAllExcludedServersActor*>(this)->ActorCallback< GetAllExcludedServersActor, 0, std::vector<AddressExclusion> >::remove();

	}
	void a_callback_fire(ActorCallback< GetAllExcludedServersActor, 0, std::vector<AddressExclusion> >*,std::vector<AddressExclusion> const& value) 
	{
		fdb_probe_actor_enter("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetAllExcludedServersActor, 0, std::vector<AddressExclusion> >*,std::vector<AddressExclusion> && value) 
	{
		fdb_probe_actor_enter("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetAllExcludedServersActor, 0, std::vector<AddressExclusion> >*,Error err) 
	{
		fdb_probe_actor_enter("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(std::vector<AddressExclusion> const& excludedFailed,int loopDepth) 
	{
															#line 1698 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		exclusions.insert(exclusions.end(), excludedFailed.begin(), excludedFailed.end());
															#line 1699 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		uniquify(exclusions);
															#line 1700 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetAllExcludedServersActor*>(this)->SAV<std::vector<AddressExclusion>>::futures) { (void)(exclusions); this->~GetAllExcludedServersActorState(); static_cast<GetAllExcludedServersActor*>(this)->destroy(); return 0; }
															#line 9851 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetAllExcludedServersActor*>(this)->SAV< std::vector<AddressExclusion> >::value()) std::vector<AddressExclusion>(std::move(exclusions)); // state_var_RVO
		this->~GetAllExcludedServersActorState();
		static_cast<GetAllExcludedServersActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(std::vector<AddressExclusion> && excludedFailed,int loopDepth) 
	{
															#line 1698 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		exclusions.insert(exclusions.end(), excludedFailed.begin(), excludedFailed.end());
															#line 1699 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		uniquify(exclusions);
															#line 1700 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetAllExcludedServersActor*>(this)->SAV<std::vector<AddressExclusion>>::futures) { (void)(exclusions); this->~GetAllExcludedServersActorState(); static_cast<GetAllExcludedServersActor*>(this)->destroy(); return 0; }
															#line 9867 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetAllExcludedServersActor*>(this)->SAV< std::vector<AddressExclusion> >::value()) std::vector<AddressExclusion>(std::move(exclusions)); // state_var_RVO
		this->~GetAllExcludedServersActorState();
		static_cast<GetAllExcludedServersActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(std::vector<AddressExclusion> const& excludedFailed,int loopDepth) 
	{
		loopDepth = a_body1cont2(excludedFailed, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(std::vector<AddressExclusion> && excludedFailed,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(excludedFailed), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetAllExcludedServersActor*>(this)->actor_wait_state > 0) static_cast<GetAllExcludedServersActor*>(this)->actor_wait_state = 0;
		static_cast<GetAllExcludedServersActor*>(this)->ActorCallback< GetAllExcludedServersActor, 1, std::vector<AddressExclusion> >::remove();

	}
	void a_callback_fire(ActorCallback< GetAllExcludedServersActor, 1, std::vector<AddressExclusion> >*,std::vector<AddressExclusion> const& value) 
	{
		fdb_probe_actor_enter("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetAllExcludedServersActor, 1, std::vector<AddressExclusion> >*,std::vector<AddressExclusion> && value) 
	{
		fdb_probe_actor_enter("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetAllExcludedServersActor, 1, std::vector<AddressExclusion> >*,Error err) 
	{
		fdb_probe_actor_enter("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1693 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 1694 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::vector<AddressExclusion> exclusions;
															#line 9942 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via getAllExcludedServers()
															#line 1693 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetAllExcludedServersActor final : public Actor<std::vector<AddressExclusion>>, public ActorCallback< GetAllExcludedServersActor, 0, std::vector<AddressExclusion> >, public ActorCallback< GetAllExcludedServersActor, 1, std::vector<AddressExclusion> >, public FastAllocated<GetAllExcludedServersActor>, public GetAllExcludedServersActorState<GetAllExcludedServersActor> {
															#line 9947 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<GetAllExcludedServersActor>::operator new;
	using FastAllocated<GetAllExcludedServersActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<AddressExclusion>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetAllExcludedServersActor, 0, std::vector<AddressExclusion> >;
friend struct ActorCallback< GetAllExcludedServersActor, 1, std::vector<AddressExclusion> >;
															#line 1693 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetAllExcludedServersActor(Transaction* const& tr) 
															#line 9959 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<std::vector<AddressExclusion>>(),
		   GetAllExcludedServersActorState<GetAllExcludedServersActor>(tr)
	{
		fdb_probe_actor_enter("getAllExcludedServers", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getAllExcludedServers");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getAllExcludedServers", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetAllExcludedServersActor, 0, std::vector<AddressExclusion> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetAllExcludedServersActor, 1, std::vector<AddressExclusion> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1693 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<std::vector<AddressExclusion>> getAllExcludedServers( Transaction* const& tr ) {
															#line 1693 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<std::vector<AddressExclusion>>(new GetAllExcludedServersActor(tr));
															#line 9988 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1702 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 9993 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via getAllExcludedServers()
															#line 1703 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class GetAllExcludedServersActor1>
															#line 1703 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetAllExcludedServersActor1State {
															#line 10000 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1703 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetAllExcludedServersActor1State(Database const& cx) 
															#line 1703 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1703 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 1704 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 10009 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("getAllExcludedServers", reinterpret_cast<unsigned long>(this));

	}
	~GetAllExcludedServersActor1State() 
	{
		fdb_probe_actor_destroy("getAllExcludedServers", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1705 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 10024 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~GetAllExcludedServersActor1State();
		static_cast<GetAllExcludedServersActor1*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1707 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 1708 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1709 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1710 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<std::vector<AddressExclusion>> __when_expr_0 = getAllExcludedServers(&tr);
															#line 1710 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetAllExcludedServersActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 10063 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetAllExcludedServersActor1*>(this)->actor_wait_state = 1;
															#line 1710 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetAllExcludedServersActor1, 0, std::vector<AddressExclusion> >*>(static_cast<GetAllExcludedServersActor1*>(this)));
															#line 10068 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1713 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.onError(e);
															#line 1713 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetAllExcludedServersActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 10092 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetAllExcludedServersActor1*>(this)->actor_wait_state = 2;
															#line 1713 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetAllExcludedServersActor1, 1, Void >*>(static_cast<GetAllExcludedServersActor1*>(this)));
															#line 10097 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::vector<AddressExclusion> const& exclusions,int loopDepth) 
	{
															#line 1711 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetAllExcludedServersActor1*>(this)->SAV<std::vector<AddressExclusion>>::futures) { (void)(exclusions); this->~GetAllExcludedServersActor1State(); static_cast<GetAllExcludedServersActor1*>(this)->destroy(); return 0; }
															#line 10112 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetAllExcludedServersActor1*>(this)->SAV< std::vector<AddressExclusion> >::value()) std::vector<AddressExclusion>(exclusions);
		this->~GetAllExcludedServersActor1State();
		static_cast<GetAllExcludedServersActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::vector<AddressExclusion> && exclusions,int loopDepth) 
	{
															#line 1711 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetAllExcludedServersActor1*>(this)->SAV<std::vector<AddressExclusion>>::futures) { (void)(exclusions); this->~GetAllExcludedServersActor1State(); static_cast<GetAllExcludedServersActor1*>(this)->destroy(); return 0; }
															#line 10124 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetAllExcludedServersActor1*>(this)->SAV< std::vector<AddressExclusion> >::value()) std::vector<AddressExclusion>(exclusions);
		this->~GetAllExcludedServersActor1State();
		static_cast<GetAllExcludedServersActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<AddressExclusion> const& exclusions,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(exclusions, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<AddressExclusion> && exclusions,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(exclusions), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetAllExcludedServersActor1*>(this)->actor_wait_state > 0) static_cast<GetAllExcludedServersActor1*>(this)->actor_wait_state = 0;
		static_cast<GetAllExcludedServersActor1*>(this)->ActorCallback< GetAllExcludedServersActor1, 0, std::vector<AddressExclusion> >::remove();

	}
	void a_callback_fire(ActorCallback< GetAllExcludedServersActor1, 0, std::vector<AddressExclusion> >*,std::vector<AddressExclusion> const& value) 
	{
		fdb_probe_actor_enter("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetAllExcludedServersActor1, 0, std::vector<AddressExclusion> >*,std::vector<AddressExclusion> && value) 
	{
		fdb_probe_actor_enter("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetAllExcludedServersActor1, 0, std::vector<AddressExclusion> >*,Error err) 
	{
		fdb_probe_actor_enter("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<GetAllExcludedServersActor1*>(this)->actor_wait_state > 0) static_cast<GetAllExcludedServersActor1*>(this)->actor_wait_state = 0;
		static_cast<GetAllExcludedServersActor1*>(this)->ActorCallback< GetAllExcludedServersActor1, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetAllExcludedServersActor1, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetAllExcludedServersActor1, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetAllExcludedServersActor1, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedServers", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1703 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 1704 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 10274 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via getAllExcludedServers()
															#line 1703 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetAllExcludedServersActor1 final : public Actor<std::vector<AddressExclusion>>, public ActorCallback< GetAllExcludedServersActor1, 0, std::vector<AddressExclusion> >, public ActorCallback< GetAllExcludedServersActor1, 1, Void >, public FastAllocated<GetAllExcludedServersActor1>, public GetAllExcludedServersActor1State<GetAllExcludedServersActor1> {
															#line 10279 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<GetAllExcludedServersActor1>::operator new;
	using FastAllocated<GetAllExcludedServersActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<AddressExclusion>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetAllExcludedServersActor1, 0, std::vector<AddressExclusion> >;
friend struct ActorCallback< GetAllExcludedServersActor1, 1, Void >;
															#line 1703 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetAllExcludedServersActor1(Database const& cx) 
															#line 10291 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<std::vector<AddressExclusion>>(),
		   GetAllExcludedServersActor1State<GetAllExcludedServersActor1>(cx)
	{
		fdb_probe_actor_enter("getAllExcludedServers", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getAllExcludedServers");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getAllExcludedServers", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetAllExcludedServersActor1, 0, std::vector<AddressExclusion> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetAllExcludedServersActor1, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1703 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<std::vector<AddressExclusion>> getAllExcludedServers( Database const& cx ) {
															#line 1703 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<std::vector<AddressExclusion>>(new GetAllExcludedServersActor1(cx));
															#line 10320 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1717 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 10325 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via getExcludedLocalityList()
															#line 1718 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class GetExcludedLocalityListActor>
															#line 1718 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetExcludedLocalityListActorState {
															#line 10332 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1718 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetExcludedLocalityListActorState(Transaction* const& tr) 
															#line 1718 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1718 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr)
															#line 10339 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("getExcludedLocalityList", reinterpret_cast<unsigned long>(this));

	}
	~GetExcludedLocalityListActorState() 
	{
		fdb_probe_actor_destroy("getExcludedLocalityList", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1719 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(excludedLocalityKeys, CLIENT_KNOBS->TOO_MANY);
															#line 1719 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetExcludedLocalityListActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10356 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetExcludedLocalityListActor*>(this)->actor_wait_state = 1;
															#line 1719 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetExcludedLocalityListActor, 0, RangeResult >*>(static_cast<GetExcludedLocalityListActor*>(this)));
															#line 10361 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~GetExcludedLocalityListActorState();
		static_cast<GetExcludedLocalityListActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1720 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(!r.more && r.size() < CLIENT_KNOBS->TOO_MANY);
															#line 1722 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::vector<std::string> excludedLocalities;
															#line 1723 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( const auto& i : r ) {
															#line 1724 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto a = decodeExcludedLocalityKey(i.key);
															#line 1725 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			excludedLocalities.push_back(a);
															#line 10392 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1727 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		uniquify(excludedLocalities);
															#line 1728 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetExcludedLocalityListActor*>(this)->SAV<std::vector<std::string>>::futures) { (void)(excludedLocalities); this->~GetExcludedLocalityListActorState(); static_cast<GetExcludedLocalityListActor*>(this)->destroy(); return 0; }
															#line 10398 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetExcludedLocalityListActor*>(this)->SAV< std::vector<std::string> >::value()) std::vector<std::string>(excludedLocalities);
		this->~GetExcludedLocalityListActorState();
		static_cast<GetExcludedLocalityListActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& __r,int loopDepth) 
	{
															#line 1719 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		r = __r;
															#line 10410 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && __r,int loopDepth) 
	{
		r = std::move(__r);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetExcludedLocalityListActor*>(this)->actor_wait_state > 0) static_cast<GetExcludedLocalityListActor*>(this)->actor_wait_state = 0;
		static_cast<GetExcludedLocalityListActor*>(this)->ActorCallback< GetExcludedLocalityListActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetExcludedLocalityListActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getExcludedLocalityList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getExcludedLocalityList", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetExcludedLocalityListActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getExcludedLocalityList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getExcludedLocalityList", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetExcludedLocalityListActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getExcludedLocalityList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getExcludedLocalityList", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1718 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 1719 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	RangeResult r;
															#line 10477 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via getExcludedLocalityList()
															#line 1718 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetExcludedLocalityListActor final : public Actor<std::vector<std::string>>, public ActorCallback< GetExcludedLocalityListActor, 0, RangeResult >, public FastAllocated<GetExcludedLocalityListActor>, public GetExcludedLocalityListActorState<GetExcludedLocalityListActor> {
															#line 10482 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<GetExcludedLocalityListActor>::operator new;
	using FastAllocated<GetExcludedLocalityListActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetExcludedLocalityListActor, 0, RangeResult >;
															#line 1718 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetExcludedLocalityListActor(Transaction* const& tr) 
															#line 10493 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<std::vector<std::string>>(),
		   GetExcludedLocalityListActorState<GetExcludedLocalityListActor>(tr)
	{
		fdb_probe_actor_enter("getExcludedLocalityList", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getExcludedLocalityList");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getExcludedLocalityList", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetExcludedLocalityListActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1718 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<std::vector<std::string>> getExcludedLocalityList( Transaction* const& tr ) {
															#line 1718 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<std::vector<std::string>>(new GetExcludedLocalityListActor(tr));
															#line 10521 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1730 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 10526 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via getExcludedFailedLocalityList()
															#line 1731 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class GetExcludedFailedLocalityListActor>
															#line 1731 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetExcludedFailedLocalityListActorState {
															#line 10533 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1731 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetExcludedFailedLocalityListActorState(Transaction* const& tr) 
															#line 1731 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1731 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr)
															#line 10540 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("getExcludedFailedLocalityList", reinterpret_cast<unsigned long>(this));

	}
	~GetExcludedFailedLocalityListActorState() 
	{
		fdb_probe_actor_destroy("getExcludedFailedLocalityList", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1732 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(failedLocalityKeys, CLIENT_KNOBS->TOO_MANY);
															#line 1732 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetExcludedFailedLocalityListActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10557 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetExcludedFailedLocalityListActor*>(this)->actor_wait_state = 1;
															#line 1732 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetExcludedFailedLocalityListActor, 0, RangeResult >*>(static_cast<GetExcludedFailedLocalityListActor*>(this)));
															#line 10562 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~GetExcludedFailedLocalityListActorState();
		static_cast<GetExcludedFailedLocalityListActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1733 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(!r.more && r.size() < CLIENT_KNOBS->TOO_MANY);
															#line 1735 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::vector<std::string> excludedLocalities;
															#line 1736 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( const auto& i : r ) {
															#line 1737 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto a = decodeFailedLocalityKey(i.key);
															#line 1738 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			excludedLocalities.push_back(a);
															#line 10593 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1740 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		uniquify(excludedLocalities);
															#line 1741 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetExcludedFailedLocalityListActor*>(this)->SAV<std::vector<std::string>>::futures) { (void)(excludedLocalities); this->~GetExcludedFailedLocalityListActorState(); static_cast<GetExcludedFailedLocalityListActor*>(this)->destroy(); return 0; }
															#line 10599 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetExcludedFailedLocalityListActor*>(this)->SAV< std::vector<std::string> >::value()) std::vector<std::string>(excludedLocalities);
		this->~GetExcludedFailedLocalityListActorState();
		static_cast<GetExcludedFailedLocalityListActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& __r,int loopDepth) 
	{
															#line 1732 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		r = __r;
															#line 10611 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && __r,int loopDepth) 
	{
		r = std::move(__r);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetExcludedFailedLocalityListActor*>(this)->actor_wait_state > 0) static_cast<GetExcludedFailedLocalityListActor*>(this)->actor_wait_state = 0;
		static_cast<GetExcludedFailedLocalityListActor*>(this)->ActorCallback< GetExcludedFailedLocalityListActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetExcludedFailedLocalityListActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getExcludedFailedLocalityList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getExcludedFailedLocalityList", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetExcludedFailedLocalityListActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getExcludedFailedLocalityList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getExcludedFailedLocalityList", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetExcludedFailedLocalityListActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getExcludedFailedLocalityList", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getExcludedFailedLocalityList", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1731 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 1732 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	RangeResult r;
															#line 10678 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via getExcludedFailedLocalityList()
															#line 1731 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetExcludedFailedLocalityListActor final : public Actor<std::vector<std::string>>, public ActorCallback< GetExcludedFailedLocalityListActor, 0, RangeResult >, public FastAllocated<GetExcludedFailedLocalityListActor>, public GetExcludedFailedLocalityListActorState<GetExcludedFailedLocalityListActor> {
															#line 10683 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<GetExcludedFailedLocalityListActor>::operator new;
	using FastAllocated<GetExcludedFailedLocalityListActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetExcludedFailedLocalityListActor, 0, RangeResult >;
															#line 1731 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetExcludedFailedLocalityListActor(Transaction* const& tr) 
															#line 10694 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<std::vector<std::string>>(),
		   GetExcludedFailedLocalityListActorState<GetExcludedFailedLocalityListActor>(tr)
	{
		fdb_probe_actor_enter("getExcludedFailedLocalityList", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getExcludedFailedLocalityList");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getExcludedFailedLocalityList", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetExcludedFailedLocalityListActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1731 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<std::vector<std::string>> getExcludedFailedLocalityList( Transaction* const& tr ) {
															#line 1731 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<std::vector<std::string>>(new GetExcludedFailedLocalityListActor(tr));
															#line 10722 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1743 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 10727 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via getAllExcludedLocalities()
															#line 1744 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class GetAllExcludedLocalitiesActor>
															#line 1744 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetAllExcludedLocalitiesActorState {
															#line 10734 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1744 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetAllExcludedLocalitiesActorState(Transaction* const& tr) 
															#line 1744 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1744 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr),
															#line 1745 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   exclusions()
															#line 10743 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this));

	}
	~GetAllExcludedLocalitiesActorState() 
	{
		fdb_probe_actor_destroy("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1746 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<std::vector<std::string>> __when_expr_0 = getExcludedLocalityList(tr);
															#line 1746 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetAllExcludedLocalitiesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10760 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetAllExcludedLocalitiesActor*>(this)->actor_wait_state = 1;
															#line 1746 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetAllExcludedLocalitiesActor, 0, std::vector<std::string> >*>(static_cast<GetAllExcludedLocalitiesActor*>(this)));
															#line 10765 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~GetAllExcludedLocalitiesActorState();
		static_cast<GetAllExcludedLocalitiesActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<std::string> const& excludedLocalities,int loopDepth) 
	{
															#line 1747 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		exclusions.insert(exclusions.end(), excludedLocalities.begin(), excludedLocalities.end());
															#line 1748 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<std::vector<std::string>> __when_expr_1 = getExcludedFailedLocalityList(tr);
															#line 1748 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<GetAllExcludedLocalitiesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10792 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetAllExcludedLocalitiesActor*>(this)->actor_wait_state = 2;
															#line 1748 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetAllExcludedLocalitiesActor, 1, std::vector<std::string> >*>(static_cast<GetAllExcludedLocalitiesActor*>(this)));
															#line 10797 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<std::string> && excludedLocalities,int loopDepth) 
	{
															#line 1747 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		exclusions.insert(exclusions.end(), excludedLocalities.begin(), excludedLocalities.end());
															#line 1748 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<std::vector<std::string>> __when_expr_1 = getExcludedFailedLocalityList(tr);
															#line 1748 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<GetAllExcludedLocalitiesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10810 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetAllExcludedLocalitiesActor*>(this)->actor_wait_state = 2;
															#line 1748 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetAllExcludedLocalitiesActor, 1, std::vector<std::string> >*>(static_cast<GetAllExcludedLocalitiesActor*>(this)));
															#line 10815 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<std::string> const& excludedLocalities,int loopDepth) 
	{
		loopDepth = a_body1cont1(excludedLocalities, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<std::string> && excludedLocalities,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(excludedLocalities), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetAllExcludedLocalitiesActor*>(this)->actor_wait_state > 0) static_cast<GetAllExcludedLocalitiesActor*>(this)->actor_wait_state = 0;
		static_cast<GetAllExcludedLocalitiesActor*>(this)->ActorCallback< GetAllExcludedLocalitiesActor, 0, std::vector<std::string> >::remove();

	}
	void a_callback_fire(ActorCallback< GetAllExcludedLocalitiesActor, 0, std::vector<std::string> >*,std::vector<std::string> const& value) 
	{
		fdb_probe_actor_enter("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetAllExcludedLocalitiesActor, 0, std::vector<std::string> >*,std::vector<std::string> && value) 
	{
		fdb_probe_actor_enter("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetAllExcludedLocalitiesActor, 0, std::vector<std::string> >*,Error err) 
	{
		fdb_probe_actor_enter("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(std::vector<std::string> const& failedLocalities,int loopDepth) 
	{
															#line 1749 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		exclusions.insert(exclusions.end(), failedLocalities.begin(), failedLocalities.end());
															#line 1750 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		uniquify(exclusions);
															#line 1751 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetAllExcludedLocalitiesActor*>(this)->SAV<std::vector<std::string>>::futures) { (void)(exclusions); this->~GetAllExcludedLocalitiesActorState(); static_cast<GetAllExcludedLocalitiesActor*>(this)->destroy(); return 0; }
															#line 10891 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetAllExcludedLocalitiesActor*>(this)->SAV< std::vector<std::string> >::value()) std::vector<std::string>(std::move(exclusions)); // state_var_RVO
		this->~GetAllExcludedLocalitiesActorState();
		static_cast<GetAllExcludedLocalitiesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(std::vector<std::string> && failedLocalities,int loopDepth) 
	{
															#line 1749 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		exclusions.insert(exclusions.end(), failedLocalities.begin(), failedLocalities.end());
															#line 1750 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		uniquify(exclusions);
															#line 1751 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetAllExcludedLocalitiesActor*>(this)->SAV<std::vector<std::string>>::futures) { (void)(exclusions); this->~GetAllExcludedLocalitiesActorState(); static_cast<GetAllExcludedLocalitiesActor*>(this)->destroy(); return 0; }
															#line 10907 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetAllExcludedLocalitiesActor*>(this)->SAV< std::vector<std::string> >::value()) std::vector<std::string>(std::move(exclusions)); // state_var_RVO
		this->~GetAllExcludedLocalitiesActorState();
		static_cast<GetAllExcludedLocalitiesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(std::vector<std::string> const& failedLocalities,int loopDepth) 
	{
		loopDepth = a_body1cont2(failedLocalities, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(std::vector<std::string> && failedLocalities,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(failedLocalities), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetAllExcludedLocalitiesActor*>(this)->actor_wait_state > 0) static_cast<GetAllExcludedLocalitiesActor*>(this)->actor_wait_state = 0;
		static_cast<GetAllExcludedLocalitiesActor*>(this)->ActorCallback< GetAllExcludedLocalitiesActor, 1, std::vector<std::string> >::remove();

	}
	void a_callback_fire(ActorCallback< GetAllExcludedLocalitiesActor, 1, std::vector<std::string> >*,std::vector<std::string> const& value) 
	{
		fdb_probe_actor_enter("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetAllExcludedLocalitiesActor, 1, std::vector<std::string> >*,std::vector<std::string> && value) 
	{
		fdb_probe_actor_enter("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetAllExcludedLocalitiesActor, 1, std::vector<std::string> >*,Error err) 
	{
		fdb_probe_actor_enter("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1744 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 1745 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::vector<std::string> exclusions;
															#line 10982 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via getAllExcludedLocalities()
															#line 1744 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetAllExcludedLocalitiesActor final : public Actor<std::vector<std::string>>, public ActorCallback< GetAllExcludedLocalitiesActor, 0, std::vector<std::string> >, public ActorCallback< GetAllExcludedLocalitiesActor, 1, std::vector<std::string> >, public FastAllocated<GetAllExcludedLocalitiesActor>, public GetAllExcludedLocalitiesActorState<GetAllExcludedLocalitiesActor> {
															#line 10987 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<GetAllExcludedLocalitiesActor>::operator new;
	using FastAllocated<GetAllExcludedLocalitiesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetAllExcludedLocalitiesActor, 0, std::vector<std::string> >;
friend struct ActorCallback< GetAllExcludedLocalitiesActor, 1, std::vector<std::string> >;
															#line 1744 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetAllExcludedLocalitiesActor(Transaction* const& tr) 
															#line 10999 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<std::vector<std::string>>(),
		   GetAllExcludedLocalitiesActorState<GetAllExcludedLocalitiesActor>(tr)
	{
		fdb_probe_actor_enter("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getAllExcludedLocalities");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetAllExcludedLocalitiesActor, 0, std::vector<std::string> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetAllExcludedLocalitiesActor, 1, std::vector<std::string> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1744 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<std::vector<std::string>> getAllExcludedLocalities( Transaction* const& tr ) {
															#line 1744 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<std::vector<std::string>>(new GetAllExcludedLocalitiesActor(tr));
															#line 11028 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1753 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

// Get the list of excluded localities by reading the keys.
															#line 11034 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via getAllExcludedLocalities()
															#line 1755 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class GetAllExcludedLocalitiesActor1>
															#line 1755 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetAllExcludedLocalitiesActor1State {
															#line 11041 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1755 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetAllExcludedLocalitiesActor1State(Database const& cx) 
															#line 1755 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1755 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 1756 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 11050 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this));

	}
	~GetAllExcludedLocalitiesActor1State() 
	{
		fdb_probe_actor_destroy("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1757 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 11065 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~GetAllExcludedLocalitiesActor1State();
		static_cast<GetAllExcludedLocalitiesActor1*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1759 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 1760 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1761 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1762 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<std::vector<std::string>> __when_expr_0 = getAllExcludedLocalities(&tr);
															#line 1762 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetAllExcludedLocalitiesActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 11104 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetAllExcludedLocalitiesActor1*>(this)->actor_wait_state = 1;
															#line 1762 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetAllExcludedLocalitiesActor1, 0, std::vector<std::string> >*>(static_cast<GetAllExcludedLocalitiesActor1*>(this)));
															#line 11109 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1765 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.onError(e);
															#line 1765 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<GetAllExcludedLocalitiesActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 11133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetAllExcludedLocalitiesActor1*>(this)->actor_wait_state = 2;
															#line 1765 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetAllExcludedLocalitiesActor1, 1, Void >*>(static_cast<GetAllExcludedLocalitiesActor1*>(this)));
															#line 11138 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::vector<std::string> const& exclusions,int loopDepth) 
	{
															#line 1763 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetAllExcludedLocalitiesActor1*>(this)->SAV<std::vector<std::string>>::futures) { (void)(exclusions); this->~GetAllExcludedLocalitiesActor1State(); static_cast<GetAllExcludedLocalitiesActor1*>(this)->destroy(); return 0; }
															#line 11153 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetAllExcludedLocalitiesActor1*>(this)->SAV< std::vector<std::string> >::value()) std::vector<std::string>(exclusions);
		this->~GetAllExcludedLocalitiesActor1State();
		static_cast<GetAllExcludedLocalitiesActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::vector<std::string> && exclusions,int loopDepth) 
	{
															#line 1763 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<GetAllExcludedLocalitiesActor1*>(this)->SAV<std::vector<std::string>>::futures) { (void)(exclusions); this->~GetAllExcludedLocalitiesActor1State(); static_cast<GetAllExcludedLocalitiesActor1*>(this)->destroy(); return 0; }
															#line 11165 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<GetAllExcludedLocalitiesActor1*>(this)->SAV< std::vector<std::string> >::value()) std::vector<std::string>(exclusions);
		this->~GetAllExcludedLocalitiesActor1State();
		static_cast<GetAllExcludedLocalitiesActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<std::string> const& exclusions,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(exclusions, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<std::string> && exclusions,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(exclusions), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetAllExcludedLocalitiesActor1*>(this)->actor_wait_state > 0) static_cast<GetAllExcludedLocalitiesActor1*>(this)->actor_wait_state = 0;
		static_cast<GetAllExcludedLocalitiesActor1*>(this)->ActorCallback< GetAllExcludedLocalitiesActor1, 0, std::vector<std::string> >::remove();

	}
	void a_callback_fire(ActorCallback< GetAllExcludedLocalitiesActor1, 0, std::vector<std::string> >*,std::vector<std::string> const& value) 
	{
		fdb_probe_actor_enter("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetAllExcludedLocalitiesActor1, 0, std::vector<std::string> >*,std::vector<std::string> && value) 
	{
		fdb_probe_actor_enter("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetAllExcludedLocalitiesActor1, 0, std::vector<std::string> >*,Error err) 
	{
		fdb_probe_actor_enter("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<GetAllExcludedLocalitiesActor1*>(this)->actor_wait_state > 0) static_cast<GetAllExcludedLocalitiesActor1*>(this)->actor_wait_state = 0;
		static_cast<GetAllExcludedLocalitiesActor1*>(this)->ActorCallback< GetAllExcludedLocalitiesActor1, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetAllExcludedLocalitiesActor1, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetAllExcludedLocalitiesActor1, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetAllExcludedLocalitiesActor1, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1755 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 1756 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 11315 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via getAllExcludedLocalities()
															#line 1755 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class GetAllExcludedLocalitiesActor1 final : public Actor<std::vector<std::string>>, public ActorCallback< GetAllExcludedLocalitiesActor1, 0, std::vector<std::string> >, public ActorCallback< GetAllExcludedLocalitiesActor1, 1, Void >, public FastAllocated<GetAllExcludedLocalitiesActor1>, public GetAllExcludedLocalitiesActor1State<GetAllExcludedLocalitiesActor1> {
															#line 11320 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<GetAllExcludedLocalitiesActor1>::operator new;
	using FastAllocated<GetAllExcludedLocalitiesActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetAllExcludedLocalitiesActor1, 0, std::vector<std::string> >;
friend struct ActorCallback< GetAllExcludedLocalitiesActor1, 1, Void >;
															#line 1755 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	GetAllExcludedLocalitiesActor1(Database const& cx) 
															#line 11332 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<std::vector<std::string>>(),
		   GetAllExcludedLocalitiesActor1State<GetAllExcludedLocalitiesActor1>(cx)
	{
		fdb_probe_actor_enter("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getAllExcludedLocalities");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getAllExcludedLocalities", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetAllExcludedLocalitiesActor1, 0, std::vector<std::string> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetAllExcludedLocalitiesActor1, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1755 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<std::vector<std::string>> getAllExcludedLocalities( Database const& cx ) {
															#line 1755 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<std::vector<std::string>>(new GetAllExcludedLocalitiesActor1(cx));
															#line 11361 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1769 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

// Decodes the locality string to a pair of locality prefix and its value.
// The prefix could be dcid, processid, machineid, processid.
std::pair<std::string, std::string> decodeLocality(const std::string& locality) {
	StringRef localityRef((const uint8_t*)(locality.c_str()), locality.size());

	std::string localityKeyValue = localityRef.removePrefix(LocalityData::ExcludeLocalityPrefix).toString();
	int split = localityKeyValue.find(':');
	if (split != std::string::npos) {
		return std::make_pair(localityKeyValue.substr(0, split), localityKeyValue.substr(split + 1));
	}

	return std::make_pair("", "");
}

// Returns the list of IPAddresses of the workers that match the given locality.
// Example: locality="dcid:primary" returns all the ip addresses of the workers in the primary dc.
std::set<AddressExclusion> getAddressesByLocality(const std::vector<ProcessData>& workers,
                                                  const std::string& locality) {
	std::pair<std::string, std::string> localityKeyValue = decodeLocality(locality);

	std::set<AddressExclusion> localityAddresses;
	for (int i = 0; i < workers.size(); i++) {
		if (workers[i].locality.isPresent(localityKeyValue.first) &&
		    workers[i].locality.get(localityKeyValue.first) == localityKeyValue.second) {
			localityAddresses.insert(AddressExclusion(workers[i].address.ip, workers[i].address.port));
		}
	}

	return localityAddresses;
}

															#line 11397 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via printHealthyZone()
															#line 1801 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class PrintHealthyZoneActor>
															#line 1801 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class PrintHealthyZoneActorState {
															#line 11404 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1801 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	PrintHealthyZoneActorState(Database const& cx) 
															#line 1801 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1801 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 1802 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 11413 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("printHealthyZone", reinterpret_cast<unsigned long>(this));

	}
	~PrintHealthyZoneActorState() 
	{
		fdb_probe_actor_destroy("printHealthyZone", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1803 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 11428 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~PrintHealthyZoneActorState();
		static_cast<PrintHealthyZoneActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1805 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::READ_LOCK_AWARE);
															#line 1806 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 1807 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(healthyZoneKey);
															#line 1807 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<PrintHealthyZoneActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 11465 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<PrintHealthyZoneActor*>(this)->actor_wait_state = 1;
															#line 1807 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< PrintHealthyZoneActor, 0, Optional<Value> >*>(static_cast<PrintHealthyZoneActor*>(this)));
															#line 11470 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1821 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.onError(e);
															#line 1821 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<PrintHealthyZoneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 11494 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<PrintHealthyZoneActor*>(this)->actor_wait_state = 2;
															#line 1821 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PrintHealthyZoneActor, 1, Void >*>(static_cast<PrintHealthyZoneActor*>(this)));
															#line 11499 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& val,int loopDepth) 
	{
															#line 1808 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present() && decodeHealthyZoneValue(val.get()).first == ignoreSSFailuresZoneString)
															#line 11514 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1809 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			printf("Data distribution has been disabled for all storage server failures in this cluster and thus " "maintenance mode is not active.\n");
															#line 11518 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
		else
		{
															#line 1811 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!val.present() || decodeHealthyZoneValue(val.get()).second <= tr.getReadVersion().get())
															#line 11524 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1812 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				printf("No ongoing maintenance.\n");
															#line 11528 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
			else
			{
															#line 1814 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				auto healthyZone = decodeHealthyZoneValue(val.get());
															#line 1815 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				fmt::print("Maintenance for zone {0} will continue for {1} seconds.\n", healthyZone.first.toString(), (healthyZone.second - tr.getReadVersion().get()) / CLIENT_KNOBS->CORE_VERSIONSPERSECOND);
															#line 11536 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1819 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<PrintHealthyZoneActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~PrintHealthyZoneActorState(); static_cast<PrintHealthyZoneActor*>(this)->destroy(); return 0; }
															#line 11541 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<PrintHealthyZoneActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~PrintHealthyZoneActorState();
		static_cast<PrintHealthyZoneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && val,int loopDepth) 
	{
															#line 1808 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present() && decodeHealthyZoneValue(val.get()).first == ignoreSSFailuresZoneString)
															#line 11553 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1809 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			printf("Data distribution has been disabled for all storage server failures in this cluster and thus " "maintenance mode is not active.\n");
															#line 11557 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
		else
		{
															#line 1811 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!val.present() || decodeHealthyZoneValue(val.get()).second <= tr.getReadVersion().get())
															#line 11563 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1812 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				printf("No ongoing maintenance.\n");
															#line 11567 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
			else
			{
															#line 1814 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				auto healthyZone = decodeHealthyZoneValue(val.get());
															#line 1815 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				fmt::print("Maintenance for zone {0} will continue for {1} seconds.\n", healthyZone.first.toString(), (healthyZone.second - tr.getReadVersion().get()) / CLIENT_KNOBS->CORE_VERSIONSPERSECOND);
															#line 11575 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1819 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<PrintHealthyZoneActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~PrintHealthyZoneActorState(); static_cast<PrintHealthyZoneActor*>(this)->destroy(); return 0; }
															#line 11580 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<PrintHealthyZoneActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~PrintHealthyZoneActorState();
		static_cast<PrintHealthyZoneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& val,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(val, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && val,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<PrintHealthyZoneActor*>(this)->actor_wait_state > 0) static_cast<PrintHealthyZoneActor*>(this)->actor_wait_state = 0;
		static_cast<PrintHealthyZoneActor*>(this)->ActorCallback< PrintHealthyZoneActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< PrintHealthyZoneActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("printHealthyZone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("printHealthyZone", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< PrintHealthyZoneActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("printHealthyZone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("printHealthyZone", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< PrintHealthyZoneActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("printHealthyZone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("printHealthyZone", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<PrintHealthyZoneActor*>(this)->actor_wait_state > 0) static_cast<PrintHealthyZoneActor*>(this)->actor_wait_state = 0;
		static_cast<PrintHealthyZoneActor*>(this)->ActorCallback< PrintHealthyZoneActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PrintHealthyZoneActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("printHealthyZone", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("printHealthyZone", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< PrintHealthyZoneActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("printHealthyZone", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("printHealthyZone", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< PrintHealthyZoneActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("printHealthyZone", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("printHealthyZone", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1801 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 1802 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 11730 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via printHealthyZone()
															#line 1801 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class PrintHealthyZoneActor final : public Actor<Void>, public ActorCallback< PrintHealthyZoneActor, 0, Optional<Value> >, public ActorCallback< PrintHealthyZoneActor, 1, Void >, public FastAllocated<PrintHealthyZoneActor>, public PrintHealthyZoneActorState<PrintHealthyZoneActor> {
															#line 11735 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<PrintHealthyZoneActor>::operator new;
	using FastAllocated<PrintHealthyZoneActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< PrintHealthyZoneActor, 0, Optional<Value> >;
friend struct ActorCallback< PrintHealthyZoneActor, 1, Void >;
															#line 1801 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	PrintHealthyZoneActor(Database const& cx) 
															#line 11747 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   PrintHealthyZoneActorState<PrintHealthyZoneActor>(cx)
	{
		fdb_probe_actor_enter("printHealthyZone", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("printHealthyZone");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("printHealthyZone", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< PrintHealthyZoneActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< PrintHealthyZoneActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1801 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> printHealthyZone( Database const& cx ) {
															#line 1801 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new PrintHealthyZoneActor(cx));
															#line 11776 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1825 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 11781 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via clearHealthyZone()
															#line 1826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class ClearHealthyZoneActor>
															#line 1826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ClearHealthyZoneActorState {
															#line 11788 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ClearHealthyZoneActorState(Database const& cx,bool const& printWarning,bool const& clearSSFailureZoneString) 
															#line 1826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 1826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   printWarning(printWarning),
															#line 1826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   clearSSFailureZoneString(clearSSFailureZoneString),
															#line 1827 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 11801 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("clearHealthyZone", reinterpret_cast<unsigned long>(this));

	}
	~ClearHealthyZoneActorState() 
	{
		fdb_probe_actor_destroy("clearHealthyZone", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1828 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("ClearHealthyZone").detail("ClearSSFailureZoneString", clearSSFailureZoneString);
															#line 1829 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 11818 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~ClearHealthyZoneActorState();
		static_cast<ClearHealthyZoneActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1831 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1832 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1833 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1834 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(healthyZoneKey);
															#line 1834 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ClearHealthyZoneActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 11857 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ClearHealthyZoneActor*>(this)->actor_wait_state = 1;
															#line 1834 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ClearHealthyZoneActor, 0, Optional<Value> >*>(static_cast<ClearHealthyZoneActor*>(this)));
															#line 11862 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1848 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_2 = tr.onError(e);
															#line 1848 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<ClearHealthyZoneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 11886 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<ClearHealthyZoneActor*>(this)->actor_wait_state = 3;
															#line 1848 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ClearHealthyZoneActor, 2, Void >*>(static_cast<ClearHealthyZoneActor*>(this)));
															#line 11891 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& val,int loopDepth) 
	{
															#line 1835 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!clearSSFailureZoneString && val.present() && decodeHealthyZoneValue(val.get()).first == ignoreSSFailuresZoneString)
															#line 11906 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1837 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (printWarning)
															#line 11910 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1838 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				printf("ERROR: Maintenance mode cannot be used while data distribution is disabled for storage " "server failures. Use 'datadistribution on' to reenable data distribution.\n");
															#line 11914 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 1841 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ClearHealthyZoneActor*>(this)->SAV<bool>::futures) { (void)(false); this->~ClearHealthyZoneActorState(); static_cast<ClearHealthyZoneActor*>(this)->destroy(); return 0; }
															#line 11918 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ClearHealthyZoneActor*>(this)->SAV< bool >::value()) bool(false);
			this->~ClearHealthyZoneActorState();
			static_cast<ClearHealthyZoneActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1844 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.clear(healthyZoneKey);
															#line 1845 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 1845 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<ClearHealthyZoneActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 11930 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ClearHealthyZoneActor*>(this)->actor_wait_state = 2;
															#line 1845 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ClearHealthyZoneActor, 1, Void >*>(static_cast<ClearHealthyZoneActor*>(this)));
															#line 11935 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && val,int loopDepth) 
	{
															#line 1835 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!clearSSFailureZoneString && val.present() && decodeHealthyZoneValue(val.get()).first == ignoreSSFailuresZoneString)
															#line 11944 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1837 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (printWarning)
															#line 11948 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1838 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				printf("ERROR: Maintenance mode cannot be used while data distribution is disabled for storage " "server failures. Use 'datadistribution on' to reenable data distribution.\n");
															#line 11952 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 1841 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<ClearHealthyZoneActor*>(this)->SAV<bool>::futures) { (void)(false); this->~ClearHealthyZoneActorState(); static_cast<ClearHealthyZoneActor*>(this)->destroy(); return 0; }
															#line 11956 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<ClearHealthyZoneActor*>(this)->SAV< bool >::value()) bool(false);
			this->~ClearHealthyZoneActorState();
			static_cast<ClearHealthyZoneActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1844 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.clear(healthyZoneKey);
															#line 1845 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 1845 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<ClearHealthyZoneActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 11968 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ClearHealthyZoneActor*>(this)->actor_wait_state = 2;
															#line 1845 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ClearHealthyZoneActor, 1, Void >*>(static_cast<ClearHealthyZoneActor*>(this)));
															#line 11973 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& val,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(val, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && val,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ClearHealthyZoneActor*>(this)->actor_wait_state > 0) static_cast<ClearHealthyZoneActor*>(this)->actor_wait_state = 0;
		static_cast<ClearHealthyZoneActor*>(this)->ActorCallback< ClearHealthyZoneActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< ClearHealthyZoneActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("clearHealthyZone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearHealthyZone", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ClearHealthyZoneActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("clearHealthyZone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearHealthyZone", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ClearHealthyZoneActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("clearHealthyZone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearHealthyZone", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 1846 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ClearHealthyZoneActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ClearHealthyZoneActorState(); static_cast<ClearHealthyZoneActor*>(this)->destroy(); return 0; }
															#line 12045 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ClearHealthyZoneActor*>(this)->SAV< bool >::value()) bool(true);
		this->~ClearHealthyZoneActorState();
		static_cast<ClearHealthyZoneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 1846 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ClearHealthyZoneActor*>(this)->SAV<bool>::futures) { (void)(true); this->~ClearHealthyZoneActorState(); static_cast<ClearHealthyZoneActor*>(this)->destroy(); return 0; }
															#line 12057 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ClearHealthyZoneActor*>(this)->SAV< bool >::value()) bool(true);
		this->~ClearHealthyZoneActorState();
		static_cast<ClearHealthyZoneActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<ClearHealthyZoneActor*>(this)->actor_wait_state > 0) static_cast<ClearHealthyZoneActor*>(this)->actor_wait_state = 0;
		static_cast<ClearHealthyZoneActor*>(this)->ActorCallback< ClearHealthyZoneActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ClearHealthyZoneActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("clearHealthyZone", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearHealthyZone", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ClearHealthyZoneActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("clearHealthyZone", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearHealthyZone", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ClearHealthyZoneActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("clearHealthyZone", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearHealthyZone", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<ClearHealthyZoneActor*>(this)->actor_wait_state > 0) static_cast<ClearHealthyZoneActor*>(this)->actor_wait_state = 0;
		static_cast<ClearHealthyZoneActor*>(this)->ActorCallback< ClearHealthyZoneActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ClearHealthyZoneActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("clearHealthyZone", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearHealthyZone", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ClearHealthyZoneActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("clearHealthyZone", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearHealthyZone", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ClearHealthyZoneActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("clearHealthyZone", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearHealthyZone", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 1826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 1826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	bool printWarning;
															#line 1826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	bool clearSSFailureZoneString;
															#line 1827 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 12211 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via clearHealthyZone()
															#line 1826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ClearHealthyZoneActor final : public Actor<bool>, public ActorCallback< ClearHealthyZoneActor, 0, Optional<Value> >, public ActorCallback< ClearHealthyZoneActor, 1, Void >, public ActorCallback< ClearHealthyZoneActor, 2, Void >, public FastAllocated<ClearHealthyZoneActor>, public ClearHealthyZoneActorState<ClearHealthyZoneActor> {
															#line 12216 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<ClearHealthyZoneActor>::operator new;
	using FastAllocated<ClearHealthyZoneActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ClearHealthyZoneActor, 0, Optional<Value> >;
friend struct ActorCallback< ClearHealthyZoneActor, 1, Void >;
friend struct ActorCallback< ClearHealthyZoneActor, 2, Void >;
															#line 1826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ClearHealthyZoneActor(Database const& cx,bool const& printWarning,bool const& clearSSFailureZoneString) 
															#line 12229 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<bool>(),
		   ClearHealthyZoneActorState<ClearHealthyZoneActor>(cx, printWarning, clearSSFailureZoneString)
	{
		fdb_probe_actor_enter("clearHealthyZone", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("clearHealthyZone");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("clearHealthyZone", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ClearHealthyZoneActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ClearHealthyZoneActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ClearHealthyZoneActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<bool> clearHealthyZone( Database const& cx, bool const& printWarning, bool const& clearSSFailureZoneString ) {
															#line 1826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<bool>(new ClearHealthyZoneActor(cx, printWarning, clearSSFailureZoneString));
															#line 12259 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1852 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 12264 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via setHealthyZone()
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class SetHealthyZoneActor>
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class SetHealthyZoneActorState {
															#line 12271 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	SetHealthyZoneActorState(Database const& cx,StringRef const& zoneId,double const& seconds,bool const& printWarning) 
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   zoneId(zoneId),
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   seconds(seconds),
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   printWarning(printWarning),
															#line 1854 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 12286 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("setHealthyZone", reinterpret_cast<unsigned long>(this));

	}
	~SetHealthyZoneActorState() 
	{
		fdb_probe_actor_destroy("setHealthyZone", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1855 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("SetHealthyZone").detail("Zone", zoneId).detail("DurationSeconds", seconds);
															#line 1856 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 12303 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~SetHealthyZoneActorState();
		static_cast<SetHealthyZoneActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1858 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1859 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1860 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1861 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(healthyZoneKey);
															#line 1861 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<SetHealthyZoneActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 12342 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SetHealthyZoneActor*>(this)->actor_wait_state = 1;
															#line 1861 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SetHealthyZoneActor, 0, Optional<Value> >*>(static_cast<SetHealthyZoneActor*>(this)));
															#line 12347 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1875 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_3 = tr.onError(e);
															#line 1875 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<SetHealthyZoneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 12371 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<SetHealthyZoneActor*>(this)->actor_wait_state = 4;
															#line 1875 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< SetHealthyZoneActor, 3, Void >*>(static_cast<SetHealthyZoneActor*>(this)));
															#line 12376 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& val,int loopDepth) 
	{
															#line 1862 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present() && decodeHealthyZoneValue(val.get()).first == ignoreSSFailuresZoneString)
															#line 12391 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1863 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (printWarning)
															#line 12395 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1864 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				printf("ERROR: Maintenance mode cannot be used while data distribution is disabled for storage " "server failures. Use 'datadistribution on' to reenable data distribution.\n");
															#line 12399 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 1867 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<SetHealthyZoneActor*>(this)->SAV<bool>::futures) { (void)(false); this->~SetHealthyZoneActorState(); static_cast<SetHealthyZoneActor*>(this)->destroy(); return 0; }
															#line 12403 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<SetHealthyZoneActor*>(this)->SAV< bool >::value()) bool(false);
			this->~SetHealthyZoneActorState();
			static_cast<SetHealthyZoneActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1869 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Version> __when_expr_1 = tr.getReadVersion();
															#line 1869 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<SetHealthyZoneActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 12413 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<SetHealthyZoneActor*>(this)->actor_wait_state = 2;
															#line 1869 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SetHealthyZoneActor, 1, Version >*>(static_cast<SetHealthyZoneActor*>(this)));
															#line 12418 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && val,int loopDepth) 
	{
															#line 1862 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present() && decodeHealthyZoneValue(val.get()).first == ignoreSSFailuresZoneString)
															#line 12427 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1863 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (printWarning)
															#line 12431 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1864 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				printf("ERROR: Maintenance mode cannot be used while data distribution is disabled for storage " "server failures. Use 'datadistribution on' to reenable data distribution.\n");
															#line 12435 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 1867 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<SetHealthyZoneActor*>(this)->SAV<bool>::futures) { (void)(false); this->~SetHealthyZoneActorState(); static_cast<SetHealthyZoneActor*>(this)->destroy(); return 0; }
															#line 12439 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<SetHealthyZoneActor*>(this)->SAV< bool >::value()) bool(false);
			this->~SetHealthyZoneActorState();
			static_cast<SetHealthyZoneActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1869 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Version> __when_expr_1 = tr.getReadVersion();
															#line 1869 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<SetHealthyZoneActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 12449 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<SetHealthyZoneActor*>(this)->actor_wait_state = 2;
															#line 1869 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SetHealthyZoneActor, 1, Version >*>(static_cast<SetHealthyZoneActor*>(this)));
															#line 12454 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& val,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(val, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && val,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SetHealthyZoneActor*>(this)->actor_wait_state > 0) static_cast<SetHealthyZoneActor*>(this)->actor_wait_state = 0;
		static_cast<SetHealthyZoneActor*>(this)->ActorCallback< SetHealthyZoneActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< SetHealthyZoneActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("setHealthyZone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setHealthyZone", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SetHealthyZoneActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("setHealthyZone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setHealthyZone", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SetHealthyZoneActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("setHealthyZone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setHealthyZone", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Version const& readVersion,int loopDepth) 
	{
															#line 1870 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.set(healthyZoneKey, healthyZoneValue(zoneId, readVersion + (seconds * CLIENT_KNOBS->CORE_VERSIONSPERSECOND)));
															#line 1872 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 1872 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<SetHealthyZoneActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 12530 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<SetHealthyZoneActor*>(this)->actor_wait_state = 3;
															#line 1872 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SetHealthyZoneActor, 2, Void >*>(static_cast<SetHealthyZoneActor*>(this)));
															#line 12535 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Version && readVersion,int loopDepth) 
	{
															#line 1870 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.set(healthyZoneKey, healthyZoneValue(zoneId, readVersion + (seconds * CLIENT_KNOBS->CORE_VERSIONSPERSECOND)));
															#line 1872 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 1872 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<SetHealthyZoneActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 12548 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<SetHealthyZoneActor*>(this)->actor_wait_state = 3;
															#line 1872 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SetHealthyZoneActor, 2, Void >*>(static_cast<SetHealthyZoneActor*>(this)));
															#line 12553 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Version const& readVersion,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(readVersion, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Version && readVersion,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(readVersion), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<SetHealthyZoneActor*>(this)->actor_wait_state > 0) static_cast<SetHealthyZoneActor*>(this)->actor_wait_state = 0;
		static_cast<SetHealthyZoneActor*>(this)->ActorCallback< SetHealthyZoneActor, 1, Version >::remove();

	}
	void a_callback_fire(ActorCallback< SetHealthyZoneActor, 1, Version >*,Version const& value) 
	{
		fdb_probe_actor_enter("setHealthyZone", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setHealthyZone", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SetHealthyZoneActor, 1, Version >*,Version && value) 
	{
		fdb_probe_actor_enter("setHealthyZone", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setHealthyZone", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SetHealthyZoneActor, 1, Version >*,Error err) 
	{
		fdb_probe_actor_enter("setHealthyZone", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setHealthyZone", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
															#line 1873 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<SetHealthyZoneActor*>(this)->SAV<bool>::futures) { (void)(true); this->~SetHealthyZoneActorState(); static_cast<SetHealthyZoneActor*>(this)->destroy(); return 0; }
															#line 12625 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<SetHealthyZoneActor*>(this)->SAV< bool >::value()) bool(true);
		this->~SetHealthyZoneActorState();
		static_cast<SetHealthyZoneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 1873 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<SetHealthyZoneActor*>(this)->SAV<bool>::futures) { (void)(true); this->~SetHealthyZoneActorState(); static_cast<SetHealthyZoneActor*>(this)->destroy(); return 0; }
															#line 12637 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<SetHealthyZoneActor*>(this)->SAV< bool >::value()) bool(true);
		this->~SetHealthyZoneActorState();
		static_cast<SetHealthyZoneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<SetHealthyZoneActor*>(this)->actor_wait_state > 0) static_cast<SetHealthyZoneActor*>(this)->actor_wait_state = 0;
		static_cast<SetHealthyZoneActor*>(this)->ActorCallback< SetHealthyZoneActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SetHealthyZoneActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("setHealthyZone", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setHealthyZone", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< SetHealthyZoneActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("setHealthyZone", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setHealthyZone", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< SetHealthyZoneActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("setHealthyZone", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setHealthyZone", reinterpret_cast<unsigned long>(this), 2);

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
	void a_exitChoose4() 
	{
		if (static_cast<SetHealthyZoneActor*>(this)->actor_wait_state > 0) static_cast<SetHealthyZoneActor*>(this)->actor_wait_state = 0;
		static_cast<SetHealthyZoneActor*>(this)->ActorCallback< SetHealthyZoneActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SetHealthyZoneActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("setHealthyZone", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setHealthyZone", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< SetHealthyZoneActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("setHealthyZone", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setHealthyZone", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< SetHealthyZoneActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("setHealthyZone", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setHealthyZone", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	StringRef zoneId;
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	double seconds;
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	bool printWarning;
															#line 1854 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 12793 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via setHealthyZone()
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class SetHealthyZoneActor final : public Actor<bool>, public ActorCallback< SetHealthyZoneActor, 0, Optional<Value> >, public ActorCallback< SetHealthyZoneActor, 1, Version >, public ActorCallback< SetHealthyZoneActor, 2, Void >, public ActorCallback< SetHealthyZoneActor, 3, Void >, public FastAllocated<SetHealthyZoneActor>, public SetHealthyZoneActorState<SetHealthyZoneActor> {
															#line 12798 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<SetHealthyZoneActor>::operator new;
	using FastAllocated<SetHealthyZoneActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SetHealthyZoneActor, 0, Optional<Value> >;
friend struct ActorCallback< SetHealthyZoneActor, 1, Version >;
friend struct ActorCallback< SetHealthyZoneActor, 2, Void >;
friend struct ActorCallback< SetHealthyZoneActor, 3, Void >;
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	SetHealthyZoneActor(Database const& cx,StringRef const& zoneId,double const& seconds,bool const& printWarning) 
															#line 12812 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<bool>(),
		   SetHealthyZoneActorState<SetHealthyZoneActor>(cx, zoneId, seconds, printWarning)
	{
		fdb_probe_actor_enter("setHealthyZone", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("setHealthyZone");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("setHealthyZone", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SetHealthyZoneActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SetHealthyZoneActor, 1, Version >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< SetHealthyZoneActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< SetHealthyZoneActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<bool> setHealthyZone( Database const& cx, StringRef const& zoneId, double const& seconds, bool const& printWarning ) {
															#line 1853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<bool>(new SetHealthyZoneActor(cx, zoneId, seconds, printWarning));
															#line 12843 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1879 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 12848 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via setDDIgnoreRebalanceSwitch()
															#line 1880 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class SetDDIgnoreRebalanceSwitchActor>
															#line 1880 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class SetDDIgnoreRebalanceSwitchActorState {
															#line 12855 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1880 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	SetDDIgnoreRebalanceSwitchActorState(Database const& cx,bool const& ignoreRebalance) 
															#line 1880 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1880 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 1880 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   ignoreRebalance(ignoreRebalance),
															#line 1881 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 12866 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this));

	}
	~SetDDIgnoreRebalanceSwitchActorState() 
	{
		fdb_probe_actor_destroy("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1882 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 12881 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~SetDDIgnoreRebalanceSwitchActorState();
		static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1884 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1885 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1886 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (ignoreRebalance)
															#line 12916 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1887 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr.set(rebalanceDDIgnoreKey, LiteralStringRef("on"));
															#line 12920 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
			else
			{
															#line 1889 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr.clear(rebalanceDDIgnoreKey);
															#line 12926 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 1891 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_0 = tr.commit();
															#line 1891 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 12932 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->actor_wait_state = 1;
															#line 1891 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SetDDIgnoreRebalanceSwitchActor, 0, Void >*>(static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)));
															#line 12937 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1894 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.onError(e);
															#line 1894 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 12961 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->actor_wait_state = 2;
															#line 1894 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SetDDIgnoreRebalanceSwitchActor, 1, Void >*>(static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)));
															#line 12966 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1892 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SetDDIgnoreRebalanceSwitchActorState(); static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->destroy(); return 0; }
															#line 12981 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SetDDIgnoreRebalanceSwitchActorState();
		static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 1892 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SetDDIgnoreRebalanceSwitchActorState(); static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->destroy(); return 0; }
															#line 12993 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SetDDIgnoreRebalanceSwitchActorState();
		static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->actor_wait_state > 0) static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->actor_wait_state = 0;
		static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->ActorCallback< SetDDIgnoreRebalanceSwitchActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SetDDIgnoreRebalanceSwitchActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SetDDIgnoreRebalanceSwitchActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SetDDIgnoreRebalanceSwitchActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->actor_wait_state > 0) static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->actor_wait_state = 0;
		static_cast<SetDDIgnoreRebalanceSwitchActor*>(this)->ActorCallback< SetDDIgnoreRebalanceSwitchActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SetDDIgnoreRebalanceSwitchActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SetDDIgnoreRebalanceSwitchActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SetDDIgnoreRebalanceSwitchActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1880 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 1880 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	bool ignoreRebalance;
															#line 1881 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 13145 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via setDDIgnoreRebalanceSwitch()
															#line 1880 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class SetDDIgnoreRebalanceSwitchActor final : public Actor<Void>, public ActorCallback< SetDDIgnoreRebalanceSwitchActor, 0, Void >, public ActorCallback< SetDDIgnoreRebalanceSwitchActor, 1, Void >, public FastAllocated<SetDDIgnoreRebalanceSwitchActor>, public SetDDIgnoreRebalanceSwitchActorState<SetDDIgnoreRebalanceSwitchActor> {
															#line 13150 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<SetDDIgnoreRebalanceSwitchActor>::operator new;
	using FastAllocated<SetDDIgnoreRebalanceSwitchActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SetDDIgnoreRebalanceSwitchActor, 0, Void >;
friend struct ActorCallback< SetDDIgnoreRebalanceSwitchActor, 1, Void >;
															#line 1880 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	SetDDIgnoreRebalanceSwitchActor(Database const& cx,bool const& ignoreRebalance) 
															#line 13162 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   SetDDIgnoreRebalanceSwitchActorState<SetDDIgnoreRebalanceSwitchActor>(cx, ignoreRebalance)
	{
		fdb_probe_actor_enter("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("setDDIgnoreRebalanceSwitch");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("setDDIgnoreRebalanceSwitch", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SetDDIgnoreRebalanceSwitchActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SetDDIgnoreRebalanceSwitchActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1880 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> setDDIgnoreRebalanceSwitch( Database const& cx, bool const& ignoreRebalance ) {
															#line 1880 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new SetDDIgnoreRebalanceSwitchActor(cx, ignoreRebalance));
															#line 13191 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1898 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 13196 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via setDDMode()
															#line 1899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class SetDDModeActor>
															#line 1899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class SetDDModeActorState {
															#line 13203 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	SetDDModeActorState(Database const& cx,int const& mode) 
															#line 1899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 1899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   mode(mode),
															#line 1900 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx),
															#line 1901 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   oldMode(-1),
															#line 1902 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   wr(Unversioned())
															#line 13218 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("setDDMode", reinterpret_cast<unsigned long>(this));

	}
	~SetDDModeActorState() 
	{
		fdb_probe_actor_destroy("setDDMode", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1903 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			wr << mode;
															#line 1905 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 13235 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~SetDDModeActorState();
		static_cast<SetDDModeActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1907 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1908 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1909 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(dataDistributionModeKey);
															#line 1909 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<SetDDModeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 13272 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SetDDModeActor*>(this)->actor_wait_state = 1;
															#line 1909 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SetDDModeActor, 0, Optional<Value> >*>(static_cast<SetDDModeActor*>(this)));
															#line 13277 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 1938 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("SetDDModeRetrying").error(e);
															#line 1939 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_3 = tr.onError(e);
															#line 1939 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<SetDDModeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 13303 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<SetDDModeActor*>(this)->actor_wait_state = 4;
															#line 1939 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< SetDDModeActor, 3, Void >*>(static_cast<SetDDModeActor*>(this)));
															#line 13308 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& old,int loopDepth) 
	{
															#line 1910 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (oldMode < 0)
															#line 13323 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1911 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			oldMode = 1;
															#line 1912 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (old.present())
															#line 13329 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1913 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				BinaryReader rd(old.get(), Unversioned());
															#line 1914 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				rd >> oldMode;
															#line 13335 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1917 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		BinaryWriter wrMyOwner(Unversioned());
															#line 1918 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		wrMyOwner << dataDistributionModeLock;
															#line 1919 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.set(moveKeysLockOwnerKey, wrMyOwner.toValue());
															#line 1920 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		BinaryWriter wrLastWrite(Unversioned());
															#line 1921 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		wrLastWrite << deterministicRandom()->randomUniqueID();
															#line 1922 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.set(moveKeysLockWriteKey, wrLastWrite.toValue());
															#line 1924 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.set(dataDistributionModeKey, wr.toValue());
															#line 1925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (mode)
															#line 13354 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1927 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_1 = tr.get(healthyZoneKey);
															#line 1927 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<SetDDModeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 13360 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<SetDDModeActor*>(this)->actor_wait_state = 2;
															#line 1927 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SetDDModeActor, 1, Optional<Value> >*>(static_cast<SetDDModeActor*>(this)));
															#line 13365 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && old,int loopDepth) 
	{
															#line 1910 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (oldMode < 0)
															#line 13379 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1911 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			oldMode = 1;
															#line 1912 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (old.present())
															#line 13385 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1913 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				BinaryReader rd(old.get(), Unversioned());
															#line 1914 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				rd >> oldMode;
															#line 13391 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1917 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		BinaryWriter wrMyOwner(Unversioned());
															#line 1918 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		wrMyOwner << dataDistributionModeLock;
															#line 1919 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.set(moveKeysLockOwnerKey, wrMyOwner.toValue());
															#line 1920 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		BinaryWriter wrLastWrite(Unversioned());
															#line 1921 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		wrLastWrite << deterministicRandom()->randomUniqueID();
															#line 1922 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.set(moveKeysLockWriteKey, wrLastWrite.toValue());
															#line 1924 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.set(dataDistributionModeKey, wr.toValue());
															#line 1925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (mode)
															#line 13410 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1927 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_1 = tr.get(healthyZoneKey);
															#line 1927 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<SetDDModeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 13416 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<SetDDModeActor*>(this)->actor_wait_state = 2;
															#line 1927 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SetDDModeActor, 1, Optional<Value> >*>(static_cast<SetDDModeActor*>(this)));
															#line 13421 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& old,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(old, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && old,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(old), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SetDDModeActor*>(this)->actor_wait_state > 0) static_cast<SetDDModeActor*>(this)->actor_wait_state = 0;
		static_cast<SetDDModeActor*>(this)->ActorCallback< SetDDModeActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< SetDDModeActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("setDDMode", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDMode", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SetDDModeActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("setDDMode", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDMode", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SetDDModeActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("setDDMode", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDMode", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 1935 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 1935 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<SetDDModeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 13500 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<SetDDModeActor*>(this)->actor_wait_state = 3;
															#line 1935 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SetDDModeActor, 2, Void >*>(static_cast<SetDDModeActor*>(this)));
															#line 13505 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(Optional<Value> const& currentHealthyZoneValue,int loopDepth) 
	{
															#line 1928 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (currentHealthyZoneValue.present() && decodeHealthyZoneValue(currentHealthyZoneValue.get()).first == ignoreSSFailuresZoneString)
															#line 13514 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1931 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.clear(healthyZoneKey);
															#line 13518 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1933 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.clear(rebalanceDDIgnoreKey);
															#line 13522 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Optional<Value> && currentHealthyZoneValue,int loopDepth) 
	{
															#line 1928 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (currentHealthyZoneValue.present() && decodeHealthyZoneValue(currentHealthyZoneValue.get()).first == ignoreSSFailuresZoneString)
															#line 13531 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1931 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.clear(healthyZoneKey);
															#line 13535 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 1933 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.clear(rebalanceDDIgnoreKey);
															#line 13539 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Optional<Value> const& currentHealthyZoneValue,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(currentHealthyZoneValue, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Optional<Value> && currentHealthyZoneValue,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(currentHealthyZoneValue), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<SetDDModeActor*>(this)->actor_wait_state > 0) static_cast<SetDDModeActor*>(this)->actor_wait_state = 0;
		static_cast<SetDDModeActor*>(this)->ActorCallback< SetDDModeActor, 1, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< SetDDModeActor, 1, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("setDDMode", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDMode", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SetDDModeActor, 1, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("setDDMode", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDMode", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SetDDModeActor, 1, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("setDDMode", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDMode", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont9(Void const& _,int loopDepth) 
	{
															#line 1936 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<SetDDModeActor*>(this)->SAV<int>::futures) { (void)(oldMode); this->~SetDDModeActorState(); static_cast<SetDDModeActor*>(this)->destroy(); return 0; }
															#line 13611 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<SetDDModeActor*>(this)->SAV< int >::value()) int(std::move(oldMode)); // state_var_RVO
		this->~SetDDModeActorState();
		static_cast<SetDDModeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void && _,int loopDepth) 
	{
															#line 1936 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<SetDDModeActor*>(this)->SAV<int>::futures) { (void)(oldMode); this->~SetDDModeActorState(); static_cast<SetDDModeActor*>(this)->destroy(); return 0; }
															#line 13623 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<SetDDModeActor*>(this)->SAV< int >::value()) int(std::move(oldMode)); // state_var_RVO
		this->~SetDDModeActorState();
		static_cast<SetDDModeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<SetDDModeActor*>(this)->actor_wait_state > 0) static_cast<SetDDModeActor*>(this)->actor_wait_state = 0;
		static_cast<SetDDModeActor*>(this)->ActorCallback< SetDDModeActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SetDDModeActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("setDDMode", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDMode", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< SetDDModeActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("setDDMode", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDMode", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< SetDDModeActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("setDDMode", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDMode", reinterpret_cast<unsigned long>(this), 2);

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
	void a_exitChoose4() 
	{
		if (static_cast<SetDDModeActor*>(this)->actor_wait_state > 0) static_cast<SetDDModeActor*>(this)->actor_wait_state = 0;
		static_cast<SetDDModeActor*>(this)->ActorCallback< SetDDModeActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SetDDModeActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("setDDMode", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDMode", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< SetDDModeActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("setDDMode", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDMode", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< SetDDModeActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("setDDMode", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("setDDMode", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 1899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 1899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	int mode;
															#line 1900 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 1901 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	int oldMode;
															#line 1902 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	BinaryWriter wr;
															#line 13779 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via setDDMode()
															#line 1899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class SetDDModeActor final : public Actor<int>, public ActorCallback< SetDDModeActor, 0, Optional<Value> >, public ActorCallback< SetDDModeActor, 1, Optional<Value> >, public ActorCallback< SetDDModeActor, 2, Void >, public ActorCallback< SetDDModeActor, 3, Void >, public FastAllocated<SetDDModeActor>, public SetDDModeActorState<SetDDModeActor> {
															#line 13784 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<SetDDModeActor>::operator new;
	using FastAllocated<SetDDModeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SetDDModeActor, 0, Optional<Value> >;
friend struct ActorCallback< SetDDModeActor, 1, Optional<Value> >;
friend struct ActorCallback< SetDDModeActor, 2, Void >;
friend struct ActorCallback< SetDDModeActor, 3, Void >;
															#line 1899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	SetDDModeActor(Database const& cx,int const& mode) 
															#line 13798 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<int>(),
		   SetDDModeActorState<SetDDModeActor>(cx, mode)
	{
		fdb_probe_actor_enter("setDDMode", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("setDDMode");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("setDDMode", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SetDDModeActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SetDDModeActor, 1, Optional<Value> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< SetDDModeActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< SetDDModeActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<int> setDDMode( Database const& cx, int const& mode ) {
															#line 1899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<int>(new SetDDModeActor(cx, mode));
															#line 13829 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1943 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 13834 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via checkForExcludingServersTxActor()
															#line 1944 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class CheckForExcludingServersTxActorActor>
															#line 1944 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class CheckForExcludingServersTxActorActorState {
															#line 13841 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1944 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	CheckForExcludingServersTxActorActorState(ReadYourWritesTransaction* const& tr,std::set<AddressExclusion>* const& exclusions,std::set<NetworkAddress>* const& inProgressExclusion) 
															#line 1944 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1944 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr),
															#line 1944 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   exclusions(exclusions),
															#line 1944 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   inProgressExclusion(inProgressExclusion)
															#line 13852 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this));

	}
	~CheckForExcludingServersTxActorActorState() 
	{
		fdb_probe_actor_destroy("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1948 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ASSERT(inProgressExclusion->size() == 0);
															#line 1949 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!exclusions->size())
															#line 13869 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1950 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (!static_cast<CheckForExcludingServersTxActorActor*>(this)->SAV<bool>::futures) { (void)(true); this->~CheckForExcludingServersTxActorActorState(); static_cast<CheckForExcludingServersTxActorActor*>(this)->destroy(); return 0; }
															#line 13873 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				new (&static_cast<CheckForExcludingServersTxActorActor*>(this)->SAV< bool >::value()) bool(true);
				this->~CheckForExcludingServersTxActorActorState();
				static_cast<CheckForExcludingServersTxActorActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1952 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 1953 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1954 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1960 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(serverListKeys, CLIENT_KNOBS->TOO_MANY);
															#line 1960 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<CheckForExcludingServersTxActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13889 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheckForExcludingServersTxActorActor*>(this)->actor_wait_state = 1;
															#line 1960 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckForExcludingServersTxActorActor, 0, RangeResult >*>(static_cast<CheckForExcludingServersTxActorActor*>(this)));
															#line 13894 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~CheckForExcludingServersTxActorActorState();
		static_cast<CheckForExcludingServersTxActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& serverList,int loopDepth) 
	{
															#line 1961 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(!serverList.more && serverList.size() < CLIENT_KNOBS->TOO_MANY);
															#line 1963 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ok = true;
															#line 1964 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto& s : serverList ) {
															#line 1965 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto addresses = decodeServerListValue(s.value).getKeyValues.getEndpoint().addresses;
															#line 1966 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (addressExcluded(*exclusions, addresses.address))
															#line 13925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1967 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				ok = false;
															#line 1968 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				inProgressExclusion->insert(addresses.address);
															#line 13931 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 1970 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (addresses.secondaryAddress.present() && addressExcluded(*exclusions, addresses.secondaryAddress.get()))
															#line 13935 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1971 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				ok = false;
															#line 1972 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				inProgressExclusion->insert(addresses.secondaryAddress.get());
															#line 13941 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1976 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (ok)
															#line 13946 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1977 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Standalone<StringRef>>> __when_expr_1 = tr->get(logsKey);
															#line 1977 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<CheckForExcludingServersTxActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13952 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<CheckForExcludingServersTxActorActor*>(this)->actor_wait_state = 2;
															#line 1977 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckForExcludingServersTxActorActor, 1, Optional<Standalone<StringRef>> >*>(static_cast<CheckForExcludingServersTxActorActor*>(this)));
															#line 13957 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(RangeResult && serverList,int loopDepth) 
	{
															#line 1961 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(!serverList.more && serverList.size() < CLIENT_KNOBS->TOO_MANY);
															#line 1963 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ok = true;
															#line 1964 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto& s : serverList ) {
															#line 1965 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto addresses = decodeServerListValue(s.value).getKeyValues.getEndpoint().addresses;
															#line 1966 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (addressExcluded(*exclusions, addresses.address))
															#line 13979 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1967 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				ok = false;
															#line 1968 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				inProgressExclusion->insert(addresses.address);
															#line 13985 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 1970 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (addresses.secondaryAddress.present() && addressExcluded(*exclusions, addresses.secondaryAddress.get()))
															#line 13989 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1971 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				ok = false;
															#line 1972 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				inProgressExclusion->insert(addresses.secondaryAddress.get());
															#line 13995 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1976 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (ok)
															#line 14000 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 1977 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Standalone<StringRef>>> __when_expr_1 = tr->get(logsKey);
															#line 1977 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<CheckForExcludingServersTxActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 14006 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<CheckForExcludingServersTxActorActor*>(this)->actor_wait_state = 2;
															#line 1977 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckForExcludingServersTxActorActor, 1, Optional<Standalone<StringRef>> >*>(static_cast<CheckForExcludingServersTxActorActor*>(this)));
															#line 14011 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(RangeResult const& serverList,int loopDepth) 
	{
		loopDepth = a_body1cont1(serverList, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && serverList,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(serverList), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CheckForExcludingServersTxActorActor*>(this)->actor_wait_state > 0) static_cast<CheckForExcludingServersTxActorActor*>(this)->actor_wait_state = 0;
		static_cast<CheckForExcludingServersTxActorActor*>(this)->ActorCallback< CheckForExcludingServersTxActorActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< CheckForExcludingServersTxActorActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckForExcludingServersTxActorActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckForExcludingServersTxActorActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 1994 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<CheckForExcludingServersTxActorActor*>(this)->SAV<bool>::futures) { (void)(ok); this->~CheckForExcludingServersTxActorActorState(); static_cast<CheckForExcludingServersTxActorActor*>(this)->destroy(); return 0; }
															#line 14088 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<CheckForExcludingServersTxActorActor*>(this)->SAV< bool >::value()) bool(std::move(ok)); // state_var_RVO
		this->~CheckForExcludingServersTxActorActorState();
		static_cast<CheckForExcludingServersTxActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(Optional<Standalone<StringRef>> const& value,int loopDepth) 
	{
															#line 1978 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(value.present());
															#line 1979 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		auto logs = decodeLogsValue(value.get());
															#line 1980 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto const& log : logs.first ) {
															#line 1981 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (log.second == NetworkAddress() || addressExcluded(*exclusions, log.second))
															#line 14106 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1982 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				ok = false;
															#line 1983 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				inProgressExclusion->insert(log.second);
															#line 14112 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1986 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto const& log : logs.second ) {
															#line 1987 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (log.second == NetworkAddress() || addressExcluded(*exclusions, log.second))
															#line 14119 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1988 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				ok = false;
															#line 1989 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				inProgressExclusion->insert(log.second);
															#line 14125 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(Optional<Standalone<StringRef>> && value,int loopDepth) 
	{
															#line 1978 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(value.present());
															#line 1979 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		auto logs = decodeLogsValue(value.get());
															#line 1980 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto const& log : logs.first ) {
															#line 1981 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (log.second == NetworkAddress() || addressExcluded(*exclusions, log.second))
															#line 14142 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1982 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				ok = false;
															#line 1983 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				inProgressExclusion->insert(log.second);
															#line 14148 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 1986 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto const& log : logs.second ) {
															#line 1987 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (log.second == NetworkAddress() || addressExcluded(*exclusions, log.second))
															#line 14155 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 1988 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				ok = false;
															#line 1989 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				inProgressExclusion->insert(log.second);
															#line 14161 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Optional<Standalone<StringRef>> const& value,int loopDepth) 
	{
		loopDepth = a_body1cont7(value, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Optional<Standalone<StringRef>> && value,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(value), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CheckForExcludingServersTxActorActor*>(this)->actor_wait_state > 0) static_cast<CheckForExcludingServersTxActorActor*>(this)->actor_wait_state = 0;
		static_cast<CheckForExcludingServersTxActorActor*>(this)->ActorCallback< CheckForExcludingServersTxActorActor, 1, Optional<Standalone<StringRef>> >::remove();

	}
	void a_callback_fire(ActorCallback< CheckForExcludingServersTxActorActor, 1, Optional<Standalone<StringRef>> >*,Optional<Standalone<StringRef>> const& value) 
	{
		fdb_probe_actor_enter("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CheckForExcludingServersTxActorActor, 1, Optional<Standalone<StringRef>> >*,Optional<Standalone<StringRef>> && value) 
	{
		fdb_probe_actor_enter("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CheckForExcludingServersTxActorActor, 1, Optional<Standalone<StringRef>> >*,Error err) 
	{
		fdb_probe_actor_enter("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1944 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ReadYourWritesTransaction* tr;
															#line 1944 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::set<AddressExclusion>* exclusions;
															#line 1944 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::set<NetworkAddress>* inProgressExclusion;
															#line 1963 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	bool ok;
															#line 14239 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via checkForExcludingServersTxActor()
															#line 1944 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class CheckForExcludingServersTxActorActor final : public Actor<bool>, public ActorCallback< CheckForExcludingServersTxActorActor, 0, RangeResult >, public ActorCallback< CheckForExcludingServersTxActorActor, 1, Optional<Standalone<StringRef>> >, public FastAllocated<CheckForExcludingServersTxActorActor>, public CheckForExcludingServersTxActorActorState<CheckForExcludingServersTxActorActor> {
															#line 14244 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<CheckForExcludingServersTxActorActor>::operator new;
	using FastAllocated<CheckForExcludingServersTxActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckForExcludingServersTxActorActor, 0, RangeResult >;
friend struct ActorCallback< CheckForExcludingServersTxActorActor, 1, Optional<Standalone<StringRef>> >;
															#line 1944 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	CheckForExcludingServersTxActorActor(ReadYourWritesTransaction* const& tr,std::set<AddressExclusion>* const& exclusions,std::set<NetworkAddress>* const& inProgressExclusion) 
															#line 14256 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<bool>(),
		   CheckForExcludingServersTxActorActorState<CheckForExcludingServersTxActorActor>(tr, exclusions, inProgressExclusion)
	{
		fdb_probe_actor_enter("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkForExcludingServersTxActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkForExcludingServersTxActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckForExcludingServersTxActorActor, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CheckForExcludingServersTxActorActor, 1, Optional<Standalone<StringRef>> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1944 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<bool> checkForExcludingServersTxActor( ReadYourWritesTransaction* const& tr, std::set<AddressExclusion>* const& exclusions, std::set<NetworkAddress>* const& inProgressExclusion ) {
															#line 1944 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<bool>(new CheckForExcludingServersTxActorActor(tr, exclusions, inProgressExclusion));
															#line 14285 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 1996 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 14290 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via checkForExcludingServers()
															#line 1997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class CheckForExcludingServersActor>
															#line 1997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class CheckForExcludingServersActorState {
															#line 14297 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 1997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	CheckForExcludingServersActorState(Database const& cx,std::vector<AddressExclusion> const& excl,bool const& waitForAllExcluded) 
															#line 1997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 1997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 1997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   excl(excl),
															#line 1997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   waitForAllExcluded(waitForAllExcluded),
															#line 2000 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   exclusions(excl.begin(), excl.end()),
															#line 2001 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   inProgressExclusion()
															#line 14312 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("checkForExcludingServers", reinterpret_cast<unsigned long>(this));

	}
	~CheckForExcludingServersActorState() 
	{
		fdb_probe_actor_destroy("checkForExcludingServers", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2003 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 14327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~CheckForExcludingServersActorState();
		static_cast<CheckForExcludingServersActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 2019 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<CheckForExcludingServersActor*>(this)->SAV<std::set<NetworkAddress>>::futures) { (void)(inProgressExclusion); this->~CheckForExcludingServersActorState(); static_cast<CheckForExcludingServersActor*>(this)->destroy(); return 0; }
															#line 14350 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<CheckForExcludingServersActor*>(this)->SAV< std::set<NetworkAddress> >::value()) std::set<NetworkAddress>(std::move(inProgressExclusion)); // state_var_RVO
		this->~CheckForExcludingServersActorState();
		static_cast<CheckForExcludingServersActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 2004 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr = ReadYourWritesTransaction(cx);
															#line 2005 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		inProgressExclusion.clear();
															#line 14371 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		try {
															#line 2007 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<bool> __when_expr_0 = checkForExcludingServersTxActor(&tr, &exclusions, &inProgressExclusion);
															#line 2007 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<CheckForExcludingServersActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 14377 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheckForExcludingServersActor*>(this)->actor_wait_state = 1;
															#line 2007 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckForExcludingServersActor, 0, bool >*>(static_cast<CheckForExcludingServersActor*>(this)));
															#line 14382 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 2015 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent("CheckForExcludingServersError").error(e);
															#line 2016 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_2 = tr.onError(e);
															#line 2016 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<CheckForExcludingServersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 14421 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<CheckForExcludingServersActor*>(this)->actor_wait_state = 3;
															#line 2016 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CheckForExcludingServersActor, 2, Void >*>(static_cast<CheckForExcludingServersActor*>(this)));
															#line 14426 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(bool const& ok,int loopDepth) 
	{
															#line 2008 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (ok)
															#line 14441 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2009 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<CheckForExcludingServersActor*>(this)->SAV<std::set<NetworkAddress>>::futures) { (void)(inProgressExclusion); this->~CheckForExcludingServersActorState(); static_cast<CheckForExcludingServersActor*>(this)->destroy(); return 0; }
															#line 14445 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<CheckForExcludingServersActor*>(this)->SAV< std::set<NetworkAddress> >::value()) std::set<NetworkAddress>(std::move(inProgressExclusion)); // state_var_RVO
			this->~CheckForExcludingServersActorState();
			static_cast<CheckForExcludingServersActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 2010 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!waitForAllExcluded)
															#line 14453 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 2013 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delayJittered(1.0);
															#line 2013 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<CheckForExcludingServersActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 14461 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<CheckForExcludingServersActor*>(this)->actor_wait_state = 2;
															#line 2013 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckForExcludingServersActor, 1, Void >*>(static_cast<CheckForExcludingServersActor*>(this)));
															#line 14466 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(bool && ok,int loopDepth) 
	{
															#line 2008 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (ok)
															#line 14475 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2009 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<CheckForExcludingServersActor*>(this)->SAV<std::set<NetworkAddress>>::futures) { (void)(inProgressExclusion); this->~CheckForExcludingServersActorState(); static_cast<CheckForExcludingServersActor*>(this)->destroy(); return 0; }
															#line 14479 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<CheckForExcludingServersActor*>(this)->SAV< std::set<NetworkAddress> >::value()) std::set<NetworkAddress>(std::move(inProgressExclusion)); // state_var_RVO
			this->~CheckForExcludingServersActorState();
			static_cast<CheckForExcludingServersActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 2010 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!waitForAllExcluded)
															#line 14487 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 2013 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delayJittered(1.0);
															#line 2013 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<CheckForExcludingServersActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 14495 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<CheckForExcludingServersActor*>(this)->actor_wait_state = 2;
															#line 2013 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckForExcludingServersActor, 1, Void >*>(static_cast<CheckForExcludingServersActor*>(this)));
															#line 14500 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(bool const& ok,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(ok, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(bool && ok,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(ok), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CheckForExcludingServersActor*>(this)->actor_wait_state > 0) static_cast<CheckForExcludingServersActor*>(this)->actor_wait_state = 0;
		static_cast<CheckForExcludingServersActor*>(this)->ActorCallback< CheckForExcludingServersActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< CheckForExcludingServersActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckForExcludingServersActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckForExcludingServersActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(loopDepth);

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
		if (static_cast<CheckForExcludingServersActor*>(this)->actor_wait_state > 0) static_cast<CheckForExcludingServersActor*>(this)->actor_wait_state = 0;
		static_cast<CheckForExcludingServersActor*>(this)->ActorCallback< CheckForExcludingServersActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CheckForExcludingServersActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CheckForExcludingServersActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CheckForExcludingServersActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<CheckForExcludingServersActor*>(this)->actor_wait_state > 0) static_cast<CheckForExcludingServersActor*>(this)->actor_wait_state = 0;
		static_cast<CheckForExcludingServersActor*>(this)->ActorCallback< CheckForExcludingServersActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CheckForExcludingServersActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CheckForExcludingServersActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CheckForExcludingServersActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkForExcludingServers", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 1997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 1997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::vector<AddressExclusion> excl;
															#line 1997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	bool waitForAllExcluded;
															#line 2000 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::set<AddressExclusion> exclusions;
															#line 2001 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::set<NetworkAddress> inProgressExclusion;
															#line 2004 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ReadYourWritesTransaction tr;
															#line 14743 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via checkForExcludingServers()
															#line 1997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class CheckForExcludingServersActor final : public Actor<std::set<NetworkAddress>>, public ActorCallback< CheckForExcludingServersActor, 0, bool >, public ActorCallback< CheckForExcludingServersActor, 1, Void >, public ActorCallback< CheckForExcludingServersActor, 2, Void >, public FastAllocated<CheckForExcludingServersActor>, public CheckForExcludingServersActorState<CheckForExcludingServersActor> {
															#line 14748 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<CheckForExcludingServersActor>::operator new;
	using FastAllocated<CheckForExcludingServersActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::set<NetworkAddress>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckForExcludingServersActor, 0, bool >;
friend struct ActorCallback< CheckForExcludingServersActor, 1, Void >;
friend struct ActorCallback< CheckForExcludingServersActor, 2, Void >;
															#line 1997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	CheckForExcludingServersActor(Database const& cx,std::vector<AddressExclusion> const& excl,bool const& waitForAllExcluded) 
															#line 14761 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<std::set<NetworkAddress>>(),
		   CheckForExcludingServersActorState<CheckForExcludingServersActor>(cx, excl, waitForAllExcluded)
	{
		fdb_probe_actor_enter("checkForExcludingServers", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkForExcludingServers");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkForExcludingServers", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckForExcludingServersActor, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CheckForExcludingServersActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CheckForExcludingServersActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<std::set<NetworkAddress>> checkForExcludingServers( Database const& cx, std::vector<AddressExclusion> const& excl, bool const& waitForAllExcluded ) {
															#line 1997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<std::set<NetworkAddress>>(new CheckForExcludingServersActor(cx, excl, waitForAllExcluded));
															#line 14791 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2021 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 14796 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via mgmtSnapCreate()
															#line 2022 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class MgmtSnapCreateActor>
															#line 2022 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class MgmtSnapCreateActorState {
															#line 14803 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2022 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	MgmtSnapCreateActorState(Database const& cx,Standalone<StringRef> const& snapCmd,UID const& snapUID) 
															#line 2022 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2022 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 2022 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   snapCmd(snapCmd),
															#line 2022 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   snapUID(snapUID)
															#line 14814 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("mgmtSnapCreate", reinterpret_cast<unsigned long>(this));

	}
	~MgmtSnapCreateActorState() 
	{
		fdb_probe_actor_destroy("mgmtSnapCreate", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 2024 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				StrictFuture<Void> __when_expr_0 = snapCreate(cx, snapCmd, snapUID);
															#line 2024 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (static_cast<MgmtSnapCreateActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 14832 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<MgmtSnapCreateActor*>(this)->actor_wait_state = 1;
															#line 2024 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MgmtSnapCreateActor, 0, Void >*>(static_cast<MgmtSnapCreateActor*>(this)));
															#line 14837 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~MgmtSnapCreateActorState();
		static_cast<MgmtSnapCreateActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 2028 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			TraceEvent(SevWarn, "SnapCreateFailed").error(e).detail("snapUID", snapUID);
															#line 2029 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 14869 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 2025 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		TraceEvent("SnapCreateSucceeded").detail("snapUID", snapUID);
															#line 2026 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<MgmtSnapCreateActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MgmtSnapCreateActorState(); static_cast<MgmtSnapCreateActor*>(this)->destroy(); return 0; }
															#line 14885 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<MgmtSnapCreateActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~MgmtSnapCreateActorState();
		static_cast<MgmtSnapCreateActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 2025 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		TraceEvent("SnapCreateSucceeded").detail("snapUID", snapUID);
															#line 2026 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<MgmtSnapCreateActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MgmtSnapCreateActorState(); static_cast<MgmtSnapCreateActor*>(this)->destroy(); return 0; }
															#line 14899 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<MgmtSnapCreateActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~MgmtSnapCreateActorState();
		static_cast<MgmtSnapCreateActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<MgmtSnapCreateActor*>(this)->actor_wait_state > 0) static_cast<MgmtSnapCreateActor*>(this)->actor_wait_state = 0;
		static_cast<MgmtSnapCreateActor*>(this)->ActorCallback< MgmtSnapCreateActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MgmtSnapCreateActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("mgmtSnapCreate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mgmtSnapCreate", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MgmtSnapCreateActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("mgmtSnapCreate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mgmtSnapCreate", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MgmtSnapCreateActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("mgmtSnapCreate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("mgmtSnapCreate", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2022 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 2022 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Standalone<StringRef> snapCmd;
															#line 2022 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UID snapUID;
															#line 14976 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via mgmtSnapCreate()
															#line 2022 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class MgmtSnapCreateActor final : public Actor<Void>, public ActorCallback< MgmtSnapCreateActor, 0, Void >, public FastAllocated<MgmtSnapCreateActor>, public MgmtSnapCreateActorState<MgmtSnapCreateActor> {
															#line 14981 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<MgmtSnapCreateActor>::operator new;
	using FastAllocated<MgmtSnapCreateActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MgmtSnapCreateActor, 0, Void >;
															#line 2022 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	MgmtSnapCreateActor(Database const& cx,Standalone<StringRef> const& snapCmd,UID const& snapUID) 
															#line 14992 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   MgmtSnapCreateActorState<MgmtSnapCreateActor>(cx, snapCmd, snapUID)
	{
		fdb_probe_actor_enter("mgmtSnapCreate", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("mgmtSnapCreate");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("mgmtSnapCreate", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MgmtSnapCreateActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2022 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> mgmtSnapCreate( Database const& cx, Standalone<StringRef> const& snapCmd, UID const& snapUID ) {
															#line 2022 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new MgmtSnapCreateActor(cx, snapCmd, snapUID));
															#line 15020 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2032 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 15025 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via waitForFullReplication()
															#line 2033 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class WaitForFullReplicationActor>
															#line 2033 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class WaitForFullReplicationActorState {
															#line 15032 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2033 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	WaitForFullReplicationActorState(Database const& cx) 
															#line 2033 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2033 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 2034 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 15041 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("waitForFullReplication", reinterpret_cast<unsigned long>(this));

	}
	~WaitForFullReplicationActorState() 
	{
		fdb_probe_actor_destroy("waitForFullReplication", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2035 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 15056 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~WaitForFullReplicationActorState();
		static_cast<WaitForFullReplicationActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 2037 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 2038 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 2039 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 2041 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr.getRange(configKeys, CLIENT_KNOBS->TOO_MANY);
															#line 2041 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 15095 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state = 1;
															#line 2041 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitForFullReplicationActor, 0, RangeResult >*>(static_cast<WaitForFullReplicationActor*>(this)));
															#line 15100 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 2068 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_4 = tr.onError(e);
															#line 2068 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 15124 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_4.get(), loopDepth); };
			static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state = 5;
															#line 2068 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< WaitForFullReplicationActor, 4, Void >*>(static_cast<WaitForFullReplicationActor*>(this)));
															#line 15129 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(RangeResult const& confResults,int loopDepth) 
	{
															#line 2042 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(!confResults.more && confResults.size() < CLIENT_KNOBS->TOO_MANY);
															#line 2043 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		config = DatabaseConfiguration();
															#line 2044 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		config.fromKeyValues((VectorRef<KeyValueRef>)confResults);
															#line 2046 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		replicasFutures = std::vector<Future<Optional<Value>>>();
															#line 2047 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto& region : config.regions ) {
															#line 2048 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			replicasFutures.push_back(tr.get(datacenterReplicasKeyFor(region.dcId)));
															#line 15154 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 2050 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = waitForAll(replicasFutures);
															#line 2050 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 15160 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state = 2;
															#line 2050 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitForFullReplicationActor, 1, Void >*>(static_cast<WaitForFullReplicationActor*>(this)));
															#line 15165 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(RangeResult && confResults,int loopDepth) 
	{
															#line 2042 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(!confResults.more && confResults.size() < CLIENT_KNOBS->TOO_MANY);
															#line 2043 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		config = DatabaseConfiguration();
															#line 2044 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		config.fromKeyValues((VectorRef<KeyValueRef>)confResults);
															#line 2046 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		replicasFutures = std::vector<Future<Optional<Value>>>();
															#line 2047 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for( auto& region : config.regions ) {
															#line 2048 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			replicasFutures.push_back(tr.get(datacenterReplicasKeyFor(region.dcId)));
															#line 15184 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 2050 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = waitForAll(replicasFutures);
															#line 2050 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 15190 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state = 2;
															#line 2050 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitForFullReplicationActor, 1, Void >*>(static_cast<WaitForFullReplicationActor*>(this)));
															#line 15195 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& confResults,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(confResults, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && confResults,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(confResults), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state > 0) static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForFullReplicationActor*>(this)->ActorCallback< WaitForFullReplicationActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForFullReplicationActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitForFullReplicationActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WaitForFullReplicationActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 2052 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		watchFutures = std::vector<Future<Void>>();
															#line 2053 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(int i = 0;i < config.regions.size();i++) {
															#line 2054 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!replicasFutures[i].get().present() || decodeDatacenterReplicasValue(replicasFutures[i].get().get()) < config.storageTeamSize)
															#line 15271 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2056 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				watchFutures.push_back(tr.watch(datacenterReplicasKeyFor(config.regions[i].dcId)));
															#line 15275 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 2060 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!watchFutures.size() || (config.usableRegions == 1 && watchFutures.size() < config.regions.size()))
															#line 15280 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<WaitForFullReplicationActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitForFullReplicationActorState(); static_cast<WaitForFullReplicationActor*>(this)->destroy(); return 0; }
															#line 15284 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<WaitForFullReplicationActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WaitForFullReplicationActorState();
			static_cast<WaitForFullReplicationActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 2064 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 2064 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 15294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state = 3;
															#line 2064 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WaitForFullReplicationActor, 2, Void >*>(static_cast<WaitForFullReplicationActor*>(this)));
															#line 15299 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 2052 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		watchFutures = std::vector<Future<Void>>();
															#line 2053 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(int i = 0;i < config.regions.size();i++) {
															#line 2054 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!replicasFutures[i].get().present() || decodeDatacenterReplicasValue(replicasFutures[i].get().get()) < config.storageTeamSize)
															#line 15312 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2056 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				watchFutures.push_back(tr.watch(datacenterReplicasKeyFor(config.regions[i].dcId)));
															#line 15316 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 2060 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!watchFutures.size() || (config.usableRegions == 1 && watchFutures.size() < config.regions.size()))
															#line 15321 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<WaitForFullReplicationActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitForFullReplicationActorState(); static_cast<WaitForFullReplicationActor*>(this)->destroy(); return 0; }
															#line 15325 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<WaitForFullReplicationActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WaitForFullReplicationActorState();
			static_cast<WaitForFullReplicationActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 2064 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 2064 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 15335 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state = 3;
															#line 2064 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WaitForFullReplicationActor, 2, Void >*>(static_cast<WaitForFullReplicationActor*>(this)));
															#line 15340 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		if (static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state > 0) static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForFullReplicationActor*>(this)->ActorCallback< WaitForFullReplicationActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForFullReplicationActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WaitForFullReplicationActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WaitForFullReplicationActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont5(Void const& _,int loopDepth) 
	{
															#line 2065 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_3 = waitForAny(watchFutures);
															#line 2065 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 15414 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state = 4;
															#line 2065 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WaitForFullReplicationActor, 3, Void >*>(static_cast<WaitForFullReplicationActor*>(this)));
															#line 15419 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont5(Void && _,int loopDepth) 
	{
															#line 2065 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_3 = waitForAny(watchFutures);
															#line 2065 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 15430 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state = 4;
															#line 2065 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WaitForFullReplicationActor, 3, Void >*>(static_cast<WaitForFullReplicationActor*>(this)));
															#line 15435 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state > 0) static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForFullReplicationActor*>(this)->ActorCallback< WaitForFullReplicationActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForFullReplicationActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< WaitForFullReplicationActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< WaitForFullReplicationActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont9(Void const& _,int loopDepth) 
	{
															#line 2066 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.reset();
															#line 15507 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void && _,int loopDepth) 
	{
															#line 2066 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.reset();
															#line 15516 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state > 0) static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForFullReplicationActor*>(this)->ActorCallback< WaitForFullReplicationActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForFullReplicationActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< WaitForFullReplicationActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< WaitForFullReplicationActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1cont11(int loopDepth) 
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
	void a_exitChoose5() 
	{
		if (static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state > 0) static_cast<WaitForFullReplicationActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForFullReplicationActor*>(this)->ActorCallback< WaitForFullReplicationActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForFullReplicationActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< WaitForFullReplicationActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< WaitForFullReplicationActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 2033 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 2034 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ReadYourWritesTransaction tr;
															#line 2043 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	DatabaseConfiguration config;
															#line 2046 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::vector<Future<Optional<Value>>> replicasFutures;
															#line 2052 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	std::vector<Future<Void>> watchFutures;
															#line 15682 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via waitForFullReplication()
															#line 2033 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class WaitForFullReplicationActor final : public Actor<Void>, public ActorCallback< WaitForFullReplicationActor, 0, RangeResult >, public ActorCallback< WaitForFullReplicationActor, 1, Void >, public ActorCallback< WaitForFullReplicationActor, 2, Void >, public ActorCallback< WaitForFullReplicationActor, 3, Void >, public ActorCallback< WaitForFullReplicationActor, 4, Void >, public FastAllocated<WaitForFullReplicationActor>, public WaitForFullReplicationActorState<WaitForFullReplicationActor> {
															#line 15687 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<WaitForFullReplicationActor>::operator new;
	using FastAllocated<WaitForFullReplicationActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitForFullReplicationActor, 0, RangeResult >;
friend struct ActorCallback< WaitForFullReplicationActor, 1, Void >;
friend struct ActorCallback< WaitForFullReplicationActor, 2, Void >;
friend struct ActorCallback< WaitForFullReplicationActor, 3, Void >;
friend struct ActorCallback< WaitForFullReplicationActor, 4, Void >;
															#line 2033 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	WaitForFullReplicationActor(Database const& cx) 
															#line 15702 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   WaitForFullReplicationActorState<WaitForFullReplicationActor>(cx)
	{
		fdb_probe_actor_enter("waitForFullReplication", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitForFullReplication");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitForFullReplication", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WaitForFullReplicationActor, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WaitForFullReplicationActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< WaitForFullReplicationActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< WaitForFullReplicationActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< WaitForFullReplicationActor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2033 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> waitForFullReplication( Database const& cx ) {
															#line 2033 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new WaitForFullReplicationActor(cx));
															#line 15734 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2072 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 15739 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via timeKeeperSetDisable()
															#line 2073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class TimeKeeperSetDisableActor>
															#line 2073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class TimeKeeperSetDisableActorState {
															#line 15746 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	TimeKeeperSetDisableActorState(Database const& cx) 
															#line 2073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx)
															#line 15753 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this));

	}
	~TimeKeeperSetDisableActorState() 
	{
		fdb_probe_actor_destroy("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2074 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 15768 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~TimeKeeperSetDisableActorState();
		static_cast<TimeKeeperSetDisableActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 2075 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr = Transaction(cx);
															#line 15798 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		try {
															#line 2077 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 2078 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 2079 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.set(timeKeeperDisableKey, StringRef());
															#line 2080 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_0 = tr.commit();
															#line 2080 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<TimeKeeperSetDisableActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 15810 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TimeKeeperSetDisableActor*>(this)->actor_wait_state = 1;
															#line 2080 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TimeKeeperSetDisableActor, 0, Void >*>(static_cast<TimeKeeperSetDisableActor*>(this)));
															#line 15815 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 2083 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.onError(e);
															#line 2083 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<TimeKeeperSetDisableActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 15839 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TimeKeeperSetDisableActor*>(this)->actor_wait_state = 2;
															#line 2083 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TimeKeeperSetDisableActor, 1, Void >*>(static_cast<TimeKeeperSetDisableActor*>(this)));
															#line 15844 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 2081 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<TimeKeeperSetDisableActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TimeKeeperSetDisableActorState(); static_cast<TimeKeeperSetDisableActor*>(this)->destroy(); return 0; }
															#line 15859 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<TimeKeeperSetDisableActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TimeKeeperSetDisableActorState();
		static_cast<TimeKeeperSetDisableActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 2081 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<TimeKeeperSetDisableActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TimeKeeperSetDisableActorState(); static_cast<TimeKeeperSetDisableActor*>(this)->destroy(); return 0; }
															#line 15871 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<TimeKeeperSetDisableActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TimeKeeperSetDisableActorState();
		static_cast<TimeKeeperSetDisableActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<TimeKeeperSetDisableActor*>(this)->actor_wait_state > 0) static_cast<TimeKeeperSetDisableActor*>(this)->actor_wait_state = 0;
		static_cast<TimeKeeperSetDisableActor*>(this)->ActorCallback< TimeKeeperSetDisableActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TimeKeeperSetDisableActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TimeKeeperSetDisableActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TimeKeeperSetDisableActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<TimeKeeperSetDisableActor*>(this)->actor_wait_state > 0) static_cast<TimeKeeperSetDisableActor*>(this)->actor_wait_state = 0;
		static_cast<TimeKeeperSetDisableActor*>(this)->ActorCallback< TimeKeeperSetDisableActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TimeKeeperSetDisableActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TimeKeeperSetDisableActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TimeKeeperSetDisableActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 2073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 2075 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 16021 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via timeKeeperSetDisable()
															#line 2073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class TimeKeeperSetDisableActor final : public Actor<Void>, public ActorCallback< TimeKeeperSetDisableActor, 0, Void >, public ActorCallback< TimeKeeperSetDisableActor, 1, Void >, public FastAllocated<TimeKeeperSetDisableActor>, public TimeKeeperSetDisableActorState<TimeKeeperSetDisableActor> {
															#line 16026 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<TimeKeeperSetDisableActor>::operator new;
	using FastAllocated<TimeKeeperSetDisableActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TimeKeeperSetDisableActor, 0, Void >;
friend struct ActorCallback< TimeKeeperSetDisableActor, 1, Void >;
															#line 2073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	TimeKeeperSetDisableActor(Database const& cx) 
															#line 16038 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   TimeKeeperSetDisableActorState<TimeKeeperSetDisableActor>(cx)
	{
		fdb_probe_actor_enter("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("timeKeeperSetDisable");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("timeKeeperSetDisable", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TimeKeeperSetDisableActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TimeKeeperSetDisableActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> timeKeeperSetDisable( Database const& cx ) {
															#line 2073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new TimeKeeperSetDisableActor(cx));
															#line 16067 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2087 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 16072 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via lockDatabase()
															#line 2088 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class LockDatabaseActor>
															#line 2088 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class LockDatabaseActorState {
															#line 16079 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2088 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	LockDatabaseActorState(Transaction* const& tr,UID const& id) 
															#line 2088 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2088 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr),
															#line 2088 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   id(id)
															#line 16088 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("lockDatabase", reinterpret_cast<unsigned long>(this));

	}
	~LockDatabaseActorState() 
	{
		fdb_probe_actor_destroy("lockDatabase", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2089 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 2090 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 2091 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(databaseLockedKey);
															#line 2091 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<LockDatabaseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 16109 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<LockDatabaseActor*>(this)->actor_wait_state = 1;
															#line 2091 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< LockDatabaseActor, 0, Optional<Value> >*>(static_cast<LockDatabaseActor*>(this)));
															#line 16114 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~LockDatabaseActorState();
		static_cast<LockDatabaseActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 2093 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present())
															#line 16137 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2094 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) == id)
															#line 16141 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2095 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (!static_cast<LockDatabaseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~LockDatabaseActorState(); static_cast<LockDatabaseActor*>(this)->destroy(); return 0; }
															#line 16145 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				new (&static_cast<LockDatabaseActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~LockDatabaseActorState();
				static_cast<LockDatabaseActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 2098 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				return a_body1Catch1(database_locked(), loopDepth);
															#line 16155 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 2102 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr->atomicOp(databaseLockedKey, BinaryWriter::toValue(id, Unversioned()) .withPrefix(LiteralStringRef("0123456789")) .withSuffix(LiteralStringRef("\x00\x00\x00\x00")), MutationRef::SetVersionstampedValue);
															#line 2107 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr->addWriteConflictRange(normalKeys);
															#line 2108 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<LockDatabaseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~LockDatabaseActorState(); static_cast<LockDatabaseActor*>(this)->destroy(); return 0; }
															#line 16164 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<LockDatabaseActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~LockDatabaseActorState();
		static_cast<LockDatabaseActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 2093 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present())
															#line 16176 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2094 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) == id)
															#line 16180 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2095 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (!static_cast<LockDatabaseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~LockDatabaseActorState(); static_cast<LockDatabaseActor*>(this)->destroy(); return 0; }
															#line 16184 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				new (&static_cast<LockDatabaseActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~LockDatabaseActorState();
				static_cast<LockDatabaseActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 2098 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				return a_body1Catch1(database_locked(), loopDepth);
															#line 16194 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 2102 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr->atomicOp(databaseLockedKey, BinaryWriter::toValue(id, Unversioned()) .withPrefix(LiteralStringRef("0123456789")) .withSuffix(LiteralStringRef("\x00\x00\x00\x00")), MutationRef::SetVersionstampedValue);
															#line 2107 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr->addWriteConflictRange(normalKeys);
															#line 2108 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<LockDatabaseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~LockDatabaseActorState(); static_cast<LockDatabaseActor*>(this)->destroy(); return 0; }
															#line 16203 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<LockDatabaseActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~LockDatabaseActorState();
		static_cast<LockDatabaseActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<LockDatabaseActor*>(this)->actor_wait_state > 0) static_cast<LockDatabaseActor*>(this)->actor_wait_state = 0;
		static_cast<LockDatabaseActor*>(this)->ActorCallback< LockDatabaseActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< LockDatabaseActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< LockDatabaseActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< LockDatabaseActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2088 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 2088 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UID id;
															#line 16278 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via lockDatabase()
															#line 2088 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class LockDatabaseActor final : public Actor<Void>, public ActorCallback< LockDatabaseActor, 0, Optional<Value> >, public FastAllocated<LockDatabaseActor>, public LockDatabaseActorState<LockDatabaseActor> {
															#line 16283 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<LockDatabaseActor>::operator new;
	using FastAllocated<LockDatabaseActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< LockDatabaseActor, 0, Optional<Value> >;
															#line 2088 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	LockDatabaseActor(Transaction* const& tr,UID const& id) 
															#line 16294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   LockDatabaseActorState<LockDatabaseActor>(tr, id)
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("lockDatabase");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< LockDatabaseActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2088 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> lockDatabase( Transaction* const& tr, UID const& id ) {
															#line 2088 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new LockDatabaseActor(tr, id));
															#line 16322 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2110 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 16327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via lockDatabase()
															#line 2111 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class LockDatabaseActor1>
															#line 2111 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class LockDatabaseActor1State {
															#line 16334 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2111 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	LockDatabaseActor1State(Reference<ReadYourWritesTransaction> const& tr,UID const& id) 
															#line 2111 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2111 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr),
															#line 2111 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   id(id)
															#line 16343 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("lockDatabase", reinterpret_cast<unsigned long>(this));

	}
	~LockDatabaseActor1State() 
	{
		fdb_probe_actor_destroy("lockDatabase", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2112 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 2113 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 2114 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(databaseLockedKey);
															#line 2114 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<LockDatabaseActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 16364 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<LockDatabaseActor1*>(this)->actor_wait_state = 1;
															#line 2114 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< LockDatabaseActor1, 0, Optional<Value> >*>(static_cast<LockDatabaseActor1*>(this)));
															#line 16369 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~LockDatabaseActor1State();
		static_cast<LockDatabaseActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 2116 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present())
															#line 16392 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2117 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) == id)
															#line 16396 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2118 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (!static_cast<LockDatabaseActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~LockDatabaseActor1State(); static_cast<LockDatabaseActor1*>(this)->destroy(); return 0; }
															#line 16400 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				new (&static_cast<LockDatabaseActor1*>(this)->SAV< Void >::value()) Void(Void());
				this->~LockDatabaseActor1State();
				static_cast<LockDatabaseActor1*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 2121 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				return a_body1Catch1(database_locked(), loopDepth);
															#line 16410 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 2125 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr->atomicOp(databaseLockedKey, BinaryWriter::toValue(id, Unversioned()) .withPrefix(LiteralStringRef("0123456789")) .withSuffix(LiteralStringRef("\x00\x00\x00\x00")), MutationRef::SetVersionstampedValue);
															#line 2130 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr->addWriteConflictRange(normalKeys);
															#line 2131 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<LockDatabaseActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~LockDatabaseActor1State(); static_cast<LockDatabaseActor1*>(this)->destroy(); return 0; }
															#line 16419 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<LockDatabaseActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~LockDatabaseActor1State();
		static_cast<LockDatabaseActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 2116 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present())
															#line 16431 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2117 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) == id)
															#line 16435 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2118 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (!static_cast<LockDatabaseActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~LockDatabaseActor1State(); static_cast<LockDatabaseActor1*>(this)->destroy(); return 0; }
															#line 16439 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				new (&static_cast<LockDatabaseActor1*>(this)->SAV< Void >::value()) Void(Void());
				this->~LockDatabaseActor1State();
				static_cast<LockDatabaseActor1*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 2121 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				return a_body1Catch1(database_locked(), loopDepth);
															#line 16449 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 2125 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr->atomicOp(databaseLockedKey, BinaryWriter::toValue(id, Unversioned()) .withPrefix(LiteralStringRef("0123456789")) .withSuffix(LiteralStringRef("\x00\x00\x00\x00")), MutationRef::SetVersionstampedValue);
															#line 2130 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr->addWriteConflictRange(normalKeys);
															#line 2131 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<LockDatabaseActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~LockDatabaseActor1State(); static_cast<LockDatabaseActor1*>(this)->destroy(); return 0; }
															#line 16458 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<LockDatabaseActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~LockDatabaseActor1State();
		static_cast<LockDatabaseActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<LockDatabaseActor1*>(this)->actor_wait_state > 0) static_cast<LockDatabaseActor1*>(this)->actor_wait_state = 0;
		static_cast<LockDatabaseActor1*>(this)->ActorCallback< LockDatabaseActor1, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< LockDatabaseActor1, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< LockDatabaseActor1, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< LockDatabaseActor1, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2111 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 2111 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UID id;
															#line 16533 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via lockDatabase()
															#line 2111 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class LockDatabaseActor1 final : public Actor<Void>, public ActorCallback< LockDatabaseActor1, 0, Optional<Value> >, public FastAllocated<LockDatabaseActor1>, public LockDatabaseActor1State<LockDatabaseActor1> {
															#line 16538 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<LockDatabaseActor1>::operator new;
	using FastAllocated<LockDatabaseActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< LockDatabaseActor1, 0, Optional<Value> >;
															#line 2111 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	LockDatabaseActor1(Reference<ReadYourWritesTransaction> const& tr,UID const& id) 
															#line 16549 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   LockDatabaseActor1State<LockDatabaseActor1>(tr, id)
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("lockDatabase");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< LockDatabaseActor1, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2111 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> lockDatabase( Reference<ReadYourWritesTransaction> const& tr, UID const& id ) {
															#line 2111 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new LockDatabaseActor1(tr, id));
															#line 16577 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 16582 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via lockDatabase()
															#line 2134 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class LockDatabaseActor2>
															#line 2134 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class LockDatabaseActor2State {
															#line 16589 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2134 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	LockDatabaseActor2State(Database const& cx,UID const& id) 
															#line 2134 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2134 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 2134 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   id(id),
															#line 2135 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 16600 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("lockDatabase", reinterpret_cast<unsigned long>(this));

	}
	~LockDatabaseActor2State() 
	{
		fdb_probe_actor_destroy("lockDatabase", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2136 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 16615 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~LockDatabaseActor2State();
		static_cast<LockDatabaseActor2*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 2138 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_0 = lockDatabase(&tr, id);
															#line 2138 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<LockDatabaseActor2*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 16648 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<LockDatabaseActor2*>(this)->actor_wait_state = 1;
															#line 2138 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< LockDatabaseActor2, 0, Void >*>(static_cast<LockDatabaseActor2*>(this)));
															#line 16653 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 2142 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (e.code() == error_code_database_locked)
															#line 16675 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2143 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 16679 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 2144 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_2 = tr.onError(e);
															#line 2144 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<LockDatabaseActor2*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 16685 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<LockDatabaseActor2*>(this)->actor_wait_state = 3;
															#line 2144 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< LockDatabaseActor2, 2, Void >*>(static_cast<LockDatabaseActor2*>(this)));
															#line 16690 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 2139 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 2139 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<LockDatabaseActor2*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 16707 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<LockDatabaseActor2*>(this)->actor_wait_state = 2;
															#line 2139 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< LockDatabaseActor2, 1, Void >*>(static_cast<LockDatabaseActor2*>(this)));
															#line 16712 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 2139 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 2139 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<LockDatabaseActor2*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 16723 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<LockDatabaseActor2*>(this)->actor_wait_state = 2;
															#line 2139 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< LockDatabaseActor2, 1, Void >*>(static_cast<LockDatabaseActor2*>(this)));
															#line 16728 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		if (static_cast<LockDatabaseActor2*>(this)->actor_wait_state > 0) static_cast<LockDatabaseActor2*>(this)->actor_wait_state = 0;
		static_cast<LockDatabaseActor2*>(this)->ActorCallback< LockDatabaseActor2, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LockDatabaseActor2, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< LockDatabaseActor2, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< LockDatabaseActor2, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 2140 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<LockDatabaseActor2*>(this)->SAV<Void>::futures) { (void)(Void()); this->~LockDatabaseActor2State(); static_cast<LockDatabaseActor2*>(this)->destroy(); return 0; }
															#line 16800 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<LockDatabaseActor2*>(this)->SAV< Void >::value()) Void(Void());
		this->~LockDatabaseActor2State();
		static_cast<LockDatabaseActor2*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 2140 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<LockDatabaseActor2*>(this)->SAV<Void>::futures) { (void)(Void()); this->~LockDatabaseActor2State(); static_cast<LockDatabaseActor2*>(this)->destroy(); return 0; }
															#line 16812 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<LockDatabaseActor2*>(this)->SAV< Void >::value()) Void(Void());
		this->~LockDatabaseActor2State();
		static_cast<LockDatabaseActor2*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<LockDatabaseActor2*>(this)->actor_wait_state > 0) static_cast<LockDatabaseActor2*>(this)->actor_wait_state = 0;
		static_cast<LockDatabaseActor2*>(this)->ActorCallback< LockDatabaseActor2, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LockDatabaseActor2, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< LockDatabaseActor2, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< LockDatabaseActor2, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<LockDatabaseActor2*>(this)->actor_wait_state > 0) static_cast<LockDatabaseActor2*>(this)->actor_wait_state = 0;
		static_cast<LockDatabaseActor2*>(this)->ActorCallback< LockDatabaseActor2, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LockDatabaseActor2, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< LockDatabaseActor2, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< LockDatabaseActor2, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 2134 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 2134 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UID id;
															#line 2135 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 16964 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via lockDatabase()
															#line 2134 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class LockDatabaseActor2 final : public Actor<Void>, public ActorCallback< LockDatabaseActor2, 0, Void >, public ActorCallback< LockDatabaseActor2, 1, Void >, public ActorCallback< LockDatabaseActor2, 2, Void >, public FastAllocated<LockDatabaseActor2>, public LockDatabaseActor2State<LockDatabaseActor2> {
															#line 16969 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<LockDatabaseActor2>::operator new;
	using FastAllocated<LockDatabaseActor2>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< LockDatabaseActor2, 0, Void >;
friend struct ActorCallback< LockDatabaseActor2, 1, Void >;
friend struct ActorCallback< LockDatabaseActor2, 2, Void >;
															#line 2134 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	LockDatabaseActor2(Database const& cx,UID const& id) 
															#line 16982 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   LockDatabaseActor2State<LockDatabaseActor2>(cx, id)
	{
		fdb_probe_actor_enter("lockDatabase", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("lockDatabase");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("lockDatabase", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< LockDatabaseActor2, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< LockDatabaseActor2, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< LockDatabaseActor2, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2134 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> lockDatabase( Database const& cx, UID const& id ) {
															#line 2134 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new LockDatabaseActor2(cx, id));
															#line 17012 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2148 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 17017 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via unlockDatabase()
															#line 2149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class UnlockDatabaseActor>
															#line 2149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class UnlockDatabaseActorState {
															#line 17024 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UnlockDatabaseActorState(Transaction* const& tr,UID const& id) 
															#line 2149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr),
															#line 2149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   id(id)
															#line 17033 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("unlockDatabase", reinterpret_cast<unsigned long>(this));

	}
	~UnlockDatabaseActorState() 
	{
		fdb_probe_actor_destroy("unlockDatabase", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2150 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 2151 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 2152 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(databaseLockedKey);
															#line 2152 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<UnlockDatabaseActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 17054 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UnlockDatabaseActor*>(this)->actor_wait_state = 1;
															#line 2152 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UnlockDatabaseActor, 0, Optional<Value> >*>(static_cast<UnlockDatabaseActor*>(this)));
															#line 17059 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~UnlockDatabaseActorState();
		static_cast<UnlockDatabaseActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 2154 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!val.present())
															#line 17082 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2155 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<UnlockDatabaseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UnlockDatabaseActorState(); static_cast<UnlockDatabaseActor*>(this)->destroy(); return 0; }
															#line 17086 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<UnlockDatabaseActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~UnlockDatabaseActorState();
			static_cast<UnlockDatabaseActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 2157 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present() && BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) != id)
															#line 17094 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2159 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			return a_body1Catch1(database_locked(), loopDepth);
															#line 17098 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 2162 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr->clear(singleKeyRange(databaseLockedKey));
															#line 2163 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<UnlockDatabaseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UnlockDatabaseActorState(); static_cast<UnlockDatabaseActor*>(this)->destroy(); return 0; }
															#line 17104 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<UnlockDatabaseActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~UnlockDatabaseActorState();
		static_cast<UnlockDatabaseActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 2154 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!val.present())
															#line 17116 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2155 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<UnlockDatabaseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UnlockDatabaseActorState(); static_cast<UnlockDatabaseActor*>(this)->destroy(); return 0; }
															#line 17120 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<UnlockDatabaseActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~UnlockDatabaseActorState();
			static_cast<UnlockDatabaseActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 2157 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present() && BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) != id)
															#line 17128 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2159 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			return a_body1Catch1(database_locked(), loopDepth);
															#line 17132 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 2162 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr->clear(singleKeyRange(databaseLockedKey));
															#line 2163 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<UnlockDatabaseActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UnlockDatabaseActorState(); static_cast<UnlockDatabaseActor*>(this)->destroy(); return 0; }
															#line 17138 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<UnlockDatabaseActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~UnlockDatabaseActorState();
		static_cast<UnlockDatabaseActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<UnlockDatabaseActor*>(this)->actor_wait_state > 0) static_cast<UnlockDatabaseActor*>(this)->actor_wait_state = 0;
		static_cast<UnlockDatabaseActor*>(this)->ActorCallback< UnlockDatabaseActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< UnlockDatabaseActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UnlockDatabaseActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< UnlockDatabaseActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 2149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UID id;
															#line 17213 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via unlockDatabase()
															#line 2149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class UnlockDatabaseActor final : public Actor<Void>, public ActorCallback< UnlockDatabaseActor, 0, Optional<Value> >, public FastAllocated<UnlockDatabaseActor>, public UnlockDatabaseActorState<UnlockDatabaseActor> {
															#line 17218 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<UnlockDatabaseActor>::operator new;
	using FastAllocated<UnlockDatabaseActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UnlockDatabaseActor, 0, Optional<Value> >;
															#line 2149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UnlockDatabaseActor(Transaction* const& tr,UID const& id) 
															#line 17229 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   UnlockDatabaseActorState<UnlockDatabaseActor>(tr, id)
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("unlockDatabase");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< UnlockDatabaseActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> unlockDatabase( Transaction* const& tr, UID const& id ) {
															#line 2149 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new UnlockDatabaseActor(tr, id));
															#line 17257 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2165 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 17262 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via unlockDatabase()
															#line 2166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class UnlockDatabaseActor1>
															#line 2166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class UnlockDatabaseActor1State {
															#line 17269 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UnlockDatabaseActor1State(Reference<ReadYourWritesTransaction> const& tr,UID const& id) 
															#line 2166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr),
															#line 2166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   id(id)
															#line 17278 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("unlockDatabase", reinterpret_cast<unsigned long>(this));

	}
	~UnlockDatabaseActor1State() 
	{
		fdb_probe_actor_destroy("unlockDatabase", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2167 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 2168 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 2169 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(databaseLockedKey);
															#line 2169 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<UnlockDatabaseActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 17299 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UnlockDatabaseActor1*>(this)->actor_wait_state = 1;
															#line 2169 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UnlockDatabaseActor1, 0, Optional<Value> >*>(static_cast<UnlockDatabaseActor1*>(this)));
															#line 17304 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~UnlockDatabaseActor1State();
		static_cast<UnlockDatabaseActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 2171 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!val.present())
															#line 17327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2172 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<UnlockDatabaseActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UnlockDatabaseActor1State(); static_cast<UnlockDatabaseActor1*>(this)->destroy(); return 0; }
															#line 17331 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<UnlockDatabaseActor1*>(this)->SAV< Void >::value()) Void(Void());
			this->~UnlockDatabaseActor1State();
			static_cast<UnlockDatabaseActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 2174 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present() && BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) != id)
															#line 17339 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2176 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			return a_body1Catch1(database_locked(), loopDepth);
															#line 17343 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 2179 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr->clear(singleKeyRange(databaseLockedKey));
															#line 2180 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<UnlockDatabaseActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UnlockDatabaseActor1State(); static_cast<UnlockDatabaseActor1*>(this)->destroy(); return 0; }
															#line 17349 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<UnlockDatabaseActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~UnlockDatabaseActor1State();
		static_cast<UnlockDatabaseActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 2171 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!val.present())
															#line 17361 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2172 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<UnlockDatabaseActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UnlockDatabaseActor1State(); static_cast<UnlockDatabaseActor1*>(this)->destroy(); return 0; }
															#line 17365 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<UnlockDatabaseActor1*>(this)->SAV< Void >::value()) Void(Void());
			this->~UnlockDatabaseActor1State();
			static_cast<UnlockDatabaseActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 2174 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present() && BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) != id)
															#line 17373 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2176 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			return a_body1Catch1(database_locked(), loopDepth);
															#line 17377 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 2179 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr->clear(singleKeyRange(databaseLockedKey));
															#line 2180 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<UnlockDatabaseActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UnlockDatabaseActor1State(); static_cast<UnlockDatabaseActor1*>(this)->destroy(); return 0; }
															#line 17383 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<UnlockDatabaseActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~UnlockDatabaseActor1State();
		static_cast<UnlockDatabaseActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<UnlockDatabaseActor1*>(this)->actor_wait_state > 0) static_cast<UnlockDatabaseActor1*>(this)->actor_wait_state = 0;
		static_cast<UnlockDatabaseActor1*>(this)->ActorCallback< UnlockDatabaseActor1, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< UnlockDatabaseActor1, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UnlockDatabaseActor1, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< UnlockDatabaseActor1, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 2166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UID id;
															#line 17458 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via unlockDatabase()
															#line 2166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class UnlockDatabaseActor1 final : public Actor<Void>, public ActorCallback< UnlockDatabaseActor1, 0, Optional<Value> >, public FastAllocated<UnlockDatabaseActor1>, public UnlockDatabaseActor1State<UnlockDatabaseActor1> {
															#line 17463 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<UnlockDatabaseActor1>::operator new;
	using FastAllocated<UnlockDatabaseActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UnlockDatabaseActor1, 0, Optional<Value> >;
															#line 2166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UnlockDatabaseActor1(Reference<ReadYourWritesTransaction> const& tr,UID const& id) 
															#line 17474 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   UnlockDatabaseActor1State<UnlockDatabaseActor1>(tr, id)
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("unlockDatabase");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< UnlockDatabaseActor1, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> unlockDatabase( Reference<ReadYourWritesTransaction> const& tr, UID const& id ) {
															#line 2166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new UnlockDatabaseActor1(tr, id));
															#line 17502 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2182 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 17507 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via unlockDatabase()
															#line 2183 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class UnlockDatabaseActor2>
															#line 2183 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class UnlockDatabaseActor2State {
															#line 17514 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2183 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UnlockDatabaseActor2State(Database const& cx,UID const& id) 
															#line 2183 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2183 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 2183 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   id(id),
															#line 2184 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 17525 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("unlockDatabase", reinterpret_cast<unsigned long>(this));

	}
	~UnlockDatabaseActor2State() 
	{
		fdb_probe_actor_destroy("unlockDatabase", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2185 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 17540 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~UnlockDatabaseActor2State();
		static_cast<UnlockDatabaseActor2*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 2187 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_0 = unlockDatabase(&tr, id);
															#line 2187 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<UnlockDatabaseActor2*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 17573 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UnlockDatabaseActor2*>(this)->actor_wait_state = 1;
															#line 2187 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UnlockDatabaseActor2, 0, Void >*>(static_cast<UnlockDatabaseActor2*>(this)));
															#line 17578 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 2191 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (e.code() == error_code_database_locked)
															#line 17600 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2192 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 17604 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 2193 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_2 = tr.onError(e);
															#line 2193 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<UnlockDatabaseActor2*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 17610 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<UnlockDatabaseActor2*>(this)->actor_wait_state = 3;
															#line 2193 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< UnlockDatabaseActor2, 2, Void >*>(static_cast<UnlockDatabaseActor2*>(this)));
															#line 17615 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 2188 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 2188 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<UnlockDatabaseActor2*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 17632 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<UnlockDatabaseActor2*>(this)->actor_wait_state = 2;
															#line 2188 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< UnlockDatabaseActor2, 1, Void >*>(static_cast<UnlockDatabaseActor2*>(this)));
															#line 17637 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 2188 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 2188 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<UnlockDatabaseActor2*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 17648 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<UnlockDatabaseActor2*>(this)->actor_wait_state = 2;
															#line 2188 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< UnlockDatabaseActor2, 1, Void >*>(static_cast<UnlockDatabaseActor2*>(this)));
															#line 17653 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		if (static_cast<UnlockDatabaseActor2*>(this)->actor_wait_state > 0) static_cast<UnlockDatabaseActor2*>(this)->actor_wait_state = 0;
		static_cast<UnlockDatabaseActor2*>(this)->ActorCallback< UnlockDatabaseActor2, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< UnlockDatabaseActor2, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UnlockDatabaseActor2, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< UnlockDatabaseActor2, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 2189 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<UnlockDatabaseActor2*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UnlockDatabaseActor2State(); static_cast<UnlockDatabaseActor2*>(this)->destroy(); return 0; }
															#line 17725 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<UnlockDatabaseActor2*>(this)->SAV< Void >::value()) Void(Void());
		this->~UnlockDatabaseActor2State();
		static_cast<UnlockDatabaseActor2*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 2189 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<UnlockDatabaseActor2*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UnlockDatabaseActor2State(); static_cast<UnlockDatabaseActor2*>(this)->destroy(); return 0; }
															#line 17737 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<UnlockDatabaseActor2*>(this)->SAV< Void >::value()) Void(Void());
		this->~UnlockDatabaseActor2State();
		static_cast<UnlockDatabaseActor2*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<UnlockDatabaseActor2*>(this)->actor_wait_state > 0) static_cast<UnlockDatabaseActor2*>(this)->actor_wait_state = 0;
		static_cast<UnlockDatabaseActor2*>(this)->ActorCallback< UnlockDatabaseActor2, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< UnlockDatabaseActor2, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< UnlockDatabaseActor2, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< UnlockDatabaseActor2, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<UnlockDatabaseActor2*>(this)->actor_wait_state > 0) static_cast<UnlockDatabaseActor2*>(this)->actor_wait_state = 0;
		static_cast<UnlockDatabaseActor2*>(this)->ActorCallback< UnlockDatabaseActor2, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< UnlockDatabaseActor2, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< UnlockDatabaseActor2, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< UnlockDatabaseActor2, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 2183 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 2183 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UID id;
															#line 2184 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 17889 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via unlockDatabase()
															#line 2183 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class UnlockDatabaseActor2 final : public Actor<Void>, public ActorCallback< UnlockDatabaseActor2, 0, Void >, public ActorCallback< UnlockDatabaseActor2, 1, Void >, public ActorCallback< UnlockDatabaseActor2, 2, Void >, public FastAllocated<UnlockDatabaseActor2>, public UnlockDatabaseActor2State<UnlockDatabaseActor2> {
															#line 17894 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<UnlockDatabaseActor2>::operator new;
	using FastAllocated<UnlockDatabaseActor2>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UnlockDatabaseActor2, 0, Void >;
friend struct ActorCallback< UnlockDatabaseActor2, 1, Void >;
friend struct ActorCallback< UnlockDatabaseActor2, 2, Void >;
															#line 2183 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UnlockDatabaseActor2(Database const& cx,UID const& id) 
															#line 17907 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   UnlockDatabaseActor2State<UnlockDatabaseActor2>(cx, id)
	{
		fdb_probe_actor_enter("unlockDatabase", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("unlockDatabase");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("unlockDatabase", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< UnlockDatabaseActor2, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< UnlockDatabaseActor2, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< UnlockDatabaseActor2, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2183 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> unlockDatabase( Database const& cx, UID const& id ) {
															#line 2183 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new UnlockDatabaseActor2(cx, id));
															#line 17937 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2197 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 17942 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via checkDatabaseLock()
															#line 2198 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class CheckDatabaseLockActor>
															#line 2198 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class CheckDatabaseLockActorState {
															#line 17949 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2198 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	CheckDatabaseLockActorState(Transaction* const& tr,UID const& id) 
															#line 2198 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2198 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr),
															#line 2198 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   id(id)
															#line 17958 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("checkDatabaseLock", reinterpret_cast<unsigned long>(this));

	}
	~CheckDatabaseLockActorState() 
	{
		fdb_probe_actor_destroy("checkDatabaseLock", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2199 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 2200 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 2201 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(databaseLockedKey);
															#line 2201 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<CheckDatabaseLockActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 17979 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheckDatabaseLockActor*>(this)->actor_wait_state = 1;
															#line 2201 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckDatabaseLockActor, 0, Optional<Value> >*>(static_cast<CheckDatabaseLockActor*>(this)));
															#line 17984 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~CheckDatabaseLockActorState();
		static_cast<CheckDatabaseLockActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 2203 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present() && BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) != id)
															#line 18007 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2205 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			return a_body1Catch1(database_locked(), loopDepth);
															#line 18011 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 2208 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<CheckDatabaseLockActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckDatabaseLockActorState(); static_cast<CheckDatabaseLockActor*>(this)->destroy(); return 0; }
															#line 18015 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<CheckDatabaseLockActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheckDatabaseLockActorState();
		static_cast<CheckDatabaseLockActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 2203 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present() && BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) != id)
															#line 18027 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2205 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			return a_body1Catch1(database_locked(), loopDepth);
															#line 18031 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 2208 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<CheckDatabaseLockActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckDatabaseLockActorState(); static_cast<CheckDatabaseLockActor*>(this)->destroy(); return 0; }
															#line 18035 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<CheckDatabaseLockActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheckDatabaseLockActorState();
		static_cast<CheckDatabaseLockActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CheckDatabaseLockActor*>(this)->actor_wait_state > 0) static_cast<CheckDatabaseLockActor*>(this)->actor_wait_state = 0;
		static_cast<CheckDatabaseLockActor*>(this)->ActorCallback< CheckDatabaseLockActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< CheckDatabaseLockActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("checkDatabaseLock", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkDatabaseLock", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckDatabaseLockActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("checkDatabaseLock", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkDatabaseLock", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckDatabaseLockActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("checkDatabaseLock", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkDatabaseLock", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2198 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 2198 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UID id;
															#line 18110 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via checkDatabaseLock()
															#line 2198 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class CheckDatabaseLockActor final : public Actor<Void>, public ActorCallback< CheckDatabaseLockActor, 0, Optional<Value> >, public FastAllocated<CheckDatabaseLockActor>, public CheckDatabaseLockActorState<CheckDatabaseLockActor> {
															#line 18115 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<CheckDatabaseLockActor>::operator new;
	using FastAllocated<CheckDatabaseLockActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckDatabaseLockActor, 0, Optional<Value> >;
															#line 2198 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	CheckDatabaseLockActor(Transaction* const& tr,UID const& id) 
															#line 18126 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   CheckDatabaseLockActorState<CheckDatabaseLockActor>(tr, id)
	{
		fdb_probe_actor_enter("checkDatabaseLock", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkDatabaseLock");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkDatabaseLock", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckDatabaseLockActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2198 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> checkDatabaseLock( Transaction* const& tr, UID const& id ) {
															#line 2198 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new CheckDatabaseLockActor(tr, id));
															#line 18154 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2210 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 18159 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via checkDatabaseLock()
															#line 2211 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class CheckDatabaseLockActor1>
															#line 2211 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class CheckDatabaseLockActor1State {
															#line 18166 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2211 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	CheckDatabaseLockActor1State(Reference<ReadYourWritesTransaction> const& tr,UID const& id) 
															#line 2211 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2211 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr),
															#line 2211 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   id(id)
															#line 18175 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("checkDatabaseLock", reinterpret_cast<unsigned long>(this));

	}
	~CheckDatabaseLockActor1State() 
	{
		fdb_probe_actor_destroy("checkDatabaseLock", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2212 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 2213 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 2214 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(databaseLockedKey);
															#line 2214 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<CheckDatabaseLockActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 18196 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheckDatabaseLockActor1*>(this)->actor_wait_state = 1;
															#line 2214 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckDatabaseLockActor1, 0, Optional<Value> >*>(static_cast<CheckDatabaseLockActor1*>(this)));
															#line 18201 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~CheckDatabaseLockActor1State();
		static_cast<CheckDatabaseLockActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 2216 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present() && BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) != id)
															#line 18224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2218 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			return a_body1Catch1(database_locked(), loopDepth);
															#line 18228 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 2221 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<CheckDatabaseLockActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckDatabaseLockActor1State(); static_cast<CheckDatabaseLockActor1*>(this)->destroy(); return 0; }
															#line 18232 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<CheckDatabaseLockActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheckDatabaseLockActor1State();
		static_cast<CheckDatabaseLockActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 2216 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (val.present() && BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) != id)
															#line 18244 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2218 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			return a_body1Catch1(database_locked(), loopDepth);
															#line 18248 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 2221 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<CheckDatabaseLockActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckDatabaseLockActor1State(); static_cast<CheckDatabaseLockActor1*>(this)->destroy(); return 0; }
															#line 18252 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<CheckDatabaseLockActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheckDatabaseLockActor1State();
		static_cast<CheckDatabaseLockActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CheckDatabaseLockActor1*>(this)->actor_wait_state > 0) static_cast<CheckDatabaseLockActor1*>(this)->actor_wait_state = 0;
		static_cast<CheckDatabaseLockActor1*>(this)->ActorCallback< CheckDatabaseLockActor1, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< CheckDatabaseLockActor1, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("checkDatabaseLock", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkDatabaseLock", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckDatabaseLockActor1, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("checkDatabaseLock", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkDatabaseLock", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckDatabaseLockActor1, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("checkDatabaseLock", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkDatabaseLock", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2211 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 2211 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UID id;
															#line 18327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via checkDatabaseLock()
															#line 2211 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class CheckDatabaseLockActor1 final : public Actor<Void>, public ActorCallback< CheckDatabaseLockActor1, 0, Optional<Value> >, public FastAllocated<CheckDatabaseLockActor1>, public CheckDatabaseLockActor1State<CheckDatabaseLockActor1> {
															#line 18332 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<CheckDatabaseLockActor1>::operator new;
	using FastAllocated<CheckDatabaseLockActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckDatabaseLockActor1, 0, Optional<Value> >;
															#line 2211 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	CheckDatabaseLockActor1(Reference<ReadYourWritesTransaction> const& tr,UID const& id) 
															#line 18343 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   CheckDatabaseLockActor1State<CheckDatabaseLockActor1>(tr, id)
	{
		fdb_probe_actor_enter("checkDatabaseLock", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkDatabaseLock");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkDatabaseLock", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckDatabaseLockActor1, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2211 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> checkDatabaseLock( Reference<ReadYourWritesTransaction> const& tr, UID const& id ) {
															#line 2211 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new CheckDatabaseLockActor1(tr, id));
															#line 18371 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2223 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 18376 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via updateChangeFeed()
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class UpdateChangeFeedActor>
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class UpdateChangeFeedActorState {
															#line 18383 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UpdateChangeFeedActorState(Transaction* const& tr,Key const& rangeID,ChangeFeedStatus const& status,KeyRange const& range) 
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr),
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   rangeID(rangeID),
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   status(status),
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   range(range),
															#line 2225 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   rangeIDKey(rangeID.withPrefix(changeFeedPrefix))
															#line 18398 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("updateChangeFeed", reinterpret_cast<unsigned long>(this));

	}
	~UpdateChangeFeedActorState() 
	{
		fdb_probe_actor_destroy("updateChangeFeed", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2226 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 2228 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(rangeIDKey);
															#line 2228 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<UpdateChangeFeedActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 18417 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UpdateChangeFeedActor*>(this)->actor_wait_state = 1;
															#line 2228 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UpdateChangeFeedActor, 0, Optional<Value> >*>(static_cast<UpdateChangeFeedActor*>(this)));
															#line 18422 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~UpdateChangeFeedActorState();
		static_cast<UpdateChangeFeedActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 2229 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (status == ChangeFeedStatus::CHANGE_FEED_CREATE)
															#line 18445 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2230 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!val.present())
															#line 18449 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2231 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr->set(rangeIDKey, changeFeedValue(range, invalidVersion, status));
															#line 18453 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
			else
			{
															#line 2232 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (std::get<0>(decodeChangeFeedValue(val.get())) != range)
															#line 18459 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 2233 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					return a_body1Catch1(unsupported_operation(), loopDepth);
															#line 18463 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
			}
		}
		else
		{
															#line 2235 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (status == ChangeFeedStatus::CHANGE_FEED_STOP)
															#line 18471 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2236 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (val.present())
															#line 18475 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 2237 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					tr->set(rangeIDKey, changeFeedValue(std::get<0>(decodeChangeFeedValue(val.get())), std::get<1>(decodeChangeFeedValue(val.get())), status));
															#line 18479 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
				else
				{
															#line 2242 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					return a_body1Catch1(unsupported_operation(), loopDepth);
															#line 18485 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
			}
			else
			{
															#line 2244 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (status == ChangeFeedStatus::CHANGE_FEED_DESTROY)
															#line 18492 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 2245 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					if (val.present())
															#line 18496 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					{
															#line 2246 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						tr->set(rangeIDKey, changeFeedValue(std::get<0>(decodeChangeFeedValue(val.get())), std::get<1>(decodeChangeFeedValue(val.get())), status));
															#line 2250 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						tr->clear(rangeIDKey);
															#line 18502 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
					else
					{
															#line 2252 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						return a_body1Catch1(unsupported_operation(), loopDepth);
															#line 18508 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
				}
			}
		}
															#line 2255 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<UpdateChangeFeedActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UpdateChangeFeedActorState(); static_cast<UpdateChangeFeedActor*>(this)->destroy(); return 0; }
															#line 18515 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<UpdateChangeFeedActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~UpdateChangeFeedActorState();
		static_cast<UpdateChangeFeedActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 2229 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (status == ChangeFeedStatus::CHANGE_FEED_CREATE)
															#line 18527 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2230 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!val.present())
															#line 18531 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2231 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr->set(rangeIDKey, changeFeedValue(range, invalidVersion, status));
															#line 18535 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
			else
			{
															#line 2232 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (std::get<0>(decodeChangeFeedValue(val.get())) != range)
															#line 18541 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 2233 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					return a_body1Catch1(unsupported_operation(), loopDepth);
															#line 18545 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
			}
		}
		else
		{
															#line 2235 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (status == ChangeFeedStatus::CHANGE_FEED_STOP)
															#line 18553 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2236 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (val.present())
															#line 18557 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 2237 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					tr->set(rangeIDKey, changeFeedValue(std::get<0>(decodeChangeFeedValue(val.get())), std::get<1>(decodeChangeFeedValue(val.get())), status));
															#line 18561 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
				else
				{
															#line 2242 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					return a_body1Catch1(unsupported_operation(), loopDepth);
															#line 18567 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
			}
			else
			{
															#line 2244 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (status == ChangeFeedStatus::CHANGE_FEED_DESTROY)
															#line 18574 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 2245 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					if (val.present())
															#line 18578 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					{
															#line 2246 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						tr->set(rangeIDKey, changeFeedValue(std::get<0>(decodeChangeFeedValue(val.get())), std::get<1>(decodeChangeFeedValue(val.get())), status));
															#line 2250 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						tr->clear(rangeIDKey);
															#line 18584 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
					else
					{
															#line 2252 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						return a_body1Catch1(unsupported_operation(), loopDepth);
															#line 18590 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
				}
			}
		}
															#line 2255 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<UpdateChangeFeedActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UpdateChangeFeedActorState(); static_cast<UpdateChangeFeedActor*>(this)->destroy(); return 0; }
															#line 18597 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<UpdateChangeFeedActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~UpdateChangeFeedActorState();
		static_cast<UpdateChangeFeedActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<UpdateChangeFeedActor*>(this)->actor_wait_state > 0) static_cast<UpdateChangeFeedActor*>(this)->actor_wait_state = 0;
		static_cast<UpdateChangeFeedActor*>(this)->ActorCallback< UpdateChangeFeedActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< UpdateChangeFeedActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UpdateChangeFeedActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< UpdateChangeFeedActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction* tr;
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Key rangeID;
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ChangeFeedStatus status;
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	KeyRange range;
															#line 2225 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Key rangeIDKey;
															#line 18678 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via updateChangeFeed()
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class UpdateChangeFeedActor final : public Actor<Void>, public ActorCallback< UpdateChangeFeedActor, 0, Optional<Value> >, public FastAllocated<UpdateChangeFeedActor>, public UpdateChangeFeedActorState<UpdateChangeFeedActor> {
															#line 18683 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<UpdateChangeFeedActor>::operator new;
	using FastAllocated<UpdateChangeFeedActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UpdateChangeFeedActor, 0, Optional<Value> >;
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UpdateChangeFeedActor(Transaction* const& tr,Key const& rangeID,ChangeFeedStatus const& status,KeyRange const& range) 
															#line 18694 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   UpdateChangeFeedActorState<UpdateChangeFeedActor>(tr, rangeID, status, range)
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("updateChangeFeed");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< UpdateChangeFeedActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> updateChangeFeed( Transaction* const& tr, Key const& rangeID, ChangeFeedStatus const& status, KeyRange const& range ) {
															#line 2224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new UpdateChangeFeedActor(tr, rangeID, status, range));
															#line 18722 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2257 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 18727 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via updateChangeFeed()
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class UpdateChangeFeedActor1>
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class UpdateChangeFeedActor1State {
															#line 18734 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UpdateChangeFeedActor1State(Reference<ReadYourWritesTransaction> const& tr,Key const& rangeID,ChangeFeedStatus const& status,KeyRange const& range) 
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : tr(tr),
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   rangeID(rangeID),
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   status(status),
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   range(range),
															#line 2262 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   rangeIDKey(rangeID.withPrefix(changeFeedPrefix))
															#line 18749 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("updateChangeFeed", reinterpret_cast<unsigned long>(this));

	}
	~UpdateChangeFeedActor1State() 
	{
		fdb_probe_actor_destroy("updateChangeFeed", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2263 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 2265 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(rangeIDKey);
															#line 2265 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<UpdateChangeFeedActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 18768 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UpdateChangeFeedActor1*>(this)->actor_wait_state = 1;
															#line 2265 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UpdateChangeFeedActor1, 0, Optional<Value> >*>(static_cast<UpdateChangeFeedActor1*>(this)));
															#line 18773 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~UpdateChangeFeedActor1State();
		static_cast<UpdateChangeFeedActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 2266 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (status == ChangeFeedStatus::CHANGE_FEED_CREATE)
															#line 18796 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2267 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!val.present())
															#line 18800 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2268 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr->set(rangeIDKey, changeFeedValue(range, invalidVersion, status));
															#line 18804 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
			else
			{
															#line 2269 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (std::get<0>(decodeChangeFeedValue(val.get())) != range)
															#line 18810 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 2270 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					return a_body1Catch1(unsupported_operation(), loopDepth);
															#line 18814 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
			}
		}
		else
		{
															#line 2272 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (status == ChangeFeedStatus::CHANGE_FEED_STOP)
															#line 18822 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2273 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (val.present())
															#line 18826 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 2274 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					tr->set(rangeIDKey, changeFeedValue(std::get<0>(decodeChangeFeedValue(val.get())), std::get<1>(decodeChangeFeedValue(val.get())), status));
															#line 18830 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
				else
				{
															#line 2279 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					return a_body1Catch1(unsupported_operation(), loopDepth);
															#line 18836 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
			}
			else
			{
															#line 2281 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (status == ChangeFeedStatus::CHANGE_FEED_DESTROY)
															#line 18843 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 2282 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					if (val.present())
															#line 18847 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					{
															#line 2283 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						tr->set(rangeIDKey, changeFeedValue(std::get<0>(decodeChangeFeedValue(val.get())), std::get<1>(decodeChangeFeedValue(val.get())), status));
															#line 2287 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						tr->clear(rangeIDKey);
															#line 18853 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
					else
					{
															#line 2289 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						return a_body1Catch1(unsupported_operation(), loopDepth);
															#line 18859 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
				}
			}
		}
															#line 2292 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<UpdateChangeFeedActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UpdateChangeFeedActor1State(); static_cast<UpdateChangeFeedActor1*>(this)->destroy(); return 0; }
															#line 18866 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<UpdateChangeFeedActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~UpdateChangeFeedActor1State();
		static_cast<UpdateChangeFeedActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 2266 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (status == ChangeFeedStatus::CHANGE_FEED_CREATE)
															#line 18878 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2267 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!val.present())
															#line 18882 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2268 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				tr->set(rangeIDKey, changeFeedValue(range, invalidVersion, status));
															#line 18886 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
			else
			{
															#line 2269 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (std::get<0>(decodeChangeFeedValue(val.get())) != range)
															#line 18892 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 2270 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					return a_body1Catch1(unsupported_operation(), loopDepth);
															#line 18896 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
			}
		}
		else
		{
															#line 2272 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (status == ChangeFeedStatus::CHANGE_FEED_STOP)
															#line 18904 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2273 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (val.present())
															#line 18908 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 2274 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					tr->set(rangeIDKey, changeFeedValue(std::get<0>(decodeChangeFeedValue(val.get())), std::get<1>(decodeChangeFeedValue(val.get())), status));
															#line 18912 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
				else
				{
															#line 2279 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					return a_body1Catch1(unsupported_operation(), loopDepth);
															#line 18918 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				}
			}
			else
			{
															#line 2281 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				if (status == ChangeFeedStatus::CHANGE_FEED_DESTROY)
															#line 18925 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
				{
															#line 2282 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
					if (val.present())
															#line 18929 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					{
															#line 2283 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						tr->set(rangeIDKey, changeFeedValue(std::get<0>(decodeChangeFeedValue(val.get())), std::get<1>(decodeChangeFeedValue(val.get())), status));
															#line 2287 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						tr->clear(rangeIDKey);
															#line 18935 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
					else
					{
															#line 2289 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
						return a_body1Catch1(unsupported_operation(), loopDepth);
															#line 18941 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
					}
				}
			}
		}
															#line 2292 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<UpdateChangeFeedActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UpdateChangeFeedActor1State(); static_cast<UpdateChangeFeedActor1*>(this)->destroy(); return 0; }
															#line 18948 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<UpdateChangeFeedActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~UpdateChangeFeedActor1State();
		static_cast<UpdateChangeFeedActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<UpdateChangeFeedActor1*>(this)->actor_wait_state > 0) static_cast<UpdateChangeFeedActor1*>(this)->actor_wait_state = 0;
		static_cast<UpdateChangeFeedActor1*>(this)->ActorCallback< UpdateChangeFeedActor1, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< UpdateChangeFeedActor1, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UpdateChangeFeedActor1, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< UpdateChangeFeedActor1, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Key rangeID;
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ChangeFeedStatus status;
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	KeyRange range;
															#line 2262 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Key rangeIDKey;
															#line 19029 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via updateChangeFeed()
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class UpdateChangeFeedActor1 final : public Actor<Void>, public ActorCallback< UpdateChangeFeedActor1, 0, Optional<Value> >, public FastAllocated<UpdateChangeFeedActor1>, public UpdateChangeFeedActor1State<UpdateChangeFeedActor1> {
															#line 19034 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<UpdateChangeFeedActor1>::operator new;
	using FastAllocated<UpdateChangeFeedActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UpdateChangeFeedActor1, 0, Optional<Value> >;
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UpdateChangeFeedActor1(Reference<ReadYourWritesTransaction> const& tr,Key const& rangeID,ChangeFeedStatus const& status,KeyRange const& range) 
															#line 19045 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   UpdateChangeFeedActor1State<UpdateChangeFeedActor1>(tr, rangeID, status, range)
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("updateChangeFeed");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< UpdateChangeFeedActor1, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> updateChangeFeed( Reference<ReadYourWritesTransaction> const& tr, Key const& rangeID, ChangeFeedStatus const& status, KeyRange const& range ) {
															#line 2258 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new UpdateChangeFeedActor1(tr, rangeID, status, range));
															#line 19073 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 19078 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via updateChangeFeed()
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class UpdateChangeFeedActor2>
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class UpdateChangeFeedActor2State {
															#line 19085 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UpdateChangeFeedActor2State(Database const& cx,Key const& rangeID,ChangeFeedStatus const& status,KeyRange const& range) 
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   rangeID(rangeID),
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   status(status),
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   range(range),
															#line 2296 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 19100 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("updateChangeFeed", reinterpret_cast<unsigned long>(this));

	}
	~UpdateChangeFeedActor2State() 
	{
		fdb_probe_actor_destroy("updateChangeFeed", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2297 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 19115 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~UpdateChangeFeedActor2State();
		static_cast<UpdateChangeFeedActor2*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 2299 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_0 = updateChangeFeed(&tr, rangeID, status, range);
															#line 2299 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<UpdateChangeFeedActor2*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 19148 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UpdateChangeFeedActor2*>(this)->actor_wait_state = 1;
															#line 2299 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UpdateChangeFeedActor2, 0, Void >*>(static_cast<UpdateChangeFeedActor2*>(this)));
															#line 19153 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 2303 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_2 = tr.onError(e);
															#line 2303 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<UpdateChangeFeedActor2*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 19177 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<UpdateChangeFeedActor2*>(this)->actor_wait_state = 3;
															#line 2303 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< UpdateChangeFeedActor2, 2, Void >*>(static_cast<UpdateChangeFeedActor2*>(this)));
															#line 19182 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 2300 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 2300 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<UpdateChangeFeedActor2*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 19199 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<UpdateChangeFeedActor2*>(this)->actor_wait_state = 2;
															#line 2300 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< UpdateChangeFeedActor2, 1, Void >*>(static_cast<UpdateChangeFeedActor2*>(this)));
															#line 19204 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 2300 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 2300 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<UpdateChangeFeedActor2*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 19215 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<UpdateChangeFeedActor2*>(this)->actor_wait_state = 2;
															#line 2300 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< UpdateChangeFeedActor2, 1, Void >*>(static_cast<UpdateChangeFeedActor2*>(this)));
															#line 19220 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		if (static_cast<UpdateChangeFeedActor2*>(this)->actor_wait_state > 0) static_cast<UpdateChangeFeedActor2*>(this)->actor_wait_state = 0;
		static_cast<UpdateChangeFeedActor2*>(this)->ActorCallback< UpdateChangeFeedActor2, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< UpdateChangeFeedActor2, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UpdateChangeFeedActor2, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< UpdateChangeFeedActor2, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 2301 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<UpdateChangeFeedActor2*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UpdateChangeFeedActor2State(); static_cast<UpdateChangeFeedActor2*>(this)->destroy(); return 0; }
															#line 19292 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<UpdateChangeFeedActor2*>(this)->SAV< Void >::value()) Void(Void());
		this->~UpdateChangeFeedActor2State();
		static_cast<UpdateChangeFeedActor2*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 2301 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<UpdateChangeFeedActor2*>(this)->SAV<Void>::futures) { (void)(Void()); this->~UpdateChangeFeedActor2State(); static_cast<UpdateChangeFeedActor2*>(this)->destroy(); return 0; }
															#line 19304 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<UpdateChangeFeedActor2*>(this)->SAV< Void >::value()) Void(Void());
		this->~UpdateChangeFeedActor2State();
		static_cast<UpdateChangeFeedActor2*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<UpdateChangeFeedActor2*>(this)->actor_wait_state > 0) static_cast<UpdateChangeFeedActor2*>(this)->actor_wait_state = 0;
		static_cast<UpdateChangeFeedActor2*>(this)->ActorCallback< UpdateChangeFeedActor2, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< UpdateChangeFeedActor2, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< UpdateChangeFeedActor2, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< UpdateChangeFeedActor2, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<UpdateChangeFeedActor2*>(this)->actor_wait_state > 0) static_cast<UpdateChangeFeedActor2*>(this)->actor_wait_state = 0;
		static_cast<UpdateChangeFeedActor2*>(this)->ActorCallback< UpdateChangeFeedActor2, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< UpdateChangeFeedActor2, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< UpdateChangeFeedActor2, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< UpdateChangeFeedActor2, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Key rangeID;
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ChangeFeedStatus status;
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	KeyRange range;
															#line 2296 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 19460 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via updateChangeFeed()
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class UpdateChangeFeedActor2 final : public Actor<Void>, public ActorCallback< UpdateChangeFeedActor2, 0, Void >, public ActorCallback< UpdateChangeFeedActor2, 1, Void >, public ActorCallback< UpdateChangeFeedActor2, 2, Void >, public FastAllocated<UpdateChangeFeedActor2>, public UpdateChangeFeedActor2State<UpdateChangeFeedActor2> {
															#line 19465 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<UpdateChangeFeedActor2>::operator new;
	using FastAllocated<UpdateChangeFeedActor2>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UpdateChangeFeedActor2, 0, Void >;
friend struct ActorCallback< UpdateChangeFeedActor2, 1, Void >;
friend struct ActorCallback< UpdateChangeFeedActor2, 2, Void >;
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UpdateChangeFeedActor2(Database const& cx,Key const& rangeID,ChangeFeedStatus const& status,KeyRange const& range) 
															#line 19478 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   UpdateChangeFeedActor2State<UpdateChangeFeedActor2>(cx, rangeID, status, range)
	{
		fdb_probe_actor_enter("updateChangeFeed", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("updateChangeFeed");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("updateChangeFeed", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< UpdateChangeFeedActor2, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< UpdateChangeFeedActor2, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< UpdateChangeFeedActor2, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> updateChangeFeed( Database const& cx, Key const& rangeID, ChangeFeedStatus const& status, KeyRange const& range ) {
															#line 2295 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new UpdateChangeFeedActor2(cx, rangeID, status, range));
															#line 19508 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2307 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 19513 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via advanceVersion()
															#line 2308 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class AdvanceVersionActor>
															#line 2308 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class AdvanceVersionActorState {
															#line 19520 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2308 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	AdvanceVersionActorState(Database const& cx,Version const& v) 
															#line 2308 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2308 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 2308 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   v(v),
															#line 2309 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 19531 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("advanceVersion", reinterpret_cast<unsigned long>(this));

	}
	~AdvanceVersionActorState() 
	{
		fdb_probe_actor_destroy("advanceVersion", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2310 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 19546 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~AdvanceVersionActorState();
		static_cast<AdvanceVersionActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 2311 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 2312 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 19578 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		try {
															#line 2314 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Version> __when_expr_0 = tr.getReadVersion();
															#line 2314 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<AdvanceVersionActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 19584 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AdvanceVersionActor*>(this)->actor_wait_state = 1;
															#line 2314 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AdvanceVersionActor, 0, Version >*>(static_cast<AdvanceVersionActor*>(this)));
															#line 19589 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 2323 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_2 = tr.onError(e);
															#line 2323 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<AdvanceVersionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 19613 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<AdvanceVersionActor*>(this)->actor_wait_state = 3;
															#line 2323 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< AdvanceVersionActor, 2, Void >*>(static_cast<AdvanceVersionActor*>(this)));
															#line 19618 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Version const& rv,int loopDepth) 
	{
															#line 2315 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (rv <= v)
															#line 19633 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2316 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.set(minRequiredCommitVersionKey, BinaryWriter::toValue(v + 1, Unversioned()));
															#line 2317 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 2317 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<AdvanceVersionActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 19641 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<AdvanceVersionActor*>(this)->actor_wait_state = 2;
															#line 2317 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AdvanceVersionActor, 1, Void >*>(static_cast<AdvanceVersionActor*>(this)));
															#line 19646 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 2319 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			fmt::print("Current read version is {}\n", rv);
															#line 2320 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<AdvanceVersionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AdvanceVersionActorState(); static_cast<AdvanceVersionActor*>(this)->destroy(); return 0; }
															#line 19655 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<AdvanceVersionActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~AdvanceVersionActorState();
			static_cast<AdvanceVersionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Version && rv,int loopDepth) 
	{
															#line 2315 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (rv <= v)
															#line 19668 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2316 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.set(minRequiredCommitVersionKey, BinaryWriter::toValue(v + 1, Unversioned()));
															#line 2317 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 2317 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<AdvanceVersionActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 19676 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<AdvanceVersionActor*>(this)->actor_wait_state = 2;
															#line 2317 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AdvanceVersionActor, 1, Void >*>(static_cast<AdvanceVersionActor*>(this)));
															#line 19681 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 2319 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			fmt::print("Current read version is {}\n", rv);
															#line 2320 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<AdvanceVersionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AdvanceVersionActorState(); static_cast<AdvanceVersionActor*>(this)->destroy(); return 0; }
															#line 19690 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<AdvanceVersionActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~AdvanceVersionActorState();
			static_cast<AdvanceVersionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(Version const& rv,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(rv, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Version && rv,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(rv), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<AdvanceVersionActor*>(this)->actor_wait_state > 0) static_cast<AdvanceVersionActor*>(this)->actor_wait_state = 0;
		static_cast<AdvanceVersionActor*>(this)->ActorCallback< AdvanceVersionActor, 0, Version >::remove();

	}
	void a_callback_fire(ActorCallback< AdvanceVersionActor, 0, Version >*,Version const& value) 
	{
		fdb_probe_actor_enter("advanceVersion", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("advanceVersion", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AdvanceVersionActor, 0, Version >*,Version && value) 
	{
		fdb_probe_actor_enter("advanceVersion", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("advanceVersion", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AdvanceVersionActor, 0, Version >*,Error err) 
	{
		fdb_probe_actor_enter("advanceVersion", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("advanceVersion", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
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
		if (static_cast<AdvanceVersionActor*>(this)->actor_wait_state > 0) static_cast<AdvanceVersionActor*>(this)->actor_wait_state = 0;
		static_cast<AdvanceVersionActor*>(this)->ActorCallback< AdvanceVersionActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AdvanceVersionActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("advanceVersion", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("advanceVersion", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< AdvanceVersionActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("advanceVersion", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("advanceVersion", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< AdvanceVersionActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("advanceVersion", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("advanceVersion", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<AdvanceVersionActor*>(this)->actor_wait_state > 0) static_cast<AdvanceVersionActor*>(this)->actor_wait_state = 0;
		static_cast<AdvanceVersionActor*>(this)->ActorCallback< AdvanceVersionActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AdvanceVersionActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("advanceVersion", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("advanceVersion", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< AdvanceVersionActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("advanceVersion", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("advanceVersion", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< AdvanceVersionActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("advanceVersion", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("advanceVersion", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 2308 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 2308 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Version v;
															#line 2309 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Transaction tr;
															#line 19937 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via advanceVersion()
															#line 2308 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class AdvanceVersionActor final : public Actor<Void>, public ActorCallback< AdvanceVersionActor, 0, Version >, public ActorCallback< AdvanceVersionActor, 1, Void >, public ActorCallback< AdvanceVersionActor, 2, Void >, public FastAllocated<AdvanceVersionActor>, public AdvanceVersionActorState<AdvanceVersionActor> {
															#line 19942 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<AdvanceVersionActor>::operator new;
	using FastAllocated<AdvanceVersionActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AdvanceVersionActor, 0, Version >;
friend struct ActorCallback< AdvanceVersionActor, 1, Void >;
friend struct ActorCallback< AdvanceVersionActor, 2, Void >;
															#line 2308 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	AdvanceVersionActor(Database const& cx,Version const& v) 
															#line 19955 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   AdvanceVersionActorState<AdvanceVersionActor>(cx, v)
	{
		fdb_probe_actor_enter("advanceVersion", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("advanceVersion");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("advanceVersion", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AdvanceVersionActor, 0, Version >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< AdvanceVersionActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< AdvanceVersionActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2308 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> advanceVersion( Database const& cx, Version const& v ) {
															#line 2308 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new AdvanceVersionActor(cx, v));
															#line 19985 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2327 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 19990 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via forceRecovery()
															#line 2328 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class ForceRecoveryActor>
															#line 2328 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ForceRecoveryActorState {
															#line 19997 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2328 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ForceRecoveryActorState(Reference<IClusterConnectionRecord> const& clusterFile,Key const& dcId) 
															#line 2328 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2328 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : clusterFile(clusterFile),
															#line 2328 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   dcId(dcId),
															#line 2329 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   clusterInterface(new AsyncVar<Optional<ClusterInterface>>),
															#line 2330 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   leaderMon(monitorLeader<ClusterInterface>(clusterFile, clusterInterface))
															#line 20010 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("forceRecovery", reinterpret_cast<unsigned long>(this));

	}
	~ForceRecoveryActorState() 
	{
		fdb_probe_actor_destroy("forceRecovery", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2332 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 20025 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~ForceRecoveryActorState();
		static_cast<ForceRecoveryActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 2334 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_0 = clusterInterface->get().present() ? brokenPromiseToNever( clusterInterface->get().get().forceRecovery.getReply(ForceRecoveryRequest(dcId))) : Never();
															#line 2333 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<ForceRecoveryActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 20057 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 2340 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = clusterInterface->onChange();
															#line 20061 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<ForceRecoveryActor*>(this)->actor_wait_state = 1;
															#line 2334 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ForceRecoveryActor, 0, Void >*>(static_cast<ForceRecoveryActor*>(this)));
															#line 2340 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ForceRecoveryActor, 1, Void >*>(static_cast<ForceRecoveryActor*>(this)));
															#line 20068 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
															#line 2338 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ForceRecoveryActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ForceRecoveryActorState(); static_cast<ForceRecoveryActor*>(this)->destroy(); return 0; }
															#line 20083 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ForceRecoveryActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ForceRecoveryActorState();
		static_cast<ForceRecoveryActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
															#line 2338 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<ForceRecoveryActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ForceRecoveryActorState(); static_cast<ForceRecoveryActor*>(this)->destroy(); return 0; }
															#line 20095 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<ForceRecoveryActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ForceRecoveryActorState();
		static_cast<ForceRecoveryActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<ForceRecoveryActor*>(this)->actor_wait_state > 0) static_cast<ForceRecoveryActor*>(this)->actor_wait_state = 0;
		static_cast<ForceRecoveryActor*>(this)->ActorCallback< ForceRecoveryActor, 0, Void >::remove();
		static_cast<ForceRecoveryActor*>(this)->ActorCallback< ForceRecoveryActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ForceRecoveryActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("forceRecovery", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forceRecovery", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ForceRecoveryActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("forceRecovery", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forceRecovery", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ForceRecoveryActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("forceRecovery", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forceRecovery", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ForceRecoveryActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("forceRecovery", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forceRecovery", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ForceRecoveryActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("forceRecovery", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forceRecovery", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ForceRecoveryActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("forceRecovery", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("forceRecovery", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 2328 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Reference<IClusterConnectionRecord> clusterFile;
															#line 2328 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Key dcId;
															#line 2329 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Reference<AsyncVar<Optional<ClusterInterface>>> clusterInterface;
															#line 2330 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Future<Void> leaderMon;
															#line 20220 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via forceRecovery()
															#line 2328 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class ForceRecoveryActor final : public Actor<Void>, public ActorCallback< ForceRecoveryActor, 0, Void >, public ActorCallback< ForceRecoveryActor, 1, Void >, public FastAllocated<ForceRecoveryActor>, public ForceRecoveryActorState<ForceRecoveryActor> {
															#line 20225 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<ForceRecoveryActor>::operator new;
	using FastAllocated<ForceRecoveryActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ForceRecoveryActor, 0, Void >;
friend struct ActorCallback< ForceRecoveryActor, 1, Void >;
															#line 2328 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ForceRecoveryActor(Reference<IClusterConnectionRecord> const& clusterFile,Key const& dcId) 
															#line 20237 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   ForceRecoveryActorState<ForceRecoveryActor>(clusterFile, dcId)
	{
		fdb_probe_actor_enter("forceRecovery", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("forceRecovery");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("forceRecovery", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ForceRecoveryActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2328 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> forceRecovery( Reference<IClusterConnectionRecord> const& clusterFile, Key const& dcId ) {
															#line 2328 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new ForceRecoveryActor(clusterFile, dcId));
															#line 20265 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2344 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

															#line 20270 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via waitForPrimaryDC()
															#line 2345 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class WaitForPrimaryDCActor>
															#line 2345 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class WaitForPrimaryDCActorState {
															#line 20277 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2345 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	WaitForPrimaryDCActorState(Database const& cx,StringRef const& dcId) 
															#line 2345 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2345 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : cx(cx),
															#line 2345 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   dcId(dcId),
															#line 2346 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		   tr(cx)
															#line 20288 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("waitForPrimaryDC", reinterpret_cast<unsigned long>(this));

	}
	~WaitForPrimaryDCActorState() 
	{
		fdb_probe_actor_destroy("waitForPrimaryDC", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2348 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			;
															#line 20303 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~WaitForPrimaryDCActorState();
		static_cast<WaitForPrimaryDCActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 2350 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 2351 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(primaryDatacenterKey);
															#line 2351 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 20338 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state = 1;
															#line 2351 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WaitForPrimaryDCActor, 0, Optional<Value> >*>(static_cast<WaitForPrimaryDCActor*>(this)));
															#line 20343 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
															#line 2361 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_3 = tr.onError(e);
															#line 2361 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 20367 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state = 4;
															#line 2361 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WaitForPrimaryDCActor, 3, Void >*>(static_cast<WaitForPrimaryDCActor*>(this)));
															#line 20372 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& res,int loopDepth) 
	{
															#line 2352 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (res.present() && res.get() == dcId)
															#line 20387 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2353 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<WaitForPrimaryDCActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitForPrimaryDCActorState(); static_cast<WaitForPrimaryDCActor*>(this)->destroy(); return 0; }
															#line 20391 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<WaitForPrimaryDCActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WaitForPrimaryDCActorState();
			static_cast<WaitForPrimaryDCActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 2356 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		watchFuture = tr.watch(primaryDatacenterKey);
															#line 2357 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 2357 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 20403 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state = 2;
															#line 2357 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitForPrimaryDCActor, 1, Void >*>(static_cast<WaitForPrimaryDCActor*>(this)));
															#line 20408 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && res,int loopDepth) 
	{
															#line 2352 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (res.present() && res.get() == dcId)
															#line 20417 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		{
															#line 2353 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (!static_cast<WaitForPrimaryDCActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WaitForPrimaryDCActorState(); static_cast<WaitForPrimaryDCActor*>(this)->destroy(); return 0; }
															#line 20421 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			new (&static_cast<WaitForPrimaryDCActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WaitForPrimaryDCActorState();
			static_cast<WaitForPrimaryDCActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 2356 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		watchFuture = tr.watch(primaryDatacenterKey);
															#line 2357 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 2357 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 20433 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state = 2;
															#line 2357 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WaitForPrimaryDCActor, 1, Void >*>(static_cast<WaitForPrimaryDCActor*>(this)));
															#line 20438 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& res,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(res, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && res,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(res), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state > 0) static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForPrimaryDCActor*>(this)->ActorCallback< WaitForPrimaryDCActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForPrimaryDCActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WaitForPrimaryDCActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WaitForPrimaryDCActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 2358 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_2 = watchFuture;
															#line 2358 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 20512 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state = 3;
															#line 2358 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WaitForPrimaryDCActor, 2, Void >*>(static_cast<WaitForPrimaryDCActor*>(this)));
															#line 20517 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 2358 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		StrictFuture<Void> __when_expr_2 = watchFuture;
															#line 2358 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 20528 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state = 3;
															#line 2358 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WaitForPrimaryDCActor, 2, Void >*>(static_cast<WaitForPrimaryDCActor*>(this)));
															#line 20533 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		if (static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state > 0) static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForPrimaryDCActor*>(this)->ActorCallback< WaitForPrimaryDCActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForPrimaryDCActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WaitForPrimaryDCActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WaitForPrimaryDCActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont5(Void const& _,int loopDepth) 
	{
															#line 2359 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.reset();
															#line 20605 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5(Void && _,int loopDepth) 
	{
															#line 2359 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		tr.reset();
															#line 20614 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		if (static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state > 0) static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForPrimaryDCActor*>(this)->ActorCallback< WaitForPrimaryDCActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForPrimaryDCActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< WaitForPrimaryDCActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< WaitForPrimaryDCActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 2);

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
	void a_exitChoose4() 
	{
		if (static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state > 0) static_cast<WaitForPrimaryDCActor*>(this)->actor_wait_state = 0;
		static_cast<WaitForPrimaryDCActor*>(this)->ActorCallback< WaitForPrimaryDCActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WaitForPrimaryDCActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< WaitForPrimaryDCActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< WaitForPrimaryDCActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 2345 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Database cx;
															#line 2345 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	StringRef dcId;
															#line 2346 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	ReadYourWritesTransaction tr;
															#line 2356 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	Future<Void> watchFuture;
															#line 20778 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via waitForPrimaryDC()
															#line 2345 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class WaitForPrimaryDCActor final : public Actor<Void>, public ActorCallback< WaitForPrimaryDCActor, 0, Optional<Value> >, public ActorCallback< WaitForPrimaryDCActor, 1, Void >, public ActorCallback< WaitForPrimaryDCActor, 2, Void >, public ActorCallback< WaitForPrimaryDCActor, 3, Void >, public FastAllocated<WaitForPrimaryDCActor>, public WaitForPrimaryDCActorState<WaitForPrimaryDCActor> {
															#line 20783 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<WaitForPrimaryDCActor>::operator new;
	using FastAllocated<WaitForPrimaryDCActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WaitForPrimaryDCActor, 0, Optional<Value> >;
friend struct ActorCallback< WaitForPrimaryDCActor, 1, Void >;
friend struct ActorCallback< WaitForPrimaryDCActor, 2, Void >;
friend struct ActorCallback< WaitForPrimaryDCActor, 3, Void >;
															#line 2345 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	WaitForPrimaryDCActor(Database const& cx,StringRef const& dcId) 
															#line 20797 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   WaitForPrimaryDCActorState<WaitForPrimaryDCActor>(cx, dcId)
	{
		fdb_probe_actor_enter("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("waitForPrimaryDC");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("waitForPrimaryDC", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WaitForPrimaryDCActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WaitForPrimaryDCActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< WaitForPrimaryDCActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< WaitForPrimaryDCActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2345 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
[[nodiscard]] Future<Void> waitForPrimaryDC( Database const& cx, StringRef const& dcId ) {
															#line 2345 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new WaitForPrimaryDCActor(cx, dcId));
															#line 20828 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}

#line 2365 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"

json_spirit::Value_type normJSONType(json_spirit::Value_type type) {
	if (type == json_spirit::int_type)
		return json_spirit::real_type;
	return type;
}

void schemaCoverage(std::string const& spath, bool covered) {
	static std::map<bool, std::set<std::string>> coveredSchemaPaths;

	if (coveredSchemaPaths[covered].insert(spath).second) {
		TraceEvent ev(SevInfo, "CodeCoverage");
		ev.detail("File", "documentation/StatusSchema.json/" + spath).detail("Line", 0);
		if (!covered)
			ev.detail("Covered", 0);
	}
}

bool schemaMatch(json_spirit::mValue const& schemaValue,
                 json_spirit::mValue const& resultValue,
                 std::string& errorStr,
                 Severity sev,
                 bool checkCoverage,
                 std::string path,
                 std::string schemaPath) {
	// Returns true if everything in `result` is permitted by `schema`
	bool ok = true;

	try {
		if (normJSONType(schemaValue.type()) != normJSONType(resultValue.type())) {
			errorStr += format("ERROR: Incorrect value type for key `%s'\n", path.c_str());
			TraceEvent(sev, "SchemaMismatch")
			    .detail("Path", path)
			    .detail("SchemaType", schemaValue.type())
			    .detail("ValueType", resultValue.type());
			return false;
		}

		if (resultValue.type() == json_spirit::obj_type) {
			auto& result = resultValue.get_obj();
			auto& schema = schemaValue.get_obj();

			for (auto& rkv : result) {
				auto& key = rkv.first;
				auto& rv = rkv.second;
				std::string kpath = path + "." + key;
				std::string spath = schemaPath + "." + key;

				if (checkCoverage) {
					schemaCoverage(spath);
				}

				if (!schema.count(key)) {
					errorStr += format("ERROR: Unknown key `%s'\n", kpath.c_str());
					TraceEvent(sev, "SchemaMismatch").detail("Path", kpath).detail("SchemaPath", spath);
					ok = false;
					continue;
				}
				auto& sv = schema.at(key);

				if (sv.type() == json_spirit::obj_type && sv.get_obj().count("$enum")) {
					auto& enum_values = sv.get_obj().at("$enum").get_array();

					bool any_match = false;
					for (auto& enum_item : enum_values)
						if (enum_item == rv) {
							any_match = true;
							if (checkCoverage) {
								schemaCoverage(spath + ".$enum." + enum_item.get_str());
							}
							break;
						}
					if (!any_match) {
						errorStr += format("ERROR: Unknown value `%s' for key `%s'\n",
						                   json_spirit::write_string(rv).c_str(),
						                   kpath.c_str());
						TraceEvent(sev, "SchemaMismatch")
						    .detail("Path", kpath)
						    .detail("SchemaEnumItems", enum_values.size())
						    .detail("Value", json_spirit::write_string(rv));
						if (checkCoverage) {
							schemaCoverage(spath + ".$enum." + json_spirit::write_string(rv));
						}
						ok = false;
					}
				} else if (sv.type() == json_spirit::obj_type && sv.get_obj().count("$map")) {
					if (rv.type() != json_spirit::obj_type) {
						errorStr += format("ERROR: Expected an object as the value for key `%s'\n", kpath.c_str());
						TraceEvent(sev, "SchemaMismatch")
						    .detail("Path", kpath)
						    .detail("SchemaType", sv.type())
						    .detail("ValueType", rv.type());
						ok = false;
						continue;
					}
					if (sv.get_obj().at("$map").type() != json_spirit::obj_type) {
						continue;
					}
					auto& schemaVal = sv.get_obj().at("$map");
					auto& valueObj = rv.get_obj();

					if (checkCoverage) {
						schemaCoverage(spath + ".$map");
					}

					for (auto& valuePair : valueObj) {
						auto vpath = kpath + "[" + valuePair.first + "]";
						auto upath = spath + ".$map";
						if (valuePair.second.type() != json_spirit::obj_type) {
							errorStr += format("ERROR: Expected an object for `%s'\n", vpath.c_str());
							TraceEvent(sev, "SchemaMismatch")
							    .detail("Path", vpath)
							    .detail("ValueType", valuePair.second.type());
							ok = false;
							continue;
						}
						if (!schemaMatch(schemaVal, valuePair.second, errorStr, sev, checkCoverage, vpath, upath)) {
							ok = false;
						}
					}
				} else {
					if (!schemaMatch(sv, rv, errorStr, sev, checkCoverage, kpath, spath)) {
						ok = false;
					}
				}
			}
		} else if (resultValue.type() == json_spirit::array_type) {
			auto& valueArray = resultValue.get_array();
			auto& schemaArray = schemaValue.get_array();
			if (!schemaArray.size()) {
				// An empty schema array means that the value array is required to be empty
				if (valueArray.size()) {
					errorStr += format("ERROR: Expected an empty array for key `%s'\n", path.c_str());
					TraceEvent(sev, "SchemaMismatch")
					    .detail("Path", path)
					    .detail("SchemaSize", schemaArray.size())
					    .detail("ValueSize", valueArray.size());
					return false;
				}
			} else if (schemaArray.size() == 1) {
				// A one item schema array means that all items in the value must match the first item in the schema
				int index = 0;
				for (auto& valueItem : valueArray) {
					if (!schemaMatch(schemaArray[0],
					                 valueItem,
					                 errorStr,
					                 sev,
					                 checkCoverage,
					                 path + format("[%d]", index),
					                 schemaPath + "[0]")) {
						ok = false;
					}
					index++;
				}
			} else {
				ASSERT(false); // Schema doesn't make sense
			}
		}
		return ok;
	} catch (std::exception& e) {
		TraceEvent(SevError, "SchemaMatchException")
		    .detail("What", e.what())
		    .detail("Path", path)
		    .detail("SchemaPath", schemaPath);
		throw unknown_error();
	}
}

std::string ManagementAPI::generateErrorMessage(const CoordinatorsResult& res) {
	// Note: the error message here should not be changed if possible
	// If you do change the message here,
	// please update the corresponding fdbcli code to support both the old and the new message

	std::string msg;
	switch (res) {
	case CoordinatorsResult::INVALID_NETWORK_ADDRESSES:
		msg = "The specified network addresses are invalid";
		break;
	case CoordinatorsResult::SAME_NETWORK_ADDRESSES:
		msg = "No change (existing configuration satisfies request)";
		break;
	case CoordinatorsResult::NOT_COORDINATORS:
		msg = "Coordination servers are not running on the specified network addresses";
		break;
	case CoordinatorsResult::DATABASE_UNREACHABLE:
		msg = "Database unreachable";
		break;
	case CoordinatorsResult::BAD_DATABASE_STATE:
		msg = "The database is in an unexpected state from which changing coordinators might be unsafe";
		break;
	case CoordinatorsResult::COORDINATOR_UNREACHABLE:
		msg = "One of the specified coordinators is unreachable";
		break;
	case CoordinatorsResult::NOT_ENOUGH_MACHINES:
		msg = "Too few fdbserver machines to provide coordination at the current redundancy level";
		break;
	default:
		break;
	}
	return msg;
}

															#line 21034 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase2567()
															#line 2567 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
template <class FlowTestCase2567Actor>
															#line 2567 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class FlowTestCase2567ActorState {
															#line 21041 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
															#line 2567 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	FlowTestCase2567ActorState(UnitTestParameters const& params) 
															#line 2567 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
															#line 2567 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		 : params(params)
															#line 21048 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase2567", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase2567ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase2567", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2568 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			StrictFuture<Void> __when_expr_0 = Future<Void>(Void());
															#line 2568 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (static_cast<FlowTestCase2567Actor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 21065 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<FlowTestCase2567Actor*>(this)->actor_wait_state = 1;
															#line 2568 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< FlowTestCase2567Actor, 0, Void >*>(static_cast<FlowTestCase2567Actor*>(this)));
															#line 21070 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
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
		this->~FlowTestCase2567ActorState();
		static_cast<FlowTestCase2567Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 2570 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::vector<ProcessData> workers;
															#line 2571 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::vector<NetworkAddress> chosen;
															#line 2572 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::set<AddressExclusion> excluded;
															#line 2573 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		AutoQuorumChange change(5);
															#line 2575 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(int i = 0;i < 10;i++) {
															#line 2576 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ProcessData data;
															#line 2577 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto dataCenter = std::to_string(i / 4 % 2);
															#line 2578 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto dataHall = dataCenter + std::to_string(i / 2 % 2);
															#line 2579 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto rack = dataHall + std::to_string(i % 2);
															#line 2580 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto machineId = rack + std::to_string(i);
															#line 2581 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			data.locality.set(LiteralStringRef("dcid"), StringRef(dataCenter));
															#line 2582 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			data.locality.set(LiteralStringRef("data_hall"), StringRef(dataHall));
															#line 2583 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			data.locality.set(LiteralStringRef("rack"), StringRef(rack));
															#line 2584 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			data.locality.set(LiteralStringRef("zoneid"), StringRef(rack));
															#line 2585 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			data.locality.set(LiteralStringRef("machineid"), StringRef(machineId));
															#line 2586 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			data.address.ip = IPAddress(i);
															#line 2588 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (g_network->isSimulated())
															#line 21125 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2589 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				g_simulator.newProcess("TestCoordinator", data.address.ip, data.address.port, false, 1, data.locality, ProcessClass(ProcessClass::CoordinatorClass, ProcessClass::CommandLineSource), "", "", currentProtocolVersion);
															#line 21129 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 2601 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			workers.push_back(data);
															#line 21133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 2604 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		auto noAssignIndex = deterministicRandom()->randomInt(0, workers.size());
															#line 2605 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		workers[noAssignIndex].processClass._class = ProcessClass::CoordinatorClass;
															#line 2607 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		change.addDesiredWorkers(chosen, workers, 5, excluded);
															#line 2608 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::map<StringRef, std::set<StringRef>> chosenValues;
															#line 2610 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(chosen.size() == 5);
															#line 2611 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::vector<StringRef> fields({ LiteralStringRef("dcid"), LiteralStringRef("data_hall"), LiteralStringRef("zoneid"), LiteralStringRef("machineid") });
															#line 2615 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(auto worker = chosen.begin();worker != chosen.end();worker++) {
															#line 2616 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ASSERT(worker->ip.toV4() < workers.size());
															#line 2617 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			LocalityData data = workers[worker->ip.toV4()].locality;
															#line 2618 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			for(auto field = fields.begin();field != fields.end();field++) {
															#line 2619 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				chosenValues[*field].insert(data.get(*field).get());
															#line 21157 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 2623 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(chosenValues[LiteralStringRef("dcid")].size() == 2);
															#line 2624 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(chosenValues[LiteralStringRef("data_hall")].size() == 4);
															#line 2625 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(chosenValues[LiteralStringRef("zoneid")].size() == 5);
															#line 2626 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(chosenValues[LiteralStringRef("machineid")].size() == 5);
															#line 2627 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(std::find(chosen.begin(), chosen.end(), workers[noAssignIndex].address) != chosen.end());
															#line 2629 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<FlowTestCase2567Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase2567ActorState(); static_cast<FlowTestCase2567Actor*>(this)->destroy(); return 0; }
															#line 21172 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<FlowTestCase2567Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase2567ActorState();
		static_cast<FlowTestCase2567Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 2570 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::vector<ProcessData> workers;
															#line 2571 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::vector<NetworkAddress> chosen;
															#line 2572 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::set<AddressExclusion> excluded;
															#line 2573 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		AutoQuorumChange change(5);
															#line 2575 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(int i = 0;i < 10;i++) {
															#line 2576 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ProcessData data;
															#line 2577 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto dataCenter = std::to_string(i / 4 % 2);
															#line 2578 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto dataHall = dataCenter + std::to_string(i / 2 % 2);
															#line 2579 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto rack = dataHall + std::to_string(i % 2);
															#line 2580 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			auto machineId = rack + std::to_string(i);
															#line 2581 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			data.locality.set(LiteralStringRef("dcid"), StringRef(dataCenter));
															#line 2582 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			data.locality.set(LiteralStringRef("data_hall"), StringRef(dataHall));
															#line 2583 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			data.locality.set(LiteralStringRef("rack"), StringRef(rack));
															#line 2584 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			data.locality.set(LiteralStringRef("zoneid"), StringRef(rack));
															#line 2585 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			data.locality.set(LiteralStringRef("machineid"), StringRef(machineId));
															#line 2586 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			data.address.ip = IPAddress(i);
															#line 2588 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			if (g_network->isSimulated())
															#line 21216 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			{
															#line 2589 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				g_simulator.newProcess("TestCoordinator", data.address.ip, data.address.port, false, 1, data.locality, ProcessClass(ProcessClass::CoordinatorClass, ProcessClass::CommandLineSource), "", "", currentProtocolVersion);
															#line 21220 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
															#line 2601 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			workers.push_back(data);
															#line 21224 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		}
															#line 2604 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		auto noAssignIndex = deterministicRandom()->randomInt(0, workers.size());
															#line 2605 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		workers[noAssignIndex].processClass._class = ProcessClass::CoordinatorClass;
															#line 2607 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		change.addDesiredWorkers(chosen, workers, 5, excluded);
															#line 2608 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::map<StringRef, std::set<StringRef>> chosenValues;
															#line 2610 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(chosen.size() == 5);
															#line 2611 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		std::vector<StringRef> fields({ LiteralStringRef("dcid"), LiteralStringRef("data_hall"), LiteralStringRef("zoneid"), LiteralStringRef("machineid") });
															#line 2615 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		for(auto worker = chosen.begin();worker != chosen.end();worker++) {
															#line 2616 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			ASSERT(worker->ip.toV4() < workers.size());
															#line 2617 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			LocalityData data = workers[worker->ip.toV4()].locality;
															#line 2618 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
			for(auto field = fields.begin();field != fields.end();field++) {
															#line 2619 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
				chosenValues[*field].insert(data.get(*field).get());
															#line 21248 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
			}
		}
															#line 2623 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(chosenValues[LiteralStringRef("dcid")].size() == 2);
															#line 2624 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(chosenValues[LiteralStringRef("data_hall")].size() == 4);
															#line 2625 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(chosenValues[LiteralStringRef("zoneid")].size() == 5);
															#line 2626 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(chosenValues[LiteralStringRef("machineid")].size() == 5);
															#line 2627 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		ASSERT(std::find(chosen.begin(), chosen.end(), workers[noAssignIndex].address) != chosen.end());
															#line 2629 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
		if (!static_cast<FlowTestCase2567Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase2567ActorState(); static_cast<FlowTestCase2567Actor*>(this)->destroy(); return 0; }
															#line 21263 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		new (&static_cast<FlowTestCase2567Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase2567ActorState();
		static_cast<FlowTestCase2567Actor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<FlowTestCase2567Actor*>(this)->actor_wait_state > 0) static_cast<FlowTestCase2567Actor*>(this)->actor_wait_state = 0;
		static_cast<FlowTestCase2567Actor*>(this)->ActorCallback< FlowTestCase2567Actor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< FlowTestCase2567Actor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("flowTestCase2567", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase2567", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< FlowTestCase2567Actor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("flowTestCase2567", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase2567", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< FlowTestCase2567Actor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("flowTestCase2567", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("flowTestCase2567", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2567 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	UnitTestParameters params;
															#line 21336 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase2567()
															#line 2567 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
class FlowTestCase2567Actor final : public Actor<Void>, public ActorCallback< FlowTestCase2567Actor, 0, Void >, public FastAllocated<FlowTestCase2567Actor>, public FlowTestCase2567ActorState<FlowTestCase2567Actor> {
															#line 21341 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase2567Actor>::operator new;
	using FastAllocated<FlowTestCase2567Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< FlowTestCase2567Actor, 0, Void >;
															#line 2567 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	FlowTestCase2567Actor(UnitTestParameters const& params) 
															#line 21352 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase2567ActorState<FlowTestCase2567Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase2567", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase2567");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase2567", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< FlowTestCase2567Actor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2567 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
static Future<Void> flowTestCase2567( UnitTestParameters const& params ) {
															#line 2567 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
	return Future<Void>(new FlowTestCase2567Actor(params));
															#line 21380 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase2567, "/ManagementAPI/AutoQuorumChange/checkLocality")

#line 2631 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.cpp"
