#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
/*
 * SpecialKeySpace.actor.cpp
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

#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"

#include <string>
#include <time.h>
#include <msgpack.hpp>

#include <exception>
#include <unordered_set>

#include "fdbclient/ActorLineageProfiler.h"
#include "fdbclient/ClusterConnectionMemoryRecord.h"
#include "fdbclient/FDBOptions.g.h"
#include "fdbclient/Knobs.h"
#include "fdbclient/ProcessInterface.h"
#include "fdbclient/GlobalConfig.actor.h"
#include "fdbclient/SpecialKeySpace.actor.h"
#include "flow/Arena.h"
#include "flow/UnitTest.h"
#include "fdbclient/ManagementAPI.actor.h"
#include "fdbclient/StatusClient.h"
#include "flow/actorcompiler.h" // This must be the last #include.

namespace {
const std::string kTracingTransactionIdKey = "transaction_id";
const std::string kTracingTokenKey = "token";

static bool isAlphaNumeric(const std::string& key) {
	// [A-Za-z0-9_]+
	if (!key.size())
		return false;
	for (const char& c : key) {
		if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'))
			return false;
	}
	return true;
}
} // namespace

std::unordered_map<SpecialKeySpace::MODULE, KeyRange> SpecialKeySpace::moduleToBoundary = {
	{ SpecialKeySpace::MODULE::TRANSACTION, KeyRangeRef("\xff\xff/transaction/"_sr, "\xff\xff/transaction0"_sr) },
	{ SpecialKeySpace::MODULE::WORKERINTERFACE,
	  KeyRangeRef("\xff\xff/worker_interfaces/"_sr, "\xff\xff/worker_interfaces0"_sr) },
	{ SpecialKeySpace::MODULE::STATUSJSON, singleKeyRange("\xff\xff/status/json"_sr) },
	{ SpecialKeySpace::MODULE::CONNECTIONSTRING, singleKeyRange("\xff\xff/connection_string"_sr) },
	{ SpecialKeySpace::MODULE::CLUSTERFILEPATH, singleKeyRange("\xff\xff/cluster_file_path"_sr) },
	{ SpecialKeySpace::MODULE::METRICS, KeyRangeRef("\xff\xff/metrics/"_sr, "\xff\xff/metrics0"_sr) },
	{ SpecialKeySpace::MODULE::MANAGEMENT, KeyRangeRef("\xff\xff/management/"_sr, "\xff\xff/management0"_sr) },
	{ SpecialKeySpace::MODULE::ERRORMSG, singleKeyRange("\xff\xff/error_message"_sr) },
	{ SpecialKeySpace::MODULE::CONFIGURATION, KeyRangeRef("\xff\xff/configuration/"_sr, "\xff\xff/configuration0"_sr) },
	{ SpecialKeySpace::MODULE::GLOBALCONFIG, KeyRangeRef("\xff\xff/global_config/"_sr, "\xff\xff/global_config0"_sr) },
	{ SpecialKeySpace::MODULE::TRACING, KeyRangeRef("\xff\xff/tracing/"_sr, "\xff\xff/tracing0"_sr) },
	{ SpecialKeySpace::MODULE::ACTORLINEAGE, KeyRangeRef("\xff\xff/actor_lineage/"_sr, "\xff\xff/actor_lineage0"_sr) },
	{ SpecialKeySpace::MODULE::ACTOR_PROFILER_CONF,
	  KeyRangeRef("\xff\xff/actor_profiler_conf/"_sr, "\xff\xff/actor_profiler_conf0"_sr) },
	{ SpecialKeySpace::MODULE::CLUSTERID, singleKeyRange("\xff\xff/cluster_id"_sr) },
};

std::unordered_map<std::string, KeyRange> SpecialKeySpace::managementApiCommandToRange = {
	{ "exclude", KeyRangeRef("excluded/"_sr, "excluded0"_sr).withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "failed", KeyRangeRef("failed/"_sr, "failed0"_sr).withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "excludedlocality",
	  KeyRangeRef("excluded_locality/"_sr, "excluded_locality0"_sr)
	      .withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "failedlocality",
	  KeyRangeRef("failed_locality/"_sr, "failed_locality0"_sr)
	      .withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "lock", singleKeyRange("db_locked"_sr).withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "consistencycheck",
	  singleKeyRange("consistency_check_suspended"_sr).withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "coordinators",
	  KeyRangeRef("coordinators/"_sr, "coordinators0"_sr).withPrefix(moduleToBoundary[MODULE::CONFIGURATION].begin) },
	{ "advanceversion",
	  singleKeyRange("min_required_commit_version"_sr).withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "versionepoch", singleKeyRange("version_epoch"_sr).withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "profile", KeyRangeRef("profiling/"_sr, "profiling0"_sr).withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "maintenance",
	  KeyRangeRef("maintenance/"_sr, "maintenance0"_sr).withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "datadistribution",
	  KeyRangeRef("data_distribution/"_sr, "data_distribution0"_sr)
	      .withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "tenant", KeyRangeRef("tenant/"_sr, "tenant0"_sr).withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "tenantmap",
	  KeyRangeRef("tenant_map/"_sr, "tenant_map0"_sr).withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) }
};

std::unordered_map<std::string, KeyRange> SpecialKeySpace::actorLineageApiCommandToRange = {
	{ "state", KeyRangeRef("state/"_sr, "state0"_sr).withPrefix(moduleToBoundary[MODULE::ACTORLINEAGE].begin) },
	{ "time", KeyRangeRef("time/"_sr, "time0"_sr).withPrefix(moduleToBoundary[MODULE::ACTORLINEAGE].begin) }
};

std::set<std::string> SpecialKeySpace::options = { "excluded/force",
	                                               "failed/force",
	                                               "excluded_locality/force",
	                                               "failed_locality/force",
	                                               "worker_interfaces/verify" };

std::set<std::string> SpecialKeySpace::tracingOptions = { kTracingTransactionIdKey, kTracingTokenKey };

RangeResult rywGetRange(ReadYourWritesTransaction* ryw, const KeyRangeRef& kr, const RangeResult& res);

// This function will move the given KeySelector as far as possible to the standard form:
// orEqual == false && offset == 1 (Standard form)
// If the corresponding key is not in the underlying key range, it will move over the range
// The cache object is used to cache the first read result from the rpc call during the key resolution,
// then when we need to do key resolution or result filtering,
// we, instead of rpc call, read from this cache object have consistent results
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via moveKeySelectorOverRangeActor()
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class MoveKeySelectorOverRangeActorActor>
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class MoveKeySelectorOverRangeActorActorState {
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	MoveKeySelectorOverRangeActorActorState(const SpecialKeyRangeReadImpl* const& skrImpl,ReadYourWritesTransaction* const& ryw,KeySelector* const& ks,KeyRangeMap<Optional<RangeResult>>* const& cache) 
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : skrImpl(skrImpl),
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   ryw(ryw),
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   ks(ks),
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   cache(cache)
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this));

	}
	~MoveKeySelectorOverRangeActorActorState() 
	{
		fdb_probe_actor_destroy("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(!ks->orEqual);
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(ks->offset != 1);
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (ryw->getTenant().present() && !skrImpl->supportsTenants())
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				return a_body1Catch1(illegal_tenant_access(), loopDepth);
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			startKey = Key(skrImpl->getKeyRange().begin);
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			endKey = Key(skrImpl->getKeyRange().end);
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result = RangeResult();
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (ks->offset < 1)
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (skrImpl->getKeyRange().contains(ks->getKey()))
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					endKey = ks->getKey();
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
			else
			{
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (skrImpl->getKeyRange().contains(ks->getKey()))
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					startKey = ks->getKey();
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(startKey < endKey);
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			DisabledTraceEvent(SevDebug, "NormalizeKeySelector") .detail("OriginalKey", ks->getKey()) .detail("OriginalOffset", ks->offset) .detail("SpecialKeyRangeStart", skrImpl->getKeyRange().begin) .detail("SpecialKeyRangeEnd", skrImpl->getKeyRange().end);
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			GetRangeLimits limitsHint(ks->offset >= 1 ? ks->offset : 1 - ks->offset);
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (skrImpl->isAsync())
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				const SpecialKeyRangeAsyncImpl* ptr = dynamic_cast<const SpecialKeyRangeAsyncImpl*>(skrImpl);
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				StrictFuture<RangeResult> __when_expr_0 = ptr->getRange(ryw, KeyRangeRef(startKey, endKey), limitsHint, cache);
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (static_cast<MoveKeySelectorOverRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<MoveKeySelectorOverRangeActorActor*>(this)->actor_wait_state = 1;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MoveKeySelectorOverRangeActorActor, 0, RangeResult >*>(static_cast<MoveKeySelectorOverRangeActorActor*>(this)));
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				StrictFuture<RangeResult> __when_expr_1 = skrImpl->getRange(ryw, KeyRangeRef(startKey, endKey), limitsHint);
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (static_cast<MoveKeySelectorOverRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
				static_cast<MoveKeySelectorOverRangeActorActor*>(this)->actor_wait_state = 2;
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MoveKeySelectorOverRangeActorActor, 1, RangeResult >*>(static_cast<MoveKeySelectorOverRangeActorActor*>(this)));
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~MoveKeySelectorOverRangeActorActorState();
		static_cast<MoveKeySelectorOverRangeActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (result.size() == 0)
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			TraceEvent(SevDebug, "ZeroElementsIntheRange").detail("Start", startKey).detail("End", endKey);
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<MoveKeySelectorOverRangeActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MoveKeySelectorOverRangeActorActorState(); static_cast<MoveKeySelectorOverRangeActorActor*>(this)->destroy(); return 0; }
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<MoveKeySelectorOverRangeActorActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~MoveKeySelectorOverRangeActorActorState();
			static_cast<MoveKeySelectorOverRangeActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (ks->offset < 1)
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (result.size() >= 1 - ks->offset)
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ks->setKey(KeyRef(ks->arena(), result[result.size() - (1 - ks->offset)].key));
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ks->offset = 1;
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ks->setKey(KeyRef(ks->arena(), result[0].key));
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ks->offset += result.size();
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
		else
		{
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (result.size() >= ks->offset)
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ks->setKey(KeyRef(ks->arena(), result[ks->offset - 1].key));
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ks->offset = 1;
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ks->setKey(KeyRef(ks->arena(), keyAfter(result[result.size() - 1].key)));
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ks->offset -= result.size();
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		DisabledTraceEvent(SevDebug, "NormalizeKeySelector") .detail("NormalizedKey", ks->getKey()) .detail("NormalizedOffset", ks->offset) .detail("SpecialKeyRangeStart", skrImpl->getKeyRange().begin) .detail("SpecialKeyRangeEnd", skrImpl->getKeyRange().end);
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<MoveKeySelectorOverRangeActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MoveKeySelectorOverRangeActorActorState(); static_cast<MoveKeySelectorOverRangeActorActor*>(this)->destroy(); return 0; }
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<MoveKeySelectorOverRangeActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~MoveKeySelectorOverRangeActorActorState();
		static_cast<MoveKeySelectorOverRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(RangeResult const& result_,int loopDepth) 
	{
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		result = result_;
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(RangeResult && result_,int loopDepth) 
	{
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		result = result_;
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult const& result_,int loopDepth) 
	{
		loopDepth = a_body1cont7(result_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && result_,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(result_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<MoveKeySelectorOverRangeActorActor*>(this)->actor_wait_state > 0) static_cast<MoveKeySelectorOverRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<MoveKeySelectorOverRangeActorActor*>(this)->ActorCallback< MoveKeySelectorOverRangeActorActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< MoveKeySelectorOverRangeActorActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MoveKeySelectorOverRangeActorActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MoveKeySelectorOverRangeActorActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont9(RangeResult const& result_,int loopDepth) 
	{
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		result = result_;
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont9(RangeResult && result_,int loopDepth) 
	{
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		result = result_;
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when2(RangeResult const& result_,int loopDepth) 
	{
		loopDepth = a_body1cont9(result_, loopDepth);

		return loopDepth;
	}
	int a_body1when2(RangeResult && result_,int loopDepth) 
	{
		loopDepth = a_body1cont9(std::move(result_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<MoveKeySelectorOverRangeActorActor*>(this)->actor_wait_state > 0) static_cast<MoveKeySelectorOverRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<MoveKeySelectorOverRangeActorActor*>(this)->ActorCallback< MoveKeySelectorOverRangeActorActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< MoveKeySelectorOverRangeActorActor, 1, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< MoveKeySelectorOverRangeActorActor, 1, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< MoveKeySelectorOverRangeActorActor, 1, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	const SpecialKeyRangeReadImpl* skrImpl;
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector* ks;
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeMap<Optional<RangeResult>>* cache;
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Key startKey;
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Key endKey;
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult result;
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via moveKeySelectorOverRangeActor()
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class MoveKeySelectorOverRangeActorActor final : public Actor<Void>, public ActorCallback< MoveKeySelectorOverRangeActorActor, 0, RangeResult >, public ActorCallback< MoveKeySelectorOverRangeActorActor, 1, RangeResult >, public FastAllocated<MoveKeySelectorOverRangeActorActor>, public MoveKeySelectorOverRangeActorActorState<MoveKeySelectorOverRangeActorActor> {
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<MoveKeySelectorOverRangeActorActor>::operator new;
	using FastAllocated<MoveKeySelectorOverRangeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MoveKeySelectorOverRangeActorActor, 0, RangeResult >;
friend struct ActorCallback< MoveKeySelectorOverRangeActorActor, 1, RangeResult >;
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	MoveKeySelectorOverRangeActorActor(const SpecialKeyRangeReadImpl* const& skrImpl,ReadYourWritesTransaction* const& ryw,KeySelector* const& ks,KeyRangeMap<Optional<RangeResult>>* const& cache) 
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<Void>(),
		   MoveKeySelectorOverRangeActorActorState<MoveKeySelectorOverRangeActorActor>(skrImpl, ryw, ks, cache)
	{
		fdb_probe_actor_enter("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("moveKeySelectorOverRangeActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("moveKeySelectorOverRangeActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MoveKeySelectorOverRangeActorActor, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< MoveKeySelectorOverRangeActorActor, 1, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Void> moveKeySelectorOverRangeActor( const SpecialKeyRangeReadImpl* const& skrImpl, ReadYourWritesTransaction* const& ryw, KeySelector* const& ks, KeyRangeMap<Optional<RangeResult>>* const& cache ) {
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Void>(new MoveKeySelectorOverRangeActorActor(skrImpl, ryw, ks, cache));
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

// This function will normalize the given KeySelector to a standard KeySelector:
// orEqual == false && offset == 1 (Standard form)
// If the corresponding key is outside the whole space, it will move to the begin or the end
// It does have overhead here since we query all keys twice in the worst case.
// However, moving the KeySelector while handling other parameters like limits makes the code much more complex and hard
// to maintain; Thus, separate each part to make the code easy to understand and more compact
// Boundary is the range of the legal key space, which, by default is the range of the module
// And (\xff\xff, \xff\xff\xff) if SPECIAL_KEY_SPACE_RELAXED is turned on
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via normalizeKeySelectorActor()
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class NormalizeKeySelectorActorActor>
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class NormalizeKeySelectorActorActorState {
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	NormalizeKeySelectorActorActorState(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeySelector* const& ks,KeyRangeRef const& boundary,int* const& actualOffset,RangeResult* const& result,KeyRangeMap<Optional<RangeResult>>* const& cache) 
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : sks(sks),
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   ryw(ryw),
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   ks(ks),
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   boundary(boundary),
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   actualOffset(actualOffset),
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   result(result),
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   cache(cache),
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   iter(ks->offset < 1 ? sks->getReadImpls().rangeContainingKeyBefore(ks->getKey()) : sks->getReadImpls().rangeContaining(ks->getKey()))
															#line 595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("normalizeKeySelectorActor", reinterpret_cast<unsigned long>(this));

	}
	~NormalizeKeySelectorActorActorState() 
	{
		fdb_probe_actor_destroy("normalizeKeySelectorActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			;
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~NormalizeKeySelectorActorActorState();
		static_cast<NormalizeKeySelectorActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		*actualOffset = ks->offset;
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!ks->isFirstGreaterOrEqual())
															#line 635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			TraceEvent(SevDebug, "ReadToBoundary") .detail("TerminateKey", ks->getKey()) .detail("TerminateOffset", ks->offset);
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (ks->offset < 1)
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result->readToBegin = true;
															#line 257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ks->setKey(boundary.begin);
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result->readThroughEnd = true;
															#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ks->setKey(boundary.end);
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ks->offset = 1;
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<NormalizeKeySelectorActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~NormalizeKeySelectorActorActorState(); static_cast<NormalizeKeySelectorActorActor*>(this)->destroy(); return 0; }
															#line 663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<NormalizeKeySelectorActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~NormalizeKeySelectorActorActorState();
		static_cast<NormalizeKeySelectorActorActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!((ks->offset < 1 && iter->begin() >= boundary.begin) || (ks->offset > 1 && iter->begin() < boundary.end)))
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (iter->value() != nullptr)
															#line 688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Void> __when_expr_0 = moveKeySelectorOverRangeActor(iter->value(), ryw, ks, cache);
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<NormalizeKeySelectorActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<NormalizeKeySelectorActorActor*>(this)->actor_wait_state = 1;
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< NormalizeKeySelectorActorActor, 0, Void >*>(static_cast<NormalizeKeySelectorActorActor*>(this)));
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont1(loopDepth);
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
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (ks->offset < 1)
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (iter == sks->getReadImpls().ranges().begin())
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
				return a_body1break1(loopDepth==0?0:loopDepth-1); // break
			}
			else
			{
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				--iter;
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
		else
		{
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (ks->offset > 1)
															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				++iter;
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

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
		if (static_cast<NormalizeKeySelectorActorActor*>(this)->actor_wait_state > 0) static_cast<NormalizeKeySelectorActorActor*>(this)->actor_wait_state = 0;
		static_cast<NormalizeKeySelectorActorActor*>(this)->ActorCallback< NormalizeKeySelectorActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< NormalizeKeySelectorActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("normalizeKeySelectorActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("normalizeKeySelectorActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< NormalizeKeySelectorActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("normalizeKeySelectorActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("normalizeKeySelectorActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< NormalizeKeySelectorActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("normalizeKeySelectorActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("normalizeKeySelectorActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace* sks;
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector* ks;
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef boundary;
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	int* actualOffset;
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult* result;
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeMap<Optional<RangeResult>>* cache;
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, SpecialKeyRangeReadImpl*, KeyRangeRef>::iterator iter;
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via normalizeKeySelectorActor()
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class NormalizeKeySelectorActorActor final : public Actor<Void>, public ActorCallback< NormalizeKeySelectorActorActor, 0, Void >, public FastAllocated<NormalizeKeySelectorActorActor>, public NormalizeKeySelectorActorActorState<NormalizeKeySelectorActorActor> {
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<NormalizeKeySelectorActorActor>::operator new;
	using FastAllocated<NormalizeKeySelectorActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< NormalizeKeySelectorActorActor, 0, Void >;
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	NormalizeKeySelectorActorActor(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeySelector* const& ks,KeyRangeRef const& boundary,int* const& actualOffset,RangeResult* const& result,KeyRangeMap<Optional<RangeResult>>* const& cache) 
															#line 863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<Void>(),
		   NormalizeKeySelectorActorActorState<NormalizeKeySelectorActorActor>(sks, ryw, ks, boundary, actualOffset, result, cache)
	{
		fdb_probe_actor_enter("normalizeKeySelectorActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("normalizeKeySelectorActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("normalizeKeySelectorActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< NormalizeKeySelectorActorActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Void> normalizeKeySelectorActor( SpecialKeySpace* const& sks, ReadYourWritesTransaction* const& ryw, KeySelector* const& ks, KeyRangeRef const& boundary, int* const& actualOffset, RangeResult* const& result, KeyRangeMap<Optional<RangeResult>>* const& cache ) {
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Void>(new NormalizeKeySelectorActorActor(sks, ryw, ks, boundary, actualOffset, result, cache));
															#line 891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

SpecialKeySpace::SpecialKeySpace(KeyRef spaceStartKey, KeyRef spaceEndKey, bool testOnly)
  : readImpls(nullptr, spaceEndKey),
    modules(testOnly ? SpecialKeySpace::MODULE::TESTONLY : SpecialKeySpace::MODULE::UNKNOWN, spaceEndKey),
    writeImpls(nullptr, spaceEndKey), range(KeyRangeRef(spaceStartKey, spaceEndKey)) {
	// Default begin of KeyRangeMap is Key(), insert the range to update start key
	readImpls.insert(range, nullptr);
	writeImpls.insert(range, nullptr);
	if (!testOnly) {
		// testOnly is used in the correctness workload
		modulesBoundaryInit();
	}
}

void SpecialKeySpace::modulesBoundaryInit() {
	for (const auto& pair : moduleToBoundary) {
		ASSERT(range.contains(pair.second));
		// Make sure the module is not overlapping with any registered read modules
		// Note: same like ranges, one module's end cannot be another module's start, relax the condition if needed
		ASSERT(modules.rangeContaining(pair.second.begin) == modules.rangeContaining(pair.second.end) &&
		       modules[pair.second.begin] == SpecialKeySpace::MODULE::UNKNOWN);
		modules.insert(pair.second, pair.first);
		// Note: Due to underlying implementation, the insertion here is important to make cross_module_read being
		// handled correctly
		readImpls.insert(pair.second, nullptr);
		writeImpls.insert(pair.second, nullptr);
	}
}

															#line 924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
// This generated class is to be used only via checkRYWValid()
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class SpecialKeySpace_CheckRYWValidActor>
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class SpecialKeySpace_CheckRYWValidActorState {
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace_CheckRYWValidActorState(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Reverse const& reverse) 
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : sks(sks),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   ryw(ryw),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   begin(begin),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   end(end),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   limits(limits),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   reverse(reverse)
															#line 947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("checkRYWValid", reinterpret_cast<unsigned long>(this));

	}
	~SpecialKeySpace_CheckRYWValidActorState() 
	{
		fdb_probe_actor_destroy("checkRYWValid", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(ryw);
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = SpecialKeySpace::getRangeAggregationActor(sks, ryw, begin, end, limits, reverse);
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ryw->resetFuture();
															#line 970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->actor_wait_state = 1;
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_CheckRYWValidActor, 0, RangeResult >*>(static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)));
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_CheckRYWValidActor, 1, Void >*>(static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)));
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~SpecialKeySpace_CheckRYWValidActorState();
		static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& result,int loopDepth) 
	{
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~SpecialKeySpace_CheckRYWValidActorState(); static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->destroy(); return 0; }
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~SpecialKeySpace_CheckRYWValidActorState();
		static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult && result,int loopDepth) 
	{
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~SpecialKeySpace_CheckRYWValidActorState(); static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->destroy(); return 0; }
															#line 1012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~SpecialKeySpace_CheckRYWValidActorState();
		static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 1024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 1032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->actor_wait_state > 0) static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->actor_wait_state = 0;
		static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->ActorCallback< SpecialKeySpace_CheckRYWValidActor, 0, RangeResult >::remove();
		static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->ActorCallback< SpecialKeySpace_CheckRYWValidActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_CheckRYWValidActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("checkRYWValid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkRYWValid", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_CheckRYWValidActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("checkRYWValid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkRYWValid", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SpecialKeySpace_CheckRYWValidActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("checkRYWValid", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkRYWValid", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_CheckRYWValidActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("checkRYWValid", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkRYWValid", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_CheckRYWValidActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("checkRYWValid", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkRYWValid", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SpecialKeySpace_CheckRYWValidActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("checkRYWValid", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkRYWValid", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace* sks;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector begin;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector end;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetRangeLimits limits;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Reverse reverse;
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via checkRYWValid()
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class SpecialKeySpace_CheckRYWValidActor final : public Actor<RangeResult>, public ActorCallback< SpecialKeySpace_CheckRYWValidActor, 0, RangeResult >, public ActorCallback< SpecialKeySpace_CheckRYWValidActor, 1, Void >, public FastAllocated<SpecialKeySpace_CheckRYWValidActor>, public SpecialKeySpace_CheckRYWValidActorState<SpecialKeySpace_CheckRYWValidActor> {
															#line 1150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<SpecialKeySpace_CheckRYWValidActor>::operator new;
	using FastAllocated<SpecialKeySpace_CheckRYWValidActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SpecialKeySpace_CheckRYWValidActor, 0, RangeResult >;
friend struct ActorCallback< SpecialKeySpace_CheckRYWValidActor, 1, Void >;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace_CheckRYWValidActor(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Reverse const& reverse) 
															#line 1162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   SpecialKeySpace_CheckRYWValidActorState<SpecialKeySpace_CheckRYWValidActor>(sks, ryw, begin, end, limits, reverse)
	{
		fdb_probe_actor_enter("checkRYWValid", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkRYWValid");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkRYWValid", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SpecialKeySpace_CheckRYWValidActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> SpecialKeySpace::checkRYWValid( SpecialKeySpace* const& sks, ReadYourWritesTransaction* const& ryw, KeySelector const& begin, KeySelector const& end, GetRangeLimits const& limits, Reverse const& reverse ) {
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new SpecialKeySpace_CheckRYWValidActor(sks, ryw, begin, end, limits, reverse));
															#line 1189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

															#line 1194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
// This generated class is to be used only via getRangeAggregationActor()
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class SpecialKeySpace_GetRangeAggregationActorActor>
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class SpecialKeySpace_GetRangeAggregationActorActorState {
															#line 1200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace_GetRangeAggregationActorActorState(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Reverse const& reverse) 
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : sks(sks),
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   ryw(ryw),
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   begin(begin),
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   end(end),
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   limits(limits),
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   reverse(reverse),
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   result(),
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   pairs(),
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   iter(),
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   actualBeginOffset(),
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   actualEndOffset(),
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   moduleBoundary(),
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   cache(Optional<RangeResult>(), specialKeys.end)
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("getRangeAggregationActor", reinterpret_cast<unsigned long>(this));

	}
	~SpecialKeySpace_GetRangeAggregationActorActorState() 
	{
		fdb_probe_actor_destroy("getRangeAggregationActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (ryw->specialKeySpaceRelaxed())
															#line 1246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				moduleBoundary = sks->range;
															#line 1250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				auto beginIter = sks->getModules().rangeContaining(begin.getKey());
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (beginIter->begin() <= end.getKey() && end.getKey() <= beginIter->end())
															#line 1258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (beginIter->value() == SpecialKeySpace::MODULE::UNKNOWN)
															#line 1262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						return a_body1Catch1(special_keys_no_module_found(), loopDepth);
															#line 1266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
					else
					{
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						moduleBoundary = beginIter->range();
															#line 1272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
				else
				{
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					TraceEvent(SevInfo, "SpecialKeyCrossModuleRead") .detail("Begin", begin) .detail("End", end) .detail("BoundaryBegin", beginIter->begin()) .detail("BoundaryEnd", beginIter->end());
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					return a_body1Catch1(special_keys_cross_module_read(), loopDepth);
															#line 1281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Void> __when_expr_0 = normalizeKeySelectorActor(sks, ryw, &begin, moduleBoundary, &actualBeginOffset, &result, &cache);
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 1;
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 0, Void >*>(static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)));
															#line 1293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~SpecialKeySpace_GetRangeAggregationActorActorState();
		static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Void> __when_expr_1 = normalizeKeySelectorActor(sks, ryw, &end, moduleBoundary, &actualEndOffset, &result, &cache);
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 2;
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 1, Void >*>(static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)));
															#line 1323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Void> __when_expr_1 = normalizeKeySelectorActor(sks, ryw, &end, moduleBoundary, &actualEndOffset, &result, &cache);
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 2;
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 1, Void >*>(static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)));
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state > 0) static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 0;
		static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont8(Void const& _,int loopDepth) 
	{
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (actualBeginOffset >= actualEndOffset && begin.getKey() >= end.getKey())
															#line 1411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			CODE_PROBE(true, "inverted range");
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(false, false)); this->~SpecialKeySpace_GetRangeAggregationActorActorState(); static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->destroy(); return 0; }
															#line 1417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(false, false));
			this->~SpecialKeySpace_GetRangeAggregationActorActorState();
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (begin.getKey() == moduleBoundary.end || end.getKey() == moduleBoundary.begin)
															#line 1425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			CODE_PROBE(true, "query touches begin or end");
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~SpecialKeySpace_GetRangeAggregationActorActorState(); static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->destroy(); return 0; }
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
			this->~SpecialKeySpace_GetRangeAggregationActorActorState();
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ranges = sks->getReadImpls().intersectingRanges(KeyRangeRef(begin.getKey(), end.getKey()));
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (ryw->getTenant().present())
															#line 1441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( auto iter : ranges ) {
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (iter->value() == nullptr)
															#line 1447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
					continue;
				}
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (!iter->value()->supportsTenants())
															#line 1453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					return a_body1Catch1(illegal_tenant_access(), loopDepth);
															#line 1457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		iter = reverse ? ranges.end() : ranges.begin();
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (reverse)
															#line 1465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			;
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = a_body1cont8loopHead1(loopDepth);
		}
		else
		{
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			iter = ranges.begin();
															#line 1476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = a_body1cont8loopHead2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont8(Void && _,int loopDepth) 
	{
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (actualBeginOffset >= actualEndOffset && begin.getKey() >= end.getKey())
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			CODE_PROBE(true, "inverted range");
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(false, false)); this->~SpecialKeySpace_GetRangeAggregationActorActorState(); static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->destroy(); return 0; }
															#line 1492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(false, false));
			this->~SpecialKeySpace_GetRangeAggregationActorActorState();
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (begin.getKey() == moduleBoundary.end || end.getKey() == moduleBoundary.begin)
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			CODE_PROBE(true, "query touches begin or end");
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~SpecialKeySpace_GetRangeAggregationActorActorState(); static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->destroy(); return 0; }
															#line 1506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
			this->~SpecialKeySpace_GetRangeAggregationActorActorState();
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ranges = sks->getReadImpls().intersectingRanges(KeyRangeRef(begin.getKey(), end.getKey()));
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (ryw->getTenant().present())
															#line 1516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( auto iter : ranges ) {
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (iter->value() == nullptr)
															#line 1522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
					continue;
				}
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (!iter->value()->supportsTenants())
															#line 1528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					return a_body1Catch1(illegal_tenant_access(), loopDepth);
															#line 1532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		iter = reverse ? ranges.end() : ranges.begin();
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (reverse)
															#line 1540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			;
															#line 1544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = a_body1cont8loopHead1(loopDepth);
		}
		else
		{
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			iter = ranges.begin();
															#line 1551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = a_body1cont8loopHead2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state > 0) static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 0;
		static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont9(int loopDepth) 
	{
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~SpecialKeySpace_GetRangeAggregationActorActorState(); static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->destroy(); return 0; }
															#line 1624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
		this->~SpecialKeySpace_GetRangeAggregationActorActorState();
		static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont16(int loopDepth) 
	{
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont8loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody1(int loopDepth) 
	{
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!(iter != ranges.begin()))
															#line 1649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
			return a_body1cont8break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		--iter;
															#line 386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (iter->value() == nullptr)
															#line 1657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
			return a_body1cont8loopHead1(loopDepth); // continue
		}
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		KeyRangeRef kr = iter->range();
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		KeyRef keyStart = kr.contains(begin.getKey()) ? begin.getKey() : kr.begin;
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		KeyRef keyEnd = kr.contains(end.getKey()) ? end.getKey() : kr.end;
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (iter->value()->isAsync() && cache.rangeContaining(keyStart).value().present())
															#line 1669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			const SpecialKeyRangeAsyncImpl* ptr = dynamic_cast<const SpecialKeyRangeAsyncImpl*>(iter->value());
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_2 = ptr->getRange(ryw, KeyRangeRef(keyStart, keyEnd), limits, &cache);
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont8loopBody1when1(__when_expr_2.get(), loopDepth); };
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 3;
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 2, RangeResult >*>(static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)));
															#line 1682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_3 = iter->value()->getRange(ryw, KeyRangeRef(keyStart, keyEnd), limits);
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1cont8loopBody1when2(__when_expr_3.get(), loopDepth); };
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 4;
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 3, RangeResult >*>(static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)));
															#line 1696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont8break1(int loopDepth) 
	{
		try {
			return a_body1cont16(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont8loopBody1cont1(int loopDepth) 
	{
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		result.arena().dependsOn(pairs.arena());
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for(int i = pairs.size() - 1;i >= 0;--i) {
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(iter->range().contains(pairs[i].key));
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back(result.arena(), pairs[i]);
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			limits.decrement(pairs[i]);
															#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (limits.isReached())
															#line 1729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.more = true;
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.readToBegin = false;
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (!static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~SpecialKeySpace_GetRangeAggregationActorActorState(); static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->destroy(); return 0; }
															#line 1737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				new (&static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
				this->~SpecialKeySpace_GetRangeAggregationActorActorState();
				static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
		if (loopDepth == 0) return a_body1cont8loopHead1(0);

		return loopDepth;
	}
	int a_body1cont8loopBody1cont4(RangeResult const& pairs_,int loopDepth) 
	{
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 1752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont8loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody1cont4(RangeResult && pairs_,int loopDepth) 
	{
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 1761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont8loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody1when1(RangeResult const& pairs_,int loopDepth) 
	{
		loopDepth = a_body1cont8loopBody1cont4(pairs_, loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody1when1(RangeResult && pairs_,int loopDepth) 
	{
		loopDepth = a_body1cont8loopBody1cont4(std::move(pairs_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state > 0) static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 0;
		static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 2, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 2, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont8loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 2, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont8loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 2, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont8loopBody1cont6(RangeResult const& pairs_,int loopDepth) 
	{
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 1833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont8loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody1cont6(RangeResult && pairs_,int loopDepth) 
	{
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 1842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont8loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody1when2(RangeResult const& pairs_,int loopDepth) 
	{
		loopDepth = a_body1cont8loopBody1cont6(pairs_, loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody1when2(RangeResult && pairs_,int loopDepth) 
	{
		loopDepth = a_body1cont8loopBody1cont6(std::move(pairs_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state > 0) static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 0;
		static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 3, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 3, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont8loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 3, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont8loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 3, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont17(int loopDepth) 
	{
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopHead2(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont8loopBody2(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody2(int loopDepth) 
	{
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!(iter != ranges.end()))
															#line 1927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
			return a_body1cont8break2(loopDepth==0?0:loopDepth-1); // break
		}
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (iter->value() == nullptr)
															#line 1933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
			return a_body1cont8continue1(loopDepth); // continue
		}
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		KeyRangeRef kr = iter->range();
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		KeyRef keyStart = kr.contains(begin.getKey()) ? begin.getKey() : kr.begin;
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		KeyRef keyEnd = kr.contains(end.getKey()) ? end.getKey() : kr.end;
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (iter->value()->isAsync() && cache.rangeContaining(keyStart).value().present())
															#line 1945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			const SpecialKeyRangeAsyncImpl* ptr = dynamic_cast<const SpecialKeyRangeAsyncImpl*>(iter->value());
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_4 = ptr->getRange(ryw, KeyRangeRef(keyStart, keyEnd), limits, &cache);
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1cont8loopBody2when1(__when_expr_4.get(), loopDepth); };
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 5;
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 4, RangeResult >*>(static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)));
															#line 1958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_5 = iter->value()->getRange(ryw, KeyRangeRef(keyStart, keyEnd), limits);
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1cont8loopBody2when2(__when_expr_5.get(), loopDepth); };
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 6;
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 5, RangeResult >*>(static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)));
															#line 1972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont8break2(int loopDepth) 
	{
		try {
			return a_body1cont17(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont8continue1(int loopDepth) 
	{
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		++iter;
															#line 1995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont8loopHead2(0);

		return loopDepth;
	}
	int a_body1cont8loopBody2cont1(int loopDepth) 
	{
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		result.arena().dependsOn(pairs.arena());
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for(int i = 0;i < pairs.size();++i) {
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(iter->range().contains(pairs[i].key));
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back(result.arena(), pairs[i]);
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			limits.decrement(pairs[i]);
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (limits.isReached())
															#line 2014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.more = true;
															#line 441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.readThroughEnd = false;
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (!static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~SpecialKeySpace_GetRangeAggregationActorActorState(); static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->destroy(); return 0; }
															#line 2022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				new (&static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
				this->~SpecialKeySpace_GetRangeAggregationActorActorState();
				static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		++iter;
															#line 2031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont8loopHead2(0);

		return loopDepth;
	}
	int a_body1cont8loopBody2cont4(RangeResult const& pairs_,int loopDepth) 
	{
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 2040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont8loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody2cont4(RangeResult && pairs_,int loopDepth) 
	{
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 2049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont8loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody2when1(RangeResult const& pairs_,int loopDepth) 
	{
		loopDepth = a_body1cont8loopBody2cont4(pairs_, loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody2when1(RangeResult && pairs_,int loopDepth) 
	{
		loopDepth = a_body1cont8loopBody2cont4(std::move(pairs_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state > 0) static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 0;
		static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 4, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 4, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont8loopBody2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 4, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont8loopBody2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 4, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont8loopBody2cont6(RangeResult const& pairs_,int loopDepth) 
	{
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 2121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont8loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody2cont6(RangeResult && pairs_,int loopDepth) 
	{
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 2130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont8loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody2when2(RangeResult const& pairs_,int loopDepth) 
	{
		loopDepth = a_body1cont8loopBody2cont6(pairs_, loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody2when2(RangeResult && pairs_,int loopDepth) 
	{
		loopDepth = a_body1cont8loopBody2cont6(std::move(pairs_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state > 0) static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 0;
		static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 5, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 5, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont8loopBody2when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 5, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont8loopBody2when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 5, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), 5);

	}
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace* sks;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector begin;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector end;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetRangeLimits limits;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Reverse reverse;
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult result;
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult pairs;
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, SpecialKeyRangeReadImpl*, KeyRangeRef>::iterator iter;
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	int actualBeginOffset;
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	int actualEndOffset;
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef moduleBoundary;
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeMap<Optional<RangeResult>> cache;
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, SpecialKeyRangeReadImpl*, KeyRangeRef>::Ranges ranges;
															#line 2226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getRangeAggregationActor()
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class SpecialKeySpace_GetRangeAggregationActorActor final : public Actor<RangeResult>, public ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 0, Void >, public ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 1, Void >, public ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 2, RangeResult >, public ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 3, RangeResult >, public ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 4, RangeResult >, public ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 5, RangeResult >, public FastAllocated<SpecialKeySpace_GetRangeAggregationActorActor>, public SpecialKeySpace_GetRangeAggregationActorActorState<SpecialKeySpace_GetRangeAggregationActorActor> {
															#line 2231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<SpecialKeySpace_GetRangeAggregationActorActor>::operator new;
	using FastAllocated<SpecialKeySpace_GetRangeAggregationActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 0, Void >;
friend struct ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 1, Void >;
friend struct ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 2, RangeResult >;
friend struct ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 3, RangeResult >;
friend struct ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 4, RangeResult >;
friend struct ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 5, RangeResult >;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace_GetRangeAggregationActorActor(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Reverse const& reverse) 
															#line 2247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   SpecialKeySpace_GetRangeAggregationActorActorState<SpecialKeySpace_GetRangeAggregationActorActor>(sks, ryw, begin, end, limits, reverse)
	{
		fdb_probe_actor_enter("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getRangeAggregationActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getRangeAggregationActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 2, RangeResult >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 3, RangeResult >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 4, RangeResult >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 5, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> SpecialKeySpace::getRangeAggregationActor( SpecialKeySpace* const& sks, ReadYourWritesTransaction* const& ryw, KeySelector const& begin, KeySelector const& end, GetRangeLimits const& limits, Reverse const& reverse ) {
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new SpecialKeySpace_GetRangeAggregationActorActor(sks, ryw, begin, end, limits, reverse));
															#line 2279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

Future<RangeResult> SpecialKeySpace::getRange(ReadYourWritesTransaction* ryw,
                                              KeySelector begin,
                                              KeySelector end,
                                              GetRangeLimits limits,
                                              Reverse reverse) {
	// validate limits here
	if (!limits.isValid())
		return range_limits_invalid();
	if (limits.isReached()) {
		CODE_PROBE(true, "Special Key Space range read limit 0");
		return RangeResult();
	}
	// make sure orEqual == false
	begin.removeOrEqual(begin.arena());
	end.removeOrEqual(end.arena());

	if (begin.offset >= end.offset && begin.getKey() >= end.getKey()) {
		CODE_PROBE(true, "range inverted");
		return RangeResult();
	}

	return checkRYWValid(this, ryw, begin, end, limits, reverse);
}

															#line 2308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
// This generated class is to be used only via getActor()
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class SpecialKeySpace_GetActorActor>
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class SpecialKeySpace_GetActorActorState {
															#line 2314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace_GetActorActorState(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeyRef const& key) 
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : sks(sks),
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   ryw(ryw),
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   key(key)
															#line 2325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("getActor", reinterpret_cast<unsigned long>(this));

	}
	~SpecialKeySpace_GetActorActorState() 
	{
		fdb_probe_actor_destroy("getActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = sks->getRange(ryw, KeySelector(firstGreaterOrEqual(key)), KeySelector(firstGreaterOrEqual(keyAfter(key))), GetRangeLimits(CLIENT_KNOBS->TOO_MANY), Reverse::False);
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<SpecialKeySpace_GetActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SpecialKeySpace_GetActorActor*>(this)->actor_wait_state = 1;
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetActorActor, 0, RangeResult >*>(static_cast<SpecialKeySpace_GetActorActor*>(this)));
															#line 2347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~SpecialKeySpace_GetActorActorState();
		static_cast<SpecialKeySpace_GetActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& result,int loopDepth) 
	{
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(result.size() <= 1);
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (result.size())
															#line 2372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetActorActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>(result[0].value)); this->~SpecialKeySpace_GetActorActorState(); static_cast<SpecialKeySpace_GetActorActor*>(this)->destroy(); return 0; }
															#line 2376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<SpecialKeySpace_GetActorActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>(result[0].value));
			this->~SpecialKeySpace_GetActorActorState();
			static_cast<SpecialKeySpace_GetActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetActorActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>()); this->~SpecialKeySpace_GetActorActorState(); static_cast<SpecialKeySpace_GetActorActor*>(this)->destroy(); return 0; }
															#line 2386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<SpecialKeySpace_GetActorActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>());
			this->~SpecialKeySpace_GetActorActorState();
			static_cast<SpecialKeySpace_GetActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1cont1(RangeResult && result,int loopDepth) 
	{
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(result.size() <= 1);
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (result.size())
															#line 2401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetActorActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>(result[0].value)); this->~SpecialKeySpace_GetActorActorState(); static_cast<SpecialKeySpace_GetActorActor*>(this)->destroy(); return 0; }
															#line 2405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<SpecialKeySpace_GetActorActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>(result[0].value));
			this->~SpecialKeySpace_GetActorActorState();
			static_cast<SpecialKeySpace_GetActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetActorActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>()); this->~SpecialKeySpace_GetActorActorState(); static_cast<SpecialKeySpace_GetActorActor*>(this)->destroy(); return 0; }
															#line 2415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<SpecialKeySpace_GetActorActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>());
			this->~SpecialKeySpace_GetActorActorState();
			static_cast<SpecialKeySpace_GetActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1when1(RangeResult const& result,int loopDepth) 
	{
		loopDepth = a_body1cont1(result, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && result,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(result), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SpecialKeySpace_GetActorActor*>(this)->actor_wait_state > 0) static_cast<SpecialKeySpace_GetActorActor*>(this)->actor_wait_state = 0;
		static_cast<SpecialKeySpace_GetActorActor*>(this)->ActorCallback< SpecialKeySpace_GetActorActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_GetActorActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SpecialKeySpace_GetActorActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SpecialKeySpace_GetActorActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace* sks;
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef key;
															#line 2493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getActor()
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class SpecialKeySpace_GetActorActor final : public Actor<Optional<Value>>, public ActorCallback< SpecialKeySpace_GetActorActor, 0, RangeResult >, public FastAllocated<SpecialKeySpace_GetActorActor>, public SpecialKeySpace_GetActorActorState<SpecialKeySpace_GetActorActor> {
															#line 2498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<SpecialKeySpace_GetActorActor>::operator new;
	using FastAllocated<SpecialKeySpace_GetActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<Value>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SpecialKeySpace_GetActorActor, 0, RangeResult >;
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace_GetActorActor(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeyRef const& key) 
															#line 2509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<Optional<Value>>(),
		   SpecialKeySpace_GetActorActorState<SpecialKeySpace_GetActorActor>(sks, ryw, key)
	{
		fdb_probe_actor_enter("getActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SpecialKeySpace_GetActorActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Optional<Value>> SpecialKeySpace::getActor( SpecialKeySpace* const& sks, ReadYourWritesTransaction* const& ryw, KeyRef const& key ) {
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<Value>>(new SpecialKeySpace_GetActorActor(sks, ryw, key));
															#line 2536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

Future<Optional<Value>> SpecialKeySpace::get(ReadYourWritesTransaction* ryw, const Key& key) {
	return getActor(this, ryw, key);
}

void SpecialKeySpace::set(ReadYourWritesTransaction* ryw, const KeyRef& key, const ValueRef& value) {
	if (!ryw->specialKeySpaceChangeConfiguration())
		throw special_keys_write_disabled();
	auto impl = writeImpls[key];
	if (impl == nullptr) {
		TraceEvent(SevDebug, "SpecialKeySpaceNoWriteModuleFound")
		    .detail("Key", key.toString())
		    .detail("Value", value.toString());
		throw special_keys_no_write_module_found();
	}
	if (!impl->supportsTenants() && ryw->getTenant().present()) {
		throw illegal_tenant_access();
	}
	return impl->set(ryw, key, value);
}

void SpecialKeySpace::clear(ReadYourWritesTransaction* ryw, const KeyRangeRef& range) {
	if (!ryw->specialKeySpaceChangeConfiguration())
		throw special_keys_write_disabled();
	if (range.empty())
		return;
	auto begin = writeImpls[range.begin];
	auto end = writeImpls.rangeContainingKeyBefore(range.end)->value();
	if (begin != end) {
		TraceEvent(SevDebug, "SpecialKeySpaceCrossModuleClear").detail("Range", range);
		throw special_keys_cross_module_clear(); // ban cross module clear
	} else if (begin == nullptr) {
		TraceEvent(SevDebug, "SpecialKeySpaceNoWriteModuleFound").detail("Range", range);
		throw special_keys_no_write_module_found();
	}
	if (!begin->supportsTenants() && ryw->getTenant().present()) {
		throw illegal_tenant_access();
	}
	return begin->clear(ryw, range);
}

void SpecialKeySpace::clear(ReadYourWritesTransaction* ryw, const KeyRef& key) {
	if (!ryw->specialKeySpaceChangeConfiguration())
		throw special_keys_write_disabled();
	auto impl = writeImpls[key];
	if (impl == nullptr)
		throw special_keys_no_write_module_found();
	if (!impl->supportsTenants() && ryw->getTenant().present()) {
		throw illegal_tenant_access();
	}
	return impl->clear(ryw, key);
}

bool validateSnakeCaseNaming(const KeyRef& k) {
	KeyRef key(k);
	// Remove prefix \xff\xff
	ASSERT(key.startsWith(specialKeys.begin));
	key = key.removePrefix(specialKeys.begin);
	// Suffix can be \xff\xff or \x00 in single key range
	if (key.endsWith(specialKeys.begin))
		key = key.removeSuffix(specialKeys.end);
	else if (key.endsWith("\x00"_sr))
		key = key.removeSuffix("\x00"_sr);
	for (const char& c : key.toString()) {
		// only small letters, numbers, '/', '_' is allowed
		ASSERT((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '/' || c == '_');
	}
	return true;
}

void SpecialKeySpace::registerKeyRange(SpecialKeySpace::MODULE module,
                                       SpecialKeySpace::IMPLTYPE type,
                                       const KeyRangeRef& kr,
                                       SpecialKeyRangeReadImpl* impl) {
	// Not allowed to register an empty range
	ASSERT(!kr.empty());
	// module boundary check
	if (module == SpecialKeySpace::MODULE::TESTONLY) {
		ASSERT(normalKeys.contains(kr));
	} else {
		ASSERT(moduleToBoundary.at(module).contains(kr));
		// validate keys follow snake case naming style
		ASSERT(validateSnakeCaseNaming(kr.begin) && validateSnakeCaseNaming(kr.end));
	}
	// make sure the registered range is not overlapping with existing ones
	// Note: kr.end should not be the same as another range's begin, although it should work even they are the same
	for (auto iter = readImpls.rangeContaining(kr.begin); true; ++iter) {
		ASSERT(iter->value() == nullptr);
		if (iter == readImpls.rangeContaining(kr.end)) {
			// Note: relax the condition that the end can be another range's start, if needed
			break;
		}
	}
	readImpls.insert(kr, impl);
	// if rw, it means the module can do both read and write
	if (type == SpecialKeySpace::IMPLTYPE::READWRITE) {
		// since write impls are always subset of read impls,
		// no need to check overlapped registration
		auto rwImpl = dynamic_cast<SpecialKeyRangeRWImpl*>(impl);
		ASSERT(rwImpl);
		writeImpls.insert(kr, rwImpl);
	}
}

Key SpecialKeySpace::decode(const KeyRef& key) {
	auto impl = writeImpls[key];
	ASSERT(impl != nullptr);
	return impl->decode(key);
}

KeyRange SpecialKeySpace::decode(const KeyRangeRef& kr) {
	// Only allow to decode key range in the same underlying impl range
	auto begin = writeImpls.rangeContaining(kr.begin);
	ASSERT(begin->value() != nullptr);
	auto end = writeImpls.rangeContainingKeyBefore(kr.end);
	ASSERT(begin == end);
	return KeyRangeRef(begin->value()->decode(kr.begin), begin->value()->decode(kr.end));
}

															#line 2659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via commitActor()
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class CommitActorActor>
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class CommitActorActorState {
															#line 2666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	CommitActorActorState(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw) 
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : sks(sks),
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   ryw(ryw),
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   ranges(ryw->getSpecialKeySpaceWriteMap().containedRanges(specialKeys)),
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   iter(ranges.begin()),
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   writeModulePtrs()
															#line 2681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("commitActor", reinterpret_cast<unsigned long>(this));

	}
	~CommitActorActorState() 
	{
		fdb_probe_actor_destroy("commitActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			std::unordered_set<SpecialKeyRangeRWImpl*> deduplicate;
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for(;iter != ranges.end();) {
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				std::pair<bool, Optional<Value>> entry = iter->value();
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (entry.first)
															#line 2702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					auto modulePtr = sks->getRWImpls().rangeContaining(iter->begin())->value();
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					auto [_, inserted] = deduplicate.insert(modulePtr);
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (inserted)
															#line 2710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						writeModulePtrs.push_back(modulePtr);
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				++iter;
															#line 2719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			it = std::vector<SpecialKeyRangeRWImpl*>::const_iterator();
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (ryw->getTenant().present())
															#line 2725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				for(it = writeModulePtrs.begin();it != writeModulePtrs.end();++it) {
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (!(*it)->supportsTenants())
															#line 2731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						return a_body1Catch1(illegal_tenant_access(), loopDepth);
															#line 2735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
			}
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			it = writeModulePtrs.begin();
															#line 2741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~CommitActorActorState();
		static_cast<CommitActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<CommitActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CommitActorActorState(); static_cast<CommitActorActor*>(this)->destroy(); return 0; }
															#line 2764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<CommitActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CommitActorActorState();
		static_cast<CommitActorActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!(it != writeModulePtrs.end()))
															#line 2783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Optional<std::string>> __when_expr_0 = (*it)->commit(ryw);
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<CommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<CommitActorActor*>(this)->actor_wait_state = 1;
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CommitActorActor, 0, Optional<std::string> >*>(static_cast<CommitActorActor*>(this)));
															#line 2796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
	int a_body1loopBody1cont1(Optional<std::string> const& msg,int loopDepth) 
	{
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (msg.present())
															#line 2818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg(msg.get());
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			TraceEvent(SevDebug, "SpecialKeySpaceManagementAPIError") .detail("Reason", msg.get()) .detail("Range", (*it)->getKeyRange().toString());
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), std::max(0, loopDepth - 1));
															#line 2826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		++it;
															#line 2830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Optional<std::string> && msg,int loopDepth) 
	{
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (msg.present())
															#line 2839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg(msg.get());
															#line 641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			TraceEvent(SevDebug, "SpecialKeySpaceManagementAPIError") .detail("Reason", msg.get()) .detail("Range", (*it)->getKeyRange().toString());
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), std::max(0, loopDepth - 1));
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		++it;
															#line 2851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<std::string> const& msg,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(msg, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<std::string> && msg,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(msg), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CommitActorActor*>(this)->actor_wait_state > 0) static_cast<CommitActorActor*>(this)->actor_wait_state = 0;
		static_cast<CommitActorActor*>(this)->ActorCallback< CommitActorActor, 0, Optional<std::string> >::remove();

	}
	void a_callback_fire(ActorCallback< CommitActorActor, 0, Optional<std::string> >*,Optional<std::string> const& value) 
	{
		fdb_probe_actor_enter("commitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CommitActorActor, 0, Optional<std::string> >*,Optional<std::string> && value) 
	{
		fdb_probe_actor_enter("commitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CommitActorActor, 0, Optional<std::string> >*,Error err) 
	{
		fdb_probe_actor_enter("commitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commitActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace* sks;
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, std::pair<bool, Optional<Value>>, KeyRangeRef>::Ranges ranges;
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, std::pair<bool, Optional<Value>>, KeyRangeRef>::iterator iter;
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<SpecialKeyRangeRWImpl*> writeModulePtrs;
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<SpecialKeyRangeRWImpl*>::const_iterator it;
															#line 2931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via commitActor()
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class CommitActorActor final : public Actor<Void>, public ActorCallback< CommitActorActor, 0, Optional<std::string> >, public FastAllocated<CommitActorActor>, public CommitActorActorState<CommitActorActor> {
															#line 2936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<CommitActorActor>::operator new;
	using FastAllocated<CommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CommitActorActor, 0, Optional<std::string> >;
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	CommitActorActor(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw) 
															#line 2947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<Void>(),
		   CommitActorActorState<CommitActorActor>(sks, ryw)
	{
		fdb_probe_actor_enter("commitActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("commitActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("commitActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CommitActorActor, 0, Optional<std::string> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Void> commitActor( SpecialKeySpace* const& sks, ReadYourWritesTransaction* const& ryw ) {
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Void>(new CommitActorActor(sks, ryw));
															#line 2975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

Future<Void> SpecialKeySpace::commit(ReadYourWritesTransaction* ryw) {
	return commitActor(this, ryw);
}

// For SKSCTestRWImpl and SKSCTestAsyncReadImpl
Future<RangeResult> SKSCTestGetRangeBase(ReadYourWritesTransaction* ryw, KeyRangeRef kr, GetRangeLimits limitsHint) {
	auto resultFuture = ryw->getRange(kr, CLIENT_KNOBS->TOO_MANY);
	// all keys are written to RYW, since GRV is set, the read should happen locally
	ASSERT(resultFuture.isReady());
	auto result = resultFuture.getValue();
	ASSERT(!result.more && result.size() < CLIENT_KNOBS->TOO_MANY);
	auto kvs = resultFuture.getValue();
	return rywGetRange(ryw, kr, kvs);
}

SKSCTestRWImpl::SKSCTestRWImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

Future<RangeResult> SKSCTestRWImpl::getRange(ReadYourWritesTransaction* ryw,
                                             KeyRangeRef kr,
                                             GetRangeLimits limitsHint) const {
	ASSERT(range.contains(kr));
	return SKSCTestGetRangeBase(ryw, kr, limitsHint);
}

Future<Optional<std::string>> SKSCTestRWImpl::commit(ReadYourWritesTransaction* ryw) {
	ASSERT(false);
	return Optional<std::string>();
}

SKSCTestAsyncReadImpl::SKSCTestAsyncReadImpl(KeyRangeRef kr) : SpecialKeyRangeAsyncImpl(kr) {}

Future<RangeResult> SKSCTestAsyncReadImpl::getRange(ReadYourWritesTransaction* ryw,
                                                    KeyRangeRef kr,
                                                    GetRangeLimits limitsHint) const {
	ASSERT(range.contains(kr));
	return SKSCTestGetRangeBase(ryw, kr, limitsHint);
}

ReadConflictRangeImpl::ReadConflictRangeImpl(KeyRangeRef kr) : SpecialKeyRangeReadImpl(kr) {}

															#line 3020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via getReadConflictRangeImpl()
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class GetReadConflictRangeImplActor>
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GetReadConflictRangeImplActorState {
															#line 3027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetReadConflictRangeImplActorState(ReadYourWritesTransaction* const& ryw,KeyRange const& kr) 
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 3036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("getReadConflictRangeImpl", reinterpret_cast<unsigned long>(this));

	}
	~GetReadConflictRangeImplActorState() 
	{
		fdb_probe_actor_destroy("getReadConflictRangeImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Void> __when_expr_0 = ryw->pendingReads();
															#line 691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GetReadConflictRangeImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetReadConflictRangeImplActor*>(this)->actor_wait_state = 1;
															#line 691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetReadConflictRangeImplActor, 0, Void >*>(static_cast<GetReadConflictRangeImplActor*>(this)));
															#line 3058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~GetReadConflictRangeImplActorState();
		static_cast<GetReadConflictRangeImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetReadConflictRangeImplActor*>(this)->SAV<RangeResult>::futures) { (void)(ryw->getReadConflictRangeIntersecting(kr)); this->~GetReadConflictRangeImplActorState(); static_cast<GetReadConflictRangeImplActor*>(this)->destroy(); return 0; }
															#line 3081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetReadConflictRangeImplActor*>(this)->SAV< RangeResult >::value()) RangeResult(ryw->getReadConflictRangeIntersecting(kr));
		this->~GetReadConflictRangeImplActorState();
		static_cast<GetReadConflictRangeImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetReadConflictRangeImplActor*>(this)->SAV<RangeResult>::futures) { (void)(ryw->getReadConflictRangeIntersecting(kr)); this->~GetReadConflictRangeImplActorState(); static_cast<GetReadConflictRangeImplActor*>(this)->destroy(); return 0; }
															#line 3093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetReadConflictRangeImplActor*>(this)->SAV< RangeResult >::value()) RangeResult(ryw->getReadConflictRangeIntersecting(kr));
		this->~GetReadConflictRangeImplActorState();
		static_cast<GetReadConflictRangeImplActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<GetReadConflictRangeImplActor*>(this)->actor_wait_state > 0) static_cast<GetReadConflictRangeImplActor*>(this)->actor_wait_state = 0;
		static_cast<GetReadConflictRangeImplActor*>(this)->ActorCallback< GetReadConflictRangeImplActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetReadConflictRangeImplActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getReadConflictRangeImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getReadConflictRangeImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetReadConflictRangeImplActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getReadConflictRangeImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getReadConflictRangeImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetReadConflictRangeImplActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getReadConflictRangeImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getReadConflictRangeImpl", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRange kr;
															#line 3168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getReadConflictRangeImpl()
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GetReadConflictRangeImplActor final : public Actor<RangeResult>, public ActorCallback< GetReadConflictRangeImplActor, 0, Void >, public FastAllocated<GetReadConflictRangeImplActor>, public GetReadConflictRangeImplActorState<GetReadConflictRangeImplActor> {
															#line 3173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GetReadConflictRangeImplActor>::operator new;
	using FastAllocated<GetReadConflictRangeImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetReadConflictRangeImplActor, 0, Void >;
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetReadConflictRangeImplActor(ReadYourWritesTransaction* const& ryw,KeyRange const& kr) 
															#line 3184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   GetReadConflictRangeImplActorState<GetReadConflictRangeImplActor>(ryw, kr)
	{
		fdb_probe_actor_enter("getReadConflictRangeImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getReadConflictRangeImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getReadConflictRangeImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetReadConflictRangeImplActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<RangeResult> getReadConflictRangeImpl( ReadYourWritesTransaction* const& ryw, KeyRange const& kr ) {
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new GetReadConflictRangeImplActor(ryw, kr));
															#line 3212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

Future<RangeResult> ReadConflictRangeImpl::getRange(ReadYourWritesTransaction* ryw,
                                                    KeyRangeRef kr,
                                                    GetRangeLimits limitsHint) const {
	return getReadConflictRangeImpl(ryw, kr);
}

WriteConflictRangeImpl::WriteConflictRangeImpl(KeyRangeRef kr) : SpecialKeyRangeReadImpl(kr) {}

Future<RangeResult> WriteConflictRangeImpl::getRange(ReadYourWritesTransaction* ryw,
                                                     KeyRangeRef kr,
                                                     GetRangeLimits limitsHint) const {
	return ryw->getWriteConflictRangeIntersecting(kr);
}

ConflictingKeysImpl::ConflictingKeysImpl(KeyRangeRef kr) : SpecialKeyRangeReadImpl(kr) {}

Future<RangeResult> ConflictingKeysImpl::getRange(ReadYourWritesTransaction* ryw,
                                                  KeyRangeRef kr,
                                                  GetRangeLimits limitsHint) const {
	RangeResult result;
	if (ryw->getTransactionState()->conflictingKeys) {
		auto krMapPtr = ryw->getTransactionState()->conflictingKeys.get();
		auto beginIter = krMapPtr->rangeContaining(kr.begin);
		auto endIter = krMapPtr->rangeContaining(kr.end);

		if (!kr.contains(beginIter->begin()) && beginIter != endIter)
			++beginIter;
		for (auto it = beginIter; it != endIter; ++it) {
			result.push_back_deep(result.arena(), KeyValueRef(it->begin(), it->value()));
		}
		if (kr.contains(endIter->begin()))
			result.push_back_deep(result.arena(), KeyValueRef(endIter->begin(), endIter->value()));
	}
	return result;
}

															#line 3253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via ddMetricsGetRangeActor()
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class DdMetricsGetRangeActorActor>
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class DdMetricsGetRangeActorActorState {
															#line 3260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	DdMetricsGetRangeActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 3269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this));

	}
	~DdMetricsGetRangeActorActorState() 
	{
		fdb_probe_actor_destroy("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			;
															#line 3284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~DdMetricsGetRangeActorActorState();
		static_cast<DdMetricsGetRangeActorActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			auto keys = kr.removePrefix(ddStatsRange.begin);
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Standalone<VectorRef<DDMetricsRef>>> __when_expr_0 = waitDataDistributionMetricsList(ryw->getDatabase(), keys, CLIENT_KNOBS->TOO_MANY);
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<DdMetricsGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DdMetricsGetRangeActorActor*>(this)->actor_wait_state = 1;
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DdMetricsGetRangeActorActor, 0, Standalone<VectorRef<DDMetricsRef>> >*>(static_cast<DdMetricsGetRangeActorActor*>(this)));
															#line 3324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			err = Error(e);
															#line 752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (e.code() == error_code_dd_not_found)
															#line 3342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				TraceEvent(SevWarnAlways, "DataDistributorNotPresent") .detail("Operation", "DDMetricsReqestThroughSpecialKeys");
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				StrictFuture<Void> __when_expr_1 = delayJittered(FLOW_KNOBS->PREVENT_FAST_SPIN_DELAY);
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (static_cast<DdMetricsGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
				static_cast<DdMetricsGetRangeActorActor*>(this)->actor_wait_state = 2;
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DdMetricsGetRangeActorActor, 1, Void >*>(static_cast<DdMetricsGetRangeActorActor*>(this)));
															#line 3355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
	int a_body1loopBody1cont2(Standalone<VectorRef<DDMetricsRef>> const& resultWithoutPrefix,int loopDepth) 
	{
															#line 737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( const auto& ddMetricsRef : resultWithoutPrefix ) {
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			KeyRef beginKey = ddMetricsRef.beginKey.withPrefix(ddStatsRange.begin, result.arena());
															#line 742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			json_spirit::mObject statsObj;
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			statsObj["shard_bytes"] = ddMetricsRef.shardBytes;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			std::string statsString = json_spirit::write_string(json_spirit::mValue(statsObj), json_spirit::Output_options::raw_utf8);
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ValueRef bytes(result.arena(), statsString);
															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back(result.arena(), KeyValueRef(beginKey, bytes));
															#line 3389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<DdMetricsGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~DdMetricsGetRangeActorActorState(); static_cast<DdMetricsGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 3393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<DdMetricsGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~DdMetricsGetRangeActorActorState();
		static_cast<DdMetricsGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Standalone<VectorRef<DDMetricsRef>> && resultWithoutPrefix,int loopDepth) 
	{
															#line 737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( const auto& ddMetricsRef : resultWithoutPrefix ) {
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			KeyRef beginKey = ddMetricsRef.beginKey.withPrefix(ddStatsRange.begin, result.arena());
															#line 742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			json_spirit::mObject statsObj;
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			statsObj["shard_bytes"] = ddMetricsRef.shardBytes;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			std::string statsString = json_spirit::write_string(json_spirit::mValue(statsObj), json_spirit::Output_options::raw_utf8);
															#line 746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ValueRef bytes(result.arena(), statsString);
															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back(result.arena(), KeyValueRef(beginKey, bytes));
															#line 3419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<DdMetricsGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~DdMetricsGetRangeActorActorState(); static_cast<DdMetricsGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 3423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<DdMetricsGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~DdMetricsGetRangeActorActorState();
		static_cast<DdMetricsGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Standalone<VectorRef<DDMetricsRef>> const& resultWithoutPrefix,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(resultWithoutPrefix, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Standalone<VectorRef<DDMetricsRef>> && resultWithoutPrefix,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(resultWithoutPrefix), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DdMetricsGetRangeActorActor*>(this)->actor_wait_state > 0) static_cast<DdMetricsGetRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<DdMetricsGetRangeActorActor*>(this)->ActorCallback< DdMetricsGetRangeActorActor, 0, Standalone<VectorRef<DDMetricsRef>> >::remove();

	}
	void a_callback_fire(ActorCallback< DdMetricsGetRangeActorActor, 0, Standalone<VectorRef<DDMetricsRef>> >*,Standalone<VectorRef<DDMetricsRef>> const& value) 
	{
		fdb_probe_actor_enter("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DdMetricsGetRangeActorActor, 0, Standalone<VectorRef<DDMetricsRef>> >*,Standalone<VectorRef<DDMetricsRef>> && value) 
	{
		fdb_probe_actor_enter("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DdMetricsGetRangeActorActor, 0, Standalone<VectorRef<DDMetricsRef>> >*,Error err) 
	{
		fdb_probe_actor_enter("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1Catch1cont1(int loopDepth) 
	{
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		return a_body1Catch1(err, std::max(0, loopDepth - 1));
															#line 3498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont2(Void const& _,int loopDepth) 
	{
		return a_body1loopHead1(loopDepth); // continue

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont2(Void && _,int loopDepth) 
	{
		return a_body1loopHead1(loopDepth); // continue

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
	void a_exitChoose2() 
	{
		if (static_cast<DdMetricsGetRangeActorActor*>(this)->actor_wait_state > 0) static_cast<DdMetricsGetRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<DdMetricsGetRangeActorActor*>(this)->ActorCallback< DdMetricsGetRangeActorActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DdMetricsGetRangeActorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DdMetricsGetRangeActorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DdMetricsGetRangeActorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Error err;
															#line 3583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via ddMetricsGetRangeActor()
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class DdMetricsGetRangeActorActor final : public Actor<RangeResult>, public ActorCallback< DdMetricsGetRangeActorActor, 0, Standalone<VectorRef<DDMetricsRef>> >, public ActorCallback< DdMetricsGetRangeActorActor, 1, Void >, public FastAllocated<DdMetricsGetRangeActorActor>, public DdMetricsGetRangeActorActorState<DdMetricsGetRangeActorActor> {
															#line 3588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<DdMetricsGetRangeActorActor>::operator new;
	using FastAllocated<DdMetricsGetRangeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DdMetricsGetRangeActorActor, 0, Standalone<VectorRef<DDMetricsRef>> >;
friend struct ActorCallback< DdMetricsGetRangeActorActor, 1, Void >;
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	DdMetricsGetRangeActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 3600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   DdMetricsGetRangeActorActorState<DdMetricsGetRangeActorActor>(ryw, kr)
	{
		fdb_probe_actor_enter("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("ddMetricsGetRangeActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("ddMetricsGetRangeActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DdMetricsGetRangeActorActor, 0, Standalone<VectorRef<DDMetricsRef>> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DdMetricsGetRangeActorActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> ddMetricsGetRangeActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new DdMetricsGetRangeActorActor(ryw, kr));
															#line 3629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

DDStatsRangeImpl::DDStatsRangeImpl(KeyRangeRef kr) : SpecialKeyRangeAsyncImpl(kr) {}

Future<RangeResult> DDStatsRangeImpl::getRange(ReadYourWritesTransaction* ryw,
                                               KeyRangeRef kr,
                                               GetRangeLimits limitsHint) const {
	return ddMetricsGetRangeActor(ryw, kr);
}

Key SpecialKeySpace::getManagementApiCommandOptionSpecialKey(const std::string& command, const std::string& option) {
	Key prefix = "options/"_sr.withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin);
	auto pair = command + "/" + option;
	ASSERT(options.find(pair) != options.end());
	return prefix.withSuffix(pair);
}

ManagementCommandsOptionsImpl::ManagementCommandsOptionsImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

Future<RangeResult> ManagementCommandsOptionsImpl::getRange(ReadYourWritesTransaction* ryw,
                                                            KeyRangeRef kr,
                                                            GetRangeLimits limitsHint) const {
	RangeResult result;
	// Since we only have limit number of options, a brute force loop here is enough
	for (const auto& option : SpecialKeySpace::getManagementApiOptionsSet()) {
		auto key = getKeyRange().begin.withSuffix(option);
		// ignore all invalid keys
		auto r = ryw->getSpecialKeySpaceWriteMap()[key];
		if (kr.contains(key) && r.first && r.second.present()) {
			result.push_back(result.arena(), KeyValueRef(key, ValueRef()));
			result.arena().dependsOn(key.arena());
		}
	}
	return result;
}

void ManagementCommandsOptionsImpl::set(ReadYourWritesTransaction* ryw, const KeyRef& key, const ValueRef& value) {
	std::string option = key.removePrefix(getKeyRange().begin).toString();
	// ignore all invalid keys
	if (SpecialKeySpace::getManagementApiOptionsSet().find(option) !=
	    SpecialKeySpace::getManagementApiOptionsSet().end()) {
		TraceEvent(SevDebug, "ManagementApiOption").detail("Option", option).detail("Key", key);
		ryw->getSpecialKeySpaceWriteMap().insert(key, std::make_pair(true, Optional<Value>(value)));
	}
}

void ManagementCommandsOptionsImpl::clear(ReadYourWritesTransaction* ryw, const KeyRangeRef& range) {
	ryw->getSpecialKeySpaceWriteMap().rawErase(range);
}

void ManagementCommandsOptionsImpl::clear(ReadYourWritesTransaction* ryw, const KeyRef& key) {
	std::string option = key.removePrefix(getKeyRange().begin).toString();
	// ignore all invalid keys
	if (SpecialKeySpace::getManagementApiOptionsSet().find(option) !=
	    SpecialKeySpace::getManagementApiOptionsSet().end()) {
		ryw->getSpecialKeySpaceWriteMap().rawErase(singleKeyRange(key));
	}
}

Future<Optional<std::string>> ManagementCommandsOptionsImpl::commit(ReadYourWritesTransaction* ryw) {
	// Nothing to do, keys should be used by other impls' commit callback
	return Optional<std::string>();
}

RangeResult rywGetRange(ReadYourWritesTransaction* ryw, const KeyRangeRef& kr, const RangeResult& res) {
	// "res" is the read result regardless of your writes, if ryw disabled, return immediately
	if (ryw->readYourWritesDisabled())
		return res;
	// If ryw enabled, we update it with writes from the transaction
	RangeResult result;
	RangeMap<Key, std::pair<bool, Optional<Value>>, KeyRangeRef>::Ranges ranges =
	    ryw->getSpecialKeySpaceWriteMap().containedRanges(kr);
	RangeMap<Key, std::pair<bool, Optional<Value>>, KeyRangeRef>::iterator iter = ranges.begin();
	auto iter2 = res.begin();
	result.arena().dependsOn(res.arena());
	while (iter != ranges.end() || iter2 != res.end()) {
		if (iter == ranges.end()) {
			result.push_back(result.arena(), KeyValueRef(iter2->key, iter2->value));
			++iter2;
		} else if (iter2 == res.end()) {
			// insert if it is a set entry
			std::pair<bool, Optional<Value>> entry = iter->value();
			if (entry.first && entry.second.present()) {
				result.push_back_deep(result.arena(), KeyValueRef(iter->begin(), entry.second.get()));
			}
			++iter;
		} else if (iter->range().contains(iter2->key)) {
			std::pair<bool, Optional<Value>> entry = iter->value();
			// if this is a valid range either for set or clear, move iter2 outside the range
			if (entry.first) {
				// insert if this is a set entry
				if (entry.second.present())
					result.push_back_deep(result.arena(), KeyValueRef(iter->begin(), entry.second.get()));
				// move iter2 outside the range
				while (iter2 != res.end() && iter->range().contains(iter2->key))
					++iter2;
			}
			++iter;
		} else if (iter->begin() > iter2->key) {
			result.push_back(result.arena(), KeyValueRef(iter2->key, iter2->value));
			++iter2;
		} else if (iter->end() <= iter2->key) {
			// insert if it is a set entry
			std::pair<bool, Optional<Value>> entry = iter->value();
			if (entry.first && entry.second.present()) {
				result.push_back_deep(result.arena(), KeyValueRef(iter->begin(), entry.second.get()));
			}
			++iter;
		}
	}
	return result;
}

// read from those readwrite modules in which special keys have one-to-one mapping with real persisted keys
															#line 3746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via rwModuleWithMappingGetRangeActor()
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class RwModuleWithMappingGetRangeActorActor>
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class RwModuleWithMappingGetRangeActorActorState {
															#line 3753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RwModuleWithMappingGetRangeActorActorState(ReadYourWritesTransaction* const& ryw,const SpecialKeyRangeRWImpl* const& impl,KeyRangeRef const& kr) 
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   impl(impl),
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 3764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("rwModuleWithMappingGetRangeActor", reinterpret_cast<unsigned long>(this));

	}
	~RwModuleWithMappingGetRangeActorActorState() 
	{
		fdb_probe_actor_destroy("rwModuleWithMappingGetRangeActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = ryw->getTransaction().getRange(ryw->getDatabase()->specialKeySpace->decode(kr), CLIENT_KNOBS->TOO_MANY);
															#line 878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->actor_wait_state = 1;
															#line 878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RwModuleWithMappingGetRangeActorActor, 0, RangeResult >*>(static_cast<RwModuleWithMappingGetRangeActorActor*>(this)));
															#line 3786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~RwModuleWithMappingGetRangeActorActorState();
		static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& resultWithoutPrefix,int loopDepth) 
	{
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(!resultWithoutPrefix.more && resultWithoutPrefix.size() < CLIENT_KNOBS->TOO_MANY);
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( const KeyValueRef& kv : resultWithoutPrefix ) {
															#line 883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(impl->encode(kv.key), kv.value));
															#line 3815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~RwModuleWithMappingGetRangeActorActorState(); static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 3819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(rywGetRange(ryw, kr, result));
		this->~RwModuleWithMappingGetRangeActorActorState();
		static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && resultWithoutPrefix,int loopDepth) 
	{
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(!resultWithoutPrefix.more && resultWithoutPrefix.size() < CLIENT_KNOBS->TOO_MANY);
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( const KeyValueRef& kv : resultWithoutPrefix ) {
															#line 883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(impl->encode(kv.key), kv.value));
															#line 3837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~RwModuleWithMappingGetRangeActorActorState(); static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 3841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(rywGetRange(ryw, kr, result));
		this->~RwModuleWithMappingGetRangeActorActorState();
		static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& resultWithoutPrefix,int loopDepth) 
	{
		loopDepth = a_body1cont1(resultWithoutPrefix, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && resultWithoutPrefix,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(resultWithoutPrefix), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->actor_wait_state > 0) static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->ActorCallback< RwModuleWithMappingGetRangeActorActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< RwModuleWithMappingGetRangeActorActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("rwModuleWithMappingGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("rwModuleWithMappingGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RwModuleWithMappingGetRangeActorActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("rwModuleWithMappingGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("rwModuleWithMappingGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RwModuleWithMappingGetRangeActorActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("rwModuleWithMappingGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("rwModuleWithMappingGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	const SpecialKeyRangeRWImpl* impl;
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 3918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via rwModuleWithMappingGetRangeActor()
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class RwModuleWithMappingGetRangeActorActor final : public Actor<RangeResult>, public ActorCallback< RwModuleWithMappingGetRangeActorActor, 0, RangeResult >, public FastAllocated<RwModuleWithMappingGetRangeActorActor>, public RwModuleWithMappingGetRangeActorActorState<RwModuleWithMappingGetRangeActorActor> {
															#line 3923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<RwModuleWithMappingGetRangeActorActor>::operator new;
	using FastAllocated<RwModuleWithMappingGetRangeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RwModuleWithMappingGetRangeActorActor, 0, RangeResult >;
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RwModuleWithMappingGetRangeActorActor(ReadYourWritesTransaction* const& ryw,const SpecialKeyRangeRWImpl* const& impl,KeyRangeRef const& kr) 
															#line 3934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   RwModuleWithMappingGetRangeActorActorState<RwModuleWithMappingGetRangeActorActor>(ryw, impl, kr)
	{
		fdb_probe_actor_enter("rwModuleWithMappingGetRangeActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("rwModuleWithMappingGetRangeActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("rwModuleWithMappingGetRangeActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RwModuleWithMappingGetRangeActorActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> rwModuleWithMappingGetRangeActor( ReadYourWritesTransaction* const& ryw, const SpecialKeyRangeRWImpl* const& impl, KeyRangeRef const& kr ) {
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new RwModuleWithMappingGetRangeActorActor(ryw, impl, kr));
															#line 3962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

ExcludeServersRangeImpl::ExcludeServersRangeImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

Future<RangeResult> ExcludeServersRangeImpl::getRange(ReadYourWritesTransaction* ryw,
                                                      KeyRangeRef kr,
                                                      GetRangeLimits limitsHint) const {
	ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
	return rwModuleWithMappingGetRangeActor(ryw, this, kr);
}

void ExcludeServersRangeImpl::set(ReadYourWritesTransaction* ryw, const KeyRef& key, const ValueRef& value) {
	// ignore value
	ryw->getSpecialKeySpaceWriteMap().insert(key, std::make_pair(true, Optional<Value>(ValueRef())));
}

Key ExcludeServersRangeImpl::decode(const KeyRef& key) const {
	return key.removePrefix(SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin)
	    .withPrefix("\xff/conf/"_sr);
}

Key ExcludeServersRangeImpl::encode(const KeyRef& key) const {
	return key.removePrefix("\xff/conf/"_sr)
	    .withPrefix(SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin);
}

bool parseNetWorkAddrFromKeys(ReadYourWritesTransaction* ryw,
                              bool failed,
                              std::vector<AddressExclusion>& addresses,
                              std::set<AddressExclusion>& exclusions,
                              Optional<std::string>& msg) {
	KeyRangeRef range = failed ? SpecialKeySpace::getManagementApiCommandRange("failed")
	                           : SpecialKeySpace::getManagementApiCommandRange("exclude");
	auto ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(range);
	auto iter = ranges.begin();
	while (iter != ranges.end()) {
		auto entry = iter->value();
		// only check for exclude(set) operation, include(clear) are not checked
		TraceEvent(SevDebug, "ParseNetworkAddress")
		    .detail("Valid", entry.first)
		    .detail("Set", entry.second.present())
		    .detail("Key", iter->begin().toString());
		if (entry.first && entry.second.present()) {
			Key address = iter->begin().removePrefix(range.begin);
			auto a = AddressExclusion::parse(address);
			if (!a.isValid()) {
				std::string error = "ERROR: \'" + address.toString() + "\' is not a valid network endpoint address\n";
				if (address.toString().find(":tls") != std::string::npos)
					error += "        Do not include the `:tls' suffix when naming a process\n";
				msg = ManagementAPIError::toJsonString(
				    false, entry.second.present() ? (failed ? "exclude failed" : "exclude") : "include", error);
				return false;
			}
			addresses.push_back(a);
			exclusions.insert(a);
		}
		++iter;
	}
	return true;
}

															#line 4026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via checkExclusion()
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class CheckExclusionActor>
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class CheckExclusionActorState {
															#line 4033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	CheckExclusionActorState(Database const& db,std::vector<AddressExclusion>* const& addresses,std::set<AddressExclusion>* const& exclusions,bool const& markFailed,Optional<std::string>* const& msg) 
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : db(db),
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   addresses(addresses),
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   exclusions(exclusions),
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   markFailed(markFailed),
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   msg(msg)
															#line 4048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("checkExclusion", reinterpret_cast<unsigned long>(this));

	}
	~CheckExclusionActorState() 
	{
		fdb_probe_actor_destroy("checkExclusion", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (markFailed)
															#line 4063 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				safe = bool();
															#line 4067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				try {
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					StrictFuture<bool> __when_expr_0 = checkSafeExclusions(db, *addresses);
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (static_cast<CheckExclusionActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 4073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<CheckExclusionActor*>(this)->actor_wait_state = 1;
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckExclusionActor, 0, bool >*>(static_cast<CheckExclusionActor*>(this)));
															#line 4078 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					loopDepth = 0;
				}
				catch (Error& error) {
					loopDepth = a_body1Catch2(error, loopDepth);
				} catch (...) {
					loopDepth = a_body1Catch2(unknown_error(), loopDepth);
				}
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
		this->~CheckExclusionActorState();
		static_cast<CheckExclusionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<StatusObject> __when_expr_1 = StatusClient::statusFetcher(db);
															#line 974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<CheckExclusionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CheckExclusionActor*>(this)->actor_wait_state = 2;
															#line 974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckExclusionActor, 1, StatusObject >*>(static_cast<CheckExclusionActor*>(this)));
															#line 4119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
															#line 963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!safe)
															#line 4128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			std::string temp = "ERROR: It is unsafe to exclude the specified servers at this time.\n" "Please check that this exclusion does not bring down an entire storage team.\n" "Please also ensure that the exclusion will keep a majority of coordinators alive.\n" "You may add more storage processes or coordinators to make the operation safe.\n" "Call set(\"0xff0xff/management/failed/<ADDRESS...>\", ...) to exclude without " "performing safety checks.\n";
															#line 970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", temp);
															#line 971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
			this->~CheckExclusionActorState();
			static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 4151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 4155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			TraceEvent("CheckSafeExclusionsError").error(e);
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			safe = false;
															#line 4161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(bool const& _safe,int loopDepth) 
	{
															#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		safe = _safe;
															#line 4176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(bool && _safe,int loopDepth) 
	{
															#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		safe = _safe;
															#line 4185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1when1(bool const& _safe,int loopDepth) 
	{
		loopDepth = a_body1cont3(_safe, loopDepth);

		return loopDepth;
	}
	int a_body1when1(bool && _safe,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_safe), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CheckExclusionActor*>(this)->actor_wait_state > 0) static_cast<CheckExclusionActor*>(this)->actor_wait_state = 0;
		static_cast<CheckExclusionActor*>(this)->ActorCallback< CheckExclusionActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< CheckExclusionActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("checkExclusion", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkExclusion", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckExclusionActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("checkExclusion", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkExclusion", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckExclusionActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("checkExclusion", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkExclusion", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont5(int loopDepth) 
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
	int a_body1cont8(StatusObject const& status,int loopDepth) 
	{
															#line 975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		errorString = "ERROR: Could not calculate the impact of this exclude on the total available space in the cluster.\n" "Please try the exclude again in 30 seconds.\n" "Call set(\"0xff0xff/management/options/exclude/force\", ...) first to exclude without checking available " "space.\n";
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StatusObjectReader statusObj(status);
															#line 983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StatusObjectReader statusObjCluster;
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!statusObj.get("cluster", statusObjCluster))
															#line 4276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
			this->~CheckExclusionActorState();
			static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StatusObjectReader processesMap;
															#line 990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!statusObjCluster.get("processes", processesMap))
															#line 4292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
			this->~CheckExclusionActorState();
			static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ssTotalCount = 0;
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ssExcludedCount = 0;
															#line 998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		diskLocalities = std::unordered_set<std::string>();
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreFreeBytes = 0;
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreFreeBytesNotExcluded = 0;
															#line 1001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreUsedBytes = 0;
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreUsedBytesNotExcluded = 0;
															#line 1003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreAvailableBytes = 0;
															#line 1005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		excludedAddressesContainsStorageRole = false;
															#line 4322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		try {
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( auto proc : processesMap.obj() ) {
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				StatusObjectReader process(proc.second);
															#line 1010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				std::string addrStr;
															#line 1011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (!process.get("address", addrStr))
															#line 4332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 1013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
					this->~CheckExclusionActorState();
					static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 1015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				NetworkAddress addr = NetworkAddress::parse(addrStr);
															#line 1016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				bool includedInExclusion = addressExcluded(*exclusions, addr);
															#line 1017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				bool excluded = (process.has("excluded") && process.last().get_bool()) || includedInExclusion;
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				StatusObjectReader localityObj;
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				std::string disk_id;
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (process.get("locality", localityObj))
															#line 4356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					process.get("disk_id", disk_id);
															#line 4360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
															#line 1025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				StatusArray rolesArray = proc.second.get_obj()["roles"].get_array();
															#line 1026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				for( StatusObjectReader role : rolesArray ) {
															#line 1027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (role["role"].get_str() == "storage")
															#line 4368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 1028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						ssTotalCount++;
															#line 1029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						if (excluded)
															#line 4374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							ssExcludedCount++;
															#line 4378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						if (!excludedAddressesContainsStorageRole && includedInExclusion)
															#line 4382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							excludedAddressesContainsStorageRole = true;
															#line 4386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
															#line 1039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						int64_t used_bytes;
															#line 1040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						if (!role.get("kvstore_used_bytes", used_bytes))
															#line 4392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							*msg = ManagementAPIError::toJsonString( false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 1043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
							new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
							this->~CheckExclusionActorState();
							static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
							return 0;
						}
															#line 1046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						int64_t free_bytes;
															#line 1047 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						if (!role.get("kvstore_free_bytes", free_bytes))
															#line 4408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							*msg = ManagementAPIError::toJsonString( false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 1050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
							new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
							this->~CheckExclusionActorState();
							static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
							return 0;
						}
															#line 1053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						int64_t available_bytes;
															#line 1054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						if (!role.get("kvstore_available_bytes", available_bytes))
															#line 4424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1055 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							*msg = ManagementAPIError::toJsonString( false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 1057 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
							new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
							this->~CheckExclusionActorState();
							static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
							return 0;
						}
															#line 1060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						totalKvStoreUsedBytes += used_bytes;
															#line 1061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						totalKvStoreFreeBytes += free_bytes;
															#line 1062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						totalKvStoreAvailableBytes += available_bytes;
															#line 1064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						if (!excluded)
															#line 4444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1065 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							totalKvStoreUsedBytesNotExcluded += used_bytes;
															#line 1067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							if (disk_id.empty() || diskLocalities.find(disk_id) == diskLocalities.end())
															#line 4450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
							{
															#line 1068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
								totalKvStoreFreeBytesNotExcluded += free_bytes;
															#line 1069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
								if (!disk_id.empty())
															#line 4456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
								{
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
									diskLocalities.insert(disk_id);
															#line 4460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
								}
							}
						}
					}
				}
			}
			loopDepth = a_body1cont26(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1cont8Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont8Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont8(StatusObject && status,int loopDepth) 
	{
															#line 975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		errorString = "ERROR: Could not calculate the impact of this exclude on the total available space in the cluster.\n" "Please try the exclude again in 30 seconds.\n" "Call set(\"0xff0xff/management/options/exclude/force\", ...) first to exclude without checking available " "space.\n";
															#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StatusObjectReader statusObj(status);
															#line 983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StatusObjectReader statusObjCluster;
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!statusObj.get("cluster", statusObjCluster))
															#line 4487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
			this->~CheckExclusionActorState();
			static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StatusObjectReader processesMap;
															#line 990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!statusObjCluster.get("processes", processesMap))
															#line 4503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
			this->~CheckExclusionActorState();
			static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ssTotalCount = 0;
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ssExcludedCount = 0;
															#line 998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		diskLocalities = std::unordered_set<std::string>();
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreFreeBytes = 0;
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreFreeBytesNotExcluded = 0;
															#line 1001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreUsedBytes = 0;
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreUsedBytesNotExcluded = 0;
															#line 1003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreAvailableBytes = 0;
															#line 1005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		excludedAddressesContainsStorageRole = false;
															#line 4533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		try {
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( auto proc : processesMap.obj() ) {
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				StatusObjectReader process(proc.second);
															#line 1010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				std::string addrStr;
															#line 1011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (!process.get("address", addrStr))
															#line 4543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 1013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
					this->~CheckExclusionActorState();
					static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 1015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				NetworkAddress addr = NetworkAddress::parse(addrStr);
															#line 1016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				bool includedInExclusion = addressExcluded(*exclusions, addr);
															#line 1017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				bool excluded = (process.has("excluded") && process.last().get_bool()) || includedInExclusion;
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				StatusObjectReader localityObj;
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				std::string disk_id;
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (process.get("locality", localityObj))
															#line 4567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					process.get("disk_id", disk_id);
															#line 4571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
															#line 1025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				StatusArray rolesArray = proc.second.get_obj()["roles"].get_array();
															#line 1026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				for( StatusObjectReader role : rolesArray ) {
															#line 1027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (role["role"].get_str() == "storage")
															#line 4579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 1028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						ssTotalCount++;
															#line 1029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						if (excluded)
															#line 4585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							ssExcludedCount++;
															#line 4589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						if (!excludedAddressesContainsStorageRole && includedInExclusion)
															#line 4593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							excludedAddressesContainsStorageRole = true;
															#line 4597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
															#line 1039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						int64_t used_bytes;
															#line 1040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						if (!role.get("kvstore_used_bytes", used_bytes))
															#line 4603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							*msg = ManagementAPIError::toJsonString( false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 1043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
							new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
							this->~CheckExclusionActorState();
							static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
							return 0;
						}
															#line 1046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						int64_t free_bytes;
															#line 1047 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						if (!role.get("kvstore_free_bytes", free_bytes))
															#line 4619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							*msg = ManagementAPIError::toJsonString( false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 1050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
							new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
							this->~CheckExclusionActorState();
							static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
							return 0;
						}
															#line 1053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						int64_t available_bytes;
															#line 1054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						if (!role.get("kvstore_available_bytes", available_bytes))
															#line 4635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1055 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							*msg = ManagementAPIError::toJsonString( false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 1057 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
							new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
							this->~CheckExclusionActorState();
							static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
							return 0;
						}
															#line 1060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						totalKvStoreUsedBytes += used_bytes;
															#line 1061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						totalKvStoreFreeBytes += free_bytes;
															#line 1062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						totalKvStoreAvailableBytes += available_bytes;
															#line 1064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						if (!excluded)
															#line 4655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1065 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							totalKvStoreUsedBytesNotExcluded += used_bytes;
															#line 1067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							if (disk_id.empty() || diskLocalities.find(disk_id) == diskLocalities.end())
															#line 4661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
							{
															#line 1068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
								totalKvStoreFreeBytesNotExcluded += free_bytes;
															#line 1069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
								if (!disk_id.empty())
															#line 4667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
								{
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
									diskLocalities.insert(disk_id);
															#line 4671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
								}
							}
						}
					}
				}
			}
			loopDepth = a_body1cont26(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1cont8Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont8Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1when1(StatusObject const& status,int loopDepth) 
	{
		loopDepth = a_body1cont8(status, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(StatusObject && status,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(status), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CheckExclusionActor*>(this)->actor_wait_state > 0) static_cast<CheckExclusionActor*>(this)->actor_wait_state = 0;
		static_cast<CheckExclusionActor*>(this)->ActorCallback< CheckExclusionActor, 1, StatusObject >::remove();

	}
	void a_callback_fire(ActorCallback< CheckExclusionActor, 1, StatusObject >*,StatusObject const& value) 
	{
		fdb_probe_actor_enter("checkExclusion", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkExclusion", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CheckExclusionActor, 1, StatusObject >*,StatusObject && value) 
	{
		fdb_probe_actor_enter("checkExclusion", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkExclusion", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CheckExclusionActor, 1, StatusObject >*,Error err) 
	{
		fdb_probe_actor_enter("checkExclusion", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkExclusion", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont9(int loopDepth) 
	{
															#line 1085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!excludedAddressesContainsStorageRole)
															#line 4755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(true); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(true);
			this->~CheckExclusionActorState();
			static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		double finalUnavailableRatio = (double)(totalKvStoreUsedBytes + totalKvStoreFreeBytes - totalKvStoreAvailableBytes) / std::max((double)(totalKvStoreUsedBytesNotExcluded + totalKvStoreFreeBytesNotExcluded), (double)1);
															#line 1096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		TraceEvent(SevInfo, "CheckExclusionDetails") .detail("SsTotalCount", ssTotalCount) .detail("SsExcludedCount", ssExcludedCount) .detail("FinalUnavailableRatio", finalUnavailableRatio) .detail("TotalKvStoreUsedBytes", totalKvStoreUsedBytes) .detail("TotalKvStoreFreeBytes", totalKvStoreFreeBytes) .detail("TotalKvStoreAvailableBytes", totalKvStoreAvailableBytes) .detail("TotalKvStoreUsedBytesNotExcluded", totalKvStoreUsedBytesNotExcluded) .detail("TotalKvStoreFreeBytesNotExcluded", totalKvStoreFreeBytesNotExcluded);
															#line 1106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (ssExcludedCount == ssTotalCount || finalUnavailableRatio > 0.9)
															#line 4771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			std::string temp = "ERROR: This exclude may cause the total available space in the cluster to drop below 10%.\n" "Call set(\"0xff0xff/management/options/exclude/force\", ...) first to exclude without " "checking available space.\n";
															#line 1110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", temp);
															#line 1111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
			this->~CheckExclusionActorState();
			static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(true); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(true);
		this->~CheckExclusionActorState();
		static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont8Catch1(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 1079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
			this->~CheckExclusionActorState();
			static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont26(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont9(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Database db;
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<AddressExclusion>* addresses;
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::set<AddressExclusion>* exclusions;
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	bool markFailed;
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Optional<std::string>* msg;
															#line 953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	bool safe;
															#line 975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::string errorString;
															#line 995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	int ssTotalCount;
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	int ssExcludedCount;
															#line 998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::unordered_set<std::string> diskLocalities;
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	int64_t totalKvStoreFreeBytes;
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	int64_t totalKvStoreFreeBytesNotExcluded;
															#line 1001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	int64_t totalKvStoreUsedBytes;
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	int64_t totalKvStoreUsedBytesNotExcluded;
															#line 1003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	int64_t totalKvStoreAvailableBytes;
															#line 1005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	bool excludedAddressesContainsStorageRole;
															#line 4861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via checkExclusion()
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class CheckExclusionActor final : public Actor<bool>, public ActorCallback< CheckExclusionActor, 0, bool >, public ActorCallback< CheckExclusionActor, 1, StatusObject >, public FastAllocated<CheckExclusionActor>, public CheckExclusionActorState<CheckExclusionActor> {
															#line 4866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<CheckExclusionActor>::operator new;
	using FastAllocated<CheckExclusionActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckExclusionActor, 0, bool >;
friend struct ActorCallback< CheckExclusionActor, 1, StatusObject >;
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	CheckExclusionActor(Database const& db,std::vector<AddressExclusion>* const& addresses,std::set<AddressExclusion>* const& exclusions,bool const& markFailed,Optional<std::string>* const& msg) 
															#line 4878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<bool>(),
		   CheckExclusionActorState<CheckExclusionActor>(db, addresses, exclusions, markFailed, msg)
	{
		fdb_probe_actor_enter("checkExclusion", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkExclusion");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkExclusion", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckExclusionActor, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CheckExclusionActor, 1, StatusObject >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<bool> checkExclusion( Database const& db, std::vector<AddressExclusion>* const& addresses, std::set<AddressExclusion>* const& exclusions, bool const& markFailed, Optional<std::string>* const& msg ) {
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<bool>(new CheckExclusionActor(db, addresses, exclusions, markFailed, msg));
															#line 4907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

void includeServers(ReadYourWritesTransaction* ryw) {
	ryw->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
	ryw->setOption(FDBTransactionOptions::LOCK_AWARE);
	ryw->setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
	ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
	// includeServers might be used in an emergency transaction, so make sure it is retry-self-conflicting and
	// CAUSAL_WRITE_RISKY
	ryw->setOption(FDBTransactionOptions::CAUSAL_WRITE_RISKY);
	std::string versionKey = deterministicRandom()->randomUniqueID().toString();
	// for exluded servers
	auto ranges =
	    ryw->getSpecialKeySpaceWriteMap().containedRanges(SpecialKeySpace::getManagementApiCommandRange("exclude"));
	auto iter = ranges.begin();
	Transaction& tr = ryw->getTransaction();
	while (iter != ranges.end()) {
		auto entry = iter->value();
		if (entry.first && !entry.second.present()) {
			tr.addReadConflictRange(singleKeyRange(excludedServersVersionKey));
			tr.set(excludedServersVersionKey, versionKey);
			tr.clear(ryw->getDatabase()->specialKeySpace->decode(iter->range()));
		}
		++iter;
	}
	// for failed servers
	ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(SpecialKeySpace::getManagementApiCommandRange("failed"));
	iter = ranges.begin();
	while (iter != ranges.end()) {
		auto entry = iter->value();
		if (entry.first && !entry.second.present()) {
			tr.addReadConflictRange(singleKeyRange(failedServersVersionKey));
			tr.set(failedServersVersionKey, versionKey);
			tr.clear(ryw->getDatabase()->specialKeySpace->decode(iter->range()));
		}
		++iter;
	}
}

															#line 4949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via excludeCommitActor()
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class ExcludeCommitActorActor>
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class ExcludeCommitActorActorState {
															#line 4956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ExcludeCommitActorActorState(ReadYourWritesTransaction* const& ryw,bool const& failed) 
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   failed(failed),
															#line 1156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   result(),
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   addresses(),
															#line 1158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   exclusions()
															#line 4971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("excludeCommitActor", reinterpret_cast<unsigned long>(this));

	}
	~ExcludeCommitActorActorState() 
	{
		fdb_probe_actor_destroy("excludeCommitActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!parseNetWorkAddrFromKeys(ryw, failed, addresses, exclusions, result))
															#line 4986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (!static_cast<ExcludeCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeCommitActorActorState(); static_cast<ExcludeCommitActorActor*>(this)->destroy(); return 0; }
															#line 4990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				new (&static_cast<ExcludeCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(result)); // state_var_RVO
				this->~ExcludeCommitActorActorState();
				static_cast<ExcludeCommitActorActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			auto force = ryw->getSpecialKeySpaceWriteMap()[SpecialKeySpace::getManagementApiCommandOptionSpecialKey( failed ? "failed" : "excluded", "force")];
															#line 1165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (addresses.size() && !(force.first && force.second.present()))
															#line 5000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				StrictFuture<bool> __when_expr_0 = checkExclusion(ryw->getDatabase(), &addresses, &exclusions, failed, &result);
															#line 1166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (static_cast<ExcludeCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ExcludeCommitActorActor*>(this)->actor_wait_state = 1;
															#line 1166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExcludeCommitActorActor, 0, bool >*>(static_cast<ExcludeCommitActorActor*>(this)));
															#line 5011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~ExcludeCommitActorActorState();
		static_cast<ExcludeCommitActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Void> __when_expr_1 = excludeServers(&(ryw->getTransaction()), addresses, failed);
															#line 1170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ExcludeCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ExcludeCommitActorActor*>(this)->actor_wait_state = 2;
															#line 1170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExcludeCommitActorActor, 1, Void >*>(static_cast<ExcludeCommitActorActor*>(this)));
															#line 5046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(bool const& safe,int loopDepth) 
	{
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!safe)
															#line 5055 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<ExcludeCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeCommitActorActorState(); static_cast<ExcludeCommitActorActor*>(this)->destroy(); return 0; }
															#line 5059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<ExcludeCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(result)); // state_var_RVO
			this->~ExcludeCommitActorActorState();
			static_cast<ExcludeCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(bool && safe,int loopDepth) 
	{
															#line 1167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!safe)
															#line 5073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<ExcludeCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeCommitActorActorState(); static_cast<ExcludeCommitActorActor*>(this)->destroy(); return 0; }
															#line 5077 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<ExcludeCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(result)); // state_var_RVO
			this->~ExcludeCommitActorActorState();
			static_cast<ExcludeCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(bool const& safe,int loopDepth) 
	{
		loopDepth = a_body1cont3(safe, loopDepth);

		return loopDepth;
	}
	int a_body1when1(bool && safe,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(safe), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ExcludeCommitActorActor*>(this)->actor_wait_state > 0) static_cast<ExcludeCommitActorActor*>(this)->actor_wait_state = 0;
		static_cast<ExcludeCommitActorActor*>(this)->ActorCallback< ExcludeCommitActorActor, 0, bool >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeCommitActorActor, 0, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("excludeCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ExcludeCommitActorActor, 0, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("excludeCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ExcludeCommitActorActor, 0, bool >*,Error err) 
	{
		fdb_probe_actor_enter("excludeCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		includeServers(ryw);
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ExcludeCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeCommitActorActorState(); static_cast<ExcludeCommitActorActor*>(this)->destroy(); return 0; }
															#line 5156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ExcludeCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(result)); // state_var_RVO
		this->~ExcludeCommitActorActorState();
		static_cast<ExcludeCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		includeServers(ryw);
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ExcludeCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeCommitActorActorState(); static_cast<ExcludeCommitActorActor*>(this)->destroy(); return 0; }
															#line 5170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ExcludeCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(result)); // state_var_RVO
		this->~ExcludeCommitActorActorState();
		static_cast<ExcludeCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ExcludeCommitActorActor*>(this)->actor_wait_state > 0) static_cast<ExcludeCommitActorActor*>(this)->actor_wait_state = 0;
		static_cast<ExcludeCommitActorActor*>(this)->ActorCallback< ExcludeCommitActorActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeCommitActorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("excludeCommitActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeCommitActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ExcludeCommitActorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("excludeCommitActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeCommitActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ExcludeCommitActorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("excludeCommitActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeCommitActor", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	bool failed;
															#line 1156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Optional<std::string> result;
															#line 1157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<AddressExclusion> addresses;
															#line 1158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::set<AddressExclusion> exclusions;
															#line 5251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via excludeCommitActor()
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class ExcludeCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< ExcludeCommitActorActor, 0, bool >, public ActorCallback< ExcludeCommitActorActor, 1, Void >, public FastAllocated<ExcludeCommitActorActor>, public ExcludeCommitActorActorState<ExcludeCommitActorActor> {
															#line 5256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<ExcludeCommitActorActor>::operator new;
	using FastAllocated<ExcludeCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ExcludeCommitActorActor, 0, bool >;
friend struct ActorCallback< ExcludeCommitActorActor, 1, Void >;
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ExcludeCommitActorActor(ReadYourWritesTransaction* const& ryw,bool const& failed) 
															#line 5268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<Optional<std::string>>(),
		   ExcludeCommitActorActorState<ExcludeCommitActorActor>(ryw, failed)
	{
		fdb_probe_actor_enter("excludeCommitActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("excludeCommitActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("excludeCommitActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ExcludeCommitActorActor, 0, bool >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ExcludeCommitActorActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Optional<std::string>> excludeCommitActor( ReadYourWritesTransaction* const& ryw, bool const& failed ) {
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new ExcludeCommitActorActor(ryw, failed));
															#line 5297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

Future<Optional<std::string>> ExcludeServersRangeImpl::commit(ReadYourWritesTransaction* ryw) {
	return excludeCommitActor(ryw, false);
}

FailedServersRangeImpl::FailedServersRangeImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

Future<RangeResult> FailedServersRangeImpl::getRange(ReadYourWritesTransaction* ryw,
                                                     KeyRangeRef kr,
                                                     GetRangeLimits limitsHint) const {
	ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
	return rwModuleWithMappingGetRangeActor(ryw, this, kr);
}

void FailedServersRangeImpl::set(ReadYourWritesTransaction* ryw, const KeyRef& key, const ValueRef& value) {
	// ignore value
	ryw->getSpecialKeySpaceWriteMap().insert(key, std::make_pair(true, Optional<Value>(ValueRef())));
}

Key FailedServersRangeImpl::decode(const KeyRef& key) const {
	return key.removePrefix(SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin)
	    .withPrefix("\xff/conf/"_sr);
}

Key FailedServersRangeImpl::encode(const KeyRef& key) const {
	return key.removePrefix("\xff/conf/"_sr)
	    .withPrefix(SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin);
}

Future<Optional<std::string>> FailedServersRangeImpl::commit(ReadYourWritesTransaction* ryw) {
	return excludeCommitActor(ryw, true);
}

															#line 5334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via ExclusionInProgressActor()
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class ExclusionInProgressActorActor>
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class ExclusionInProgressActorActorState {
															#line 5341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ExclusionInProgressActorActorState(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   prefix(prefix),
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr),
															#line 1209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   result(),
															#line 1210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   tr(ryw->getTransaction())
															#line 5356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this));

	}
	~ExclusionInProgressActorActorState() 
	{
		fdb_probe_actor_destroy("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			tr.setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<std::vector<AddressExclusion>> __when_expr_0 = (getAllExcludedServers(&tr));
															#line 1215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state = 1;
															#line 1215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExclusionInProgressActorActor, 0, std::vector<AddressExclusion> >*>(static_cast<ExclusionInProgressActorActor*>(this)));
															#line 5384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~ExclusionInProgressActorActorState();
		static_cast<ExclusionInProgressActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		exclusions = std::set<AddressExclusion>(excl.begin(), excl.end());
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		inProgressExclusion = std::set<NetworkAddress>();
															#line 1220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr.getRange(serverListKeys, CLIENT_KNOBS->TOO_MANY);
															#line 1220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state = 2;
															#line 1220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExclusionInProgressActorActor, 1, RangeResult >*>(static_cast<ExclusionInProgressActorActor*>(this)));
															#line 5418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<AddressExclusion> const& __excl,int loopDepth) 
	{
															#line 1215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		excl = __excl;
															#line 5427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<AddressExclusion> && __excl,int loopDepth) 
	{
		excl = std::move(__excl);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state > 0) static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state = 0;
		static_cast<ExclusionInProgressActorActor*>(this)->ActorCallback< ExclusionInProgressActorActor, 0, std::vector<AddressExclusion> >::remove();

	}
	void a_callback_fire(ActorCallback< ExclusionInProgressActorActor, 0, std::vector<AddressExclusion> >*,std::vector<AddressExclusion> const& value) 
	{
		fdb_probe_actor_enter("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ExclusionInProgressActorActor, 0, std::vector<AddressExclusion> >*,std::vector<AddressExclusion> && value) 
	{
		fdb_probe_actor_enter("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ExclusionInProgressActorActor, 0, std::vector<AddressExclusion> >*,Error err) 
	{
		fdb_probe_actor_enter("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(!serverList.more && serverList.size() < CLIENT_KNOBS->TOO_MANY);
															#line 1223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto& s : serverList ) {
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			auto addresses = decodeServerListValue(s.value).getKeyValues.getEndpoint().addresses;
															#line 1225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (addressExcluded(exclusions, addresses.address))
															#line 5500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				inProgressExclusion.insert(addresses.address);
															#line 5504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 1228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (addresses.secondaryAddress.present() && addressExcluded(exclusions, addresses.secondaryAddress.get()))
															#line 5508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				inProgressExclusion.insert(addresses.secondaryAddress.get());
															#line 5512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Optional<Standalone<StringRef>>> __when_expr_2 = tr.get(logsKey);
															#line 1233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state = 3;
															#line 1233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExclusionInProgressActorActor, 2, Optional<Standalone<StringRef>> >*>(static_cast<ExclusionInProgressActorActor*>(this)));
															#line 5524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult const& __serverList,int loopDepth) 
	{
															#line 1220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		serverList = __serverList;
															#line 5533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult && __serverList,int loopDepth) 
	{
		serverList = std::move(__serverList);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state > 0) static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state = 0;
		static_cast<ExclusionInProgressActorActor*>(this)->ActorCallback< ExclusionInProgressActorActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ExclusionInProgressActorActor, 1, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ExclusionInProgressActorActor, 1, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ExclusionInProgressActorActor, 1, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Optional<Standalone<StringRef>> const& value,int loopDepth) 
	{
															#line 1234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(value.present());
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto logs = decodeLogsValue(value.get());
															#line 1236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& log : logs.first ) {
															#line 1237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (log.second == NetworkAddress() || addressExcluded(exclusions, log.second))
															#line 5606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				inProgressExclusion.insert(log.second);
															#line 5610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& log : logs.second ) {
															#line 1242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (log.second == NetworkAddress() || addressExcluded(exclusions, log.second))
															#line 5617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				inProgressExclusion.insert(log.second);
															#line 5621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		std::set<std::string> inProgressAddresses;
															#line 1249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& address : inProgressExclusion ) {
															#line 1250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			inProgressAddresses.insert(formatIpPort(address.ip, address.port));
															#line 5630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& address : inProgressAddresses ) {
															#line 1254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			Key addrKey = prefix.withSuffix(address);
															#line 1255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.contains(addrKey))
															#line 5638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back(result.arena(), KeyValueRef(addrKey, ValueRef()));
															#line 1257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.arena().dependsOn(addrKey.arena());
															#line 5644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ExclusionInProgressActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~ExclusionInProgressActorActorState(); static_cast<ExclusionInProgressActorActor*>(this)->destroy(); return 0; }
															#line 5649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ExclusionInProgressActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
		this->~ExclusionInProgressActorActorState();
		static_cast<ExclusionInProgressActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Optional<Standalone<StringRef>> && value,int loopDepth) 
	{
															#line 1234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(value.present());
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto logs = decodeLogsValue(value.get());
															#line 1236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& log : logs.first ) {
															#line 1237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (log.second == NetworkAddress() || addressExcluded(exclusions, log.second))
															#line 5667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				inProgressExclusion.insert(log.second);
															#line 5671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& log : logs.second ) {
															#line 1242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (log.second == NetworkAddress() || addressExcluded(exclusions, log.second))
															#line 5678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				inProgressExclusion.insert(log.second);
															#line 5682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		std::set<std::string> inProgressAddresses;
															#line 1249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& address : inProgressExclusion ) {
															#line 1250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			inProgressAddresses.insert(formatIpPort(address.ip, address.port));
															#line 5691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& address : inProgressAddresses ) {
															#line 1254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			Key addrKey = prefix.withSuffix(address);
															#line 1255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.contains(addrKey))
															#line 5699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back(result.arena(), KeyValueRef(addrKey, ValueRef()));
															#line 1257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.arena().dependsOn(addrKey.arena());
															#line 5705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ExclusionInProgressActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~ExclusionInProgressActorActorState(); static_cast<ExclusionInProgressActorActor*>(this)->destroy(); return 0; }
															#line 5710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ExclusionInProgressActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
		this->~ExclusionInProgressActorActorState();
		static_cast<ExclusionInProgressActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(Optional<Standalone<StringRef>> const& value,int loopDepth) 
	{
		loopDepth = a_body1cont3(value, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Optional<Standalone<StringRef>> && value,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(value), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state > 0) static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state = 0;
		static_cast<ExclusionInProgressActorActor*>(this)->ActorCallback< ExclusionInProgressActorActor, 2, Optional<Standalone<StringRef>> >::remove();

	}
	void a_callback_fire(ActorCallback< ExclusionInProgressActorActor, 2, Optional<Standalone<StringRef>> >*,Optional<Standalone<StringRef>> const& value) 
	{
		fdb_probe_actor_enter("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ExclusionInProgressActorActor, 2, Optional<Standalone<StringRef>> >*,Optional<Standalone<StringRef>> && value) 
	{
		fdb_probe_actor_enter("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ExclusionInProgressActorActor, 2, Optional<Standalone<StringRef>> >*,Error err) 
	{
		fdb_probe_actor_enter("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef prefix;
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 1209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult result;
															#line 1210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Transaction& tr;
															#line 1215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<AddressExclusion> excl;
															#line 1216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::set<AddressExclusion> exclusions;
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::set<NetworkAddress> inProgressExclusion;
															#line 1220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult serverList;
															#line 5799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via ExclusionInProgressActor()
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class ExclusionInProgressActorActor final : public Actor<RangeResult>, public ActorCallback< ExclusionInProgressActorActor, 0, std::vector<AddressExclusion> >, public ActorCallback< ExclusionInProgressActorActor, 1, RangeResult >, public ActorCallback< ExclusionInProgressActorActor, 2, Optional<Standalone<StringRef>> >, public FastAllocated<ExclusionInProgressActorActor>, public ExclusionInProgressActorActorState<ExclusionInProgressActorActor> {
															#line 5804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<ExclusionInProgressActorActor>::operator new;
	using FastAllocated<ExclusionInProgressActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ExclusionInProgressActorActor, 0, std::vector<AddressExclusion> >;
friend struct ActorCallback< ExclusionInProgressActorActor, 1, RangeResult >;
friend struct ActorCallback< ExclusionInProgressActorActor, 2, Optional<Standalone<StringRef>> >;
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ExclusionInProgressActorActor(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 5817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   ExclusionInProgressActorActorState<ExclusionInProgressActorActor>(ryw, prefix, kr)
	{
		fdb_probe_actor_enter("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("ExclusionInProgressActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("ExclusionInProgressActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ExclusionInProgressActorActor, 0, std::vector<AddressExclusion> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ExclusionInProgressActorActor, 1, RangeResult >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ExclusionInProgressActorActor, 2, Optional<Standalone<StringRef>> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> ExclusionInProgressActor( ReadYourWritesTransaction* const& ryw, KeyRef const& prefix, KeyRangeRef const& kr ) {
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new ExclusionInProgressActorActor(ryw, prefix, kr));
															#line 5847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

ExclusionInProgressRangeImpl::ExclusionInProgressRangeImpl(KeyRangeRef kr) : SpecialKeyRangeAsyncImpl(kr) {}

Future<RangeResult> ExclusionInProgressRangeImpl::getRange(ReadYourWritesTransaction* ryw,
                                                           KeyRangeRef kr,
                                                           GetRangeLimits limitsHint) const {
	return ExclusionInProgressActor(ryw, getKeyRange().begin, kr);
}

															#line 5860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via getProcessClassActor()
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class GetProcessClassActorActor>
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GetProcessClassActorActorState {
															#line 5867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetProcessClassActorActorState(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   prefix(prefix),
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 5878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("getProcessClassActor", reinterpret_cast<unsigned long>(this));

	}
	~GetProcessClassActorActorState() 
	{
		fdb_probe_actor_destroy("getProcessClassActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<std::vector<ProcessData>> __when_expr_0 = getWorkers(&ryw->getTransaction());
															#line 1273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GetProcessClassActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetProcessClassActorActor*>(this)->actor_wait_state = 1;
															#line 1273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetProcessClassActorActor, 0, std::vector<ProcessData> >*>(static_cast<GetProcessClassActorActor*>(this)));
															#line 5902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~GetProcessClassActorActorState();
		static_cast<GetProcessClassActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<ProcessData> const& _workers,int loopDepth) 
	{
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto workers = _workers;
															#line 1276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		std::sort(workers.begin(), workers.end(), [](const ProcessData& lhs, const ProcessData& rhs) { return formatIpPort(lhs.address.ip, lhs.address.port) < formatIpPort(rhs.address.ip, rhs.address.port); });
															#line 1280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto last = std::unique(workers.begin(), workers.end(), [](const ProcessData& lhs, const ProcessData& rhs) { return formatIpPort(lhs.address.ip, lhs.address.port) == formatIpPort(rhs.address.ip, rhs.address.port); });
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		workers.erase(last, workers.end());
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto& w : workers ) {
															#line 1288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			KeyRef k(prefix.withSuffix(formatIpPort(w.address.ip, w.address.port), result.arena()));
															#line 1289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.contains(k))
															#line 5939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ValueRef v(result.arena(), w.processClass.toString());
															#line 1291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back(result.arena(), KeyValueRef(k, v));
															#line 5945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetProcessClassActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~GetProcessClassActorActorState(); static_cast<GetProcessClassActorActor*>(this)->destroy(); return 0; }
															#line 5950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetProcessClassActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(rywGetRange(ryw, kr, result));
		this->~GetProcessClassActorActorState();
		static_cast<GetProcessClassActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<ProcessData> && _workers,int loopDepth) 
	{
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto workers = _workers;
															#line 1276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		std::sort(workers.begin(), workers.end(), [](const ProcessData& lhs, const ProcessData& rhs) { return formatIpPort(lhs.address.ip, lhs.address.port) < formatIpPort(rhs.address.ip, rhs.address.port); });
															#line 1280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto last = std::unique(workers.begin(), workers.end(), [](const ProcessData& lhs, const ProcessData& rhs) { return formatIpPort(lhs.address.ip, lhs.address.port) == formatIpPort(rhs.address.ip, rhs.address.port); });
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		workers.erase(last, workers.end());
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto& w : workers ) {
															#line 1288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			KeyRef k(prefix.withSuffix(formatIpPort(w.address.ip, w.address.port), result.arena()));
															#line 1289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.contains(k))
															#line 5976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ValueRef v(result.arena(), w.processClass.toString());
															#line 1291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back(result.arena(), KeyValueRef(k, v));
															#line 5982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetProcessClassActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~GetProcessClassActorActorState(); static_cast<GetProcessClassActorActor*>(this)->destroy(); return 0; }
															#line 5987 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetProcessClassActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(rywGetRange(ryw, kr, result));
		this->~GetProcessClassActorActorState();
		static_cast<GetProcessClassActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<ProcessData> const& _workers,int loopDepth) 
	{
		loopDepth = a_body1cont1(_workers, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<ProcessData> && _workers,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_workers), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetProcessClassActorActor*>(this)->actor_wait_state > 0) static_cast<GetProcessClassActorActor*>(this)->actor_wait_state = 0;
		static_cast<GetProcessClassActorActor*>(this)->ActorCallback< GetProcessClassActorActor, 0, std::vector<ProcessData> >::remove();

	}
	void a_callback_fire(ActorCallback< GetProcessClassActorActor, 0, std::vector<ProcessData> >*,std::vector<ProcessData> const& value) 
	{
		fdb_probe_actor_enter("getProcessClassActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getProcessClassActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetProcessClassActorActor, 0, std::vector<ProcessData> >*,std::vector<ProcessData> && value) 
	{
		fdb_probe_actor_enter("getProcessClassActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getProcessClassActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetProcessClassActorActor, 0, std::vector<ProcessData> >*,Error err) 
	{
		fdb_probe_actor_enter("getProcessClassActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getProcessClassActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef prefix;
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 6064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getProcessClassActor()
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GetProcessClassActorActor final : public Actor<RangeResult>, public ActorCallback< GetProcessClassActorActor, 0, std::vector<ProcessData> >, public FastAllocated<GetProcessClassActorActor>, public GetProcessClassActorActorState<GetProcessClassActorActor> {
															#line 6069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GetProcessClassActorActor>::operator new;
	using FastAllocated<GetProcessClassActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetProcessClassActorActor, 0, std::vector<ProcessData> >;
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetProcessClassActorActor(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 6080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   GetProcessClassActorActorState<GetProcessClassActorActor>(ryw, prefix, kr)
	{
		fdb_probe_actor_enter("getProcessClassActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getProcessClassActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getProcessClassActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetProcessClassActorActor, 0, std::vector<ProcessData> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> getProcessClassActor( ReadYourWritesTransaction* const& ryw, KeyRef const& prefix, KeyRangeRef const& kr ) {
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new GetProcessClassActorActor(ryw, prefix, kr));
															#line 6108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

															#line 6113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via processClassCommitActor()
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class ProcessClassCommitActorActor>
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class ProcessClassCommitActorActorState {
															#line 6120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ProcessClassCommitActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& range) 
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   range(range)
															#line 6129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("processClassCommitActor", reinterpret_cast<unsigned long>(this));

	}
	~ProcessClassCommitActorActorState() 
	{
		fdb_probe_actor_destroy("processClassCommitActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 1302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<std::vector<ProcessData>> __when_expr_0 = getWorkers(&ryw->getTransaction());
															#line 1303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<ProcessClassCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ProcessClassCommitActorActor*>(this)->actor_wait_state = 1;
															#line 1303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ProcessClassCommitActorActor, 0, std::vector<ProcessData> >*>(static_cast<ProcessClassCommitActorActor*>(this)));
															#line 6159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~ProcessClassCommitActorActorState();
		static_cast<ProcessClassCommitActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<ProcessData> const& workers,int loopDepth) 
	{
															#line 1306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(range);
															#line 1307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto iter = ranges.begin();
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for(;iter != ranges.end();) {
															#line 1309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			auto entry = iter->value();
															#line 1311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (entry.first && entry.second.present())
															#line 6190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				Key address = iter->begin().removePrefix(range.begin);
															#line 1314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				AddressExclusion addr = AddressExclusion::parse(address);
															#line 1316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ValueRef processClassType = entry.second.get();
															#line 1317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ProcessClass processClass(processClassType.toString(), ProcessClass::DBSource);
															#line 1319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				bool foundChange = false;
															#line 1320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				for(int i = 0;i < workers.size();i++) {
															#line 1321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (addr.excludes(workers[i].address))
															#line 6206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						if (processClass.classType() != ProcessClass::InvalidClass)
															#line 6210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							ryw->getTransaction().set(processClassKeyFor(workers[i].locality.processId().get()), processClassValue(processClass));
															#line 6214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
						else
						{
															#line 1326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							ryw->getTransaction().clear(processClassKeyFor(workers[i].locality.processId().get()));
															#line 6220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
															#line 1327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						foundChange = true;
															#line 6224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
															#line 1330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (foundChange)
															#line 6229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					ryw->getTransaction().set(processClassChangeKey, deterministicRandom()->randomUniqueID().toString());
															#line 6233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
															#line 1333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			++iter;
															#line 6238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ProcessClassCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~ProcessClassCommitActorActorState(); static_cast<ProcessClassCommitActorActor*>(this)->destroy(); return 0; }
															#line 6242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ProcessClassCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~ProcessClassCommitActorActorState();
		static_cast<ProcessClassCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<ProcessData> && workers,int loopDepth) 
	{
															#line 1306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(range);
															#line 1307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto iter = ranges.begin();
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for(;iter != ranges.end();) {
															#line 1309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			auto entry = iter->value();
															#line 1311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (entry.first && entry.second.present())
															#line 6262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				Key address = iter->begin().removePrefix(range.begin);
															#line 1314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				AddressExclusion addr = AddressExclusion::parse(address);
															#line 1316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ValueRef processClassType = entry.second.get();
															#line 1317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ProcessClass processClass(processClassType.toString(), ProcessClass::DBSource);
															#line 1319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				bool foundChange = false;
															#line 1320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				for(int i = 0;i < workers.size();i++) {
															#line 1321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (addr.excludes(workers[i].address))
															#line 6278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 1322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						if (processClass.classType() != ProcessClass::InvalidClass)
															#line 6282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							ryw->getTransaction().set(processClassKeyFor(workers[i].locality.processId().get()), processClassValue(processClass));
															#line 6286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
						else
						{
															#line 1326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							ryw->getTransaction().clear(processClassKeyFor(workers[i].locality.processId().get()));
															#line 6292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
															#line 1327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						foundChange = true;
															#line 6296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
															#line 1330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (foundChange)
															#line 6301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					ryw->getTransaction().set(processClassChangeKey, deterministicRandom()->randomUniqueID().toString());
															#line 6305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
															#line 1333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			++iter;
															#line 6310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ProcessClassCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~ProcessClassCommitActorActorState(); static_cast<ProcessClassCommitActorActor*>(this)->destroy(); return 0; }
															#line 6314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ProcessClassCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~ProcessClassCommitActorActorState();
		static_cast<ProcessClassCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<ProcessData> const& workers,int loopDepth) 
	{
		loopDepth = a_body1cont1(workers, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<ProcessData> && workers,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(workers), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ProcessClassCommitActorActor*>(this)->actor_wait_state > 0) static_cast<ProcessClassCommitActorActor*>(this)->actor_wait_state = 0;
		static_cast<ProcessClassCommitActorActor*>(this)->ActorCallback< ProcessClassCommitActorActor, 0, std::vector<ProcessData> >::remove();

	}
	void a_callback_fire(ActorCallback< ProcessClassCommitActorActor, 0, std::vector<ProcessData> >*,std::vector<ProcessData> const& value) 
	{
		fdb_probe_actor_enter("processClassCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("processClassCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ProcessClassCommitActorActor, 0, std::vector<ProcessData> >*,std::vector<ProcessData> && value) 
	{
		fdb_probe_actor_enter("processClassCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("processClassCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ProcessClassCommitActorActor, 0, std::vector<ProcessData> >*,Error err) 
	{
		fdb_probe_actor_enter("processClassCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("processClassCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef range;
															#line 6389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via processClassCommitActor()
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class ProcessClassCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< ProcessClassCommitActorActor, 0, std::vector<ProcessData> >, public FastAllocated<ProcessClassCommitActorActor>, public ProcessClassCommitActorActorState<ProcessClassCommitActorActor> {
															#line 6394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<ProcessClassCommitActorActor>::operator new;
	using FastAllocated<ProcessClassCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ProcessClassCommitActorActor, 0, std::vector<ProcessData> >;
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ProcessClassCommitActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& range) 
															#line 6405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<Optional<std::string>>(),
		   ProcessClassCommitActorActorState<ProcessClassCommitActorActor>(ryw, range)
	{
		fdb_probe_actor_enter("processClassCommitActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("processClassCommitActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("processClassCommitActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ProcessClassCommitActorActor, 0, std::vector<ProcessData> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Optional<std::string>> processClassCommitActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& range ) {
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new ProcessClassCommitActorActor(ryw, range));
															#line 6433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

ProcessClassRangeImpl::ProcessClassRangeImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

Future<RangeResult> ProcessClassRangeImpl::getRange(ReadYourWritesTransaction* ryw,
                                                    KeyRangeRef kr,
                                                    GetRangeLimits limitsHint) const {
	return getProcessClassActor(ryw, getKeyRange().begin, kr);
}

Future<Optional<std::string>> ProcessClassRangeImpl::commit(ReadYourWritesTransaction* ryw) {
	// Validate network address and process class type
	Optional<std::string> errorMsg;
	auto ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(getKeyRange());
	auto iter = ranges.begin();
	while (iter != ranges.end()) {
		auto entry = iter->value();
		// only check for setclass(set) operation, (clear) are forbidden thus not exist
		if (entry.first && entry.second.present()) {
			// validate network address
			Key address = iter->begin().removePrefix(range.begin);
			AddressExclusion addr = AddressExclusion::parse(address);
			if (!addr.isValid()) {
				std::string error = "ERROR: \'" + address.toString() + "\' is not a valid network endpoint address\n";
				if (address.toString().find(":tls") != std::string::npos)
					error += "        Do not include the `:tls' suffix when naming a process\n";
				errorMsg = ManagementAPIError::toJsonString(false, "setclass", error);
				return errorMsg;
			}
			// validate class type
			ValueRef processClassType = entry.second.get();
			ProcessClass processClass(processClassType.toString(), ProcessClass::DBSource);
			if (processClass.classType() == ProcessClass::InvalidClass && processClassType != "default"_sr) {
				std::string error = "ERROR: \'" + processClassType.toString() + "\' is not a valid process class\n";
				errorMsg = ManagementAPIError::toJsonString(false, "setclass", error);
				return errorMsg;
			}
		}
		++iter;
	}
	return processClassCommitActor(ryw, getKeyRange());
}

void throwSpecialKeyApiFailure(ReadYourWritesTransaction* ryw, std::string command, std::string message) {
	auto msg = ManagementAPIError::toJsonString(false, command, message);
	ryw->setSpecialKeySpaceErrorMsg(msg);
	throw special_keys_api_failure();
}

void ProcessClassRangeImpl::clear(ReadYourWritesTransaction* ryw, const KeyRangeRef& range) {
	return throwSpecialKeyApiFailure(ryw, "setclass", "Clear operation is meaningless thus forbidden for setclass");
}

void ProcessClassRangeImpl::clear(ReadYourWritesTransaction* ryw, const KeyRef& key) {
	return throwSpecialKeyApiFailure(
	    ryw, "setclass", "Clear range operation is meaningless thus forbidden for setclass");
}

															#line 6494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via getProcessClassSourceActor()
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class GetProcessClassSourceActorActor>
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GetProcessClassSourceActorActorState {
															#line 6501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetProcessClassSourceActorActorState(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   prefix(prefix),
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 6512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("getProcessClassSourceActor", reinterpret_cast<unsigned long>(this));

	}
	~GetProcessClassSourceActorActorState() 
	{
		fdb_probe_actor_destroy("getProcessClassSourceActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<std::vector<ProcessData>> __when_expr_0 = getWorkers(&ryw->getTransaction());
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GetProcessClassSourceActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetProcessClassSourceActorActor*>(this)->actor_wait_state = 1;
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetProcessClassSourceActorActor, 0, std::vector<ProcessData> >*>(static_cast<GetProcessClassSourceActorActor*>(this)));
															#line 6536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~GetProcessClassSourceActorActorState();
		static_cast<GetProcessClassSourceActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<ProcessData> const& _workers,int loopDepth) 
	{
															#line 1397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto workers = _workers;
															#line 1399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		std::sort(workers.begin(), workers.end(), [](const ProcessData& lhs, const ProcessData& rhs) { return formatIpPort(lhs.address.ip, lhs.address.port) < formatIpPort(rhs.address.ip, rhs.address.port); });
															#line 1403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto last = std::unique(workers.begin(), workers.end(), [](const ProcessData& lhs, const ProcessData& rhs) { return formatIpPort(lhs.address.ip, lhs.address.port) == formatIpPort(rhs.address.ip, rhs.address.port); });
															#line 1407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		workers.erase(last, workers.end());
															#line 1408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto& w : workers ) {
															#line 1411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			Key k(prefix.withSuffix(formatIpPort(w.address.ip, w.address.port)));
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.contains(k))
															#line 6573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				Value v(w.processClass.sourceString());
															#line 1414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back(result.arena(), KeyValueRef(k, v));
															#line 1415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.arena().dependsOn(k.arena());
															#line 1416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.arena().dependsOn(v.arena());
															#line 6583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetProcessClassSourceActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetProcessClassSourceActorActorState(); static_cast<GetProcessClassSourceActorActor*>(this)->destroy(); return 0; }
															#line 6588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetProcessClassSourceActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetProcessClassSourceActorActorState();
		static_cast<GetProcessClassSourceActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<ProcessData> && _workers,int loopDepth) 
	{
															#line 1397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto workers = _workers;
															#line 1399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		std::sort(workers.begin(), workers.end(), [](const ProcessData& lhs, const ProcessData& rhs) { return formatIpPort(lhs.address.ip, lhs.address.port) < formatIpPort(rhs.address.ip, rhs.address.port); });
															#line 1403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto last = std::unique(workers.begin(), workers.end(), [](const ProcessData& lhs, const ProcessData& rhs) { return formatIpPort(lhs.address.ip, lhs.address.port) == formatIpPort(rhs.address.ip, rhs.address.port); });
															#line 1407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		workers.erase(last, workers.end());
															#line 1408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto& w : workers ) {
															#line 1411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			Key k(prefix.withSuffix(formatIpPort(w.address.ip, w.address.port)));
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.contains(k))
															#line 6614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				Value v(w.processClass.sourceString());
															#line 1414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back(result.arena(), KeyValueRef(k, v));
															#line 1415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.arena().dependsOn(k.arena());
															#line 1416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.arena().dependsOn(v.arena());
															#line 6624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetProcessClassSourceActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetProcessClassSourceActorActorState(); static_cast<GetProcessClassSourceActorActor*>(this)->destroy(); return 0; }
															#line 6629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetProcessClassSourceActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetProcessClassSourceActorActorState();
		static_cast<GetProcessClassSourceActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<ProcessData> const& _workers,int loopDepth) 
	{
		loopDepth = a_body1cont1(_workers, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<ProcessData> && _workers,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_workers), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetProcessClassSourceActorActor*>(this)->actor_wait_state > 0) static_cast<GetProcessClassSourceActorActor*>(this)->actor_wait_state = 0;
		static_cast<GetProcessClassSourceActorActor*>(this)->ActorCallback< GetProcessClassSourceActorActor, 0, std::vector<ProcessData> >::remove();

	}
	void a_callback_fire(ActorCallback< GetProcessClassSourceActorActor, 0, std::vector<ProcessData> >*,std::vector<ProcessData> const& value) 
	{
		fdb_probe_actor_enter("getProcessClassSourceActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getProcessClassSourceActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetProcessClassSourceActorActor, 0, std::vector<ProcessData> >*,std::vector<ProcessData> && value) 
	{
		fdb_probe_actor_enter("getProcessClassSourceActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getProcessClassSourceActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetProcessClassSourceActorActor, 0, std::vector<ProcessData> >*,Error err) 
	{
		fdb_probe_actor_enter("getProcessClassSourceActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getProcessClassSourceActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef prefix;
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 6706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getProcessClassSourceActor()
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GetProcessClassSourceActorActor final : public Actor<RangeResult>, public ActorCallback< GetProcessClassSourceActorActor, 0, std::vector<ProcessData> >, public FastAllocated<GetProcessClassSourceActorActor>, public GetProcessClassSourceActorActorState<GetProcessClassSourceActorActor> {
															#line 6711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GetProcessClassSourceActorActor>::operator new;
	using FastAllocated<GetProcessClassSourceActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetProcessClassSourceActorActor, 0, std::vector<ProcessData> >;
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetProcessClassSourceActorActor(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 6722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   GetProcessClassSourceActorActorState<GetProcessClassSourceActorActor>(ryw, prefix, kr)
	{
		fdb_probe_actor_enter("getProcessClassSourceActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getProcessClassSourceActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getProcessClassSourceActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetProcessClassSourceActorActor, 0, std::vector<ProcessData> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> getProcessClassSourceActor( ReadYourWritesTransaction* const& ryw, KeyRef const& prefix, KeyRangeRef const& kr ) {
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new GetProcessClassSourceActorActor(ryw, prefix, kr));
															#line 6750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

ProcessClassSourceRangeImpl::ProcessClassSourceRangeImpl(KeyRangeRef kr) : SpecialKeyRangeReadImpl(kr) {}

Future<RangeResult> ProcessClassSourceRangeImpl::getRange(ReadYourWritesTransaction* ryw,
                                                          KeyRangeRef kr,
                                                          GetRangeLimits limitsHint) const {
	return getProcessClassSourceActor(ryw, getKeyRange().begin, kr);
}

															#line 6763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via getLockedKeyActor()
															#line 1430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class GetLockedKeyActorActor>
															#line 1430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GetLockedKeyActorActorState {
															#line 6770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetLockedKeyActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 1430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 6779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("getLockedKeyActor", reinterpret_cast<unsigned long>(this));

	}
	~GetLockedKeyActorActorState() 
	{
		fdb_probe_actor_destroy("getLockedKeyActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(databaseLockedKey);
															#line 1433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GetLockedKeyActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetLockedKeyActorActor*>(this)->actor_wait_state = 1;
															#line 1433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetLockedKeyActorActor, 0, Optional<Value> >*>(static_cast<GetLockedKeyActorActor*>(this)));
															#line 6805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~GetLockedKeyActorActorState();
		static_cast<GetLockedKeyActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 1434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 6830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			UID uid = UID::fromString(BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()).toString());
															#line 1437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, Value(uid.toString())));
															#line 6836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetLockedKeyActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetLockedKeyActorActorState(); static_cast<GetLockedKeyActorActor*>(this)->destroy(); return 0; }
															#line 6840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetLockedKeyActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetLockedKeyActorActorState();
		static_cast<GetLockedKeyActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 1434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 6854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			UID uid = UID::fromString(BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()).toString());
															#line 1437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, Value(uid.toString())));
															#line 6860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetLockedKeyActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetLockedKeyActorActorState(); static_cast<GetLockedKeyActorActor*>(this)->destroy(); return 0; }
															#line 6864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetLockedKeyActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetLockedKeyActorActorState();
		static_cast<GetLockedKeyActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<GetLockedKeyActorActor*>(this)->actor_wait_state > 0) static_cast<GetLockedKeyActorActor*>(this)->actor_wait_state = 0;
		static_cast<GetLockedKeyActorActor*>(this)->ActorCallback< GetLockedKeyActorActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< GetLockedKeyActorActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("getLockedKeyActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getLockedKeyActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetLockedKeyActorActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("getLockedKeyActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getLockedKeyActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetLockedKeyActorActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("getLockedKeyActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getLockedKeyActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 6939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getLockedKeyActor()
															#line 1430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GetLockedKeyActorActor final : public Actor<RangeResult>, public ActorCallback< GetLockedKeyActorActor, 0, Optional<Value> >, public FastAllocated<GetLockedKeyActorActor>, public GetLockedKeyActorActorState<GetLockedKeyActorActor> {
															#line 6944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GetLockedKeyActorActor>::operator new;
	using FastAllocated<GetLockedKeyActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetLockedKeyActorActor, 0, Optional<Value> >;
															#line 1430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetLockedKeyActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 6955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   GetLockedKeyActorActorState<GetLockedKeyActorActor>(ryw, kr)
	{
		fdb_probe_actor_enter("getLockedKeyActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getLockedKeyActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getLockedKeyActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetLockedKeyActorActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> getLockedKeyActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 1430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new GetLockedKeyActorActor(ryw, kr));
															#line 6983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

LockDatabaseImpl::LockDatabaseImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

Future<RangeResult> LockDatabaseImpl::getRange(ReadYourWritesTransaction* ryw,
                                               KeyRangeRef kr,
                                               GetRangeLimits limitsHint) const {
	// single key range, the queried range should always be the same as the underlying range
	ASSERT(kr == getKeyRange());
	auto lockEntry = ryw->getSpecialKeySpaceWriteMap()[SpecialKeySpace::getManagementApiCommandPrefix("lock")];
	if (!ryw->readYourWritesDisabled() && lockEntry.first) {
		// ryw enabled and we have written to the special key
		RangeResult result;
		if (lockEntry.second.present()) {
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, lockEntry.second.get()));
		}
		return result;
	} else {
		return getLockedKeyActor(ryw, kr);
	}
}

															#line 7008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via lockDatabaseCommitActor()
															#line 1462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class LockDatabaseCommitActorActor>
															#line 1462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class LockDatabaseCommitActorActorState {
															#line 7015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	LockDatabaseCommitActorActorState(ReadYourWritesTransaction* const& ryw,UID const& uid) 
															#line 1462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   uid(uid),
															#line 1463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   msg()
															#line 7026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("lockDatabaseCommitActor", reinterpret_cast<unsigned long>(this));

	}
	~LockDatabaseCommitActorActorState() 
	{
		fdb_probe_actor_destroy("lockDatabaseCommitActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(databaseLockedKey);
															#line 1466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<LockDatabaseCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7047 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<LockDatabaseCommitActorActor*>(this)->actor_wait_state = 1;
															#line 1466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< LockDatabaseCommitActorActor, 0, Optional<Value> >*>(static_cast<LockDatabaseCommitActorActor*>(this)));
															#line 7052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~LockDatabaseCommitActorActorState();
		static_cast<LockDatabaseCommitActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 1468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present() && BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) != uid)
															#line 7075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(database_locked(), loopDepth);
															#line 7079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
		else
		{
															#line 1472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!val.present())
															#line 7085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ryw->getTransaction().atomicOp( databaseLockedKey, BinaryWriter::toValue(uid, Unversioned()).withPrefix("0123456789"_sr).withSuffix("\x00\x00\x00\x00"_sr), MutationRef::SetVersionstampedValue);
															#line 1478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ryw->getTransaction().addWriteConflictRange(normalKeys);
															#line 7091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<LockDatabaseCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(msg); this->~LockDatabaseCommitActorActorState(); static_cast<LockDatabaseCommitActorActor*>(this)->destroy(); return 0; }
															#line 7096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<LockDatabaseCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(msg)); // state_var_RVO
		this->~LockDatabaseCommitActorActorState();
		static_cast<LockDatabaseCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 1468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present() && BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) != uid)
															#line 7108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(database_locked(), loopDepth);
															#line 7112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
		else
		{
															#line 1472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!val.present())
															#line 7118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ryw->getTransaction().atomicOp( databaseLockedKey, BinaryWriter::toValue(uid, Unversioned()).withPrefix("0123456789"_sr).withSuffix("\x00\x00\x00\x00"_sr), MutationRef::SetVersionstampedValue);
															#line 1478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ryw->getTransaction().addWriteConflictRange(normalKeys);
															#line 7124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<LockDatabaseCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(msg); this->~LockDatabaseCommitActorActorState(); static_cast<LockDatabaseCommitActorActor*>(this)->destroy(); return 0; }
															#line 7129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<LockDatabaseCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(msg)); // state_var_RVO
		this->~LockDatabaseCommitActorActorState();
		static_cast<LockDatabaseCommitActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<LockDatabaseCommitActorActor*>(this)->actor_wait_state > 0) static_cast<LockDatabaseCommitActorActor*>(this)->actor_wait_state = 0;
		static_cast<LockDatabaseCommitActorActor*>(this)->ActorCallback< LockDatabaseCommitActorActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< LockDatabaseCommitActorActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("lockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< LockDatabaseCommitActorActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("lockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< LockDatabaseCommitActorActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("lockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("lockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	UID uid;
															#line 1463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Optional<std::string> msg;
															#line 7206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via lockDatabaseCommitActor()
															#line 1462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class LockDatabaseCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< LockDatabaseCommitActorActor, 0, Optional<Value> >, public FastAllocated<LockDatabaseCommitActorActor>, public LockDatabaseCommitActorActorState<LockDatabaseCommitActorActor> {
															#line 7211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<LockDatabaseCommitActorActor>::operator new;
	using FastAllocated<LockDatabaseCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< LockDatabaseCommitActorActor, 0, Optional<Value> >;
															#line 1462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	LockDatabaseCommitActorActor(ReadYourWritesTransaction* const& ryw,UID const& uid) 
															#line 7222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<Optional<std::string>>(),
		   LockDatabaseCommitActorActorState<LockDatabaseCommitActorActor>(ryw, uid)
	{
		fdb_probe_actor_enter("lockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("lockDatabaseCommitActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("lockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< LockDatabaseCommitActorActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Optional<std::string>> lockDatabaseCommitActor( ReadYourWritesTransaction* const& ryw, UID const& uid ) {
															#line 1462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new LockDatabaseCommitActorActor(ryw, uid));
															#line 7250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

															#line 7255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via unlockDatabaseCommitActor()
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class UnlockDatabaseCommitActorActor>
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class UnlockDatabaseCommitActorActorState {
															#line 7262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	UnlockDatabaseCommitActorActorState(ReadYourWritesTransaction* const& ryw) 
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw)
															#line 7269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("unlockDatabaseCommitActor", reinterpret_cast<unsigned long>(this));

	}
	~UnlockDatabaseCommitActorActorState() 
	{
		fdb_probe_actor_destroy("unlockDatabaseCommitActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(databaseLockedKey);
															#line 1487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<UnlockDatabaseCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UnlockDatabaseCommitActorActor*>(this)->actor_wait_state = 1;
															#line 1487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UnlockDatabaseCommitActorActor, 0, Optional<Value> >*>(static_cast<UnlockDatabaseCommitActorActor*>(this)));
															#line 7295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~UnlockDatabaseCommitActorActorState();
		static_cast<UnlockDatabaseCommitActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 1488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 7318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().clear(singleKeyRange(databaseLockedKey));
															#line 7322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<UnlockDatabaseCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~UnlockDatabaseCommitActorActorState(); static_cast<UnlockDatabaseCommitActorActor*>(this)->destroy(); return 0; }
															#line 7326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<UnlockDatabaseCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~UnlockDatabaseCommitActorActorState();
		static_cast<UnlockDatabaseCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 1488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 7338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().clear(singleKeyRange(databaseLockedKey));
															#line 7342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<UnlockDatabaseCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~UnlockDatabaseCommitActorActorState(); static_cast<UnlockDatabaseCommitActorActor*>(this)->destroy(); return 0; }
															#line 7346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<UnlockDatabaseCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~UnlockDatabaseCommitActorActorState();
		static_cast<UnlockDatabaseCommitActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<UnlockDatabaseCommitActorActor*>(this)->actor_wait_state > 0) static_cast<UnlockDatabaseCommitActorActor*>(this)->actor_wait_state = 0;
		static_cast<UnlockDatabaseCommitActorActor*>(this)->ActorCallback< UnlockDatabaseCommitActorActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< UnlockDatabaseCommitActorActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("unlockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< UnlockDatabaseCommitActorActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("unlockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< UnlockDatabaseCommitActorActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("unlockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("unlockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 7419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via unlockDatabaseCommitActor()
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class UnlockDatabaseCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< UnlockDatabaseCommitActorActor, 0, Optional<Value> >, public FastAllocated<UnlockDatabaseCommitActorActor>, public UnlockDatabaseCommitActorActorState<UnlockDatabaseCommitActorActor> {
															#line 7424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<UnlockDatabaseCommitActorActor>::operator new;
	using FastAllocated<UnlockDatabaseCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UnlockDatabaseCommitActorActor, 0, Optional<Value> >;
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	UnlockDatabaseCommitActorActor(ReadYourWritesTransaction* const& ryw) 
															#line 7435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<Optional<std::string>>(),
		   UnlockDatabaseCommitActorActorState<UnlockDatabaseCommitActorActor>(ryw)
	{
		fdb_probe_actor_enter("unlockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("unlockDatabaseCommitActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("unlockDatabaseCommitActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< UnlockDatabaseCommitActorActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Optional<std::string>> unlockDatabaseCommitActor( ReadYourWritesTransaction* const& ryw ) {
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new UnlockDatabaseCommitActorActor(ryw));
															#line 7463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

Future<Optional<std::string>> LockDatabaseImpl::commit(ReadYourWritesTransaction* ryw) {
	auto lockId = ryw->getSpecialKeySpaceWriteMap()[SpecialKeySpace::getManagementApiCommandPrefix("lock")].second;
	if (lockId.present()) {
		std::string uidStr = lockId.get().toString();
		UID uid;
		try {
			uid = UID::fromString(uidStr);
		} catch (Error& e) {
			return Optional<std::string>(
			    ManagementAPIError::toJsonString(false, "lock", "Invalid UID hex string: " + uidStr));
		}
		return lockDatabaseCommitActor(ryw, uid);
	} else {
		return unlockDatabaseCommitActor(ryw);
	}
}

															#line 7485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via getConsistencyCheckKeyActor()
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class GetConsistencyCheckKeyActorActor>
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GetConsistencyCheckKeyActorActorState {
															#line 7492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetConsistencyCheckKeyActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 7501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("getConsistencyCheckKeyActor", reinterpret_cast<unsigned long>(this));

	}
	~GetConsistencyCheckKeyActorActorState() 
	{
		fdb_probe_actor_destroy("getConsistencyCheckKeyActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(fdbShouldConsistencyCheckBeSuspended);
															#line 1515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GetConsistencyCheckKeyActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetConsistencyCheckKeyActorActor*>(this)->actor_wait_state = 1;
															#line 1515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetConsistencyCheckKeyActorActor, 0, Optional<Value> >*>(static_cast<GetConsistencyCheckKeyActorActor*>(this)));
															#line 7529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~GetConsistencyCheckKeyActorActorState();
		static_cast<GetConsistencyCheckKeyActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 1516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		bool ccSuspendSetting = val.present() ? BinaryReader::fromStringRef<bool>(val.get(), Unversioned()) : false;
															#line 1517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (ccSuspendSetting)
															#line 7556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, ValueRef()));
															#line 7560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetConsistencyCheckKeyActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetConsistencyCheckKeyActorActorState(); static_cast<GetConsistencyCheckKeyActorActor*>(this)->destroy(); return 0; }
															#line 7564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetConsistencyCheckKeyActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetConsistencyCheckKeyActorActorState();
		static_cast<GetConsistencyCheckKeyActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 1516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		bool ccSuspendSetting = val.present() ? BinaryReader::fromStringRef<bool>(val.get(), Unversioned()) : false;
															#line 1517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (ccSuspendSetting)
															#line 7580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, ValueRef()));
															#line 7584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetConsistencyCheckKeyActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetConsistencyCheckKeyActorActorState(); static_cast<GetConsistencyCheckKeyActorActor*>(this)->destroy(); return 0; }
															#line 7588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetConsistencyCheckKeyActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetConsistencyCheckKeyActorActorState();
		static_cast<GetConsistencyCheckKeyActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<GetConsistencyCheckKeyActorActor*>(this)->actor_wait_state > 0) static_cast<GetConsistencyCheckKeyActorActor*>(this)->actor_wait_state = 0;
		static_cast<GetConsistencyCheckKeyActorActor*>(this)->ActorCallback< GetConsistencyCheckKeyActorActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< GetConsistencyCheckKeyActorActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("getConsistencyCheckKeyActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConsistencyCheckKeyActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetConsistencyCheckKeyActorActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("getConsistencyCheckKeyActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConsistencyCheckKeyActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetConsistencyCheckKeyActorActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("getConsistencyCheckKeyActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConsistencyCheckKeyActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 7663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getConsistencyCheckKeyActor()
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GetConsistencyCheckKeyActorActor final : public Actor<RangeResult>, public ActorCallback< GetConsistencyCheckKeyActorActor, 0, Optional<Value> >, public FastAllocated<GetConsistencyCheckKeyActorActor>, public GetConsistencyCheckKeyActorActorState<GetConsistencyCheckKeyActorActor> {
															#line 7668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GetConsistencyCheckKeyActorActor>::operator new;
	using FastAllocated<GetConsistencyCheckKeyActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetConsistencyCheckKeyActorActor, 0, Optional<Value> >;
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetConsistencyCheckKeyActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 7679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   GetConsistencyCheckKeyActorActorState<GetConsistencyCheckKeyActorActor>(ryw, kr)
	{
		fdb_probe_actor_enter("getConsistencyCheckKeyActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getConsistencyCheckKeyActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getConsistencyCheckKeyActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetConsistencyCheckKeyActorActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> getConsistencyCheckKeyActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new GetConsistencyCheckKeyActorActor(ryw, kr));
															#line 7707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

ConsistencyCheckImpl::ConsistencyCheckImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

Future<RangeResult> ConsistencyCheckImpl::getRange(ReadYourWritesTransaction* ryw,
                                                   KeyRangeRef kr,
                                                   GetRangeLimits limitsHint) const {
	// single key range, the queried range should always be the same as the underlying range
	ASSERT(kr == getKeyRange());
	auto entry = ryw->getSpecialKeySpaceWriteMap()[SpecialKeySpace::getManagementApiCommandPrefix("consistencycheck")];
	if (!ryw->readYourWritesDisabled() && entry.first) {
		// ryw enabled and we have written to the special key
		RangeResult result;
		if (entry.second.present()) {
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, entry.second.get()));
		}
		return result;
	} else {
		return getConsistencyCheckKeyActor(ryw, kr);
	}
}

Future<Optional<std::string>> ConsistencyCheckImpl::commit(ReadYourWritesTransaction* ryw) {
	auto entry =
	    ryw->getSpecialKeySpaceWriteMap()[SpecialKeySpace::getManagementApiCommandPrefix("consistencycheck")].second;
	ryw->getTransaction().setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
	ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
	ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
	ryw->getTransaction().set(fdbShouldConsistencyCheckBeSuspended,
	                          BinaryWriter::toValue(entry.present(), Unversioned()));
	return Optional<std::string>();
}

GlobalConfigImpl::GlobalConfigImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

// Returns key-value pairs for each value stored in the global configuration
// framework within the range specified. The special-key-space getrange
// function should only be used for informational purposes. All values are
// returned as strings regardless of their true type.
Future<RangeResult> GlobalConfigImpl::getRange(ReadYourWritesTransaction* ryw,
                                               KeyRangeRef kr,
                                               GetRangeLimits limitsHint) const {
	RangeResult result;
	KeyRangeRef modified =
	    KeyRangeRef(kr.begin.removePrefix(getKeyRange().begin), kr.end.removePrefix(getKeyRange().begin));
	std::map<KeyRef, Reference<ConfigValue>> values = ryw->getDatabase()->globalConfig->get(modified);
	for (const auto& [key, config] : values) {
		Key prefixedKey = key.withPrefix(getKeyRange().begin);
		if (config.isValid() && config->value.has_value()) {
			if (config->value.type() == typeid(StringRef)) {
				result.push_back_deep(result.arena(),
				                      KeyValueRef(prefixedKey, std::any_cast<StringRef>(config->value).toString()));
			} else if (config->value.type() == typeid(int64_t)) {
				result.push_back_deep(result.arena(),
				                      KeyValueRef(prefixedKey, std::to_string(std::any_cast<int64_t>(config->value))));
			} else if (config->value.type() == typeid(bool)) {
				result.push_back_deep(result.arena(),
				                      KeyValueRef(prefixedKey, std::to_string(std::any_cast<bool>(config->value))));
			} else if (config->value.type() == typeid(float)) {
				result.push_back_deep(result.arena(),
				                      KeyValueRef(prefixedKey, std::to_string(std::any_cast<float>(config->value))));
			} else if (config->value.type() == typeid(double)) {
				result.push_back_deep(result.arena(),
				                      KeyValueRef(prefixedKey, std::to_string(std::any_cast<double>(config->value))));
			} else {
				ASSERT(false);
			}
		}
	}

	return result;
}

// Marks the key for insertion into global configuration.
void GlobalConfigImpl::set(ReadYourWritesTransaction* ryw, const KeyRef& key, const ValueRef& value) {
	ryw->getSpecialKeySpaceWriteMap().insert(key, std::make_pair(true, Optional<Value>(value)));
}

// Writes global configuration changes to durable memory. Also writes the
// changes made in the transaction to a recent history set, and updates the
// latest version which the global configuration was updated at.
															#line 7791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via globalConfigCommitActor()
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class GlobalConfigCommitActorActor>
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GlobalConfigCommitActorActorState {
															#line 7798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GlobalConfigCommitActorActorState(GlobalConfigImpl* const& globalConfig,ReadYourWritesTransaction* const& ryw) 
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : globalConfig(globalConfig),
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   ryw(ryw),
															#line 1605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   tr(ryw->getTransaction())
															#line 7809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("globalConfigCommitActor", reinterpret_cast<unsigned long>(this));

	}
	~GlobalConfigCommitActorActorState() 
	{
		fdb_probe_actor_destroy("globalConfigCommitActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr.getRange(globalConfigHistoryKeys, CLIENT_KNOBS->TOO_MANY);
															#line 1610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GlobalConfigCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GlobalConfigCommitActorActor*>(this)->actor_wait_state = 1;
															#line 1610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GlobalConfigCommitActorActor, 0, RangeResult >*>(static_cast<GlobalConfigCommitActorActor*>(this)));
															#line 7833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~GlobalConfigCommitActorActorState();
		static_cast<GlobalConfigCommitActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& history,int loopDepth) 
	{
															#line 1611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		constexpr int kGlobalConfigMaxHistorySize = 3;
															#line 1612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (history.size() > kGlobalConfigMaxHistorySize - 1)
															#line 7858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for(int i = 0;i < history.size() - (kGlobalConfigMaxHistorySize - 1);++i) {
															#line 1614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				tr.clear(history[i].key);
															#line 7864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		Standalone<VectorRef<KeyValueRef>> insertions;
															#line 1619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		Standalone<VectorRef<KeyRangeRef>> clears;
															#line 1624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(specialKeys);
															#line 1626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		iter = ranges.begin();
															#line 1627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for(;iter != ranges.end();) {
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			std::pair<bool, Optional<Value>> entry = iter->value();
															#line 1629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (entry.first)
															#line 7881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (entry.second.present() && iter->begin().startsWith(globalConfig->getKeyRange().begin))
															#line 7885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					Key bareKey = iter->begin().removePrefix(globalConfig->getKeyRange().begin);
															#line 1632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					insertions.push_back_deep(insertions.arena(), KeyValueRef(bareKey, entry.second.get()));
															#line 7891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
				else
				{
															#line 1633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (!entry.second.present() && iter->range().begin.startsWith(globalConfig->getKeyRange().begin) && iter->range().end.startsWith(globalConfig->getKeyRange().begin))
															#line 7897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 1635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						KeyRef bareRangeBegin = iter->range().begin.removePrefix(globalConfig->getKeyRange().begin);
															#line 1636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						KeyRef bareRangeEnd = iter->range().end.removePrefix(globalConfig->getKeyRange().begin);
															#line 1637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						clears.push_back_deep(clears.arena(), KeyRangeRef(bareRangeBegin, bareRangeEnd));
															#line 7905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
			}
															#line 1640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			++iter;
															#line 7911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		GlobalConfig::applyChanges(tr, insertions, clears);
															#line 1644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GlobalConfigCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~GlobalConfigCommitActorActorState(); static_cast<GlobalConfigCommitActorActor*>(this)->destroy(); return 0; }
															#line 7917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GlobalConfigCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~GlobalConfigCommitActorActorState();
		static_cast<GlobalConfigCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && history,int loopDepth) 
	{
															#line 1611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		constexpr int kGlobalConfigMaxHistorySize = 3;
															#line 1612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (history.size() > kGlobalConfigMaxHistorySize - 1)
															#line 7931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for(int i = 0;i < history.size() - (kGlobalConfigMaxHistorySize - 1);++i) {
															#line 1614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				tr.clear(history[i].key);
															#line 7937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		Standalone<VectorRef<KeyValueRef>> insertions;
															#line 1619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		Standalone<VectorRef<KeyRangeRef>> clears;
															#line 1624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(specialKeys);
															#line 1626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		iter = ranges.begin();
															#line 1627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for(;iter != ranges.end();) {
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			std::pair<bool, Optional<Value>> entry = iter->value();
															#line 1629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (entry.first)
															#line 7954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (entry.second.present() && iter->begin().startsWith(globalConfig->getKeyRange().begin))
															#line 7958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					Key bareKey = iter->begin().removePrefix(globalConfig->getKeyRange().begin);
															#line 1632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					insertions.push_back_deep(insertions.arena(), KeyValueRef(bareKey, entry.second.get()));
															#line 7964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
				else
				{
															#line 1633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (!entry.second.present() && iter->range().begin.startsWith(globalConfig->getKeyRange().begin) && iter->range().end.startsWith(globalConfig->getKeyRange().begin))
															#line 7970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 1635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						KeyRef bareRangeBegin = iter->range().begin.removePrefix(globalConfig->getKeyRange().begin);
															#line 1636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						KeyRef bareRangeEnd = iter->range().end.removePrefix(globalConfig->getKeyRange().begin);
															#line 1637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						clears.push_back_deep(clears.arena(), KeyRangeRef(bareRangeBegin, bareRangeEnd));
															#line 7978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
			}
															#line 1640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			++iter;
															#line 7984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		GlobalConfig::applyChanges(tr, insertions, clears);
															#line 1644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GlobalConfigCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~GlobalConfigCommitActorActorState(); static_cast<GlobalConfigCommitActorActor*>(this)->destroy(); return 0; }
															#line 7990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GlobalConfigCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~GlobalConfigCommitActorActorState();
		static_cast<GlobalConfigCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& history,int loopDepth) 
	{
		loopDepth = a_body1cont1(history, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && history,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(history), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GlobalConfigCommitActorActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfigCommitActorActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfigCommitActorActor*>(this)->ActorCallback< GlobalConfigCommitActorActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfigCommitActorActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("globalConfigCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("globalConfigCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GlobalConfigCommitActorActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("globalConfigCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("globalConfigCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GlobalConfigCommitActorActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("globalConfigCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("globalConfigCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GlobalConfigImpl* globalConfig;
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Transaction& tr;
															#line 1624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, std::pair<bool, Optional<Value>>, KeyRangeRef>::Ranges ranges;
															#line 1626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, std::pair<bool, Optional<Value>>, KeyRangeRef>::iterator iter;
															#line 8071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via globalConfigCommitActor()
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GlobalConfigCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< GlobalConfigCommitActorActor, 0, RangeResult >, public FastAllocated<GlobalConfigCommitActorActor>, public GlobalConfigCommitActorActorState<GlobalConfigCommitActorActor> {
															#line 8076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GlobalConfigCommitActorActor>::operator new;
	using FastAllocated<GlobalConfigCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GlobalConfigCommitActorActor, 0, RangeResult >;
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GlobalConfigCommitActorActor(GlobalConfigImpl* const& globalConfig,ReadYourWritesTransaction* const& ryw) 
															#line 8087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<Optional<std::string>>(),
		   GlobalConfigCommitActorActorState<GlobalConfigCommitActorActor>(globalConfig, ryw)
	{
		fdb_probe_actor_enter("globalConfigCommitActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("globalConfigCommitActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("globalConfigCommitActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GlobalConfigCommitActorActor, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Optional<std::string>> globalConfigCommitActor( GlobalConfigImpl* const& globalConfig, ReadYourWritesTransaction* const& ryw ) {
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new GlobalConfigCommitActorActor(globalConfig, ryw));
															#line 8115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

// Called when a transaction includes keys in the global configuration special-key-space range.
Future<Optional<std::string>> GlobalConfigImpl::commit(ReadYourWritesTransaction* ryw) {
	return globalConfigCommitActor(this, ryw);
}

// Marks the range for deletion from global configuration.
void GlobalConfigImpl::clear(ReadYourWritesTransaction* ryw, const KeyRangeRef& range) {
	ryw->getSpecialKeySpaceWriteMap().insert(range, std::make_pair(true, Optional<Value>()));
}

// Marks the key for deletion from global configuration.
void GlobalConfigImpl::clear(ReadYourWritesTransaction* ryw, const KeyRef& key) {
	ryw->getSpecialKeySpaceWriteMap().insert(key, std::make_pair(true, Optional<Value>()));
}

TracingOptionsImpl::TracingOptionsImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

Future<RangeResult> TracingOptionsImpl::getRange(ReadYourWritesTransaction* ryw,
                                                 KeyRangeRef kr,
                                                 GetRangeLimits limitsHint) const {
	RangeResult result;
	for (const auto& option : SpecialKeySpace::getTracingOptions()) {
		auto key = getKeyRange().begin.withSuffix(option);
		if (!kr.contains(key)) {
			continue;
		}

		if (key.endsWith(kTracingTransactionIdKey)) {
			result.push_back_deep(result.arena(),
			                      KeyValueRef(key, ryw->getTransactionState()->spanContext.traceID.toString()));
		} else if (key.endsWith(kTracingTokenKey)) {
			result.push_back_deep(result.arena(),
			                      KeyValueRef(key, std::to_string(ryw->getTransactionState()->spanContext.spanID)));
		}
	}
	return result;
}

void TracingOptionsImpl::set(ReadYourWritesTransaction* ryw, const KeyRef& key, const ValueRef& value) {
	if (ryw->getApproximateSize() > 0) {
		ryw->setSpecialKeySpaceErrorMsg(
		    ManagementAPIError::toJsonString(false, "configure trace", "tracing options must be set first"));
		ryw->getSpecialKeySpaceWriteMap().insert(key, std::make_pair(true, Optional<Value>()));
		return;
	}

	if (key.endsWith(kTracingTransactionIdKey)) {
		ryw->setTransactionID(UID::fromString(value.toString()));
	} else if (key.endsWith(kTracingTokenKey)) {
		if (value.toString() == "true") {
			ryw->setToken(deterministicRandom()->randomUInt64());
		} else if (value.toString() == "false") {
			ryw->setToken(0);
		} else {
			ryw->setSpecialKeySpaceErrorMsg(
			    ManagementAPIError::toJsonString(false, "configure trace token", "token must be set to true/false"));
			throw special_keys_api_failure();
		}
	}
}

Future<Optional<std::string>> TracingOptionsImpl::commit(ReadYourWritesTransaction* ryw) {
	if (ryw->getSpecialKeySpaceWriteMap().size() > 0) {
		throw special_keys_api_failure();
	}
	return Optional<std::string>();
}

void TracingOptionsImpl::clear(ReadYourWritesTransaction* ryw, const KeyRangeRef& range) {
	ryw->setSpecialKeySpaceErrorMsg(ManagementAPIError::toJsonString(false, "clear trace", "clear range disabled"));
	throw special_keys_api_failure();
}

void TracingOptionsImpl::clear(ReadYourWritesTransaction* ryw, const KeyRef& key) {
	ryw->setSpecialKeySpaceErrorMsg(ManagementAPIError::toJsonString(false, "clear trace", "clear disabled"));
	throw special_keys_api_failure();
}

CoordinatorsImpl::CoordinatorsImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

															#line 8200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via coordinatorsGetRangeActor()
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class CoordinatorsGetRangeActorActor>
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class CoordinatorsGetRangeActorActorState {
															#line 8207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	CoordinatorsGetRangeActorActorState(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   prefix(prefix),
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr),
															#line 1728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   cs(ryw->getDatabase()->getConnectionRecord()->getConnectionString())
															#line 8220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("coordinatorsGetRangeActor", reinterpret_cast<unsigned long>(this));

	}
	~CoordinatorsGetRangeActorActorState() 
	{
		fdb_probe_actor_destroy("coordinatorsGetRangeActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<std::vector<NetworkAddress>> __when_expr_0 = cs.tryResolveHostnames();
															#line 1729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<CoordinatorsGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CoordinatorsGetRangeActorActor*>(this)->actor_wait_state = 1;
															#line 1729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CoordinatorsGetRangeActorActor, 0, std::vector<NetworkAddress> >*>(static_cast<CoordinatorsGetRangeActorActor*>(this)));
															#line 8242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~CoordinatorsGetRangeActorActorState();
		static_cast<CoordinatorsGetRangeActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		Key cluster_decription_key = prefix.withSuffix("cluster_description"_sr);
															#line 1732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (kr.contains(cluster_decription_key))
															#line 8269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(cluster_decription_key, cs.clusterKeyName()));
															#line 8273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		std::sort(coordinator_processes.begin(), coordinator_processes.end(), [](const NetworkAddress& lhs, const NetworkAddress& rhs) { return lhs.toString() < rhs.toString(); });
															#line 1740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		std::string processes_str;
															#line 1741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( const auto& w : coordinator_processes ) {
															#line 1742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (processes_str.size())
															#line 8283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				processes_str += ",";
															#line 8287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 1744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			processes_str += w.toString();
															#line 8291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		Key processes_key = prefix.withSuffix("processes"_sr);
															#line 1747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (kr.contains(processes_key))
															#line 8297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(processes_key, Value(processes_str)));
															#line 8301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<CoordinatorsGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~CoordinatorsGetRangeActorActorState(); static_cast<CoordinatorsGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 8305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<CoordinatorsGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(rywGetRange(ryw, kr, result));
		this->~CoordinatorsGetRangeActorActorState();
		static_cast<CoordinatorsGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<NetworkAddress> const& __coordinator_processes,int loopDepth) 
	{
															#line 1729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		coordinator_processes = __coordinator_processes;
															#line 8317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<NetworkAddress> && __coordinator_processes,int loopDepth) 
	{
		coordinator_processes = std::move(__coordinator_processes);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CoordinatorsGetRangeActorActor*>(this)->actor_wait_state > 0) static_cast<CoordinatorsGetRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<CoordinatorsGetRangeActorActor*>(this)->ActorCallback< CoordinatorsGetRangeActorActor, 0, std::vector<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< CoordinatorsGetRangeActorActor, 0, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> const& value) 
	{
		fdb_probe_actor_enter("coordinatorsGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("coordinatorsGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CoordinatorsGetRangeActorActor, 0, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("coordinatorsGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("coordinatorsGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CoordinatorsGetRangeActorActor, 0, std::vector<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("coordinatorsGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("coordinatorsGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef prefix;
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 1728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ClusterConnectionString cs;
															#line 1729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<NetworkAddress> coordinator_processes;
															#line 8390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via coordinatorsGetRangeActor()
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class CoordinatorsGetRangeActorActor final : public Actor<RangeResult>, public ActorCallback< CoordinatorsGetRangeActorActor, 0, std::vector<NetworkAddress> >, public FastAllocated<CoordinatorsGetRangeActorActor>, public CoordinatorsGetRangeActorActorState<CoordinatorsGetRangeActorActor> {
															#line 8395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<CoordinatorsGetRangeActorActor>::operator new;
	using FastAllocated<CoordinatorsGetRangeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CoordinatorsGetRangeActorActor, 0, std::vector<NetworkAddress> >;
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	CoordinatorsGetRangeActorActor(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 8406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   CoordinatorsGetRangeActorActorState<CoordinatorsGetRangeActorActor>(ryw, prefix, kr)
	{
		fdb_probe_actor_enter("coordinatorsGetRangeActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("coordinatorsGetRangeActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("coordinatorsGetRangeActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CoordinatorsGetRangeActorActor, 0, std::vector<NetworkAddress> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> coordinatorsGetRangeActor( ReadYourWritesTransaction* const& ryw, KeyRef const& prefix, KeyRangeRef const& kr ) {
															#line 1727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new CoordinatorsGetRangeActorActor(ryw, prefix, kr));
															#line 8434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

Future<RangeResult> CoordinatorsImpl::getRange(ReadYourWritesTransaction* ryw,
                                               KeyRangeRef kr,
                                               GetRangeLimits limitsHint) const {
	KeyRef prefix(getKeyRange().begin);
	return coordinatorsGetRangeActor(ryw, prefix, kr);
}

															#line 8446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via coordinatorsCommitActor()
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class CoordinatorsCommitActorActor>
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class CoordinatorsCommitActorActorState {
															#line 8453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	CoordinatorsCommitActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr),
															#line 1761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   conn(),
															#line 1762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   process_address_or_hostname_strs(),
															#line 1763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   msg(),
															#line 1764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   index(),
															#line 1765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   parse_error(false)
															#line 8472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("coordinatorsCommitActor", reinterpret_cast<unsigned long>(this));

	}
	~CoordinatorsCommitActorActorState() 
	{
		fdb_probe_actor_destroy("coordinatorsCommitActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			Key processes_key = "processes"_sr.withPrefix(kr.begin);
															#line 1769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			auto processes_entry = ryw->getSpecialKeySpaceWriteMap()[processes_key];
															#line 1770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (processes_entry.first)
															#line 8491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ASSERT(processes_entry.second.present());
															#line 1772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				auto processesStr = processes_entry.second.get().toString();
															#line 1773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				boost::split(process_address_or_hostname_strs, processesStr, [](char c) { return c == ','; });
															#line 1774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (!process_address_or_hostname_strs.size())
															#line 8501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (!static_cast<CoordinatorsCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(ManagementAPIError::toJsonString( false, "coordinators", "New coordinators\' processes are empty, please specify new processes\' network addresses with format " "\"IP:PORT,IP:PORT,...,IP:PORT\" or \"HOSTNAME:PORT,HOSTNAME:PORT,...,HOSTNAME:PORT\"")); this->~CoordinatorsCommitActorActorState(); static_cast<CoordinatorsCommitActorActor*>(this)->destroy(); return 0; }
															#line 8505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					new (&static_cast<CoordinatorsCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(ManagementAPIError::toJsonString( false, "coordinators", "New coordinators\' processes are empty, please specify new processes\' network addresses with format " "\"IP:PORT,IP:PORT,...,IP:PORT\" or \"HOSTNAME:PORT,HOSTNAME:PORT,...,HOSTNAME:PORT\""));
					this->~CoordinatorsCommitActorActorState();
					static_cast<CoordinatorsCommitActorActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 1781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				index = 0;
															#line 8513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				loopDepth = a_body1loopHead1(loopDepth);
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
		this->~CoordinatorsCommitActorActorState();
		static_cast<CoordinatorsCommitActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		std::string newName;
															#line 1808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		Key cluster_decription_key = "cluster_description"_sr.withPrefix(kr.begin);
															#line 1809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto entry = ryw->getSpecialKeySpaceWriteMap()[cluster_decription_key];
															#line 1810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (entry.first)
															#line 8547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (entry.second.present() && isAlphaNumeric(entry.second.get().toString()))
															#line 8551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				newName = entry.second.get().toString();
															#line 8555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 1817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (!static_cast<CoordinatorsCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(ManagementAPIError::toJsonString( false, "coordinators", "Cluster description must match [A-Za-z0-9_]+")); this->~CoordinatorsCommitActorActorState(); static_cast<CoordinatorsCommitActorActor*>(this)->destroy(); return 0; }
															#line 8561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				new (&static_cast<CoordinatorsCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(ManagementAPIError::toJsonString( false, "coordinators", "Cluster description must match [A-Za-z0-9_]+"));
				this->~CoordinatorsCommitActorActorState();
				static_cast<CoordinatorsCommitActorActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 1822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto configDBEntry = ryw->getSpecialKeySpaceWriteMap()["config_db"_sr.withPrefix(kr.begin)];
															#line 1824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		TraceEvent(SevDebug, "SKSChangeCoordinatorsStart") .detail("NewConnectionString", conn.toString()) .detail("Description", entry.first ? entry.second.get().toString() : "") .detail("ConfigDBDisabled", configDBEntry.first);
															#line 1829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Optional<CoordinatorsResult>> __when_expr_0 = changeQuorumChecker(&ryw->getTransaction(), &conn, newName, configDBEntry.first);
															#line 1829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<CoordinatorsCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1cont1when1(__when_expr_0.get(), loopDepth); };
		static_cast<CoordinatorsCommitActorActor*>(this)->actor_wait_state = 1;
															#line 1829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CoordinatorsCommitActorActor, 0, Optional<CoordinatorsResult> >*>(static_cast<CoordinatorsCommitActorActor*>(this)));
															#line 8581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

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
															#line 1781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!(index < process_address_or_hostname_strs.size()))
															#line 8603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
		try {
															#line 1783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (Hostname::isHostname(process_address_or_hostname_strs[index]))
															#line 8610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				conn.hostnames.push_back(Hostname::parse(process_address_or_hostname_strs[index]));
															#line 8614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 1786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				NetworkAddress a = NetworkAddress::parse(process_address_or_hostname_strs[index]);
															#line 1787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (!a.isValid())
															#line 8622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					parse_error = true;
															#line 8626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
				else
				{
															#line 1790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					conn.coords.push_back(a);
															#line 8632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
			loopDepth = a_body1loopBody1cont8(loopDepth);
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
			return a_body1cont2(loopDepth);
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
															#line 1798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (parse_error)
															#line 8662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			std::string error = "ERROR: \'" + process_address_or_hostname_strs[index] + "\' is not a valid network endpoint address\n";
															#line 1801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CoordinatorsCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(ManagementAPIError::toJsonString(false, "coordinators", error)); this->~CoordinatorsCommitActorActorState(); static_cast<CoordinatorsCommitActorActor*>(this)->destroy(); return 0; }
															#line 8668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CoordinatorsCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(ManagementAPIError::toJsonString(false, "coordinators", error));
			this->~CoordinatorsCommitActorActorState();
			static_cast<CoordinatorsCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		index++;
															#line 8676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			TraceEvent(SevDebug, "SpecialKeysNetworkParseError").error(e);
															#line 1795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			parse_error = true;
															#line 8688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont8(int loopDepth) 
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
	int a_body1cont4(Optional<CoordinatorsResult> const& r,int loopDepth) 
	{
															#line 1832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		TraceEvent(SevDebug, "SKSChangeCoordinatorsFinish") .detail("Result", r.present() ? static_cast<int>(r.get()) : -1);
															#line 1834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (r.present())
															#line 8718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			auto res = r.get();
															#line 1836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			bool retriable = false;
															#line 1837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (res == CoordinatorsResult::COORDINATOR_UNREACHABLE)
															#line 8726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				retriable = true;
															#line 8730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 1839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (res == CoordinatorsResult::SUCCESS)
															#line 8736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					TraceEvent(SevError, "SpecialKeysForCoordinators").detail("UnexpectedSuccessfulResult", "");
															#line 1841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					ASSERT(false);
															#line 8742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
															#line 1843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			msg = ManagementAPIError::toJsonString(retriable, "coordinators", ManagementAPI::generateErrorMessage(res));
															#line 8747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<CoordinatorsCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(msg); this->~CoordinatorsCommitActorActorState(); static_cast<CoordinatorsCommitActorActor*>(this)->destroy(); return 0; }
															#line 8751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<CoordinatorsCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(msg)); // state_var_RVO
		this->~CoordinatorsCommitActorActorState();
		static_cast<CoordinatorsCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Optional<CoordinatorsResult> && r,int loopDepth) 
	{
															#line 1832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		TraceEvent(SevDebug, "SKSChangeCoordinatorsFinish") .detail("Result", r.present() ? static_cast<int>(r.get()) : -1);
															#line 1834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (r.present())
															#line 8765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			auto res = r.get();
															#line 1836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			bool retriable = false;
															#line 1837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (res == CoordinatorsResult::COORDINATOR_UNREACHABLE)
															#line 8773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				retriable = true;
															#line 8777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 1839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (res == CoordinatorsResult::SUCCESS)
															#line 8783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					TraceEvent(SevError, "SpecialKeysForCoordinators").detail("UnexpectedSuccessfulResult", "");
															#line 1841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					ASSERT(false);
															#line 8789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
															#line 1843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			msg = ManagementAPIError::toJsonString(retriable, "coordinators", ManagementAPI::generateErrorMessage(res));
															#line 8794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<CoordinatorsCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(msg); this->~CoordinatorsCommitActorActorState(); static_cast<CoordinatorsCommitActorActor*>(this)->destroy(); return 0; }
															#line 8798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<CoordinatorsCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(msg)); // state_var_RVO
		this->~CoordinatorsCommitActorActorState();
		static_cast<CoordinatorsCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Optional<CoordinatorsResult> const& r,int loopDepth) 
	{
		loopDepth = a_body1cont4(r, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Optional<CoordinatorsResult> && r,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(r), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CoordinatorsCommitActorActor*>(this)->actor_wait_state > 0) static_cast<CoordinatorsCommitActorActor*>(this)->actor_wait_state = 0;
		static_cast<CoordinatorsCommitActorActor*>(this)->ActorCallback< CoordinatorsCommitActorActor, 0, Optional<CoordinatorsResult> >::remove();

	}
	void a_callback_fire(ActorCallback< CoordinatorsCommitActorActor, 0, Optional<CoordinatorsResult> >*,Optional<CoordinatorsResult> const& value) 
	{
		fdb_probe_actor_enter("coordinatorsCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("coordinatorsCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CoordinatorsCommitActorActor, 0, Optional<CoordinatorsResult> >*,Optional<CoordinatorsResult> && value) 
	{
		fdb_probe_actor_enter("coordinatorsCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("coordinatorsCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CoordinatorsCommitActorActor, 0, Optional<CoordinatorsResult> >*,Error err) 
	{
		fdb_probe_actor_enter("coordinatorsCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("coordinatorsCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 1761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ClusterConnectionString conn;
															#line 1762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<std::string> process_address_or_hostname_strs;
															#line 1763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Optional<std::string> msg;
															#line 1764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	int index;
															#line 1765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	bool parse_error;
															#line 8883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via coordinatorsCommitActor()
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class CoordinatorsCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< CoordinatorsCommitActorActor, 0, Optional<CoordinatorsResult> >, public FastAllocated<CoordinatorsCommitActorActor>, public CoordinatorsCommitActorActorState<CoordinatorsCommitActorActor> {
															#line 8888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<CoordinatorsCommitActorActor>::operator new;
	using FastAllocated<CoordinatorsCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CoordinatorsCommitActorActor, 0, Optional<CoordinatorsResult> >;
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	CoordinatorsCommitActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 8899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<Optional<std::string>>(),
		   CoordinatorsCommitActorActorState<CoordinatorsCommitActorActor>(ryw, kr)
	{
		fdb_probe_actor_enter("coordinatorsCommitActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("coordinatorsCommitActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("coordinatorsCommitActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CoordinatorsCommitActorActor, 0, Optional<CoordinatorsResult> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<Optional<std::string>> coordinatorsCommitActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 1760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new CoordinatorsCommitActorActor(ryw, kr));
															#line 8927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

Future<Optional<std::string>> CoordinatorsImpl::commit(ReadYourWritesTransaction* ryw) {
	return coordinatorsCommitActor(ryw, getKeyRange());
}

void CoordinatorsImpl::clear(ReadYourWritesTransaction* ryw, const KeyRangeRef& range) {
	return throwSpecialKeyApiFailure(ryw, "coordinators", "Clear range is meaningless thus forbidden for coordinators");
}

void CoordinatorsImpl::clear(ReadYourWritesTransaction* ryw, const KeyRef& key) {
	return throwSpecialKeyApiFailure(
	    ryw, "coordinators", "Clear operation is meaningless thus forbidden for coordinators");
}

CoordinatorsAutoImpl::CoordinatorsAutoImpl(KeyRangeRef kr) : SpecialKeyRangeReadImpl(kr) {}

															#line 8947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via CoordinatorsAutoImplActor()
															#line 1863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class CoordinatorsAutoImplActorActor>
															#line 1863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class CoordinatorsAutoImplActorActorState {
															#line 8954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	CoordinatorsAutoImplActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 1863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr),
															#line 1864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   res(),
															#line 1865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   autoCoordinatorsKey(),
															#line 1866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   tr(ryw->getTransaction())
															#line 8969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this));

	}
	~CoordinatorsAutoImplActorActorState() 
	{
		fdb_probe_actor_destroy("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			tr.setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			tr.setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 1871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(coordinatorsKey);
															#line 1872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state = 1;
															#line 1872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CoordinatorsAutoImplActorActor, 0, Optional<Value> >*>(static_cast<CoordinatorsAutoImplActorActor*>(this)));
															#line 8999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~CoordinatorsAutoImplActorActorState();
		static_cast<CoordinatorsAutoImplActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& currentKey,int loopDepth) 
	{
															#line 1874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!currentKey.present())
															#line 9022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg( ManagementAPIError::toJsonString(false, "auto_coordinators", "The coordinator key does not exist"));
															#line 1877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 9028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		old = ClusterConnectionString(currentKey.get().toString());
															#line 1880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		result = CoordinatorsResult::SUCCESS;
															#line 1882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<std::vector<NetworkAddress>> __when_expr_1 = old.tryResolveHostnames();
															#line 1882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state = 2;
															#line 1882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CoordinatorsAutoImplActorActor, 1, std::vector<NetworkAddress> >*>(static_cast<CoordinatorsAutoImplActorActor*>(this)));
															#line 9043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && currentKey,int loopDepth) 
	{
															#line 1874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!currentKey.present())
															#line 9052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg( ManagementAPIError::toJsonString(false, "auto_coordinators", "The coordinator key does not exist"));
															#line 1877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 9058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		old = ClusterConnectionString(currentKey.get().toString());
															#line 1880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		result = CoordinatorsResult::SUCCESS;
															#line 1882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<std::vector<NetworkAddress>> __when_expr_1 = old.tryResolveHostnames();
															#line 1882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state = 2;
															#line 1882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CoordinatorsAutoImplActorActor, 1, std::vector<NetworkAddress> >*>(static_cast<CoordinatorsAutoImplActorActor*>(this)));
															#line 9073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		if (static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state > 0) static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state = 0;
		static_cast<CoordinatorsAutoImplActorActor*>(this)->ActorCallback< CoordinatorsAutoImplActorActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< CoordinatorsAutoImplActorActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CoordinatorsAutoImplActorActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CoordinatorsAutoImplActorActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(std::vector<NetworkAddress> const& oldCoordinators,int loopDepth) 
	{
															#line 1883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<std::vector<NetworkAddress>> __when_expr_2 = autoQuorumChange()->getDesiredCoordinators( &tr, oldCoordinators, Reference<ClusterConnectionMemoryRecord>(new ClusterConnectionMemoryRecord(old)), result);
															#line 1883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state = 3;
															#line 1883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CoordinatorsAutoImplActorActor, 2, std::vector<NetworkAddress> >*>(static_cast<CoordinatorsAutoImplActorActor*>(this)));
															#line 9152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(std::vector<NetworkAddress> && oldCoordinators,int loopDepth) 
	{
															#line 1883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<std::vector<NetworkAddress>> __when_expr_2 = autoQuorumChange()->getDesiredCoordinators( &tr, oldCoordinators, Reference<ClusterConnectionMemoryRecord>(new ClusterConnectionMemoryRecord(old)), result);
															#line 1883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state = 3;
															#line 1883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CoordinatorsAutoImplActorActor, 2, std::vector<NetworkAddress> >*>(static_cast<CoordinatorsAutoImplActorActor*>(this)));
															#line 9168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(std::vector<NetworkAddress> const& oldCoordinators,int loopDepth) 
	{
		loopDepth = a_body1cont2(oldCoordinators, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(std::vector<NetworkAddress> && oldCoordinators,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(oldCoordinators), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state > 0) static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state = 0;
		static_cast<CoordinatorsAutoImplActorActor*>(this)->ActorCallback< CoordinatorsAutoImplActorActor, 1, std::vector<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< CoordinatorsAutoImplActorActor, 1, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> const& value) 
	{
		fdb_probe_actor_enter("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CoordinatorsAutoImplActorActor, 1, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CoordinatorsAutoImplActorActor, 1, std::vector<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(std::vector<NetworkAddress> const& _desiredCoordinators,int loopDepth) 
	{
															#line 1889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (result == CoordinatorsResult::NOT_ENOUGH_MACHINES)
															#line 9240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg(ManagementAPIError::toJsonString( true, "auto_coordinators", "Too few fdbserver machines to provide coordination at the current redundancy level"));
															#line 1896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 9246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (result == CoordinatorsResult::SAME_NETWORK_ADDRESSES)
															#line 9250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& host : old.hostnames ) {
															#line 1901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += autoCoordinatorsKey.size() ? "," : "";
															#line 1902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += host.toString();
															#line 9258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 1904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& coord : old.coords ) {
															#line 1905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += autoCoordinatorsKey.size() ? "," : "";
															#line 1906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += coord.toString();
															#line 9266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
		else
		{
															#line 1909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& address : _desiredCoordinators ) {
															#line 1910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += autoCoordinatorsKey.size() ? "," : "";
															#line 1911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += address.toString();
															#line 9277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		res.push_back_deep(res.arena(), KeyValueRef(kr.begin, Value(autoCoordinatorsKey)));
															#line 1915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<CoordinatorsAutoImplActorActor*>(this)->SAV<RangeResult>::futures) { (void)(res); this->~CoordinatorsAutoImplActorActorState(); static_cast<CoordinatorsAutoImplActorActor*>(this)->destroy(); return 0; }
															#line 9284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<CoordinatorsAutoImplActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(res)); // state_var_RVO
		this->~CoordinatorsAutoImplActorActorState();
		static_cast<CoordinatorsAutoImplActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(std::vector<NetworkAddress> && _desiredCoordinators,int loopDepth) 
	{
															#line 1889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (result == CoordinatorsResult::NOT_ENOUGH_MACHINES)
															#line 9296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg(ManagementAPIError::toJsonString( true, "auto_coordinators", "Too few fdbserver machines to provide coordination at the current redundancy level"));
															#line 1896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 9302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (result == CoordinatorsResult::SAME_NETWORK_ADDRESSES)
															#line 9306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& host : old.hostnames ) {
															#line 1901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += autoCoordinatorsKey.size() ? "," : "";
															#line 1902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += host.toString();
															#line 9314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 1904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& coord : old.coords ) {
															#line 1905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += autoCoordinatorsKey.size() ? "," : "";
															#line 1906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += coord.toString();
															#line 9322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
		else
		{
															#line 1909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& address : _desiredCoordinators ) {
															#line 1910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += autoCoordinatorsKey.size() ? "," : "";
															#line 1911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += address.toString();
															#line 9333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		res.push_back_deep(res.arena(), KeyValueRef(kr.begin, Value(autoCoordinatorsKey)));
															#line 1915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<CoordinatorsAutoImplActorActor*>(this)->SAV<RangeResult>::futures) { (void)(res); this->~CoordinatorsAutoImplActorActorState(); static_cast<CoordinatorsAutoImplActorActor*>(this)->destroy(); return 0; }
															#line 9340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<CoordinatorsAutoImplActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(res)); // state_var_RVO
		this->~CoordinatorsAutoImplActorActorState();
		static_cast<CoordinatorsAutoImplActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(std::vector<NetworkAddress> const& _desiredCoordinators,int loopDepth) 
	{
		loopDepth = a_body1cont4(_desiredCoordinators, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(std::vector<NetworkAddress> && _desiredCoordinators,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_desiredCoordinators), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state > 0) static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state = 0;
		static_cast<CoordinatorsAutoImplActorActor*>(this)->ActorCallback< CoordinatorsAutoImplActorActor, 2, std::vector<NetworkAddress> >::remove();

	}
	void a_callback_fire(ActorCallback< CoordinatorsAutoImplActorActor, 2, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> const& value) 
	{
		fdb_probe_actor_enter("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CoordinatorsAutoImplActorActor, 2, std::vector<NetworkAddress> >*,std::vector<NetworkAddress> && value) 
	{
		fdb_probe_actor_enter("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CoordinatorsAutoImplActorActor, 2, std::vector<NetworkAddress> >*,Error err) 
	{
		fdb_probe_actor_enter("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 1863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 1864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult res;
															#line 1865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::string autoCoordinatorsKey;
															#line 1866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Transaction& tr;
															#line 1879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ClusterConnectionString old;
															#line 1880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	CoordinatorsResult result;
															#line 9425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via CoordinatorsAutoImplActor()
															#line 1863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class CoordinatorsAutoImplActorActor final : public Actor<RangeResult>, public ActorCallback< CoordinatorsAutoImplActorActor, 0, Optional<Value> >, public ActorCallback< CoordinatorsAutoImplActorActor, 1, std::vector<NetworkAddress> >, public ActorCallback< CoordinatorsAutoImplActorActor, 2, std::vector<NetworkAddress> >, public FastAllocated<CoordinatorsAutoImplActorActor>, public CoordinatorsAutoImplActorActorState<CoordinatorsAutoImplActorActor> {
															#line 9430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<CoordinatorsAutoImplActorActor>::operator new;
	using FastAllocated<CoordinatorsAutoImplActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CoordinatorsAutoImplActorActor, 0, Optional<Value> >;
friend struct ActorCallback< CoordinatorsAutoImplActorActor, 1, std::vector<NetworkAddress> >;
friend struct ActorCallback< CoordinatorsAutoImplActorActor, 2, std::vector<NetworkAddress> >;
															#line 1863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	CoordinatorsAutoImplActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 9443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   CoordinatorsAutoImplActorActorState<CoordinatorsAutoImplActorActor>(ryw, kr)
	{
		fdb_probe_actor_enter("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("CoordinatorsAutoImplActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("CoordinatorsAutoImplActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CoordinatorsAutoImplActorActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CoordinatorsAutoImplActorActor, 1, std::vector<NetworkAddress> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CoordinatorsAutoImplActorActor, 2, std::vector<NetworkAddress> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<RangeResult> CoordinatorsAutoImplActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 1863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new CoordinatorsAutoImplActorActor(ryw, kr));
															#line 9473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

Future<RangeResult> CoordinatorsAutoImpl::getRange(ReadYourWritesTransaction* ryw,
                                                   KeyRangeRef kr,
                                                   GetRangeLimits limitsHint) const {
	// single key range, the queried range should always be the same as the underlying range
	ASSERT(kr == getKeyRange());
	return CoordinatorsAutoImplActor(ryw, kr);
}

															#line 9486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via getMinCommitVersionActor()
															#line 1926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class GetMinCommitVersionActorActor>
															#line 1926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GetMinCommitVersionActorActorState {
															#line 9493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetMinCommitVersionActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 1926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 9502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("getMinCommitVersionActor", reinterpret_cast<unsigned long>(this));

	}
	~GetMinCommitVersionActorActorState() 
	{
		fdb_probe_actor_destroy("getMinCommitVersionActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(minRequiredCommitVersionKey);
															#line 1929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GetMinCommitVersionActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetMinCommitVersionActorActor*>(this)->actor_wait_state = 1;
															#line 1929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetMinCommitVersionActorActor, 0, Optional<Value> >*>(static_cast<GetMinCommitVersionActorActor*>(this)));
															#line 9528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~GetMinCommitVersionActorActorState();
		static_cast<GetMinCommitVersionActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 1930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 9553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			Version minRequiredCommitVersion = BinaryReader::fromStringRef<Version>(val.get(), Unversioned());
															#line 1933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ValueRef version(result.arena(), boost::lexical_cast<std::string>(minRequiredCommitVersion));
															#line 1934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, version));
															#line 9561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetMinCommitVersionActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetMinCommitVersionActorActorState(); static_cast<GetMinCommitVersionActorActor*>(this)->destroy(); return 0; }
															#line 9565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetMinCommitVersionActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetMinCommitVersionActorActorState();
		static_cast<GetMinCommitVersionActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 1930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 9579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			Version minRequiredCommitVersion = BinaryReader::fromStringRef<Version>(val.get(), Unversioned());
															#line 1933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ValueRef version(result.arena(), boost::lexical_cast<std::string>(minRequiredCommitVersion));
															#line 1934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, version));
															#line 9587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetMinCommitVersionActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetMinCommitVersionActorActorState(); static_cast<GetMinCommitVersionActorActor*>(this)->destroy(); return 0; }
															#line 9591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetMinCommitVersionActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetMinCommitVersionActorActorState();
		static_cast<GetMinCommitVersionActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<GetMinCommitVersionActorActor*>(this)->actor_wait_state > 0) static_cast<GetMinCommitVersionActorActor*>(this)->actor_wait_state = 0;
		static_cast<GetMinCommitVersionActorActor*>(this)->ActorCallback< GetMinCommitVersionActorActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< GetMinCommitVersionActorActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("getMinCommitVersionActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getMinCommitVersionActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetMinCommitVersionActorActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("getMinCommitVersionActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getMinCommitVersionActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetMinCommitVersionActorActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("getMinCommitVersionActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getMinCommitVersionActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 9666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getMinCommitVersionActor()
															#line 1926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GetMinCommitVersionActorActor final : public Actor<RangeResult>, public ActorCallback< GetMinCommitVersionActorActor, 0, Optional<Value> >, public FastAllocated<GetMinCommitVersionActorActor>, public GetMinCommitVersionActorActorState<GetMinCommitVersionActorActor> {
															#line 9671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GetMinCommitVersionActorActor>::operator new;
	using FastAllocated<GetMinCommitVersionActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetMinCommitVersionActorActor, 0, Optional<Value> >;
															#line 1926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetMinCommitVersionActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 9682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   GetMinCommitVersionActorActorState<GetMinCommitVersionActorActor>(ryw, kr)
	{
		fdb_probe_actor_enter("getMinCommitVersionActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getMinCommitVersionActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getMinCommitVersionActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetMinCommitVersionActorActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<RangeResult> getMinCommitVersionActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 1926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new GetMinCommitVersionActorActor(ryw, kr));
															#line 9710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

AdvanceVersionImpl::AdvanceVersionImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

Future<RangeResult> AdvanceVersionImpl::getRange(ReadYourWritesTransaction* ryw,
                                                 KeyRangeRef kr,
                                                 GetRangeLimits limitsHint) const {
	// single key range, the queried range should always be the same as the underlying range
	ASSERT(kr == getKeyRange());
	auto entry = ryw->getSpecialKeySpaceWriteMap()[SpecialKeySpace::getManagementApiCommandPrefix("advanceversion")];
	if (!ryw->readYourWritesDisabled() && entry.first) {
		// ryw enabled and we have written to the special key
		RangeResult result;
		if (entry.second.present()) {
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, entry.second.get()));
		}
		return result;
	} else {
		return getMinCommitVersionActor(ryw, kr);
	}
}

															#line 9735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via advanceVersionCommitActor()
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class AdvanceVersionCommitActorActor>
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class AdvanceVersionCommitActorActorState {
															#line 9742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	AdvanceVersionCommitActorActorState(ReadYourWritesTransaction* const& ryw,Version const& v) 
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   v(v)
															#line 9751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this));

	}
	~AdvanceVersionCommitActorActorState() 
	{
		fdb_probe_actor_destroy("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Standalone<StringRef>>> __when_expr_0 = ryw->getTransaction().get(versionEpochKey);
															#line 1960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<AdvanceVersionCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AdvanceVersionCommitActorActor*>(this)->actor_wait_state = 1;
															#line 1960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AdvanceVersionCommitActorActor, 0, Optional<Standalone<StringRef>> >*>(static_cast<AdvanceVersionCommitActorActor*>(this)));
															#line 9773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~AdvanceVersionCommitActorActorState();
		static_cast<AdvanceVersionCommitActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Standalone<StringRef>> const& versionEpochValue,int loopDepth) 
	{
															#line 1961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (versionEpochValue.present())
															#line 9796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<AdvanceVersionCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(ManagementAPIError::toJsonString( false, "advanceversion", "Illegal to modify the version while the version epoch is enabled")); this->~AdvanceVersionCommitActorActorState(); static_cast<AdvanceVersionCommitActorActor*>(this)->destroy(); return 0; }
															#line 9800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<AdvanceVersionCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(ManagementAPIError::toJsonString( false, "advanceversion", "Illegal to modify the version while the version epoch is enabled"));
			this->~AdvanceVersionCommitActorActorState();
			static_cast<AdvanceVersionCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		static const Version maxAllowedVerion = std::numeric_limits<int64_t>::max() - 1 - CLIENT_KNOBS->VERSIONS_PER_SECOND * 3600 * 24 * 365 * 1000;
															#line 1971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		TraceEvent(SevDebug, "AdvanceVersion").detail("MaxAllowedVersion", maxAllowedVerion);
															#line 1974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (v > maxAllowedVerion)
															#line 9816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<AdvanceVersionCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(ManagementAPIError::toJsonString( false, "advanceversion", "The given version is larger than the maximum allowed value(2**63-1-version_per_second*3600*24*365*1000)")); this->~AdvanceVersionCommitActorActorState(); static_cast<AdvanceVersionCommitActorActor*>(this)->destroy(); return 0; }
															#line 9820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<AdvanceVersionCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(ManagementAPIError::toJsonString( false, "advanceversion", "The given version is larger than the maximum allowed value(2**63-1-version_per_second*3600*24*365*1000)"));
			this->~AdvanceVersionCommitActorActorState();
			static_cast<AdvanceVersionCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Version> __when_expr_1 = ryw->getTransaction().getReadVersion();
															#line 1980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<AdvanceVersionCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<AdvanceVersionCommitActorActor*>(this)->actor_wait_state = 2;
															#line 1980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AdvanceVersionCommitActorActor, 1, Version >*>(static_cast<AdvanceVersionCommitActorActor*>(this)));
															#line 9835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Standalone<StringRef>> && versionEpochValue,int loopDepth) 
	{
															#line 1961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (versionEpochValue.present())
															#line 9844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<AdvanceVersionCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(ManagementAPIError::toJsonString( false, "advanceversion", "Illegal to modify the version while the version epoch is enabled")); this->~AdvanceVersionCommitActorActorState(); static_cast<AdvanceVersionCommitActorActor*>(this)->destroy(); return 0; }
															#line 9848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<AdvanceVersionCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(ManagementAPIError::toJsonString( false, "advanceversion", "Illegal to modify the version while the version epoch is enabled"));
			this->~AdvanceVersionCommitActorActorState();
			static_cast<AdvanceVersionCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		static const Version maxAllowedVerion = std::numeric_limits<int64_t>::max() - 1 - CLIENT_KNOBS->VERSIONS_PER_SECOND * 3600 * 24 * 365 * 1000;
															#line 1971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1972 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		TraceEvent(SevDebug, "AdvanceVersion").detail("MaxAllowedVersion", maxAllowedVerion);
															#line 1974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (v > maxAllowedVerion)
															#line 9864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<AdvanceVersionCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(ManagementAPIError::toJsonString( false, "advanceversion", "The given version is larger than the maximum allowed value(2**63-1-version_per_second*3600*24*365*1000)")); this->~AdvanceVersionCommitActorActorState(); static_cast<AdvanceVersionCommitActorActor*>(this)->destroy(); return 0; }
															#line 9868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<AdvanceVersionCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(ManagementAPIError::toJsonString( false, "advanceversion", "The given version is larger than the maximum allowed value(2**63-1-version_per_second*3600*24*365*1000)"));
			this->~AdvanceVersionCommitActorActorState();
			static_cast<AdvanceVersionCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Version> __when_expr_1 = ryw->getTransaction().getReadVersion();
															#line 1980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<AdvanceVersionCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<AdvanceVersionCommitActorActor*>(this)->actor_wait_state = 2;
															#line 1980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AdvanceVersionCommitActorActor, 1, Version >*>(static_cast<AdvanceVersionCommitActorActor*>(this)));
															#line 9883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Standalone<StringRef>> const& versionEpochValue,int loopDepth) 
	{
		loopDepth = a_body1cont1(versionEpochValue, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Standalone<StringRef>> && versionEpochValue,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(versionEpochValue), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<AdvanceVersionCommitActorActor*>(this)->actor_wait_state > 0) static_cast<AdvanceVersionCommitActorActor*>(this)->actor_wait_state = 0;
		static_cast<AdvanceVersionCommitActorActor*>(this)->ActorCallback< AdvanceVersionCommitActorActor, 0, Optional<Standalone<StringRef>> >::remove();

	}
	void a_callback_fire(ActorCallback< AdvanceVersionCommitActorActor, 0, Optional<Standalone<StringRef>> >*,Optional<Standalone<StringRef>> const& value) 
	{
		fdb_probe_actor_enter("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AdvanceVersionCommitActorActor, 0, Optional<Standalone<StringRef>> >*,Optional<Standalone<StringRef>> && value) 
	{
		fdb_probe_actor_enter("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AdvanceVersionCommitActorActor, 0, Optional<Standalone<StringRef>> >*,Error err) 
	{
		fdb_probe_actor_enter("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Version const& rv,int loopDepth) 
	{
															#line 1981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (rv <= v)
															#line 9955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().set(minRequiredCommitVersionKey, BinaryWriter::toValue(v + 1, Unversioned()));
															#line 9959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
		else
		{
															#line 1984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<AdvanceVersionCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(ManagementAPIError::toJsonString( false, "advanceversion", "Current read version is larger than the given version")); this->~AdvanceVersionCommitActorActorState(); static_cast<AdvanceVersionCommitActorActor*>(this)->destroy(); return 0; }
															#line 9965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<AdvanceVersionCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(ManagementAPIError::toJsonString( false, "advanceversion", "Current read version is larger than the given version"));
			this->~AdvanceVersionCommitActorActorState();
			static_cast<AdvanceVersionCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1987 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<AdvanceVersionCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~AdvanceVersionCommitActorActorState(); static_cast<AdvanceVersionCommitActorActor*>(this)->destroy(); return 0; }
															#line 9973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<AdvanceVersionCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~AdvanceVersionCommitActorActorState();
		static_cast<AdvanceVersionCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Version && rv,int loopDepth) 
	{
															#line 1981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (rv <= v)
															#line 9985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().set(minRequiredCommitVersionKey, BinaryWriter::toValue(v + 1, Unversioned()));
															#line 9989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
		else
		{
															#line 1984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<AdvanceVersionCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(ManagementAPIError::toJsonString( false, "advanceversion", "Current read version is larger than the given version")); this->~AdvanceVersionCommitActorActorState(); static_cast<AdvanceVersionCommitActorActor*>(this)->destroy(); return 0; }
															#line 9995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<AdvanceVersionCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(ManagementAPIError::toJsonString( false, "advanceversion", "Current read version is larger than the given version"));
			this->~AdvanceVersionCommitActorActorState();
			static_cast<AdvanceVersionCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1987 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<AdvanceVersionCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~AdvanceVersionCommitActorActorState(); static_cast<AdvanceVersionCommitActorActor*>(this)->destroy(); return 0; }
															#line 10003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<AdvanceVersionCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~AdvanceVersionCommitActorActorState();
		static_cast<AdvanceVersionCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Version const& rv,int loopDepth) 
	{
		loopDepth = a_body1cont2(rv, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Version && rv,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(rv), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<AdvanceVersionCommitActorActor*>(this)->actor_wait_state > 0) static_cast<AdvanceVersionCommitActorActor*>(this)->actor_wait_state = 0;
		static_cast<AdvanceVersionCommitActorActor*>(this)->ActorCallback< AdvanceVersionCommitActorActor, 1, Version >::remove();

	}
	void a_callback_fire(ActorCallback< AdvanceVersionCommitActorActor, 1, Version >*,Version const& value) 
	{
		fdb_probe_actor_enter("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< AdvanceVersionCommitActorActor, 1, Version >*,Version && value) 
	{
		fdb_probe_actor_enter("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< AdvanceVersionCommitActorActor, 1, Version >*,Error err) 
	{
		fdb_probe_actor_enter("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Version v;
															#line 10078 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via advanceVersionCommitActor()
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class AdvanceVersionCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< AdvanceVersionCommitActorActor, 0, Optional<Standalone<StringRef>> >, public ActorCallback< AdvanceVersionCommitActorActor, 1, Version >, public FastAllocated<AdvanceVersionCommitActorActor>, public AdvanceVersionCommitActorActorState<AdvanceVersionCommitActorActor> {
															#line 10083 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<AdvanceVersionCommitActorActor>::operator new;
	using FastAllocated<AdvanceVersionCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AdvanceVersionCommitActorActor, 0, Optional<Standalone<StringRef>> >;
friend struct ActorCallback< AdvanceVersionCommitActorActor, 1, Version >;
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	AdvanceVersionCommitActorActor(ReadYourWritesTransaction* const& ryw,Version const& v) 
															#line 10095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<Optional<std::string>>(),
		   AdvanceVersionCommitActorActorState<AdvanceVersionCommitActorActor>(ryw, v)
	{
		fdb_probe_actor_enter("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("advanceVersionCommitActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("advanceVersionCommitActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AdvanceVersionCommitActorActor, 0, Optional<Standalone<StringRef>> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< AdvanceVersionCommitActorActor, 1, Version >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<Optional<std::string>> advanceVersionCommitActor( ReadYourWritesTransaction* const& ryw, Version const& v ) {
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new AdvanceVersionCommitActorActor(ryw, v));
															#line 10124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

Future<Optional<std::string>> AdvanceVersionImpl::commit(ReadYourWritesTransaction* ryw) {
	ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
	auto minCommitVersion =
	    ryw->getSpecialKeySpaceWriteMap()[SpecialKeySpace::getManagementApiCommandPrefix("advanceversion")].second;
	if (minCommitVersion.present()) {
		try {
			// Version is int64_t
			Version v = boost::lexical_cast<int64_t>(minCommitVersion.get().toString());
			return advanceVersionCommitActor(ryw, v);
		} catch (boost::bad_lexical_cast& e) {
			return Optional<std::string>(ManagementAPIError::toJsonString(
			    false, "advanceversion", "Invalid version(int64_t) argument: " + minCommitVersion.get().toString()));
		}
	} else {
		ryw->getTransaction().clear(minRequiredCommitVersionKey);
	}
	return Optional<std::string>();
}

															#line 10148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via getVersionEpochActor()
															#line 2009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class GetVersionEpochActorActor>
															#line 2009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GetVersionEpochActorActorState {
															#line 10155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetVersionEpochActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 2009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 10164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("getVersionEpochActor", reinterpret_cast<unsigned long>(this));

	}
	~GetVersionEpochActorActorState() 
	{
		fdb_probe_actor_destroy("getVersionEpochActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 2011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 2012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(versionEpochKey);
															#line 2012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GetVersionEpochActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetVersionEpochActorActor*>(this)->actor_wait_state = 1;
															#line 2012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetVersionEpochActorActor, 0, Optional<Value> >*>(static_cast<GetVersionEpochActorActor*>(this)));
															#line 10190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~GetVersionEpochActorActorState();
		static_cast<GetVersionEpochActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 2013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 2014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 10215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			int64_t versionEpoch = BinaryReader::fromStringRef<int64_t>(val.get(), Unversioned());
															#line 2016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ValueRef version(result.arena(), boost::lexical_cast<std::string>(versionEpoch));
															#line 2017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, version));
															#line 10223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetVersionEpochActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetVersionEpochActorActorState(); static_cast<GetVersionEpochActorActor*>(this)->destroy(); return 0; }
															#line 10227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetVersionEpochActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetVersionEpochActorActorState();
		static_cast<GetVersionEpochActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 2013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 2014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 10241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			int64_t versionEpoch = BinaryReader::fromStringRef<int64_t>(val.get(), Unversioned());
															#line 2016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ValueRef version(result.arena(), boost::lexical_cast<std::string>(versionEpoch));
															#line 2017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, version));
															#line 10249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetVersionEpochActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetVersionEpochActorActorState(); static_cast<GetVersionEpochActorActor*>(this)->destroy(); return 0; }
															#line 10253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetVersionEpochActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetVersionEpochActorActorState();
		static_cast<GetVersionEpochActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<GetVersionEpochActorActor*>(this)->actor_wait_state > 0) static_cast<GetVersionEpochActorActor*>(this)->actor_wait_state = 0;
		static_cast<GetVersionEpochActorActor*>(this)->ActorCallback< GetVersionEpochActorActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< GetVersionEpochActorActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("getVersionEpochActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getVersionEpochActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetVersionEpochActorActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("getVersionEpochActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getVersionEpochActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetVersionEpochActorActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("getVersionEpochActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getVersionEpochActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 10328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getVersionEpochActor()
															#line 2009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class GetVersionEpochActorActor final : public Actor<RangeResult>, public ActorCallback< GetVersionEpochActorActor, 0, Optional<Value> >, public FastAllocated<GetVersionEpochActorActor>, public GetVersionEpochActorActorState<GetVersionEpochActorActor> {
															#line 10333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GetVersionEpochActorActor>::operator new;
	using FastAllocated<GetVersionEpochActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetVersionEpochActorActor, 0, Optional<Value> >;
															#line 2009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetVersionEpochActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 10344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   GetVersionEpochActorActorState<GetVersionEpochActorActor>(ryw, kr)
	{
		fdb_probe_actor_enter("getVersionEpochActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getVersionEpochActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getVersionEpochActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetVersionEpochActorActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<RangeResult> getVersionEpochActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 2009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new GetVersionEpochActorActor(ryw, kr));
															#line 10372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

VersionEpochImpl::VersionEpochImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

Future<RangeResult> VersionEpochImpl::getRange(ReadYourWritesTransaction* ryw,
                                               KeyRangeRef kr,
                                               GetRangeLimits limitsHint) const {
	ASSERT(kr == getKeyRange());
	return getVersionEpochActor(ryw, kr);
}

Future<Optional<std::string>> VersionEpochImpl::commit(ReadYourWritesTransaction* ryw) {
	auto versionEpoch =
	    ryw->getSpecialKeySpaceWriteMap()[SpecialKeySpace::getManagementApiCommandPrefix("versionepoch")].second;
	if (versionEpoch.present()) {
		int64_t epoch = BinaryReader::fromStringRef<int64_t>(versionEpoch.get(), Unversioned());
		ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
		ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
		ryw->getTransaction().set(versionEpochKey, BinaryWriter::toValue(epoch, Unversioned()));
	} else {
		ryw->getTransaction().clear(versionEpochKey);
	}
	return Optional<std::string>();
}

ClientProfilingImpl::ClientProfilingImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

Future<RangeResult> ClientProfilingImpl::getRange(ReadYourWritesTransaction* ryw,
                                                  KeyRangeRef kr,
                                                  GetRangeLimits limitsHint) const {
	KeyRef prefix = getKeyRange().begin;
	RangeResult result = RangeResult();
	// client_txn_sample_rate
	Key sampleRateKey = "client_txn_sample_rate"_sr.withPrefix(prefix);

	ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);

	if (kr.contains(sampleRateKey)) {
		auto entry = ryw->getSpecialKeySpaceWriteMap()[sampleRateKey];
		if (!ryw->readYourWritesDisabled() && entry.first) {
			// clear is forbidden
			ASSERT(entry.second.present());
			result.push_back_deep(result.arena(), KeyValueRef(sampleRateKey, entry.second.get()));
		} else {
			std::string sampleRateStr = "default";
			const double sampleRateDbl = ryw->getDatabase()->globalConfig->get<double>(
			    fdbClientInfoTxnSampleRate, std::numeric_limits<double>::infinity());
			if (!std::isinf(sampleRateDbl)) {
				sampleRateStr = std::to_string(sampleRateDbl);
			}
			result.push_back_deep(result.arena(), KeyValueRef(sampleRateKey, Value(sampleRateStr)));
		}
	}
	// client_txn_size_limit
	Key txnSizeLimitKey = "client_txn_size_limit"_sr.withPrefix(prefix);
	if (kr.contains(txnSizeLimitKey)) {
		auto entry = ryw->getSpecialKeySpaceWriteMap()[txnSizeLimitKey];
		if (!ryw->readYourWritesDisabled() && entry.first) {
			// clear is forbidden
			ASSERT(entry.second.present());
			result.push_back_deep(result.arena(), KeyValueRef(txnSizeLimitKey, entry.second.get()));
		} else {
			std::string sizeLimitStr = "default";
			const int64_t sizeLimit = ryw->getDatabase()->globalConfig->get<int64_t>(fdbClientInfoTxnSizeLimit, -1);
			if (sizeLimit != -1) {
				sizeLimitStr = boost::lexical_cast<std::string>(sizeLimit);
			}
			result.push_back_deep(result.arena(), KeyValueRef(txnSizeLimitKey, Value(sizeLimitStr)));
		}
	}
	return result;
}

Future<Optional<std::string>> ClientProfilingImpl::commit(ReadYourWritesTransaction* ryw) {
	ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);

	Standalone<VectorRef<KeyValueRef>> insertions;
	Standalone<VectorRef<KeyRangeRef>> clears;

	// client_txn_sample_rate
	Key sampleRateKey = "client_txn_sample_rate"_sr.withPrefix(getKeyRange().begin);
	auto rateEntry = ryw->getSpecialKeySpaceWriteMap()[sampleRateKey];

	if (rateEntry.first && rateEntry.second.present()) {
		std::string sampleRateStr = rateEntry.second.get().toString();
		if (sampleRateStr == "default") {
			clears.push_back_deep(clears.arena(),
			                      KeyRangeRef(fdbClientInfoTxnSampleRate, keyAfter(fdbClientInfoTxnSampleRate)));
		} else {
			try {
				double sampleRate = boost::lexical_cast<double>(sampleRateStr);
				Tuple rate = Tuple::makeTuple(sampleRate);
				insertions.push_back_deep(insertions.arena(), KeyValueRef(fdbClientInfoTxnSampleRate, rate.pack()));
			} catch (boost::bad_lexical_cast& e) {
				return Optional<std::string>(ManagementAPIError::toJsonString(
				    false, "profile", "Invalid transaction sample rate(double): " + sampleRateStr));
			}
		}
	}
	// client_txn_size_limit
	Key txnSizeLimitKey = "client_txn_size_limit"_sr.withPrefix(getKeyRange().begin);
	auto sizeLimitEntry = ryw->getSpecialKeySpaceWriteMap()[txnSizeLimitKey];
	if (sizeLimitEntry.first && sizeLimitEntry.second.present()) {
		std::string sizeLimitStr = sizeLimitEntry.second.get().toString();
		if (sizeLimitStr == "default") {
			clears.push_back_deep(clears.arena(),
			                      KeyRangeRef(fdbClientInfoTxnSizeLimit, keyAfter(fdbClientInfoTxnSizeLimit)));
		} else {
			try {
				int64_t sizeLimit = boost::lexical_cast<int64_t>(sizeLimitStr);
				Tuple size = Tuple::makeTuple(sizeLimit);
				insertions.push_back_deep(insertions.arena(), KeyValueRef(fdbClientInfoTxnSizeLimit, size.pack()));
			} catch (boost::bad_lexical_cast& e) {
				return Optional<std::string>(ManagementAPIError::toJsonString(
				    false, "profile", "Invalid transaction size limit(int64_t): " + sizeLimitStr));
			}
		}
	}
	GlobalConfig::applyChanges(ryw->getTransaction(), insertions, clears);
	return Optional<std::string>();
}

void ClientProfilingImpl::clear(ReadYourWritesTransaction* ryw, const KeyRangeRef& range) {
	return throwSpecialKeyApiFailure(
	    ryw, "profile", "Clear range is forbidden for profile client. You can set it to default to disable profiling.");
}

void ClientProfilingImpl::clear(ReadYourWritesTransaction* ryw, const KeyRef& key) {
	return throwSpecialKeyApiFailure(
	    ryw,
	    "profile",
	    "Clear operation is forbidden for profile client. You can set it to default to disable profiling.");
}

ActorLineageImpl::ActorLineageImpl(KeyRangeRef kr) : SpecialKeyRangeReadImpl(kr) {}

void parse(StringRef& val, int& i) {
	i = std::stoi(val.toString());
}

void parse(StringRef& val, double& d) {
	d = std::stod(val.toString());
}

void parse(StringRef& val, WaitState& w) {
	if (val == "disk"_sr || val == "Disk"_sr) {
		w = WaitState::Disk;
	} else if (val == "network"_sr || val == "Network"_sr) {
		w = WaitState::Network;
	} else if (val == "running"_sr || val == "Running"_sr) {
		w = WaitState::Running;
	} else {
		throw std::range_error("failed to parse run state");
	}
}

void parse(StringRef& val, time_t& t) {
	struct tm tm;
#ifdef _WIN32
	std::istringstream s(val.toString());
	s.imbue(std::locale(setlocale(LC_TIME, nullptr)));
	s >> std::get_time(&tm, "%FT%T%z");
	if (s.fail()) {
		throw std::invalid_argument("failed to parse ISO 8601 datetime");
	}
	long timezone;
	if (_get_timezone(&timezone) != 0) {
		throw std::runtime_error("failed to convert ISO 8601 datetime");
	}
	timezone = -timezone;
#else
	if (strptime(val.toString().c_str(), "%FT%T%z", &tm) == nullptr) {
		throw std::invalid_argument("failed to parse ISO 8601 datetime");
	}
	long timezone = tm.tm_gmtoff;
	t = timegm(&tm);
	if (t == -1) {
		throw std::runtime_error("failed to convert ISO 8601 datetime");
	}
	t -= timezone;
#endif
}

void parse(StringRef& val, NetworkAddress& a) {
	auto address = NetworkAddress::parse(val.toString());
	if (!address.isValid()) {
		throw std::invalid_argument("invalid host");
	}
	a = address;
}

// Base case function for parsing function below.
template <typename T>
void parse(std::vector<StringRef>::iterator it, std::vector<StringRef>::iterator end, T& t1) {
	if (it == end) {
		return;
	}
	parse(*it, t1);
}

// Given an iterator into a vector of string tokens, an iterator to the end of
// the search space in the vector (exclusive), and a list of references to
// types, parses each token in the vector into the associated type according to
// the order of the arguments.
//
// For example, given the vector ["1", "1.5", "127.0.0.1:4000"] and the
// argument list int a, double b, NetworkAddress c, after this function returns
// each parameter passed in will hold the parsed value from the token list.
//
// The appropriate parsing function must be implemented for the type you wish
// to parse. See the existing parsing functions above, and add your own if
// necessary.
template <typename T, typename... Types>
void parse(std::vector<StringRef>::iterator it, std::vector<StringRef>::iterator end, T& t1, Types&... remaining) {
	// Return as soon as all tokens have been parsed. This allows parameters
	// passed at the end to act as optional parameters -- they will only be set
	// if the value exists.
	if (it == end) {
		return;
	}

	try {
		parse(*it, t1);
		parse(++it, end, remaining...);
	} catch (Error& e) {
		throw e;
	} catch (std::exception& e) {
		throw e;
	}
}

															#line 10606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via actorLineageGetRangeActor()
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class ActorLineageGetRangeActorActor>
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class ActorLineageGetRangeActorActorState {
															#line 10613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ActorLineageGetRangeActorActorState(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   prefix(prefix),
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr),
															#line 2254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   result(),
															#line 2258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   host(),
															#line 2259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   waitStateStart(WaitState{ 0 }),
															#line 2260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   waitStateEnd(WaitState{ 2 }),
															#line 2261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   timeStart(0),
															#line 2262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   timeEnd(std::numeric_limits<time_t>::max()),
															#line 2263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   seqStart(0),
															#line 2264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   seqEnd(std::numeric_limits<int>::max()),
															#line 2266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   beginValues(kr.begin.removePrefix(prefix).splitAny("/"_sr)),
															#line 2267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   endValues(kr.end.removePrefix(prefix).splitAny("/"_sr))
															#line 10644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this));

	}
	~ActorLineageGetRangeActorActorState() 
	{
		fdb_probe_actor_destroy("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (beginValues.size() < 2 || endValues.size() < 2)
															#line 10659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ryw->setSpecialKeySpaceErrorMsg( ManagementAPIError::toJsonString(false, "read actor_lineage", "missing required parameters (index, host)"));
															#line 2272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 10665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			endRangeHost = NetworkAddress();
															#line 10669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			try {
															#line 2277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (SpecialKeySpace::getActorLineageApiCommandRange("state").contains(kr))
															#line 10673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					parse(beginValues.begin() + 1, beginValues.end(), host, waitStateStart, timeStart, seqStart);
															#line 2280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (kr.begin != kr.end)
															#line 10679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 2281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						parse(endValues.begin() + 1, endValues.end(), endRangeHost, waitStateEnd, timeEnd, seqEnd);
															#line 10683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
				else
				{
															#line 2283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (SpecialKeySpace::getActorLineageApiCommandRange("time").contains(kr))
															#line 10690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 2285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						parse(beginValues.begin() + 1, beginValues.end(), host, timeStart, waitStateStart, seqStart);
															#line 2286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						if (kr.begin != kr.end)
															#line 10696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 2287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
							parse(endValues.begin() + 1, endValues.end(), endRangeHost, timeEnd, waitStateEnd, seqEnd);
															#line 10700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
					}
					else
					{
															#line 2290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						ryw->setSpecialKeySpaceErrorMsg( ManagementAPIError::toJsonString(false, "read actor_lineage", "invalid index in actor_lineage"));
															#line 2292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						return a_body1Catch2(special_keys_api_failure(), loopDepth);
															#line 10709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
				loopDepth = a_body1cont10(loopDepth);
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
		this->~ActorLineageGetRangeActorActorState();
		static_cast<ActorLineageGetRangeActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 2304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (kr.begin != kr.end && host != endRangeHost)
															#line 10740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg(ManagementAPIError::toJsonString( false, "read actor_lineage", "the host must remain the same on both ends of the range"));
															#line 2309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 10746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		process = ProcessInterface();
															#line 2315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		process.getInterface = RequestStream<GetProcessInterfaceRequest>(Endpoint::wellKnown({ host }, WLTOKEN_PROCESS));
															#line 2316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<ProcessInterface> __when_expr_0 = retryBrokenPromise(process.getInterface, GetProcessInterfaceRequest{});
															#line 2316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1cont1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state = 1;
															#line 2316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ActorLineageGetRangeActorActor, 0, ProcessInterface >*>(static_cast<ActorLineageGetRangeActorActor*>(this)));
															#line 10761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 2295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (e.code() != special_keys_api_failure().code())
															#line 10771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				ryw->setSpecialKeySpaceErrorMsg( ManagementAPIError::toJsonString(false, "read actor_lineage", "failed to parse key"));
															#line 2298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 10777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 2300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 10783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
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
		try {
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont11(ProcessInterface const& p,int loopDepth) 
	{
															#line 2317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		process = p;
															#line 2319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ActorLineageRequest actorLineageRequest;
															#line 2320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.waitStateStart = waitStateStart;
															#line 2321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.waitStateEnd = waitStateEnd;
															#line 2322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.timeStart = timeStart;
															#line 2323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.timeEnd = timeEnd;
															#line 2324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<ActorLineageReply> __when_expr_1 = process.actorLineage.getReply(actorLineageRequest);
															#line 2324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont11when1(__when_expr_1.get(), loopDepth); };
		static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state = 2;
															#line 2324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ActorLineageGetRangeActorActor, 1, ActorLineageReply >*>(static_cast<ActorLineageGetRangeActorActor*>(this)));
															#line 10830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont11(ProcessInterface && p,int loopDepth) 
	{
															#line 2317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		process = p;
															#line 2319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ActorLineageRequest actorLineageRequest;
															#line 2320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.waitStateStart = waitStateStart;
															#line 2321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.waitStateEnd = waitStateEnd;
															#line 2322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.timeStart = timeStart;
															#line 2323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.timeEnd = timeEnd;
															#line 2324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<ActorLineageReply> __when_expr_1 = process.actorLineage.getReply(actorLineageRequest);
															#line 2324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont11when1(__when_expr_1.get(), loopDepth); };
		static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state = 2;
															#line 2324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ActorLineageGetRangeActorActor, 1, ActorLineageReply >*>(static_cast<ActorLineageGetRangeActorActor*>(this)));
															#line 10858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(ProcessInterface const& p,int loopDepth) 
	{
		loopDepth = a_body1cont11(p, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(ProcessInterface && p,int loopDepth) 
	{
		loopDepth = a_body1cont11(std::move(p), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state > 0) static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<ActorLineageGetRangeActorActor*>(this)->ActorCallback< ActorLineageGetRangeActorActor, 0, ProcessInterface >::remove();

	}
	void a_callback_fire(ActorCallback< ActorLineageGetRangeActorActor, 0, ProcessInterface >*,ProcessInterface const& value) 
	{
		fdb_probe_actor_enter("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ActorLineageGetRangeActorActor, 0, ProcessInterface >*,ProcessInterface && value) 
	{
		fdb_probe_actor_enter("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ActorLineageGetRangeActorActor, 0, ProcessInterface >*,Error err) 
	{
		fdb_probe_actor_enter("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont11cont1(ActorLineageReply const& reply,int loopDepth) 
	{
															#line 2326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		time_t dt = 0;
															#line 2327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		int seq = -1;
															#line 2328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( const auto& sample : reply.samples ) {
															#line 2329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			time_t datetime = (time_t)sample.time;
															#line 2330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			char buf[50];
															#line 2331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			struct tm* tm;
															#line 2332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			tm = localtime(&datetime);
															#line 2333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			size_t size = strftime(buf, 50, "%FT%T%z", tm);
															#line 2334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			std::string date(buf, size);
															#line 2336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			seq = dt == datetime ? seq + 1 : 0;
															#line 2337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			dt = datetime;
															#line 2339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& [waitState, data] : sample.data ) {
															#line 2340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (seq < seqStart)
															#line 10954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
					continue;
				}
				else
				{
															#line 2342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (seq >= seqEnd)
															#line 10962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
						break;
					}
				}
															#line 2346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				std::ostringstream streamKey;
															#line 2347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (SpecialKeySpace::getActorLineageApiCommandRange("state").contains(kr))
															#line 10971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("state").toString() << host.toString() << "/" << to_string(waitState) << "/" << date;
															#line 10975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
				else
				{
															#line 2350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (SpecialKeySpace::getActorLineageApiCommandRange("time").contains(kr))
															#line 10981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 2351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("time").toString() << host.toString() << "/" << date << "/" << to_string(waitState);
															#line 10985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
					else
					{
															#line 2354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						ASSERT(false);
															#line 10991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
															#line 2356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				streamKey << "/" << seq;
															#line 2358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				msgpack::object_handle oh = msgpack::unpack(data.data(), data.size());
															#line 2359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				msgpack::object deserialized = oh.get();
															#line 2361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				std::ostringstream stream;
															#line 2362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				stream << deserialized;
															#line 2364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back_deep(result.arena(), KeyValueRef(streamKey.str(), stream.str()));
															#line 11006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (sample.data.size() == 0)
															#line 11010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				std::ostringstream streamKey;
															#line 2369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (SpecialKeySpace::getActorLineageApiCommandRange("state").contains(kr))
															#line 11016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("state").toString() << host.toString() << "/Running/" << date;
															#line 11020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
				else
				{
															#line 2372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (SpecialKeySpace::getActorLineageApiCommandRange("time").contains(kr))
															#line 11026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 2373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("time").toString() << host.toString() << "/" << date << "/Running";
															#line 11030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
					else
					{
															#line 2376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						ASSERT(false);
															#line 11036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
															#line 2378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				streamKey << "/" << seq;
															#line 2379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back_deep(result.arena(), KeyValueRef(streamKey.str(), "{}"_sr));
															#line 11043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 2383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ActorLineageGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~ActorLineageGetRangeActorActorState(); static_cast<ActorLineageGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 11048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ActorLineageGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
		this->~ActorLineageGetRangeActorActorState();
		static_cast<ActorLineageGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont11cont1(ActorLineageReply && reply,int loopDepth) 
	{
															#line 2326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		time_t dt = 0;
															#line 2327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		int seq = -1;
															#line 2328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( const auto& sample : reply.samples ) {
															#line 2329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			time_t datetime = (time_t)sample.time;
															#line 2330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			char buf[50];
															#line 2331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			struct tm* tm;
															#line 2332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			tm = localtime(&datetime);
															#line 2333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			size_t size = strftime(buf, 50, "%FT%T%z", tm);
															#line 2334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			std::string date(buf, size);
															#line 2336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			seq = dt == datetime ? seq + 1 : 0;
															#line 2337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			dt = datetime;
															#line 2339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& [waitState, data] : sample.data ) {
															#line 2340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (seq < seqStart)
															#line 11084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
					continue;
				}
				else
				{
															#line 2342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (seq >= seqEnd)
															#line 11092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
						break;
					}
				}
															#line 2346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				std::ostringstream streamKey;
															#line 2347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (SpecialKeySpace::getActorLineageApiCommandRange("state").contains(kr))
															#line 11101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("state").toString() << host.toString() << "/" << to_string(waitState) << "/" << date;
															#line 11105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
				else
				{
															#line 2350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (SpecialKeySpace::getActorLineageApiCommandRange("time").contains(kr))
															#line 11111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 2351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("time").toString() << host.toString() << "/" << date << "/" << to_string(waitState);
															#line 11115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
					else
					{
															#line 2354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						ASSERT(false);
															#line 11121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
															#line 2356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				streamKey << "/" << seq;
															#line 2358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				msgpack::object_handle oh = msgpack::unpack(data.data(), data.size());
															#line 2359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				msgpack::object deserialized = oh.get();
															#line 2361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				std::ostringstream stream;
															#line 2362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				stream << deserialized;
															#line 2364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back_deep(result.arena(), KeyValueRef(streamKey.str(), stream.str()));
															#line 11136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (sample.data.size() == 0)
															#line 11140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				std::ostringstream streamKey;
															#line 2369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (SpecialKeySpace::getActorLineageApiCommandRange("state").contains(kr))
															#line 11146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("state").toString() << host.toString() << "/Running/" << date;
															#line 11150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
				else
				{
															#line 2372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (SpecialKeySpace::getActorLineageApiCommandRange("time").contains(kr))
															#line 11156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 2373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("time").toString() << host.toString() << "/" << date << "/Running";
															#line 11160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
					else
					{
															#line 2376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
						ASSERT(false);
															#line 11166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
															#line 2378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				streamKey << "/" << seq;
															#line 2379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back_deep(result.arena(), KeyValueRef(streamKey.str(), "{}"_sr));
															#line 11173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 2383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ActorLineageGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~ActorLineageGetRangeActorActorState(); static_cast<ActorLineageGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 11178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ActorLineageGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
		this->~ActorLineageGetRangeActorActorState();
		static_cast<ActorLineageGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont11when1(ActorLineageReply const& reply,int loopDepth) 
	{
		loopDepth = a_body1cont11cont1(reply, loopDepth);

		return loopDepth;
	}
	int a_body1cont11when1(ActorLineageReply && reply,int loopDepth) 
	{
		loopDepth = a_body1cont11cont1(std::move(reply), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state > 0) static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<ActorLineageGetRangeActorActor*>(this)->ActorCallback< ActorLineageGetRangeActorActor, 1, ActorLineageReply >::remove();

	}
	void a_callback_fire(ActorCallback< ActorLineageGetRangeActorActor, 1, ActorLineageReply >*,ActorLineageReply const& value) 
	{
		fdb_probe_actor_enter("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont11when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ActorLineageGetRangeActorActor, 1, ActorLineageReply >*,ActorLineageReply && value) 
	{
		fdb_probe_actor_enter("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont11when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ActorLineageGetRangeActorActor, 1, ActorLineageReply >*,Error err) 
	{
		fdb_probe_actor_enter("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef prefix;
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 2254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult result;
															#line 2258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	NetworkAddress host;
															#line 2259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	WaitState waitStateStart;
															#line 2260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	WaitState waitStateEnd;
															#line 2261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	time_t timeStart;
															#line 2262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	time_t timeEnd;
															#line 2263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	int seqStart;
															#line 2264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	int seqEnd;
															#line 2266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<StringRef> beginValues;
															#line 2267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<StringRef> endValues;
															#line 2275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	NetworkAddress endRangeHost;
															#line 2314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ProcessInterface process;
															#line 11279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via actorLineageGetRangeActor()
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class ActorLineageGetRangeActorActor final : public Actor<RangeResult>, public ActorCallback< ActorLineageGetRangeActorActor, 0, ProcessInterface >, public ActorCallback< ActorLineageGetRangeActorActor, 1, ActorLineageReply >, public FastAllocated<ActorLineageGetRangeActorActor>, public ActorLineageGetRangeActorActorState<ActorLineageGetRangeActorActor> {
															#line 11284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<ActorLineageGetRangeActorActor>::operator new;
	using FastAllocated<ActorLineageGetRangeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ActorLineageGetRangeActorActor, 0, ProcessInterface >;
friend struct ActorCallback< ActorLineageGetRangeActorActor, 1, ActorLineageReply >;
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ActorLineageGetRangeActorActor(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 11296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   ActorLineageGetRangeActorActorState<ActorLineageGetRangeActorActor>(ryw, prefix, kr)
	{
		fdb_probe_actor_enter("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("actorLineageGetRangeActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("actorLineageGetRangeActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ActorLineageGetRangeActorActor, 0, ProcessInterface >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ActorLineageGetRangeActorActor, 1, ActorLineageReply >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<RangeResult> actorLineageGetRangeActor( ReadYourWritesTransaction* const& ryw, KeyRef const& prefix, KeyRangeRef const& kr ) {
															#line 2251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new ActorLineageGetRangeActorActor(ryw, prefix, kr));
															#line 11325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

Future<RangeResult> ActorLineageImpl::getRange(ReadYourWritesTransaction* ryw,
                                               KeyRangeRef kr,
                                               GetRangeLimits limitsHint) const {
	return actorLineageGetRangeActor(ryw, getKeyRange().begin, kr);
}

namespace {
std::string_view to_string_view(StringRef sr) {
	return std::string_view(reinterpret_cast<const char*>(sr.begin()), sr.size());
}
} // namespace

ActorProfilerConf::ActorProfilerConf(KeyRangeRef kr)
  : SpecialKeyRangeRWImpl(kr), config(ProfilerConfig::instance().getConfig()) {}

Future<RangeResult> ActorProfilerConf::getRange(ReadYourWritesTransaction* ryw,
                                                KeyRangeRef kr,
                                                GetRangeLimits limitsHint) const {
	RangeResult res;
	std::string_view begin(to_string_view(kr.begin.removePrefix(range.begin))),
	    end(to_string_view(kr.end.removePrefix(range.begin)));
	for (auto& p : config) {
		if (p.first > end) {
			break;
		} else if (p.first > begin) {
			KeyValueRef kv;
			kv.key = StringRef(res.arena(), p.first).withPrefix(kr.begin, res.arena());
			kv.value = StringRef(res.arena(), p.second);
			res.push_back(res.arena(), kv);
		}
	}
	return res;
}

void ActorProfilerConf::set(ReadYourWritesTransaction* ryw, const KeyRef& key, const ValueRef& value) {
	config[key.removePrefix(range.begin).toString()] = value.toString();
	ryw->getSpecialKeySpaceWriteMap().insert(key, std::make_pair(true, Optional<Value>(value)));
	didWrite = true;
}

void ActorProfilerConf::clear(ReadYourWritesTransaction* ryw, const KeyRangeRef& kr) {
	std::string begin(kr.begin.removePrefix(range.begin).toString()), end(kr.end.removePrefix(range.begin).toString());
	auto first = config.lower_bound(begin);
	if (first == config.end()) {
		// nothing to clear
		return;
	}
	didWrite = true;
	auto last = config.upper_bound(end);
	config.erase(first, last);
}

void ActorProfilerConf::clear(ReadYourWritesTransaction* ryw, const KeyRef& key) {
	std::string k = key.removePrefix(range.begin).toString();
	auto iter = config.find(k);
	if (iter != config.end()) {
		config.erase(iter);
	}
	didWrite = true;
}

Future<Optional<std::string>> ActorProfilerConf::commit(ReadYourWritesTransaction* ryw) {
	Optional<std::string> res{};
	try {
		if (didWrite) {
			ProfilerConfig::instance().reset(config);
		}
		return res;
	} catch (ConfigError& err) {
		return Optional<std::string>{ err.description };
	}
}

MaintenanceImpl::MaintenanceImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

// Used to read the healthZoneKey
// If the key is persisted and the delayed read version is still larger than current read version,
// we will calculate the remaining time(truncated to integer, the same as fdbcli) and return back as the value
// If the zoneId is the special one `ignoreSSFailuresZoneString`,
// value will be 0 (same as fdbcli)
															#line 11410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via MaintenanceGetRangeActor()
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class MaintenanceGetRangeActorActor>
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class MaintenanceGetRangeActorActorState {
															#line 11417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	MaintenanceGetRangeActorActorState(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   prefix(prefix),
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr),
															#line 2469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   result()
															#line 11430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("MaintenanceGetRangeActor", reinterpret_cast<unsigned long>(this));

	}
	~MaintenanceGetRangeActorActorState() 
	{
		fdb_probe_actor_destroy("MaintenanceGetRangeActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 2472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 2473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(healthyZoneKey);
															#line 2473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<MaintenanceGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<MaintenanceGetRangeActorActor*>(this)->actor_wait_state = 1;
															#line 2473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MaintenanceGetRangeActorActor, 0, Optional<Value> >*>(static_cast<MaintenanceGetRangeActorActor*>(this)));
															#line 11456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~MaintenanceGetRangeActorActorState();
		static_cast<MaintenanceGetRangeActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 2474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 11479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			auto healthyZone = decodeHealthyZoneValue(val.get());
															#line 2476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if ((healthyZone.first == ignoreSSFailuresZoneString) || (healthyZone.second > ryw->getTransaction().getReadVersion().get()))
															#line 11485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				Key zone_key = healthyZone.first.withPrefix(prefix);
															#line 2479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				double seconds = healthyZone.first == ignoreSSFailuresZoneString ? 0 : (healthyZone.second - ryw->getTransaction().getReadVersion().get()) / CLIENT_KNOBS->CORE_VERSIONSPERSECOND;
															#line 2483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (kr.contains(zone_key))
															#line 11493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					result.push_back_deep(result.arena(), KeyValueRef(zone_key, Value(boost::lexical_cast<std::string>(seconds))));
															#line 11497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 2489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<MaintenanceGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~MaintenanceGetRangeActorActorState(); static_cast<MaintenanceGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 11503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<MaintenanceGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(rywGetRange(ryw, kr, result));
		this->~MaintenanceGetRangeActorActorState();
		static_cast<MaintenanceGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 2474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 11515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			auto healthyZone = decodeHealthyZoneValue(val.get());
															#line 2476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if ((healthyZone.first == ignoreSSFailuresZoneString) || (healthyZone.second > ryw->getTransaction().getReadVersion().get()))
															#line 11521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				Key zone_key = healthyZone.first.withPrefix(prefix);
															#line 2479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				double seconds = healthyZone.first == ignoreSSFailuresZoneString ? 0 : (healthyZone.second - ryw->getTransaction().getReadVersion().get()) / CLIENT_KNOBS->CORE_VERSIONSPERSECOND;
															#line 2483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (kr.contains(zone_key))
															#line 11529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					result.push_back_deep(result.arena(), KeyValueRef(zone_key, Value(boost::lexical_cast<std::string>(seconds))));
															#line 11533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 2489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<MaintenanceGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~MaintenanceGetRangeActorActorState(); static_cast<MaintenanceGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 11539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<MaintenanceGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(rywGetRange(ryw, kr, result));
		this->~MaintenanceGetRangeActorActorState();
		static_cast<MaintenanceGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<MaintenanceGetRangeActorActor*>(this)->actor_wait_state > 0) static_cast<MaintenanceGetRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<MaintenanceGetRangeActorActor*>(this)->ActorCallback< MaintenanceGetRangeActorActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< MaintenanceGetRangeActorActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("MaintenanceGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("MaintenanceGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MaintenanceGetRangeActorActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("MaintenanceGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("MaintenanceGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MaintenanceGetRangeActorActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("MaintenanceGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("MaintenanceGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef prefix;
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 2469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult result;
															#line 11618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via MaintenanceGetRangeActor()
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class MaintenanceGetRangeActorActor final : public Actor<RangeResult>, public ActorCallback< MaintenanceGetRangeActorActor, 0, Optional<Value> >, public FastAllocated<MaintenanceGetRangeActorActor>, public MaintenanceGetRangeActorActorState<MaintenanceGetRangeActorActor> {
															#line 11623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<MaintenanceGetRangeActorActor>::operator new;
	using FastAllocated<MaintenanceGetRangeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MaintenanceGetRangeActorActor, 0, Optional<Value> >;
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	MaintenanceGetRangeActorActor(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 11634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   MaintenanceGetRangeActorActorState<MaintenanceGetRangeActorActor>(ryw, prefix, kr)
	{
		fdb_probe_actor_enter("MaintenanceGetRangeActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("MaintenanceGetRangeActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("MaintenanceGetRangeActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MaintenanceGetRangeActorActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<RangeResult> MaintenanceGetRangeActor( ReadYourWritesTransaction* const& ryw, KeyRef const& prefix, KeyRangeRef const& kr ) {
															#line 2466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new MaintenanceGetRangeActorActor(ryw, prefix, kr));
															#line 11662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

Future<RangeResult> MaintenanceImpl::getRange(ReadYourWritesTransaction* ryw,
                                              KeyRangeRef kr,
                                              GetRangeLimits limitsHint) const {
	return MaintenanceGetRangeActor(ryw, getKeyRange().begin, kr);
}

// Commit the change to healthZoneKey
// We do not allow more than one zone to be set in maintenance in one transaction
// In addition, if the zoneId now is 'ignoreSSFailuresZoneString',
// which means the data distribution is disabled for storage failures.
// Only clear this specific key is allowed, any other operations will throw error
															#line 11678 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via maintenanceCommitActor()
															#line 2503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class MaintenanceCommitActorActor>
															#line 2503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class MaintenanceCommitActorActorState {
															#line 11685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	MaintenanceCommitActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 2503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 11694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("maintenanceCommitActor", reinterpret_cast<unsigned long>(this));

	}
	~MaintenanceCommitActorActorState() 
	{
		fdb_probe_actor_destroy("maintenanceCommitActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 2506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 2507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 2508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(healthyZoneKey);
															#line 2508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<MaintenanceCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<MaintenanceCommitActorActor*>(this)->actor_wait_state = 1;
															#line 2508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MaintenanceCommitActorActor, 0, Optional<Value> >*>(static_cast<MaintenanceCommitActorActor*>(this)));
															#line 11722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~MaintenanceCommitActorActorState();
		static_cast<MaintenanceCommitActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 2509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		Optional<std::pair<Key, Version>> healthyZone = val.present() ? decodeHealthyZoneValue(val.get()) : Optional<std::pair<Key, Version>>();
															#line 2512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(kr);
															#line 2514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		Key zoneId;
															#line 2515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		double seconds;
															#line 2516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		bool isSet = false;
															#line 2520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for(auto iter = ranges.begin();iter != ranges.end();++iter) {
															#line 2521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!iter->value().first)
															#line 11757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
				continue;
			}
															#line 2523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (iter->value().second.present())
															#line 11763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (isSet)
															#line 11767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>(ManagementAPIError::toJsonString( false, "maintenance", "Multiple zones given for maintenance, only one allowed at the same time"))); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					new (&static_cast<MaintenanceCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>(ManagementAPIError::toJsonString( false, "maintenance", "Multiple zones given for maintenance, only one allowed at the same time")));
					this->~MaintenanceCommitActorActorState();
					static_cast<MaintenanceCommitActorActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 2527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				isSet = true;
															#line 2528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				zoneId = iter->begin().removePrefix(kr.begin);
															#line 2529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				seconds = boost::lexical_cast<double>(iter->value().second.get().toString());
															#line 11783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 2532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (!isSet && healthyZone.present() && iter.range().contains(healthyZone.get().first.withPrefix(kr.begin)))
															#line 11789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					ryw->getTransaction().clear(healthyZoneKey);
															#line 11793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 2537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (isSet)
															#line 11799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (healthyZone.present() && healthyZone.get().first == ignoreSSFailuresZoneString)
															#line 11803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				std::string msg = "Maintenance mode cannot be used while data distribution is disabled for storage " "server failures.";
															#line 2541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg))); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				new (&static_cast<MaintenanceCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg)));
				this->~MaintenanceCommitActorActorState();
				static_cast<MaintenanceCommitActorActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 2542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (seconds < 0)
															#line 11819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					std::string msg = "The specified maintenance time " + boost::lexical_cast<std::string>(seconds) + " is a negative value";
															#line 2545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg))); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					new (&static_cast<MaintenanceCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg)));
					this->~MaintenanceCommitActorActorState();
					static_cast<MaintenanceCommitActorActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
				else
				{
															#line 2547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					TraceEvent(SevDebug, "SKSMaintenanceSet").detail("ZoneId", zoneId.toString());
															#line 2548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					ryw->getTransaction().set(healthyZoneKey, healthyZoneValue(zoneId, ryw->getTransaction().getReadVersion().get() + (seconds * CLIENT_KNOBS->CORE_VERSIONSPERSECOND)));
															#line 11837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 2554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<MaintenanceCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~MaintenanceCommitActorActorState();
		static_cast<MaintenanceCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 2509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		Optional<std::pair<Key, Version>> healthyZone = val.present() ? decodeHealthyZoneValue(val.get()) : Optional<std::pair<Key, Version>>();
															#line 2512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(kr);
															#line 2514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		Key zoneId;
															#line 2515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		double seconds;
															#line 2516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		bool isSet = false;
															#line 2520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for(auto iter = ranges.begin();iter != ranges.end();++iter) {
															#line 2521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!iter->value().first)
															#line 11867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
				continue;
			}
															#line 2523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (iter->value().second.present())
															#line 11873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (isSet)
															#line 11877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>(ManagementAPIError::toJsonString( false, "maintenance", "Multiple zones given for maintenance, only one allowed at the same time"))); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					new (&static_cast<MaintenanceCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>(ManagementAPIError::toJsonString( false, "maintenance", "Multiple zones given for maintenance, only one allowed at the same time")));
					this->~MaintenanceCommitActorActorState();
					static_cast<MaintenanceCommitActorActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 2527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				isSet = true;
															#line 2528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				zoneId = iter->begin().removePrefix(kr.begin);
															#line 2529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				seconds = boost::lexical_cast<double>(iter->value().second.get().toString());
															#line 11893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 2532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (!isSet && healthyZone.present() && iter.range().contains(healthyZone.get().first.withPrefix(kr.begin)))
															#line 11899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					ryw->getTransaction().clear(healthyZoneKey);
															#line 11903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 2537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (isSet)
															#line 11909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (healthyZone.present() && healthyZone.get().first == ignoreSSFailuresZoneString)
															#line 11913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				std::string msg = "Maintenance mode cannot be used while data distribution is disabled for storage " "server failures.";
															#line 2541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg))); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				new (&static_cast<MaintenanceCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg)));
				this->~MaintenanceCommitActorActorState();
				static_cast<MaintenanceCommitActorActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 2542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (seconds < 0)
															#line 11929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					std::string msg = "The specified maintenance time " + boost::lexical_cast<std::string>(seconds) + " is a negative value";
															#line 2545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg))); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					new (&static_cast<MaintenanceCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg)));
					this->~MaintenanceCommitActorActorState();
					static_cast<MaintenanceCommitActorActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
				else
				{
															#line 2547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					TraceEvent(SevDebug, "SKSMaintenanceSet").detail("ZoneId", zoneId.toString());
															#line 2548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					ryw->getTransaction().set(healthyZoneKey, healthyZoneValue(zoneId, ryw->getTransaction().getReadVersion().get() + (seconds * CLIENT_KNOBS->CORE_VERSIONSPERSECOND)));
															#line 11947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 2554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<MaintenanceCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~MaintenanceCommitActorActorState();
		static_cast<MaintenanceCommitActorActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<MaintenanceCommitActorActor*>(this)->actor_wait_state > 0) static_cast<MaintenanceCommitActorActor*>(this)->actor_wait_state = 0;
		static_cast<MaintenanceCommitActorActor*>(this)->ActorCallback< MaintenanceCommitActorActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< MaintenanceCommitActorActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("maintenanceCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("maintenanceCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MaintenanceCommitActorActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("maintenanceCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("maintenanceCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MaintenanceCommitActorActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("maintenanceCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("maintenanceCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 2503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 2512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, std::pair<bool, Optional<Value>>, KeyRangeRef>::Ranges ranges;
															#line 12030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via maintenanceCommitActor()
															#line 2503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class MaintenanceCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< MaintenanceCommitActorActor, 0, Optional<Value> >, public FastAllocated<MaintenanceCommitActorActor>, public MaintenanceCommitActorActorState<MaintenanceCommitActorActor> {
															#line 12035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<MaintenanceCommitActorActor>::operator new;
	using FastAllocated<MaintenanceCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MaintenanceCommitActorActor, 0, Optional<Value> >;
															#line 2503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	MaintenanceCommitActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 12046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<Optional<std::string>>(),
		   MaintenanceCommitActorActorState<MaintenanceCommitActorActor>(ryw, kr)
	{
		fdb_probe_actor_enter("maintenanceCommitActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("maintenanceCommitActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("maintenanceCommitActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MaintenanceCommitActorActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<Optional<std::string>> maintenanceCommitActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 2503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new MaintenanceCommitActorActor(ryw, kr));
															#line 12074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

Future<Optional<std::string>> MaintenanceImpl::commit(ReadYourWritesTransaction* ryw) {
	return maintenanceCommitActor(ryw, getKeyRange());
}

DataDistributionImpl::DataDistributionImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

// Read the system keys dataDistributionModeKey and rebalanceDDIgnoreKey
															#line 12086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via DataDistributionGetRangeActor()
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class DataDistributionGetRangeActorActor>
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class DataDistributionGetRangeActorActorState {
															#line 12093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	DataDistributionGetRangeActorActorState(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   prefix(prefix),
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr),
															#line 2567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   result(),
															#line 2569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   modeKey("mode"_sr.withPrefix(prefix))
															#line 12108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this));

	}
	~DataDistributionGetRangeActorActorState() 
	{
		fdb_probe_actor_destroy("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 2573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.contains(modeKey))
															#line 12125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				auto entry = ryw->getSpecialKeySpaceWriteMap()[modeKey];
															#line 2575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (ryw->readYourWritesDisabled() || !entry.first)
															#line 12131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(dataDistributionModeKey);
															#line 2576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					if (static_cast<DataDistributionGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<DataDistributionGetRangeActorActor*>(this)->actor_wait_state = 1;
															#line 2576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DataDistributionGetRangeActorActor, 0, Optional<Value> >*>(static_cast<DataDistributionGetRangeActorActor*>(this)));
															#line 12142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
					loopDepth = 0;
				}
				else
				{
					loopDepth = a_body1cont2(loopDepth);
				}
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
		this->~DataDistributionGetRangeActorActorState();
		static_cast<DataDistributionGetRangeActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 2585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		rebalanceIgnoredKey = "rebalance_ignored"_sr.withPrefix(prefix);
															#line 2586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (kr.contains(rebalanceIgnoredKey))
															#line 12177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			auto entry = ryw->getSpecialKeySpaceWriteMap()[rebalanceIgnoredKey];
															#line 2588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (ryw->readYourWritesDisabled() || !entry.first)
															#line 12183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				StrictFuture<Optional<Value>> __when_expr_1 = ryw->getTransaction().get(rebalanceDDIgnoreKey);
															#line 2589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (static_cast<DataDistributionGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
				static_cast<DataDistributionGetRangeActorActor*>(this)->actor_wait_state = 2;
															#line 2589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DataDistributionGetRangeActorActor, 1, Optional<Value> >*>(static_cast<DataDistributionGetRangeActorActor*>(this)));
															#line 12194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1cont7(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1cont6(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Optional<Value> const& f,int loopDepth) 
	{
															#line 2577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		int mode = -1;
															#line 2578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (f.present())
															#line 12221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			mode = BinaryReader::fromStringRef<int>(f.get(), Unversioned());
															#line 12225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		result.push_back_deep(result.arena(), KeyValueRef(modeKey, Value(boost::lexical_cast<std::string>(mode))));
															#line 12229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Optional<Value> && f,int loopDepth) 
	{
															#line 2577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		int mode = -1;
															#line 2578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (f.present())
															#line 12240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			mode = BinaryReader::fromStringRef<int>(f.get(), Unversioned());
															#line 12244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		result.push_back_deep(result.arena(), KeyValueRef(modeKey, Value(boost::lexical_cast<std::string>(mode))));
															#line 12248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& f,int loopDepth) 
	{
		loopDepth = a_body1cont3(f, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && f,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(f), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DataDistributionGetRangeActorActor*>(this)->actor_wait_state > 0) static_cast<DataDistributionGetRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<DataDistributionGetRangeActorActor*>(this)->ActorCallback< DataDistributionGetRangeActorActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< DataDistributionGetRangeActorActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DataDistributionGetRangeActorActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DataDistributionGetRangeActorActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont6(int loopDepth) 
	{
															#line 2595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<DataDistributionGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~DataDistributionGetRangeActorActorState(); static_cast<DataDistributionGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 12320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<DataDistributionGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(rywGetRange(ryw, kr, result));
		this->~DataDistributionGetRangeActorActorState();
		static_cast<DataDistributionGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(int loopDepth) 
	{
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont8(Optional<Value> const& f,int loopDepth) 
	{
															#line 2590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (f.present())
															#line 12338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(rebalanceIgnoredKey, Value()));
															#line 12342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont8(Optional<Value> && f,int loopDepth) 
	{
															#line 2590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (f.present())
															#line 12352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(rebalanceIgnoredKey, Value()));
															#line 12356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Optional<Value> const& f,int loopDepth) 
	{
		loopDepth = a_body1cont8(f, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Optional<Value> && f,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(f), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DataDistributionGetRangeActorActor*>(this)->actor_wait_state > 0) static_cast<DataDistributionGetRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<DataDistributionGetRangeActorActor*>(this)->ActorCallback< DataDistributionGetRangeActorActor, 1, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< DataDistributionGetRangeActorActor, 1, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DataDistributionGetRangeActorActor, 1, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DataDistributionGetRangeActorActor, 1, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef prefix;
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 2567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult result;
															#line 2569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Key modeKey;
															#line 2585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Key rebalanceIgnoredKey;
															#line 12437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via DataDistributionGetRangeActor()
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class DataDistributionGetRangeActorActor final : public Actor<RangeResult>, public ActorCallback< DataDistributionGetRangeActorActor, 0, Optional<Value> >, public ActorCallback< DataDistributionGetRangeActorActor, 1, Optional<Value> >, public FastAllocated<DataDistributionGetRangeActorActor>, public DataDistributionGetRangeActorActorState<DataDistributionGetRangeActorActor> {
															#line 12442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<DataDistributionGetRangeActorActor>::operator new;
	using FastAllocated<DataDistributionGetRangeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DataDistributionGetRangeActorActor, 0, Optional<Value> >;
friend struct ActorCallback< DataDistributionGetRangeActorActor, 1, Optional<Value> >;
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	DataDistributionGetRangeActorActor(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 12454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   DataDistributionGetRangeActorActorState<DataDistributionGetRangeActorActor>(ryw, prefix, kr)
	{
		fdb_probe_actor_enter("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("DataDistributionGetRangeActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("DataDistributionGetRangeActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DataDistributionGetRangeActorActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DataDistributionGetRangeActorActor, 1, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<RangeResult> DataDistributionGetRangeActor( ReadYourWritesTransaction* const& ryw, KeyRef const& prefix, KeyRangeRef const& kr ) {
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new DataDistributionGetRangeActorActor(ryw, prefix, kr));
															#line 12483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

Future<RangeResult> DataDistributionImpl::getRange(ReadYourWritesTransaction* ryw,
                                                   KeyRangeRef kr,
                                                   GetRangeLimits limitsHint) const {
	return DataDistributionGetRangeActor(ryw, getKeyRange().begin, kr);
}

Future<Optional<std::string>> DataDistributionImpl::commit(ReadYourWritesTransaction* ryw) {
	// there are two valid keys in the range
	// <prefix>/mode -> dataDistributionModeKey, the value is only allowed to be set as "0"(disable) or "1"(enable)
	// <prefix>/rebalance_ignored -> rebalanceDDIgnoreKey, value is unused thus empty
	ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);

	Optional<std::string> msg;
	KeyRangeRef kr = getKeyRange();
	Key modeKey = "mode"_sr.withPrefix(kr.begin);
	Key rebalanceIgnoredKey = "rebalance_ignored"_sr.withPrefix(kr.begin);
	auto ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(kr);
	for (auto iter = ranges.begin(); iter != ranges.end(); ++iter) {
		if (!iter->value().first)
			continue;
		if (iter->value().second.present()) {
			if (iter->range() == singleKeyRange(modeKey)) {
				try {
					int mode = boost::lexical_cast<int>(iter->value().second.get().toString());
					Value modeVal = BinaryWriter::toValue(mode, Unversioned());
					if (mode == 0 || mode == 1 || mode == 2) {
						// Whenever configuration changes or DD related system keyspace is changed,
						// actor must grab the moveKeysLockOwnerKey and update moveKeysLockWriteKey.
						// This prevents concurrent write to the same system keyspace.
						// When the owner of the DD related system keyspace changes, DD will reboot
						BinaryWriter wrMyOwner(Unversioned());
						wrMyOwner << dataDistributionModeLock;
						ryw->getTransaction().set(moveKeysLockOwnerKey, wrMyOwner.toValue());
						BinaryWriter wrLastWrite(Unversioned());
						wrLastWrite << deterministicRandom()->randomUniqueID();
						ryw->getTransaction().set(moveKeysLockWriteKey, wrLastWrite.toValue());
						// set mode
						ryw->getTransaction().set(dataDistributionModeKey, modeVal);
					} else
						msg = ManagementAPIError::toJsonString(false,
						                                       "datadistribution",
						                                       "Please set the value of the data_distribution/mode to "
						                                       "0(disable) or 1(enable), other values are not allowed");
				} catch (boost::bad_lexical_cast& e) {
					msg = ManagementAPIError::toJsonString(false,
					                                       "datadistribution",
					                                       "Invalid datadistribution mode(int): " +
					                                           iter->value().second.get().toString());
				}
			} else if (iter->range() == singleKeyRange(rebalanceIgnoredKey)) {
				ValueRef val = iter->value().second.get();
				try {
					boost::lexical_cast<int>(iter->value().second.get().toString());
				} catch (boost::bad_lexical_cast& e) {
					ManagementAPIError::toJsonString(
					    false,
					    "datadistribution",
					    "Invalid datadistribution rebalance ignore option (int or empty): " +
					        iter->value().second.get().toString());
					val = ""_sr;
				}
				ryw->getTransaction().set(rebalanceDDIgnoreKey, iter->value().second.get());
			} else {
				msg = ManagementAPIError::toJsonString(
				    false,
				    "datadistribution",
				    "Changing invalid keys, please read the documentation to check valid keys in the range");
			}
		} else {
			// clear
			if (iter->range().contains(modeKey))
				ryw->getTransaction().clear(dataDistributionModeKey);
			else if (iter->range().contains(rebalanceIgnoredKey))
				ryw->getTransaction().clear(rebalanceDDIgnoreKey);
		}
	}
	return msg;
}

// Clears the special management api keys excludeLocality and failedLocality.
void includeLocalities(ReadYourWritesTransaction* ryw) {
	ryw->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
	ryw->setOption(FDBTransactionOptions::LOCK_AWARE);
	ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
	ryw->setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
	// includeLocalities might be used in an emergency transaction, so make sure it is retry-self-conflicting and
	// CAUSAL_WRITE_RISKY
	ryw->setOption(FDBTransactionOptions::CAUSAL_WRITE_RISKY);
	std::string versionKey = deterministicRandom()->randomUniqueID().toString();
	// for excluded localities
	auto ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(
	    SpecialKeySpace::getManagementApiCommandRange("excludedlocality"));
	Transaction& tr = ryw->getTransaction();
	for (auto& iter : ranges) {
		auto entry = iter.value();
		if (entry.first && !entry.second.present()) {
			tr.addReadConflictRange(singleKeyRange(excludedLocalityVersionKey));
			tr.set(excludedLocalityVersionKey, versionKey);
			tr.clear(ryw->getDatabase()->specialKeySpace->decode(iter.range()));
		}
	}
	// for failed localities
	ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(
	    SpecialKeySpace::getManagementApiCommandRange("failedlocality"));
	for (auto& iter : ranges) {
		auto entry = iter.value();
		if (entry.first && !entry.second.present()) {
			tr.addReadConflictRange(singleKeyRange(failedLocalityVersionKey));
			tr.set(failedLocalityVersionKey, versionKey);
			tr.clear(ryw->getDatabase()->specialKeySpace->decode(iter.range()));
		}
	}
}

// Reads the excludedlocality and failed locality keys using management api,
// parses them and returns the list.
bool parseLocalitiesFromKeys(ReadYourWritesTransaction* ryw,
                             bool failed,
                             std::unordered_set<std::string>& localities,
                             std::vector<AddressExclusion>& addresses,
                             std::set<AddressExclusion>& exclusions,
                             std::vector<ProcessData>& workers,
                             Optional<std::string>& msg) {
	KeyRangeRef range = failed ? SpecialKeySpace::getManagementApiCommandRange("failedlocality")
	                           : SpecialKeySpace::getManagementApiCommandRange("excludedlocality");
	auto ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(range);
	auto iter = ranges.begin();
	while (iter != ranges.end()) {
		auto entry = iter->value();
		// only check for exclude(set) operation, include(clear) are not checked
		TraceEvent(SevDebug, "ParseLocalities")
		    .detail("Valid", entry.first)
		    .detail("Set", entry.second.present())
		    .detail("Key", iter->begin().toString());
		if (entry.first && entry.second.present()) {
			Key locality = iter->begin().removePrefix(range.begin);
			if (locality.startsWith(LocalityData::ExcludeLocalityPrefix) &&
			    locality.toString().find(':') != std::string::npos) {
				std::set<AddressExclusion> localityAddresses = getAddressesByLocality(workers, locality.toString());
				if (!localityAddresses.empty()) {
					std::copy(localityAddresses.begin(), localityAddresses.end(), back_inserter(addresses));
					exclusions.insert(localityAddresses.begin(), localityAddresses.end());
				}

				localities.insert(locality.toString());
			} else {
				std::string error = "ERROR: \'" + locality.toString() + "\' is not a valid locality\n";
				msg = ManagementAPIError::toJsonString(
				    false, entry.second.present() ? (failed ? "exclude failed" : "exclude") : "include", error);
				return false;
			}
		}
		++iter;
	}
	return true;
}

// On commit, parses the special exclusion keys and get the localities to be excluded, check for exclusions
// and add them to the exclusion list. Also, clears the special management api keys with includeLocalities.
															#line 12647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via excludeLocalityCommitActor()
															#line 2757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class ExcludeLocalityCommitActorActor>
															#line 2757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class ExcludeLocalityCommitActorActorState {
															#line 12654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ExcludeLocalityCommitActorActorState(ReadYourWritesTransaction* const& ryw,bool const& failed) 
															#line 2757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   failed(failed),
															#line 2758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   result(),
															#line 2759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   localities(),
															#line 2760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   addresses(),
															#line 2761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   exclusions()
															#line 12671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this));

	}
	~ExcludeLocalityCommitActorActorState() 
	{
		fdb_probe_actor_destroy("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 2765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<std::vector<ProcessData>> __when_expr_0 = getWorkers(&ryw->getTransaction());
															#line 2765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state = 1;
															#line 2765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExcludeLocalityCommitActorActor, 0, std::vector<ProcessData> >*>(static_cast<ExcludeLocalityCommitActorActor*>(this)));
															#line 12695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~ExcludeLocalityCommitActorActorState();
		static_cast<ExcludeLocalityCommitActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 2766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!parseLocalitiesFromKeys(ryw, failed, localities, addresses, exclusions, workers, result))
															#line 12718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeLocalityCommitActorActorState(); static_cast<ExcludeLocalityCommitActorActor*>(this)->destroy(); return 0; }
															#line 12722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(result)); // state_var_RVO
			this->~ExcludeLocalityCommitActorActorState();
			static_cast<ExcludeLocalityCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 2769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto force = ryw->getSpecialKeySpaceWriteMap()[SpecialKeySpace::getManagementApiCommandOptionSpecialKey( failed ? "failed_locality" : "excluded_locality", "force")];
															#line 2772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (localities.size() && !(force.first && force.second.present()))
															#line 12732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<bool> __when_expr_1 = checkExclusion(ryw->getDatabase(), &addresses, &exclusions, failed, &result);
															#line 2773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state = 2;
															#line 2773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExcludeLocalityCommitActorActor, 1, bool >*>(static_cast<ExcludeLocalityCommitActorActor*>(this)));
															#line 12743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(std::vector<ProcessData> const& __workers,int loopDepth) 
	{
															#line 2765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		workers = __workers;
															#line 12757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<ProcessData> && __workers,int loopDepth) 
	{
		workers = std::move(__workers);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state > 0) static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state = 0;
		static_cast<ExcludeLocalityCommitActorActor*>(this)->ActorCallback< ExcludeLocalityCommitActorActor, 0, std::vector<ProcessData> >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeLocalityCommitActorActor, 0, std::vector<ProcessData> >*,std::vector<ProcessData> const& value) 
	{
		fdb_probe_actor_enter("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ExcludeLocalityCommitActorActor, 0, std::vector<ProcessData> >*,std::vector<ProcessData> && value) 
	{
		fdb_probe_actor_enter("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ExcludeLocalityCommitActorActor, 0, std::vector<ProcessData> >*,Error err) 
	{
		fdb_probe_actor_enter("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 2778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Void> __when_expr_2 = excludeLocalities(&ryw->getTransaction(), localities, failed);
															#line 2778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state = 3;
															#line 2778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExcludeLocalityCommitActorActor, 2, Void >*>(static_cast<ExcludeLocalityCommitActorActor*>(this)));
															#line 12831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(bool const& safe,int loopDepth) 
	{
															#line 2774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!safe)
															#line 12840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeLocalityCommitActorActorState(); static_cast<ExcludeLocalityCommitActorActor*>(this)->destroy(); return 0; }
															#line 12844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(result)); // state_var_RVO
			this->~ExcludeLocalityCommitActorActorState();
			static_cast<ExcludeLocalityCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(bool && safe,int loopDepth) 
	{
															#line 2774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!safe)
															#line 12858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeLocalityCommitActorActorState(); static_cast<ExcludeLocalityCommitActorActor*>(this)->destroy(); return 0; }
															#line 12862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(result)); // state_var_RVO
			this->~ExcludeLocalityCommitActorActorState();
			static_cast<ExcludeLocalityCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(bool const& safe,int loopDepth) 
	{
		loopDepth = a_body1cont4(safe, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(bool && safe,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(safe), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state > 0) static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state = 0;
		static_cast<ExcludeLocalityCommitActorActor*>(this)->ActorCallback< ExcludeLocalityCommitActorActor, 1, bool >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeLocalityCommitActorActor, 1, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ExcludeLocalityCommitActorActor, 1, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ExcludeLocalityCommitActorActor, 1, bool >*,Error err) 
	{
		fdb_probe_actor_enter("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 2779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		includeLocalities(ryw);
															#line 2781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeLocalityCommitActorActorState(); static_cast<ExcludeLocalityCommitActorActor*>(this)->destroy(); return 0; }
															#line 12941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(result)); // state_var_RVO
		this->~ExcludeLocalityCommitActorActorState();
		static_cast<ExcludeLocalityCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 2779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		includeLocalities(ryw);
															#line 2781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeLocalityCommitActorActorState(); static_cast<ExcludeLocalityCommitActorActor*>(this)->destroy(); return 0; }
															#line 12955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(result)); // state_var_RVO
		this->~ExcludeLocalityCommitActorActorState();
		static_cast<ExcludeLocalityCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state > 0) static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state = 0;
		static_cast<ExcludeLocalityCommitActorActor*>(this)->ActorCallback< ExcludeLocalityCommitActorActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ExcludeLocalityCommitActorActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ExcludeLocalityCommitActorActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ExcludeLocalityCommitActorActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 2757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	bool failed;
															#line 2758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Optional<std::string> result;
															#line 2759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::unordered_set<std::string> localities;
															#line 2760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<AddressExclusion> addresses;
															#line 2761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::set<AddressExclusion> exclusions;
															#line 2765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<ProcessData> workers;
															#line 13040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via excludeLocalityCommitActor()
															#line 2757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class ExcludeLocalityCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< ExcludeLocalityCommitActorActor, 0, std::vector<ProcessData> >, public ActorCallback< ExcludeLocalityCommitActorActor, 1, bool >, public ActorCallback< ExcludeLocalityCommitActorActor, 2, Void >, public FastAllocated<ExcludeLocalityCommitActorActor>, public ExcludeLocalityCommitActorActorState<ExcludeLocalityCommitActorActor> {
															#line 13045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<ExcludeLocalityCommitActorActor>::operator new;
	using FastAllocated<ExcludeLocalityCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ExcludeLocalityCommitActorActor, 0, std::vector<ProcessData> >;
friend struct ActorCallback< ExcludeLocalityCommitActorActor, 1, bool >;
friend struct ActorCallback< ExcludeLocalityCommitActorActor, 2, Void >;
															#line 2757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ExcludeLocalityCommitActorActor(ReadYourWritesTransaction* const& ryw,bool const& failed) 
															#line 13058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<Optional<std::string>>(),
		   ExcludeLocalityCommitActorActorState<ExcludeLocalityCommitActorActor>(ryw, failed)
	{
		fdb_probe_actor_enter("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("excludeLocalityCommitActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("excludeLocalityCommitActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ExcludeLocalityCommitActorActor, 0, std::vector<ProcessData> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ExcludeLocalityCommitActorActor, 1, bool >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ExcludeLocalityCommitActorActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Optional<std::string>> excludeLocalityCommitActor( ReadYourWritesTransaction* const& ryw, bool const& failed ) {
															#line 2757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new ExcludeLocalityCommitActorActor(ryw, failed));
															#line 13088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

ExcludedLocalitiesRangeImpl::ExcludedLocalitiesRangeImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

Future<RangeResult> ExcludedLocalitiesRangeImpl::getRange(ReadYourWritesTransaction* ryw,
                                                          KeyRangeRef kr,
                                                          GetRangeLimits limitsHint) const {
	ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
	return rwModuleWithMappingGetRangeActor(ryw, this, kr);
}

void ExcludedLocalitiesRangeImpl::set(ReadYourWritesTransaction* ryw, const KeyRef& key, const ValueRef& value) {
	// ignore value
	ryw->getSpecialKeySpaceWriteMap().insert(key, std::make_pair(true, Optional<Value>(ValueRef())));
}

Key ExcludedLocalitiesRangeImpl::decode(const KeyRef& key) const {
	return key.removePrefix(SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin)
	    .withPrefix("\xff/conf/"_sr);
}

Key ExcludedLocalitiesRangeImpl::encode(const KeyRef& key) const {
	return key.removePrefix("\xff/conf/"_sr)
	    .withPrefix(SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin);
}

Future<Optional<std::string>> ExcludedLocalitiesRangeImpl::commit(ReadYourWritesTransaction* ryw) {
	// exclude locality with failed option as false.
	return excludeLocalityCommitActor(ryw, false);
}

FailedLocalitiesRangeImpl::FailedLocalitiesRangeImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

Future<RangeResult> FailedLocalitiesRangeImpl::getRange(ReadYourWritesTransaction* ryw,
                                                        KeyRangeRef kr,
                                                        GetRangeLimits limitsHint) const {
	ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
	return rwModuleWithMappingGetRangeActor(ryw, this, kr);
}

void FailedLocalitiesRangeImpl::set(ReadYourWritesTransaction* ryw, const KeyRef& key, const ValueRef& value) {
	// ignore value
	ryw->getSpecialKeySpaceWriteMap().insert(key, std::make_pair(true, Optional<Value>(ValueRef())));
}

Key FailedLocalitiesRangeImpl::decode(const KeyRef& key) const {
	return key.removePrefix(SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin)
	    .withPrefix("\xff/conf/"_sr);
}

Key FailedLocalitiesRangeImpl::encode(const KeyRef& key) const {
	return key.removePrefix("\xff/conf/"_sr)
	    .withPrefix(SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin);
}

Future<Optional<std::string>> FailedLocalitiesRangeImpl::commit(ReadYourWritesTransaction* ryw) {
	// exclude locality with failed option as true.
	return excludeLocalityCommitActor(ryw, true);
}

// Defined in ReadYourWrites.actor.cpp
															#line 13152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
[[nodiscard]] Future<RangeResult> getWorkerInterfaces( Reference<IClusterConnectionRecord> const& clusterRecord );

#line 2844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
// Defined in NativeAPI.actor.cpp
															#line 13157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
[[nodiscard]] Future<bool> verifyInterfaceActor( Reference<FlowLock> const& connectLock, ClientWorkerInterface const& workerInterf );

#line 2846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

															#line 13162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via workerInterfacesImplGetRangeActor()
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class WorkerInterfacesImplGetRangeActorActor>
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class WorkerInterfacesImplGetRangeActorActorState {
															#line 13169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	WorkerInterfacesImplGetRangeActorActorState(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   prefix(prefix),
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 13180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this));

	}
	~WorkerInterfacesImplGetRangeActorActorState() 
	{
		fdb_probe_actor_destroy("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!ryw->getDatabase().getPtr() || !ryw->getDatabase()->getConnectionRecord())
															#line 13195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (!static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResult()); this->~WorkerInterfacesImplGetRangeActorActorState(); static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 13199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				new (&static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResult());
				this->~WorkerInterfacesImplGetRangeActorActorState();
				static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 2853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = getWorkerInterfaces(ryw->getDatabase()->getConnectionRecord());
															#line 2853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->actor_wait_state = 1;
															#line 2853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WorkerInterfacesImplGetRangeActorActor, 0, RangeResult >*>(static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)));
															#line 13214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~WorkerInterfacesImplGetRangeActorActorState();
		static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 2855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		auto [verify, _] = ryw->getSpecialKeySpaceWriteMap()[SpecialKeySpace::getManagementApiCommandOptionSpecialKey( "worker_interfaces", "verify")];
															#line 2857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		result = RangeResult();
															#line 2858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (verify)
															#line 13241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			Reference<FlowLock> connectLock(new FlowLock(CLIENT_KNOBS->CLI_CONNECT_PARALLELISM));
															#line 2861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			verifyInterfs = std::vector<Future<bool>>();
															#line 2862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& [k_, value] : interfs ) {
															#line 2863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				auto k = k_.withPrefix(prefix);
															#line 2864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (kr.contains(k))
															#line 13253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					ClientWorkerInterface workerInterf = BinaryReader::fromStringRef<ClientWorkerInterface>(value, IncludeVersion());
															#line 2867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					verifyInterfs.push_back(verifyInterfaceActor(connectLock, workerInterf));
															#line 13259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
				else
				{
															#line 2869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					verifyInterfs.push_back(false);
															#line 13265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
															#line 2872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Void> __when_expr_1 = waitForAll(verifyInterfs);
															#line 2872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->actor_wait_state = 2;
															#line 2872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WorkerInterfacesImplGetRangeActorActor, 1, Void >*>(static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)));
															#line 13277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 2882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& [k_, v] : interfs ) {
															#line 2883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				auto k = k_.withPrefix(prefix);
															#line 2884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (kr.contains(k))
															#line 13288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					result.push_back_deep(result.arena(), KeyValueRef(k, v));
															#line 13292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
			loopDepth = a_body1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(RangeResult const& __interfs,int loopDepth) 
	{
															#line 2853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		interfs = __interfs;
															#line 13304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && __interfs,int loopDepth) 
	{
		interfs = std::move(__interfs);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->actor_wait_state > 0) static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->ActorCallback< WorkerInterfacesImplGetRangeActorActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< WorkerInterfacesImplGetRangeActorActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WorkerInterfacesImplGetRangeActorActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WorkerInterfacesImplGetRangeActorActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 2888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		std::sort(result.begin(), result.end(), KeyValueRef::OrderByKey{});
															#line 2889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~WorkerInterfacesImplGetRangeActorActorState(); static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 13373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
		this->~WorkerInterfacesImplGetRangeActorActorState();
		static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 2874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for(int index = 0;index < interfs.size();index++) {
															#line 2875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (verifyInterfs[index].get())
															#line 13387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back_deep(result.arena(), KeyValueRef(interfs[index].key.withPrefix(prefix), interfs[index].value));
															#line 13391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 2874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for(int index = 0;index < interfs.size();index++) {
															#line 2875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (verifyInterfs[index].get())
															#line 13404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back_deep(result.arena(), KeyValueRef(interfs[index].key.withPrefix(prefix), interfs[index].value));
															#line 13408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->actor_wait_state > 0) static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<WorkerInterfacesImplGetRangeActorActor*>(this)->ActorCallback< WorkerInterfacesImplGetRangeActorActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WorkerInterfacesImplGetRangeActorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WorkerInterfacesImplGetRangeActorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WorkerInterfacesImplGetRangeActorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef prefix;
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 2853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult interfs;
															#line 2857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult result;
															#line 2861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<Future<bool>> verifyInterfs;
															#line 13490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via workerInterfacesImplGetRangeActor()
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class WorkerInterfacesImplGetRangeActorActor final : public Actor<RangeResult>, public ActorCallback< WorkerInterfacesImplGetRangeActorActor, 0, RangeResult >, public ActorCallback< WorkerInterfacesImplGetRangeActorActor, 1, Void >, public FastAllocated<WorkerInterfacesImplGetRangeActorActor>, public WorkerInterfacesImplGetRangeActorActorState<WorkerInterfacesImplGetRangeActorActor> {
															#line 13495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<WorkerInterfacesImplGetRangeActorActor>::operator new;
	using FastAllocated<WorkerInterfacesImplGetRangeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WorkerInterfacesImplGetRangeActorActor, 0, RangeResult >;
friend struct ActorCallback< WorkerInterfacesImplGetRangeActorActor, 1, Void >;
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	WorkerInterfacesImplGetRangeActorActor(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 13507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   WorkerInterfacesImplGetRangeActorActorState<WorkerInterfacesImplGetRangeActorActor>(ryw, prefix, kr)
	{
		fdb_probe_actor_enter("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("workerInterfacesImplGetRangeActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("workerInterfacesImplGetRangeActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WorkerInterfacesImplGetRangeActorActor, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WorkerInterfacesImplGetRangeActorActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<RangeResult> workerInterfacesImplGetRangeActor( ReadYourWritesTransaction* const& ryw, KeyRef const& prefix, KeyRangeRef const& kr ) {
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new WorkerInterfacesImplGetRangeActorActor(ryw, prefix, kr));
															#line 13536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"

WorkerInterfacesSpecialKeyImpl::WorkerInterfacesSpecialKeyImpl(KeyRangeRef kr) : SpecialKeyRangeReadImpl(kr) {}

Future<RangeResult> WorkerInterfacesSpecialKeyImpl::getRange(ReadYourWritesTransaction* ryw,
                                                             KeyRangeRef kr,
                                                             GetRangeLimits limitsHint) const {
	return workerInterfacesImplGetRangeActor(ryw, getKeyRange().begin, kr);
}

															#line 13549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via validateSpecialSubrangeRead()
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
template <class ValidateSpecialSubrangeReadActor>
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class ValidateSpecialSubrangeReadActorState {
															#line 13556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ValidateSpecialSubrangeReadActorState(ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Reverse const& reverse,RangeResult const& result) 
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   begin(begin),
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   end(end),
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   limits(limits),
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   reverse(reverse),
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		   result(result)
															#line 13573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this));

	}
	~ValidateSpecialSubrangeReadActorState() 
	{
		fdb_probe_actor_destroy("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (!result.size())
															#line 13588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				StrictFuture<RangeResult> __when_expr_0 = ryw->getRange(begin, end, limits, Snapshot::True, reverse);
															#line 2907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (static_cast<ValidateSpecialSubrangeReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ValidateSpecialSubrangeReadActor*>(this)->actor_wait_state = 1;
															#line 2907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ValidateSpecialSubrangeReadActor, 0, RangeResult >*>(static_cast<ValidateSpecialSubrangeReadActor*>(this)));
															#line 13599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~ValidateSpecialSubrangeReadActorState();
		static_cast<ValidateSpecialSubrangeReadActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 2912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (reverse)
															#line 13627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(std::is_sorted(result.begin(), result.end(), KeyValueRef::OrderByKeyBack{}));
															#line 13631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
		else
		{
															#line 2915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(std::is_sorted(result.begin(), result.end(), KeyValueRef::OrderByKey{}));
															#line 13637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		std::vector<Key> candidateKeys;
															#line 2921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (reverse)
															#line 13643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for(int i = result.size() - 1;i >= 0;--i) {
															#line 2923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				candidateKeys.emplace_back(result[i].key);
															#line 2924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (i - 1 >= 0)
															#line 13651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					candidateKeys.emplace_back(keyBetween(KeyRangeRef(result[i].key, result[i - 1].key)));
															#line 13655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
		else
		{
															#line 2929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for(int i = 0;i < result.size();++i) {
															#line 2930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				candidateKeys.emplace_back(result[i].key);
															#line 2931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				if (i + 1 < result.size())
															#line 13667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
					candidateKeys.emplace_back(keyBetween(KeyRangeRef(result[i].key, result[i + 1].key)));
															#line 13671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 2936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		std::sort(candidateKeys.begin(), candidateKeys.end());
															#line 2937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		int originalSize = candidateKeys.size();
															#line 2939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for(int i = 0;i < originalSize - 1;++i) {
															#line 2940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			candidateKeys.emplace_back(keyBetween(KeyRangeRef(candidateKeys[i], candidateKeys[i + 1])));
															#line 13683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		std::vector<Key> keys;
															#line 2943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		keys = { deterministicRandom()->randomChoice(candidateKeys), deterministicRandom()->randomChoice(candidateKeys) };
															#line 2944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		std::sort(keys.begin(), keys.end());
															#line 2945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		testBegin = firstGreaterOrEqual(keys[0]);
															#line 2946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		testEnd = firstGreaterOrEqual(keys[1]);
															#line 2950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		expectedResult = RangeResult();
															#line 2953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		for( const auto& kr : result ) {
															#line 2954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.key >= keys[0] && kr.key < keys[1])
															#line 13701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				expectedResult.push_back(expectedResult.arena(), kr);
															#line 13705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 2960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = ryw->getRange(testBegin, testEnd, limits, Snapshot::True, reverse);
															#line 2960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ValidateSpecialSubrangeReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ValidateSpecialSubrangeReadActor*>(this)->actor_wait_state = 2;
															#line 2960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ValidateSpecialSubrangeReadActor, 1, RangeResult >*>(static_cast<ValidateSpecialSubrangeReadActor*>(this)));
															#line 13717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(RangeResult const& testResult,int loopDepth) 
	{
															#line 2908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(testResult == result);
															#line 2909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ValidateSpecialSubrangeReadActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ValidateSpecialSubrangeReadActorState(); static_cast<ValidateSpecialSubrangeReadActor*>(this)->destroy(); return 0; }
															#line 13728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ValidateSpecialSubrangeReadActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ValidateSpecialSubrangeReadActorState();
		static_cast<ValidateSpecialSubrangeReadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(RangeResult && testResult,int loopDepth) 
	{
															#line 2908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(testResult == result);
															#line 2909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ValidateSpecialSubrangeReadActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ValidateSpecialSubrangeReadActorState(); static_cast<ValidateSpecialSubrangeReadActor*>(this)->destroy(); return 0; }
															#line 13742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ValidateSpecialSubrangeReadActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ValidateSpecialSubrangeReadActorState();
		static_cast<ValidateSpecialSubrangeReadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& testResult,int loopDepth) 
	{
		loopDepth = a_body1cont2(testResult, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && testResult,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(testResult), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ValidateSpecialSubrangeReadActor*>(this)->actor_wait_state > 0) static_cast<ValidateSpecialSubrangeReadActor*>(this)->actor_wait_state = 0;
		static_cast<ValidateSpecialSubrangeReadActor*>(this)->ActorCallback< ValidateSpecialSubrangeReadActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ValidateSpecialSubrangeReadActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ValidateSpecialSubrangeReadActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ValidateSpecialSubrangeReadActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(RangeResult const& testResult,int loopDepth) 
	{
															#line 2961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (testResult != expectedResult)
															#line 13817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Reverse: {}\n", reverse);
															#line 2963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Original range: [{}, {})\n", begin.toString(), end.toString());
															#line 2964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Original result:\n");
															#line 2965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& kr : result ) {
															#line 2966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				fmt::print("	{} -> {}\n", kr.key.printable(), kr.value.printable());
															#line 13829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Test range: [{}, {})\n", testBegin.getKey().printable(), testEnd.getKey().printable());
															#line 2969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Expected:\n");
															#line 2970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& kr : expectedResult ) {
															#line 2971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				fmt::print("	{} -> {}\n", kr.key.printable(), kr.value.printable());
															#line 13839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Got:\n");
															#line 2974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& kr : testResult ) {
															#line 2975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				fmt::print("	{} -> {}\n", kr.key.printable(), kr.value.printable());
															#line 13847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(testResult == expectedResult);
															#line 13851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ValidateSpecialSubrangeReadActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ValidateSpecialSubrangeReadActorState(); static_cast<ValidateSpecialSubrangeReadActor*>(this)->destroy(); return 0; }
															#line 13855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ValidateSpecialSubrangeReadActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ValidateSpecialSubrangeReadActorState();
		static_cast<ValidateSpecialSubrangeReadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(RangeResult && testResult,int loopDepth) 
	{
															#line 2961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (testResult != expectedResult)
															#line 13867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Reverse: {}\n", reverse);
															#line 2963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Original range: [{}, {})\n", begin.toString(), end.toString());
															#line 2964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Original result:\n");
															#line 2965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& kr : result ) {
															#line 2966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				fmt::print("	{} -> {}\n", kr.key.printable(), kr.value.printable());
															#line 13879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Test range: [{}, {})\n", testBegin.getKey().printable(), testEnd.getKey().printable());
															#line 2969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Expected:\n");
															#line 2970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& kr : expectedResult ) {
															#line 2971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				fmt::print("	{} -> {}\n", kr.key.printable(), kr.value.printable());
															#line 13889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Got:\n");
															#line 2974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& kr : testResult ) {
															#line 2975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
				fmt::print("	{} -> {}\n", kr.key.printable(), kr.value.printable());
															#line 13897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(testResult == expectedResult);
															#line 13901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ValidateSpecialSubrangeReadActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ValidateSpecialSubrangeReadActorState(); static_cast<ValidateSpecialSubrangeReadActor*>(this)->destroy(); return 0; }
															#line 13905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ValidateSpecialSubrangeReadActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ValidateSpecialSubrangeReadActorState();
		static_cast<ValidateSpecialSubrangeReadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult const& testResult,int loopDepth) 
	{
		loopDepth = a_body1cont4(testResult, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult && testResult,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(testResult), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ValidateSpecialSubrangeReadActor*>(this)->actor_wait_state > 0) static_cast<ValidateSpecialSubrangeReadActor*>(this)->actor_wait_state = 0;
		static_cast<ValidateSpecialSubrangeReadActor*>(this)->ActorCallback< ValidateSpecialSubrangeReadActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ValidateSpecialSubrangeReadActor, 1, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ValidateSpecialSubrangeReadActor, 1, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ValidateSpecialSubrangeReadActor, 1, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector begin;
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector end;
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	GetRangeLimits limits;
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	Reverse reverse;
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult result;
															#line 2945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector testBegin;
															#line 2946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector testEnd;
															#line 2950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult expectedResult;
															#line 13994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via validateSpecialSubrangeRead()
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
class ValidateSpecialSubrangeReadActor final : public Actor<Void>, public ActorCallback< ValidateSpecialSubrangeReadActor, 0, RangeResult >, public ActorCallback< ValidateSpecialSubrangeReadActor, 1, RangeResult >, public FastAllocated<ValidateSpecialSubrangeReadActor>, public ValidateSpecialSubrangeReadActorState<ValidateSpecialSubrangeReadActor> {
															#line 13999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<ValidateSpecialSubrangeReadActor>::operator new;
	using FastAllocated<ValidateSpecialSubrangeReadActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ValidateSpecialSubrangeReadActor, 0, RangeResult >;
friend struct ActorCallback< ValidateSpecialSubrangeReadActor, 1, RangeResult >;
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	ValidateSpecialSubrangeReadActor(ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Reverse const& reverse,RangeResult const& result) 
															#line 14011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<Void>(),
		   ValidateSpecialSubrangeReadActorState<ValidateSpecialSubrangeReadActor>(ryw, begin, end, limits, reverse, result)
	{
		fdb_probe_actor_enter("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("validateSpecialSubrangeRead");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("validateSpecialSubrangeRead", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ValidateSpecialSubrangeReadActor, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ValidateSpecialSubrangeReadActor, 1, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Void> validateSpecialSubrangeRead( ReadYourWritesTransaction* const& ryw, KeySelector const& begin, KeySelector const& end, GetRangeLimits const& limits, Reverse const& reverse, RangeResult const& result ) {
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Void>(new ValidateSpecialSubrangeReadActor(ryw, begin, end, limits, reverse, result));
															#line 14040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SpecialKeySpace.actor.cpp"
