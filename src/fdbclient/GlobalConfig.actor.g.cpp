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

const KeyRef fdbClientInfoTxnSampleRate = LiteralStringRef("config/fdb_client_info/client_txn_sample_rate");
const KeyRef fdbClientInfoTxnSizeLimit = LiteralStringRef("config/fdb_client_info/client_txn_size_limit");

const KeyRef transactionTagSampleRate = LiteralStringRef("config/transaction_tag_sample_rate");
const KeyRef transactionTagSampleCost = LiteralStringRef("config/transaction_tag_sample_cost");

const KeyRef samplingFrequency = LiteralStringRef("visibility/sampling/frequency");
const KeyRef samplingWindow = LiteralStringRef("visibility/sampling/window");

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
	            LiteralStringRef("0123456789\x00\x00\x00\x00"), // versionstamp
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
	// TraceEvent(SevInfo, "GlobalConfig_Insert").detail("Key", key).detail("Value", value);
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
	// TraceEvent(SevInfo, "GlobalConfig_Erase").detail("Range", range);
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

// Similar to tr.onError(), but doesn't require a DatabaseContext.
struct Backoff {
	Future<Void> onError() {
		double currentBackoff = backoff;
		backoff = std::min(backoff * CLIENT_KNOBS->BACKOFF_GROWTH_RATE, CLIENT_KNOBS->DEFAULT_MAX_BACKOFF);
		return delay(currentBackoff * deterministicRandom()->random01());
	}

private:
	double backoff = CLIENT_KNOBS->DEFAULT_BACKOFF;
};

