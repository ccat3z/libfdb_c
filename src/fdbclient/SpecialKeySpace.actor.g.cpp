#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
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

const KeyRangeRef TenantMapRangeImpl::submoduleRange = KeyRangeRef("tenant_map/"_sr, "tenant_map0"_sr);

std::unordered_map<SpecialKeySpace::MODULE, KeyRange> SpecialKeySpace::moduleToBoundary = {
	{ SpecialKeySpace::MODULE::TRANSACTION,
	  KeyRangeRef(LiteralStringRef("\xff\xff/transaction/"), LiteralStringRef("\xff\xff/transaction0")) },
	{ SpecialKeySpace::MODULE::WORKERINTERFACE,
	  KeyRangeRef(LiteralStringRef("\xff\xff/worker_interfaces/"), LiteralStringRef("\xff\xff/worker_interfaces0")) },
	{ SpecialKeySpace::MODULE::STATUSJSON, singleKeyRange(LiteralStringRef("\xff\xff/status/json")) },
	{ SpecialKeySpace::MODULE::CONNECTIONSTRING, singleKeyRange(LiteralStringRef("\xff\xff/connection_string")) },
	{ SpecialKeySpace::MODULE::CLUSTERFILEPATH, singleKeyRange(LiteralStringRef("\xff\xff/cluster_file_path")) },
	{ SpecialKeySpace::MODULE::METRICS,
	  KeyRangeRef(LiteralStringRef("\xff\xff/metrics/"), LiteralStringRef("\xff\xff/metrics0")) },
	{ SpecialKeySpace::MODULE::MANAGEMENT,
	  KeyRangeRef(LiteralStringRef("\xff\xff/management/"), LiteralStringRef("\xff\xff/management0")) },
	{ SpecialKeySpace::MODULE::ERRORMSG, singleKeyRange(LiteralStringRef("\xff\xff/error_message")) },
	{ SpecialKeySpace::MODULE::CONFIGURATION,
	  KeyRangeRef(LiteralStringRef("\xff\xff/configuration/"), LiteralStringRef("\xff\xff/configuration0")) },
	{ SpecialKeySpace::MODULE::GLOBALCONFIG,
	  KeyRangeRef(LiteralStringRef("\xff\xff/global_config/"), LiteralStringRef("\xff\xff/global_config0")) },
	{ SpecialKeySpace::MODULE::TRACING,
	  KeyRangeRef(LiteralStringRef("\xff\xff/tracing/"), LiteralStringRef("\xff\xff/tracing0")) },
	{ SpecialKeySpace::MODULE::ACTORLINEAGE,
	  KeyRangeRef(LiteralStringRef("\xff\xff/actor_lineage/"), LiteralStringRef("\xff\xff/actor_lineage0")) },
	{ SpecialKeySpace::MODULE::ACTOR_PROFILER_CONF,
	  KeyRangeRef(LiteralStringRef("\xff\xff/actor_profiler_conf/"),
	              LiteralStringRef("\xff\xff/actor_profiler_conf0")) }
};

std::unordered_map<std::string, KeyRange> SpecialKeySpace::managementApiCommandToRange = {
	{ "exclude",
	  KeyRangeRef(LiteralStringRef("excluded/"), LiteralStringRef("excluded0"))
	      .withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "failed",
	  KeyRangeRef(LiteralStringRef("failed/"), LiteralStringRef("failed0"))
	      .withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "excludedlocality",
	  KeyRangeRef(LiteralStringRef("excluded_locality/"), LiteralStringRef("excluded_locality0"))
	      .withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "failedlocality",
	  KeyRangeRef(LiteralStringRef("failed_locality/"), LiteralStringRef("failed_locality0"))
	      .withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "lock", singleKeyRange(LiteralStringRef("db_locked")).withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "consistencycheck",
	  singleKeyRange(LiteralStringRef("consistency_check_suspended"))
	      .withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "coordinators",
	  KeyRangeRef(LiteralStringRef("coordinators/"), LiteralStringRef("coordinators0"))
	      .withPrefix(moduleToBoundary[MODULE::CONFIGURATION].begin) },
	{ "advanceversion",
	  singleKeyRange(LiteralStringRef("min_required_commit_version"))
	      .withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "versionepoch",
	  singleKeyRange(LiteralStringRef("version_epoch")).withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "profile",
	  KeyRangeRef(LiteralStringRef("profiling/"), LiteralStringRef("profiling0"))
	      .withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "maintenance",
	  KeyRangeRef(LiteralStringRef("maintenance/"), LiteralStringRef("maintenance0"))
	      .withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "datadistribution",
	  KeyRangeRef(LiteralStringRef("data_distribution/"), LiteralStringRef("data_distribution0"))
	      .withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) },
	{ "tenantmap", TenantMapRangeImpl::submoduleRange.withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin) }
};

std::unordered_map<std::string, KeyRange> SpecialKeySpace::actorLineageApiCommandToRange = {
	{ "state",
	  KeyRangeRef(LiteralStringRef("state/"), LiteralStringRef("state0"))
	      .withPrefix(moduleToBoundary[MODULE::ACTORLINEAGE].begin) },
	{ "time",
	  KeyRangeRef(LiteralStringRef("time/"), LiteralStringRef("time0"))
	      .withPrefix(moduleToBoundary[MODULE::ACTORLINEAGE].begin) }
};

std::set<std::string> SpecialKeySpace::options = { "excluded/force",
	                                               "failed/force",
	                                               "excluded_locality/force",
	                                               "failed_locality/force" };

std::set<std::string> SpecialKeySpace::tracingOptions = { kTracingTransactionIdKey, kTracingTokenKey };

RangeResult rywGetRange(ReadYourWritesTransaction* ryw, const KeyRangeRef& kr, const RangeResult& res);

