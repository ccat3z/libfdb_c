#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
/*
 * GlobalConfig.actor.cpp
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

#include "fdbclient/DatabaseContext.h"
#include "fdbclient/GlobalConfig.actor.h"
#include "fdbclient/SpecialKeySpace.actor.h"
#include "fdbclient/SystemData.h"
#include "fdbclient/Tuple.h"
#include "flow/flow.h"
#include "flow/genericactors.actor.h"

#include "flow/actorcompiler.h" // This must be the last #include.

const KeyRef fdbClientInfoTxnSampleRate = "config/fdb_client_info/client_txn_sample_rate"_sr;
const KeyRef fdbClientInfoTxnSizeLimit = "config/fdb_client_info/client_txn_size_limit"_sr;

const KeyRef transactionTagSampleRate = "config/transaction_tag_sample_rate"_sr;
const KeyRef transactionTagSampleCost = "config/transaction_tag_sample_cost"_sr;

const KeyRef samplingFrequency = "visibility/sampling/frequency"_sr;
const KeyRef samplingWindow = "visibility/sampling/window"_sr;

GlobalConfig::GlobalConfig(DatabaseContext* cx) : cx(cx), lastUpdate(0) {}

void GlobalConfig::applyChanges(Transaction& tr,
                                const VectorRef<KeyValueRef>& insertions,
                                const VectorRef<KeyRangeRef>& clears) {
	VersionHistory vh{ 0 };
	for (const auto& kv : insertions) {
		vh.mutations.emplace_back_deep(vh.mutations.arena(), MutationRef(MutationRef::SetValue, kv.key, kv.value));
		tr.set(kv.key.withPrefix(globalConfigKeysPrefix), kv.value);
	}
	for (const auto& range : clears) {
		vh.mutations.emplace_back_deep(vh.mutations.arena(),
		                               MutationRef(MutationRef::ClearRange, range.begin, range.end));
		tr.clear(
		    KeyRangeRef(range.begin.withPrefix(globalConfigKeysPrefix), range.end.withPrefix(globalConfigKeysPrefix)));
	}

	// Record the mutations in this commit into the global configuration history.
	Key historyKey = addVersionStampAtEnd(globalConfigHistoryPrefix);
	ObjectWriter historyWriter(IncludeVersion());
	historyWriter.serialize(vh);
	tr.atomicOp(historyKey, historyWriter.toStringRef(), MutationRef::SetVersionstampedKey);

	// Write version key to trigger update in cluster controller.
	tr.atomicOp(globalConfigVersionKey,
	            "0123456789\x00\x00\x00\x00"_sr, // versionstamp
	            MutationRef::SetVersionstampedValue);
}

Key GlobalConfig::prefixedKey(KeyRef key) {
	return key.withPrefix(SpecialKeySpace::getModuleRange(SpecialKeySpace::MODULE::GLOBALCONFIG).begin);
}

const Reference<ConfigValue> GlobalConfig::get(KeyRef name) {
	auto it = data.find(name);
	if (it == data.end()) {
		return Reference<ConfigValue>();
	}
	return it->second;
}

const std::map<KeyRef, Reference<ConfigValue>> GlobalConfig::get(KeyRangeRef range) {
	std::map<KeyRef, Reference<ConfigValue>> results;
	for (const auto& [key, value] : data) {
		if (range.contains(key)) {
			results[key] = value;
		}
	}
	return results;
}

Future<Void> GlobalConfig::onInitialized() {
	return initialized.getFuture();
}

Future<Void> GlobalConfig::onChange() {
	return configChanged.onTrigger();
}

void GlobalConfig::trigger(KeyRef key, std::function<void(std::optional<std::any>)> fn) {
	callbacks.emplace(key, std::move(fn));
}

void GlobalConfig::insert(KeyRef key, ValueRef value) {
	// TraceEvent(SevInfo, "GlobalConfigInsert").detail("Key", key).detail("Value", value);
	data.erase(key);

	Arena arena(key.expectedSize() + value.expectedSize());
	KeyRef stableKey = KeyRef(arena, key);
	try {
		std::any any;
		Tuple t = Tuple::unpack(value);
		if (t.getType(0) == Tuple::ElementType::UTF8) {
			any = StringRef(arena, t.getString(0).contents());
		} else if (t.getType(0) == Tuple::ElementType::INT) {
			any = t.getInt(0);
		} else if (t.getType(0) == Tuple::ElementType::BOOL) {
			any = t.getBool(0);
		} else if (t.getType(0) == Tuple::ElementType::FLOAT) {
			any = t.getFloat(0);
		} else if (t.getType(0) == Tuple::ElementType::DOUBLE) {
			any = t.getDouble(0);
		} else if (t.getType(0) == Tuple::ElementType::VERSIONSTAMP) {
			any = t.getVersionstamp(0);
		} else {
			ASSERT(false);
		}
		data[stableKey] = makeReference<ConfigValue>(std::move(arena), std::move(any));

		if (callbacks.find(stableKey) != callbacks.end()) {
			callbacks[stableKey](data[stableKey]->value);
		}
	} catch (Error& e) {
		TraceEvent(SevWarn, "GlobalConfigTupleParseError").detail("What", e.what());
	}
}

void GlobalConfig::erase(Key key) {
	erase(KeyRangeRef(key, keyAfter(key)));
}

void GlobalConfig::erase(KeyRangeRef range) {
	// TraceEvent(SevInfo, "GlobalConfigErase").detail("Range", range);
	auto it = data.begin();
	while (it != data.end()) {
		if (range.contains(it->first)) {
			if (callbacks.find(it->first) != callbacks.end()) {
				callbacks[it->first](std::nullopt);
			}
			it = data.erase(it);
		} else {
			++it;
		}
	}
}

// Updates local copy of global configuration by reading the entire key-range
// from storage (proxied through the GrvProxies).
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
// This generated class is to be used only via refresh()
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
template <class GlobalConfig_RefreshActor>
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
class GlobalConfig_RefreshActorState {
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
public:
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	GlobalConfig_RefreshActorState(GlobalConfig* const& self,Version const& lastKnown) 
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		 : self(self),
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		   lastKnown(lastKnown)
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
	{
		fdb_probe_actor_create("refresh", reinterpret_cast<unsigned long>(this));

	}
	~GlobalConfig_RefreshActorState() 
	{
		fdb_probe_actor_destroy("refresh", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			self->erase(KeyRangeRef(""_sr, "\xff"_sr));
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			backoff = Backoff(CLIENT_KNOBS->GLOBAL_CONFIG_REFRESH_BACKOFF, CLIENT_KNOBS->GLOBAL_CONFIG_REFRESH_MAX_BACKOFF);
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			;
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
		this->~GlobalConfig_RefreshActorState();
		static_cast<GlobalConfig_RefreshActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			StrictFuture<GlobalConfigRefreshReply> __when_expr_0 = timeoutError(basicLoadBalance(self->cx->getGrvProxies(UseProvisionalProxies::False), &GrvProxyInterface::refreshGlobalConfig, GlobalConfigRefreshRequest{ lastKnown }), CLIENT_KNOBS->GLOBAL_CONFIG_REFRESH_TIMEOUT);
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state = 1;
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_RefreshActor, 0, GlobalConfigRefreshReply >*>(static_cast<GlobalConfig_RefreshActor*>(this)));
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			StrictFuture<Void> __when_expr_1 = backoff.onError();
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state = 2;
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_RefreshActor, 1, Void >*>(static_cast<GlobalConfig_RefreshActor*>(this)));
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(GlobalConfigRefreshReply const& reply,int loopDepth) 
	{
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		for( const auto& kv : reply.result ) {
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			KeyRef systemKey = kv.key.removePrefix(globalConfigKeysPrefix);
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			self->insert(systemKey, kv.value);
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		}
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (!static_cast<GlobalConfig_RefreshActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~GlobalConfig_RefreshActorState(); static_cast<GlobalConfig_RefreshActor*>(this)->destroy(); return 0; }
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		new (&static_cast<GlobalConfig_RefreshActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~GlobalConfig_RefreshActorState();
		static_cast<GlobalConfig_RefreshActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(GlobalConfigRefreshReply && reply,int loopDepth) 
	{
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		for( const auto& kv : reply.result ) {
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			KeyRef systemKey = kv.key.removePrefix(globalConfigKeysPrefix);
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			self->insert(systemKey, kv.value);
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		}
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (!static_cast<GlobalConfig_RefreshActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~GlobalConfig_RefreshActorState(); static_cast<GlobalConfig_RefreshActor*>(this)->destroy(); return 0; }
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		new (&static_cast<GlobalConfig_RefreshActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~GlobalConfig_RefreshActorState();
		static_cast<GlobalConfig_RefreshActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(GlobalConfigRefreshReply const& reply,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(reply, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(GlobalConfigRefreshReply && reply,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(reply), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_RefreshActor*>(this)->ActorCallback< GlobalConfig_RefreshActor, 0, GlobalConfigRefreshReply >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_RefreshActor, 0, GlobalConfigRefreshReply >*,GlobalConfigRefreshReply const& value) 
	{
		fdb_probe_actor_enter("refresh", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refresh", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_RefreshActor, 0, GlobalConfigRefreshReply >*,GlobalConfigRefreshReply && value) 
	{
		fdb_probe_actor_enter("refresh", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refresh", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GlobalConfig_RefreshActor, 0, GlobalConfigRefreshReply >*,Error err) 
	{
		fdb_probe_actor_enter("refresh", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refresh", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_RefreshActor*>(this)->ActorCallback< GlobalConfig_RefreshActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_RefreshActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("refresh", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refresh", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_RefreshActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("refresh", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refresh", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GlobalConfig_RefreshActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("refresh", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refresh", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	GlobalConfig* self;
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	Version lastKnown;
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	Backoff backoff;
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
};
// This generated class is to be used only via refresh()
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
class GlobalConfig_RefreshActor final : public Actor<Void>, public ActorCallback< GlobalConfig_RefreshActor, 0, GlobalConfigRefreshReply >, public ActorCallback< GlobalConfig_RefreshActor, 1, Void >, public FastAllocated<GlobalConfig_RefreshActor>, public GlobalConfig_RefreshActorState<GlobalConfig_RefreshActor> {
															#line 461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
public:
	using FastAllocated<GlobalConfig_RefreshActor>::operator new;
	using FastAllocated<GlobalConfig_RefreshActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GlobalConfig_RefreshActor, 0, GlobalConfigRefreshReply >;
friend struct ActorCallback< GlobalConfig_RefreshActor, 1, Void >;
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	GlobalConfig_RefreshActor(GlobalConfig* const& self,Version const& lastKnown) 
															#line 473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		 : Actor<Void>(),
		   GlobalConfig_RefreshActorState<GlobalConfig_RefreshActor>(self, lastKnown)
	{
		fdb_probe_actor_enter("refresh", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("refresh");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("refresh", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GlobalConfig_RefreshActor, 0, GlobalConfigRefreshReply >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GlobalConfig_RefreshActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
[[nodiscard]] Future<Void> GlobalConfig::refresh( GlobalConfig* const& self, Version const& lastKnown ) {
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	return Future<Void>(new GlobalConfig_RefreshActor(self, lastKnown));
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
}

#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"

// Applies updates to the local copy of the global configuration when this
// process receives an updated history.
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
// This generated class is to be used only via updater()
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
template <class GlobalConfig_UpdaterActor>
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
class GlobalConfig_UpdaterActorState {
															#line 514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
public:
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	GlobalConfig_UpdaterActorState(GlobalConfig* const& self,const ClientDBInfo* const& dbInfo) 
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		 : self(self),
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		   dbInfo(dbInfo)
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
	{
		fdb_probe_actor_create("updater", reinterpret_cast<unsigned long>(this));

	}
	~GlobalConfig_UpdaterActorState() 
	{
		fdb_probe_actor_destroy("updater", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			;
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
		this->~GlobalConfig_UpdaterActorState();
		static_cast<GlobalConfig_UpdaterActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (self->initialized.canBeSet())
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			{
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				StrictFuture<Void> __when_expr_0 = self->cx->onConnected();
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 1;
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 0, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			TraceEvent("GlobalConfigUpdaterError").error(e);
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			StrictFuture<Void> __when_expr_4 = delay(1.0);
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_4.get(), loopDepth); };
			static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 5;
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 4, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		;
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		StrictFuture<Void> __when_expr_1 = self->refresh(self, -1);
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
		static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 2;
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 1, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		StrictFuture<Void> __when_expr_1 = self->refresh(self, -1);
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
		static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 2;
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 1, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_UpdaterActor*>(this)->ActorCallback< GlobalConfig_UpdaterActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_UpdaterActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_UpdaterActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GlobalConfig_UpdaterActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		self->initialized.send(Void());
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		self->initialized.send(Void());
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

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
	void a_exitChoose2() 
	{
		if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_UpdaterActor*>(this)->ActorCallback< GlobalConfig_UpdaterActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_UpdaterActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_UpdaterActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GlobalConfig_UpdaterActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont2loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1cont2loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1(int loopDepth) 
	{
		try {
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			StrictFuture<Void> __when_expr_2 = self->dbInfoChanged.onTrigger();
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont2loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1cont2loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont2loopBody1when1(__when_expr_2.get(), loopDepth); };
			static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 3;
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 2, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1cont2loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1cont2loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			return a_body1loopBody1Catch1(e, std::max(0, loopDepth - 1));
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		auto& history = dbInfo->history;
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (history.size() == 0)
															#line 869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		{
			return a_body1loopBody1cont2loopHead1(loopDepth); // continue
		}
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (self->lastUpdate < history[0].version)
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		{
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			StrictFuture<Void> __when_expr_3 = self->refresh(self, history.back().version);
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont2loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1cont2loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont2loopBody1cont2when1(__when_expr_3.get(), loopDepth); };
			static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 4;
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 3, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			for( const auto& vh : history ) {
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				if (vh.version <= self->lastUpdate)
															#line 895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
				{
					continue;
				}
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				for( const auto& mutation : vh.mutations.contents() ) {
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
					if (mutation.type == MutationRef::SetValue)
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
					{
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
						self->insert(mutation.param1, mutation.param2);
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
					}
					else
					{
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
						if (mutation.type == MutationRef::ClearRange)
															#line 913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
						{
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
							self->erase(KeyRangeRef(mutation.param1, mutation.param2));
															#line 917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
						}
						else
						{
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
							ASSERT(false);
															#line 923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
						}
					}
				}
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				ASSERT(vh.version > self->lastUpdate);
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				self->lastUpdate = vh.version;
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			}
			loopDepth = a_body1loopBody1cont2loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		auto& history = dbInfo->history;
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (history.size() == 0)
															#line 944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		{
			return a_body1loopBody1cont2loopHead1(loopDepth); // continue
		}
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (self->lastUpdate < history[0].version)
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		{
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			StrictFuture<Void> __when_expr_3 = self->refresh(self, history.back().version);
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont2loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1cont2loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont2loopBody1cont2when1(__when_expr_3.get(), loopDepth); };
			static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 4;
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 3, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			for( const auto& vh : history ) {
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				if (vh.version <= self->lastUpdate)
															#line 970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
				{
					continue;
				}
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				for( const auto& mutation : vh.mutations.contents() ) {
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
					if (mutation.type == MutationRef::SetValue)
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
					{
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
						self->insert(mutation.param1, mutation.param2);
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
					}
					else
					{
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
						if (mutation.type == MutationRef::ClearRange)
															#line 988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
						{
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
							self->erase(KeyRangeRef(mutation.param1, mutation.param2));
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
						}
						else
						{
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
							ASSERT(false);
															#line 998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
						}
					}
				}
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				ASSERT(vh.version > self->lastUpdate);
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				self->lastUpdate = vh.version;
															#line 1006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			}
			loopDepth = a_body1loopBody1cont2loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_UpdaterActor*>(this)->ActorCallback< GlobalConfig_UpdaterActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_UpdaterActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont2loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_UpdaterActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont2loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GlobalConfig_UpdaterActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont2loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont2loopBody1cont3(int loopDepth) 
	{
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		self->configChanged.trigger();
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2loopBody1cont17(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont5(Void const& _,int loopDepth) 
	{
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (dbInfo->history.size() > 0)
															#line 1089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		{
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			self->lastUpdate = dbInfo->history.back().version;
															#line 1093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont2loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont5(Void && _,int loopDepth) 
	{
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (dbInfo->history.size() > 0)
															#line 1103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		{
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			self->lastUpdate = dbInfo->history.back().version;
															#line 1107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont2loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_UpdaterActor*>(this)->ActorCallback< GlobalConfig_UpdaterActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_UpdaterActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont2loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_UpdaterActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont2loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< GlobalConfig_UpdaterActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont2loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1cont2loopBody1cont17(int loopDepth) 
	{
		try {
			loopDepth = a_body1loopBody1cont2loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), std::max(0, loopDepth - 1));
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
		if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_UpdaterActor*>(this)->ActorCallback< GlobalConfig_UpdaterActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_UpdaterActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_UpdaterActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< GlobalConfig_UpdaterActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	GlobalConfig* self;
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	const ClientDBInfo* dbInfo;
															#line 1268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
};
// This generated class is to be used only via updater()
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
class GlobalConfig_UpdaterActor final : public Actor<Void>, public ActorCallback< GlobalConfig_UpdaterActor, 0, Void >, public ActorCallback< GlobalConfig_UpdaterActor, 1, Void >, public ActorCallback< GlobalConfig_UpdaterActor, 2, Void >, public ActorCallback< GlobalConfig_UpdaterActor, 3, Void >, public ActorCallback< GlobalConfig_UpdaterActor, 4, Void >, public FastAllocated<GlobalConfig_UpdaterActor>, public GlobalConfig_UpdaterActorState<GlobalConfig_UpdaterActor> {
															#line 1273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
public:
	using FastAllocated<GlobalConfig_UpdaterActor>::operator new;
	using FastAllocated<GlobalConfig_UpdaterActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GlobalConfig_UpdaterActor, 0, Void >;
friend struct ActorCallback< GlobalConfig_UpdaterActor, 1, Void >;
friend struct ActorCallback< GlobalConfig_UpdaterActor, 2, Void >;
friend struct ActorCallback< GlobalConfig_UpdaterActor, 3, Void >;
friend struct ActorCallback< GlobalConfig_UpdaterActor, 4, Void >;
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	GlobalConfig_UpdaterActor(GlobalConfig* const& self,const ClientDBInfo* const& dbInfo) 
															#line 1288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		 : Actor<Void>(),
		   GlobalConfig_UpdaterActorState<GlobalConfig_UpdaterActor>(self, dbInfo)
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("updater");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GlobalConfig_UpdaterActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GlobalConfig_UpdaterActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GlobalConfig_UpdaterActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< GlobalConfig_UpdaterActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< GlobalConfig_UpdaterActor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
[[nodiscard]] Future<Void> GlobalConfig::updater( GlobalConfig* const& self, const ClientDBInfo* const& dbInfo ) {
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	return Future<Void>(new GlobalConfig_UpdaterActor(self, dbInfo));
															#line 1319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
}

#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