// Older FDB versions used different keys for client profiling data. This
// function performs a one-time migration of data in these keys to the new
// global configuration key space.
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
// This generated class is to be used only via migrate()
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
template <class GlobalConfig_MigrateActor>
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
class GlobalConfig_MigrateActorState {
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
public:
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	GlobalConfig_MigrateActorState(GlobalConfig* const& self) 
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		 : self(self),
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		   migratedKey("\xff\x02/fdbClientInfo/migrated/"_sr),
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		   tr()
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
	{
		fdb_probe_actor_create("migrate", reinterpret_cast<unsigned long>(this));

	}
	~GlobalConfig_MigrateActorState() 
	{
		fdb_probe_actor_destroy("migrate", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				backoff = Backoff();
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				;
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
		this->~GlobalConfig_MigrateActorState();
		static_cast<GlobalConfig_MigrateActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (!static_cast<GlobalConfig_MigrateActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~GlobalConfig_MigrateActorState(); static_cast<GlobalConfig_MigrateActor*>(this)->destroy(); return 0; }
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		new (&static_cast<GlobalConfig_MigrateActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~GlobalConfig_MigrateActorState();
		static_cast<GlobalConfig_MigrateActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			TraceEvent(SevWarnAlways, "GlobalConfig_MigrationError").error(e);
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		tr = makeReference<ReadYourWritesTransaction>(Database(Reference<DatabaseContext>::addRef(self->cx)));
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		try {
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(migratedKey);
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state = 1;
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_MigrateActor, 0, Optional<Value> >*>(static_cast<GlobalConfig_MigrateActor*>(this)));
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
			return a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), loopDepth);
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
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			TraceEvent(SevInfo, "GlobalConfig_RetryableMigrationError").errorUnsuppressed(e).suppressFor(1.0);
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			StrictFuture<Void> __when_expr_4 = tr->onError(e);
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch2(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_4.get(), loopDepth); };
			static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state = 5;
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_MigrateActor, 4, Void >*>(static_cast<GlobalConfig_MigrateActor*>(this)));
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (migrated.present())
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		{
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (!static_cast<GlobalConfig_MigrateActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~GlobalConfig_MigrateActorState(); static_cast<GlobalConfig_MigrateActor*>(this)->destroy(); return 0; }
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			new (&static_cast<GlobalConfig_MigrateActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~GlobalConfig_MigrateActorState();
			static_cast<GlobalConfig_MigrateActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		StrictFuture<Optional<Value>> __when_expr_1 = tr->get(Key("\xff\x02/fdbClientInfo/client_txn_sample_rate/"_sr));
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state = 2;
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_MigrateActor, 1, Optional<Value> >*>(static_cast<GlobalConfig_MigrateActor*>(this)));
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& __migrated,int loopDepth) 
	{
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		migrated = __migrated;
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && __migrated,int loopDepth) 
	{
		migrated = std::move(__migrated);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_MigrateActor*>(this)->ActorCallback< GlobalConfig_MigrateActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_MigrateActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_MigrateActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GlobalConfig_MigrateActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		StrictFuture<Optional<Value>> __when_expr_2 = tr->get(Key("\xff\x02/fdbClientInfo/client_txn_size_limit/"_sr));
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state = 3;
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_MigrateActor, 2, Optional<Value> >*>(static_cast<GlobalConfig_MigrateActor*>(this)));
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Optional<Value> const& __sampleRate,int loopDepth) 
	{
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		sampleRate = __sampleRate;
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Optional<Value> && __sampleRate,int loopDepth) 
	{
		sampleRate = std::move(__sampleRate);
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_MigrateActor*>(this)->ActorCallback< GlobalConfig_MigrateActor, 1, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_MigrateActor, 1, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_MigrateActor, 1, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GlobalConfig_MigrateActor, 1, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont5(int loopDepth) 
	{
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		tr->setOption(FDBTransactionOptions::SPECIAL_KEY_SPACE_ENABLE_WRITES);
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		tr->set(migratedKey.contents(), "1"_sr);
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (sampleRate.present())
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		{
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			const double sampleRateDbl = BinaryReader::fromStringRef<double>(sampleRate.get().contents(), Unversioned());
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			Tuple rate = Tuple().appendDouble(sampleRateDbl);
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			tr->set(GlobalConfig::prefixedKey(fdbClientInfoTxnSampleRate), rate.pack());
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		}
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (sizeLimit.present())
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		{
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			const int64_t sizeLimitInt = BinaryReader::fromStringRef<int64_t>(sizeLimit.get().contents(), Unversioned());
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			Tuple size = Tuple().append(sizeLimitInt);
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			tr->set(GlobalConfig::prefixedKey(fdbClientInfoTxnSizeLimit), size.pack());
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		}
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		StrictFuture<Void> __when_expr_3 = tr->commit();
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state = 4;
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_MigrateActor, 3, Void >*>(static_cast<GlobalConfig_MigrateActor*>(this)));
															#line 565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Optional<Value> const& __sizeLimit,int loopDepth) 
	{
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		sizeLimit = __sizeLimit;
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Optional<Value> && __sizeLimit,int loopDepth) 
	{
		sizeLimit = std::move(__sizeLimit);
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_MigrateActor*>(this)->ActorCallback< GlobalConfig_MigrateActor, 2, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_MigrateActor, 2, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_MigrateActor, 2, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GlobalConfig_MigrateActor, 2, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_MigrateActor*>(this)->ActorCallback< GlobalConfig_MigrateActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_MigrateActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_MigrateActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< GlobalConfig_MigrateActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		tr.clear();
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		StrictFuture<Void> __when_expr_5 = backoff.onError();
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch2(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1cont1when1(__when_expr_5.get(), loopDepth); };
		static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state = 6;
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_MigrateActor, 5, Void >*>(static_cast<GlobalConfig_MigrateActor*>(this)));
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		tr.clear();
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		StrictFuture<Void> __when_expr_5 = backoff.onError();
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch2(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1cont1when1(__when_expr_5.get(), loopDepth); };
		static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state = 6;
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_MigrateActor, 5, Void >*>(static_cast<GlobalConfig_MigrateActor*>(this)));
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
	void a_exitChoose5() 
	{
		if (static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_MigrateActor*>(this)->ActorCallback< GlobalConfig_MigrateActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_MigrateActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_MigrateActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< GlobalConfig_MigrateActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1loopBody1Catch1cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_MigrateActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_MigrateActor*>(this)->ActorCallback< GlobalConfig_MigrateActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_MigrateActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1Catch1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_MigrateActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1Catch1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< GlobalConfig_MigrateActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1cont3(int loopDepth) 
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
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	GlobalConfig* self;
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	Key migratedKey;
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	Backoff backoff;
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	Optional<Value> migrated;
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	Optional<Value> sampleRate;
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	Optional<Value> sizeLimit;
															#line 913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
};
// This generated class is to be used only via migrate()
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
class GlobalConfig_MigrateActor final : public Actor<Void>, public ActorCallback< GlobalConfig_MigrateActor, 0, Optional<Value> >, public ActorCallback< GlobalConfig_MigrateActor, 1, Optional<Value> >, public ActorCallback< GlobalConfig_MigrateActor, 2, Optional<Value> >, public ActorCallback< GlobalConfig_MigrateActor, 3, Void >, public ActorCallback< GlobalConfig_MigrateActor, 4, Void >, public ActorCallback< GlobalConfig_MigrateActor, 5, Void >, public FastAllocated<GlobalConfig_MigrateActor>, public GlobalConfig_MigrateActorState<GlobalConfig_MigrateActor> {
															#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
public:
	using FastAllocated<GlobalConfig_MigrateActor>::operator new;
	using FastAllocated<GlobalConfig_MigrateActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GlobalConfig_MigrateActor, 0, Optional<Value> >;
friend struct ActorCallback< GlobalConfig_MigrateActor, 1, Optional<Value> >;
friend struct ActorCallback< GlobalConfig_MigrateActor, 2, Optional<Value> >;
friend struct ActorCallback< GlobalConfig_MigrateActor, 3, Void >;
friend struct ActorCallback< GlobalConfig_MigrateActor, 4, Void >;
friend struct ActorCallback< GlobalConfig_MigrateActor, 5, Void >;
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	GlobalConfig_MigrateActor(GlobalConfig* const& self) 
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		 : Actor<Void>(),
		   GlobalConfig_MigrateActorState<GlobalConfig_MigrateActor>(self)
	{
		fdb_probe_actor_enter("migrate", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("migrate");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("migrate", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GlobalConfig_MigrateActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GlobalConfig_MigrateActor, 1, Optional<Value> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GlobalConfig_MigrateActor, 2, Optional<Value> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< GlobalConfig_MigrateActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< GlobalConfig_MigrateActor, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< GlobalConfig_MigrateActor, 5, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
[[nodiscard]] Future<Void> GlobalConfig::migrate( GlobalConfig* const& self ) {
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	return Future<Void>(new GlobalConfig_MigrateActor(self));
															#line 966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
}

#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"

// Updates local copy of global configuration by reading the entire key-range
// from storage.
															#line 973 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
// This generated class is to be used only via refresh()
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
template <class GlobalConfig_RefreshActor>
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
class GlobalConfig_RefreshActorState {
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
public:
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	GlobalConfig_RefreshActorState(GlobalConfig* const& self) 
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		 : self(self)
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			self->erase(KeyRangeRef(""_sr, "\xff"_sr));
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			backoff = Backoff();
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			tr = Reference<ReadYourWritesTransaction>();
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			;
															#line 1007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
	int a_body1cont1(int loopDepth) 
	{
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (!static_cast<GlobalConfig_RefreshActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~GlobalConfig_RefreshActorState(); static_cast<GlobalConfig_RefreshActor*>(this)->destroy(); return 0; }
															#line 1030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		new (&static_cast<GlobalConfig_RefreshActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~GlobalConfig_RefreshActorState();
		static_cast<GlobalConfig_RefreshActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			tr = makeReference<ReadYourWritesTransaction>(Database(Reference<DatabaseContext>::addRef(self->cx)));
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(globalConfigDataKeys, CLIENT_KNOBS->TOO_MANY);
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state = 1;
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_RefreshActor, 0, RangeResult >*>(static_cast<GlobalConfig_RefreshActor*>(this)));
															#line 1063 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
															#line 251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			TraceEvent("GlobalConfigRefreshError").errorUnsuppressed(e).suppressFor(1.0);
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr->onError(e);
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state = 2;
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_RefreshActor, 1, Void >*>(static_cast<GlobalConfig_RefreshActor*>(this)));
															#line 1107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(RangeResult const& result,int loopDepth) 
	{
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		for( const auto& kv : result ) {
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			KeyRef systemKey = kv.key.removePrefix(globalConfigKeysPrefix);
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			self->insert(systemKey, kv.value);
															#line 1126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		}
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont2(RangeResult && result,int loopDepth) 
	{
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		for( const auto& kv : result ) {
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			KeyRef systemKey = kv.key.removePrefix(globalConfigKeysPrefix);
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			self->insert(systemKey, kv.value);
															#line 1140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		}
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& result,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(result, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && result,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(result), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_RefreshActor*>(this)->ActorCallback< GlobalConfig_RefreshActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_RefreshActor, 0, RangeResult >*,RangeResult const& value) 
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
	void a_callback_fire(ActorCallback< GlobalConfig_RefreshActor, 0, RangeResult >*,RangeResult && value) 
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
	void a_callback_error(ActorCallback< GlobalConfig_RefreshActor, 0, RangeResult >*,Error err) 
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
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		tr.clear();
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		StrictFuture<Void> __when_expr_2 = backoff.onError();
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state = 3;
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_RefreshActor, 2, Void >*>(static_cast<GlobalConfig_RefreshActor*>(this)));
															#line 1222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		tr.clear();
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		StrictFuture<Void> __when_expr_2 = backoff.onError();
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1cont1when1(__when_expr_2.get(), loopDepth); };
		static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state = 3;
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_RefreshActor, 2, Void >*>(static_cast<GlobalConfig_RefreshActor*>(this)));
															#line 1240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
	int a_body1loopBody1Catch1cont2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_RefreshActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_RefreshActor*>(this)->ActorCallback< GlobalConfig_RefreshActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_RefreshActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("refresh", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refresh", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_RefreshActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("refresh", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refresh", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GlobalConfig_RefreshActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("refresh", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("refresh", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	GlobalConfig* self;
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	Backoff backoff;
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
};
// This generated class is to be used only via refresh()
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
class GlobalConfig_RefreshActor final : public Actor<Void>, public ActorCallback< GlobalConfig_RefreshActor, 0, RangeResult >, public ActorCallback< GlobalConfig_RefreshActor, 1, Void >, public ActorCallback< GlobalConfig_RefreshActor, 2, Void >, public FastAllocated<GlobalConfig_RefreshActor>, public GlobalConfig_RefreshActorState<GlobalConfig_RefreshActor> {
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
public:
	using FastAllocated<GlobalConfig_RefreshActor>::operator new;
	using FastAllocated<GlobalConfig_RefreshActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GlobalConfig_RefreshActor, 0, RangeResult >;
friend struct ActorCallback< GlobalConfig_RefreshActor, 1, Void >;
friend struct ActorCallback< GlobalConfig_RefreshActor, 2, Void >;
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	GlobalConfig_RefreshActor(GlobalConfig* const& self) 
															#line 1407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		 : Actor<Void>(),
		   GlobalConfig_RefreshActorState<GlobalConfig_RefreshActor>(self)
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
		case 1: this->a_callback_error((ActorCallback< GlobalConfig_RefreshActor, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GlobalConfig_RefreshActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GlobalConfig_RefreshActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
[[nodiscard]] Future<Void> GlobalConfig::refresh( GlobalConfig* const& self ) {
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	return Future<Void>(new GlobalConfig_RefreshActor(self));
															#line 1436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
}

#line 260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"

// Applies updates to the local copy of the global configuration when this
// process receives an updated history.
															#line 1443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
// This generated class is to be used only via updater()
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
template <class GlobalConfig_UpdaterActor>
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
class GlobalConfig_UpdaterActorState {
															#line 1449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
public:
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	GlobalConfig_UpdaterActorState(GlobalConfig* const& self,const ClientDBInfo* const& dbInfo) 
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		 : self(self),
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		   dbInfo(dbInfo)
															#line 1458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			;
															#line 1473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (self->initialized.canBeSet())
															#line 1504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			{
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				StrictFuture<Void> __when_expr_0 = self->cx->onConnected();
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 1;
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 0, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 1515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			TraceEvent("GlobalConfigUpdaterError").error(e);
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			StrictFuture<Void> __when_expr_5 = delay(1.0);
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_5.get(), loopDepth); };
			static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 6;
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 5, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 1551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		;
															#line 1566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		StrictFuture<Void> __when_expr_1 = self->migrate(self);
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
		static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 2;
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 1, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 1582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		StrictFuture<Void> __when_expr_1 = self->migrate(self);
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
		static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 2;
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 1, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 1598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		StrictFuture<Void> __when_expr_2 = self->refresh(self);
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont4when1(__when_expr_2.get(), loopDepth); };
		static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 3;
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 2, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 1677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		StrictFuture<Void> __when_expr_2 = self->refresh(self);
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont4when1(__when_expr_2.get(), loopDepth); };
		static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 3;
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 2, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 1693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = 0;

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
	int a_body1loopBody1cont5(Void const& _,int loopDepth) 
	{
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		self->initialized.send(Void());
															#line 1765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5(Void && _,int loopDepth) 
	{
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		self->initialized.send(Void());
															#line 1774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(std::move(_), loopDepth);

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
			a_body1loopBody1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_UpdaterActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GlobalConfig_UpdaterActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 2);

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
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			StrictFuture<Void> __when_expr_3 = self->dbInfoChanged.onTrigger();
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont2loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1cont2loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont2loopBody1when1(__when_expr_3.get(), loopDepth); };
			static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 4;
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 3, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 1861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			return a_body1loopBody1Catch1(e, std::max(0, loopDepth - 1));
															#line 1883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		auto& history = dbInfo->history;
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (history.size() == 0)
															#line 1899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		{
			return a_body1loopBody1cont2loopHead1(loopDepth); // continue
		}
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (self->lastUpdate < history[0].version)
															#line 1905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		{
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			StrictFuture<Void> __when_expr_4 = self->refresh(self);
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont2loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1loopBody1cont2loopBody1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1loopBody1cont2loopBody1cont2when1(__when_expr_4.get(), loopDepth); };
			static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 5;
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 4, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 1916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			for( const auto& vh : history ) {
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				if (vh.version <= self->lastUpdate)
															#line 1925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
				{
					continue;
				}
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				for( const auto& mutation : vh.mutations.contents() ) {
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
					if (mutation.type == MutationRef::SetValue)
															#line 1933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
					{
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
						self->insert(mutation.param1, mutation.param2);
															#line 1937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
					}
					else
					{
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
						if (mutation.type == MutationRef::ClearRange)
															#line 1943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
						{
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
							self->erase(KeyRangeRef(mutation.param1, mutation.param2));
															#line 1947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
						}
						else
						{
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
							ASSERT(false);
															#line 1953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
						}
					}
				}
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				ASSERT(vh.version > self->lastUpdate);
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				self->lastUpdate = vh.version;
															#line 1961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			}
			loopDepth = a_body1loopBody1cont2loopBody1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		auto& history = dbInfo->history;
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (history.size() == 0)
															#line 1974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		{
			return a_body1loopBody1cont2loopHead1(loopDepth); // continue
		}
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (self->lastUpdate < history[0].version)
															#line 1980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		{
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			StrictFuture<Void> __when_expr_4 = self->refresh(self);
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont2loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1loopBody1cont2loopBody1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1loopBody1cont2loopBody1cont2when1(__when_expr_4.get(), loopDepth); };
			static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 5;
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< GlobalConfig_UpdaterActor, 4, Void >*>(static_cast<GlobalConfig_UpdaterActor*>(this)));
															#line 1991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			for( const auto& vh : history ) {
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				if (vh.version <= self->lastUpdate)
															#line 2000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
				{
					continue;
				}
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				for( const auto& mutation : vh.mutations.contents() ) {
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
					if (mutation.type == MutationRef::SetValue)
															#line 2008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
					{
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
						self->insert(mutation.param1, mutation.param2);
															#line 2012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
					}
					else
					{
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
						if (mutation.type == MutationRef::ClearRange)
															#line 2018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
						{
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
							self->erase(KeyRangeRef(mutation.param1, mutation.param2));
															#line 2022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
						}
						else
						{
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
							ASSERT(false);
															#line 2028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
						}
					}
				}
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				ASSERT(vh.version > self->lastUpdate);
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
				self->lastUpdate = vh.version;
															#line 2036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
			a_body1loopBody1cont2loopBody1when1(value, 0);
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
			a_body1loopBody1cont2loopBody1when1(std::move(value), 0);
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
	int a_body1loopBody1cont2loopBody1cont3(int loopDepth) 
	{
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		self->configChanged.trigger();
															#line 2110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2loopBody1cont17(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont5(Void const& _,int loopDepth) 
	{
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (dbInfo->history.size() > 0)
															#line 2119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		{
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			self->lastUpdate = dbInfo->history.back().version;
															#line 2123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		}
		loopDepth = a_body1loopBody1cont2loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont5(Void && _,int loopDepth) 
	{
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
		if (dbInfo->history.size() > 0)
															#line 2133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
		{
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
			self->lastUpdate = dbInfo->history.back().version;
															#line 2137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
			a_body1loopBody1cont2loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_UpdaterActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont2loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< GlobalConfig_UpdaterActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont2loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 4);

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
	void a_exitChoose6() 
	{
		if (static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state > 0) static_cast<GlobalConfig_UpdaterActor*>(this)->actor_wait_state = 0;
		static_cast<GlobalConfig_UpdaterActor*>(this)->ActorCallback< GlobalConfig_UpdaterActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GlobalConfig_UpdaterActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< GlobalConfig_UpdaterActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< GlobalConfig_UpdaterActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("updater", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("updater", reinterpret_cast<unsigned long>(this), 5);

	}
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	GlobalConfig* self;
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	const ClientDBInfo* dbInfo;
															#line 2298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
};
// This generated class is to be used only via updater()
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
class GlobalConfig_UpdaterActor final : public Actor<Void>, public ActorCallback< GlobalConfig_UpdaterActor, 0, Void >, public ActorCallback< GlobalConfig_UpdaterActor, 1, Void >, public ActorCallback< GlobalConfig_UpdaterActor, 2, Void >, public ActorCallback< GlobalConfig_UpdaterActor, 3, Void >, public ActorCallback< GlobalConfig_UpdaterActor, 4, Void >, public ActorCallback< GlobalConfig_UpdaterActor, 5, Void >, public FastAllocated<GlobalConfig_UpdaterActor>, public GlobalConfig_UpdaterActorState<GlobalConfig_UpdaterActor> {
															#line 2303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
friend struct ActorCallback< GlobalConfig_UpdaterActor, 5, Void >;
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	GlobalConfig_UpdaterActor(GlobalConfig* const& self,const ClientDBInfo* const& dbInfo) 
															#line 2319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
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
		case 6: this->a_callback_error((ActorCallback< GlobalConfig_UpdaterActor, 5, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
[[nodiscard]] Future<Void> GlobalConfig::updater( GlobalConfig* const& self, const ClientDBInfo* const& dbInfo ) {
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
	return Future<Void>(new GlobalConfig_UpdaterActor(self, dbInfo));
															#line 2351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.g.cpp"
}

#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/GlobalConfig.actor.cpp"