// This function will move the given KeySelector as far as possible to the standard form:
// orEqual == false && offset == 1 (Standard form)
// If the corresponding key is not in the underlying key range, it will move over the range
// The cache object is used to cache the first read result from the rpc call during the key resolution,
// then when we need to do key resolution or result filtering,
// we, instead of rpc call, read from this cache object have consistent results
															#line 151 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via moveKeySelectorOverRangeActor()
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class MoveKeySelectorOverRangeActorActor>
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class MoveKeySelectorOverRangeActorActorState {
															#line 158 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	MoveKeySelectorOverRangeActorActorState(const SpecialKeyRangeReadImpl* const& skrImpl,ReadYourWritesTransaction* const& ryw,KeySelector* const& ks,KeyRangeMap<Optional<RangeResult>>* const& cache) 
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : skrImpl(skrImpl),
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   ryw(ryw),
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   ks(ks),
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   cache(cache)
															#line 171 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 154 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(!ks->orEqual);
															#line 157 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(ks->offset != 1);
															#line 159 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			startKey = Key(skrImpl->getKeyRange().begin);
															#line 160 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			endKey = Key(skrImpl->getKeyRange().end);
															#line 161 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result = RangeResult();
															#line 163 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (ks->offset < 1)
															#line 196 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 165 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (skrImpl->getKeyRange().contains(ks->getKey()))
															#line 200 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 166 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					endKey = ks->getKey();
															#line 204 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
			else
			{
															#line 169 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (skrImpl->getKeyRange().contains(ks->getKey()))
															#line 211 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 170 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					startKey = ks->getKey();
															#line 215 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
															#line 174 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(startKey < endKey);
															#line 176 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			DisabledTraceEvent(SevDebug, "NormalizeKeySelector") .detail("OriginalKey", ks->getKey()) .detail("OriginalOffset", ks->offset) .detail("SpecialKeyRangeStart", skrImpl->getKeyRange().begin) .detail("SpecialKeyRangeEnd", skrImpl->getKeyRange().end);
															#line 182 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			GetRangeLimits limitsHint(ks->offset >= 1 ? ks->offset : 1 - ks->offset);
															#line 184 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (skrImpl->isAsync())
															#line 226 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 185 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				const SpecialKeyRangeAsyncImpl* ptr = dynamic_cast<const SpecialKeyRangeAsyncImpl*>(skrImpl);
															#line 186 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				StrictFuture<RangeResult> __when_expr_0 = ptr->getRange(ryw, KeyRangeRef(startKey, endKey), limitsHint, cache);
															#line 186 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (static_cast<MoveKeySelectorOverRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 234 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<MoveKeySelectorOverRangeActorActor*>(this)->actor_wait_state = 1;
															#line 186 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MoveKeySelectorOverRangeActorActor, 0, RangeResult >*>(static_cast<MoveKeySelectorOverRangeActorActor*>(this)));
															#line 239 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 189 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				StrictFuture<RangeResult> __when_expr_1 = skrImpl->getRange(ryw, KeyRangeRef(startKey, endKey), limitsHint);
															#line 189 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (static_cast<MoveKeySelectorOverRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 248 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
				static_cast<MoveKeySelectorOverRangeActorActor*>(this)->actor_wait_state = 2;
															#line 189 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MoveKeySelectorOverRangeActorActor, 1, RangeResult >*>(static_cast<MoveKeySelectorOverRangeActorActor*>(this)));
															#line 253 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 193 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (result.size() == 0)
															#line 277 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 194 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			TraceEvent(SevDebug, "ZeroElementsIntheRange").detail("Start", startKey).detail("End", endKey);
															#line 195 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<MoveKeySelectorOverRangeActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MoveKeySelectorOverRangeActorActorState(); static_cast<MoveKeySelectorOverRangeActorActor*>(this)->destroy(); return 0; }
															#line 283 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<MoveKeySelectorOverRangeActorActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~MoveKeySelectorOverRangeActorActorState();
			static_cast<MoveKeySelectorOverRangeActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 198 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (ks->offset < 1)
															#line 291 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 199 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (result.size() >= 1 - ks->offset)
															#line 295 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 200 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ks->setKey(KeyRef(ks->arena(), result[result.size() - (1 - ks->offset)].key));
															#line 201 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ks->offset = 1;
															#line 301 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 203 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ks->setKey(KeyRef(ks->arena(), result[0].key));
															#line 204 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ks->offset += result.size();
															#line 309 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
		else
		{
															#line 207 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (result.size() >= ks->offset)
															#line 316 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 208 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ks->setKey(KeyRef(ks->arena(), result[ks->offset - 1].key));
															#line 209 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ks->offset = 1;
															#line 322 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 212 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ks->setKey(KeyRef(ks->arena(), keyAfter(result[result.size() - 1].key)));
															#line 213 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ks->offset -= result.size();
															#line 330 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 216 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		DisabledTraceEvent(SevDebug, "NormalizeKeySelector") .detail("NormalizedKey", ks->getKey()) .detail("NormalizedOffset", ks->offset) .detail("SpecialKeyRangeStart", skrImpl->getKeyRange().begin) .detail("SpecialKeyRangeEnd", skrImpl->getKeyRange().end);
															#line 221 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<MoveKeySelectorOverRangeActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MoveKeySelectorOverRangeActorActorState(); static_cast<MoveKeySelectorOverRangeActorActor*>(this)->destroy(); return 0; }
															#line 337 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<MoveKeySelectorOverRangeActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~MoveKeySelectorOverRangeActorActorState();
		static_cast<MoveKeySelectorOverRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(RangeResult const& result_,int loopDepth) 
	{
															#line 187 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		result = result_;
															#line 349 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont6(RangeResult && result_,int loopDepth) 
	{
															#line 187 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		result = result_;
															#line 358 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult const& result_,int loopDepth) 
	{
		loopDepth = a_body1cont6(result_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && result_,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(result_), loopDepth);

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
	int a_body1cont8(RangeResult const& result_,int loopDepth) 
	{
															#line 190 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		result = result_;
															#line 430 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8(RangeResult && result_,int loopDepth) 
	{
															#line 190 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		result = result_;
															#line 439 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when2(RangeResult const& result_,int loopDepth) 
	{
		loopDepth = a_body1cont8(result_, loopDepth);

		return loopDepth;
	}
	int a_body1when2(RangeResult && result_,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(result_), loopDepth);

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
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	const SpecialKeyRangeReadImpl* skrImpl;
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector* ks;
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeMap<Optional<RangeResult>>* cache;
															#line 159 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Key startKey;
															#line 160 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Key endKey;
															#line 161 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult result;
															#line 521 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via moveKeySelectorOverRangeActor()
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class MoveKeySelectorOverRangeActorActor final : public Actor<Void>, public ActorCallback< MoveKeySelectorOverRangeActorActor, 0, RangeResult >, public ActorCallback< MoveKeySelectorOverRangeActorActor, 1, RangeResult >, public FastAllocated<MoveKeySelectorOverRangeActorActor>, public MoveKeySelectorOverRangeActorActorState<MoveKeySelectorOverRangeActorActor> {
															#line 526 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<MoveKeySelectorOverRangeActorActor>::operator new;
	using FastAllocated<MoveKeySelectorOverRangeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MoveKeySelectorOverRangeActorActor, 0, RangeResult >;
friend struct ActorCallback< MoveKeySelectorOverRangeActorActor, 1, RangeResult >;
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	MoveKeySelectorOverRangeActorActor(const SpecialKeyRangeReadImpl* const& skrImpl,ReadYourWritesTransaction* const& ryw,KeySelector* const& ks,KeyRangeMap<Optional<RangeResult>>* const& cache) 
															#line 538 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Void> moveKeySelectorOverRangeActor( const SpecialKeyRangeReadImpl* const& skrImpl, ReadYourWritesTransaction* const& ryw, KeySelector* const& ks, KeyRangeMap<Optional<RangeResult>>* const& cache ) {
															#line 149 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Void>(new MoveKeySelectorOverRangeActorActor(skrImpl, ryw, ks, cache));
															#line 567 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 223 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

// This function will normalize the given KeySelector to a standard KeySelector:
// orEqual == false && offset == 1 (Standard form)
// If the corresponding key is outside the whole space, it will move to the begin or the end
// It does have overhead here since we query all keys twice in the worst case.
// However, moving the KeySelector while handling other parameters like limits makes the code much more complex and hard
// to maintain; Thus, separate each part to make the code easy to understand and more compact
// Boundary is the range of the legal key space, which, by default is the range of the module
// And (\xff\xff, \xff\xff\xff) if SPECIAL_KEY_SPACE_RELAXED is turned on
															#line 580 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via normalizeKeySelectorActor()
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class NormalizeKeySelectorActorActor>
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class NormalizeKeySelectorActorActorState {
															#line 587 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	NormalizeKeySelectorActorActorState(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeySelector* const& ks,KeyRangeRef const& boundary,int* const& actualOffset,RangeResult* const& result,KeyRangeMap<Optional<RangeResult>>* const& cache) 
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : sks(sks),
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   ryw(ryw),
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   ks(ks),
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   boundary(boundary),
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   actualOffset(actualOffset),
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   result(result),
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   cache(cache),
															#line 244 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   iter(ks->offset < 1 ? sks->getReadImpls().rangeContainingKeyBefore(ks->getKey()) : sks->getReadImpls().rangeContaining(ks->getKey()))
															#line 608 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 247 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			;
															#line 623 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 263 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		*actualOffset = ks->offset;
															#line 265 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!ks->isFirstGreaterOrEqual())
															#line 648 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 266 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			TraceEvent(SevDebug, "ReadToBoundary") .detail("TerminateKey", ks->getKey()) .detail("TerminateOffset", ks->offset);
															#line 271 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (ks->offset < 1)
															#line 654 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 272 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result->readToBegin = true;
															#line 273 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ks->setKey(boundary.begin);
															#line 660 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 275 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result->readThroughEnd = true;
															#line 276 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ks->setKey(boundary.end);
															#line 668 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 278 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ks->offset = 1;
															#line 672 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 280 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<NormalizeKeySelectorActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~NormalizeKeySelectorActorActorState(); static_cast<NormalizeKeySelectorActorActor*>(this)->destroy(); return 0; }
															#line 676 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 247 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!((ks->offset < 1 && iter->begin() >= boundary.begin) || (ks->offset > 1 && iter->begin() < boundary.end)))
															#line 695 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 248 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (iter->value() != nullptr)
															#line 701 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 249 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Void> __when_expr_0 = moveKeySelectorOverRangeActor(iter->value(), ryw, ks, cache);
															#line 249 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<NormalizeKeySelectorActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 707 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<NormalizeKeySelectorActorActor*>(this)->actor_wait_state = 1;
															#line 249 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< NormalizeKeySelectorActorActor, 0, Void >*>(static_cast<NormalizeKeySelectorActorActor*>(this)));
															#line 712 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 252 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (ks->offset < 1)
															#line 739 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 253 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (iter == sks->getReadImpls().ranges().begin())
															#line 743 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
				return a_body1break1(loopDepth==0?0:loopDepth-1); // break
			}
			else
			{
															#line 256 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				--iter;
															#line 751 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
		else
		{
															#line 258 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (ks->offset > 1)
															#line 758 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 260 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				++iter;
															#line 762 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace* sks;
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector* ks;
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef boundary;
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	int* actualOffset;
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult* result;
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeMap<Optional<RangeResult>>* cache;
															#line 244 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, SpecialKeyRangeReadImpl*, KeyRangeRef>::iterator iter;
															#line 860 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via normalizeKeySelectorActor()
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class NormalizeKeySelectorActorActor final : public Actor<Void>, public ActorCallback< NormalizeKeySelectorActorActor, 0, Void >, public FastAllocated<NormalizeKeySelectorActorActor>, public NormalizeKeySelectorActorActorState<NormalizeKeySelectorActorActor> {
															#line 865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<NormalizeKeySelectorActorActor>::operator new;
	using FastAllocated<NormalizeKeySelectorActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< NormalizeKeySelectorActorActor, 0, Void >;
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	NormalizeKeySelectorActorActor(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeySelector* const& ks,KeyRangeRef const& boundary,int* const& actualOffset,RangeResult* const& result,KeyRangeMap<Optional<RangeResult>>* const& cache) 
															#line 876 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Void> normalizeKeySelectorActor( SpecialKeySpace* const& sks, ReadYourWritesTransaction* const& ryw, KeySelector* const& ks, KeyRangeRef const& boundary, int* const& actualOffset, RangeResult* const& result, KeyRangeMap<Optional<RangeResult>>* const& cache ) {
															#line 232 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Void>(new NormalizeKeySelectorActorActor(sks, ryw, ks, boundary, actualOffset, result, cache));
															#line 904 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 282 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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

															#line 937 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
// This generated class is to be used only via checkRYWValid()
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class SpecialKeySpace_CheckRYWValidActor>
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class SpecialKeySpace_CheckRYWValidActorState {
															#line 943 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace_CheckRYWValidActorState(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Reverse const& reverse) 
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : sks(sks),
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   ryw(ryw),
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   begin(begin),
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   end(end),
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   limits(limits),
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   reverse(reverse)
															#line 960 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 317 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(ryw);
															#line 319 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = SpecialKeySpace::getRangeAggregationActor(sks, ryw, begin, end, limits, reverse);
															#line 318 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 979 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 323 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ryw->resetFuture();
															#line 983 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->actor_wait_state = 1;
															#line 319 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_CheckRYWValidActor, 0, RangeResult >*>(static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)));
															#line 323 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_CheckRYWValidActor, 1, Void >*>(static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)));
															#line 990 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 321 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~SpecialKeySpace_CheckRYWValidActorState(); static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->destroy(); return 0; }
															#line 1013 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~SpecialKeySpace_CheckRYWValidActorState();
		static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult && result,int loopDepth) 
	{
															#line 321 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~SpecialKeySpace_CheckRYWValidActorState(); static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->destroy(); return 0; }
															#line 1025 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~SpecialKeySpace_CheckRYWValidActorState();
		static_cast<SpecialKeySpace_CheckRYWValidActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 324 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 1037 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 324 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 1045 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"

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
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace* sks;
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector begin;
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector end;
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetRangeLimits limits;
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Reverse reverse;
															#line 1158 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via checkRYWValid()
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class SpecialKeySpace_CheckRYWValidActor final : public Actor<RangeResult>, public ActorCallback< SpecialKeySpace_CheckRYWValidActor, 0, RangeResult >, public ActorCallback< SpecialKeySpace_CheckRYWValidActor, 1, Void >, public FastAllocated<SpecialKeySpace_CheckRYWValidActor>, public SpecialKeySpace_CheckRYWValidActorState<SpecialKeySpace_CheckRYWValidActor> {
															#line 1163 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<SpecialKeySpace_CheckRYWValidActor>::operator new;
	using FastAllocated<SpecialKeySpace_CheckRYWValidActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SpecialKeySpace_CheckRYWValidActor, 0, RangeResult >;
friend struct ActorCallback< SpecialKeySpace_CheckRYWValidActor, 1, Void >;
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace_CheckRYWValidActor(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Reverse const& reverse) 
															#line 1175 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> SpecialKeySpace::checkRYWValid( SpecialKeySpace* const& sks, ReadYourWritesTransaction* const& ryw, KeySelector const& begin, KeySelector const& end, GetRangeLimits const& limits, Reverse const& reverse ) {
															#line 311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new SpecialKeySpace_CheckRYWValidActor(sks, ryw, begin, end, limits, reverse));
															#line 1202 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 328 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

															#line 1207 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
// This generated class is to be used only via getRangeAggregationActor()
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class SpecialKeySpace_GetRangeAggregationActorActor>
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class SpecialKeySpace_GetRangeAggregationActorActorState {
															#line 1213 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace_GetRangeAggregationActorActorState(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Reverse const& reverse) 
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : sks(sks),
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   ryw(ryw),
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   begin(begin),
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   end(end),
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   limits(limits),
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   reverse(reverse),
															#line 337 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   result(),
															#line 338 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   pairs(),
															#line 339 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   iter(),
															#line 340 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   actualBeginOffset(),
															#line 341 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   actualEndOffset(),
															#line 342 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   moduleBoundary(),
															#line 345 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   cache(Optional<RangeResult>(), specialKeys.end)
															#line 1244 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 347 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (ryw->specialKeySpaceRelaxed())
															#line 1259 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 348 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				moduleBoundary = sks->range;
															#line 1263 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 350 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				auto beginIter = sks->getModules().rangeContaining(begin.getKey());
															#line 351 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (beginIter->begin() <= end.getKey() && end.getKey() <= beginIter->end())
															#line 1271 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 352 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (beginIter->value() == SpecialKeySpace::MODULE::UNKNOWN)
															#line 1275 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 353 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						return a_body1Catch1(special_keys_no_module_found(), loopDepth);
															#line 1279 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
					else
					{
															#line 355 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						moduleBoundary = beginIter->range();
															#line 1285 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
				else
				{
															#line 357 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					TraceEvent(SevInfo, "SpecialKeyCrossModuleRead") .detail("Begin", begin) .detail("End", end) .detail("BoundaryBegin", beginIter->begin()) .detail("BoundaryEnd", beginIter->end());
															#line 362 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					return a_body1Catch1(special_keys_cross_module_read(), loopDepth);
															#line 1294 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
															#line 366 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Void> __when_expr_0 = normalizeKeySelectorActor(sks, ryw, &begin, moduleBoundary, &actualBeginOffset, &result, &cache);
															#line 366 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1301 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 1;
															#line 366 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 0, Void >*>(static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)));
															#line 1306 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 367 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Void> __when_expr_1 = normalizeKeySelectorActor(sks, ryw, &end, moduleBoundary, &actualEndOffset, &result, &cache);
															#line 367 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1331 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 2;
															#line 367 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 1, Void >*>(static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)));
															#line 1336 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 367 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Void> __when_expr_1 = normalizeKeySelectorActor(sks, ryw, &end, moduleBoundary, &actualEndOffset, &result, &cache);
															#line 367 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1347 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 2;
															#line 367 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 1, Void >*>(static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)));
															#line 1352 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 370 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (actualBeginOffset >= actualEndOffset && begin.getKey() >= end.getKey())
															#line 1424 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 371 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			TEST(true);
															#line 372 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(false, false)); this->~SpecialKeySpace_GetRangeAggregationActorActorState(); static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->destroy(); return 0; }
															#line 1430 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(false, false));
			this->~SpecialKeySpace_GetRangeAggregationActorActorState();
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 375 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (begin.getKey() == moduleBoundary.end || end.getKey() == moduleBoundary.begin)
															#line 1438 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 376 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			TEST(true);
															#line 377 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~SpecialKeySpace_GetRangeAggregationActorActorState(); static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->destroy(); return 0; }
															#line 1444 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
			this->~SpecialKeySpace_GetRangeAggregationActorActorState();
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 379 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ranges = sks->getReadImpls().intersectingRanges(KeyRangeRef(begin.getKey(), end.getKey()));
															#line 383 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		iter = reverse ? ranges.end() : ranges.begin();
															#line 384 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (reverse)
															#line 1456 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 385 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			;
															#line 1460 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = a_body1cont8loopHead1(loopDepth);
		}
		else
		{
															#line 417 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			iter = ranges.begin();
															#line 1467 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = a_body1cont8loopHead2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont8(Void && _,int loopDepth) 
	{
															#line 370 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (actualBeginOffset >= actualEndOffset && begin.getKey() >= end.getKey())
															#line 1477 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 371 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			TEST(true);
															#line 372 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(false, false)); this->~SpecialKeySpace_GetRangeAggregationActorActorState(); static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->destroy(); return 0; }
															#line 1483 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(false, false));
			this->~SpecialKeySpace_GetRangeAggregationActorActorState();
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 375 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (begin.getKey() == moduleBoundary.end || end.getKey() == moduleBoundary.begin)
															#line 1491 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 376 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			TEST(true);
															#line 377 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~SpecialKeySpace_GetRangeAggregationActorActorState(); static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->destroy(); return 0; }
															#line 1497 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
			this->~SpecialKeySpace_GetRangeAggregationActorActorState();
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 379 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ranges = sks->getReadImpls().intersectingRanges(KeyRangeRef(begin.getKey(), end.getKey()));
															#line 383 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		iter = reverse ? ranges.end() : ranges.begin();
															#line 384 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (reverse)
															#line 1509 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 385 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			;
															#line 1513 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = a_body1cont8loopHead1(loopDepth);
		}
		else
		{
															#line 417 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			iter = ranges.begin();
															#line 1520 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 448 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~SpecialKeySpace_GetRangeAggregationActorActorState(); static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->destroy(); return 0; }
															#line 1593 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
		this->~SpecialKeySpace_GetRangeAggregationActorActorState();
		static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont12(int loopDepth) 
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
															#line 385 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!(iter != ranges.begin()))
															#line 1618 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
			return a_body1cont8break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 386 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		--iter;
															#line 387 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (iter->value() == nullptr)
															#line 1626 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
			return a_body1cont8loopHead1(loopDepth); // continue
		}
															#line 389 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		KeyRangeRef kr = iter->range();
															#line 390 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		KeyRef keyStart = kr.contains(begin.getKey()) ? begin.getKey() : kr.begin;
															#line 391 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		KeyRef keyEnd = kr.contains(end.getKey()) ? end.getKey() : kr.end;
															#line 392 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (iter->value()->isAsync() && cache.rangeContaining(keyStart).value().present())
															#line 1638 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 393 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			const SpecialKeyRangeAsyncImpl* ptr = dynamic_cast<const SpecialKeyRangeAsyncImpl*>(iter->value());
															#line 394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_2 = ptr->getRange(ryw, KeyRangeRef(keyStart, keyEnd), limits, &cache);
															#line 394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1646 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont8loopBody1when1(__when_expr_2.get(), loopDepth); };
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 3;
															#line 394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 2, RangeResult >*>(static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)));
															#line 1651 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 397 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_3 = iter->value()->getRange(ryw, KeyRangeRef(keyStart, keyEnd), limits);
															#line 397 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1660 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1cont8loopBody1when2(__when_expr_3.get(), loopDepth); };
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 4;
															#line 397 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 3, RangeResult >*>(static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)));
															#line 1665 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont8break1(int loopDepth) 
	{
		try {
			return a_body1cont12(loopDepth);
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
															#line 400 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		result.arena().dependsOn(pairs.arena());
															#line 402 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for(int i = pairs.size() - 1;i >= 0;--i) {
															#line 403 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(iter->range().contains(pairs[i].key));
															#line 404 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back(result.arena(), pairs[i]);
															#line 408 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			limits.decrement(pairs[i]);
															#line 409 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (limits.isReached())
															#line 1698 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 410 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.more = true;
															#line 411 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.readToBegin = false;
															#line 412 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (!static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~SpecialKeySpace_GetRangeAggregationActorActorState(); static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->destroy(); return 0; }
															#line 1706 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 395 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 1721 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont8loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody1cont4(RangeResult && pairs_,int loopDepth) 
	{
															#line 395 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 1730 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 398 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 1802 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont8loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody1cont6(RangeResult && pairs_,int loopDepth) 
	{
															#line 398 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 1811 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
	int a_body1cont13(int loopDepth) 
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
															#line 417 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!(iter != ranges.end()))
															#line 1896 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
			return a_body1cont8break2(loopDepth==0?0:loopDepth-1); // break
		}
															#line 418 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (iter->value() == nullptr)
															#line 1902 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
			return a_body1cont8continue1(loopDepth); // continue
		}
															#line 420 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		KeyRangeRef kr = iter->range();
															#line 421 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		KeyRef keyStart = kr.contains(begin.getKey()) ? begin.getKey() : kr.begin;
															#line 422 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		KeyRef keyEnd = kr.contains(end.getKey()) ? end.getKey() : kr.end;
															#line 423 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (iter->value()->isAsync() && cache.rangeContaining(keyStart).value().present())
															#line 1914 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 424 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			const SpecialKeyRangeAsyncImpl* ptr = dynamic_cast<const SpecialKeyRangeAsyncImpl*>(iter->value());
															#line 425 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_4 = ptr->getRange(ryw, KeyRangeRef(keyStart, keyEnd), limits, &cache);
															#line 425 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1922 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1cont8loopBody2when1(__when_expr_4.get(), loopDepth); };
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 5;
															#line 425 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 4, RangeResult >*>(static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)));
															#line 1927 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 428 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_5 = iter->value()->getRange(ryw, KeyRangeRef(keyStart, keyEnd), limits);
															#line 428 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1936 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1cont8loopBody2when2(__when_expr_5.get(), loopDepth); };
			static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->actor_wait_state = 6;
															#line 428 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 5, RangeResult >*>(static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)));
															#line 1941 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont8break2(int loopDepth) 
	{
		try {
			return a_body1cont13(loopDepth);
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
															#line 417 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		++iter;
															#line 1964 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont8loopHead2(0);

		return loopDepth;
	}
	int a_body1cont8loopBody2cont1(int loopDepth) 
	{
															#line 431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		result.arena().dependsOn(pairs.arena());
															#line 433 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for(int i = 0;i < pairs.size();++i) {
															#line 434 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(iter->range().contains(pairs[i].key));
															#line 435 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back(result.arena(), pairs[i]);
															#line 439 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			limits.decrement(pairs[i]);
															#line 440 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (limits.isReached())
															#line 1983 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 441 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.more = true;
															#line 442 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.readThroughEnd = false;
															#line 443 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (!static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~SpecialKeySpace_GetRangeAggregationActorActorState(); static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->destroy(); return 0; }
															#line 1991 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				new (&static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
				this->~SpecialKeySpace_GetRangeAggregationActorActorState();
				static_cast<SpecialKeySpace_GetRangeAggregationActorActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 417 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		++iter;
															#line 2000 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont8loopHead2(0);

		return loopDepth;
	}
	int a_body1cont8loopBody2cont4(RangeResult const& pairs_,int loopDepth) 
	{
															#line 426 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 2009 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont8loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody2cont4(RangeResult && pairs_,int loopDepth) 
	{
															#line 426 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 2018 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 429 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 2090 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont8loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8loopBody2cont6(RangeResult && pairs_,int loopDepth) 
	{
															#line 429 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		pairs = pairs_;
															#line 2099 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace* sks;
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector begin;
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector end;
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetRangeLimits limits;
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Reverse reverse;
															#line 337 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult result;
															#line 338 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult pairs;
															#line 339 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, SpecialKeyRangeReadImpl*, KeyRangeRef>::iterator iter;
															#line 340 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	int actualBeginOffset;
															#line 341 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	int actualEndOffset;
															#line 342 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef moduleBoundary;
															#line 345 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeMap<Optional<RangeResult>> cache;
															#line 379 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, SpecialKeyRangeReadImpl*, KeyRangeRef>::Ranges ranges;
															#line 2195 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getRangeAggregationActor()
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class SpecialKeySpace_GetRangeAggregationActorActor final : public Actor<RangeResult>, public ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 0, Void >, public ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 1, Void >, public ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 2, RangeResult >, public ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 3, RangeResult >, public ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 4, RangeResult >, public ActorCallback< SpecialKeySpace_GetRangeAggregationActorActor, 5, RangeResult >, public FastAllocated<SpecialKeySpace_GetRangeAggregationActorActor>, public SpecialKeySpace_GetRangeAggregationActorActorState<SpecialKeySpace_GetRangeAggregationActorActor> {
															#line 2200 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace_GetRangeAggregationActorActor(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Reverse const& reverse) 
															#line 2216 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> SpecialKeySpace::getRangeAggregationActor( SpecialKeySpace* const& sks, ReadYourWritesTransaction* const& ryw, KeySelector const& begin, KeySelector const& end, GetRangeLimits const& limits, Reverse const& reverse ) {
															#line 329 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new SpecialKeySpace_GetRangeAggregationActorActor(sks, ryw, begin, end, limits, reverse));
															#line 2248 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 450 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

Future<RangeResult> SpecialKeySpace::getRange(ReadYourWritesTransaction* ryw,
                                              KeySelector begin,
                                              KeySelector end,
                                              GetRangeLimits limits,
                                              Reverse reverse) {
	// validate limits here
	if (!limits.isValid())
		return range_limits_invalid();
	if (limits.isReached()) {
		TEST(true); // read limit 0
		return RangeResult();
	}
	// make sure orEqual == false
	begin.removeOrEqual(begin.arena());
	end.removeOrEqual(end.arena());

	if (begin.offset >= end.offset && begin.getKey() >= end.getKey()) {
		TEST(true); // range inverted
		return RangeResult();
	}

	return checkRYWValid(this, ryw, begin, end, limits, reverse);
}

															#line 2277 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
// This generated class is to be used only via getActor()
															#line 475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class SpecialKeySpace_GetActorActor>
															#line 475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class SpecialKeySpace_GetActorActorState {
															#line 2283 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace_GetActorActorState(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeyRef const& key) 
															#line 475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : sks(sks),
															#line 475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   ryw(ryw),
															#line 475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   key(key)
															#line 2294 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 479 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = sks->getRange(ryw, KeySelector(firstGreaterOrEqual(key)), KeySelector(firstGreaterOrEqual(keyAfter(key))), GetRangeLimits(CLIENT_KNOBS->TOO_MANY), Reverse::False);
															#line 479 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<SpecialKeySpace_GetActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SpecialKeySpace_GetActorActor*>(this)->actor_wait_state = 1;
															#line 479 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SpecialKeySpace_GetActorActor, 0, RangeResult >*>(static_cast<SpecialKeySpace_GetActorActor*>(this)));
															#line 2316 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 484 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(result.size() <= 1);
															#line 485 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (result.size())
															#line 2341 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 486 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetActorActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>(result[0].value)); this->~SpecialKeySpace_GetActorActorState(); static_cast<SpecialKeySpace_GetActorActor*>(this)->destroy(); return 0; }
															#line 2345 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<SpecialKeySpace_GetActorActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>(result[0].value));
			this->~SpecialKeySpace_GetActorActorState();
			static_cast<SpecialKeySpace_GetActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 488 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetActorActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>()); this->~SpecialKeySpace_GetActorActorState(); static_cast<SpecialKeySpace_GetActorActor*>(this)->destroy(); return 0; }
															#line 2355 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<SpecialKeySpace_GetActorActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>());
			this->~SpecialKeySpace_GetActorActorState();
			static_cast<SpecialKeySpace_GetActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1cont1(RangeResult && result,int loopDepth) 
	{
															#line 484 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(result.size() <= 1);
															#line 485 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (result.size())
															#line 2370 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 486 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetActorActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>(result[0].value)); this->~SpecialKeySpace_GetActorActorState(); static_cast<SpecialKeySpace_GetActorActor*>(this)->destroy(); return 0; }
															#line 2374 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<SpecialKeySpace_GetActorActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>(result[0].value));
			this->~SpecialKeySpace_GetActorActorState();
			static_cast<SpecialKeySpace_GetActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 488 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<SpecialKeySpace_GetActorActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>()); this->~SpecialKeySpace_GetActorActorState(); static_cast<SpecialKeySpace_GetActorActor*>(this)->destroy(); return 0; }
															#line 2384 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace* sks;
															#line 475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef key;
															#line 2462 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getActor()
															#line 475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class SpecialKeySpace_GetActorActor final : public Actor<Optional<Value>>, public ActorCallback< SpecialKeySpace_GetActorActor, 0, RangeResult >, public FastAllocated<SpecialKeySpace_GetActorActor>, public SpecialKeySpace_GetActorActorState<SpecialKeySpace_GetActorActor> {
															#line 2467 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<SpecialKeySpace_GetActorActor>::operator new;
	using FastAllocated<SpecialKeySpace_GetActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<Value>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SpecialKeySpace_GetActorActor, 0, RangeResult >;
															#line 475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace_GetActorActor(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw,KeyRef const& key) 
															#line 2478 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Optional<Value>> SpecialKeySpace::getActor( SpecialKeySpace* const& sks, ReadYourWritesTransaction* const& ryw, KeyRef const& key ) {
															#line 475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<Value>>(new SpecialKeySpace_GetActorActor(sks, ryw, key));
															#line 2505 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 491 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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
	return begin->clear(ryw, range);
}

void SpecialKeySpace::clear(ReadYourWritesTransaction* ryw, const KeyRef& key) {
	if (!ryw->specialKeySpaceChangeConfiguration())
		throw special_keys_write_disabled();
	auto impl = writeImpls[key];
	if (impl == nullptr)
		throw special_keys_no_write_module_found();
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
	else if (key.endsWith(LiteralStringRef("\x00")))
		key = key.removeSuffix(LiteralStringRef("\x00"));
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

															#line 2617 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via commitActor()
															#line 599 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class CommitActorActor>
															#line 599 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class CommitActorActorState {
															#line 2624 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 599 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	CommitActorActorState(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw) 
															#line 599 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 599 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : sks(sks),
															#line 599 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   ryw(ryw),
															#line 600 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   ranges(ryw->getSpecialKeySpaceWriteMap().containedRanges(specialKeys)),
															#line 602 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   iter(ranges.begin()),
															#line 603 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   writeModulePtrs()
															#line 2639 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 604 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			std::unordered_set<SpecialKeyRangeRWImpl*> deduplicate;
															#line 605 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for(;iter != ranges.end();) {
															#line 606 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				std::pair<bool, Optional<Value>> entry = iter->value();
															#line 607 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (entry.first)
															#line 2660 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 608 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					auto modulePtr = sks->getRWImpls().rangeContaining(iter->begin())->value();
															#line 609 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					auto [_, inserted] = deduplicate.insert(modulePtr);
															#line 610 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (inserted)
															#line 2668 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 611 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						writeModulePtrs.push_back(modulePtr);
															#line 2672 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
															#line 614 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				++iter;
															#line 2677 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 616 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			it = std::vector<SpecialKeyRangeRWImpl*>::const_iterator();
															#line 617 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			it = writeModulePtrs.begin();
															#line 2683 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 627 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<CommitActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CommitActorActorState(); static_cast<CommitActorActor*>(this)->destroy(); return 0; }
															#line 2706 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 617 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!(it != writeModulePtrs.end()))
															#line 2725 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 618 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Optional<std::string>> __when_expr_0 = (*it)->commit(ryw);
															#line 618 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<CommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2733 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<CommitActorActor*>(this)->actor_wait_state = 1;
															#line 618 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CommitActorActor, 0, Optional<std::string> >*>(static_cast<CommitActorActor*>(this)));
															#line 2738 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 619 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (msg.present())
															#line 2760 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 620 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg(msg.get());
															#line 621 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			TraceEvent(SevDebug, "SpecialKeySpaceManagementAPIError") .detail("Reason", msg.get()) .detail("Range", (*it)->getKeyRange().toString());
															#line 624 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), std::max(0, loopDepth - 1));
															#line 2768 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 617 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		++it;
															#line 2772 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Optional<std::string> && msg,int loopDepth) 
	{
															#line 619 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (msg.present())
															#line 2781 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 620 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg(msg.get());
															#line 621 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			TraceEvent(SevDebug, "SpecialKeySpaceManagementAPIError") .detail("Reason", msg.get()) .detail("Range", (*it)->getKeyRange().toString());
															#line 624 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), std::max(0, loopDepth - 1));
															#line 2789 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 617 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		++it;
															#line 2793 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 599 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	SpecialKeySpace* sks;
															#line 599 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 600 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, std::pair<bool, Optional<Value>>, KeyRangeRef>::Ranges ranges;
															#line 602 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, std::pair<bool, Optional<Value>>, KeyRangeRef>::iterator iter;
															#line 603 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<SpecialKeyRangeRWImpl*> writeModulePtrs;
															#line 616 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<SpecialKeyRangeRWImpl*>::const_iterator it;
															#line 2873 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via commitActor()
															#line 599 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class CommitActorActor final : public Actor<Void>, public ActorCallback< CommitActorActor, 0, Optional<std::string> >, public FastAllocated<CommitActorActor>, public CommitActorActorState<CommitActorActor> {
															#line 2878 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<CommitActorActor>::operator new;
	using FastAllocated<CommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CommitActorActor, 0, Optional<std::string> >;
															#line 599 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	CommitActorActor(SpecialKeySpace* const& sks,ReadYourWritesTransaction* const& ryw) 
															#line 2889 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 599 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Void> commitActor( SpecialKeySpace* const& sks, ReadYourWritesTransaction* const& ryw ) {
															#line 599 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Void>(new CommitActorActor(sks, ryw));
															#line 2917 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 629 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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

															#line 2962 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via getReadConflictRangeImpl()
															#line 670 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class GetReadConflictRangeImplActor>
															#line 670 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetReadConflictRangeImplActorState {
															#line 2969 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 670 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetReadConflictRangeImplActorState(ReadYourWritesTransaction* const& ryw,KeyRange const& kr) 
															#line 670 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 670 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 670 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 2978 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 671 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Void> __when_expr_0 = ryw->pendingReads();
															#line 671 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GetReadConflictRangeImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2995 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetReadConflictRangeImplActor*>(this)->actor_wait_state = 1;
															#line 671 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetReadConflictRangeImplActor, 0, Void >*>(static_cast<GetReadConflictRangeImplActor*>(this)));
															#line 3000 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 672 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetReadConflictRangeImplActor*>(this)->SAV<RangeResult>::futures) { (void)(ryw->getReadConflictRangeIntersecting(kr)); this->~GetReadConflictRangeImplActorState(); static_cast<GetReadConflictRangeImplActor*>(this)->destroy(); return 0; }
															#line 3023 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetReadConflictRangeImplActor*>(this)->SAV< RangeResult >::value()) RangeResult(ryw->getReadConflictRangeIntersecting(kr));
		this->~GetReadConflictRangeImplActorState();
		static_cast<GetReadConflictRangeImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 672 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetReadConflictRangeImplActor*>(this)->SAV<RangeResult>::futures) { (void)(ryw->getReadConflictRangeIntersecting(kr)); this->~GetReadConflictRangeImplActorState(); static_cast<GetReadConflictRangeImplActor*>(this)->destroy(); return 0; }
															#line 3035 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 670 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 670 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRange kr;
															#line 3110 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getReadConflictRangeImpl()
															#line 670 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetReadConflictRangeImplActor final : public Actor<RangeResult>, public ActorCallback< GetReadConflictRangeImplActor, 0, Void >, public FastAllocated<GetReadConflictRangeImplActor>, public GetReadConflictRangeImplActorState<GetReadConflictRangeImplActor> {
															#line 3115 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GetReadConflictRangeImplActor>::operator new;
	using FastAllocated<GetReadConflictRangeImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetReadConflictRangeImplActor, 0, Void >;
															#line 670 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetReadConflictRangeImplActor(ReadYourWritesTransaction* const& ryw,KeyRange const& kr) 
															#line 3126 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 670 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<RangeResult> getReadConflictRangeImpl( ReadYourWritesTransaction* const& ryw, KeyRange const& kr ) {
															#line 670 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new GetReadConflictRangeImplActor(ryw, kr));
															#line 3154 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 674 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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

															#line 3195 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via ddMetricsGetRangeActor()
															#line 711 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class DdMetricsGetRangeActorActor>
															#line 711 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class DdMetricsGetRangeActorActorState {
															#line 3202 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 711 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	DdMetricsGetRangeActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 711 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 711 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 711 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 3211 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 712 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			;
															#line 3226 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 714 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			auto keys = kr.removePrefix(ddStatsRange.begin);
															#line 715 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Standalone<VectorRef<DDMetricsRef>>> __when_expr_0 = waitDataDistributionMetricsList(ryw->getDatabase(), keys, CLIENT_KNOBS->STORAGE_METRICS_SHARD_LIMIT);
															#line 715 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<DdMetricsGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3261 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DdMetricsGetRangeActorActor*>(this)->actor_wait_state = 1;
															#line 715 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DdMetricsGetRangeActorActor, 0, Standalone<VectorRef<DDMetricsRef>> >*>(static_cast<DdMetricsGetRangeActorActor*>(this)));
															#line 3266 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 731 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			err = Error(e);
															#line 732 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (e.code() == error_code_dd_not_found)
															#line 3284 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 733 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				TraceEvent(SevWarnAlways, "DataDistributorNotPresent") .detail("Operation", "DDMetricsReqestThroughSpecialKeys");
															#line 735 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				StrictFuture<Void> __when_expr_1 = delayJittered(FLOW_KNOBS->PREVENT_FAST_SPIN_DELAY);
															#line 735 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (static_cast<DdMetricsGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3292 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
				static_cast<DdMetricsGetRangeActorActor*>(this)->actor_wait_state = 2;
															#line 735 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DdMetricsGetRangeActorActor, 1, Void >*>(static_cast<DdMetricsGetRangeActorActor*>(this)));
															#line 3297 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 717 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 718 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( const auto& ddMetricsRef : resultWithoutPrefix ) {
															#line 720 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			KeyRef beginKey = ddMetricsRef.beginKey.withPrefix(ddStatsRange.begin, result.arena());
															#line 722 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			json_spirit::mObject statsObj;
															#line 723 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			statsObj["shard_bytes"] = ddMetricsRef.shardBytes;
															#line 724 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			std::string statsString = json_spirit::write_string(json_spirit::mValue(statsObj), json_spirit::Output_options::raw_utf8);
															#line 726 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ValueRef bytes(result.arena(), statsString);
															#line 727 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back(result.arena(), KeyValueRef(beginKey, bytes));
															#line 3331 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 729 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<DdMetricsGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~DdMetricsGetRangeActorActorState(); static_cast<DdMetricsGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 3335 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<DdMetricsGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~DdMetricsGetRangeActorActorState();
		static_cast<DdMetricsGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Standalone<VectorRef<DDMetricsRef>> && resultWithoutPrefix,int loopDepth) 
	{
															#line 717 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 718 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( const auto& ddMetricsRef : resultWithoutPrefix ) {
															#line 720 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			KeyRef beginKey = ddMetricsRef.beginKey.withPrefix(ddStatsRange.begin, result.arena());
															#line 722 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			json_spirit::mObject statsObj;
															#line 723 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			statsObj["shard_bytes"] = ddMetricsRef.shardBytes;
															#line 724 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			std::string statsString = json_spirit::write_string(json_spirit::mValue(statsObj), json_spirit::Output_options::raw_utf8);
															#line 726 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ValueRef bytes(result.arena(), statsString);
															#line 727 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back(result.arena(), KeyValueRef(beginKey, bytes));
															#line 3361 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 729 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<DdMetricsGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~DdMetricsGetRangeActorActorState(); static_cast<DdMetricsGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 3365 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 738 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		return a_body1Catch1(err, std::max(0, loopDepth - 1));
															#line 3440 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"

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
															#line 711 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 711 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 731 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Error err;
															#line 3525 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via ddMetricsGetRangeActor()
															#line 711 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class DdMetricsGetRangeActorActor final : public Actor<RangeResult>, public ActorCallback< DdMetricsGetRangeActorActor, 0, Standalone<VectorRef<DDMetricsRef>> >, public ActorCallback< DdMetricsGetRangeActorActor, 1, Void >, public FastAllocated<DdMetricsGetRangeActorActor>, public DdMetricsGetRangeActorActorState<DdMetricsGetRangeActorActor> {
															#line 3530 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<DdMetricsGetRangeActorActor>::operator new;
	using FastAllocated<DdMetricsGetRangeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DdMetricsGetRangeActorActor, 0, Standalone<VectorRef<DDMetricsRef>> >;
friend struct ActorCallback< DdMetricsGetRangeActorActor, 1, Void >;
															#line 711 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	DdMetricsGetRangeActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 3542 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 711 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> ddMetricsGetRangeActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 711 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new DdMetricsGetRangeActorActor(ryw, kr));
															#line 3571 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 742 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

DDStatsRangeImpl::DDStatsRangeImpl(KeyRangeRef kr) : SpecialKeyRangeAsyncImpl(kr) {}

Future<RangeResult> DDStatsRangeImpl::getRange(ReadYourWritesTransaction* ryw,
                                               KeyRangeRef kr,
                                               GetRangeLimits limitsHint) const {
	return ddMetricsGetRangeActor(ryw, kr);
}

Key SpecialKeySpace::getManagementApiCommandOptionSpecialKey(const std::string& command, const std::string& option) {
	Key prefix = LiteralStringRef("options/").withPrefix(moduleToBoundary[MODULE::MANAGEMENT].begin);
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
															#line 3688 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via rwModuleWithMappingGetRangeActor()
															#line 855 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class RwModuleWithMappingGetRangeActorActor>
															#line 855 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class RwModuleWithMappingGetRangeActorActorState {
															#line 3695 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 855 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RwModuleWithMappingGetRangeActorActorState(ReadYourWritesTransaction* const& ryw,const SpecialKeyRangeRWImpl* const& impl,KeyRangeRef const& kr) 
															#line 855 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 855 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 855 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   impl(impl),
															#line 855 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 3706 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 858 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = ryw->getTransaction().getRange(ryw->getDatabase()->specialKeySpace->decode(kr), CLIENT_KNOBS->TOO_MANY);
															#line 858 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3723 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->actor_wait_state = 1;
															#line 858 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RwModuleWithMappingGetRangeActorActor, 0, RangeResult >*>(static_cast<RwModuleWithMappingGetRangeActorActor*>(this)));
															#line 3728 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 860 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(!resultWithoutPrefix.more && resultWithoutPrefix.size() < CLIENT_KNOBS->TOO_MANY);
															#line 861 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 862 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( const KeyValueRef& kv : resultWithoutPrefix ) {
															#line 863 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(impl->encode(kv.key), kv.value));
															#line 3757 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 864 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~RwModuleWithMappingGetRangeActorActorState(); static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 3761 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(rywGetRange(ryw, kr, result));
		this->~RwModuleWithMappingGetRangeActorActorState();
		static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && resultWithoutPrefix,int loopDepth) 
	{
															#line 860 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(!resultWithoutPrefix.more && resultWithoutPrefix.size() < CLIENT_KNOBS->TOO_MANY);
															#line 861 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 862 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( const KeyValueRef& kv : resultWithoutPrefix ) {
															#line 863 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(impl->encode(kv.key), kv.value));
															#line 3779 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 864 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~RwModuleWithMappingGetRangeActorActorState(); static_cast<RwModuleWithMappingGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 3783 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 855 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 855 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	const SpecialKeyRangeRWImpl* impl;
															#line 855 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 3860 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via rwModuleWithMappingGetRangeActor()
															#line 855 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class RwModuleWithMappingGetRangeActorActor final : public Actor<RangeResult>, public ActorCallback< RwModuleWithMappingGetRangeActorActor, 0, RangeResult >, public FastAllocated<RwModuleWithMappingGetRangeActorActor>, public RwModuleWithMappingGetRangeActorActorState<RwModuleWithMappingGetRangeActorActor> {
															#line 3865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<RwModuleWithMappingGetRangeActorActor>::operator new;
	using FastAllocated<RwModuleWithMappingGetRangeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RwModuleWithMappingGetRangeActorActor, 0, RangeResult >;
															#line 855 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RwModuleWithMappingGetRangeActorActor(ReadYourWritesTransaction* const& ryw,const SpecialKeyRangeRWImpl* const& impl,KeyRangeRef const& kr) 
															#line 3876 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 855 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> rwModuleWithMappingGetRangeActor( ReadYourWritesTransaction* const& ryw, const SpecialKeyRangeRWImpl* const& impl, KeyRangeRef const& kr ) {
															#line 855 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new RwModuleWithMappingGetRangeActorActor(ryw, impl, kr));
															#line 3904 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 866 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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
	    .withPrefix(LiteralStringRef("\xff/conf/"));
}

Key ExcludeServersRangeImpl::encode(const KeyRef& key) const {
	return key.removePrefix(LiteralStringRef("\xff/conf/"))
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

															#line 3968 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via checkExclusion()
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class CheckExclusionActor>
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class CheckExclusionActorState {
															#line 3975 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	CheckExclusionActorState(Database const& db,std::vector<AddressExclusion>* const& addresses,std::set<AddressExclusion>* const& exclusions,bool const& markFailed,Optional<std::string>* const& msg) 
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : db(db),
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   addresses(addresses),
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   exclusions(exclusions),
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   markFailed(markFailed),
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   msg(msg)
															#line 3990 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 932 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (markFailed)
															#line 4005 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 933 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				safe = bool();
															#line 4009 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				try {
															#line 935 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					StrictFuture<bool> __when_expr_0 = checkSafeExclusions(db, *addresses);
															#line 935 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (static_cast<CheckExclusionActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 4015 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<CheckExclusionActor*>(this)->actor_wait_state = 1;
															#line 935 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckExclusionActor, 0, bool >*>(static_cast<CheckExclusionActor*>(this)));
															#line 4020 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 954 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<StatusObject> __when_expr_1 = StatusClient::statusFetcher(db);
															#line 954 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<CheckExclusionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4056 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CheckExclusionActor*>(this)->actor_wait_state = 2;
															#line 954 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckExclusionActor, 1, StatusObject >*>(static_cast<CheckExclusionActor*>(this)));
															#line 4061 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
															#line 943 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!safe)
															#line 4070 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 944 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			std::string temp = "ERROR: It is unsafe to exclude the specified servers at this time.\n" "Please check that this exclusion does not bring down an entire storage team.\n" "Please also ensure that the exclusion will keep a majority of coordinators alive.\n" "You may add more storage processes or coordinators to make the operation safe.\n" "Call set(\"0xff0xff/management/failed/<ADDRESS...>\", ...) to exclude without " "performing safety checks.\n";
															#line 950 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", temp);
															#line 951 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4078 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 938 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 4093 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 939 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 4097 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 940 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			TraceEvent("CheckSafeExclusionsError").error(e);
															#line 941 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			safe = false;
															#line 4103 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 936 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		safe = _safe;
															#line 4118 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(bool && _safe,int loopDepth) 
	{
															#line 936 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		safe = _safe;
															#line 4127 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 955 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		errorString = "ERROR: Could not calculate the impact of this exclude on the total free space in the cluster.\n" "Please try the exclude again in 30 seconds.\n" "Call set(\"0xff0xff/management/options/exclude/force\", ...) first to exclude without checking free " "space.\n";
															#line 961 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StatusObjectReader statusObj(status);
															#line 963 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StatusObjectReader statusObjCluster;
															#line 964 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!statusObj.get("cluster", statusObjCluster))
															#line 4218 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 965 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 966 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4224 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
			this->~CheckExclusionActorState();
			static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 969 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StatusObjectReader processesMap;
															#line 970 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!statusObjCluster.get("processes", processesMap))
															#line 4234 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 971 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 972 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4240 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
			this->~CheckExclusionActorState();
			static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 975 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ssTotalCount = 0;
															#line 976 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ssExcludedCount = 0;
															#line 978 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		diskLocalities = std::unordered_set<std::string>();
															#line 979 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreFreeBytes = 0;
															#line 980 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreUsedBytes = 0;
															#line 981 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreUsedBytesNonExcluded = 0;
															#line 983 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		excludedAddressesContainsStorageRole = false;
															#line 4260 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		try {
															#line 986 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( auto proc : processesMap.obj() ) {
															#line 987 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				StatusObjectReader process(proc.second);
															#line 988 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				std::string addrStr;
															#line 989 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (!process.get("address", addrStr))
															#line 4270 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 990 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 991 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4276 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
					this->~CheckExclusionActorState();
					static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 993 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				NetworkAddress addr = NetworkAddress::parse(addrStr);
															#line 994 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				bool includedInExclusion = addressExcluded(*exclusions, addr);
															#line 995 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				bool excluded = (process.has("excluded") && process.last().get_bool()) || includedInExclusion;
															#line 997 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				StatusObjectReader localityObj;
															#line 998 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				std::string disk_id;
															#line 999 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (process.get("locality", localityObj))
															#line 4294 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1000 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					process.get("disk_id", disk_id);
															#line 4298 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
															#line 1003 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				StatusArray rolesArray = proc.second.get_obj()["roles"].get_array();
															#line 1004 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				for( StatusObjectReader role : rolesArray ) {
															#line 1005 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (role["role"].get_str() == "storage")
															#line 4306 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 1006 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						ssTotalCount++;
															#line 1010 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						if (!excludedAddressesContainsStorageRole && includedInExclusion)
															#line 4312 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1011 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							excludedAddressesContainsStorageRole = true;
															#line 4316 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
															#line 1014 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						int64_t used_bytes;
															#line 1015 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						if (!role.get("kvstore_used_bytes", used_bytes))
															#line 4322 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1016 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							*msg = ManagementAPIError::toJsonString( false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 1018 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4328 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
							new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
							this->~CheckExclusionActorState();
							static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
							return 0;
						}
															#line 1021 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						int64_t free_bytes;
															#line 1022 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						if (!role.get("kvstore_free_bytes", free_bytes))
															#line 4338 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1023 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							*msg = ManagementAPIError::toJsonString( false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 1025 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4344 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
							new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
							this->~CheckExclusionActorState();
							static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
							return 0;
						}
															#line 1028 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						totalKvStoreUsedBytes += used_bytes;
															#line 1030 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						if (!excluded)
															#line 4354 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1031 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							totalKvStoreUsedBytesNonExcluded += used_bytes;
															#line 1033 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							if (disk_id.empty() || diskLocalities.find(disk_id) == diskLocalities.end())
															#line 4360 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
							{
															#line 1034 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
								totalKvStoreFreeBytes += free_bytes;
															#line 1035 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
								if (!disk_id.empty())
															#line 4366 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
								{
															#line 1036 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
									diskLocalities.insert(disk_id);
															#line 4370 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
								}
							}
						}
					}
															#line 1042 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (excluded)
															#line 4377 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 1043 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						ssExcludedCount++;
															#line 4381 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
			}
			loopDepth = a_body1cont25(loopDepth);
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
															#line 955 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		errorString = "ERROR: Could not calculate the impact of this exclude on the total free space in the cluster.\n" "Please try the exclude again in 30 seconds.\n" "Call set(\"0xff0xff/management/options/exclude/force\", ...) first to exclude without checking free " "space.\n";
															#line 961 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StatusObjectReader statusObj(status);
															#line 963 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StatusObjectReader statusObjCluster;
															#line 964 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!statusObj.get("cluster", statusObjCluster))
															#line 4405 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 965 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 966 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4411 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
			this->~CheckExclusionActorState();
			static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 969 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StatusObjectReader processesMap;
															#line 970 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!statusObjCluster.get("processes", processesMap))
															#line 4421 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 971 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 972 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4427 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
			this->~CheckExclusionActorState();
			static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 975 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ssTotalCount = 0;
															#line 976 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ssExcludedCount = 0;
															#line 978 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		diskLocalities = std::unordered_set<std::string>();
															#line 979 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreFreeBytes = 0;
															#line 980 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreUsedBytes = 0;
															#line 981 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		totalKvStoreUsedBytesNonExcluded = 0;
															#line 983 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		excludedAddressesContainsStorageRole = false;
															#line 4447 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		try {
															#line 986 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( auto proc : processesMap.obj() ) {
															#line 987 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				StatusObjectReader process(proc.second);
															#line 988 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				std::string addrStr;
															#line 989 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (!process.get("address", addrStr))
															#line 4457 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 990 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 991 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4463 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
					this->~CheckExclusionActorState();
					static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 993 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				NetworkAddress addr = NetworkAddress::parse(addrStr);
															#line 994 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				bool includedInExclusion = addressExcluded(*exclusions, addr);
															#line 995 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				bool excluded = (process.has("excluded") && process.last().get_bool()) || includedInExclusion;
															#line 997 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				StatusObjectReader localityObj;
															#line 998 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				std::string disk_id;
															#line 999 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (process.get("locality", localityObj))
															#line 4481 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1000 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					process.get("disk_id", disk_id);
															#line 4485 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
															#line 1003 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				StatusArray rolesArray = proc.second.get_obj()["roles"].get_array();
															#line 1004 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				for( StatusObjectReader role : rolesArray ) {
															#line 1005 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (role["role"].get_str() == "storage")
															#line 4493 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 1006 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						ssTotalCount++;
															#line 1010 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						if (!excludedAddressesContainsStorageRole && includedInExclusion)
															#line 4499 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1011 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							excludedAddressesContainsStorageRole = true;
															#line 4503 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
															#line 1014 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						int64_t used_bytes;
															#line 1015 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						if (!role.get("kvstore_used_bytes", used_bytes))
															#line 4509 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1016 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							*msg = ManagementAPIError::toJsonString( false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 1018 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4515 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
							new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
							this->~CheckExclusionActorState();
							static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
							return 0;
						}
															#line 1021 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						int64_t free_bytes;
															#line 1022 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						if (!role.get("kvstore_free_bytes", free_bytes))
															#line 4525 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1023 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							*msg = ManagementAPIError::toJsonString( false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 1025 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4531 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
							new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
							this->~CheckExclusionActorState();
							static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
							return 0;
						}
															#line 1028 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						totalKvStoreUsedBytes += used_bytes;
															#line 1030 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						if (!excluded)
															#line 4541 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1031 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							totalKvStoreUsedBytesNonExcluded += used_bytes;
															#line 1033 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							if (disk_id.empty() || diskLocalities.find(disk_id) == diskLocalities.end())
															#line 4547 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
							{
															#line 1034 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
								totalKvStoreFreeBytes += free_bytes;
															#line 1035 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
								if (!disk_id.empty())
															#line 4553 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
								{
															#line 1036 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
									diskLocalities.insert(disk_id);
															#line 4557 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
								}
							}
						}
					}
															#line 1042 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (excluded)
															#line 4564 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 1043 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						ssExcludedCount++;
															#line 4568 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
			}
			loopDepth = a_body1cont25(loopDepth);
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
															#line 1055 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!excludedAddressesContainsStorageRole)
															#line 4649 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1056 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(true); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4653 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(true);
			this->~CheckExclusionActorState();
			static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1059 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		double finalFreeRatio = 1 - (totalKvStoreUsedBytes / (totalKvStoreUsedBytesNonExcluded + totalKvStoreFreeBytes));
															#line 1060 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (ssExcludedCount == ssTotalCount || finalFreeRatio <= 0.1)
															#line 4663 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1061 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			std::string temp = "ERROR: This exclude may cause the total free space in the cluster to drop below 10%.\n" "Call set(\"0xff0xff/management/options/exclude/force\", ...) first to exclude without " "checking free space.\n";
															#line 1064 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", temp);
															#line 1065 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4671 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(false);
			this->~CheckExclusionActorState();
			static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1068 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(true); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4679 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<CheckExclusionActor*>(this)->SAV< bool >::value()) bool(true);
		this->~CheckExclusionActorState();
		static_cast<CheckExclusionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont8Catch1(const Error& __current_error,int loopDepth=0) 
	{
		try {
															#line 1049 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			*msg = ManagementAPIError::toJsonString(false, markFailed ? "exclude failed" : "exclude", errorString);
															#line 1050 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CheckExclusionActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckExclusionActorState(); static_cast<CheckExclusionActor*>(this)->destroy(); return 0; }
															#line 4694 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
	int a_body1cont25(int loopDepth) 
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
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Database db;
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<AddressExclusion>* addresses;
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::set<AddressExclusion>* exclusions;
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	bool markFailed;
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Optional<std::string>* msg;
															#line 933 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	bool safe;
															#line 955 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::string errorString;
															#line 975 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	int ssTotalCount;
															#line 976 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	int ssExcludedCount;
															#line 978 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::unordered_set<std::string> diskLocalities;
															#line 979 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	int64_t totalKvStoreFreeBytes;
															#line 980 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	int64_t totalKvStoreUsedBytes;
															#line 981 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	int64_t totalKvStoreUsedBytesNonExcluded;
															#line 983 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	bool excludedAddressesContainsStorageRole;
															#line 4749 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via checkExclusion()
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class CheckExclusionActor final : public Actor<bool>, public ActorCallback< CheckExclusionActor, 0, bool >, public ActorCallback< CheckExclusionActor, 1, StatusObject >, public FastAllocated<CheckExclusionActor>, public CheckExclusionActorState<CheckExclusionActor> {
															#line 4754 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<CheckExclusionActor>::operator new;
	using FastAllocated<CheckExclusionActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckExclusionActor, 0, bool >;
friend struct ActorCallback< CheckExclusionActor, 1, StatusObject >;
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	CheckExclusionActor(Database const& db,std::vector<AddressExclusion>* const& addresses,std::set<AddressExclusion>* const& exclusions,bool const& markFailed,Optional<std::string>* const& msg) 
															#line 4766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<bool> checkExclusion( Database const& db, std::vector<AddressExclusion>* const& addresses, std::set<AddressExclusion>* const& exclusions, bool const& markFailed, Optional<std::string>* const& msg ) {
															#line 926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<bool>(new CheckExclusionActor(db, addresses, exclusions, markFailed, msg));
															#line 4795 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1070 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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

															#line 4837 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via excludeCommitActor()
															#line 1108 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class ExcludeCommitActorActor>
															#line 1108 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class ExcludeCommitActorActorState {
															#line 4844 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1108 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ExcludeCommitActorActorState(ReadYourWritesTransaction* const& ryw,bool const& failed) 
															#line 1108 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1108 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1108 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   failed(failed),
															#line 1110 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   result(),
															#line 1111 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   addresses(),
															#line 1112 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   exclusions()
															#line 4859 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1113 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!parseNetWorkAddrFromKeys(ryw, failed, addresses, exclusions, result))
															#line 4874 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1114 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (!static_cast<ExcludeCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeCommitActorActorState(); static_cast<ExcludeCommitActorActor*>(this)->destroy(); return 0; }
															#line 4878 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				new (&static_cast<ExcludeCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(result)); // state_var_RVO
				this->~ExcludeCommitActorActorState();
				static_cast<ExcludeCommitActorActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1116 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			auto force = ryw->getSpecialKeySpaceWriteMap()[SpecialKeySpace::getManagementApiCommandOptionSpecialKey( failed ? "failed" : "excluded", "force")];
															#line 1119 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (addresses.size() && !(force.first && force.second.present()))
															#line 4888 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1120 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				StrictFuture<bool> __when_expr_0 = checkExclusion(ryw->getDatabase(), &addresses, &exclusions, failed, &result);
															#line 1120 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (static_cast<ExcludeCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4894 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ExcludeCommitActorActor*>(this)->actor_wait_state = 1;
															#line 1120 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExcludeCommitActorActor, 0, bool >*>(static_cast<ExcludeCommitActorActor*>(this)));
															#line 4899 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1124 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Void> __when_expr_1 = excludeServers(&(ryw->getTransaction()), addresses, failed);
															#line 1124 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ExcludeCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4929 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ExcludeCommitActorActor*>(this)->actor_wait_state = 2;
															#line 1124 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExcludeCommitActorActor, 1, Void >*>(static_cast<ExcludeCommitActorActor*>(this)));
															#line 4934 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(bool const& safe,int loopDepth) 
	{
															#line 1121 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!safe)
															#line 4943 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1122 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<ExcludeCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeCommitActorActorState(); static_cast<ExcludeCommitActorActor*>(this)->destroy(); return 0; }
															#line 4947 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1121 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!safe)
															#line 4961 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1122 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<ExcludeCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeCommitActorActorState(); static_cast<ExcludeCommitActorActor*>(this)->destroy(); return 0; }
															#line 4965 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1125 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		includeServers(ryw);
															#line 1127 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ExcludeCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeCommitActorActorState(); static_cast<ExcludeCommitActorActor*>(this)->destroy(); return 0; }
															#line 5044 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ExcludeCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(result)); // state_var_RVO
		this->~ExcludeCommitActorActorState();
		static_cast<ExcludeCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 1125 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		includeServers(ryw);
															#line 1127 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ExcludeCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeCommitActorActorState(); static_cast<ExcludeCommitActorActor*>(this)->destroy(); return 0; }
															#line 5058 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1108 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1108 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	bool failed;
															#line 1110 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Optional<std::string> result;
															#line 1111 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<AddressExclusion> addresses;
															#line 1112 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::set<AddressExclusion> exclusions;
															#line 5139 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via excludeCommitActor()
															#line 1108 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class ExcludeCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< ExcludeCommitActorActor, 0, bool >, public ActorCallback< ExcludeCommitActorActor, 1, Void >, public FastAllocated<ExcludeCommitActorActor>, public ExcludeCommitActorActorState<ExcludeCommitActorActor> {
															#line 5144 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<ExcludeCommitActorActor>::operator new;
	using FastAllocated<ExcludeCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ExcludeCommitActorActor, 0, bool >;
friend struct ActorCallback< ExcludeCommitActorActor, 1, Void >;
															#line 1108 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ExcludeCommitActorActor(ReadYourWritesTransaction* const& ryw,bool const& failed) 
															#line 5156 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1108 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Optional<std::string>> excludeCommitActor( ReadYourWritesTransaction* const& ryw, bool const& failed ) {
															#line 1108 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new ExcludeCommitActorActor(ryw, failed));
															#line 5185 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1129 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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
	    .withPrefix(LiteralStringRef("\xff/conf/"));
}

Key FailedServersRangeImpl::encode(const KeyRef& key) const {
	return key.removePrefix(LiteralStringRef("\xff/conf/"))
	    .withPrefix(SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin);
}

Future<Optional<std::string>> FailedServersRangeImpl::commit(ReadYourWritesTransaction* ryw) {
	return excludeCommitActor(ryw, true);
}

															#line 5222 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via ExclusionInProgressActor()
															#line 1162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class ExclusionInProgressActorActor>
															#line 1162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class ExclusionInProgressActorActorState {
															#line 5229 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ExclusionInProgressActorActorState(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 1162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   prefix(prefix),
															#line 1162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr),
															#line 1163 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   result(),
															#line 1164 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   tr(ryw->getTransaction())
															#line 5244 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1165 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			tr.setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1166 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1167 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1169 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<std::vector<AddressExclusion>> __when_expr_0 = (getAllExcludedServers(&tr));
															#line 1169 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5267 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state = 1;
															#line 1169 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExclusionInProgressActorActor, 0, std::vector<AddressExclusion> >*>(static_cast<ExclusionInProgressActorActor*>(this)));
															#line 5272 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1170 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		exclusions = std::set<AddressExclusion>(excl.begin(), excl.end());
															#line 1171 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		inProgressExclusion = std::set<NetworkAddress>();
															#line 1174 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr.getRange(serverListKeys, CLIENT_KNOBS->TOO_MANY);
															#line 1174 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5301 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state = 2;
															#line 1174 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExclusionInProgressActorActor, 1, RangeResult >*>(static_cast<ExclusionInProgressActorActor*>(this)));
															#line 5306 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<AddressExclusion> const& __excl,int loopDepth) 
	{
															#line 1169 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		excl = __excl;
															#line 5315 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1175 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(!serverList.more && serverList.size() < CLIENT_KNOBS->TOO_MANY);
															#line 1177 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto& s : serverList ) {
															#line 1178 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			auto addresses = decodeServerListValue(s.value).getKeyValues.getEndpoint().addresses;
															#line 1179 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (addressExcluded(exclusions, addresses.address))
															#line 5388 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1180 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				inProgressExclusion.insert(addresses.address);
															#line 5392 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 1182 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (addresses.secondaryAddress.present() && addressExcluded(exclusions, addresses.secondaryAddress.get()))
															#line 5396 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				inProgressExclusion.insert(addresses.secondaryAddress.get());
															#line 5400 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1187 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Optional<Standalone<StringRef>>> __when_expr_2 = tr.get(logsKey);
															#line 1187 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5407 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<ExclusionInProgressActorActor*>(this)->actor_wait_state = 3;
															#line 1187 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExclusionInProgressActorActor, 2, Optional<Standalone<StringRef>> >*>(static_cast<ExclusionInProgressActorActor*>(this)));
															#line 5412 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult const& __serverList,int loopDepth) 
	{
															#line 1174 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		serverList = __serverList;
															#line 5421 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1188 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(value.present());
															#line 1189 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		auto logs = decodeLogsValue(value.get());
															#line 1190 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& log : logs.first ) {
															#line 1191 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (log.second == NetworkAddress() || addressExcluded(exclusions, log.second))
															#line 5494 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1192 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				inProgressExclusion.insert(log.second);
															#line 5498 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1195 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& log : logs.second ) {
															#line 1196 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (log.second == NetworkAddress() || addressExcluded(exclusions, log.second))
															#line 5505 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1197 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				inProgressExclusion.insert(log.second);
															#line 5509 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1202 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		std::set<std::string> inProgressAddresses;
															#line 1203 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& address : inProgressExclusion ) {
															#line 1204 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			inProgressAddresses.insert(formatIpPort(address.ip, address.port));
															#line 5518 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1207 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& address : inProgressAddresses ) {
															#line 1208 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			Key addrKey = prefix.withSuffix(address);
															#line 1209 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.contains(addrKey))
															#line 5526 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1210 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back(result.arena(), KeyValueRef(addrKey, ValueRef()));
															#line 1211 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.arena().dependsOn(addrKey.arena());
															#line 5532 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1214 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ExclusionInProgressActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~ExclusionInProgressActorActorState(); static_cast<ExclusionInProgressActorActor*>(this)->destroy(); return 0; }
															#line 5537 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ExclusionInProgressActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
		this->~ExclusionInProgressActorActorState();
		static_cast<ExclusionInProgressActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Optional<Standalone<StringRef>> && value,int loopDepth) 
	{
															#line 1188 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(value.present());
															#line 1189 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		auto logs = decodeLogsValue(value.get());
															#line 1190 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& log : logs.first ) {
															#line 1191 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (log.second == NetworkAddress() || addressExcluded(exclusions, log.second))
															#line 5555 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1192 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				inProgressExclusion.insert(log.second);
															#line 5559 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1195 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& log : logs.second ) {
															#line 1196 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (log.second == NetworkAddress() || addressExcluded(exclusions, log.second))
															#line 5566 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1197 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				inProgressExclusion.insert(log.second);
															#line 5570 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1202 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		std::set<std::string> inProgressAddresses;
															#line 1203 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& address : inProgressExclusion ) {
															#line 1204 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			inProgressAddresses.insert(formatIpPort(address.ip, address.port));
															#line 5579 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1207 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto const& address : inProgressAddresses ) {
															#line 1208 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			Key addrKey = prefix.withSuffix(address);
															#line 1209 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.contains(addrKey))
															#line 5587 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1210 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back(result.arena(), KeyValueRef(addrKey, ValueRef()));
															#line 1211 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.arena().dependsOn(addrKey.arena());
															#line 5593 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1214 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ExclusionInProgressActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~ExclusionInProgressActorActorState(); static_cast<ExclusionInProgressActorActor*>(this)->destroy(); return 0; }
															#line 5598 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef prefix;
															#line 1162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 1163 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult result;
															#line 1164 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Transaction& tr;
															#line 1169 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<AddressExclusion> excl;
															#line 1170 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::set<AddressExclusion> exclusions;
															#line 1171 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::set<NetworkAddress> inProgressExclusion;
															#line 1174 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult serverList;
															#line 5687 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via ExclusionInProgressActor()
															#line 1162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class ExclusionInProgressActorActor final : public Actor<RangeResult>, public ActorCallback< ExclusionInProgressActorActor, 0, std::vector<AddressExclusion> >, public ActorCallback< ExclusionInProgressActorActor, 1, RangeResult >, public ActorCallback< ExclusionInProgressActorActor, 2, Optional<Standalone<StringRef>> >, public FastAllocated<ExclusionInProgressActorActor>, public ExclusionInProgressActorActorState<ExclusionInProgressActorActor> {
															#line 5692 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ExclusionInProgressActorActor(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 5705 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> ExclusionInProgressActor( ReadYourWritesTransaction* const& ryw, KeyRef const& prefix, KeyRangeRef const& kr ) {
															#line 1162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new ExclusionInProgressActorActor(ryw, prefix, kr));
															#line 5735 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1216 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

ExclusionInProgressRangeImpl::ExclusionInProgressRangeImpl(KeyRangeRef kr) : SpecialKeyRangeAsyncImpl(kr) {}

Future<RangeResult> ExclusionInProgressRangeImpl::getRange(ReadYourWritesTransaction* ryw,
                                                           KeyRangeRef kr,
                                                           GetRangeLimits limitsHint) const {
	return ExclusionInProgressActor(ryw, getKeyRange().begin, kr);
}

															#line 5748 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via getProcessClassActor()
															#line 1225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class GetProcessClassActorActor>
															#line 1225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetProcessClassActorActorState {
															#line 5755 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetProcessClassActorActorState(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 1225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   prefix(prefix),
															#line 1225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 5766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1226 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1227 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<std::vector<ProcessData>> __when_expr_0 = getWorkers(&ryw->getTransaction());
															#line 1227 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GetProcessClassActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5785 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetProcessClassActorActor*>(this)->actor_wait_state = 1;
															#line 1227 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetProcessClassActorActor, 0, std::vector<ProcessData> >*>(static_cast<GetProcessClassActorActor*>(this)));
															#line 5790 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1228 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		auto workers = _workers;
															#line 1230 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		std::sort(workers.begin(), workers.end(), [](const ProcessData& lhs, const ProcessData& rhs) { return formatIpPort(lhs.address.ip, lhs.address.port) < formatIpPort(rhs.address.ip, rhs.address.port); });
															#line 1233 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1234 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto& w : workers ) {
															#line 1236 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			KeyRef k(prefix.withSuffix(formatIpPort(w.address.ip, w.address.port), result.arena()));
															#line 1237 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.contains(k))
															#line 5823 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1238 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ValueRef v(result.arena(), w.processClass.toString());
															#line 1239 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back(result.arena(), KeyValueRef(k, v));
															#line 5829 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1242 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetProcessClassActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~GetProcessClassActorActorState(); static_cast<GetProcessClassActorActor*>(this)->destroy(); return 0; }
															#line 5834 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetProcessClassActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(rywGetRange(ryw, kr, result));
		this->~GetProcessClassActorActorState();
		static_cast<GetProcessClassActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<ProcessData> && _workers,int loopDepth) 
	{
															#line 1228 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		auto workers = _workers;
															#line 1230 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		std::sort(workers.begin(), workers.end(), [](const ProcessData& lhs, const ProcessData& rhs) { return formatIpPort(lhs.address.ip, lhs.address.port) < formatIpPort(rhs.address.ip, rhs.address.port); });
															#line 1233 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1234 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto& w : workers ) {
															#line 1236 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			KeyRef k(prefix.withSuffix(formatIpPort(w.address.ip, w.address.port), result.arena()));
															#line 1237 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.contains(k))
															#line 5856 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1238 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ValueRef v(result.arena(), w.processClass.toString());
															#line 1239 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back(result.arena(), KeyValueRef(k, v));
															#line 5862 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1242 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetProcessClassActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~GetProcessClassActorActorState(); static_cast<GetProcessClassActorActor*>(this)->destroy(); return 0; }
															#line 5867 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef prefix;
															#line 1225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 5944 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getProcessClassActor()
															#line 1225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetProcessClassActorActor final : public Actor<RangeResult>, public ActorCallback< GetProcessClassActorActor, 0, std::vector<ProcessData> >, public FastAllocated<GetProcessClassActorActor>, public GetProcessClassActorActorState<GetProcessClassActorActor> {
															#line 5949 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GetProcessClassActorActor>::operator new;
	using FastAllocated<GetProcessClassActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetProcessClassActorActor, 0, std::vector<ProcessData> >;
															#line 1225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetProcessClassActorActor(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 5960 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> getProcessClassActor( ReadYourWritesTransaction* const& ryw, KeyRef const& prefix, KeyRangeRef const& kr ) {
															#line 1225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new GetProcessClassActorActor(ryw, prefix, kr));
															#line 5988 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1244 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

															#line 5993 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via processClassCommitActor()
															#line 1245 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class ProcessClassCommitActorActor>
															#line 1245 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class ProcessClassCommitActorActorState {
															#line 6000 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1245 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ProcessClassCommitActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& range) 
															#line 1245 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1245 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1245 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   range(range)
															#line 6009 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1247 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1248 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1249 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 1250 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1251 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<std::vector<ProcessData>> __when_expr_0 = getWorkers(&ryw->getTransaction());
															#line 1251 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<ProcessClassCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6034 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ProcessClassCommitActorActor*>(this)->actor_wait_state = 1;
															#line 1251 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ProcessClassCommitActorActor, 0, std::vector<ProcessData> >*>(static_cast<ProcessClassCommitActorActor*>(this)));
															#line 6039 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1254 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		auto ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(range);
															#line 1255 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		auto iter = ranges.begin();
															#line 1256 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for(;iter != ranges.end();) {
															#line 1257 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			auto entry = iter->value();
															#line 1259 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (entry.first && entry.second.present())
															#line 6070 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1261 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				Key address = iter->begin().removePrefix(range.begin);
															#line 1262 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				AddressExclusion addr = AddressExclusion::parse(address);
															#line 1264 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ValueRef processClassType = entry.second.get();
															#line 1265 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ProcessClass processClass(processClassType.toString(), ProcessClass::DBSource);
															#line 1267 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				bool foundChange = false;
															#line 1268 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				for(int i = 0;i < workers.size();i++) {
															#line 1269 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (addr.excludes(workers[i].address))
															#line 6086 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 1270 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						if (processClass.classType() != ProcessClass::InvalidClass)
															#line 6090 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1271 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							ryw->getTransaction().set(processClassKeyFor(workers[i].locality.processId().get()), processClassValue(processClass));
															#line 6094 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
						else
						{
															#line 1274 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							ryw->getTransaction().clear(processClassKeyFor(workers[i].locality.processId().get()));
															#line 6100 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
															#line 1275 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						foundChange = true;
															#line 6104 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
															#line 1278 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (foundChange)
															#line 6109 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1279 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					ryw->getTransaction().set(processClassChangeKey, deterministicRandom()->randomUniqueID().toString());
															#line 6113 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
															#line 1281 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			++iter;
															#line 6118 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1283 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ProcessClassCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~ProcessClassCommitActorActorState(); static_cast<ProcessClassCommitActorActor*>(this)->destroy(); return 0; }
															#line 6122 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ProcessClassCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~ProcessClassCommitActorActorState();
		static_cast<ProcessClassCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<ProcessData> && workers,int loopDepth) 
	{
															#line 1254 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		auto ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(range);
															#line 1255 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		auto iter = ranges.begin();
															#line 1256 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for(;iter != ranges.end();) {
															#line 1257 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			auto entry = iter->value();
															#line 1259 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (entry.first && entry.second.present())
															#line 6142 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1261 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				Key address = iter->begin().removePrefix(range.begin);
															#line 1262 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				AddressExclusion addr = AddressExclusion::parse(address);
															#line 1264 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ValueRef processClassType = entry.second.get();
															#line 1265 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ProcessClass processClass(processClassType.toString(), ProcessClass::DBSource);
															#line 1267 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				bool foundChange = false;
															#line 1268 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				for(int i = 0;i < workers.size();i++) {
															#line 1269 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (addr.excludes(workers[i].address))
															#line 6158 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 1270 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						if (processClass.classType() != ProcessClass::InvalidClass)
															#line 6162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 1271 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							ryw->getTransaction().set(processClassKeyFor(workers[i].locality.processId().get()), processClassValue(processClass));
															#line 6166 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
						else
						{
															#line 1274 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							ryw->getTransaction().clear(processClassKeyFor(workers[i].locality.processId().get()));
															#line 6172 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
															#line 1275 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						foundChange = true;
															#line 6176 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
															#line 1278 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (foundChange)
															#line 6181 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1279 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					ryw->getTransaction().set(processClassChangeKey, deterministicRandom()->randomUniqueID().toString());
															#line 6185 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
															#line 1281 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			++iter;
															#line 6190 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1283 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ProcessClassCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~ProcessClassCommitActorActorState(); static_cast<ProcessClassCommitActorActor*>(this)->destroy(); return 0; }
															#line 6194 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1245 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1245 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef range;
															#line 6269 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via processClassCommitActor()
															#line 1245 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class ProcessClassCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< ProcessClassCommitActorActor, 0, std::vector<ProcessData> >, public FastAllocated<ProcessClassCommitActorActor>, public ProcessClassCommitActorActorState<ProcessClassCommitActorActor> {
															#line 6274 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<ProcessClassCommitActorActor>::operator new;
	using FastAllocated<ProcessClassCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ProcessClassCommitActorActor, 0, std::vector<ProcessData> >;
															#line 1245 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ProcessClassCommitActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& range) 
															#line 6285 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1245 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Optional<std::string>> processClassCommitActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& range ) {
															#line 1245 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new ProcessClassCommitActorActor(ryw, range));
															#line 6313 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1285 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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
			if (processClass.classType() == ProcessClass::InvalidClass &&
			    processClassType != LiteralStringRef("default")) {
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

															#line 6375 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via getProcessClassSourceActor()
															#line 1343 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class GetProcessClassSourceActorActor>
															#line 1343 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetProcessClassSourceActorActorState {
															#line 6382 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1343 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetProcessClassSourceActorActorState(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 1343 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1343 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1343 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   prefix(prefix),
															#line 1343 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 6393 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1344 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1345 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<std::vector<ProcessData>> __when_expr_0 = getWorkers(&ryw->getTransaction());
															#line 1345 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GetProcessClassSourceActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6412 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetProcessClassSourceActorActor*>(this)->actor_wait_state = 1;
															#line 1345 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetProcessClassSourceActorActor, 0, std::vector<ProcessData> >*>(static_cast<GetProcessClassSourceActorActor*>(this)));
															#line 6417 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1346 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		auto workers = _workers;
															#line 1348 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		std::sort(workers.begin(), workers.end(), [](const ProcessData& lhs, const ProcessData& rhs) { return formatIpPort(lhs.address.ip, lhs.address.port) < formatIpPort(rhs.address.ip, rhs.address.port); });
															#line 1351 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1352 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto& w : workers ) {
															#line 1354 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			Key k(prefix.withSuffix(formatIpPort(w.address.ip, w.address.port)));
															#line 1355 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.contains(k))
															#line 6450 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1356 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				Value v(w.processClass.sourceString());
															#line 1357 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back(result.arena(), KeyValueRef(k, v));
															#line 1358 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.arena().dependsOn(k.arena());
															#line 1359 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.arena().dependsOn(v.arena());
															#line 6460 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1362 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetProcessClassSourceActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetProcessClassSourceActorActorState(); static_cast<GetProcessClassSourceActorActor*>(this)->destroy(); return 0; }
															#line 6465 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetProcessClassSourceActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetProcessClassSourceActorActorState();
		static_cast<GetProcessClassSourceActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<ProcessData> && _workers,int loopDepth) 
	{
															#line 1346 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		auto workers = _workers;
															#line 1348 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		std::sort(workers.begin(), workers.end(), [](const ProcessData& lhs, const ProcessData& rhs) { return formatIpPort(lhs.address.ip, lhs.address.port) < formatIpPort(rhs.address.ip, rhs.address.port); });
															#line 1351 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1352 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto& w : workers ) {
															#line 1354 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			Key k(prefix.withSuffix(formatIpPort(w.address.ip, w.address.port)));
															#line 1355 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.contains(k))
															#line 6487 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1356 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				Value v(w.processClass.sourceString());
															#line 1357 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back(result.arena(), KeyValueRef(k, v));
															#line 1358 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.arena().dependsOn(k.arena());
															#line 1359 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.arena().dependsOn(v.arena());
															#line 6497 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1362 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetProcessClassSourceActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetProcessClassSourceActorActorState(); static_cast<GetProcessClassSourceActorActor*>(this)->destroy(); return 0; }
															#line 6502 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1343 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1343 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef prefix;
															#line 1343 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 6579 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getProcessClassSourceActor()
															#line 1343 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetProcessClassSourceActorActor final : public Actor<RangeResult>, public ActorCallback< GetProcessClassSourceActorActor, 0, std::vector<ProcessData> >, public FastAllocated<GetProcessClassSourceActorActor>, public GetProcessClassSourceActorActorState<GetProcessClassSourceActorActor> {
															#line 6584 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GetProcessClassSourceActorActor>::operator new;
	using FastAllocated<GetProcessClassSourceActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetProcessClassSourceActorActor, 0, std::vector<ProcessData> >;
															#line 1343 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetProcessClassSourceActorActor(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 6595 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1343 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> getProcessClassSourceActor( ReadYourWritesTransaction* const& ryw, KeyRef const& prefix, KeyRangeRef const& kr ) {
															#line 1343 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new GetProcessClassSourceActorActor(ryw, prefix, kr));
															#line 6623 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1364 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

ProcessClassSourceRangeImpl::ProcessClassSourceRangeImpl(KeyRangeRef kr) : SpecialKeyRangeReadImpl(kr) {}

Future<RangeResult> ProcessClassSourceRangeImpl::getRange(ReadYourWritesTransaction* ryw,
                                                          KeyRangeRef kr,
                                                          GetRangeLimits limitsHint) const {
	return getProcessClassSourceActor(ryw, getKeyRange().begin, kr);
}

															#line 6636 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via getLockedKeyActor()
															#line 1373 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class GetLockedKeyActorActor>
															#line 1373 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetLockedKeyActorActorState {
															#line 6643 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1373 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetLockedKeyActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 1373 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1373 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1373 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 6652 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1374 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1375 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1376 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(databaseLockedKey);
															#line 1376 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GetLockedKeyActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6673 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetLockedKeyActorActor*>(this)->actor_wait_state = 1;
															#line 1376 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetLockedKeyActorActor, 0, Optional<Value> >*>(static_cast<GetLockedKeyActorActor*>(this)));
															#line 6678 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1377 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1378 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 6703 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1379 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			UID uid = UID::fromString(BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()).toString());
															#line 1380 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, Value(uid.toString())));
															#line 6709 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1382 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetLockedKeyActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetLockedKeyActorActorState(); static_cast<GetLockedKeyActorActor*>(this)->destroy(); return 0; }
															#line 6713 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetLockedKeyActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetLockedKeyActorActorState();
		static_cast<GetLockedKeyActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 1377 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1378 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 6727 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1379 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			UID uid = UID::fromString(BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()).toString());
															#line 1380 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, Value(uid.toString())));
															#line 6733 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1382 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetLockedKeyActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetLockedKeyActorActorState(); static_cast<GetLockedKeyActorActor*>(this)->destroy(); return 0; }
															#line 6737 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1373 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1373 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 6812 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getLockedKeyActor()
															#line 1373 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetLockedKeyActorActor final : public Actor<RangeResult>, public ActorCallback< GetLockedKeyActorActor, 0, Optional<Value> >, public FastAllocated<GetLockedKeyActorActor>, public GetLockedKeyActorActorState<GetLockedKeyActorActor> {
															#line 6817 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GetLockedKeyActorActor>::operator new;
	using FastAllocated<GetLockedKeyActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetLockedKeyActorActor, 0, Optional<Value> >;
															#line 1373 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetLockedKeyActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 6828 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1373 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> getLockedKeyActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 1373 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new GetLockedKeyActorActor(ryw, kr));
															#line 6856 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1384 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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

															#line 6881 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via lockDatabaseCommitActor()
															#line 1405 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class LockDatabaseCommitActorActor>
															#line 1405 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class LockDatabaseCommitActorActorState {
															#line 6888 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1405 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	LockDatabaseCommitActorActorState(ReadYourWritesTransaction* const& ryw,UID const& uid) 
															#line 1405 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1405 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1405 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   uid(uid),
															#line 1406 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   msg()
															#line 6899 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1407 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1408 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1409 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(databaseLockedKey);
															#line 1409 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<LockDatabaseCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6920 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<LockDatabaseCommitActorActor*>(this)->actor_wait_state = 1;
															#line 1409 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< LockDatabaseCommitActorActor, 0, Optional<Value> >*>(static_cast<LockDatabaseCommitActorActor*>(this)));
															#line 6925 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1411 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present() && BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) != uid)
															#line 6948 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1414 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(database_locked(), loopDepth);
															#line 6952 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
		else
		{
															#line 1415 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!val.present())
															#line 6958 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1417 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ryw->getTransaction().atomicOp(databaseLockedKey, BinaryWriter::toValue(uid, Unversioned()) .withPrefix(LiteralStringRef("0123456789")) .withSuffix(LiteralStringRef("\x00\x00\x00\x00")), MutationRef::SetVersionstampedValue);
															#line 1422 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ryw->getTransaction().addWriteConflictRange(normalKeys);
															#line 6964 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1425 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<LockDatabaseCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(msg); this->~LockDatabaseCommitActorActorState(); static_cast<LockDatabaseCommitActorActor*>(this)->destroy(); return 0; }
															#line 6969 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<LockDatabaseCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(msg)); // state_var_RVO
		this->~LockDatabaseCommitActorActorState();
		static_cast<LockDatabaseCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 1411 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present() && BinaryReader::fromStringRef<UID>(val.get().substr(10), Unversioned()) != uid)
															#line 6981 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1414 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(database_locked(), loopDepth);
															#line 6985 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
		else
		{
															#line 1415 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!val.present())
															#line 6991 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1417 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ryw->getTransaction().atomicOp(databaseLockedKey, BinaryWriter::toValue(uid, Unversioned()) .withPrefix(LiteralStringRef("0123456789")) .withSuffix(LiteralStringRef("\x00\x00\x00\x00")), MutationRef::SetVersionstampedValue);
															#line 1422 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ryw->getTransaction().addWriteConflictRange(normalKeys);
															#line 6997 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1425 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<LockDatabaseCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(msg); this->~LockDatabaseCommitActorActorState(); static_cast<LockDatabaseCommitActorActor*>(this)->destroy(); return 0; }
															#line 7002 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1405 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1405 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	UID uid;
															#line 1406 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Optional<std::string> msg;
															#line 7079 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via lockDatabaseCommitActor()
															#line 1405 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class LockDatabaseCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< LockDatabaseCommitActorActor, 0, Optional<Value> >, public FastAllocated<LockDatabaseCommitActorActor>, public LockDatabaseCommitActorActorState<LockDatabaseCommitActorActor> {
															#line 7084 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<LockDatabaseCommitActorActor>::operator new;
	using FastAllocated<LockDatabaseCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< LockDatabaseCommitActorActor, 0, Optional<Value> >;
															#line 1405 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	LockDatabaseCommitActorActor(ReadYourWritesTransaction* const& ryw,UID const& uid) 
															#line 7095 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1405 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Optional<std::string>> lockDatabaseCommitActor( ReadYourWritesTransaction* const& ryw, UID const& uid ) {
															#line 1405 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new LockDatabaseCommitActorActor(ryw, uid));
															#line 7123 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1427 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

															#line 7128 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via unlockDatabaseCommitActor()
															#line 1428 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class UnlockDatabaseCommitActorActor>
															#line 1428 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class UnlockDatabaseCommitActorActorState {
															#line 7135 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1428 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	UnlockDatabaseCommitActorActorState(ReadYourWritesTransaction* const& ryw) 
															#line 1428 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1428 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw)
															#line 7142 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1429 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1430 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(databaseLockedKey);
															#line 1431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<UnlockDatabaseCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7163 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<UnlockDatabaseCommitActorActor*>(this)->actor_wait_state = 1;
															#line 1431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< UnlockDatabaseCommitActorActor, 0, Optional<Value> >*>(static_cast<UnlockDatabaseCommitActorActor*>(this)));
															#line 7168 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1432 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 7191 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1433 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().clear(singleKeyRange(databaseLockedKey));
															#line 7195 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1435 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<UnlockDatabaseCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~UnlockDatabaseCommitActorActorState(); static_cast<UnlockDatabaseCommitActorActor*>(this)->destroy(); return 0; }
															#line 7199 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<UnlockDatabaseCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~UnlockDatabaseCommitActorActorState();
		static_cast<UnlockDatabaseCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 1432 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 7211 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1433 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().clear(singleKeyRange(databaseLockedKey));
															#line 7215 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1435 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<UnlockDatabaseCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~UnlockDatabaseCommitActorActorState(); static_cast<UnlockDatabaseCommitActorActor*>(this)->destroy(); return 0; }
															#line 7219 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1428 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 7292 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via unlockDatabaseCommitActor()
															#line 1428 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class UnlockDatabaseCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< UnlockDatabaseCommitActorActor, 0, Optional<Value> >, public FastAllocated<UnlockDatabaseCommitActorActor>, public UnlockDatabaseCommitActorActorState<UnlockDatabaseCommitActorActor> {
															#line 7297 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<UnlockDatabaseCommitActorActor>::operator new;
	using FastAllocated<UnlockDatabaseCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< UnlockDatabaseCommitActorActor, 0, Optional<Value> >;
															#line 1428 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	UnlockDatabaseCommitActorActor(ReadYourWritesTransaction* const& ryw) 
															#line 7308 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1428 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Optional<std::string>> unlockDatabaseCommitActor( ReadYourWritesTransaction* const& ryw ) {
															#line 1428 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new UnlockDatabaseCommitActorActor(ryw));
															#line 7336 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1437 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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

															#line 7358 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via getConsistencyCheckKeyActor()
															#line 1455 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class GetConsistencyCheckKeyActorActor>
															#line 1455 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetConsistencyCheckKeyActorActorState {
															#line 7365 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1455 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetConsistencyCheckKeyActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 1455 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1455 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1455 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 7374 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1456 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1457 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1458 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1459 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(fdbShouldConsistencyCheckBeSuspended);
															#line 1459 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GetConsistencyCheckKeyActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7397 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetConsistencyCheckKeyActorActor*>(this)->actor_wait_state = 1;
															#line 1459 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetConsistencyCheckKeyActorActor, 0, Optional<Value> >*>(static_cast<GetConsistencyCheckKeyActorActor*>(this)));
															#line 7402 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1460 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		bool ccSuspendSetting = val.present() ? BinaryReader::fromStringRef<bool>(val.get(), Unversioned()) : false;
															#line 1461 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1462 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (ccSuspendSetting)
															#line 7429 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1463 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, ValueRef()));
															#line 7433 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1465 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetConsistencyCheckKeyActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetConsistencyCheckKeyActorActorState(); static_cast<GetConsistencyCheckKeyActorActor*>(this)->destroy(); return 0; }
															#line 7437 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetConsistencyCheckKeyActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetConsistencyCheckKeyActorActorState();
		static_cast<GetConsistencyCheckKeyActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 1460 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		bool ccSuspendSetting = val.present() ? BinaryReader::fromStringRef<bool>(val.get(), Unversioned()) : false;
															#line 1461 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1462 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (ccSuspendSetting)
															#line 7453 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1463 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, ValueRef()));
															#line 7457 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1465 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetConsistencyCheckKeyActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetConsistencyCheckKeyActorActorState(); static_cast<GetConsistencyCheckKeyActorActor*>(this)->destroy(); return 0; }
															#line 7461 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1455 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1455 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 7536 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getConsistencyCheckKeyActor()
															#line 1455 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetConsistencyCheckKeyActorActor final : public Actor<RangeResult>, public ActorCallback< GetConsistencyCheckKeyActorActor, 0, Optional<Value> >, public FastAllocated<GetConsistencyCheckKeyActorActor>, public GetConsistencyCheckKeyActorActorState<GetConsistencyCheckKeyActorActor> {
															#line 7541 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GetConsistencyCheckKeyActorActor>::operator new;
	using FastAllocated<GetConsistencyCheckKeyActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetConsistencyCheckKeyActorActor, 0, Optional<Value> >;
															#line 1455 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetConsistencyCheckKeyActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 7552 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1455 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> getConsistencyCheckKeyActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 1455 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new GetConsistencyCheckKeyActorActor(ryw, kr));
															#line 7580 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1467 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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
															#line 7664 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via globalConfigCommitActor()
															#line 1547 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class GlobalConfigCommitActorActor>
															#line 1547 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GlobalConfigCommitActorActorState {
															#line 7671 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1547 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GlobalConfigCommitActorActorState(GlobalConfigImpl* const& globalConfig,ReadYourWritesTransaction* const& ryw) 
															#line 1547 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1547 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : globalConfig(globalConfig),
															#line 1547 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   ryw(ryw),
															#line 1549 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   tr(ryw->getTransaction())
															#line 7682 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1550 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1554 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr.getRange(globalConfigHistoryKeys, CLIENT_KNOBS->TOO_MANY);
															#line 1554 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GlobalConfigCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7701 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GlobalConfigCommitActorActor*>(this)->actor_wait_state = 1;
															#line 1554 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GlobalConfigCommitActorActor, 0, RangeResult >*>(static_cast<GlobalConfigCommitActorActor*>(this)));
															#line 7706 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1555 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		constexpr int kGlobalConfigMaxHistorySize = 3;
															#line 1556 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (history.size() > kGlobalConfigMaxHistorySize - 1)
															#line 7731 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1557 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for(int i = 0;i < history.size() - (kGlobalConfigMaxHistorySize - 1);++i) {
															#line 1558 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				tr.clear(history[i].key);
															#line 7737 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1562 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		Standalone<VectorRef<KeyValueRef>> insertions;
															#line 1563 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		Standalone<VectorRef<KeyRangeRef>> clears;
															#line 1568 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(specialKeys);
															#line 1570 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		iter = ranges.begin();
															#line 1571 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for(;iter != ranges.end();) {
															#line 1572 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			std::pair<bool, Optional<Value>> entry = iter->value();
															#line 1573 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (entry.first)
															#line 7754 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1574 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (entry.second.present() && iter->begin().startsWith(globalConfig->getKeyRange().begin))
															#line 7758 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1575 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					Key bareKey = iter->begin().removePrefix(globalConfig->getKeyRange().begin);
															#line 1576 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					insertions.push_back_deep(insertions.arena(), KeyValueRef(bareKey, entry.second.get()));
															#line 7764 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
				else
				{
															#line 1577 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (!entry.second.present() && iter->range().begin.startsWith(globalConfig->getKeyRange().begin) && iter->range().end.startsWith(globalConfig->getKeyRange().begin))
															#line 7770 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 1579 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						KeyRef bareRangeBegin = iter->range().begin.removePrefix(globalConfig->getKeyRange().begin);
															#line 1580 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						KeyRef bareRangeEnd = iter->range().end.removePrefix(globalConfig->getKeyRange().begin);
															#line 1581 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						clears.push_back_deep(clears.arena(), KeyRangeRef(bareRangeBegin, bareRangeEnd));
															#line 7778 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
			}
															#line 1584 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			++iter;
															#line 7784 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1586 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		GlobalConfig::applyChanges(tr, insertions, clears);
															#line 1588 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GlobalConfigCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~GlobalConfigCommitActorActorState(); static_cast<GlobalConfigCommitActorActor*>(this)->destroy(); return 0; }
															#line 7790 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GlobalConfigCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~GlobalConfigCommitActorActorState();
		static_cast<GlobalConfigCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && history,int loopDepth) 
	{
															#line 1555 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		constexpr int kGlobalConfigMaxHistorySize = 3;
															#line 1556 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (history.size() > kGlobalConfigMaxHistorySize - 1)
															#line 7804 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1557 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for(int i = 0;i < history.size() - (kGlobalConfigMaxHistorySize - 1);++i) {
															#line 1558 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				tr.clear(history[i].key);
															#line 7810 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1562 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		Standalone<VectorRef<KeyValueRef>> insertions;
															#line 1563 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		Standalone<VectorRef<KeyRangeRef>> clears;
															#line 1568 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(specialKeys);
															#line 1570 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		iter = ranges.begin();
															#line 1571 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for(;iter != ranges.end();) {
															#line 1572 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			std::pair<bool, Optional<Value>> entry = iter->value();
															#line 1573 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (entry.first)
															#line 7827 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1574 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (entry.second.present() && iter->begin().startsWith(globalConfig->getKeyRange().begin))
															#line 7831 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1575 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					Key bareKey = iter->begin().removePrefix(globalConfig->getKeyRange().begin);
															#line 1576 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					insertions.push_back_deep(insertions.arena(), KeyValueRef(bareKey, entry.second.get()));
															#line 7837 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
				else
				{
															#line 1577 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (!entry.second.present() && iter->range().begin.startsWith(globalConfig->getKeyRange().begin) && iter->range().end.startsWith(globalConfig->getKeyRange().begin))
															#line 7843 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 1579 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						KeyRef bareRangeBegin = iter->range().begin.removePrefix(globalConfig->getKeyRange().begin);
															#line 1580 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						KeyRef bareRangeEnd = iter->range().end.removePrefix(globalConfig->getKeyRange().begin);
															#line 1581 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						clears.push_back_deep(clears.arena(), KeyRangeRef(bareRangeBegin, bareRangeEnd));
															#line 7851 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
			}
															#line 1584 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			++iter;
															#line 7857 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1586 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		GlobalConfig::applyChanges(tr, insertions, clears);
															#line 1588 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GlobalConfigCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~GlobalConfigCommitActorActorState(); static_cast<GlobalConfigCommitActorActor*>(this)->destroy(); return 0; }
															#line 7863 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1547 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GlobalConfigImpl* globalConfig;
															#line 1547 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1549 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Transaction& tr;
															#line 1568 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, std::pair<bool, Optional<Value>>, KeyRangeRef>::Ranges ranges;
															#line 1570 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, std::pair<bool, Optional<Value>>, KeyRangeRef>::iterator iter;
															#line 7944 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via globalConfigCommitActor()
															#line 1547 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GlobalConfigCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< GlobalConfigCommitActorActor, 0, RangeResult >, public FastAllocated<GlobalConfigCommitActorActor>, public GlobalConfigCommitActorActorState<GlobalConfigCommitActorActor> {
															#line 7949 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GlobalConfigCommitActorActor>::operator new;
	using FastAllocated<GlobalConfigCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GlobalConfigCommitActorActor, 0, RangeResult >;
															#line 1547 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GlobalConfigCommitActorActor(GlobalConfigImpl* const& globalConfig,ReadYourWritesTransaction* const& ryw) 
															#line 7960 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1547 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Optional<std::string>> globalConfigCommitActor( GlobalConfigImpl* const& globalConfig, ReadYourWritesTransaction* const& ryw ) {
															#line 1547 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new GlobalConfigCommitActorActor(globalConfig, ryw));
															#line 7988 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1590 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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
			                      KeyValueRef(key, std::to_string(ryw->getTransactionState()->spanID.first())));
		} else if (key.endsWith(kTracingTokenKey)) {
			result.push_back_deep(result.arena(),
			                      KeyValueRef(key, std::to_string(ryw->getTransactionState()->spanID.second())));
		}
	}
	return result;
}

void TracingOptionsImpl::set(ReadYourWritesTransaction* ryw, const KeyRef& key, const ValueRef& value) {
	if (ryw->getApproximateSize() > 0) {
		ryw->setSpecialKeySpaceErrorMsg("tracing options must be set first");
		ryw->getSpecialKeySpaceWriteMap().insert(key, std::make_pair(true, Optional<Value>()));
		return;
	}

	if (key.endsWith(kTracingTransactionIdKey)) {
		ryw->setTransactionID(std::stoul(value.toString()));
	} else if (key.endsWith(kTracingTokenKey)) {
		if (value.toString() == "true") {
			ryw->setToken(deterministicRandom()->randomUInt64());
		} else if (value.toString() == "false") {
			ryw->setToken(0);
		} else {
			ryw->setSpecialKeySpaceErrorMsg("token must be set to true/false");
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
	ryw->setSpecialKeySpaceErrorMsg("clear range disabled");
	throw special_keys_api_failure();
}

void TracingOptionsImpl::clear(ReadYourWritesTransaction* ryw, const KeyRef& key) {
	ryw->setSpecialKeySpaceErrorMsg("clear disabled");
	throw special_keys_api_failure();
}

CoordinatorsImpl::CoordinatorsImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

															#line 8071 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via coordinatorsGetRangeActor()
															#line 1669 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class CoordinatorsGetRangeActorActor>
															#line 1669 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class CoordinatorsGetRangeActorActorState {
															#line 8078 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1669 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	CoordinatorsGetRangeActorActorState(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 1669 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1669 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1669 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   prefix(prefix),
															#line 1669 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr),
															#line 1670 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   cs(ryw->getDatabase()->getConnectionRecord()->getConnectionString())
															#line 8091 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1671 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<std::vector<NetworkAddress>> __when_expr_0 = cs.tryResolveHostnames();
															#line 1671 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<CoordinatorsGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8108 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CoordinatorsGetRangeActorActor*>(this)->actor_wait_state = 1;
															#line 1671 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CoordinatorsGetRangeActorActor, 0, std::vector<NetworkAddress> >*>(static_cast<CoordinatorsGetRangeActorActor*>(this)));
															#line 8113 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1672 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1673 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		Key cluster_decription_key = prefix.withSuffix(LiteralStringRef("cluster_description"));
															#line 1674 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (kr.contains(cluster_decription_key))
															#line 8140 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1675 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(cluster_decription_key, cs.clusterKeyName()));
															#line 8144 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1679 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		std::sort(coordinator_processes.begin(), coordinator_processes.end(), [](const NetworkAddress& lhs, const NetworkAddress& rhs) { return lhs.toString() < rhs.toString(); });
															#line 1682 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		std::string processes_str;
															#line 1683 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( const auto& w : coordinator_processes ) {
															#line 1684 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (processes_str.size())
															#line 8154 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1685 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				processes_str += ",";
															#line 8158 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 1686 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			processes_str += w.toString();
															#line 8162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1688 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		Key processes_key = prefix.withSuffix(LiteralStringRef("processes"));
															#line 1689 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (kr.contains(processes_key))
															#line 8168 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1690 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(processes_key, Value(processes_str)));
															#line 8172 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1692 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<CoordinatorsGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~CoordinatorsGetRangeActorActorState(); static_cast<CoordinatorsGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 8176 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<CoordinatorsGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(rywGetRange(ryw, kr, result));
		this->~CoordinatorsGetRangeActorActorState();
		static_cast<CoordinatorsGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<NetworkAddress> const& __coordinator_processes,int loopDepth) 
	{
															#line 1671 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		coordinator_processes = __coordinator_processes;
															#line 8188 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1669 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1669 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef prefix;
															#line 1669 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 1670 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ClusterConnectionString cs;
															#line 1671 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<NetworkAddress> coordinator_processes;
															#line 8261 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via coordinatorsGetRangeActor()
															#line 1669 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class CoordinatorsGetRangeActorActor final : public Actor<RangeResult>, public ActorCallback< CoordinatorsGetRangeActorActor, 0, std::vector<NetworkAddress> >, public FastAllocated<CoordinatorsGetRangeActorActor>, public CoordinatorsGetRangeActorActorState<CoordinatorsGetRangeActorActor> {
															#line 8266 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<CoordinatorsGetRangeActorActor>::operator new;
	using FastAllocated<CoordinatorsGetRangeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CoordinatorsGetRangeActorActor, 0, std::vector<NetworkAddress> >;
															#line 1669 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	CoordinatorsGetRangeActorActor(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 8277 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1669 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> coordinatorsGetRangeActor( ReadYourWritesTransaction* const& ryw, KeyRef const& prefix, KeyRangeRef const& kr ) {
															#line 1669 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new CoordinatorsGetRangeActorActor(ryw, prefix, kr));
															#line 8305 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1694 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

Future<RangeResult> CoordinatorsImpl::getRange(ReadYourWritesTransaction* ryw,
                                               KeyRangeRef kr,
                                               GetRangeLimits limitsHint) const {
	KeyRef prefix(getKeyRange().begin);
	return coordinatorsGetRangeActor(ryw, prefix, kr);
}

															#line 8317 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via coordinatorsCommitActor()
															#line 1702 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class CoordinatorsCommitActorActor>
															#line 1702 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class CoordinatorsCommitActorActorState {
															#line 8324 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1702 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	CoordinatorsCommitActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 1702 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1702 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1702 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr),
															#line 1703 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   conn(),
															#line 1704 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   process_address_or_hostname_strs(),
															#line 1705 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   msg(),
															#line 1706 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   index(),
															#line 1707 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   parse_error(false)
															#line 8343 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1710 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			Key processes_key = LiteralStringRef("processes").withPrefix(kr.begin);
															#line 1711 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			auto processes_entry = ryw->getSpecialKeySpaceWriteMap()[processes_key];
															#line 1712 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (processes_entry.first)
															#line 8362 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1713 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ASSERT(processes_entry.second.present());
															#line 1714 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				auto processesStr = processes_entry.second.get().toString();
															#line 1715 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				boost::split(process_address_or_hostname_strs, processesStr, [](char c) { return c == ','; });
															#line 1716 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (!process_address_or_hostname_strs.size())
															#line 8372 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1717 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (!static_cast<CoordinatorsCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(ManagementAPIError::toJsonString( false, "coordinators", "New coordinators\' processes are empty, please specify new processes\' network addresses with format " "\"IP:PORT,IP:PORT,...,IP:PORT\" or \"HOSTNAME:PORT,HOSTNAME:PORT,...,HOSTNAME:PORT\"")); this->~CoordinatorsCommitActorActorState(); static_cast<CoordinatorsCommitActorActor*>(this)->destroy(); return 0; }
															#line 8376 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					new (&static_cast<CoordinatorsCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(ManagementAPIError::toJsonString( false, "coordinators", "New coordinators\' processes are empty, please specify new processes\' network addresses with format " "\"IP:PORT,IP:PORT,...,IP:PORT\" or \"HOSTNAME:PORT,HOSTNAME:PORT,...,HOSTNAME:PORT\""));
					this->~CoordinatorsCommitActorActorState();
					static_cast<CoordinatorsCommitActorActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 1723 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				index = 0;
															#line 8384 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1748 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		std::string newName;
															#line 1750 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		Key cluster_decription_key = LiteralStringRef("cluster_description").withPrefix(kr.begin);
															#line 1751 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		auto entry = ryw->getSpecialKeySpaceWriteMap()[cluster_decription_key];
															#line 1752 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (entry.first)
															#line 8418 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1754 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (entry.second.present() && isAlphaNumeric(entry.second.get().toString()))
															#line 8422 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1756 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				newName = entry.second.get().toString();
															#line 8426 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 1759 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (!static_cast<CoordinatorsCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(ManagementAPIError::toJsonString( false, "coordinators", "Cluster description must match [A-Za-z0-9_]+")); this->~CoordinatorsCommitActorActorState(); static_cast<CoordinatorsCommitActorActor*>(this)->destroy(); return 0; }
															#line 8432 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				new (&static_cast<CoordinatorsCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(ManagementAPIError::toJsonString( false, "coordinators", "Cluster description must match [A-Za-z0-9_]+"));
				this->~CoordinatorsCommitActorActorState();
				static_cast<CoordinatorsCommitActorActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 1764 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		TraceEvent(SevDebug, "SKSChangeCoordinatorsStart") .detail("NewConnectionString", conn.toString()) .detail("Description", entry.first ? entry.second.get().toString() : "");
															#line 1768 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Optional<CoordinatorsResult>> __when_expr_0 = changeQuorumChecker(&ryw->getTransaction(), &conn, newName);
															#line 1768 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<CoordinatorsCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8445 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1cont1when1(__when_expr_0.get(), loopDepth); };
		static_cast<CoordinatorsCommitActorActor*>(this)->actor_wait_state = 1;
															#line 1768 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CoordinatorsCommitActorActor, 0, Optional<CoordinatorsResult> >*>(static_cast<CoordinatorsCommitActorActor*>(this)));
															#line 8450 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1723 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!(index < process_address_or_hostname_strs.size()))
															#line 8472 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
		try {
															#line 1725 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (Hostname::isHostname(process_address_or_hostname_strs[index]))
															#line 8479 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1726 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				conn.hostnames.push_back(Hostname::parse(process_address_or_hostname_strs[index]));
															#line 8483 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 1728 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				NetworkAddress a = NetworkAddress::parse(process_address_or_hostname_strs[index]);
															#line 1729 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (!a.isValid())
															#line 8491 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1730 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					parse_error = true;
															#line 8495 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
				else
				{
															#line 1732 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					conn.coords.push_back(a);
															#line 8501 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1740 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (parse_error)
															#line 8531 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1741 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			std::string error = "ERROR: \'" + process_address_or_hostname_strs[index] + "\' is not a valid network endpoint address\n";
															#line 1743 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<CoordinatorsCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(ManagementAPIError::toJsonString(false, "coordinators", error)); this->~CoordinatorsCommitActorActorState(); static_cast<CoordinatorsCommitActorActor*>(this)->destroy(); return 0; }
															#line 8537 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<CoordinatorsCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(ManagementAPIError::toJsonString(false, "coordinators", error));
			this->~CoordinatorsCommitActorActorState();
			static_cast<CoordinatorsCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1723 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		index++;
															#line 8545 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1736 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			TraceEvent(SevDebug, "SpecialKeysNetworkParseError").error(e);
															#line 1737 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			parse_error = true;
															#line 8557 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1770 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		TraceEvent(SevDebug, "SKSChangeCoordinatorsFinish") .detail("Result", r.present() ? static_cast<int>(r.get()) : -1);
															#line 1772 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (r.present())
															#line 8587 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1773 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			auto res = r.get();
															#line 1774 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			bool retriable = false;
															#line 1775 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (res == CoordinatorsResult::COORDINATOR_UNREACHABLE)
															#line 8595 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1776 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				retriable = true;
															#line 8599 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 1777 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (res == CoordinatorsResult::SUCCESS)
															#line 8605 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1778 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					TraceEvent(SevError, "SpecialKeysForCoordinators").detail("UnexpectedSuccessfulResult", "");
															#line 1779 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					ASSERT(false);
															#line 8611 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
															#line 1781 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			msg = ManagementAPIError::toJsonString(retriable, "coordinators", ManagementAPI::generateErrorMessage(res));
															#line 8616 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1783 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<CoordinatorsCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(msg); this->~CoordinatorsCommitActorActorState(); static_cast<CoordinatorsCommitActorActor*>(this)->destroy(); return 0; }
															#line 8620 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<CoordinatorsCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(msg)); // state_var_RVO
		this->~CoordinatorsCommitActorActorState();
		static_cast<CoordinatorsCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Optional<CoordinatorsResult> && r,int loopDepth) 
	{
															#line 1770 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		TraceEvent(SevDebug, "SKSChangeCoordinatorsFinish") .detail("Result", r.present() ? static_cast<int>(r.get()) : -1);
															#line 1772 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (r.present())
															#line 8634 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1773 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			auto res = r.get();
															#line 1774 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			bool retriable = false;
															#line 1775 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (res == CoordinatorsResult::COORDINATOR_UNREACHABLE)
															#line 8642 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1776 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				retriable = true;
															#line 8646 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 1777 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (res == CoordinatorsResult::SUCCESS)
															#line 8652 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 1778 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					TraceEvent(SevError, "SpecialKeysForCoordinators").detail("UnexpectedSuccessfulResult", "");
															#line 1779 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					ASSERT(false);
															#line 8658 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
															#line 1781 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			msg = ManagementAPIError::toJsonString(retriable, "coordinators", ManagementAPI::generateErrorMessage(res));
															#line 8663 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1783 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<CoordinatorsCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(msg); this->~CoordinatorsCommitActorActorState(); static_cast<CoordinatorsCommitActorActor*>(this)->destroy(); return 0; }
															#line 8667 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1702 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1702 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 1703 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ClusterConnectionString conn;
															#line 1704 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<std::string> process_address_or_hostname_strs;
															#line 1705 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Optional<std::string> msg;
															#line 1706 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	int index;
															#line 1707 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	bool parse_error;
															#line 8752 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via coordinatorsCommitActor()
															#line 1702 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class CoordinatorsCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< CoordinatorsCommitActorActor, 0, Optional<CoordinatorsResult> >, public FastAllocated<CoordinatorsCommitActorActor>, public CoordinatorsCommitActorActorState<CoordinatorsCommitActorActor> {
															#line 8757 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<CoordinatorsCommitActorActor>::operator new;
	using FastAllocated<CoordinatorsCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CoordinatorsCommitActorActor, 0, Optional<CoordinatorsResult> >;
															#line 1702 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	CoordinatorsCommitActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 8768 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1702 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<Optional<std::string>> coordinatorsCommitActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 1702 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new CoordinatorsCommitActorActor(ryw, kr));
															#line 8796 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1785 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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

															#line 8816 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via CoordinatorsAutoImplActor()
															#line 1801 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class CoordinatorsAutoImplActorActor>
															#line 1801 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class CoordinatorsAutoImplActorActorState {
															#line 8823 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1801 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	CoordinatorsAutoImplActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 1801 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1801 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1801 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr),
															#line 1802 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   res(),
															#line 1803 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   autoCoordinatorsKey(),
															#line 1804 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   tr(ryw->getTransaction())
															#line 8838 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1806 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1807 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			tr.setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1808 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			tr.setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 1809 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 1810 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(coordinatorsKey);
															#line 1810 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8863 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state = 1;
															#line 1810 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CoordinatorsAutoImplActorActor, 0, Optional<Value> >*>(static_cast<CoordinatorsAutoImplActorActor*>(this)));
															#line 8868 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1812 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!currentKey.present())
															#line 8891 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1813 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg( ManagementAPIError::toJsonString(false, "auto_coordinators", "The coordinator key does not exist"));
															#line 1815 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 8897 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1817 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		old = ClusterConnectionString(currentKey.get().toString());
															#line 1818 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		result = CoordinatorsResult::SUCCESS;
															#line 1820 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<std::vector<NetworkAddress>> __when_expr_1 = old.tryResolveHostnames();
															#line 1820 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8907 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state = 2;
															#line 1820 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CoordinatorsAutoImplActorActor, 1, std::vector<NetworkAddress> >*>(static_cast<CoordinatorsAutoImplActorActor*>(this)));
															#line 8912 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && currentKey,int loopDepth) 
	{
															#line 1812 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!currentKey.present())
															#line 8921 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1813 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg( ManagementAPIError::toJsonString(false, "auto_coordinators", "The coordinator key does not exist"));
															#line 1815 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 8927 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1817 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		old = ClusterConnectionString(currentKey.get().toString());
															#line 1818 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		result = CoordinatorsResult::SUCCESS;
															#line 1820 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<std::vector<NetworkAddress>> __when_expr_1 = old.tryResolveHostnames();
															#line 1820 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8937 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state = 2;
															#line 1820 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CoordinatorsAutoImplActorActor, 1, std::vector<NetworkAddress> >*>(static_cast<CoordinatorsAutoImplActorActor*>(this)));
															#line 8942 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1821 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<std::vector<NetworkAddress>> __when_expr_2 = autoQuorumChange()->getDesiredCoordinators( &tr, oldCoordinators, Reference<ClusterConnectionMemoryRecord>(new ClusterConnectionMemoryRecord(old)), result);
															#line 1821 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9016 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state = 3;
															#line 1821 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CoordinatorsAutoImplActorActor, 2, std::vector<NetworkAddress> >*>(static_cast<CoordinatorsAutoImplActorActor*>(this)));
															#line 9021 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(std::vector<NetworkAddress> && oldCoordinators,int loopDepth) 
	{
															#line 1821 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<std::vector<NetworkAddress>> __when_expr_2 = autoQuorumChange()->getDesiredCoordinators( &tr, oldCoordinators, Reference<ClusterConnectionMemoryRecord>(new ClusterConnectionMemoryRecord(old)), result);
															#line 1821 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9032 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<CoordinatorsAutoImplActorActor*>(this)->actor_wait_state = 3;
															#line 1821 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CoordinatorsAutoImplActorActor, 2, std::vector<NetworkAddress> >*>(static_cast<CoordinatorsAutoImplActorActor*>(this)));
															#line 9037 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1827 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (result == CoordinatorsResult::NOT_ENOUGH_MACHINES)
															#line 9109 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1830 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg(ManagementAPIError::toJsonString( true, "auto_coordinators", "Too few fdbserver machines to provide coordination at the current redundancy level"));
															#line 1834 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 9115 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1837 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (result == CoordinatorsResult::SAME_NETWORK_ADDRESSES)
															#line 9119 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1838 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& host : old.hostnames ) {
															#line 1839 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += autoCoordinatorsKey.size() ? "," : "";
															#line 1840 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += host.toString();
															#line 9127 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 1842 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& coord : old.coords ) {
															#line 1843 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += autoCoordinatorsKey.size() ? "," : "";
															#line 1844 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += coord.toString();
															#line 9135 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
		else
		{
															#line 1847 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& address : _desiredCoordinators ) {
															#line 1848 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += autoCoordinatorsKey.size() ? "," : "";
															#line 1849 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += address.toString();
															#line 9146 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1852 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		res.push_back_deep(res.arena(), KeyValueRef(kr.begin, Value(autoCoordinatorsKey)));
															#line 1853 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<CoordinatorsAutoImplActorActor*>(this)->SAV<RangeResult>::futures) { (void)(res); this->~CoordinatorsAutoImplActorActorState(); static_cast<CoordinatorsAutoImplActorActor*>(this)->destroy(); return 0; }
															#line 9153 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<CoordinatorsAutoImplActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(res)); // state_var_RVO
		this->~CoordinatorsAutoImplActorActorState();
		static_cast<CoordinatorsAutoImplActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(std::vector<NetworkAddress> && _desiredCoordinators,int loopDepth) 
	{
															#line 1827 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (result == CoordinatorsResult::NOT_ENOUGH_MACHINES)
															#line 9165 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1830 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg(ManagementAPIError::toJsonString( true, "auto_coordinators", "Too few fdbserver machines to provide coordination at the current redundancy level"));
															#line 1834 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 9171 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1837 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (result == CoordinatorsResult::SAME_NETWORK_ADDRESSES)
															#line 9175 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1838 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& host : old.hostnames ) {
															#line 1839 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += autoCoordinatorsKey.size() ? "," : "";
															#line 1840 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += host.toString();
															#line 9183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 1842 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& coord : old.coords ) {
															#line 1843 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += autoCoordinatorsKey.size() ? "," : "";
															#line 1844 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += coord.toString();
															#line 9191 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
		else
		{
															#line 1847 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& address : _desiredCoordinators ) {
															#line 1848 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += autoCoordinatorsKey.size() ? "," : "";
															#line 1849 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				autoCoordinatorsKey += address.toString();
															#line 9202 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 1852 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		res.push_back_deep(res.arena(), KeyValueRef(kr.begin, Value(autoCoordinatorsKey)));
															#line 1853 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<CoordinatorsAutoImplActorActor*>(this)->SAV<RangeResult>::futures) { (void)(res); this->~CoordinatorsAutoImplActorActorState(); static_cast<CoordinatorsAutoImplActorActor*>(this)->destroy(); return 0; }
															#line 9209 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1801 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1801 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 1802 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult res;
															#line 1803 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::string autoCoordinatorsKey;
															#line 1804 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Transaction& tr;
															#line 1817 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ClusterConnectionString old;
															#line 1818 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	CoordinatorsResult result;
															#line 9294 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via CoordinatorsAutoImplActor()
															#line 1801 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class CoordinatorsAutoImplActorActor final : public Actor<RangeResult>, public ActorCallback< CoordinatorsAutoImplActorActor, 0, Optional<Value> >, public ActorCallback< CoordinatorsAutoImplActorActor, 1, std::vector<NetworkAddress> >, public ActorCallback< CoordinatorsAutoImplActorActor, 2, std::vector<NetworkAddress> >, public FastAllocated<CoordinatorsAutoImplActorActor>, public CoordinatorsAutoImplActorActorState<CoordinatorsAutoImplActorActor> {
															#line 9299 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1801 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	CoordinatorsAutoImplActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 9312 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1801 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<RangeResult> CoordinatorsAutoImplActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 1801 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new CoordinatorsAutoImplActorActor(ryw, kr));
															#line 9342 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1855 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

Future<RangeResult> CoordinatorsAutoImpl::getRange(ReadYourWritesTransaction* ryw,
                                                   KeyRangeRef kr,
                                                   GetRangeLimits limitsHint) const {
	// single key range, the queried range should always be the same as the underlying range
	ASSERT(kr == getKeyRange());
	return CoordinatorsAutoImplActor(ryw, kr);
}

															#line 9355 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via getMinCommitVersionActor()
															#line 1864 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class GetMinCommitVersionActorActor>
															#line 1864 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetMinCommitVersionActorActorState {
															#line 9362 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1864 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetMinCommitVersionActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 1864 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1864 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1864 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 9371 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1866 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1867 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(minRequiredCommitVersionKey);
															#line 1867 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GetMinCommitVersionActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9392 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetMinCommitVersionActorActor*>(this)->actor_wait_state = 1;
															#line 1867 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetMinCommitVersionActorActor, 0, Optional<Value> >*>(static_cast<GetMinCommitVersionActorActor*>(this)));
															#line 9397 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1868 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1869 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 9422 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1870 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			Version minRequiredCommitVersion = BinaryReader::fromStringRef<Version>(val.get(), Unversioned());
															#line 1871 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ValueRef version(result.arena(), boost::lexical_cast<std::string>(minRequiredCommitVersion));
															#line 1872 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, version));
															#line 9430 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1874 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetMinCommitVersionActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetMinCommitVersionActorActorState(); static_cast<GetMinCommitVersionActorActor*>(this)->destroy(); return 0; }
															#line 9434 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetMinCommitVersionActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetMinCommitVersionActorActorState();
		static_cast<GetMinCommitVersionActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 1868 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1869 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 9448 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1870 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			Version minRequiredCommitVersion = BinaryReader::fromStringRef<Version>(val.get(), Unversioned());
															#line 1871 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ValueRef version(result.arena(), boost::lexical_cast<std::string>(minRequiredCommitVersion));
															#line 1872 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, version));
															#line 9456 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1874 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetMinCommitVersionActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetMinCommitVersionActorActorState(); static_cast<GetMinCommitVersionActorActor*>(this)->destroy(); return 0; }
															#line 9460 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1864 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1864 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 9535 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getMinCommitVersionActor()
															#line 1864 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetMinCommitVersionActorActor final : public Actor<RangeResult>, public ActorCallback< GetMinCommitVersionActorActor, 0, Optional<Value> >, public FastAllocated<GetMinCommitVersionActorActor>, public GetMinCommitVersionActorActorState<GetMinCommitVersionActorActor> {
															#line 9540 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GetMinCommitVersionActorActor>::operator new;
	using FastAllocated<GetMinCommitVersionActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetMinCommitVersionActorActor, 0, Optional<Value> >;
															#line 1864 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetMinCommitVersionActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 9551 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1864 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<RangeResult> getMinCommitVersionActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 1864 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new GetMinCommitVersionActorActor(ryw, kr));
															#line 9579 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1876 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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

															#line 9604 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via advanceVersionCommitActor()
															#line 1897 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class AdvanceVersionCommitActorActor>
															#line 1897 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class AdvanceVersionCommitActorActorState {
															#line 9611 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1897 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	AdvanceVersionCommitActorActorState(ReadYourWritesTransaction* const& ryw,Version const& v) 
															#line 1897 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1897 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1897 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   v(v)
															#line 9620 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1900 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			static const Version maxAllowedVerion = std::numeric_limits<int64_t>::max() - 1 - CLIENT_KNOBS->VERSIONS_PER_SECOND * 3600 * 24 * 365 * 1000;
															#line 1903 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1904 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1905 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			TraceEvent(SevDebug, "AdvanceVersion").detail("MaxAllowedVersion", maxAllowedVerion);
															#line 1906 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (v > maxAllowedVerion)
															#line 9643 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 1907 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (!static_cast<AdvanceVersionCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(ManagementAPIError::toJsonString( false, "advanceversion", "The given version is larger than the maximum allowed value(2**63-1-version_per_second*3600*24*365*1000)")); this->~AdvanceVersionCommitActorActorState(); static_cast<AdvanceVersionCommitActorActor*>(this)->destroy(); return 0; }
															#line 9647 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				new (&static_cast<AdvanceVersionCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(ManagementAPIError::toJsonString( false, "advanceversion", "The given version is larger than the maximum allowed value(2**63-1-version_per_second*3600*24*365*1000)"));
				this->~AdvanceVersionCommitActorActorState();
				static_cast<AdvanceVersionCommitActorActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1912 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Version> __when_expr_0 = ryw->getTransaction().getReadVersion();
															#line 1912 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<AdvanceVersionCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9657 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AdvanceVersionCommitActorActor*>(this)->actor_wait_state = 1;
															#line 1912 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AdvanceVersionCommitActorActor, 0, Version >*>(static_cast<AdvanceVersionCommitActorActor*>(this)));
															#line 9662 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
	int a_body1cont1(Version const& rv,int loopDepth) 
	{
															#line 1913 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (rv <= v)
															#line 9685 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1914 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().set(minRequiredCommitVersionKey, BinaryWriter::toValue(v + 1, Unversioned()));
															#line 9689 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
		else
		{
															#line 1916 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<AdvanceVersionCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(ManagementAPIError::toJsonString( false, "advanceversion", "Current read version is larger than the given version")); this->~AdvanceVersionCommitActorActorState(); static_cast<AdvanceVersionCommitActorActor*>(this)->destroy(); return 0; }
															#line 9695 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<AdvanceVersionCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(ManagementAPIError::toJsonString( false, "advanceversion", "Current read version is larger than the given version"));
			this->~AdvanceVersionCommitActorActorState();
			static_cast<AdvanceVersionCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1919 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<AdvanceVersionCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~AdvanceVersionCommitActorActorState(); static_cast<AdvanceVersionCommitActorActor*>(this)->destroy(); return 0; }
															#line 9703 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<AdvanceVersionCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~AdvanceVersionCommitActorActorState();
		static_cast<AdvanceVersionCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Version && rv,int loopDepth) 
	{
															#line 1913 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (rv <= v)
															#line 9715 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1914 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().set(minRequiredCommitVersionKey, BinaryWriter::toValue(v + 1, Unversioned()));
															#line 9719 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
		else
		{
															#line 1916 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<AdvanceVersionCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(ManagementAPIError::toJsonString( false, "advanceversion", "Current read version is larger than the given version")); this->~AdvanceVersionCommitActorActorState(); static_cast<AdvanceVersionCommitActorActor*>(this)->destroy(); return 0; }
															#line 9725 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<AdvanceVersionCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(ManagementAPIError::toJsonString( false, "advanceversion", "Current read version is larger than the given version"));
			this->~AdvanceVersionCommitActorActorState();
			static_cast<AdvanceVersionCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1919 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<AdvanceVersionCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~AdvanceVersionCommitActorActorState(); static_cast<AdvanceVersionCommitActorActor*>(this)->destroy(); return 0; }
															#line 9733 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<AdvanceVersionCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~AdvanceVersionCommitActorActorState();
		static_cast<AdvanceVersionCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Version const& rv,int loopDepth) 
	{
		loopDepth = a_body1cont1(rv, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Version && rv,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(rv), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<AdvanceVersionCommitActorActor*>(this)->actor_wait_state > 0) static_cast<AdvanceVersionCommitActorActor*>(this)->actor_wait_state = 0;
		static_cast<AdvanceVersionCommitActorActor*>(this)->ActorCallback< AdvanceVersionCommitActorActor, 0, Version >::remove();

	}
	void a_callback_fire(ActorCallback< AdvanceVersionCommitActorActor, 0, Version >*,Version const& value) 
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
	void a_callback_fire(ActorCallback< AdvanceVersionCommitActorActor, 0, Version >*,Version && value) 
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
	void a_callback_error(ActorCallback< AdvanceVersionCommitActorActor, 0, Version >*,Error err) 
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
															#line 1897 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1897 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Version v;
															#line 9808 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via advanceVersionCommitActor()
															#line 1897 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class AdvanceVersionCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< AdvanceVersionCommitActorActor, 0, Version >, public FastAllocated<AdvanceVersionCommitActorActor>, public AdvanceVersionCommitActorActorState<AdvanceVersionCommitActorActor> {
															#line 9813 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<AdvanceVersionCommitActorActor>::operator new;
	using FastAllocated<AdvanceVersionCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AdvanceVersionCommitActorActor, 0, Version >;
															#line 1897 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	AdvanceVersionCommitActorActor(ReadYourWritesTransaction* const& ryw,Version const& v) 
															#line 9824 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		case 1: this->a_callback_error((ActorCallback< AdvanceVersionCommitActorActor, 0, Version >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1897 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<Optional<std::string>> advanceVersionCommitActor( ReadYourWritesTransaction* const& ryw, Version const& v ) {
															#line 1897 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new AdvanceVersionCommitActorActor(ryw, v));
															#line 9852 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1921 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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

															#line 9876 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via getVersionEpochActor()
															#line 1941 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class GetVersionEpochActorActor>
															#line 1941 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetVersionEpochActorActorState {
															#line 9883 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 1941 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetVersionEpochActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 1941 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 1941 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 1941 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 9892 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1942 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1943 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 1944 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(versionEpochKey);
															#line 1944 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GetVersionEpochActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 9913 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetVersionEpochActorActor*>(this)->actor_wait_state = 1;
															#line 1944 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetVersionEpochActorActor, 0, Optional<Value> >*>(static_cast<GetVersionEpochActorActor*>(this)));
															#line 9918 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1945 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1946 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 9943 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1947 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			int64_t versionEpoch = BinaryReader::fromStringRef<int64_t>(val.get(), Unversioned());
															#line 1948 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ValueRef version(result.arena(), boost::lexical_cast<std::string>(versionEpoch));
															#line 1949 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, version));
															#line 9951 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1951 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetVersionEpochActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetVersionEpochActorActorState(); static_cast<GetVersionEpochActorActor*>(this)->destroy(); return 0; }
															#line 9955 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetVersionEpochActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetVersionEpochActorActorState();
		static_cast<GetVersionEpochActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 1945 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult result;
															#line 1946 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 9969 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 1947 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			int64_t versionEpoch = BinaryReader::fromStringRef<int64_t>(val.get(), Unversioned());
															#line 1948 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ValueRef version(result.arena(), boost::lexical_cast<std::string>(versionEpoch));
															#line 1949 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(kr.begin, version));
															#line 9977 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 1951 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetVersionEpochActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetVersionEpochActorActorState(); static_cast<GetVersionEpochActorActor*>(this)->destroy(); return 0; }
															#line 9981 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1941 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1941 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 10056 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getVersionEpochActor()
															#line 1941 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetVersionEpochActorActor final : public Actor<RangeResult>, public ActorCallback< GetVersionEpochActorActor, 0, Optional<Value> >, public FastAllocated<GetVersionEpochActorActor>, public GetVersionEpochActorActorState<GetVersionEpochActorActor> {
															#line 10061 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GetVersionEpochActorActor>::operator new;
	using FastAllocated<GetVersionEpochActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetVersionEpochActorActor, 0, Optional<Value> >;
															#line 1941 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetVersionEpochActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 10072 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 1941 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<RangeResult> getVersionEpochActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 1941 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new GetVersionEpochActorActor(ryw, kr));
															#line 10100 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 1953 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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
	Key sampleRateKey = LiteralStringRef("client_txn_sample_rate").withPrefix(prefix);

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
	Key txnSizeLimitKey = LiteralStringRef("client_txn_size_limit").withPrefix(prefix);
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
	Key sampleRateKey = LiteralStringRef("client_txn_sample_rate").withPrefix(getKeyRange().begin);
	auto rateEntry = ryw->getSpecialKeySpaceWriteMap()[sampleRateKey];

	if (rateEntry.first && rateEntry.second.present()) {
		std::string sampleRateStr = rateEntry.second.get().toString();
		if (sampleRateStr == "default") {
			clears.push_back_deep(clears.arena(),
			                      KeyRangeRef(fdbClientInfoTxnSampleRate, keyAfter(fdbClientInfoTxnSampleRate)));
		} else {
			try {
				double sampleRate = boost::lexical_cast<double>(sampleRateStr);
				Tuple rate = Tuple().appendDouble(sampleRate);
				insertions.push_back_deep(insertions.arena(), KeyValueRef(fdbClientInfoTxnSampleRate, rate.pack()));
			} catch (boost::bad_lexical_cast& e) {
				return Optional<std::string>(ManagementAPIError::toJsonString(
				    false, "profile", "Invalid transaction sample rate(double): " + sampleRateStr));
			}
		}
	}
	// client_txn_size_limit
	Key txnSizeLimitKey = LiteralStringRef("client_txn_size_limit").withPrefix(getKeyRange().begin);
	auto sizeLimitEntry = ryw->getSpecialKeySpaceWriteMap()[txnSizeLimitKey];
	if (sizeLimitEntry.first && sizeLimitEntry.second.present()) {
		std::string sizeLimitStr = sizeLimitEntry.second.get().toString();
		if (sizeLimitStr == "default") {
			clears.push_back_deep(clears.arena(),
			                      KeyRangeRef(fdbClientInfoTxnSizeLimit, keyAfter(fdbClientInfoTxnSizeLimit)));
		} else {
			try {
				int64_t sizeLimit = boost::lexical_cast<int64_t>(sizeLimitStr);
				Tuple size = Tuple().append(sizeLimit);
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
	if (val == LiteralStringRef("disk") || val == LiteralStringRef("Disk")) {
		w = WaitState::Disk;
	} else if (val == LiteralStringRef("network") || val == LiteralStringRef("Network")) {
		w = WaitState::Network;
	} else if (val == LiteralStringRef("running") || val == LiteralStringRef("Running")) {
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

															#line 10334 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via actorLineageGetRangeActor()
															#line 2183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class ActorLineageGetRangeActorActor>
															#line 2183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class ActorLineageGetRangeActorActorState {
															#line 10341 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ActorLineageGetRangeActorActorState(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 2183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   prefix(prefix),
															#line 2183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr),
															#line 2186 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   result(),
															#line 2190 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   host(),
															#line 2191 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   waitStateStart(WaitState{ 0 }),
															#line 2192 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   waitStateEnd(WaitState{ 2 }),
															#line 2193 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   timeStart(0),
															#line 2194 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   timeEnd(std::numeric_limits<time_t>::max()),
															#line 2195 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   seqStart(0),
															#line 2196 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   seqEnd(std::numeric_limits<int>::max()),
															#line 2198 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   beginValues(kr.begin.removePrefix(prefix).splitAny("/"_sr)),
															#line 2199 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   endValues(kr.end.removePrefix(prefix).splitAny("/"_sr))
															#line 10372 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2201 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (beginValues.size() < 2 || endValues.size() < 2)
															#line 10387 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2202 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ryw->setSpecialKeySpaceErrorMsg("missing required parameters (index, host)");
															#line 2203 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 10393 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2206 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			endRangeHost = NetworkAddress();
															#line 10397 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			try {
															#line 2208 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (SpecialKeySpace::getActorLineageApiCommandRange("state").contains(kr))
															#line 10401 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2210 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					parse(beginValues.begin() + 1, beginValues.end(), host, waitStateStart, timeStart, seqStart);
															#line 2211 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (kr.begin != kr.end)
															#line 10407 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 2212 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						parse(endValues.begin() + 1, endValues.end(), endRangeHost, waitStateEnd, timeEnd, seqEnd);
															#line 10411 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
				else
				{
															#line 2214 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (SpecialKeySpace::getActorLineageApiCommandRange("time").contains(kr))
															#line 10418 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 2216 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						parse(beginValues.begin() + 1, beginValues.end(), host, timeStart, waitStateStart, seqStart);
															#line 2217 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						if (kr.begin != kr.end)
															#line 10424 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						{
															#line 2218 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
							parse(endValues.begin() + 1, endValues.end(), endRangeHost, timeEnd, waitStateEnd, seqEnd);
															#line 10428 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
						}
					}
					else
					{
															#line 2221 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						ryw->setSpecialKeySpaceErrorMsg("invalid index in actor_lineage");
															#line 2222 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						return a_body1Catch2(special_keys_api_failure(), loopDepth);
															#line 10437 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2233 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (kr.begin != kr.end && host != endRangeHost)
															#line 10468 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2236 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg("the host must remain the same on both ends of the range");
															#line 2237 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 10474 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2242 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		process = ProcessInterface();
															#line 2243 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		process.getInterface = RequestStream<GetProcessInterfaceRequest>(Endpoint::wellKnown({ host }, WLTOKEN_PROCESS));
															#line 2244 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<ProcessInterface> __when_expr_0 = retryBrokenPromise(process.getInterface, GetProcessInterfaceRequest{});
															#line 2244 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10484 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1cont1when1(__when_expr_0.get(), loopDepth); };
		static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state = 1;
															#line 2244 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ActorLineageGetRangeActorActor, 0, ProcessInterface >*>(static_cast<ActorLineageGetRangeActorActor*>(this)));
															#line 10489 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 2225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (e.code() != special_keys_api_failure().code())
															#line 10499 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2226 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				ryw->setSpecialKeySpaceErrorMsg("failed to parse key");
															#line 2227 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 10505 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 2229 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 10511 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2245 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		process = p;
															#line 2247 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ActorLineageRequest actorLineageRequest;
															#line 2248 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.waitStateStart = waitStateStart;
															#line 2249 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.waitStateEnd = waitStateEnd;
															#line 2250 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.timeStart = timeStart;
															#line 2251 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.timeEnd = timeEnd;
															#line 2252 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<ActorLineageReply> __when_expr_1 = process.actorLineage.getReply(actorLineageRequest);
															#line 2252 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10553 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont11when1(__when_expr_1.get(), loopDepth); };
		static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state = 2;
															#line 2252 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ActorLineageGetRangeActorActor, 1, ActorLineageReply >*>(static_cast<ActorLineageGetRangeActorActor*>(this)));
															#line 10558 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont11(ProcessInterface && p,int loopDepth) 
	{
															#line 2245 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		process = p;
															#line 2247 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ActorLineageRequest actorLineageRequest;
															#line 2248 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.waitStateStart = waitStateStart;
															#line 2249 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.waitStateEnd = waitStateEnd;
															#line 2250 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.timeStart = timeStart;
															#line 2251 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		actorLineageRequest.timeEnd = timeEnd;
															#line 2252 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<ActorLineageReply> __when_expr_1 = process.actorLineage.getReply(actorLineageRequest);
															#line 2252 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 10581 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont11when1(__when_expr_1.get(), loopDepth); };
		static_cast<ActorLineageGetRangeActorActor*>(this)->actor_wait_state = 2;
															#line 2252 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ActorLineageGetRangeActorActor, 1, ActorLineageReply >*>(static_cast<ActorLineageGetRangeActorActor*>(this)));
															#line 10586 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2254 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		time_t dt = 0;
															#line 2255 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		int seq = -1;
															#line 2256 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( const auto& sample : reply.samples ) {
															#line 2257 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			time_t datetime = (time_t)sample.time;
															#line 2258 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			char buf[50];
															#line 2259 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			struct tm* tm;
															#line 2260 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			tm = localtime(&datetime);
															#line 2261 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			size_t size = strftime(buf, 50, "%FT%T%z", tm);
															#line 2262 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			std::string date(buf, size);
															#line 2264 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			seq = dt == datetime ? seq + 1 : 0;
															#line 2265 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			dt = datetime;
															#line 2267 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& [waitState, data] : sample.data ) {
															#line 2268 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (seq < seqStart)
															#line 10682 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
					continue;
				}
				else
				{
															#line 2270 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (seq >= seqEnd)
															#line 10690 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
						break;
					}
				}
															#line 2274 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				std::ostringstream streamKey;
															#line 2275 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (SpecialKeySpace::getActorLineageApiCommandRange("state").contains(kr))
															#line 10699 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2276 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("state").toString() << host.toString() << "/" << to_string(waitState) << "/" << date;
															#line 10703 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
				else
				{
															#line 2278 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (SpecialKeySpace::getActorLineageApiCommandRange("time").contains(kr))
															#line 10709 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 2279 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("time").toString() << host.toString() << "/" << date << "/" << to_string(waitState);
															#line 10713 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
					else
					{
															#line 2282 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						ASSERT(false);
															#line 10719 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
															#line 2284 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				streamKey << "/" << seq;
															#line 2286 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				msgpack::object_handle oh = msgpack::unpack(data.data(), data.size());
															#line 2287 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				msgpack::object deserialized = oh.get();
															#line 2289 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				std::ostringstream stream;
															#line 2290 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				stream << deserialized;
															#line 2292 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back_deep(result.arena(), KeyValueRef(streamKey.str(), stream.str()));
															#line 10734 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2295 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (sample.data.size() == 0)
															#line 10738 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2296 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				std::ostringstream streamKey;
															#line 2297 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (SpecialKeySpace::getActorLineageApiCommandRange("state").contains(kr))
															#line 10744 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2298 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("state").toString() << host.toString() << "/Running/" << date;
															#line 10748 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
				else
				{
															#line 2300 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (SpecialKeySpace::getActorLineageApiCommandRange("time").contains(kr))
															#line 10754 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 2301 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("time").toString() << host.toString() << "/" << date << "/Running";
															#line 10758 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
					else
					{
															#line 2304 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						ASSERT(false);
															#line 10764 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
															#line 2306 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				streamKey << "/" << seq;
															#line 2307 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back_deep(result.arena(), KeyValueRef(streamKey.str(), "{}"_sr));
															#line 10771 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 2311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ActorLineageGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~ActorLineageGetRangeActorActorState(); static_cast<ActorLineageGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 10776 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ActorLineageGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
		this->~ActorLineageGetRangeActorActorState();
		static_cast<ActorLineageGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont11cont1(ActorLineageReply && reply,int loopDepth) 
	{
															#line 2254 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		time_t dt = 0;
															#line 2255 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		int seq = -1;
															#line 2256 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( const auto& sample : reply.samples ) {
															#line 2257 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			time_t datetime = (time_t)sample.time;
															#line 2258 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			char buf[50];
															#line 2259 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			struct tm* tm;
															#line 2260 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			tm = localtime(&datetime);
															#line 2261 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			size_t size = strftime(buf, 50, "%FT%T%z", tm);
															#line 2262 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			std::string date(buf, size);
															#line 2264 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			seq = dt == datetime ? seq + 1 : 0;
															#line 2265 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			dt = datetime;
															#line 2267 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& [waitState, data] : sample.data ) {
															#line 2268 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (seq < seqStart)
															#line 10812 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
					continue;
				}
				else
				{
															#line 2270 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (seq >= seqEnd)
															#line 10820 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
						break;
					}
				}
															#line 2274 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				std::ostringstream streamKey;
															#line 2275 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (SpecialKeySpace::getActorLineageApiCommandRange("state").contains(kr))
															#line 10829 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2276 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("state").toString() << host.toString() << "/" << to_string(waitState) << "/" << date;
															#line 10833 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
				else
				{
															#line 2278 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (SpecialKeySpace::getActorLineageApiCommandRange("time").contains(kr))
															#line 10839 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 2279 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("time").toString() << host.toString() << "/" << date << "/" << to_string(waitState);
															#line 10843 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
					else
					{
															#line 2282 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						ASSERT(false);
															#line 10849 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
															#line 2284 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				streamKey << "/" << seq;
															#line 2286 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				msgpack::object_handle oh = msgpack::unpack(data.data(), data.size());
															#line 2287 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				msgpack::object deserialized = oh.get();
															#line 2289 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				std::ostringstream stream;
															#line 2290 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				stream << deserialized;
															#line 2292 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back_deep(result.arena(), KeyValueRef(streamKey.str(), stream.str()));
															#line 10864 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2295 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (sample.data.size() == 0)
															#line 10868 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2296 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				std::ostringstream streamKey;
															#line 2297 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (SpecialKeySpace::getActorLineageApiCommandRange("state").contains(kr))
															#line 10874 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2298 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("state").toString() << host.toString() << "/Running/" << date;
															#line 10878 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
				else
				{
															#line 2300 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (SpecialKeySpace::getActorLineageApiCommandRange("time").contains(kr))
															#line 10884 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					{
															#line 2301 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						streamKey << SpecialKeySpace::getActorLineageApiCommandPrefix("time").toString() << host.toString() << "/" << date << "/Running";
															#line 10888 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
					else
					{
															#line 2304 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
						ASSERT(false);
															#line 10894 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					}
				}
															#line 2306 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				streamKey << "/" << seq;
															#line 2307 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				result.push_back_deep(result.arena(), KeyValueRef(streamKey.str(), "{}"_sr));
															#line 10901 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 2311 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ActorLineageGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~ActorLineageGetRangeActorActorState(); static_cast<ActorLineageGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 10906 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef prefix;
															#line 2183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 2186 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult result;
															#line 2190 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	NetworkAddress host;
															#line 2191 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	WaitState waitStateStart;
															#line 2192 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	WaitState waitStateEnd;
															#line 2193 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	time_t timeStart;
															#line 2194 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	time_t timeEnd;
															#line 2195 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	int seqStart;
															#line 2196 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	int seqEnd;
															#line 2198 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<StringRef> beginValues;
															#line 2199 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<StringRef> endValues;
															#line 2206 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	NetworkAddress endRangeHost;
															#line 2242 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ProcessInterface process;
															#line 11007 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via actorLineageGetRangeActor()
															#line 2183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class ActorLineageGetRangeActorActor final : public Actor<RangeResult>, public ActorCallback< ActorLineageGetRangeActorActor, 0, ProcessInterface >, public ActorCallback< ActorLineageGetRangeActorActor, 1, ActorLineageReply >, public FastAllocated<ActorLineageGetRangeActorActor>, public ActorLineageGetRangeActorActorState<ActorLineageGetRangeActorActor> {
															#line 11012 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<ActorLineageGetRangeActorActor>::operator new;
	using FastAllocated<ActorLineageGetRangeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ActorLineageGetRangeActorActor, 0, ProcessInterface >;
friend struct ActorCallback< ActorLineageGetRangeActorActor, 1, ActorLineageReply >;
															#line 2183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ActorLineageGetRangeActorActor(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 11024 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<RangeResult> actorLineageGetRangeActor( ReadYourWritesTransaction* const& ryw, KeyRef const& prefix, KeyRangeRef const& kr ) {
															#line 2183 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new ActorLineageGetRangeActorActor(ryw, prefix, kr));
															#line 11053 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2313 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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
															#line 11138 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via MaintenanceGetRangeActor()
															#line 2394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class MaintenanceGetRangeActorActor>
															#line 2394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class MaintenanceGetRangeActorActorState {
															#line 11145 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	MaintenanceGetRangeActorActorState(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 2394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   prefix(prefix),
															#line 2394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr),
															#line 2397 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   result()
															#line 11158 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2399 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 2400 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 2401 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(healthyZoneKey);
															#line 2401 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<MaintenanceGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11179 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<MaintenanceGetRangeActorActor*>(this)->actor_wait_state = 1;
															#line 2401 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MaintenanceGetRangeActorActor, 0, Optional<Value> >*>(static_cast<MaintenanceGetRangeActorActor*>(this)));
															#line 11184 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2402 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 11207 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2403 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			auto healthyZone = decodeHealthyZoneValue(val.get());
															#line 2404 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if ((healthyZone.first == ignoreSSFailuresZoneString) || (healthyZone.second > ryw->getTransaction().getReadVersion().get()))
															#line 11213 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2406 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				Key zone_key = healthyZone.first.withPrefix(prefix);
															#line 2407 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				double seconds = healthyZone.first == ignoreSSFailuresZoneString ? 0 : (healthyZone.second - ryw->getTransaction().getReadVersion().get()) / CLIENT_KNOBS->CORE_VERSIONSPERSECOND;
															#line 2411 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (kr.contains(zone_key))
															#line 11221 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2412 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					result.push_back_deep(result.arena(), KeyValueRef(zone_key, Value(boost::lexical_cast<std::string>(seconds))));
															#line 11225 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 2417 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<MaintenanceGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~MaintenanceGetRangeActorActorState(); static_cast<MaintenanceGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 11231 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<MaintenanceGetRangeActorActor*>(this)->SAV< RangeResult >::value()) RangeResult(rywGetRange(ryw, kr, result));
		this->~MaintenanceGetRangeActorActorState();
		static_cast<MaintenanceGetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 2402 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (val.present())
															#line 11243 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2403 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			auto healthyZone = decodeHealthyZoneValue(val.get());
															#line 2404 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if ((healthyZone.first == ignoreSSFailuresZoneString) || (healthyZone.second > ryw->getTransaction().getReadVersion().get()))
															#line 11249 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2406 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				Key zone_key = healthyZone.first.withPrefix(prefix);
															#line 2407 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				double seconds = healthyZone.first == ignoreSSFailuresZoneString ? 0 : (healthyZone.second - ryw->getTransaction().getReadVersion().get()) / CLIENT_KNOBS->CORE_VERSIONSPERSECOND;
															#line 2411 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (kr.contains(zone_key))
															#line 11257 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2412 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					result.push_back_deep(result.arena(), KeyValueRef(zone_key, Value(boost::lexical_cast<std::string>(seconds))));
															#line 11261 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 2417 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<MaintenanceGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~MaintenanceGetRangeActorActorState(); static_cast<MaintenanceGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 11267 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef prefix;
															#line 2394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 2397 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult result;
															#line 11346 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via MaintenanceGetRangeActor()
															#line 2394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class MaintenanceGetRangeActorActor final : public Actor<RangeResult>, public ActorCallback< MaintenanceGetRangeActorActor, 0, Optional<Value> >, public FastAllocated<MaintenanceGetRangeActorActor>, public MaintenanceGetRangeActorActorState<MaintenanceGetRangeActorActor> {
															#line 11351 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<MaintenanceGetRangeActorActor>::operator new;
	using FastAllocated<MaintenanceGetRangeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MaintenanceGetRangeActorActor, 0, Optional<Value> >;
															#line 2394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	MaintenanceGetRangeActorActor(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 11362 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<RangeResult> MaintenanceGetRangeActor( ReadYourWritesTransaction* const& ryw, KeyRef const& prefix, KeyRangeRef const& kr ) {
															#line 2394 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new MaintenanceGetRangeActorActor(ryw, prefix, kr));
															#line 11390 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2419 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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
															#line 11406 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via maintenanceCommitActor()
															#line 2431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class MaintenanceCommitActorActor>
															#line 2431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class MaintenanceCommitActorActorState {
															#line 11413 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	MaintenanceCommitActorActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 2431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr)
															#line 11422 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2433 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 2434 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 2435 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 2436 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(healthyZoneKey);
															#line 2436 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<MaintenanceCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11445 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<MaintenanceCommitActorActor*>(this)->actor_wait_state = 1;
															#line 2436 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MaintenanceCommitActorActor, 0, Optional<Value> >*>(static_cast<MaintenanceCommitActorActor*>(this)));
															#line 11450 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2437 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		Optional<std::pair<Key, Version>> healthyZone = val.present() ? decodeHealthyZoneValue(val.get()) : Optional<std::pair<Key, Version>>();
															#line 2440 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(kr);
															#line 2442 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		Key zoneId;
															#line 2443 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		double seconds;
															#line 2444 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		bool isSet = false;
															#line 2448 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for(auto iter = ranges.begin();iter != ranges.end();++iter) {
															#line 2449 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!iter->value().first)
															#line 11485 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
				continue;
			}
															#line 2451 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (iter->value().second.present())
															#line 11491 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2452 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (isSet)
															#line 11495 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2453 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>(ManagementAPIError::toJsonString( false, "maintenance", "Multiple zones given for maintenance, only one allowed at the same time"))); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11499 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					new (&static_cast<MaintenanceCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>(ManagementAPIError::toJsonString( false, "maintenance", "Multiple zones given for maintenance, only one allowed at the same time")));
					this->~MaintenanceCommitActorActorState();
					static_cast<MaintenanceCommitActorActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 2455 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				isSet = true;
															#line 2456 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				zoneId = iter->begin().removePrefix(kr.begin);
															#line 2457 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				seconds = boost::lexical_cast<double>(iter->value().second.get().toString());
															#line 11511 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 2460 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (!isSet && healthyZone.present() && iter.range().contains(healthyZone.get().first.withPrefix(kr.begin)))
															#line 11517 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2461 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					ryw->getTransaction().clear(healthyZoneKey);
															#line 11521 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 2465 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (isSet)
															#line 11527 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2466 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (healthyZone.present() && healthyZone.get().first == ignoreSSFailuresZoneString)
															#line 11531 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2467 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				std::string msg = "Maintenance mode cannot be used while data distribution is disabled for storage " "server failures.";
															#line 2469 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg))); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11537 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				new (&static_cast<MaintenanceCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg)));
				this->~MaintenanceCommitActorActorState();
				static_cast<MaintenanceCommitActorActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 2470 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (seconds < 0)
															#line 11547 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2471 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					std::string msg = "The specified maintenance time " + boost::lexical_cast<std::string>(seconds) + " is a negative value";
															#line 2473 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg))); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11553 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					new (&static_cast<MaintenanceCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg)));
					this->~MaintenanceCommitActorActorState();
					static_cast<MaintenanceCommitActorActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
				else
				{
															#line 2475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					TraceEvent(SevDebug, "SKSMaintenanceSet").detail("ZoneId", zoneId.toString());
															#line 2476 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					ryw->getTransaction().set(healthyZoneKey, healthyZoneValue(zoneId, ryw->getTransaction().getReadVersion().get() + (seconds * CLIENT_KNOBS->CORE_VERSIONSPERSECOND)));
															#line 11565 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 2482 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11571 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<MaintenanceCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~MaintenanceCommitActorActorState();
		static_cast<MaintenanceCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 2437 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		Optional<std::pair<Key, Version>> healthyZone = val.present() ? decodeHealthyZoneValue(val.get()) : Optional<std::pair<Key, Version>>();
															#line 2440 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(kr);
															#line 2442 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		Key zoneId;
															#line 2443 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		double seconds;
															#line 2444 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		bool isSet = false;
															#line 2448 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for(auto iter = ranges.begin();iter != ranges.end();++iter) {
															#line 2449 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!iter->value().first)
															#line 11595 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
				continue;
			}
															#line 2451 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (iter->value().second.present())
															#line 11601 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2452 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (isSet)
															#line 11605 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2453 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>(ManagementAPIError::toJsonString( false, "maintenance", "Multiple zones given for maintenance, only one allowed at the same time"))); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11609 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					new (&static_cast<MaintenanceCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>(ManagementAPIError::toJsonString( false, "maintenance", "Multiple zones given for maintenance, only one allowed at the same time")));
					this->~MaintenanceCommitActorActorState();
					static_cast<MaintenanceCommitActorActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 2455 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				isSet = true;
															#line 2456 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				zoneId = iter->begin().removePrefix(kr.begin);
															#line 2457 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				seconds = boost::lexical_cast<double>(iter->value().second.get().toString());
															#line 11621 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 2460 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (!isSet && healthyZone.present() && iter.range().contains(healthyZone.get().first.withPrefix(kr.begin)))
															#line 11627 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2461 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					ryw->getTransaction().clear(healthyZoneKey);
															#line 11631 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 2465 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (isSet)
															#line 11637 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2466 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (healthyZone.present() && healthyZone.get().first == ignoreSSFailuresZoneString)
															#line 11641 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2467 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				std::string msg = "Maintenance mode cannot be used while data distribution is disabled for storage " "server failures.";
															#line 2469 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg))); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11647 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				new (&static_cast<MaintenanceCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg)));
				this->~MaintenanceCommitActorActorState();
				static_cast<MaintenanceCommitActorActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 2470 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (seconds < 0)
															#line 11657 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2471 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					std::string msg = "The specified maintenance time " + boost::lexical_cast<std::string>(seconds) + " is a negative value";
															#line 2473 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg))); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11663 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					new (&static_cast<MaintenanceCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>(ManagementAPIError::toJsonString(false, "maintenance", msg)));
					this->~MaintenanceCommitActorActorState();
					static_cast<MaintenanceCommitActorActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
				else
				{
															#line 2475 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					TraceEvent(SevDebug, "SKSMaintenanceSet").detail("ZoneId", zoneId.toString());
															#line 2476 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					ryw->getTransaction().set(healthyZoneKey, healthyZoneValue(zoneId, ryw->getTransaction().getReadVersion().get() + (seconds * CLIENT_KNOBS->CORE_VERSIONSPERSECOND)));
															#line 11675 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 2482 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<MaintenanceCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~MaintenanceCommitActorActorState(); static_cast<MaintenanceCommitActorActor*>(this)->destroy(); return 0; }
															#line 11681 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 2440 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeMap<Key, std::pair<bool, Optional<Value>>, KeyRangeRef>::Ranges ranges;
															#line 11758 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via maintenanceCommitActor()
															#line 2431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class MaintenanceCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< MaintenanceCommitActorActor, 0, Optional<Value> >, public FastAllocated<MaintenanceCommitActorActor>, public MaintenanceCommitActorActorState<MaintenanceCommitActorActor> {
															#line 11763 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<MaintenanceCommitActorActor>::operator new;
	using FastAllocated<MaintenanceCommitActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MaintenanceCommitActorActor, 0, Optional<Value> >;
															#line 2431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	MaintenanceCommitActorActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr) 
															#line 11774 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<Optional<std::string>> maintenanceCommitActor( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr ) {
															#line 2431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new MaintenanceCommitActorActor(ryw, kr));
															#line 11802 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2484 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

Future<Optional<std::string>> MaintenanceImpl::commit(ReadYourWritesTransaction* ryw) {
	return maintenanceCommitActor(ryw, getKeyRange());
}

DataDistributionImpl::DataDistributionImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

// Read the system keys dataDistributionModeKey and rebalanceDDIgnoreKey
															#line 11814 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via DataDistributionGetRangeActor()
															#line 2492 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class DataDistributionGetRangeActorActor>
															#line 2492 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class DataDistributionGetRangeActorActorState {
															#line 11821 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2492 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	DataDistributionGetRangeActorActorState(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 2492 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2492 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2492 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   prefix(prefix),
															#line 2492 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr),
															#line 2495 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   result(),
															#line 2497 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   modeKey(LiteralStringRef("mode").withPrefix(prefix))
															#line 11836 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2499 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->getTransaction().setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 2501 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.contains(modeKey))
															#line 11853 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2502 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				auto entry = ryw->getSpecialKeySpaceWriteMap()[modeKey];
															#line 2503 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (ryw->readYourWritesDisabled() || !entry.first)
															#line 11859 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2504 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					StrictFuture<Optional<Value>> __when_expr_0 = ryw->getTransaction().get(dataDistributionModeKey);
															#line 2504 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					if (static_cast<DataDistributionGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<DataDistributionGetRangeActorActor*>(this)->actor_wait_state = 1;
															#line 2504 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DataDistributionGetRangeActorActor, 0, Optional<Value> >*>(static_cast<DataDistributionGetRangeActorActor*>(this)));
															#line 11870 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2513 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		rebalanceIgnoredKey = LiteralStringRef("rebalance_ignored").withPrefix(prefix);
															#line 2514 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (kr.contains(rebalanceIgnoredKey))
															#line 11905 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2515 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			auto entry = ryw->getSpecialKeySpaceWriteMap()[rebalanceIgnoredKey];
															#line 2516 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (ryw->readYourWritesDisabled() || !entry.first)
															#line 11911 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2517 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				StrictFuture<Optional<Value>> __when_expr_1 = ryw->getTransaction().get(rebalanceDDIgnoreKey);
															#line 2517 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (static_cast<DataDistributionGetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 11917 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
				static_cast<DataDistributionGetRangeActorActor*>(this)->actor_wait_state = 2;
															#line 2517 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DataDistributionGetRangeActorActor, 1, Optional<Value> >*>(static_cast<DataDistributionGetRangeActorActor*>(this)));
															#line 11922 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2505 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		int mode = -1;
															#line 2506 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (f.present())
															#line 11949 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2507 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			mode = BinaryReader::fromStringRef<int>(f.get(), Unversioned());
															#line 11953 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2509 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		result.push_back_deep(result.arena(), KeyValueRef(modeKey, Value(boost::lexical_cast<std::string>(mode))));
															#line 11957 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Optional<Value> && f,int loopDepth) 
	{
															#line 2505 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		int mode = -1;
															#line 2506 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (f.present())
															#line 11968 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2507 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			mode = BinaryReader::fromStringRef<int>(f.get(), Unversioned());
															#line 11972 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2509 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		result.push_back_deep(result.arena(), KeyValueRef(modeKey, Value(boost::lexical_cast<std::string>(mode))));
															#line 11976 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2523 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<DataDistributionGetRangeActorActor*>(this)->SAV<RangeResult>::futures) { (void)(rywGetRange(ryw, kr, result)); this->~DataDistributionGetRangeActorActorState(); static_cast<DataDistributionGetRangeActorActor*>(this)->destroy(); return 0; }
															#line 12048 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2518 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (f.present())
															#line 12066 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2519 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(rebalanceIgnoredKey, Value()));
															#line 12070 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont8(Optional<Value> && f,int loopDepth) 
	{
															#line 2518 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (f.present())
															#line 12080 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2519 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(rebalanceIgnoredKey, Value()));
															#line 12084 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2492 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2492 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef prefix;
															#line 2492 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 2495 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult result;
															#line 2497 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Key modeKey;
															#line 2513 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Key rebalanceIgnoredKey;
															#line 12165 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via DataDistributionGetRangeActor()
															#line 2492 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class DataDistributionGetRangeActorActor final : public Actor<RangeResult>, public ActorCallback< DataDistributionGetRangeActorActor, 0, Optional<Value> >, public ActorCallback< DataDistributionGetRangeActorActor, 1, Optional<Value> >, public FastAllocated<DataDistributionGetRangeActorActor>, public DataDistributionGetRangeActorActorState<DataDistributionGetRangeActorActor> {
															#line 12170 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<DataDistributionGetRangeActorActor>::operator new;
	using FastAllocated<DataDistributionGetRangeActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DataDistributionGetRangeActorActor, 0, Optional<Value> >;
friend struct ActorCallback< DataDistributionGetRangeActorActor, 1, Optional<Value> >;
															#line 2492 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	DataDistributionGetRangeActorActor(ReadYourWritesTransaction* const& ryw,KeyRef const& prefix,KeyRangeRef const& kr) 
															#line 12182 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2492 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] static Future<RangeResult> DataDistributionGetRangeActor( ReadYourWritesTransaction* const& ryw, KeyRef const& prefix, KeyRangeRef const& kr ) {
															#line 2492 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new DataDistributionGetRangeActorActor(ryw, prefix, kr));
															#line 12211 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2525 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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
	Key modeKey = LiteralStringRef("mode").withPrefix(kr.begin);
	Key rebalanceIgnoredKey = LiteralStringRef("rebalance_ignored").withPrefix(kr.begin);
	auto ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(kr);
	for (auto iter = ranges.begin(); iter != ranges.end(); ++iter) {
		if (!iter->value().first)
			continue;
		if (iter->value().second.present()) {
			if (iter->range() == singleKeyRange(modeKey)) {
				try {
					int mode = boost::lexical_cast<int>(iter->value().second.get().toString());
					Value modeVal = BinaryWriter::toValue(mode, Unversioned());
					if (mode == 0 || mode == 1) {
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
				if (iter->value().second.get().size())
					msg =
					    ManagementAPIError::toJsonString(false,
					                                     "datadistribution",
					                                     "Value is unused for the data_distribution/rebalance_ignored "
					                                     "key, please set it to an empty value");
				else
					ryw->getTransaction().set(rebalanceDDIgnoreKey, LiteralStringRef("on"));
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
															#line 12371 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via excludeLocalityCommitActor()
															#line 2681 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class ExcludeLocalityCommitActorActor>
															#line 2681 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class ExcludeLocalityCommitActorActorState {
															#line 12378 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2681 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ExcludeLocalityCommitActorActorState(ReadYourWritesTransaction* const& ryw,bool const& failed) 
															#line 2681 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2681 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2681 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   failed(failed),
															#line 2682 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   result(),
															#line 2683 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   localities(),
															#line 2684 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   addresses(),
															#line 2685 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   exclusions()
															#line 12395 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2687 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 2689 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<std::vector<ProcessData>> __when_expr_0 = getWorkers(&ryw->getTransaction());
															#line 2689 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12414 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state = 1;
															#line 2689 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ExcludeLocalityCommitActorActor, 0, std::vector<ProcessData> >*>(static_cast<ExcludeLocalityCommitActorActor*>(this)));
															#line 12419 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2690 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!parseLocalitiesFromKeys(ryw, failed, localities, addresses, exclusions, workers, result))
															#line 12442 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2691 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeLocalityCommitActorActorState(); static_cast<ExcludeLocalityCommitActorActor*>(this)->destroy(); return 0; }
															#line 12446 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			new (&static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(result)); // state_var_RVO
			this->~ExcludeLocalityCommitActorActorState();
			static_cast<ExcludeLocalityCommitActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 2693 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		auto force = ryw->getSpecialKeySpaceWriteMap()[SpecialKeySpace::getManagementApiCommandOptionSpecialKey( failed ? "failed_locality" : "excluded_locality", "force")];
															#line 2696 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (localities.size() && !(force.first && force.second.present()))
															#line 12456 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2697 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<bool> __when_expr_1 = checkExclusion(ryw->getDatabase(), &addresses, &exclusions, failed, &result);
															#line 2697 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12462 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state = 2;
															#line 2697 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ExcludeLocalityCommitActorActor, 1, bool >*>(static_cast<ExcludeLocalityCommitActorActor*>(this)));
															#line 12467 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2689 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		workers = __workers;
															#line 12481 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2702 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Void> __when_expr_2 = excludeLocalities(&ryw->getTransaction(), localities, failed);
															#line 2702 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12550 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<ExcludeLocalityCommitActorActor*>(this)->actor_wait_state = 3;
															#line 2702 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ExcludeLocalityCommitActorActor, 2, Void >*>(static_cast<ExcludeLocalityCommitActorActor*>(this)));
															#line 12555 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont4(bool const& safe,int loopDepth) 
	{
															#line 2698 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!safe)
															#line 12564 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2699 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeLocalityCommitActorActorState(); static_cast<ExcludeLocalityCommitActorActor*>(this)->destroy(); return 0; }
															#line 12568 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2698 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!safe)
															#line 12582 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2699 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeLocalityCommitActorActorState(); static_cast<ExcludeLocalityCommitActorActor*>(this)->destroy(); return 0; }
															#line 12586 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2703 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		includeLocalities(ryw);
															#line 2705 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeLocalityCommitActorActorState(); static_cast<ExcludeLocalityCommitActorActor*>(this)->destroy(); return 0; }
															#line 12665 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(std::move(result)); // state_var_RVO
		this->~ExcludeLocalityCommitActorActorState();
		static_cast<ExcludeLocalityCommitActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 2703 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		includeLocalities(ryw);
															#line 2705 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ExcludeLocalityCommitActorActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(result); this->~ExcludeLocalityCommitActorActorState(); static_cast<ExcludeLocalityCommitActorActor*>(this)->destroy(); return 0; }
															#line 12679 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2681 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2681 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	bool failed;
															#line 2682 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Optional<std::string> result;
															#line 2683 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::unordered_set<std::string> localities;
															#line 2684 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<AddressExclusion> addresses;
															#line 2685 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::set<AddressExclusion> exclusions;
															#line 2689 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	std::vector<ProcessData> workers;
															#line 12764 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via excludeLocalityCommitActor()
															#line 2681 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class ExcludeLocalityCommitActorActor final : public Actor<Optional<std::string>>, public ActorCallback< ExcludeLocalityCommitActorActor, 0, std::vector<ProcessData> >, public ActorCallback< ExcludeLocalityCommitActorActor, 1, bool >, public ActorCallback< ExcludeLocalityCommitActorActor, 2, Void >, public FastAllocated<ExcludeLocalityCommitActorActor>, public ExcludeLocalityCommitActorActorState<ExcludeLocalityCommitActorActor> {
															#line 12769 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2681 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ExcludeLocalityCommitActorActor(ReadYourWritesTransaction* const& ryw,bool const& failed) 
															#line 12782 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2681 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Optional<std::string>> excludeLocalityCommitActor( ReadYourWritesTransaction* const& ryw, bool const& failed ) {
															#line 2681 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Optional<std::string>>(new ExcludeLocalityCommitActorActor(ryw, failed));
															#line 12812 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2707 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

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
	    .withPrefix(LiteralStringRef("\xff/conf/"));
}

Key ExcludedLocalitiesRangeImpl::encode(const KeyRef& key) const {
	return key.removePrefix(LiteralStringRef("\xff/conf/"))
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
	    .withPrefix(LiteralStringRef("\xff/conf/"));
}

Key FailedLocalitiesRangeImpl::encode(const KeyRef& key) const {
	return key.removePrefix(LiteralStringRef("\xff/conf/"))
	    .withPrefix(SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin);
}

Future<Optional<std::string>> FailedLocalitiesRangeImpl::commit(ReadYourWritesTransaction* ryw) {
	// exclude locality with failed option as true.
	return excludeLocalityCommitActor(ryw, true);
}

															#line 12875 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via getTenantList()
															#line 2766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class GetTenantListActor>
															#line 2766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetTenantListActorState {
															#line 12882 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetTenantListActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr,GetRangeLimits const& limitsHint) 
															#line 2766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   kr(kr),
															#line 2766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   limitsHint(limitsHint),
															#line 2767 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   managementPrefix(kr.begin.substr(0, SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin.size() + TenantMapRangeImpl::submoduleRange.begin.size()))
															#line 12895 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2772 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			kr = kr.removePrefix(SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin);
															#line 2773 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			TenantNameRef beginTenant = kr.begin.removePrefix(TenantMapRangeImpl::submoduleRange.begin);
															#line 2775 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			TenantNameRef endTenant = kr.end;
															#line 2776 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (endTenant.startsWith(TenantMapRangeImpl::submoduleRange.begin))
															#line 12916 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2777 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				endTenant = endTenant.removePrefix(TenantMapRangeImpl::submoduleRange.begin);
															#line 12920 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
			else
			{
															#line 2779 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				endTenant = "\xff"_sr;
															#line 12926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2782 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<std::map<TenantName, TenantMapEntry>> __when_expr_0 = ManagementAPI::listTenantsTransaction(&ryw->getTransaction(), beginTenant, endTenant, limitsHint.rows);
															#line 2782 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<GetTenantListActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 12932 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetTenantListActor*>(this)->actor_wait_state = 1;
															#line 2782 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetTenantListActor, 0, std::map<TenantName, TenantMapEntry> >*>(static_cast<GetTenantListActor*>(this)));
															#line 12937 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
	int a_body1cont1(std::map<TenantName, TenantMapEntry> const& tenants,int loopDepth) 
	{
															#line 2785 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult results;
															#line 2786 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto tenant : tenants ) {
															#line 2787 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			json_spirit::mObject tenantEntry;
															#line 2788 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			tenantEntry["id"] = tenant.second.id;
															#line 2789 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			tenantEntry["prefix"] = tenant.second.prefix.toString();
															#line 2790 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			std::string tenantEntryString = json_spirit::write_string(json_spirit::mValue(tenantEntry));
															#line 2791 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ValueRef tenantEntryBytes(results.arena(), tenantEntryString);
															#line 2792 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			results.push_back(results.arena(), KeyValueRef(tenant.first.withPrefix(managementPrefix, results.arena()), tenantEntryBytes));
															#line 12974 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2796 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetTenantListActor*>(this)->SAV<RangeResult>::futures) { (void)(results); this->~GetTenantListActorState(); static_cast<GetTenantListActor*>(this)->destroy(); return 0; }
															#line 12978 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetTenantListActor*>(this)->SAV< RangeResult >::value()) RangeResult(results);
		this->~GetTenantListActorState();
		static_cast<GetTenantListActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::map<TenantName, TenantMapEntry> && tenants,int loopDepth) 
	{
															#line 2785 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		RangeResult results;
															#line 2786 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto tenant : tenants ) {
															#line 2787 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			json_spirit::mObject tenantEntry;
															#line 2788 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			tenantEntry["id"] = tenant.second.id;
															#line 2789 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			tenantEntry["prefix"] = tenant.second.prefix.toString();
															#line 2790 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			std::string tenantEntryString = json_spirit::write_string(json_spirit::mValue(tenantEntry));
															#line 2791 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ValueRef tenantEntryBytes(results.arena(), tenantEntryString);
															#line 2792 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			results.push_back(results.arena(), KeyValueRef(tenant.first.withPrefix(managementPrefix, results.arena()), tenantEntryBytes));
															#line 13004 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2796 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<GetTenantListActor*>(this)->SAV<RangeResult>::futures) { (void)(results); this->~GetTenantListActorState(); static_cast<GetTenantListActor*>(this)->destroy(); return 0; }
															#line 13008 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<GetTenantListActor*>(this)->SAV< RangeResult >::value()) RangeResult(results);
		this->~GetTenantListActorState();
		static_cast<GetTenantListActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::map<TenantName, TenantMapEntry> const& tenants,int loopDepth) 
	{
		loopDepth = a_body1cont1(tenants, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::map<TenantName, TenantMapEntry> && tenants,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(tenants), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetTenantListActor*>(this)->actor_wait_state > 0) static_cast<GetTenantListActor*>(this)->actor_wait_state = 0;
		static_cast<GetTenantListActor*>(this)->ActorCallback< GetTenantListActor, 0, std::map<TenantName, TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< GetTenantListActor, 0, std::map<TenantName, TenantMapEntry> >*,std::map<TenantName, TenantMapEntry> const& value) 
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
	void a_callback_fire(ActorCallback< GetTenantListActor, 0, std::map<TenantName, TenantMapEntry> >*,std::map<TenantName, TenantMapEntry> && value) 
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
	void a_callback_error(ActorCallback< GetTenantListActor, 0, std::map<TenantName, TenantMapEntry> >*,Error err) 
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
															#line 2766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRangeRef kr;
															#line 2766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetRangeLimits limitsHint;
															#line 2767 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeyRef managementPrefix;
															#line 13087 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via getTenantList()
															#line 2766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class GetTenantListActor final : public Actor<RangeResult>, public ActorCallback< GetTenantListActor, 0, std::map<TenantName, TenantMapEntry> >, public FastAllocated<GetTenantListActor>, public GetTenantListActorState<GetTenantListActor> {
															#line 13092 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<GetTenantListActor>::operator new;
	using FastAllocated<GetTenantListActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetTenantListActor, 0, std::map<TenantName, TenantMapEntry> >;
															#line 2766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetTenantListActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr,GetRangeLimits const& limitsHint) 
															#line 13103 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<RangeResult>(),
		   GetTenantListActorState<GetTenantListActor>(ryw, kr, limitsHint)
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
		case 1: this->a_callback_error((ActorCallback< GetTenantListActor, 0, std::map<TenantName, TenantMapEntry> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<RangeResult> getTenantList( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr, GetRangeLimits const& limitsHint ) {
															#line 2766 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<RangeResult>(new GetTenantListActor(ryw, kr, limitsHint));
															#line 13131 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2798 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

TenantMapRangeImpl::TenantMapRangeImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

Future<RangeResult> TenantMapRangeImpl::getRange(ReadYourWritesTransaction* ryw,
                                                 KeyRangeRef kr,
                                                 GetRangeLimits limitsHint) const {
	return getTenantList(ryw, kr, limitsHint);
}

															#line 13144 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via deleteTenantRange()
															#line 2807 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class DeleteTenantRangeActor>
															#line 2807 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class DeleteTenantRangeActorState {
															#line 13151 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2807 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	DeleteTenantRangeActorState(ReadYourWritesTransaction* const& ryw,TenantName const& beginTenant,TenantName const& endTenant) 
															#line 2807 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2807 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2807 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   beginTenant(beginTenant),
															#line 2807 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   endTenant(endTenant)
															#line 13162 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
	{
		fdb_probe_actor_create("deleteTenantRange", reinterpret_cast<unsigned long>(this));

	}
	~DeleteTenantRangeActorState() 
	{
		fdb_probe_actor_destroy("deleteTenantRange", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 2808 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			StrictFuture<std::map<TenantName, TenantMapEntry>> __when_expr_0 = ManagementAPI::listTenantsTransaction(&ryw->getTransaction(), beginTenant, endTenant, CLIENT_KNOBS->TOO_MANY);
															#line 2808 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13179 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state = 1;
															#line 2808 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantRangeActor, 0, std::map<TenantName, TenantMapEntry> >*>(static_cast<DeleteTenantRangeActor*>(this)));
															#line 13184 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
		this->~DeleteTenantRangeActorState();
		static_cast<DeleteTenantRangeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::map<TenantName, TenantMapEntry> const& tenants,int loopDepth) 
	{
															#line 2811 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (tenants.size() == CLIENT_KNOBS->TOO_MANY)
															#line 13207 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2812 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			TraceEvent(SevWarn, "DeleteTenantRangeTooLange") .detail("BeginTenant", beginTenant) .detail("EndTenant", endTenant);
															#line 2815 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg("too many tenants to range delete");
															#line 2816 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 13215 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2819 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		std::vector<Future<Void>> deleteFutures;
															#line 2820 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto tenant : tenants ) {
															#line 2821 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			deleteFutures.push_back(ManagementAPI::deleteTenantTransaction(&ryw->getTransaction(), tenant.first));
															#line 13223 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2824 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Void> __when_expr_1 = waitForAll(deleteFutures);
															#line 2824 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13229 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state = 2;
															#line 2824 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantRangeActor, 1, Void >*>(static_cast<DeleteTenantRangeActor*>(this)));
															#line 13234 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::map<TenantName, TenantMapEntry> && tenants,int loopDepth) 
	{
															#line 2811 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (tenants.size() == CLIENT_KNOBS->TOO_MANY)
															#line 13243 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2812 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			TraceEvent(SevWarn, "DeleteTenantRangeTooLange") .detail("BeginTenant", beginTenant) .detail("EndTenant", endTenant);
															#line 2815 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ryw->setSpecialKeySpaceErrorMsg("too many tenants to range delete");
															#line 2816 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 13251 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2819 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		std::vector<Future<Void>> deleteFutures;
															#line 2820 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( auto tenant : tenants ) {
															#line 2821 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			deleteFutures.push_back(ManagementAPI::deleteTenantTransaction(&ryw->getTransaction(), tenant.first));
															#line 13259 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2824 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<Void> __when_expr_1 = waitForAll(deleteFutures);
															#line 2824 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13265 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state = 2;
															#line 2824 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantRangeActor, 1, Void >*>(static_cast<DeleteTenantRangeActor*>(this)));
															#line 13270 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(std::map<TenantName, TenantMapEntry> const& tenants,int loopDepth) 
	{
		loopDepth = a_body1cont1(tenants, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::map<TenantName, TenantMapEntry> && tenants,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(tenants), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantRangeActor*>(this)->ActorCallback< DeleteTenantRangeActor, 0, std::map<TenantName, TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantRangeActor, 0, std::map<TenantName, TenantMapEntry> >*,std::map<TenantName, TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("deleteTenantRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DeleteTenantRangeActor, 0, std::map<TenantName, TenantMapEntry> >*,std::map<TenantName, TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("deleteTenantRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DeleteTenantRangeActor, 0, std::map<TenantName, TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenantRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantRange", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 2825 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<DeleteTenantRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteTenantRangeActorState(); static_cast<DeleteTenantRangeActor*>(this)->destroy(); return 0; }
															#line 13342 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<DeleteTenantRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteTenantRangeActorState();
		static_cast<DeleteTenantRangeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 2825 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<DeleteTenantRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteTenantRangeActorState(); static_cast<DeleteTenantRangeActor*>(this)->destroy(); return 0; }
															#line 13354 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<DeleteTenantRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteTenantRangeActorState();
		static_cast<DeleteTenantRangeActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantRangeActor*>(this)->ActorCallback< DeleteTenantRangeActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantRangeActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteTenantRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantRange", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DeleteTenantRangeActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteTenantRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantRange", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DeleteTenantRangeActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenantRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantRange", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 2807 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2807 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	TenantName beginTenant;
															#line 2807 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	TenantName endTenant;
															#line 13431 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via deleteTenantRange()
															#line 2807 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class DeleteTenantRangeActor final : public Actor<Void>, public ActorCallback< DeleteTenantRangeActor, 0, std::map<TenantName, TenantMapEntry> >, public ActorCallback< DeleteTenantRangeActor, 1, Void >, public FastAllocated<DeleteTenantRangeActor>, public DeleteTenantRangeActorState<DeleteTenantRangeActor> {
															#line 13436 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<DeleteTenantRangeActor>::operator new;
	using FastAllocated<DeleteTenantRangeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DeleteTenantRangeActor, 0, std::map<TenantName, TenantMapEntry> >;
friend struct ActorCallback< DeleteTenantRangeActor, 1, Void >;
															#line 2807 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	DeleteTenantRangeActor(ReadYourWritesTransaction* const& ryw,TenantName const& beginTenant,TenantName const& endTenant) 
															#line 13448 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		 : Actor<Void>(),
		   DeleteTenantRangeActorState<DeleteTenantRangeActor>(ryw, beginTenant, endTenant)
	{
		fdb_probe_actor_enter("deleteTenantRange", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("deleteTenantRange");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("deleteTenantRange", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DeleteTenantRangeActor, 0, std::map<TenantName, TenantMapEntry> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DeleteTenantRangeActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 2807 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Void> deleteTenantRange( ReadYourWritesTransaction* const& ryw, TenantName const& beginTenant, TenantName const& endTenant ) {
															#line 2807 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Void>(new DeleteTenantRangeActor(ryw, beginTenant, endTenant));
															#line 13477 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2827 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"

Future<Optional<std::string>> TenantMapRangeImpl::commit(ReadYourWritesTransaction* ryw) {
	auto ranges = ryw->getSpecialKeySpaceWriteMap().containedRanges(range);
	std::vector<Future<Void>> tenantManagementFutures;
	for (auto range : ranges) {
		if (!range.value().first) {
			continue;
		}

		TenantNameRef tenantName =
		    range.begin()
		        .removePrefix(SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin)
		        .removePrefix(TenantMapRangeImpl::submoduleRange.begin);

		if (range.value().second.present()) {
			tenantManagementFutures.push_back(
			    success(ManagementAPI::createTenantTransaction(&ryw->getTransaction(), tenantName)));
		} else {
			// For a single key clear, just issue the delete
			if (KeyRangeRef(range.begin(), range.end()).singleKeyRange()) {
				tenantManagementFutures.push_back(
				    ManagementAPI::deleteTenantTransaction(&ryw->getTransaction(), tenantName));
			} else {
				TenantNameRef endTenant = range.end().removePrefix(
				    SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin);
				if (endTenant.startsWith(submoduleRange.begin)) {
					endTenant = endTenant.removePrefix(submoduleRange.begin);
				} else {
					endTenant = "\xff"_sr;
				}
				tenantManagementFutures.push_back(deleteTenantRange(ryw, tenantName, endTenant));
			}
		}
	}

	return tag(waitForAll(tenantManagementFutures), Optional<std::string>());
}

															#line 13519 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
namespace {
// This generated class is to be used only via validateSpecialSubrangeRead()
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
template <class ValidateSpecialSubrangeReadActor>
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class ValidateSpecialSubrangeReadActorState {
															#line 13526 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ValidateSpecialSubrangeReadActorState(ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Reverse const& reverse,RangeResult const& result) 
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		 : ryw(ryw),
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   begin(begin),
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   end(end),
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   limits(limits),
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   reverse(reverse),
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		   result(result)
															#line 13543 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2871 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (!result.size())
															#line 13558 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2872 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				StrictFuture<RangeResult> __when_expr_0 = ryw->getRange(begin, end, limits, Snapshot::True, reverse);
															#line 2872 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (static_cast<ValidateSpecialSubrangeReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13564 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ValidateSpecialSubrangeReadActor*>(this)->actor_wait_state = 1;
															#line 2872 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ValidateSpecialSubrangeReadActor, 0, RangeResult >*>(static_cast<ValidateSpecialSubrangeReadActor*>(this)));
															#line 13569 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2877 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (reverse)
															#line 13597 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2878 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(std::is_sorted(result.begin(), result.end(), KeyValueRef::OrderByKeyBack{}));
															#line 13601 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
		else
		{
															#line 2880 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(std::is_sorted(result.begin(), result.end(), KeyValueRef::OrderByKey{}));
															#line 13607 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2885 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		std::vector<Key> candidateKeys;
															#line 2886 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (reverse)
															#line 13613 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2887 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for(int i = result.size() - 1;i >= 0;--i) {
															#line 2888 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				candidateKeys.emplace_back(result[i].key);
															#line 2889 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (i - 1 >= 0)
															#line 13621 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2890 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					candidateKeys.emplace_back(keyBetween(KeyRangeRef(result[i].key, result[i - 1].key)));
															#line 13625 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
		else
		{
															#line 2894 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for(int i = 0;i < result.size();++i) {
															#line 2895 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				candidateKeys.emplace_back(result[i].key);
															#line 2896 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				if (i + 1 < result.size())
															#line 13637 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				{
															#line 2897 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
					candidateKeys.emplace_back(keyBetween(KeyRangeRef(result[i].key, result[i + 1].key)));
															#line 13641 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
				}
			}
		}
															#line 2901 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		std::sort(candidateKeys.begin(), candidateKeys.end());
															#line 2902 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		int originalSize = candidateKeys.size();
															#line 2904 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for(int i = 0;i < originalSize - 1;++i) {
															#line 2905 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			candidateKeys.emplace_back(keyBetween(KeyRangeRef(candidateKeys[i], candidateKeys[i + 1])));
															#line 13653 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2907 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		std::vector<Key> keys;
															#line 2908 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		keys = { deterministicRandom()->randomChoice(candidateKeys), deterministicRandom()->randomChoice(candidateKeys) };
															#line 2909 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		std::sort(keys.begin(), keys.end());
															#line 2910 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		testBegin = firstGreaterOrEqual(keys[0]);
															#line 2911 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		testEnd = firstGreaterOrEqual(keys[1]);
															#line 2915 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		expectedResult = RangeResult();
															#line 2918 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		for( const auto& kr : result ) {
															#line 2919 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			if (kr.key >= keys[0] && kr.key < keys[1])
															#line 13671 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			{
															#line 2920 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				expectedResult.push_back(expectedResult.arena(), kr);
															#line 13675 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
		}
															#line 2925 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = ryw->getRange(testBegin, testEnd, limits, Snapshot::True, reverse);
															#line 2925 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (static_cast<ValidateSpecialSubrangeReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 13682 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ValidateSpecialSubrangeReadActor*>(this)->actor_wait_state = 2;
															#line 2925 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ValidateSpecialSubrangeReadActor, 1, RangeResult >*>(static_cast<ValidateSpecialSubrangeReadActor*>(this)));
															#line 13687 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(RangeResult const& testResult,int loopDepth) 
	{
															#line 2873 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(testResult == result);
															#line 2874 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ValidateSpecialSubrangeReadActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ValidateSpecialSubrangeReadActorState(); static_cast<ValidateSpecialSubrangeReadActor*>(this)->destroy(); return 0; }
															#line 13698 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ValidateSpecialSubrangeReadActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ValidateSpecialSubrangeReadActorState();
		static_cast<ValidateSpecialSubrangeReadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(RangeResult && testResult,int loopDepth) 
	{
															#line 2873 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		ASSERT(testResult == result);
															#line 2874 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ValidateSpecialSubrangeReadActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ValidateSpecialSubrangeReadActorState(); static_cast<ValidateSpecialSubrangeReadActor*>(this)->destroy(); return 0; }
															#line 13712 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (testResult != expectedResult)
															#line 13787 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2927 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Reverse: {}\n", reverse);
															#line 2928 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Original range: [{}, {})\n", begin.toString(), end.toString());
															#line 2929 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Original result:\n");
															#line 2930 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& kr : result ) {
															#line 2931 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				fmt::print("	{} -> {}\n", kr.key.printable(), kr.value.printable());
															#line 13799 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2933 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Test range: [{}, {})\n", testBegin.getKey().printable(), testEnd.getKey().printable());
															#line 2934 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Expected:\n");
															#line 2935 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& kr : expectedResult ) {
															#line 2936 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				fmt::print("	{} -> {}\n", kr.key.printable(), kr.value.printable());
															#line 13809 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2938 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Got:\n");
															#line 2939 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& kr : testResult ) {
															#line 2940 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				fmt::print("	{} -> {}\n", kr.key.printable(), kr.value.printable());
															#line 13817 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2942 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(testResult == expectedResult);
															#line 13821 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2944 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ValidateSpecialSubrangeReadActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ValidateSpecialSubrangeReadActorState(); static_cast<ValidateSpecialSubrangeReadActor*>(this)->destroy(); return 0; }
															#line 13825 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		new (&static_cast<ValidateSpecialSubrangeReadActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ValidateSpecialSubrangeReadActorState();
		static_cast<ValidateSpecialSubrangeReadActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(RangeResult && testResult,int loopDepth) 
	{
															#line 2926 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (testResult != expectedResult)
															#line 13837 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		{
															#line 2927 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Reverse: {}\n", reverse);
															#line 2928 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Original range: [{}, {})\n", begin.toString(), end.toString());
															#line 2929 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Original result:\n");
															#line 2930 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& kr : result ) {
															#line 2931 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				fmt::print("	{} -> {}\n", kr.key.printable(), kr.value.printable());
															#line 13849 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2933 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Test range: [{}, {})\n", testBegin.getKey().printable(), testEnd.getKey().printable());
															#line 2934 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Expected:\n");
															#line 2935 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& kr : expectedResult ) {
															#line 2936 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				fmt::print("	{} -> {}\n", kr.key.printable(), kr.value.printable());
															#line 13859 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2938 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			fmt::print("Got:\n");
															#line 2939 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			for( const auto& kr : testResult ) {
															#line 2940 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
				fmt::print("	{} -> {}\n", kr.key.printable(), kr.value.printable());
															#line 13867 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
			}
															#line 2942 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
			ASSERT(testResult == expectedResult);
															#line 13871 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
		}
															#line 2944 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
		if (!static_cast<ValidateSpecialSubrangeReadActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ValidateSpecialSubrangeReadActorState(); static_cast<ValidateSpecialSubrangeReadActor*>(this)->destroy(); return 0; }
															#line 13875 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector begin;
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector end;
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	GetRangeLimits limits;
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	Reverse reverse;
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult result;
															#line 2910 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector testBegin;
															#line 2911 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	KeySelector testEnd;
															#line 2915 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	RangeResult expectedResult;
															#line 13964 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
};
// This generated class is to be used only via validateSpecialSubrangeRead()
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
class ValidateSpecialSubrangeReadActor final : public Actor<Void>, public ActorCallback< ValidateSpecialSubrangeReadActor, 0, RangeResult >, public ActorCallback< ValidateSpecialSubrangeReadActor, 1, RangeResult >, public FastAllocated<ValidateSpecialSubrangeReadActor>, public ValidateSpecialSubrangeReadActorState<ValidateSpecialSubrangeReadActor> {
															#line 13969 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
public:
	using FastAllocated<ValidateSpecialSubrangeReadActor>::operator new;
	using FastAllocated<ValidateSpecialSubrangeReadActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ValidateSpecialSubrangeReadActor, 0, RangeResult >;
friend struct ActorCallback< ValidateSpecialSubrangeReadActor, 1, RangeResult >;
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	ValidateSpecialSubrangeReadActor(ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Reverse const& reverse,RangeResult const& result) 
															#line 13981 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
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
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
[[nodiscard]] Future<Void> validateSpecialSubrangeRead( ReadYourWritesTransaction* const& ryw, KeySelector const& begin, KeySelector const& end, GetRangeLimits const& limits, Reverse const& reverse, RangeResult const& result ) {
															#line 2865 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
	return Future<Void>(new ValidateSpecialSubrangeReadActor(ryw, begin, end, limits, reverse, result));
															#line 14010 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.g.cpp"
}

#line 2946 "/usr/src/libfdb_c/fdbclient/SpecialKeySpace.actor.cpp"
