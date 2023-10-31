#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
/*
 * TenantSpecialKeys.actor.h
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
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_TENANT_SPECIAL_KEYS_ACTOR_G_H)
#define FDBCLIENT_TENANT_SPECIAL_KEYS_ACTOR_G_H
#include "fdbclient/TenantSpecialKeys.actor.g.h"
#elif !defined(FDBCLIENT_TENANT_SPECIAL_KEYS_ACTOR_H)
#define FDBCLIENT_TENANT_SPECIAL_KEYS_ACTOR_H

#include "fdbclient/ActorLineageProfiler.h"
#include "fdbclient/FDBOptions.g.h"
#include "fdbclient/Knobs.h"
#include "fdbclient/DatabaseContext.h"
#include "fdbclient/SpecialKeySpace.actor.h"
#include "fdbclient/TenantManagement.actor.h"
#include "fdbclient/Tuple.h"
#include "flow/Arena.h"
#include "flow/UnitTest.h"
#include "flow/actorcompiler.h" // This must be the last #include.

class TenantRangeImpl : public SpecialKeyRangeRWImpl {
private:
	static KeyRangeRef removePrefix(KeyRangeRef range, KeyRef prefix, KeyRef defaultEnd) {
		KeyRef begin = range.begin.removePrefix(prefix);
		KeyRef end;
		if (range.end.startsWith(prefix)) {
			end = range.end.removePrefix(prefix);
		} else {
			end = defaultEnd;
		}

		return KeyRangeRef(begin, end);
	}

	static KeyRef withTenantMapPrefix(KeyRef key, Arena& ar) {
		int keySize = SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin.size() +
		              submoduleRange.begin.size() + mapSubRange.begin.size() + key.size();

		KeyRef prefixedKey = makeString(keySize, ar);
		uint8_t* mutableKey = mutateString(prefixedKey);

		mutableKey = SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin.copyTo(mutableKey);
		mutableKey = submoduleRange.begin.copyTo(mutableKey);
		mutableKey = mapSubRange.begin.copyTo(mutableKey);

		key.copyTo(mutableKey);
		return prefixedKey;
	}

																#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
// This generated class is to be used only via getTenantList()
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
template <class GetTenantListActor>
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class GetTenantListActorState {
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	GetTenantListActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr,RangeResult* const& results,GetRangeLimits const& limitsHint) 
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		 : ryw(ryw),
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   kr(kr),
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   results(results),
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   limitsHint(limitsHint)
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
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
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			StrictFuture<std::vector<std::pair<TenantName, TenantMapEntry>>> __when_expr_0 = TenantAPI::listTenantMetadataTransaction(&ryw->getTransaction(), kr.begin, kr.end, limitsHint.rows);
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (static_cast<GetTenantListActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetTenantListActor*>(this)->actor_wait_state = 1;
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetTenantListActor, 0, std::vector<std::pair<TenantName, TenantMapEntry>> >*>(static_cast<GetTenantListActor*>(this)));
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
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
	int a_body1cont1(std::vector<std::pair<TenantName, TenantMapEntry>> const& tenants,int loopDepth) 
	{
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		for( auto tenant : tenants ) {
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			std::string jsonString = tenant.second.toJson();
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			ValueRef tenantEntryBytes(results->arena(), jsonString);
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			results->push_back(results->arena(), KeyValueRef(withTenantMapPrefix(tenant.first, results->arena()), tenantEntryBytes));
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<GetTenantListActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~GetTenantListActorState(); static_cast<GetTenantListActor*>(this)->destroy(); return 0; }
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		new (&static_cast<GetTenantListActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~GetTenantListActorState();
		static_cast<GetTenantListActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<std::pair<TenantName, TenantMapEntry>> && tenants,int loopDepth) 
	{
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		for( auto tenant : tenants ) {
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			std::string jsonString = tenant.second.toJson();
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			ValueRef tenantEntryBytes(results->arena(), jsonString);
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			results->push_back(results->arena(), KeyValueRef(withTenantMapPrefix(tenant.first, results->arena()), tenantEntryBytes));
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<GetTenantListActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~GetTenantListActorState(); static_cast<GetTenantListActor*>(this)->destroy(); return 0; }
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		new (&static_cast<GetTenantListActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~GetTenantListActorState();
		static_cast<GetTenantListActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<std::pair<TenantName, TenantMapEntry>> const& tenants,int loopDepth) 
	{
		loopDepth = a_body1cont1(tenants, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<std::pair<TenantName, TenantMapEntry>> && tenants,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(tenants), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetTenantListActor*>(this)->actor_wait_state > 0) static_cast<GetTenantListActor*>(this)->actor_wait_state = 0;
		static_cast<GetTenantListActor*>(this)->ActorCallback< GetTenantListActor, 0, std::vector<std::pair<TenantName, TenantMapEntry>> >::remove();

	}
	void a_callback_fire(ActorCallback< GetTenantListActor, 0, std::vector<std::pair<TenantName, TenantMapEntry>> >*,std::vector<std::pair<TenantName, TenantMapEntry>> const& value) 
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
	void a_callback_fire(ActorCallback< GetTenantListActor, 0, std::vector<std::pair<TenantName, TenantMapEntry>> >*,std::vector<std::pair<TenantName, TenantMapEntry>> && value) 
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
	void a_callback_error(ActorCallback< GetTenantListActor, 0, std::vector<std::pair<TenantName, TenantMapEntry>> >*,Error err) 
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
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	ReadYourWritesTransaction* ryw;
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	KeyRangeRef kr;
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	RangeResult* results;
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	GetRangeLimits limitsHint;
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
};
// This generated class is to be used only via getTenantList()
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class GetTenantListActor final : public Actor<Void>, public ActorCallback< GetTenantListActor, 0, std::vector<std::pair<TenantName, TenantMapEntry>> >, public FastAllocated<GetTenantListActor>, public GetTenantListActorState<GetTenantListActor> {
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
	using FastAllocated<GetTenantListActor>::operator new;
	using FastAllocated<GetTenantListActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetTenantListActor, 0, std::vector<std::pair<TenantName, TenantMapEntry>> >;
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	GetTenantListActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr,RangeResult* const& results,GetRangeLimits const& limitsHint) 
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		 : Actor<Void>(),
		   GetTenantListActorState<GetTenantListActor>(ryw, kr, results, limitsHint)
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
		case 1: this->a_callback_error((ActorCallback< GetTenantListActor, 0, std::vector<std::pair<TenantName, TenantMapEntry>> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
[[nodiscard]] static Future<Void> getTenantList( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr, RangeResult* const& results, GetRangeLimits const& limitsHint ) {
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	return Future<Void>(new GetTenantListActor(ryw, kr, results, limitsHint));
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
}

#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"

																#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
// This generated class is to be used only via getTenantRange()
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
template <class GetTenantRangeActor>
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class GetTenantRangeActorState {
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	GetTenantRangeActorState(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr,GetRangeLimits const& limitsHint) 
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		 : ryw(ryw),
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   kr(kr),
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   limitsHint(limitsHint),
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   results()
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
	{
		fdb_probe_actor_create("getTenantRange", reinterpret_cast<unsigned long>(this));

	}
	~GetTenantRangeActorState() 
	{
		fdb_probe_actor_destroy("getTenantRange", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			kr = kr.removePrefix(SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin) .removePrefix(TenantRangeImpl::submoduleRange.begin);
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (kr.intersects(TenantRangeImpl::mapSubRange))
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				GetRangeLimits limits = limitsHint;
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				limits.decrement(results);
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				StrictFuture<Void> __when_expr_0 = getTenantList( ryw, removePrefix(kr & TenantRangeImpl::mapSubRange, TenantRangeImpl::mapSubRange.begin, "\xff"_sr), &results, limits);
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				if (static_cast<GetTenantRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetTenantRangeActor*>(this)->actor_wait_state = 1;
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetTenantRangeActor, 0, Void >*>(static_cast<GetTenantRangeActor*>(this)));
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
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
		this->~GetTenantRangeActorState();
		static_cast<GetTenantRangeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<GetTenantRangeActor*>(this)->SAV<RangeResult>::futures) { (void)(results); this->~GetTenantRangeActorState(); static_cast<GetTenantRangeActor*>(this)->destroy(); return 0; }
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		new (&static_cast<GetTenantRangeActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(results)); // state_var_RVO
		this->~GetTenantRangeActorState();
		static_cast<GetTenantRangeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<GetTenantRangeActor*>(this)->actor_wait_state > 0) static_cast<GetTenantRangeActor*>(this)->actor_wait_state = 0;
		static_cast<GetTenantRangeActor*>(this)->ActorCallback< GetTenantRangeActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetTenantRangeActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getTenantRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenantRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetTenantRangeActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getTenantRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenantRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetTenantRangeActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getTenantRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenantRange", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	ReadYourWritesTransaction* ryw;
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	KeyRangeRef kr;
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	GetRangeLimits limitsHint;
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	RangeResult results;
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
};
// This generated class is to be used only via getTenantRange()
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class GetTenantRangeActor final : public Actor<RangeResult>, public ActorCallback< GetTenantRangeActor, 0, Void >, public FastAllocated<GetTenantRangeActor>, public GetTenantRangeActorState<GetTenantRangeActor> {
															#line 468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
	using FastAllocated<GetTenantRangeActor>::operator new;
	using FastAllocated<GetTenantRangeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetTenantRangeActor, 0, Void >;
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	GetTenantRangeActor(ReadYourWritesTransaction* const& ryw,KeyRangeRef const& kr,GetRangeLimits const& limitsHint) 
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		 : Actor<RangeResult>(),
		   GetTenantRangeActorState<GetTenantRangeActor>(ryw, kr, limitsHint)
	{
		fdb_probe_actor_enter("getTenantRange", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getTenantRange");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getTenantRange", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetTenantRangeActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
[[nodiscard]] static Future<RangeResult> getTenantRange( ReadYourWritesTransaction* const& ryw, KeyRangeRef const& kr, GetRangeLimits const& limitsHint ) {
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	return Future<RangeResult>(new GetTenantRangeActor(ryw, kr, limitsHint));
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
}

#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"

	// Returns true if the tenant was created, false if it already existed
																#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
// This generated class is to be used only via createTenant()
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
template <class CreateTenantActor>
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class CreateTenantActorState {
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	CreateTenantActorState(ReadYourWritesTransaction* const& ryw,TenantNameRef const& tenantName,std::vector<std::pair<Standalone<StringRef>, Optional<Value>>> const& configMutations,int64_t const& tenantId,std::map<TenantGroupName, int>* const& tenantGroupNetTenantDelta) 
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		 : ryw(ryw),
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenantName(tenantName),
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   configMutations(configMutations),
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenantId(tenantId),
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenantGroupNetTenantDelta(tenantGroupNetTenantDelta),
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenantEntry(tenantId, tenantName)
															#line 535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
	{
		fdb_probe_actor_create("createTenant", reinterpret_cast<unsigned long>(this));

	}
	~CreateTenantActorState() 
	{
		fdb_probe_actor_destroy("createTenant", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			for( auto const& [name, value] : configMutations ) {
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				tenantEntry.configure(name, value);
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			}
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (tenantEntry.tenantGroup.present())
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				(*tenantGroupNetTenantDelta)[tenantEntry.tenantGroup.get()]++;
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			}
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			StrictFuture<std::pair<Optional<TenantMapEntry>, bool>> __when_expr_0 = TenantAPI::createTenantTransaction(&ryw->getTransaction(), tenantEntry);
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (static_cast<CreateTenantActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CreateTenantActor*>(this)->actor_wait_state = 1;
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CreateTenantActor, 0, std::pair<Optional<TenantMapEntry>, bool> >*>(static_cast<CreateTenantActor*>(this)));
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
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
		this->~CreateTenantActorState();
		static_cast<CreateTenantActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::pair<Optional<TenantMapEntry>, bool> const& entry,int loopDepth) 
	{
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<CreateTenantActor*>(this)->SAV<bool>::futures) { (void)(entry.second); this->~CreateTenantActorState(); static_cast<CreateTenantActor*>(this)->destroy(); return 0; }
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		new (&static_cast<CreateTenantActor*>(this)->SAV< bool >::value()) bool(entry.second);
		this->~CreateTenantActorState();
		static_cast<CreateTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(std::pair<Optional<TenantMapEntry>, bool> && entry,int loopDepth) 
	{
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<CreateTenantActor*>(this)->SAV<bool>::futures) { (void)(entry.second); this->~CreateTenantActorState(); static_cast<CreateTenantActor*>(this)->destroy(); return 0; }
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		new (&static_cast<CreateTenantActor*>(this)->SAV< bool >::value()) bool(entry.second);
		this->~CreateTenantActorState();
		static_cast<CreateTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(std::pair<Optional<TenantMapEntry>, bool> const& entry,int loopDepth) 
	{
		loopDepth = a_body1cont1(entry, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::pair<Optional<TenantMapEntry>, bool> && entry,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(entry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CreateTenantActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantActor*>(this)->ActorCallback< CreateTenantActor, 0, std::pair<Optional<TenantMapEntry>, bool> >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 0, std::pair<Optional<TenantMapEntry>, bool> >*,std::pair<Optional<TenantMapEntry>, bool> const& value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 0, std::pair<Optional<TenantMapEntry>, bool> >*,std::pair<Optional<TenantMapEntry>, bool> && value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CreateTenantActor, 0, std::pair<Optional<TenantMapEntry>, bool> >*,Error err) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	ReadYourWritesTransaction* ryw;
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	TenantNameRef tenantName;
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::vector<std::pair<Standalone<StringRef>, Optional<Value>>> configMutations;
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	int64_t tenantId;
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::map<TenantGroupName, int>* tenantGroupNetTenantDelta;
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	TenantMapEntry tenantEntry;
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
};
// This generated class is to be used only via createTenant()
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class CreateTenantActor final : public Actor<bool>, public ActorCallback< CreateTenantActor, 0, std::pair<Optional<TenantMapEntry>, bool> >, public FastAllocated<CreateTenantActor>, public CreateTenantActorState<CreateTenantActor> {
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
	using FastAllocated<CreateTenantActor>::operator new;
	using FastAllocated<CreateTenantActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CreateTenantActor, 0, std::pair<Optional<TenantMapEntry>, bool> >;
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	CreateTenantActor(ReadYourWritesTransaction* const& ryw,TenantNameRef const& tenantName,std::vector<std::pair<Standalone<StringRef>, Optional<Value>>> const& configMutations,int64_t const& tenantId,std::map<TenantGroupName, int>* const& tenantGroupNetTenantDelta) 
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		 : Actor<bool>(),
		   CreateTenantActorState<CreateTenantActor>(ryw, tenantName, configMutations, tenantId, tenantGroupNetTenantDelta)
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("createTenant");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CreateTenantActor, 0, std::pair<Optional<TenantMapEntry>, bool> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
[[nodiscard]] static Future<bool> createTenant( ReadYourWritesTransaction* const& ryw, TenantNameRef const& tenantName, std::vector<std::pair<Standalone<StringRef>, Optional<Value>>> const& configMutations, int64_t const& tenantId, std::map<TenantGroupName, int>* const& tenantGroupNetTenantDelta ) {
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	return Future<bool>(new CreateTenantActor(ryw, tenantName, configMutations, tenantId, tenantGroupNetTenantDelta));
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
}

#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"

																#line 737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
// This generated class is to be used only via createTenants()
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
template <class CreateTenantsActor>
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class CreateTenantsActorState {
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	CreateTenantsActorState(ReadYourWritesTransaction* const& ryw,std::map<TenantName, std::vector<std::pair<Standalone<StringRef>, Optional<Value>>>> const& tenants,std::map<TenantGroupName, int>* const& tenantGroupNetTenantDelta) 
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		 : ryw(ryw),
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenants(tenants),
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenantGroupNetTenantDelta(tenantGroupNetTenantDelta),
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenantCountFuture(TenantMetadata::tenantCount().getD(&ryw->getTransaction(), Snapshot::False, 0))
															#line 756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
	{
		fdb_probe_actor_create("createTenants", reinterpret_cast<unsigned long>(this));

	}
	~CreateTenantsActorState() 
	{
		fdb_probe_actor_destroy("createTenants", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			StrictFuture<int64_t> __when_expr_0 = TenantAPI::getNextTenantId(&ryw->getTransaction());
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (static_cast<CreateTenantsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CreateTenantsActor*>(this)->actor_wait_state = 1;
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CreateTenantsActor, 0, int64_t >*>(static_cast<CreateTenantsActor*>(this)));
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
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
		this->~CreateTenantsActorState();
		static_cast<CreateTenantsActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int64_t const& _nextId,int loopDepth) 
	{
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		nextId = _nextId;
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		ASSERT(nextId >= 0);
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		createFutures = std::vector<Future<bool>>();
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		int itrCount = 0;
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		for( auto const& [tenant, config] : tenants ) {
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			createFutures.push_back(createTenant(ryw, tenant, config, nextId, tenantGroupNetTenantDelta));
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (++itrCount < tenants.size())
															#line 813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				nextId = TenantAPI::computeNextTenantId(nextId, 1);
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			}
		}
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		TenantMetadata::lastTenantId().set(&ryw->getTransaction(), nextId);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		StrictFuture<Void> __when_expr_1 = waitForAll(createFutures);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (static_cast<CreateTenantsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CreateTenantsActor*>(this)->actor_wait_state = 2;
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CreateTenantsActor, 1, Void >*>(static_cast<CreateTenantsActor*>(this)));
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int64_t && _nextId,int loopDepth) 
	{
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		nextId = _nextId;
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		ASSERT(nextId >= 0);
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		createFutures = std::vector<Future<bool>>();
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		int itrCount = 0;
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		for( auto const& [tenant, config] : tenants ) {
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			createFutures.push_back(createTenant(ryw, tenant, config, nextId, tenantGroupNetTenantDelta));
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (++itrCount < tenants.size())
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				nextId = TenantAPI::computeNextTenantId(nextId, 1);
															#line 856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			}
		}
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		TenantMetadata::lastTenantId().set(&ryw->getTransaction(), nextId);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		StrictFuture<Void> __when_expr_1 = waitForAll(createFutures);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (static_cast<CreateTenantsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CreateTenantsActor*>(this)->actor_wait_state = 2;
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CreateTenantsActor, 1, Void >*>(static_cast<CreateTenantsActor*>(this)));
															#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(int64_t const& _nextId,int loopDepth) 
	{
		loopDepth = a_body1cont1(_nextId, loopDepth);

		return loopDepth;
	}
	int a_body1when1(int64_t && _nextId,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_nextId), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CreateTenantsActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantsActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantsActor*>(this)->ActorCallback< CreateTenantsActor, 0, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantsActor, 0, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("createTenants", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenants", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CreateTenantsActor, 0, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("createTenants", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenants", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CreateTenantsActor, 0, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("createTenants", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenants", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		numCreatedTenants = 0;
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		for( auto f : createFutures ) {
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (f.get())
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				++numCreatedTenants;
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			}
		}
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		StrictFuture<int64_t> __when_expr_2 = tenantCountFuture;
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (static_cast<CreateTenantsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<CreateTenantsActor*>(this)->actor_wait_state = 3;
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CreateTenantsActor, 2, int64_t >*>(static_cast<CreateTenantsActor*>(this)));
															#line 962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		numCreatedTenants = 0;
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		for( auto f : createFutures ) {
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (f.get())
															#line 975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				++numCreatedTenants;
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			}
		}
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		StrictFuture<int64_t> __when_expr_2 = tenantCountFuture;
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (static_cast<CreateTenantsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<CreateTenantsActor*>(this)->actor_wait_state = 3;
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CreateTenantsActor, 2, int64_t >*>(static_cast<CreateTenantsActor*>(this)));
															#line 991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
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
		if (static_cast<CreateTenantsActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantsActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantsActor*>(this)->ActorCallback< CreateTenantsActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantsActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("createTenants", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenants", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CreateTenantsActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("createTenants", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenants", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CreateTenantsActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("createTenants", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenants", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont5(int64_t const& tenantCount,int loopDepth) 
	{
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (tenantCount + numCreatedTenants > CLIENT_KNOBS->MAX_TENANTS_PER_CLUSTER)
															#line 1063 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		{
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			return a_body1Catch1(cluster_no_capacity(), loopDepth);
															#line 1067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<CreateTenantsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CreateTenantsActorState(); static_cast<CreateTenantsActor*>(this)->destroy(); return 0; }
															#line 1071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		new (&static_cast<CreateTenantsActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CreateTenantsActorState();
		static_cast<CreateTenantsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5(int64_t && tenantCount,int loopDepth) 
	{
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (tenantCount + numCreatedTenants > CLIENT_KNOBS->MAX_TENANTS_PER_CLUSTER)
															#line 1083 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		{
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			return a_body1Catch1(cluster_no_capacity(), loopDepth);
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<CreateTenantsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CreateTenantsActorState(); static_cast<CreateTenantsActor*>(this)->destroy(); return 0; }
															#line 1091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		new (&static_cast<CreateTenantsActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CreateTenantsActorState();
		static_cast<CreateTenantsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(int64_t const& tenantCount,int loopDepth) 
	{
		loopDepth = a_body1cont5(tenantCount, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(int64_t && tenantCount,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(tenantCount), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<CreateTenantsActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantsActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantsActor*>(this)->ActorCallback< CreateTenantsActor, 2, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantsActor, 2, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("createTenants", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenants", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CreateTenantsActor, 2, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("createTenants", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenants", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CreateTenantsActor, 2, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("createTenants", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenants", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	ReadYourWritesTransaction* ryw;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::map<TenantName, std::vector<std::pair<Standalone<StringRef>, Optional<Value>>>> tenants;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::map<TenantGroupName, int>* tenantGroupNetTenantDelta;
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	Future<int64_t> tenantCountFuture;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	int64_t nextId;
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::vector<Future<bool>> createFutures;
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	int numCreatedTenants;
															#line 1176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
};
// This generated class is to be used only via createTenants()
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class CreateTenantsActor final : public Actor<Void>, public ActorCallback< CreateTenantsActor, 0, int64_t >, public ActorCallback< CreateTenantsActor, 1, Void >, public ActorCallback< CreateTenantsActor, 2, int64_t >, public FastAllocated<CreateTenantsActor>, public CreateTenantsActorState<CreateTenantsActor> {
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
	using FastAllocated<CreateTenantsActor>::operator new;
	using FastAllocated<CreateTenantsActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CreateTenantsActor, 0, int64_t >;
friend struct ActorCallback< CreateTenantsActor, 1, Void >;
friend struct ActorCallback< CreateTenantsActor, 2, int64_t >;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	CreateTenantsActor(ReadYourWritesTransaction* const& ryw,std::map<TenantName, std::vector<std::pair<Standalone<StringRef>, Optional<Value>>>> const& tenants,std::map<TenantGroupName, int>* const& tenantGroupNetTenantDelta) 
															#line 1194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		 : Actor<Void>(),
		   CreateTenantsActorState<CreateTenantsActor>(ryw, tenants, tenantGroupNetTenantDelta)
	{
		fdb_probe_actor_enter("createTenants", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("createTenants");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("createTenants", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CreateTenantsActor, 0, int64_t >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CreateTenantsActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CreateTenantsActor, 2, int64_t >*)0, actor_cancelled()); break;
		}

	}
};
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
[[nodiscard]] static Future<Void> createTenants( ReadYourWritesTransaction* const& ryw, std::map<TenantName, std::vector<std::pair<Standalone<StringRef>, Optional<Value>>>> const& tenants, std::map<TenantGroupName, int>* const& tenantGroupNetTenantDelta ) {
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	return Future<Void>(new CreateTenantsActor(ryw, tenants, tenantGroupNetTenantDelta));
															#line 1223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
}

#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"

																#line 1228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
// This generated class is to be used only via changeTenantConfig()
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
template <class ChangeTenantConfigActor>
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class ChangeTenantConfigActorState {
															#line 1234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	ChangeTenantConfigActorState(ReadYourWritesTransaction* const& ryw,TenantName const& tenantName,std::vector<std::pair<Standalone<StringRef>, Optional<Value>>> const& configEntries,std::map<TenantGroupName, int>* const& tenantGroupNetTenantDelta) 
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		 : ryw(ryw),
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenantName(tenantName),
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   configEntries(configEntries),
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenantGroupNetTenantDelta(tenantGroupNetTenantDelta)
															#line 1247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
	{
		fdb_probe_actor_create("changeTenantConfig", reinterpret_cast<unsigned long>(this));

	}
	~ChangeTenantConfigActorState() 
	{
		fdb_probe_actor_destroy("changeTenantConfig", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			StrictFuture<TenantMapEntry> __when_expr_0 = TenantAPI::getTenantTransaction(&ryw->getTransaction(), tenantName);
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (static_cast<ChangeTenantConfigActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ChangeTenantConfigActor*>(this)->actor_wait_state = 1;
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ChangeTenantConfigActor, 0, TenantMapEntry >*>(static_cast<ChangeTenantConfigActor*>(this)));
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
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
		this->~ChangeTenantConfigActorState();
		static_cast<ChangeTenantConfigActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(TenantMapEntry const& originalEntry,int loopDepth) 
	{
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		TenantMapEntry updatedEntry = originalEntry;
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		for( auto const& [name, value] : configEntries ) {
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			updatedEntry.configure(name, value);
															#line 1296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (originalEntry.tenantGroup != updatedEntry.tenantGroup)
															#line 1300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		{
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (originalEntry.tenantGroup.present())
															#line 1304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				(*tenantGroupNetTenantDelta)[originalEntry.tenantGroup.get()]--;
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			}
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (updatedEntry.tenantGroup.present())
															#line 1312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				(*tenantGroupNetTenantDelta)[updatedEntry.tenantGroup.get()]++;
															#line 1316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			}
		}
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		StrictFuture<Void> __when_expr_1 = TenantAPI::configureTenantTransaction(&ryw->getTransaction(), originalEntry, updatedEntry);
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (static_cast<ChangeTenantConfigActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ChangeTenantConfigActor*>(this)->actor_wait_state = 2;
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChangeTenantConfigActor, 1, Void >*>(static_cast<ChangeTenantConfigActor*>(this)));
															#line 1328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(TenantMapEntry && originalEntry,int loopDepth) 
	{
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		TenantMapEntry updatedEntry = originalEntry;
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		for( auto const& [name, value] : configEntries ) {
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			updatedEntry.configure(name, value);
															#line 1341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (originalEntry.tenantGroup != updatedEntry.tenantGroup)
															#line 1345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		{
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (originalEntry.tenantGroup.present())
															#line 1349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				(*tenantGroupNetTenantDelta)[originalEntry.tenantGroup.get()]--;
															#line 1353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			}
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (updatedEntry.tenantGroup.present())
															#line 1357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				(*tenantGroupNetTenantDelta)[updatedEntry.tenantGroup.get()]++;
															#line 1361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			}
		}
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		StrictFuture<Void> __when_expr_1 = TenantAPI::configureTenantTransaction(&ryw->getTransaction(), originalEntry, updatedEntry);
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (static_cast<ChangeTenantConfigActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ChangeTenantConfigActor*>(this)->actor_wait_state = 2;
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChangeTenantConfigActor, 1, Void >*>(static_cast<ChangeTenantConfigActor*>(this)));
															#line 1373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(TenantMapEntry const& originalEntry,int loopDepth) 
	{
		loopDepth = a_body1cont1(originalEntry, loopDepth);

		return loopDepth;
	}
	int a_body1when1(TenantMapEntry && originalEntry,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(originalEntry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ChangeTenantConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeTenantConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeTenantConfigActor*>(this)->ActorCallback< ChangeTenantConfigActor, 0, TenantMapEntry >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeTenantConfigActor, 0, TenantMapEntry >*,TenantMapEntry const& value) 
	{
		fdb_probe_actor_enter("changeTenantConfig", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeTenantConfig", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ChangeTenantConfigActor, 0, TenantMapEntry >*,TenantMapEntry && value) 
	{
		fdb_probe_actor_enter("changeTenantConfig", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeTenantConfig", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ChangeTenantConfigActor, 0, TenantMapEntry >*,Error err) 
	{
		fdb_probe_actor_enter("changeTenantConfig", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeTenantConfig", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<ChangeTenantConfigActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ChangeTenantConfigActorState(); static_cast<ChangeTenantConfigActor*>(this)->destroy(); return 0; }
															#line 1445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		new (&static_cast<ChangeTenantConfigActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ChangeTenantConfigActorState();
		static_cast<ChangeTenantConfigActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<ChangeTenantConfigActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ChangeTenantConfigActorState(); static_cast<ChangeTenantConfigActor*>(this)->destroy(); return 0; }
															#line 1457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		new (&static_cast<ChangeTenantConfigActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ChangeTenantConfigActorState();
		static_cast<ChangeTenantConfigActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<ChangeTenantConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeTenantConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeTenantConfigActor*>(this)->ActorCallback< ChangeTenantConfigActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeTenantConfigActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeTenantConfig", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeTenantConfig", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ChangeTenantConfigActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeTenantConfig", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeTenantConfig", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ChangeTenantConfigActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeTenantConfig", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeTenantConfig", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	ReadYourWritesTransaction* ryw;
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	TenantName tenantName;
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::vector<std::pair<Standalone<StringRef>, Optional<Value>>> configEntries;
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::map<TenantGroupName, int>* tenantGroupNetTenantDelta;
															#line 1536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
};
// This generated class is to be used only via changeTenantConfig()
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class ChangeTenantConfigActor final : public Actor<Void>, public ActorCallback< ChangeTenantConfigActor, 0, TenantMapEntry >, public ActorCallback< ChangeTenantConfigActor, 1, Void >, public FastAllocated<ChangeTenantConfigActor>, public ChangeTenantConfigActorState<ChangeTenantConfigActor> {
															#line 1541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
	using FastAllocated<ChangeTenantConfigActor>::operator new;
	using FastAllocated<ChangeTenantConfigActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ChangeTenantConfigActor, 0, TenantMapEntry >;
friend struct ActorCallback< ChangeTenantConfigActor, 1, Void >;
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	ChangeTenantConfigActor(ReadYourWritesTransaction* const& ryw,TenantName const& tenantName,std::vector<std::pair<Standalone<StringRef>, Optional<Value>>> const& configEntries,std::map<TenantGroupName, int>* const& tenantGroupNetTenantDelta) 
															#line 1553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		 : Actor<Void>(),
		   ChangeTenantConfigActorState<ChangeTenantConfigActor>(ryw, tenantName, configEntries, tenantGroupNetTenantDelta)
	{
		fdb_probe_actor_enter("changeTenantConfig", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("changeTenantConfig");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("changeTenantConfig", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ChangeTenantConfigActor, 0, TenantMapEntry >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ChangeTenantConfigActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
[[nodiscard]] static Future<Void> changeTenantConfig( ReadYourWritesTransaction* const& ryw, TenantName const& tenantName, std::vector<std::pair<Standalone<StringRef>, Optional<Value>>> const& configEntries, std::map<TenantGroupName, int>* const& tenantGroupNetTenantDelta ) {
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	return Future<Void>(new ChangeTenantConfigActor(ryw, tenantName, configEntries, tenantGroupNetTenantDelta));
															#line 1581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
}

#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"

																#line 1586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
// This generated class is to be used only via deleteSingleTenant()
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
template <class DeleteSingleTenantActor>
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class DeleteSingleTenantActorState {
															#line 1592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	DeleteSingleTenantActorState(ReadYourWritesTransaction* const& ryw,TenantName const& tenantName,std::map<TenantGroupName, int>* const& tenantGroupNetTenantDelta) 
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		 : ryw(ryw),
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenantName(tenantName),
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenantGroupNetTenantDelta(tenantGroupNetTenantDelta)
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
	{
		fdb_probe_actor_create("deleteSingleTenant", reinterpret_cast<unsigned long>(this));

	}
	~DeleteSingleTenantActorState() 
	{
		fdb_probe_actor_destroy("deleteSingleTenant", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			StrictFuture<Optional<TenantMapEntry>> __when_expr_0 = TenantAPI::tryGetTenantTransaction(&ryw->getTransaction(), tenantName);
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (static_cast<DeleteSingleTenantActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DeleteSingleTenantActor*>(this)->actor_wait_state = 1;
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DeleteSingleTenantActor, 0, Optional<TenantMapEntry> >*>(static_cast<DeleteSingleTenantActor*>(this)));
															#line 1625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
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
		this->~DeleteSingleTenantActorState();
		static_cast<DeleteSingleTenantActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (tenantEntry.present())
															#line 1648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		{
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			StrictFuture<Void> __when_expr_1 = TenantAPI::deleteTenantTransaction(&ryw->getTransaction(), tenantEntry.get().id);
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (static_cast<DeleteSingleTenantActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<DeleteSingleTenantActor*>(this)->actor_wait_state = 2;
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DeleteSingleTenantActor, 1, Void >*>(static_cast<DeleteSingleTenantActor*>(this)));
															#line 1659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(Optional<TenantMapEntry> const& __tenantEntry,int loopDepth) 
	{
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		tenantEntry = __tenantEntry;
															#line 1673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<TenantMapEntry> && __tenantEntry,int loopDepth) 
	{
		tenantEntry = std::move(__tenantEntry);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DeleteSingleTenantActor*>(this)->actor_wait_state > 0) static_cast<DeleteSingleTenantActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteSingleTenantActor*>(this)->ActorCallback< DeleteSingleTenantActor, 0, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteSingleTenantActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("deleteSingleTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteSingleTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DeleteSingleTenantActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("deleteSingleTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteSingleTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DeleteSingleTenantActor, 0, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("deleteSingleTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteSingleTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<DeleteSingleTenantActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteSingleTenantActorState(); static_cast<DeleteSingleTenantActor*>(this)->destroy(); return 0; }
															#line 1740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		new (&static_cast<DeleteSingleTenantActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteSingleTenantActorState();
		static_cast<DeleteSingleTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (tenantEntry.get().tenantGroup.present())
															#line 1752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		{
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			(*tenantGroupNetTenantDelta)[tenantEntry.get().tenantGroup.get()]--;
															#line 1756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (tenantEntry.get().tenantGroup.present())
															#line 1766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		{
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			(*tenantGroupNetTenantDelta)[tenantEntry.get().tenantGroup.get()]--;
															#line 1770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
		loopDepth = a_body1cont2(loopDepth);

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
		if (static_cast<DeleteSingleTenantActor*>(this)->actor_wait_state > 0) static_cast<DeleteSingleTenantActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteSingleTenantActor*>(this)->ActorCallback< DeleteSingleTenantActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteSingleTenantActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteSingleTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteSingleTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DeleteSingleTenantActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteSingleTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteSingleTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DeleteSingleTenantActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteSingleTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteSingleTenant", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	ReadYourWritesTransaction* ryw;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	TenantName tenantName;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::map<TenantGroupName, int>* tenantGroupNetTenantDelta;
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	Optional<TenantMapEntry> tenantEntry;
															#line 1847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
};
// This generated class is to be used only via deleteSingleTenant()
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class DeleteSingleTenantActor final : public Actor<Void>, public ActorCallback< DeleteSingleTenantActor, 0, Optional<TenantMapEntry> >, public ActorCallback< DeleteSingleTenantActor, 1, Void >, public FastAllocated<DeleteSingleTenantActor>, public DeleteSingleTenantActorState<DeleteSingleTenantActor> {
															#line 1852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
	using FastAllocated<DeleteSingleTenantActor>::operator new;
	using FastAllocated<DeleteSingleTenantActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DeleteSingleTenantActor, 0, Optional<TenantMapEntry> >;
friend struct ActorCallback< DeleteSingleTenantActor, 1, Void >;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	DeleteSingleTenantActor(ReadYourWritesTransaction* const& ryw,TenantName const& tenantName,std::map<TenantGroupName, int>* const& tenantGroupNetTenantDelta) 
															#line 1864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		 : Actor<Void>(),
		   DeleteSingleTenantActorState<DeleteSingleTenantActor>(ryw, tenantName, tenantGroupNetTenantDelta)
	{
		fdb_probe_actor_enter("deleteSingleTenant", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("deleteSingleTenant");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("deleteSingleTenant", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DeleteSingleTenantActor, 0, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DeleteSingleTenantActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
[[nodiscard]] static Future<Void> deleteSingleTenant( ReadYourWritesTransaction* const& ryw, TenantName const& tenantName, std::map<TenantGroupName, int>* const& tenantGroupNetTenantDelta ) {
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	return Future<Void>(new DeleteSingleTenantActor(ryw, tenantName, tenantGroupNetTenantDelta));
															#line 1892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
}

#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"

																#line 1897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
// This generated class is to be used only via deleteTenantRange()
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
template <class DeleteTenantRangeActor>
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class DeleteTenantRangeActorState {
															#line 1903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	DeleteTenantRangeActorState(ReadYourWritesTransaction* const& ryw,TenantName const& beginTenant,TenantName const& endTenant,std::map<TenantGroupName, int>* const& tenantGroupNetTenantDelta) 
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		 : ryw(ryw),
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   beginTenant(beginTenant),
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   endTenant(endTenant),
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenantGroupNetTenantDelta(tenantGroupNetTenantDelta)
															#line 1916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
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
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			StrictFuture<std::vector<std::pair<TenantName, int64_t>>> __when_expr_0 = TenantAPI::listTenantsTransaction(&ryw->getTransaction(), beginTenant, endTenant, CLIENT_KNOBS->TOO_MANY);
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state = 1;
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantRangeActor, 0, std::vector<std::pair<TenantName, int64_t>> >*>(static_cast<DeleteTenantRangeActor*>(this)));
															#line 1938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
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
	int a_body1cont1(int loopDepth) 
	{
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (tenants.size() == CLIENT_KNOBS->TOO_MANY)
															#line 1961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		{
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			TraceEvent(SevWarn, "DeleteTenantRangeTooLange") .detail("BeginTenant", beginTenant) .detail("EndTenant", endTenant);
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			ryw->setSpecialKeySpaceErrorMsg( ManagementAPIError::toJsonString(false, "delete tenants", "too many tenants to range delete"));
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 1969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		std::vector<Future<Void>> deleteFutures;
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		for( auto tenant : tenants ) {
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			deleteFutures.push_back(deleteSingleTenant(ryw, tenant.first, tenantGroupNetTenantDelta));
															#line 1977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		StrictFuture<Void> __when_expr_1 = waitForAll(deleteFutures);
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state = 2;
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantRangeActor, 1, Void >*>(static_cast<DeleteTenantRangeActor*>(this)));
															#line 1988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<std::pair<TenantName, int64_t>> const& __tenants,int loopDepth) 
	{
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		tenants = __tenants;
															#line 1997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<std::pair<TenantName, int64_t>> && __tenants,int loopDepth) 
	{
		tenants = std::move(__tenants);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantRangeActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantRangeActor*>(this)->ActorCallback< DeleteTenantRangeActor, 0, std::vector<std::pair<TenantName, int64_t>> >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantRangeActor, 0, std::vector<std::pair<TenantName, int64_t>> >*,std::vector<std::pair<TenantName, int64_t>> const& value) 
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
	void a_callback_fire(ActorCallback< DeleteTenantRangeActor, 0, std::vector<std::pair<TenantName, int64_t>> >*,std::vector<std::pair<TenantName, int64_t>> && value) 
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
	void a_callback_error(ActorCallback< DeleteTenantRangeActor, 0, std::vector<std::pair<TenantName, int64_t>> >*,Error err) 
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
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<DeleteTenantRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteTenantRangeActorState(); static_cast<DeleteTenantRangeActor*>(this)->destroy(); return 0; }
															#line 2064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		new (&static_cast<DeleteTenantRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteTenantRangeActorState();
		static_cast<DeleteTenantRangeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<DeleteTenantRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteTenantRangeActorState(); static_cast<DeleteTenantRangeActor*>(this)->destroy(); return 0; }
															#line 2076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
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
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	ReadYourWritesTransaction* ryw;
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	TenantName beginTenant;
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	TenantName endTenant;
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::map<TenantGroupName, int>* tenantGroupNetTenantDelta;
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::vector<std::pair<TenantName, int64_t>> tenants;
															#line 2157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
};
// This generated class is to be used only via deleteTenantRange()
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class DeleteTenantRangeActor final : public Actor<Void>, public ActorCallback< DeleteTenantRangeActor, 0, std::vector<std::pair<TenantName, int64_t>> >, public ActorCallback< DeleteTenantRangeActor, 1, Void >, public FastAllocated<DeleteTenantRangeActor>, public DeleteTenantRangeActorState<DeleteTenantRangeActor> {
															#line 2162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
	using FastAllocated<DeleteTenantRangeActor>::operator new;
	using FastAllocated<DeleteTenantRangeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DeleteTenantRangeActor, 0, std::vector<std::pair<TenantName, int64_t>> >;
friend struct ActorCallback< DeleteTenantRangeActor, 1, Void >;
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	DeleteTenantRangeActor(ReadYourWritesTransaction* const& ryw,TenantName const& beginTenant,TenantName const& endTenant,std::map<TenantGroupName, int>* const& tenantGroupNetTenantDelta) 
															#line 2174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		 : Actor<Void>(),
		   DeleteTenantRangeActorState<DeleteTenantRangeActor>(ryw, beginTenant, endTenant, tenantGroupNetTenantDelta)
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
		case 1: this->a_callback_error((ActorCallback< DeleteTenantRangeActor, 0, std::vector<std::pair<TenantName, int64_t>> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DeleteTenantRangeActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
[[nodiscard]] static Future<Void> deleteTenantRange( ReadYourWritesTransaction* const& ryw, TenantName const& beginTenant, TenantName const& endTenant, std::map<TenantGroupName, int>* const& tenantGroupNetTenantDelta ) {
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	return Future<Void>(new DeleteTenantRangeActor(ryw, beginTenant, endTenant, tenantGroupNetTenantDelta));
															#line 2202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
}

#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"

	// Check if the number of tenants in the tenant group is equal to the net reduction in the number of tenants.
	// If it is, then we can delete the tenant group.
																#line 2209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
// This generated class is to be used only via checkAndRemoveTenantGroup()
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
template <class CheckAndRemoveTenantGroupActor>
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class CheckAndRemoveTenantGroupActorState {
															#line 2215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	CheckAndRemoveTenantGroupActorState(ReadYourWritesTransaction* const& ryw,TenantGroupName const& tenantGroup,int const& tenantDelta) 
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		 : ryw(ryw),
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenantGroup(tenantGroup),
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenantDelta(tenantDelta)
															#line 2226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
	{
		fdb_probe_actor_create("checkAndRemoveTenantGroup", reinterpret_cast<unsigned long>(this));

	}
	~CheckAndRemoveTenantGroupActorState() 
	{
		fdb_probe_actor_destroy("checkAndRemoveTenantGroup", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			ASSERT(tenantDelta < 0);
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			removedTenants = -tenantDelta;
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			StrictFuture<KeyBackedSet<Tuple>::RangeResultType> __when_expr_0 = TenantMetadata::tenantGroupTenantIndex().getRange(&ryw->getTransaction(), Tuple::makeTuple(tenantGroup), Tuple::makeTuple(keyAfter(tenantGroup)), removedTenants + 1);
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (static_cast<CheckAndRemoveTenantGroupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheckAndRemoveTenantGroupActor*>(this)->actor_wait_state = 1;
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckAndRemoveTenantGroupActor, 0, KeyBackedSet<Tuple>::RangeResultType >*>(static_cast<CheckAndRemoveTenantGroupActor*>(this)));
															#line 2252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
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
		this->~CheckAndRemoveTenantGroupActorState();
		static_cast<CheckAndRemoveTenantGroupActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(KeyBackedSet<Tuple>::RangeResultType const& tenantsInGroup,int loopDepth) 
	{
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		ASSERT(tenantsInGroup.results.size() >= removedTenants);
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (tenantsInGroup.results.size() == removedTenants)
															#line 2277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		{
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			TenantMetadata::tenantGroupMap().erase(&ryw->getTransaction(), tenantGroup);
															#line 2281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<CheckAndRemoveTenantGroupActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckAndRemoveTenantGroupActorState(); static_cast<CheckAndRemoveTenantGroupActor*>(this)->destroy(); return 0; }
															#line 2285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		new (&static_cast<CheckAndRemoveTenantGroupActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheckAndRemoveTenantGroupActorState();
		static_cast<CheckAndRemoveTenantGroupActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(KeyBackedSet<Tuple>::RangeResultType && tenantsInGroup,int loopDepth) 
	{
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		ASSERT(tenantsInGroup.results.size() >= removedTenants);
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (tenantsInGroup.results.size() == removedTenants)
															#line 2299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		{
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			TenantMetadata::tenantGroupMap().erase(&ryw->getTransaction(), tenantGroup);
															#line 2303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<CheckAndRemoveTenantGroupActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckAndRemoveTenantGroupActorState(); static_cast<CheckAndRemoveTenantGroupActor*>(this)->destroy(); return 0; }
															#line 2307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		new (&static_cast<CheckAndRemoveTenantGroupActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheckAndRemoveTenantGroupActorState();
		static_cast<CheckAndRemoveTenantGroupActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(KeyBackedSet<Tuple>::RangeResultType const& tenantsInGroup,int loopDepth) 
	{
		loopDepth = a_body1cont1(tenantsInGroup, loopDepth);

		return loopDepth;
	}
	int a_body1when1(KeyBackedSet<Tuple>::RangeResultType && tenantsInGroup,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(tenantsInGroup), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CheckAndRemoveTenantGroupActor*>(this)->actor_wait_state > 0) static_cast<CheckAndRemoveTenantGroupActor*>(this)->actor_wait_state = 0;
		static_cast<CheckAndRemoveTenantGroupActor*>(this)->ActorCallback< CheckAndRemoveTenantGroupActor, 0, KeyBackedSet<Tuple>::RangeResultType >::remove();

	}
	void a_callback_fire(ActorCallback< CheckAndRemoveTenantGroupActor, 0, KeyBackedSet<Tuple>::RangeResultType >*,KeyBackedSet<Tuple>::RangeResultType const& value) 
	{
		fdb_probe_actor_enter("checkAndRemoveTenantGroup", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkAndRemoveTenantGroup", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckAndRemoveTenantGroupActor, 0, KeyBackedSet<Tuple>::RangeResultType >*,KeyBackedSet<Tuple>::RangeResultType && value) 
	{
		fdb_probe_actor_enter("checkAndRemoveTenantGroup", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkAndRemoveTenantGroup", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckAndRemoveTenantGroupActor, 0, KeyBackedSet<Tuple>::RangeResultType >*,Error err) 
	{
		fdb_probe_actor_enter("checkAndRemoveTenantGroup", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkAndRemoveTenantGroup", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	ReadYourWritesTransaction* ryw;
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	TenantGroupName tenantGroup;
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	int tenantDelta;
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	int removedTenants;
															#line 2386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
};
// This generated class is to be used only via checkAndRemoveTenantGroup()
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class CheckAndRemoveTenantGroupActor final : public Actor<Void>, public ActorCallback< CheckAndRemoveTenantGroupActor, 0, KeyBackedSet<Tuple>::RangeResultType >, public FastAllocated<CheckAndRemoveTenantGroupActor>, public CheckAndRemoveTenantGroupActorState<CheckAndRemoveTenantGroupActor> {
															#line 2391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
	using FastAllocated<CheckAndRemoveTenantGroupActor>::operator new;
	using FastAllocated<CheckAndRemoveTenantGroupActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckAndRemoveTenantGroupActor, 0, KeyBackedSet<Tuple>::RangeResultType >;
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	CheckAndRemoveTenantGroupActor(ReadYourWritesTransaction* const& ryw,TenantGroupName const& tenantGroup,int const& tenantDelta) 
															#line 2402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		 : Actor<Void>(),
		   CheckAndRemoveTenantGroupActorState<CheckAndRemoveTenantGroupActor>(ryw, tenantGroup, tenantDelta)
	{
		fdb_probe_actor_enter("checkAndRemoveTenantGroup", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkAndRemoveTenantGroup");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkAndRemoveTenantGroup", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckAndRemoveTenantGroupActor, 0, KeyBackedSet<Tuple>::RangeResultType >*)0, actor_cancelled()); break;
		}

	}
};
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
[[nodiscard]] static Future<Void> checkAndRemoveTenantGroup( ReadYourWritesTransaction* const& ryw, TenantGroupName const& tenantGroup, int const& tenantDelta ) {
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	return Future<Void>(new CheckAndRemoveTenantGroupActor(ryw, tenantGroup, tenantDelta));
															#line 2429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
}

#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"

public:
	const inline static KeyRangeRef submoduleRange = KeyRangeRef("tenant/"_sr, "tenant0"_sr);
	const inline static KeyRangeRef mapSubRange = KeyRangeRef("map/"_sr, "map0"_sr);
	const inline static KeyRangeRef configureSubRange = KeyRangeRef("configure/"_sr, "configure0"_sr);
	const inline static KeyRangeRef renameSubRange = KeyRangeRef("rename/"_sr, "rename0"_sr);

	explicit TenantRangeImpl(KeyRangeRef kr) : SpecialKeyRangeRWImpl(kr) {}

	Future<RangeResult> getRange(ReadYourWritesTransaction* ryw,
	                             KeyRangeRef kr,
	                             GetRangeLimits limitsHint) const override {
		return getTenantRange(ryw, kr, limitsHint);
	}

																#line 2448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
// This generated class is to be used only via commitImpl()
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
template <class CommitImplActor>
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class CommitImplActorState {
															#line 2454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	CommitImplActorState(TenantRangeImpl* const& self,ReadYourWritesTransaction* const& ryw) 
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		 : self(self),
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   ryw(ryw),
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenantManagementFutures(),
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   tenantGroupNetTenantDelta(),
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   ranges(ryw->getSpecialKeySpaceWriteMap().containedRanges(self->range)),
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   mapMutations(),
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   configMutations(),
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   renameSet(),
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		   renameMutations()
															#line 2477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
	{
		fdb_probe_actor_create("commitImpl", reinterpret_cast<unsigned long>(this));

	}
	~CommitImplActorState() 
	{
		fdb_probe_actor_destroy("commitImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			tenantManagementFutures.push_back(TenantAPI::checkTenantMode(&ryw->getTransaction(), ClusterType::STANDALONE));
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			RangeForbody1Iterator0 = std::begin(ranges);
															#line 2494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
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
		this->~CommitImplActorState();
		static_cast<CommitImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		std::map<TenantName, std::vector<std::pair<Standalone<StringRef>, Optional<Value>>>> tenantsToCreate;
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		for( auto mapMutation : mapMutations ) {
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			TenantNameRef tenantName = mapMutation.first.begin;
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			auto set_iter = renameSet.lower_bound(tenantName);
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (set_iter != renameSet.end() && mapMutation.first.contains(*set_iter))
															#line 2525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				ryw->setSpecialKeySpaceErrorMsg( ManagementAPIError::toJsonString(false, "rename tenant", "tenant rename conflict"));
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 2531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			}
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (mapMutation.second.present())
															#line 2535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				std::vector<std::pair<Standalone<StringRef>, Optional<Value>>> createMutations;
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				auto itr = configMutations.find(tenantName);
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				if (itr != configMutations.end())
															#line 2543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
				{
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
					createMutations = itr->second;
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
					configMutations.erase(itr);
															#line 2549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
				}
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				tenantsToCreate[tenantName] = createMutations;
															#line 2553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			}
			else
			{
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				if (mapMutation.first.singleKeyRange())
															#line 2559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
				{
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
					tenantManagementFutures.push_back(deleteSingleTenant(ryw, tenantName, &tenantGroupNetTenantDelta));
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
					configMutations.erase(tenantName);
															#line 2565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
				}
				else
				{
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
					tenantManagementFutures.push_back( deleteTenantRange(ryw, tenantName, mapMutation.first.end, &tenantGroupNetTenantDelta));
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
					configMutations.erase(configMutations.lower_bound(tenantName), configMutations.lower_bound(mapMutation.first.end));
															#line 2573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
				}
			}
		}
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!tenantsToCreate.empty())
															#line 2579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		{
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			tenantManagementFutures.push_back(createTenants(ryw, tenantsToCreate, &tenantGroupNetTenantDelta));
															#line 2583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		for( auto configMutation : configMutations ) {
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (renameSet.count(configMutation.first))
															#line 2589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				ryw->setSpecialKeySpaceErrorMsg( ManagementAPIError::toJsonString(false, "rename tenant", "tenant rename conflict"));
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				return a_body1Catch1(special_keys_api_failure(), loopDepth);
															#line 2595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			}
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			tenantManagementFutures.push_back( changeTenantConfig(ryw, configMutation.first, configMutation.second, &tenantGroupNetTenantDelta));
															#line 2599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		for( auto renameMutation : renameMutations ) {
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			tenantManagementFutures.push_back(TenantAPI::renameTenantTransaction( &ryw->getTransaction(), renameMutation.first, renameMutation.second));
															#line 2605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		StrictFuture<Void> __when_expr_0 = waitForAll(tenantManagementFutures);
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (static_cast<CommitImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1cont1when1(__when_expr_0.get(), loopDepth); };
		static_cast<CommitImplActor*>(this)->actor_wait_state = 1;
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CommitImplActor, 0, Void >*>(static_cast<CommitImplActor*>(this)));
															#line 2616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
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
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!(RangeForbody1Iterator0 != std::end(ranges)))
															#line 2632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		auto range = *RangeForbody1Iterator0;
															#line 2638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		{
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (!range.value().first)
															#line 2642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
				return a_body1continue1(loopDepth); // continue
			}
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			adjustedRange = range.range() .removePrefix(SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::MANAGEMENT).begin) .removePrefix(submoduleRange.begin);
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (mapSubRange.intersects(adjustedRange))
															#line 2650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				adjustedRange = mapSubRange & adjustedRange;
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				adjustedRange = removePrefix(adjustedRange, mapSubRange.begin, "\xff"_sr);
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				mapMutations.push_back(std::make_pair(adjustedRange, range.value().second));
															#line 2658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
				loopDepth = a_body1loopBody1cont3(loopDepth);
			}
			else
			{
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				if (configureSubRange.intersects(adjustedRange) && adjustedRange.singleKeyRange())
															#line 2665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
				{
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
					StringRef configTupleStr = adjustedRange.begin.removePrefix(configureSubRange.begin);
															#line 2669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
					try {
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
						Tuple tuple = Tuple::unpack(configTupleStr);
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
						if (tuple.size() != 2)
															#line 2675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
						{
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
							return a_body1loopBody1Catch1(invalid_tuple_index(), loopDepth);
															#line 2679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
						}
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
						configMutations[tuple.getString(0)].push_back( std::make_pair(tuple.getString(1), range.value().second));
															#line 2683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
						loopDepth = a_body1loopBody1cont10(loopDepth);
					}
					catch (Error& error) {
						loopDepth = a_body1loopBody1Catch1(error, loopDepth);
					} catch (...) {
						loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
					}
				}
				else
				{
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
					if (renameSubRange.intersects(adjustedRange))
															#line 2696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
					{
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
						StringRef oldName = adjustedRange.begin.removePrefix(renameSubRange.begin);
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
						StringRef newName = range.value().second.get();
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
						if (renameSet.count(oldName) || renameSet.count(newName) || oldName == newName)
															#line 2704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
						{
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
							ryw->setSpecialKeySpaceErrorMsg( ManagementAPIError::toJsonString(false, "rename tenant", "tenant rename conflict"));
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
							return a_body1Catch1(special_keys_api_failure(), std::max(0, loopDepth - 1));
															#line 2710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
						}
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
						renameSet.insert(oldName);
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
						renameSet.insert(newName);
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
						renameMutations.push_back(std::make_pair(oldName, newName));
															#line 2718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
					}
					loopDepth = a_body1loopBody1cont6(loopDepth);
				}
			}
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
	int a_body1continue1(int loopDepth) 
	{
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		++RangeForbody1Iterator0;
															#line 2744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		++RangeForbody1Iterator0;
															#line 2753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			TraceEvent(SevWarn, "InvalidTenantConfigurationKey").error(e).detail("Key", adjustedRange.begin);
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			ryw->setSpecialKeySpaceErrorMsg(ManagementAPIError::toJsonString( false, "configure tenant", "invalid tenant configuration key"));
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			return a_body1Catch1(special_keys_api_failure(), std::max(0, loopDepth - 1));
															#line 2785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont10(int loopDepth) 
	{
		try {
			loopDepth = a_body1loopBody1cont7(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		tenantGroupUpdateFutures = std::vector<Future<Void>>();
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		for( auto [tenantGroup, count] : tenantGroupNetTenantDelta ) {
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (count < 0)
															#line 2816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				tenantGroupUpdateFutures.push_back(checkAndRemoveTenantGroup(ryw, tenantGroup, count));
															#line 2820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			}
		}
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		StrictFuture<Void> __when_expr_1 = waitForAll(tenantGroupUpdateFutures);
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (static_cast<CommitImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<CommitImplActor*>(this)->actor_wait_state = 2;
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CommitImplActor, 1, Void >*>(static_cast<CommitImplActor*>(this)));
															#line 2832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		tenantGroupUpdateFutures = std::vector<Future<Void>>();
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		for( auto [tenantGroup, count] : tenantGroupNetTenantDelta ) {
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
			if (count < 0)
															#line 2845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			{
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
				tenantGroupUpdateFutures.push_back(checkAndRemoveTenantGroup(ryw, tenantGroup, count));
															#line 2849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
			}
		}
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		StrictFuture<Void> __when_expr_1 = waitForAll(tenantGroupUpdateFutures);
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (static_cast<CommitImplActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<CommitImplActor*>(this)->actor_wait_state = 2;
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CommitImplActor, 1, Void >*>(static_cast<CommitImplActor*>(this)));
															#line 2861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
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
	void a_exitChoose1() 
	{
		if (static_cast<CommitImplActor*>(this)->actor_wait_state > 0) static_cast<CommitImplActor*>(this)->actor_wait_state = 0;
		static_cast<CommitImplActor*>(this)->ActorCallback< CommitImplActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CommitImplActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("commitImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commitImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CommitImplActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("commitImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commitImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CommitImplActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("commitImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commitImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont14(Void const& _,int loopDepth) 
	{
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<CommitImplActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~CommitImplActorState(); static_cast<CommitImplActor*>(this)->destroy(); return 0; }
															#line 2933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		new (&static_cast<CommitImplActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~CommitImplActorState();
		static_cast<CommitImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont14(Void && _,int loopDepth) 
	{
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
		if (!static_cast<CommitImplActor*>(this)->SAV<Optional<std::string>>::futures) { (void)(Optional<std::string>()); this->~CommitImplActorState(); static_cast<CommitImplActor*>(this)->destroy(); return 0; }
															#line 2945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		new (&static_cast<CommitImplActor*>(this)->SAV< Optional<std::string> >::value()) Optional<std::string>(Optional<std::string>());
		this->~CommitImplActorState();
		static_cast<CommitImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont14(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont14(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CommitImplActor*>(this)->actor_wait_state > 0) static_cast<CommitImplActor*>(this)->actor_wait_state = 0;
		static_cast<CommitImplActor*>(this)->ActorCallback< CommitImplActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CommitImplActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("commitImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commitImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CommitImplActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("commitImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commitImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CommitImplActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("commitImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commitImpl", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	TenantRangeImpl* self;
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	ReadYourWritesTransaction* ryw;
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::vector<Future<Void>> tenantManagementFutures;
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::map<TenantGroupName, int> tenantGroupNetTenantDelta;
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	KeyRangeMap<std::pair<bool, Optional<Value>>>::Ranges ranges;
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::vector<std::pair<KeyRangeRef, Optional<Value>>> mapMutations;
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::map<TenantName, std::vector<std::pair<Standalone<StringRef>, Optional<Value>>>> configMutations;
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::set<TenantName> renameSet;
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::vector<std::pair<TenantName, TenantName>> renameMutations;
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	decltype(std::begin(std::declval<KeyRangeMap<std::pair<bool, Optional<Value>>>::Ranges>())) RangeForbody1Iterator0;
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	KeyRangeRef adjustedRange;
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	std::vector<Future<Void>> tenantGroupUpdateFutures;
															#line 3040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
};
// This generated class is to be used only via commitImpl()
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
class CommitImplActor final : public Actor<Optional<std::string>>, public ActorCallback< CommitImplActor, 0, Void >, public ActorCallback< CommitImplActor, 1, Void >, public FastAllocated<CommitImplActor>, public CommitImplActorState<CommitImplActor> {
															#line 3045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
public:
	using FastAllocated<CommitImplActor>::operator new;
	using FastAllocated<CommitImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<std::string>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CommitImplActor, 0, Void >;
friend struct ActorCallback< CommitImplActor, 1, Void >;
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	CommitImplActor(TenantRangeImpl* const& self,ReadYourWritesTransaction* const& ryw) 
															#line 3057 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
		 : Actor<Optional<std::string>>(),
		   CommitImplActorState<CommitImplActor>(self, ryw)
	{
		fdb_probe_actor_enter("commitImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("commitImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("commitImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CommitImplActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CommitImplActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
[[nodiscard]] static Future<Optional<std::string>> commitImpl( TenantRangeImpl* const& self, ReadYourWritesTransaction* const& ryw ) {
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"
	return Future<Optional<std::string>>(new CommitImplActor(self, ryw));
															#line 3085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.g.h"
}

#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantSpecialKeys.actor.h"

	Future<Optional<std::string>> commit(ReadYourWritesTransaction* ryw) override { return commitImpl(this, ryw); }
};

#include "flow/unactorcompiler.h"
#endif