#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
/*
 * ReadYourWrites.actor.cpp
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

#include "fdbclient/ReadYourWrites.h"
#include "fdbclient/NativeAPI.actor.h"
#include "fdbclient/Atomic.h"
#include "fdbclient/DatabaseContext.h"
#include "fdbclient/SpecialKeySpace.actor.h"
#include "fdbclient/StatusClient.h"
#include "fdbclient/MonitorLeader.h"
#include "flow/Util.h"
#include "flow/actorcompiler.h" // This must be the last #include.

class RYWImpl {
public:
	template <class Iter>
	static void dump(Iter it) {
		it.skip(allKeys.begin);
		Arena arena;
		while (true) {
			Optional<StringRef> key = StringRef();
			if (it.is_kv()) {
				auto kv = it.kv(arena);
				if (kv)
					key = kv->key;
			}
			TraceEvent("RYWDump")
			    .detail("Begin", it.beginKey())
			    .detail("End", it.endKey())
			    .detail("Unknown", it.is_unknown_range())
			    .detail("Empty", it.is_empty_range())
			    .detail("KV", it.is_kv())
			    .detail("Key", key.get());
			if (it.endKey() == allKeys.end)
				break;
			++it;
		}
	}

	struct GetValueReq {
		explicit GetValueReq(Key key) : key(key) {}
		Key key;
		typedef Optional<Value> Result;
	};

	struct GetKeyReq {
		explicit GetKeyReq(KeySelector key) : key(key) {}
		KeySelector key;
		typedef Key Result;
	};

	template <bool reverse>
	struct GetRangeReq {
		GetRangeReq(KeySelector begin, KeySelector end, GetRangeLimits limits)
		  : begin(begin), end(end), limits(limits) {}
		KeySelector begin, end;
		GetRangeLimits limits;
		using Result = RangeResult;
	};

	template <bool reverse>
	struct GetMappedRangeReq {
		GetMappedRangeReq(KeySelector begin, KeySelector end, Key mapper, GetRangeLimits limits)
		  : begin(begin), end(end), mapper(mapper), limits(limits) {}
		KeySelector begin, end;
		Key mapper;
		GetRangeLimits limits;
		using Result = MappedRangeResult;
	};

	// read() Performs a read (get, getKey, getRange, etc), in the context of the given transaction.  Snapshot or RYW
	// reads are distingushed by the type Iter being SnapshotCache::iterator or RYWIterator. Fills in the snapshot cache
	// as a side effect but does not affect conflict ranges. Some (indicated) overloads of read are required to update
	// the given *it to point to the key that was read, so that the corresponding overload of addConflictRange() can
	// make use of it.

																#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via read()
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Iter, class ReadActor>
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadActorState {
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadActorState(ReadYourWritesTransaction* const& ryw,GetValueReq const& read,Iter* const& it) 
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw(ryw),
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   read(read),
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   it(it)
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("read", reinterpret_cast<unsigned long>(this));

	}
	~ReadActorState() 
	{
		fdb_probe_actor_destroy("read", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (ryw->options.bypassUnreadable)
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				it->bypassUnreadableProtection();
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			it->skip(read.key);
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			dependent = it->is_dependent();
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (it->is_kv())
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				const KeyValueRef* result = it->kv(ryw->arena);
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (result != nullptr)
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					if (!static_cast<ReadActor*>(this)->SAV<Optional<Value>>::futures) { (void)(result->value); this->~ReadActorState(); static_cast<ReadActor*>(this)->destroy(); return 0; }
															#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					new (&static_cast<ReadActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(result->value);
					this->~ReadActorState();
					static_cast<ReadActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
				else
				{
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					if (!static_cast<ReadActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>()); this->~ReadActorState(); static_cast<ReadActor*>(this)->destroy(); return 0; }
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					new (&static_cast<ReadActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>());
					this->~ReadActorState();
					static_cast<ReadActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
			else
			{
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (it->is_empty_range())
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					if (!static_cast<ReadActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>()); this->~ReadActorState(); static_cast<ReadActor*>(this)->destroy(); return 0; }
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					new (&static_cast<ReadActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>());
					this->~ReadActorState();
					static_cast<ReadActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
				else
				{
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					StrictFuture<Optional<Value>> __when_expr_0 = ryw->tr.get(read.key, Snapshot::True);
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					if (static_cast<ReadActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<ReadActor*>(this)->actor_wait_state = 1;
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadActor, 0, Optional<Value> >*>(static_cast<ReadActor*>(this)));
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					loopDepth = 0;
				}
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
		this->~ReadActorState();
		static_cast<ReadActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont8(Optional<Value> const& res,int loopDepth) 
	{
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		KeyRef k(ryw->arena, read.key);
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (res.present())
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (ryw->cache.insert(k, res.get()))
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				ryw->arena.dependsOn(res.get().arena());
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!dependent)
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (!static_cast<ReadActor*>(this)->SAV<Optional<Value>>::futures) { (void)(res); this->~ReadActorState(); static_cast<ReadActor*>(this)->destroy(); return 0; }
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				new (&static_cast<ReadActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(res);
				this->~ReadActorState();
				static_cast<ReadActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
		else
		{
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->cache.insert(k, Optional<ValueRef>());
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!dependent)
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (!static_cast<ReadActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>()); this->~ReadActorState(); static_cast<ReadActor*>(this)->destroy(); return 0; }
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				new (&static_cast<ReadActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>());
				this->~ReadActorState();
				static_cast<ReadActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		it->skip(k);
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		ASSERT(it->is_kv());
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		const KeyValueRef* result = it->kv(ryw->arena);
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (result != nullptr)
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<ReadActor*>(this)->SAV<Optional<Value>>::futures) { (void)(result->value); this->~ReadActorState(); static_cast<ReadActor*>(this)->destroy(); return 0; }
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<ReadActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(result->value);
			this->~ReadActorState();
			static_cast<ReadActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<ReadActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>()); this->~ReadActorState(); static_cast<ReadActor*>(this)->destroy(); return 0; }
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<ReadActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>());
			this->~ReadActorState();
			static_cast<ReadActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1cont8(Optional<Value> && res,int loopDepth) 
	{
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		KeyRef k(ryw->arena, read.key);
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (res.present())
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (ryw->cache.insert(k, res.get()))
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				ryw->arena.dependsOn(res.get().arena());
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!dependent)
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (!static_cast<ReadActor*>(this)->SAV<Optional<Value>>::futures) { (void)(res); this->~ReadActorState(); static_cast<ReadActor*>(this)->destroy(); return 0; }
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				new (&static_cast<ReadActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(res);
				this->~ReadActorState();
				static_cast<ReadActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
		else
		{
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->cache.insert(k, Optional<ValueRef>());
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!dependent)
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (!static_cast<ReadActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>()); this->~ReadActorState(); static_cast<ReadActor*>(this)->destroy(); return 0; }
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				new (&static_cast<ReadActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>());
				this->~ReadActorState();
				static_cast<ReadActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		it->skip(k);
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		ASSERT(it->is_kv());
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		const KeyValueRef* result = it->kv(ryw->arena);
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (result != nullptr)
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<ReadActor*>(this)->SAV<Optional<Value>>::futures) { (void)(result->value); this->~ReadActorState(); static_cast<ReadActor*>(this)->destroy(); return 0; }
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<ReadActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(result->value);
			this->~ReadActorState();
			static_cast<ReadActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<ReadActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>()); this->~ReadActorState(); static_cast<ReadActor*>(this)->destroy(); return 0; }
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<ReadActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>());
			this->~ReadActorState();
			static_cast<ReadActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& res,int loopDepth) 
	{
		loopDepth = a_body1cont8(res, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && res,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(res), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadActor*>(this)->actor_wait_state > 0) static_cast<ReadActor*>(this)->actor_wait_state = 0;
		static_cast<ReadActor*>(this)->ActorCallback< ReadActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< ReadActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetValueReq read;
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Iter* it;
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	bool dependent;
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via read()
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Iter>
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadActor final : public Actor<Optional<Value>>, public ActorCallback< ReadActor<Iter>, 0, Optional<Value> >, public FastAllocated<ReadActor<Iter>>, public ReadActorState<Iter, ReadActor<Iter>> {
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<ReadActor<Iter>>::operator new;
	using FastAllocated<ReadActor<Iter>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<Value>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadActor<Iter>, 0, Optional<Value> >;
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadActor(ReadYourWritesTransaction* const& ryw,GetValueReq const& read,Iter* const& it) 
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<Optional<Value>>(),
		   ReadActorState<Iter, ReadActor<Iter>>(ryw, read, it)
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("read");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadActor<Iter>, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Iter>
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<Optional<Value>> read( ReadYourWritesTransaction* const& ryw, GetValueReq const& read, Iter* const& it ) {
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<Optional<Value>>(new ReadActor<Iter>(ryw, read, it));
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"

																#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via read()
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Iter, class ReadActor1>
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadActor1State {
															#line 496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadActor1State(ReadYourWritesTransaction* const& ryw,GetKeyReq const& read,Iter* const& it) 
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw(ryw),
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   read(read),
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   it(it)
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("read", reinterpret_cast<unsigned long>(this));

	}
	~ReadActor1State() 
	{
		fdb_probe_actor_destroy("read", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (read.key.offset > 0)
															#line 522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				StrictFuture<RangeResult> __when_expr_0 = getRangeValue(ryw, read.key, firstGreaterOrEqual(ryw->getMaxReadKey()), GetRangeLimits(1), it);
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (static_cast<ReadActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ReadActor1*>(this)->actor_wait_state = 1;
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadActor1, 0, RangeResult >*>(static_cast<ReadActor1*>(this)));
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				read.key.offset++;
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				StrictFuture<RangeResult> __when_expr_1 = getRangeValueBack(ryw, firstGreaterOrEqual(allKeys.begin), read.key, GetRangeLimits(1), it);
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (static_cast<ReadActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
				static_cast<ReadActor1*>(this)->actor_wait_state = 2;
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadActor1, 1, RangeResult >*>(static_cast<ReadActor1*>(this)));
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~ReadActor1State();
		static_cast<ReadActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(RangeResult const& result,int loopDepth) 
	{
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (result.readToBegin)
															#line 573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<ReadActor1*>(this)->SAV<Key>::futures) { (void)(allKeys.begin); this->~ReadActor1State(); static_cast<ReadActor1*>(this)->destroy(); return 0; }
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<ReadActor1*>(this)->SAV< Key >::value()) Key(allKeys.begin);
			this->~ReadActor1State();
			static_cast<ReadActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (result.readThroughEnd || !result.size())
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<ReadActor1*>(this)->SAV<Key>::futures) { (void)(ryw->getMaxReadKey()); this->~ReadActor1State(); static_cast<ReadActor1*>(this)->destroy(); return 0; }
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<ReadActor1*>(this)->SAV< Key >::value()) Key(ryw->getMaxReadKey());
			this->~ReadActor1State();
			static_cast<ReadActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadActor1*>(this)->SAV<Key>::futures) { (void)(result[0].key); this->~ReadActor1State(); static_cast<ReadActor1*>(this)->destroy(); return 0; }
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadActor1*>(this)->SAV< Key >::value()) Key(result[0].key);
		this->~ReadActor1State();
		static_cast<ReadActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(RangeResult && result,int loopDepth) 
	{
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (result.readToBegin)
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<ReadActor1*>(this)->SAV<Key>::futures) { (void)(allKeys.begin); this->~ReadActor1State(); static_cast<ReadActor1*>(this)->destroy(); return 0; }
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<ReadActor1*>(this)->SAV< Key >::value()) Key(allKeys.begin);
			this->~ReadActor1State();
			static_cast<ReadActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (result.readThroughEnd || !result.size())
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<ReadActor1*>(this)->SAV<Key>::futures) { (void)(ryw->getMaxReadKey()); this->~ReadActor1State(); static_cast<ReadActor1*>(this)->destroy(); return 0; }
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<ReadActor1*>(this)->SAV< Key >::value()) Key(ryw->getMaxReadKey());
			this->~ReadActor1State();
			static_cast<ReadActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadActor1*>(this)->SAV<Key>::futures) { (void)(result[0].key); this->~ReadActor1State(); static_cast<ReadActor1*>(this)->destroy(); return 0; }
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadActor1*>(this)->SAV< Key >::value()) Key(result[0].key);
		this->~ReadActor1State();
		static_cast<ReadActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& result,int loopDepth) 
	{
		loopDepth = a_body1cont2(result, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && result,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(result), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadActor1*>(this)->actor_wait_state > 0) static_cast<ReadActor1*>(this)->actor_wait_state = 0;
		static_cast<ReadActor1*>(this)->ActorCallback< ReadActor1, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ReadActor1, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadActor1, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadActor1, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont6(RangeResult const& result,int loopDepth) 
	{
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (result.readThroughEnd)
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<ReadActor1*>(this)->SAV<Key>::futures) { (void)(ryw->getMaxReadKey()); this->~ReadActor1State(); static_cast<ReadActor1*>(this)->destroy(); return 0; }
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<ReadActor1*>(this)->SAV< Key >::value()) Key(ryw->getMaxReadKey());
			this->~ReadActor1State();
			static_cast<ReadActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (result.readToBegin || !result.size())
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<ReadActor1*>(this)->SAV<Key>::futures) { (void)(allKeys.begin); this->~ReadActor1State(); static_cast<ReadActor1*>(this)->destroy(); return 0; }
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<ReadActor1*>(this)->SAV< Key >::value()) Key(allKeys.begin);
			this->~ReadActor1State();
			static_cast<ReadActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadActor1*>(this)->SAV<Key>::futures) { (void)(result[0].key); this->~ReadActor1State(); static_cast<ReadActor1*>(this)->destroy(); return 0; }
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadActor1*>(this)->SAV< Key >::value()) Key(result[0].key);
		this->~ReadActor1State();
		static_cast<ReadActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(RangeResult && result,int loopDepth) 
	{
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (result.readThroughEnd)
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<ReadActor1*>(this)->SAV<Key>::futures) { (void)(ryw->getMaxReadKey()); this->~ReadActor1State(); static_cast<ReadActor1*>(this)->destroy(); return 0; }
															#line 748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<ReadActor1*>(this)->SAV< Key >::value()) Key(ryw->getMaxReadKey());
			this->~ReadActor1State();
			static_cast<ReadActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (result.readToBegin || !result.size())
															#line 756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<ReadActor1*>(this)->SAV<Key>::futures) { (void)(allKeys.begin); this->~ReadActor1State(); static_cast<ReadActor1*>(this)->destroy(); return 0; }
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<ReadActor1*>(this)->SAV< Key >::value()) Key(allKeys.begin);
			this->~ReadActor1State();
			static_cast<ReadActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadActor1*>(this)->SAV<Key>::futures) { (void)(result[0].key); this->~ReadActor1State(); static_cast<ReadActor1*>(this)->destroy(); return 0; }
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadActor1*>(this)->SAV< Key >::value()) Key(result[0].key);
		this->~ReadActor1State();
		static_cast<ReadActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(RangeResult const& result,int loopDepth) 
	{
		loopDepth = a_body1cont6(result, loopDepth);

		return loopDepth;
	}
	int a_body1when2(RangeResult && result,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(result), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReadActor1*>(this)->actor_wait_state > 0) static_cast<ReadActor1*>(this)->actor_wait_state = 0;
		static_cast<ReadActor1*>(this)->ActorCallback< ReadActor1, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ReadActor1, 1, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadActor1, 1, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadActor1, 1, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetKeyReq read;
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Iter* it;
															#line 845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via read()
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Iter>
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadActor1 final : public Actor<Key>, public ActorCallback< ReadActor1<Iter>, 0, RangeResult >, public ActorCallback< ReadActor1<Iter>, 1, RangeResult >, public FastAllocated<ReadActor1<Iter>>, public ReadActor1State<Iter, ReadActor1<Iter>> {
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<ReadActor1<Iter>>::operator new;
	using FastAllocated<ReadActor1<Iter>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Key>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadActor1<Iter>, 0, RangeResult >;
friend struct ActorCallback< ReadActor1<Iter>, 1, RangeResult >;
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadActor1(ReadYourWritesTransaction* const& ryw,GetKeyReq const& read,Iter* const& it) 
															#line 864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<Key>(),
		   ReadActor1State<Iter, ReadActor1<Iter>>(ryw, read, it)
	{
		fdb_probe_actor_enter("read", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("read");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("read", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadActor1<Iter>, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReadActor1<Iter>, 1, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Iter>
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<Key> read( ReadYourWritesTransaction* const& ryw, GetKeyReq const& read, Iter* const& it ) {
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<Key>(new ReadActor1<Iter>(ryw, read, it));
															#line 894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"


	template <class Iter>
	static Future<RangeResult> read(ReadYourWritesTransaction* ryw, GetRangeReq<false> read, Iter* it) {
		return getRangeValue(ryw, read.begin, read.end, read.limits, it);
	};

	template <class Iter>
	static Future<RangeResult> read(ReadYourWritesTransaction* ryw, GetRangeReq<true> read, Iter* it) {
		return getRangeValueBack(ryw, read.begin, read.end, read.limits, it);
	};

	// readThrough() performs a read in the RYW disabled case, passing it on relatively directly to the underlying
	// transaction. Responsible for clipping results to the non-system keyspace when appropriate, since NativeAPI
	// doesn't do that.

	static Future<Optional<Value>> readThrough(ReadYourWritesTransaction* ryw, GetValueReq read, Snapshot snapshot) {
		return ryw->tr.get(read.key, snapshot);
	}

																#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via readThrough()
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class ReadThroughActor>
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadThroughActorState {
															#line 924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadThroughActorState(ReadYourWritesTransaction* const& ryw,GetKeyReq const& read,Snapshot const& snapshot) 
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw(ryw),
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   read(read),
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   snapshot(snapshot)
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("readThrough", reinterpret_cast<unsigned long>(this));

	}
	~ReadThroughActorState() 
	{
		fdb_probe_actor_destroy("readThrough", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<Key> __when_expr_0 = ryw->tr.getKey(read.key, snapshot);
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (static_cast<ReadThroughActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReadThroughActor*>(this)->actor_wait_state = 1;
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadThroughActor, 0, Key >*>(static_cast<ReadThroughActor*>(this)));
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~ReadThroughActorState();
		static_cast<ReadThroughActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Key const& key,int loopDepth) 
	{
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (ryw->getMaxReadKey() < key)
															#line 980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<ReadThroughActor*>(this)->SAV<Key>::futures) { (void)(ryw->getMaxReadKey()); this->~ReadThroughActorState(); static_cast<ReadThroughActor*>(this)->destroy(); return 0; }
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<ReadThroughActor*>(this)->SAV< Key >::value()) Key(ryw->getMaxReadKey());
			this->~ReadThroughActorState();
			static_cast<ReadThroughActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadThroughActor*>(this)->SAV<Key>::futures) { (void)(key); this->~ReadThroughActorState(); static_cast<ReadThroughActor*>(this)->destroy(); return 0; }
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadThroughActor*>(this)->SAV< Key >::value()) Key(key);
		this->~ReadThroughActorState();
		static_cast<ReadThroughActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Key && key,int loopDepth) 
	{
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (ryw->getMaxReadKey() < key)
															#line 1004 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<ReadThroughActor*>(this)->SAV<Key>::futures) { (void)(ryw->getMaxReadKey()); this->~ReadThroughActorState(); static_cast<ReadThroughActor*>(this)->destroy(); return 0; }
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<ReadThroughActor*>(this)->SAV< Key >::value()) Key(ryw->getMaxReadKey());
			this->~ReadThroughActorState();
			static_cast<ReadThroughActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadThroughActor*>(this)->SAV<Key>::futures) { (void)(key); this->~ReadThroughActorState(); static_cast<ReadThroughActor*>(this)->destroy(); return 0; }
															#line 1016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadThroughActor*>(this)->SAV< Key >::value()) Key(key);
		this->~ReadThroughActorState();
		static_cast<ReadThroughActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Key const& key,int loopDepth) 
	{
		loopDepth = a_body1cont1(key, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Key && key,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(key), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadThroughActor*>(this)->actor_wait_state > 0) static_cast<ReadThroughActor*>(this)->actor_wait_state = 0;
		static_cast<ReadThroughActor*>(this)->ActorCallback< ReadThroughActor, 0, Key >::remove();

	}
	void a_callback_fire(ActorCallback< ReadThroughActor, 0, Key >*,Key const& value) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadThroughActor, 0, Key >*,Key && value) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadThroughActor, 0, Key >*,Error err) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetKeyReq read;
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Snapshot snapshot;
															#line 1093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via readThrough()
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadThroughActor final : public Actor<Key>, public ActorCallback< ReadThroughActor, 0, Key >, public FastAllocated<ReadThroughActor>, public ReadThroughActorState<ReadThroughActor> {
															#line 1098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<ReadThroughActor>::operator new;
	using FastAllocated<ReadThroughActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Key>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadThroughActor, 0, Key >;
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadThroughActor(ReadYourWritesTransaction* const& ryw,GetKeyReq const& read,Snapshot const& snapshot) 
															#line 1109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<Key>(),
		   ReadThroughActorState<ReadThroughActor>(ryw, read, snapshot)
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readThrough");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadThroughActor, 0, Key >*)0, actor_cancelled()); break;
		}

	}
};
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<Key> readThrough( ReadYourWritesTransaction* const& ryw, GetKeyReq const& read, Snapshot const& snapshot ) {
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<Key>(new ReadThroughActor(ryw, read, snapshot));
															#line 1136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"

																#line 1141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via readThrough()
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <bool backwards, class ReadThroughActor1>
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadThroughActor1State {
															#line 1147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadThroughActor1State(ReadYourWritesTransaction* const& ryw,GetRangeReq<backwards> const& read,Snapshot const& snapshot) 
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw(ryw),
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   read(read),
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   snapshot(snapshot)
															#line 1158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("readThrough", reinterpret_cast<unsigned long>(this));

	}
	~ReadThroughActor1State() 
	{
		fdb_probe_actor_destroy("readThrough", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (backwards && read.end.offset > 1)
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				StrictFuture<Key> __when_expr_0 = ryw->tr.getKey(read.end, snapshot);
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (static_cast<ReadThroughActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ReadThroughActor1*>(this)->actor_wait_state = 1;
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadThroughActor1, 0, Key >*>(static_cast<ReadThroughActor1*>(this)));
															#line 1184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~ReadThroughActor1State();
		static_cast<ReadThroughActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = ryw->tr.getRange(read.begin, read.end, read.limits, snapshot, backwards ? Reverse::True : Reverse::False);
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (static_cast<ReadThroughActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadThroughActor1*>(this)->actor_wait_state = 2;
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadThroughActor1, 1, RangeResult >*>(static_cast<ReadThroughActor1*>(this)));
															#line 1219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Key const& key,int loopDepth) 
	{
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (key > ryw->getMaxReadKey())
															#line 1228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			read.end = firstGreaterOrEqual(ryw->getMaxReadKey());
															#line 1232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
		else
		{
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			read.end = KeySelector(firstGreaterOrEqual(key), key.arena());
															#line 1238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Key && key,int loopDepth) 
	{
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (key > ryw->getMaxReadKey())
															#line 1248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			read.end = firstGreaterOrEqual(ryw->getMaxReadKey());
															#line 1252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
		else
		{
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			read.end = KeySelector(firstGreaterOrEqual(key), key.arena());
															#line 1258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Key const& key,int loopDepth) 
	{
		loopDepth = a_body1cont2(key, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Key && key,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(key), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadThroughActor1*>(this)->actor_wait_state > 0) static_cast<ReadThroughActor1*>(this)->actor_wait_state = 0;
		static_cast<ReadThroughActor1*>(this)->ActorCallback< ReadThroughActor1, 0, Key >::remove();

	}
	void a_callback_fire(ActorCallback< ReadThroughActor1, 0, Key >*,Key const& value) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadThroughActor1, 0, Key >*,Key && value) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadThroughActor1, 0, Key >*,Error err) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont6(RangeResult const& v,int loopDepth) 
	{
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		KeyRef maxKey = ryw->getMaxReadKey();
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (v.size() > 0)
															#line 1333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!backwards && v[v.size() - 1].key >= maxKey)
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				_v = v;
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				int i = _v.size() - 2;
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				for(;i >= 0 && _v[i].key >= maxKey;--i) {
															#line 1345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (!static_cast<ReadThroughActor1*>(this)->SAV<RangeResult>::futures) { (void)(RangeResult(RangeResultRef(VectorRef<KeyValueRef>(&_v[0], i + 1), false), _v.arena())); this->~ReadThroughActor1State(); static_cast<ReadThroughActor1*>(this)->destroy(); return 0; }
															#line 1349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				new (&static_cast<ReadThroughActor1*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResult(RangeResultRef(VectorRef<KeyValueRef>(&_v[0], i + 1), false), _v.arena()));
				this->~ReadThroughActor1State();
				static_cast<ReadThroughActor1*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadThroughActor1*>(this)->SAV<RangeResult>::futures) { (void)(v); this->~ReadThroughActor1State(); static_cast<ReadThroughActor1*>(this)->destroy(); return 0; }
															#line 1358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadThroughActor1*>(this)->SAV< RangeResult >::value()) RangeResult(v);
		this->~ReadThroughActor1State();
		static_cast<ReadThroughActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(RangeResult && v,int loopDepth) 
	{
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		KeyRef maxKey = ryw->getMaxReadKey();
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (v.size() > 0)
															#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!backwards && v[v.size() - 1].key >= maxKey)
															#line 1376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				_v = v;
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				int i = _v.size() - 2;
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				for(;i >= 0 && _v[i].key >= maxKey;--i) {
															#line 1384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (!static_cast<ReadThroughActor1*>(this)->SAV<RangeResult>::futures) { (void)(RangeResult(RangeResultRef(VectorRef<KeyValueRef>(&_v[0], i + 1), false), _v.arena())); this->~ReadThroughActor1State(); static_cast<ReadThroughActor1*>(this)->destroy(); return 0; }
															#line 1388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				new (&static_cast<ReadThroughActor1*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResult(RangeResultRef(VectorRef<KeyValueRef>(&_v[0], i + 1), false), _v.arena()));
				this->~ReadThroughActor1State();
				static_cast<ReadThroughActor1*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadThroughActor1*>(this)->SAV<RangeResult>::futures) { (void)(v); this->~ReadThroughActor1State(); static_cast<ReadThroughActor1*>(this)->destroy(); return 0; }
															#line 1397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadThroughActor1*>(this)->SAV< RangeResult >::value()) RangeResult(v);
		this->~ReadThroughActor1State();
		static_cast<ReadThroughActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult const& v,int loopDepth) 
	{
		loopDepth = a_body1cont6(v, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult && v,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(v), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReadThroughActor1*>(this)->actor_wait_state > 0) static_cast<ReadThroughActor1*>(this)->actor_wait_state = 0;
		static_cast<ReadThroughActor1*>(this)->ActorCallback< ReadThroughActor1, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ReadThroughActor1, 1, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadThroughActor1, 1, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadThroughActor1, 1, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetRangeReq<backwards> read;
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Snapshot snapshot;
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	RangeResult _v;
															#line 1476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via readThrough()
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <bool backwards>
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadThroughActor1 final : public Actor<RangeResult>, public ActorCallback< ReadThroughActor1<backwards>, 0, Key >, public ActorCallback< ReadThroughActor1<backwards>, 1, RangeResult >, public FastAllocated<ReadThroughActor1<backwards>>, public ReadThroughActor1State<backwards, ReadThroughActor1<backwards>> {
															#line 1483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<ReadThroughActor1<backwards>>::operator new;
	using FastAllocated<ReadThroughActor1<backwards>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadThroughActor1<backwards>, 0, Key >;
friend struct ActorCallback< ReadThroughActor1<backwards>, 1, RangeResult >;
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadThroughActor1(ReadYourWritesTransaction* const& ryw,GetRangeReq<backwards> const& read,Snapshot const& snapshot) 
															#line 1495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<RangeResult>(),
		   ReadThroughActor1State<backwards, ReadThroughActor1<backwards>>(ryw, read, snapshot)
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readThrough");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadThroughActor1<backwards>, 0, Key >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReadThroughActor1<backwards>, 1, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <bool backwards>
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<RangeResult> readThrough( ReadYourWritesTransaction* const& ryw, GetRangeReq<backwards> const& read, Snapshot const& snapshot ) {
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<RangeResult>(new ReadThroughActor1<backwards>(ryw, read, snapshot));
															#line 1525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"

	// addConflictRange(ryw,read,result) is called after a serializable read and is responsible for adding the relevant
	// conflict range

	template <bool mustUnmodified = false>
	static void addConflictRange(ReadYourWritesTransaction* ryw,
	                             GetValueReq read,
	                             WriteMap::iterator& it,
	                             Optional<Value> result) {
		// it will already point to the right segment (see the calling code in read()), so we don't need to skip
		// read.key will be copied into ryw->arena inside of updateConflictMap if it is being added
		updateConflictMap<mustUnmodified>(ryw, read.key, it);
	}

	static void addConflictRange(ReadYourWritesTransaction* ryw, GetKeyReq read, WriteMap::iterator& it, Key result) {
		KeyRangeRef readRange;
		if (read.key.offset <= 0)
			readRange = KeyRangeRef(KeyRef(ryw->arena, result),
			                        read.key.orEqual ? keyAfter(read.key.getKey(), ryw->arena)
			                                         : KeyRef(ryw->arena, read.key.getKey()));
		else
			readRange = KeyRangeRef(read.key.orEqual ? keyAfter(read.key.getKey(), ryw->arena)
			                                         : KeyRef(ryw->arena, read.key.getKey()),
			                        keyAfter(result, ryw->arena));

		it.skip(readRange.begin);
		ryw->updateConflictMap(readRange, it);
	}

	template <bool mustUnmodified = false, class RangeResultFamily = RangeResult>
	static void addConflictRange(ReadYourWritesTransaction* ryw,
	                             GetRangeReq<false> read,
	                             WriteMap::iterator& it,
	                             RangeResultFamily& result) {
		KeyRef rangeBegin, rangeEnd;
		bool endInArena = false;

		if (read.begin.getKey() < read.end.getKey()) {
			rangeBegin = read.begin.getKey();
			// If the end offset is 1 (first greater than / first greater or equal) or more, then no changes to the
			// range after the returned results can change the outcome.
			rangeEnd = read.end.offset > 0 && result.more ? read.begin.getKey() : read.end.getKey();
		} else {
			rangeBegin = read.end.getKey();
			rangeEnd = read.begin.getKey();
		}

		if (result.readToBegin && read.begin.offset <= 0)
			rangeBegin = allKeys.begin;
		if (result.readThroughEnd && read.end.offset > 0)
			rangeEnd = ryw->getMaxReadKey();

		if (result.size()) {
			if (read.begin.offset <= 0)
				rangeBegin = std::min(rangeBegin, result[0].key);
			if (rangeEnd <= result.end()[-1].key) {
				rangeEnd = keyAfter(result.end()[-1].key, ryw->arena);
				endInArena = true;
			}
		}

		KeyRangeRef readRange =
		    KeyRangeRef(KeyRef(ryw->arena, rangeBegin), endInArena ? rangeEnd : KeyRef(ryw->arena, rangeEnd));
		it.skip(readRange.begin);
		updateConflictMap<mustUnmodified>(ryw, readRange, it);
	}

	// In the case where RangeResultFamily is MappedRangeResult, it only adds the primary range to conflict.
	template <bool mustUnmodified = false, class RangeResultFamily = RangeResult>
	static void addConflictRange(ReadYourWritesTransaction* ryw,
	                             GetRangeReq<true> read,
	                             WriteMap::iterator& it,
	                             RangeResultFamily& result) {
		KeyRef rangeBegin, rangeEnd;
		bool endInArena = false;

		if (read.begin.getKey() < read.end.getKey()) {
			// If the begin offset is 1 (first greater than / first greater or equal) or less, then no changes to the
			// range prior to the returned results can change the outcome.
			rangeBegin = read.begin.offset <= 1 && result.more ? read.end.getKey() : read.begin.getKey();
			rangeEnd = read.end.getKey();
		} else {
			rangeBegin = read.end.getKey();
			rangeEnd = read.begin.getKey();
		}

		if (result.readToBegin && read.begin.offset <= 0)
			rangeBegin = allKeys.begin;
		if (result.readThroughEnd && read.end.offset > 0)
			rangeEnd = ryw->getMaxReadKey();

		if (result.size()) {
			rangeBegin = std::min(rangeBegin, result.end()[-1].key);
			if (read.end.offset > 0 && rangeEnd <= result[0].key) {
				rangeEnd = keyAfter(result[0].key, ryw->arena);
				endInArena = true;
			}
		}

		KeyRangeRef readRange =
		    KeyRangeRef(KeyRef(ryw->arena, rangeBegin), endInArena ? rangeEnd : KeyRef(ryw->arena, rangeEnd));
		it.skip(readRange.begin);
		updateConflictMap<mustUnmodified>(ryw, readRange, it);
	}

	template <bool mustUnmodified = false>
	static void updateConflictMap(ReadYourWritesTransaction* ryw, KeyRef const& key, WriteMap::iterator& it) {
		// it.skip( key );
		// ASSERT( it.beginKey() <= key && key < it.endKey() );
		if (mustUnmodified && !it.is_unmodified_range()) {
			throw get_mapped_range_reads_your_writes();
		}
		if (it.is_unmodified_range() || (it.is_operation() && !it.is_independent())) {
			ryw->approximateSize += 2 * key.expectedSize() + 1 + sizeof(KeyRangeRef);
			ryw->readConflicts.insert(singleKeyRange(key, ryw->arena), true);
		}
	}

	template <bool mustUnmodified = false>
	static void updateConflictMap(ReadYourWritesTransaction* ryw, KeyRangeRef const& keys, WriteMap::iterator& it) {
		// it.skip( keys.begin );
		// ASSERT( it.beginKey() <= keys.begin && keys.begin < it.endKey() );
		for (; it.beginKey() < keys.end; ++it) {
			if (mustUnmodified && !it.is_unmodified_range()) {
				throw get_mapped_range_reads_your_writes();
			}
			if (it.is_unmodified_range() || (it.is_operation() && !it.is_independent())) {
				KeyRangeRef insert_range = KeyRangeRef(std::max(keys.begin, it.beginKey().toArenaOrRef(ryw->arena)),
				                                       std::min(keys.end, it.endKey().toArenaOrRef(ryw->arena)));
				if (!insert_range.empty()) {
					ryw->approximateSize += keys.expectedSize() + sizeof(KeyRangeRef);
					ryw->readConflicts.insert(insert_range, true);
				}
			}
		}
	}

																#line 1666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via readWithConflictRangeThrough()
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Req, class ReadWithConflictRangeThroughActor>
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadWithConflictRangeThroughActorState {
															#line 1672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadWithConflictRangeThroughActorState(ReadYourWritesTransaction* const& ryw,Req const& req,Snapshot const& snapshot) 
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw(ryw),
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   req(req),
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   snapshot(snapshot)
															#line 1683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this));

	}
	~ReadWithConflictRangeThroughActorState() 
	{
		fdb_probe_actor_destroy("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<typename Req::Result> __when_expr_0 = readThrough(ryw, req, snapshot);
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (static_cast<ReadWithConflictRangeThroughActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ryw->resetPromise.getFuture();
															#line 1704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<ReadWithConflictRangeThroughActor*>(this)->actor_wait_state = 1;
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadWithConflictRangeThroughActor, 0, typename Req::Result >*>(static_cast<ReadWithConflictRangeThroughActor*>(this)));
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadWithConflictRangeThroughActor, 1, Void >*>(static_cast<ReadWithConflictRangeThroughActor*>(this)));
															#line 1711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~ReadWithConflictRangeThroughActorState();
		static_cast<ReadWithConflictRangeThroughActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(typename Req::Result const& result,int loopDepth) 
	{
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadWithConflictRangeThroughActor*>(this)->SAV<typename Req::Result>::futures) { (void)(result); this->~ReadWithConflictRangeThroughActorState(); static_cast<ReadWithConflictRangeThroughActor*>(this)->destroy(); return 0; }
															#line 1734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadWithConflictRangeThroughActor*>(this)->SAV< typename Req::Result >::value()) typename Req::Result(result);
		this->~ReadWithConflictRangeThroughActorState();
		static_cast<ReadWithConflictRangeThroughActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(typename Req::Result && result,int loopDepth) 
	{
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadWithConflictRangeThroughActor*>(this)->SAV<typename Req::Result>::futures) { (void)(result); this->~ReadWithConflictRangeThroughActorState(); static_cast<ReadWithConflictRangeThroughActor*>(this)->destroy(); return 0; }
															#line 1746 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadWithConflictRangeThroughActor*>(this)->SAV< typename Req::Result >::value()) typename Req::Result(result);
		this->~ReadWithConflictRangeThroughActorState();
		static_cast<ReadWithConflictRangeThroughActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 1758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 1766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadWithConflictRangeThroughActor*>(this)->actor_wait_state > 0) static_cast<ReadWithConflictRangeThroughActor*>(this)->actor_wait_state = 0;
		static_cast<ReadWithConflictRangeThroughActor*>(this)->ActorCallback< ReadWithConflictRangeThroughActor, 0, typename Req::Result >::remove();
		static_cast<ReadWithConflictRangeThroughActor*>(this)->ActorCallback< ReadWithConflictRangeThroughActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeThroughActor, 0, typename Req::Result >*,typename Req::Result const& value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeThroughActor, 0, typename Req::Result >*,typename Req::Result && value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadWithConflictRangeThroughActor, 0, typename Req::Result >*,Error err) 
	{
		fdb_probe_actor_enter("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeThroughActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeThroughActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadWithConflictRangeThroughActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Req req;
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Snapshot snapshot;
															#line 1873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via readWithConflictRangeThrough()
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Req>
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadWithConflictRangeThroughActor final : public Actor<typename Req::Result>, public ActorCallback< ReadWithConflictRangeThroughActor<Req>, 0, typename Req::Result >, public ActorCallback< ReadWithConflictRangeThroughActor<Req>, 1, Void >, public FastAllocated<ReadWithConflictRangeThroughActor<Req>>, public ReadWithConflictRangeThroughActorState<Req, ReadWithConflictRangeThroughActor<Req>> {
															#line 1880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<ReadWithConflictRangeThroughActor<Req>>::operator new;
	using FastAllocated<ReadWithConflictRangeThroughActor<Req>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<typename Req::Result>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadWithConflictRangeThroughActor<Req>, 0, typename Req::Result >;
friend struct ActorCallback< ReadWithConflictRangeThroughActor<Req>, 1, Void >;
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadWithConflictRangeThroughActor(ReadYourWritesTransaction* const& ryw,Req const& req,Snapshot const& snapshot) 
															#line 1892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<typename Req::Result>(),
		   ReadWithConflictRangeThroughActorState<Req, ReadWithConflictRangeThroughActor<Req>>(ryw, req, snapshot)
	{
		fdb_probe_actor_enter("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readWithConflictRangeThrough");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readWithConflictRangeThrough", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadWithConflictRangeThroughActor<Req>, 0, typename Req::Result >*)0, actor_cancelled()); break;
		}

	}
};
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Req>
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<typename Req::Result> readWithConflictRangeThrough( ReadYourWritesTransaction* const& ryw, Req const& req, Snapshot const& snapshot ) {
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<typename Req::Result>(new ReadWithConflictRangeThroughActor<Req>(ryw, req, snapshot));
															#line 1921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
																#line 1925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via readWithConflictRangeSnapshot()
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Req, class ReadWithConflictRangeSnapshotActor>
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadWithConflictRangeSnapshotActorState {
															#line 1931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadWithConflictRangeSnapshotActorState(ReadYourWritesTransaction* const& ryw,Req const& req) 
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw(ryw),
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   req(req),
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   it(&ryw->cache, &ryw->writes)
															#line 1942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this));

	}
	~ReadWithConflictRangeSnapshotActorState() 
	{
		fdb_probe_actor_destroy("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<typename Req::Result> __when_expr_0 = read(ryw, req, &it);
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (static_cast<ReadWithConflictRangeSnapshotActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ryw->resetPromise.getFuture();
															#line 1963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<ReadWithConflictRangeSnapshotActor*>(this)->actor_wait_state = 1;
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadWithConflictRangeSnapshotActor, 0, typename Req::Result >*>(static_cast<ReadWithConflictRangeSnapshotActor*>(this)));
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadWithConflictRangeSnapshotActor, 1, Void >*>(static_cast<ReadWithConflictRangeSnapshotActor*>(this)));
															#line 1970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~ReadWithConflictRangeSnapshotActorState();
		static_cast<ReadWithConflictRangeSnapshotActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(typename Req::Result const& result,int loopDepth) 
	{
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadWithConflictRangeSnapshotActor*>(this)->SAV<typename Req::Result>::futures) { (void)(result); this->~ReadWithConflictRangeSnapshotActorState(); static_cast<ReadWithConflictRangeSnapshotActor*>(this)->destroy(); return 0; }
															#line 1993 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadWithConflictRangeSnapshotActor*>(this)->SAV< typename Req::Result >::value()) typename Req::Result(result);
		this->~ReadWithConflictRangeSnapshotActorState();
		static_cast<ReadWithConflictRangeSnapshotActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(typename Req::Result && result,int loopDepth) 
	{
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadWithConflictRangeSnapshotActor*>(this)->SAV<typename Req::Result>::futures) { (void)(result); this->~ReadWithConflictRangeSnapshotActorState(); static_cast<ReadWithConflictRangeSnapshotActor*>(this)->destroy(); return 0; }
															#line 2005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadWithConflictRangeSnapshotActor*>(this)->SAV< typename Req::Result >::value()) typename Req::Result(result);
		this->~ReadWithConflictRangeSnapshotActorState();
		static_cast<ReadWithConflictRangeSnapshotActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 2017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 2025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadWithConflictRangeSnapshotActor*>(this)->actor_wait_state > 0) static_cast<ReadWithConflictRangeSnapshotActor*>(this)->actor_wait_state = 0;
		static_cast<ReadWithConflictRangeSnapshotActor*>(this)->ActorCallback< ReadWithConflictRangeSnapshotActor, 0, typename Req::Result >::remove();
		static_cast<ReadWithConflictRangeSnapshotActor*>(this)->ActorCallback< ReadWithConflictRangeSnapshotActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeSnapshotActor, 0, typename Req::Result >*,typename Req::Result const& value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeSnapshotActor, 0, typename Req::Result >*,typename Req::Result && value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadWithConflictRangeSnapshotActor, 0, typename Req::Result >*,Error err) 
	{
		fdb_probe_actor_enter("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeSnapshotActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeSnapshotActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadWithConflictRangeSnapshotActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Req req;
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	SnapshotCache::iterator it;
															#line 2132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via readWithConflictRangeSnapshot()
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Req>
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadWithConflictRangeSnapshotActor final : public Actor<typename Req::Result>, public ActorCallback< ReadWithConflictRangeSnapshotActor<Req>, 0, typename Req::Result >, public ActorCallback< ReadWithConflictRangeSnapshotActor<Req>, 1, Void >, public FastAllocated<ReadWithConflictRangeSnapshotActor<Req>>, public ReadWithConflictRangeSnapshotActorState<Req, ReadWithConflictRangeSnapshotActor<Req>> {
															#line 2139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<ReadWithConflictRangeSnapshotActor<Req>>::operator new;
	using FastAllocated<ReadWithConflictRangeSnapshotActor<Req>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<typename Req::Result>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadWithConflictRangeSnapshotActor<Req>, 0, typename Req::Result >;
friend struct ActorCallback< ReadWithConflictRangeSnapshotActor<Req>, 1, Void >;
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadWithConflictRangeSnapshotActor(ReadYourWritesTransaction* const& ryw,Req const& req) 
															#line 2151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<typename Req::Result>(),
		   ReadWithConflictRangeSnapshotActorState<Req, ReadWithConflictRangeSnapshotActor<Req>>(ryw, req)
	{
		fdb_probe_actor_enter("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readWithConflictRangeSnapshot");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readWithConflictRangeSnapshot", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadWithConflictRangeSnapshotActor<Req>, 0, typename Req::Result >*)0, actor_cancelled()); break;
		}

	}
};
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Req>
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<typename Req::Result> readWithConflictRangeSnapshot( ReadYourWritesTransaction* const& ryw, Req const& req ) {
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<typename Req::Result>(new ReadWithConflictRangeSnapshotActor<Req>(ryw, req));
															#line 2180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
																#line 2184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via readWithConflictRangeRYW()
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Req, class ReadWithConflictRangeRYWActor>
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadWithConflictRangeRYWActorState {
															#line 2190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadWithConflictRangeRYWActorState(ReadYourWritesTransaction* const& ryw,Req const& req,Snapshot const& snapshot) 
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw(ryw),
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   req(req),
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   snapshot(snapshot),
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   it(&ryw->cache, &ryw->writes)
															#line 2203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this));

	}
	~ReadWithConflictRangeRYWActorState() 
	{
		fdb_probe_actor_destroy("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<typename Req::Result> __when_expr_0 = read(ryw, req, &it);
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (static_cast<ReadWithConflictRangeRYWActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ryw->resetPromise.getFuture();
															#line 2224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<ReadWithConflictRangeRYWActor*>(this)->actor_wait_state = 1;
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadWithConflictRangeRYWActor, 0, typename Req::Result >*>(static_cast<ReadWithConflictRangeRYWActor*>(this)));
															#line 391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadWithConflictRangeRYWActor, 1, Void >*>(static_cast<ReadWithConflictRangeRYWActor*>(this)));
															#line 2231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~ReadWithConflictRangeRYWActorState();
		static_cast<ReadWithConflictRangeRYWActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(typename Req::Result const& result,int loopDepth) 
	{
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!snapshot)
															#line 2254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			addConflictRange(ryw, req, it.extractWriteMapIterator(), result);
															#line 2258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadWithConflictRangeRYWActor*>(this)->SAV<typename Req::Result>::futures) { (void)(result); this->~ReadWithConflictRangeRYWActorState(); static_cast<ReadWithConflictRangeRYWActor*>(this)->destroy(); return 0; }
															#line 2262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadWithConflictRangeRYWActor*>(this)->SAV< typename Req::Result >::value()) typename Req::Result(result);
		this->~ReadWithConflictRangeRYWActorState();
		static_cast<ReadWithConflictRangeRYWActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(typename Req::Result && result,int loopDepth) 
	{
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!snapshot)
															#line 2274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			addConflictRange(ryw, req, it.extractWriteMapIterator(), result);
															#line 2278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadWithConflictRangeRYWActor*>(this)->SAV<typename Req::Result>::futures) { (void)(result); this->~ReadWithConflictRangeRYWActorState(); static_cast<ReadWithConflictRangeRYWActor*>(this)->destroy(); return 0; }
															#line 2282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadWithConflictRangeRYWActor*>(this)->SAV< typename Req::Result >::value()) typename Req::Result(result);
		this->~ReadWithConflictRangeRYWActorState();
		static_cast<ReadWithConflictRangeRYWActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 2294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 2302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadWithConflictRangeRYWActor*>(this)->actor_wait_state > 0) static_cast<ReadWithConflictRangeRYWActor*>(this)->actor_wait_state = 0;
		static_cast<ReadWithConflictRangeRYWActor*>(this)->ActorCallback< ReadWithConflictRangeRYWActor, 0, typename Req::Result >::remove();
		static_cast<ReadWithConflictRangeRYWActor*>(this)->ActorCallback< ReadWithConflictRangeRYWActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeRYWActor, 0, typename Req::Result >*,typename Req::Result const& value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeRYWActor, 0, typename Req::Result >*,typename Req::Result && value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadWithConflictRangeRYWActor, 0, typename Req::Result >*,Error err) 
	{
		fdb_probe_actor_enter("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeRYWActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeRYWActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadWithConflictRangeRYWActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Req req;
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Snapshot snapshot;
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	RYWIterator it;
															#line 2411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via readWithConflictRangeRYW()
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Req>
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadWithConflictRangeRYWActor final : public Actor<typename Req::Result>, public ActorCallback< ReadWithConflictRangeRYWActor<Req>, 0, typename Req::Result >, public ActorCallback< ReadWithConflictRangeRYWActor<Req>, 1, Void >, public FastAllocated<ReadWithConflictRangeRYWActor<Req>>, public ReadWithConflictRangeRYWActorState<Req, ReadWithConflictRangeRYWActor<Req>> {
															#line 2418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<ReadWithConflictRangeRYWActor<Req>>::operator new;
	using FastAllocated<ReadWithConflictRangeRYWActor<Req>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<typename Req::Result>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadWithConflictRangeRYWActor<Req>, 0, typename Req::Result >;
friend struct ActorCallback< ReadWithConflictRangeRYWActor<Req>, 1, Void >;
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadWithConflictRangeRYWActor(ReadYourWritesTransaction* const& ryw,Req const& req,Snapshot const& snapshot) 
															#line 2430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<typename Req::Result>(),
		   ReadWithConflictRangeRYWActorState<Req, ReadWithConflictRangeRYWActor<Req>>(ryw, req, snapshot)
	{
		fdb_probe_actor_enter("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readWithConflictRangeRYW");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadWithConflictRangeRYWActor<Req>, 0, typename Req::Result >*)0, actor_cancelled()); break;
		}

	}
};
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Req>
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<typename Req::Result> readWithConflictRangeRYW( ReadYourWritesTransaction* const& ryw, Req const& req, Snapshot const& snapshot ) {
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<typename Req::Result>(new ReadWithConflictRangeRYWActor<Req>(ryw, req, snapshot));
															#line 2459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	template <class Req>
	static inline Future<typename Req::Result> readWithConflictRange(ReadYourWritesTransaction* ryw,
	                                                                 Req const& req,
	                                                                 Snapshot snapshot) {
		if (ryw->options.readYourWritesDisabled) {
			return readWithConflictRangeThrough(ryw, req, snapshot);
		} else if (snapshot && ryw->options.snapshotRywEnabled <= 0) {
			return readWithConflictRangeSnapshot(ryw, req);
		}
		return readWithConflictRangeRYW(ryw, req, snapshot);
	}

	template <class Iter>
	static void resolveKeySelectorFromCache(KeySelector& key,
	                                        Iter& it,
	                                        KeyRef const& maxKey,
	                                        bool* readToBegin,
	                                        bool* readThroughEnd,
	                                        int* actualOffset) {
		// If the key indicated by `key` can be determined without reading unknown data from the snapshot, then
		// it.kv().key is the resolved key. If the indicated key is determined to be "off the beginning or end" of the
		// database, it points to the first or last segment in the DB,
		//   and key is an equivalent key selector relative to the beginning or end of the database.
		// Otherwise it points to an unknown segment, and key is an equivalent key selector whose base key is in or
		// adjoining the segment.

		key.removeOrEqual(key.arena());

		bool alreadyExhausted = key.offset == 1;

		it.skip(key.getKey()); // TODO: or precondition?

		if (key.offset <= 0 && it.beginKey() == key.getKey() && key.getKey() != allKeys.begin)
			--it;

		ExtStringRef keykey = key.getKey();
		bool keyNeedsCopy = false;

		// Invariant: it.beginKey() <= keykey && keykey <= it.endKey() && (key.isBackward() ? it.beginKey() != keykey :
		// it.endKey() != keykey) Maintaining this invariant, we transform the key selector toward firstGreaterOrEqual
		// form until we reach an unknown range or the result
		while (key.offset > 1 && !it.is_unreadable() && !it.is_unknown_range() && it.endKey() < maxKey) {
			if (it.is_kv())
				--key.offset;
			++it;
			keykey = it.beginKey();
			keyNeedsCopy = true;
		}
		while (key.offset < 1 && !it.is_unreadable() && !it.is_unknown_range() && it.beginKey() != allKeys.begin) {
			if (it.is_kv()) {
				++key.offset;
				if (key.offset == 1) {
					keykey = it.beginKey();
					keyNeedsCopy = true;
					break;
				}
			}
			--it;
			keykey = it.endKey();
			keyNeedsCopy = true;
		}

		if (!alreadyExhausted) {
			*actualOffset = key.offset;
		}

		if (!it.is_unreadable() && !it.is_unknown_range() && key.offset < 1) {
			*readToBegin = true;
			key.setKey(allKeys.begin);
			key.offset = 1;
			return;
		}

		if (!it.is_unreadable() && !it.is_unknown_range() && key.offset > 1) {
			*readThroughEnd = true;
			key.setKey(maxKey); // maxKey is a KeyRef, but points to a literal. TODO: how can we ASSERT this?
			key.offset = 1;
			return;
		}

		while (!it.is_unreadable() && it.is_empty_range() && it.endKey() < maxKey) {
			++it;
			keykey = it.beginKey();
			keyNeedsCopy = true;
		}

		if (keyNeedsCopy) {
			key.setKey(keykey.toArena(key.arena()));
		}
	}

	static KeyRangeRef getKnownKeyRange(RangeResultRef data, KeySelector begin, KeySelector end, Arena& arena) {
		StringRef beginKey = begin.offset <= 1 ? begin.getKey() : allKeys.end;
		ExtStringRef endKey = !data.more && end.offset >= 1 ? end.getKey() : allKeys.begin;

		if (data.readToBegin)
			beginKey = allKeys.begin;
		if (data.readThroughEnd)
			endKey = allKeys.end;

		if (data.size()) {
			beginKey = std::min(beginKey, data[0].key);
			if (data.readThrough.present()) {
				endKey = std::max<ExtStringRef>(endKey, data.readThrough.get());
			} else {
				endKey = !data.more && data.end()[-1].key < endKey ? endKey : ExtStringRef(data.end()[-1].key, 1);
			}
		}
		if (beginKey >= endKey)
			return KeyRangeRef();

		return KeyRangeRef(StringRef(arena, beginKey), endKey.toArena(arena));
	}

	// Pre: it points to an unknown range
	// Increments it to point to the unknown range just before the next nontrivial known range (skips over trivial known
	// ranges), but not more than iterationLimit ranges away
	template <class Iter>
	static int skipUncached(Iter& it, Iter const& end, int iterationLimit) {
		ExtStringRef b = it.beginKey();
		ExtStringRef e = it.endKey();
		int singleEmpty = 0;

		ASSERT(!it.is_unreadable() && it.is_unknown_range());

		// b is the beginning of the most recent contiguous *empty* range
		// e is it.endKey()
		while (it != end && --iterationLimit >= 0) {
			if (it.is_unreadable() || it.is_empty_range()) {
				if (it.is_unreadable() || !e.isKeyAfter(b)) { // Assumes no degenerate ranges
					while (it.is_unreadable() || !it.is_unknown_range())
						--it;
					return singleEmpty;
				}
				singleEmpty++;
			} else
				b = e;
			++it;
			e = it.endKey();
		}
		while (it.is_unreadable() || !it.is_unknown_range())
			--it;
		return singleEmpty;
	}

	// Pre: it points to an unknown range
	// Returns the number of following empty single-key known ranges between it and the next nontrivial known range, but
	// no more than maxClears Leaves `it` in an indeterminate state
	template <class Iter>
	static int countUncached(Iter&& it, KeyRef maxKey, int maxClears) {
		if (maxClears <= 0)
			return 0;

		ExtStringRef b = it.beginKey();
		ExtStringRef e = it.endKey();
		int singleEmpty = 0;

		while (e < maxKey) {
			if (it.is_unreadable() || it.is_empty_range()) {
				if (it.is_unreadable() || !e.isKeyAfter(b)) { // Assumes no degenerate ranges
					return singleEmpty;
				}
				singleEmpty++;
				if (singleEmpty >= maxClears)
					return maxClears;
			} else
				b = e;
			++it;
			e = it.endKey();
		}
		return singleEmpty;
	}

	static void setRequestLimits(GetRangeLimits& requestLimit, int64_t additionalRows, int offset, int requestCount) {
		requestLimit.minRows =
		    (int)std::min(std::max(1 + additionalRows, (int64_t)offset), (int64_t)std::numeric_limits<int>::max());
		if (requestLimit.hasRowLimit()) {
			requestLimit.rows =
			    (int)std::min(std::max(std::max(1, requestLimit.rows) + additionalRows, (int64_t)offset),
			                  (int64_t)std::numeric_limits<int>::max());
		}

		// Calculating request byte limit
		if (requestLimit.bytes == 0) {
			requestLimit.bytes = GetRangeLimits::BYTE_LIMIT_UNLIMITED;
			if (!requestLimit.hasRowLimit()) {
				requestLimit.rows =
				    (int)std::min(std::max(std::max(1, requestLimit.rows) + additionalRows, (int64_t)offset),
				                  (int64_t)std::numeric_limits<int>::max());
			}
		} else if (requestLimit.hasByteLimit()) {
			requestLimit.bytes = std::min(int64_t(requestLimit.bytes) << std::min(requestCount, 20),
			                              (int64_t)CLIENT_KNOBS->REPLY_BYTE_LIMIT);
		}
	}

	// TODO: read to begin, read through end flags for result
																#line 2660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via getRangeValue()
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Iter, class GetRangeValueActor>
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class GetRangeValueActorState {
															#line 2666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetRangeValueActorState(ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Iter* const& pit) 
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw(ryw),
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   begin(begin),
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   end(end),
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   limits(limits),
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   pit(pit),
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   it(*pit),
															#line 600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   itEnd(*pit),
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   result(),
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   additionalRows(0),
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   itemsPastEnd(0),
															#line 604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   requestCount(0),
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   readToBegin(false),
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   readThroughEnd(false),
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   actualBeginOffset(begin.offset),
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   actualEndOffset(end.offset)
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("getRangeValue", reinterpret_cast<unsigned long>(this));

	}
	~GetRangeValueActorState() 
	{
		fdb_probe_actor_destroy("getRangeValue", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			resolveKeySelectorFromCache(begin, it, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualBeginOffset);
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			resolveKeySelectorFromCache(end, itEnd, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualEndOffset);
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (actualBeginOffset >= actualEndOffset && begin.getKey() >= end.getKey())
															#line 2720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (!static_cast<GetRangeValueActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(false, false)); this->~GetRangeValueActorState(); static_cast<GetRangeValueActor*>(this)->destroy(); return 0; }
															#line 2724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				new (&static_cast<GetRangeValueActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(false, false));
				this->~GetRangeValueActorState();
				static_cast<GetRangeValueActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if ((begin.isFirstGreaterOrEqual() && begin.getKey() == ryw->getMaxReadKey()) || (end.isFirstGreaterOrEqual() && end.getKey() == allKeys.begin))
															#line 2734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					if (!static_cast<GetRangeValueActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(readToBegin, readThroughEnd)); this->~GetRangeValueActorState(); static_cast<GetRangeValueActor*>(this)->destroy(); return 0; }
															#line 2738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					new (&static_cast<GetRangeValueActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(readToBegin, readThroughEnd));
					this->~GetRangeValueActorState();
					static_cast<GetRangeValueActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!end.isFirstGreaterOrEqual() && begin.getKey() > end.getKey())
															#line 2747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				StrictFuture<Key> __when_expr_0 = read(ryw, GetKeyReq(end), pit);
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (static_cast<GetRangeValueActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetRangeValueActor*>(this)->actor_wait_state = 1;
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetRangeValueActor, 0, Key >*>(static_cast<GetRangeValueActor*>(this)));
															#line 2758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~GetRangeValueActorState();
		static_cast<GetRangeValueActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		;
															#line 2786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Key const& resolvedEnd,int loopDepth) 
	{
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedEnd == allKeys.begin)
															#line 2795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readToBegin = true;
															#line 2799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedEnd == ryw->getMaxReadKey())
															#line 2803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readThroughEnd = true;
															#line 2807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (begin.getKey() >= resolvedEnd && !begin.isBackward())
															#line 2811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<GetRangeValueActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(false, false)); this->~GetRangeValueActorState(); static_cast<GetRangeValueActor*>(this)->destroy(); return 0; }
															#line 2815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<GetRangeValueActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(false, false));
			this->~GetRangeValueActorState();
			static_cast<GetRangeValueActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (resolvedEnd == allKeys.begin)
															#line 2825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (!static_cast<GetRangeValueActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(readToBegin, readThroughEnd)); this->~GetRangeValueActorState(); static_cast<GetRangeValueActor*>(this)->destroy(); return 0; }
															#line 2829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				new (&static_cast<GetRangeValueActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(readToBegin, readThroughEnd));
				this->~GetRangeValueActorState();
				static_cast<GetRangeValueActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache( begin, it, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualBeginOffset);
															#line 636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache( end, itEnd, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualEndOffset);
															#line 2840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Key && resolvedEnd,int loopDepth) 
	{
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedEnd == allKeys.begin)
															#line 2849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readToBegin = true;
															#line 2853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedEnd == ryw->getMaxReadKey())
															#line 2857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readThroughEnd = true;
															#line 2861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (begin.getKey() >= resolvedEnd && !begin.isBackward())
															#line 2865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<GetRangeValueActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(false, false)); this->~GetRangeValueActorState(); static_cast<GetRangeValueActor*>(this)->destroy(); return 0; }
															#line 2869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<GetRangeValueActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(false, false));
			this->~GetRangeValueActorState();
			static_cast<GetRangeValueActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (resolvedEnd == allKeys.begin)
															#line 2879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (!static_cast<GetRangeValueActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(readToBegin, readThroughEnd)); this->~GetRangeValueActorState(); static_cast<GetRangeValueActor*>(this)->destroy(); return 0; }
															#line 2883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				new (&static_cast<GetRangeValueActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(readToBegin, readThroughEnd));
				this->~GetRangeValueActorState();
				static_cast<GetRangeValueActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache( begin, it, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualBeginOffset);
															#line 636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache( end, itEnd, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualEndOffset);
															#line 2894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Key const& resolvedEnd,int loopDepth) 
	{
		loopDepth = a_body1cont5(resolvedEnd, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Key && resolvedEnd,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(resolvedEnd), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetRangeValueActor*>(this)->actor_wait_state > 0) static_cast<GetRangeValueActor*>(this)->actor_wait_state = 0;
		static_cast<GetRangeValueActor*>(this)->ActorCallback< GetRangeValueActor, 0, Key >::remove();

	}
	void a_callback_fire(ActorCallback< GetRangeValueActor, 0, Key >*,Key const& value) 
	{
		fdb_probe_actor_enter("getRangeValue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValue", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetRangeValueActor, 0, Key >*,Key && value) 
	{
		fdb_probe_actor_enter("getRangeValue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValue", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetRangeValueActor, 0, Key >*,Error err) 
	{
		fdb_probe_actor_enter("getRangeValue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValue", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont12(int loopDepth) 
	{
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		result.more = result.more || limits.isReached();
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (end.isFirstGreaterOrEqual())
															#line 2968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			int keepItems = std::lower_bound(result.begin(), result.end(), end.getKey(), KeyValueRef::OrderByKey()) - result.begin();
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (keepItems < result.size())
															#line 2974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				result.more = false;
															#line 2978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			result.resize(result.arena(), keepItems);
															#line 2982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		result.readToBegin = readToBegin;
															#line 810 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		result.readThroughEnd = !result.more && readThroughEnd;
															#line 811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		result.arena().dependsOn(ryw->arena);
															#line 813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<GetRangeValueActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetRangeValueActorState(); static_cast<GetRangeValueActor*>(this)->destroy(); return 0; }
															#line 2992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<GetRangeValueActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
		this->~GetRangeValueActorState();
		static_cast<GetRangeValueActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!result.size() && actualBeginOffset >= actualEndOffset && begin.getKey() >= end.getKey())
															#line 3011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<GetRangeValueActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(false, false)); this->~GetRangeValueActorState(); static_cast<GetRangeValueActor*>(this)->destroy(); return 0; }
															#line 3015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<GetRangeValueActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(false, false));
			this->~GetRangeValueActorState();
			static_cast<GetRangeValueActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (end.offset <= 1 && end.getKey() == allKeys.begin)
															#line 3023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<GetRangeValueActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(readToBegin, readThroughEnd)); this->~GetRangeValueActorState(); static_cast<GetRangeValueActor*>(this)->destroy(); return 0; }
															#line 3027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<GetRangeValueActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(readToBegin, readThroughEnd));
			this->~GetRangeValueActorState();
			static_cast<GetRangeValueActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if ((begin.offset >= end.offset && begin.getKey() >= end.getKey()) || (begin.offset >= 1 && begin.getKey() >= ryw->getMaxReadKey()))
															#line 3035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (end.isFirstGreaterOrEqual())
															#line 3039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
				return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
			}
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!result.size())
															#line 3045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
				return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
			}
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<Key> __when_expr_1 = read(ryw, GetKeyReq(end), pit);
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (static_cast<GetRangeValueActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetRangeValueActor*>(this)->actor_wait_state = 2;
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetRangeValueActor, 1, Key >*>(static_cast<GetRangeValueActor*>(this)));
															#line 3058 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont1loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1break1(int loopDepth) 
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
	int a_body1cont1loopBody1cont1(int loopDepth) 
	{
															#line 679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!it.is_unreadable() && !it.is_unknown_range() && it.beginKey() > itEnd.beginKey())
															#line 3085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (end.isFirstGreaterOrEqual())
															#line 3089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
				return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
			}
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<GetRangeValueActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(readToBegin, readThroughEnd)); this->~GetRangeValueActorState(); static_cast<GetRangeValueActor*>(this)->destroy(); return 0; }
															#line 3095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<GetRangeValueActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(readToBegin, readThroughEnd));
			this->~GetRangeValueActorState();
			static_cast<GetRangeValueActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (limits.isReached() && itemsPastEnd >= 1 - end.offset)
															#line 3103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (it == itEnd && ((!it.is_unreadable() && !it.is_unknown_range()) || (begin.offset > 0 && end.isFirstGreaterOrEqual() && end.getKey() == it.beginKey())))
															#line 3109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (it.is_unknown_range())
															#line 3115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (limits.hasByteLimit() && limits.hasSatisfiedMinRows() && result.size() && itemsPastEnd >= 1 - end.offset)
															#line 3119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				result.more = true;
															#line 3123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
			}
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			Iter ucEnd(it);
															#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			int singleClears = 0;
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			int clearLimit = requestCount ? 1 << std::min(requestCount, 20) : 0;
															#line 702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (it.beginKey() < itEnd.beginKey())
															#line 3134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				singleClears = std::min(skipUncached(ucEnd, itEnd, BUGGIFY ? 0 : clearLimit + 100), clearLimit);
															#line 3138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			read_end = KeySelector();
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (ucEnd != itEnd)
															#line 3144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				Key k = ucEnd.endKey().toStandaloneStringRef();
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				read_end = KeySelector(firstGreaterOrEqual(k), k.arena());
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (end.offset < 1)
															#line 3152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					additionalRows += 1 - end.offset;
															#line 3156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
			}
			else
			{
															#line 711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (end.offset < 1)
															#line 3163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					read_end = KeySelector(firstGreaterOrEqual(end.getKey()), end.arena());
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					additionalRows += 1 - end.offset;
															#line 3169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
				else
				{
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					read_end = end;
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					if (end.offset > 1)
															#line 3177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					{
															#line 717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
						singleClears += countUncached(std::move(ucEnd), ryw->getMaxReadKey(), clearLimit - singleClears);
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
						read_end.offset += singleClears;
															#line 3183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					}
				}
			}
															#line 723 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			additionalRows += singleClears;
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			read_begin = KeySelector();
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (begin.isFirstGreaterOrEqual())
															#line 3193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				Key k = it.beginKey() > begin.getKey() ? it.beginKey().toStandaloneStringRef() : Key(begin.getKey(), begin.arena());
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				begin = KeySelector(firstGreaterOrEqual(k), k.arena());
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				read_begin = begin;
															#line 3201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
			else
			{
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (begin.offset > 1)
															#line 3207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					read_begin = KeySelector(firstGreaterOrEqual(begin.getKey()), begin.arena());
															#line 733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					additionalRows += begin.offset - 1;
															#line 3213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
				else
				{
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					read_begin = begin;
															#line 736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					ucEnd = it;
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					singleClears = countUncachedBack(std::move(ucEnd), clearLimit);
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					read_begin.offset -= singleClears;
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					additionalRows += singleClears;
															#line 3227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
			}
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (read_end.getKey() < read_begin.getKey())
															#line 3232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				read_end.setKey(read_begin.getKey());
															#line 745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				read_end.arena().dependsOn(read_begin.arena());
															#line 3238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
															#line 748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			requestLimit = limits;
															#line 749 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			setRequestLimits(requestLimit, additionalRows, 2 - read_begin.offset, requestCount);
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			requestCount++;
															#line 752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ASSERT(!requestLimit.hasRowLimit() || requestLimit.rows > 0);
															#line 753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ASSERT(requestLimit.hasRowLimit() || requestLimit.hasByteLimit());
															#line 757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			additionalRows = 0;
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<RangeResult> __when_expr_2 = ryw->tr.getRange(read_begin, read_end, requestLimit, Snapshot::True, Reverse::False);
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (static_cast<GetRangeValueActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
			static_cast<GetRangeValueActor*>(this)->actor_wait_state = 3;
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetRangeValueActor, 2, RangeResult >*>(static_cast<GetRangeValueActor*>(this)));
															#line 3261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (it.is_kv())
															#line 3268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				KeyValueRef const* start = it.kv(ryw->arena);
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (start == nullptr)
															#line 3274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					++it;
															#line 3278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					return a_body1cont1loopHead1(loopDepth); // continue
				}
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				it.skipContiguous(end.isFirstGreaterOrEqual() ? end.getKey() : ryw->getMaxReadKey());
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				int maxCount = it.kv(ryw->arena) - start + 1;
															#line 784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				int count = 0;
															#line 785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				for(;count < maxCount && !limits.isReached();count++) {
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					limits.decrement(start[count]);
															#line 3291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
															#line 789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				itemsPastEnd += maxCount - count;
															#line 792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (count)
															#line 3297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					result.append(result.arena(), start, count);
															#line 3301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				++it;
															#line 3305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
			else
			{
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				++it;
															#line 3311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
			loopDepth = a_body1cont1loopBody1cont10(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont4(Key const& resolvedEnd,int loopDepth) 
	{
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedEnd == allKeys.begin)
															#line 3322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readToBegin = true;
															#line 3326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedEnd == ryw->getMaxReadKey())
															#line 3330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readThroughEnd = true;
															#line 3334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		end = firstGreaterOrEqual(resolvedEnd);
															#line 3338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1cont1loopBody1cont4(Key && resolvedEnd,int loopDepth) 
	{
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedEnd == allKeys.begin)
															#line 3347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readToBegin = true;
															#line 3351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedEnd == ryw->getMaxReadKey())
															#line 3355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readThroughEnd = true;
															#line 3359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		end = firstGreaterOrEqual(resolvedEnd);
															#line 3363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Key const& resolvedEnd,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont4(resolvedEnd, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Key && resolvedEnd,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont4(std::move(resolvedEnd), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetRangeValueActor*>(this)->actor_wait_state > 0) static_cast<GetRangeValueActor*>(this)->actor_wait_state = 0;
		static_cast<GetRangeValueActor*>(this)->ActorCallback< GetRangeValueActor, 1, Key >::remove();

	}
	void a_callback_fire(ActorCallback< GetRangeValueActor, 1, Key >*,Key const& value) 
	{
		fdb_probe_actor_enter("getRangeValue", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValue", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetRangeValueActor, 1, Key >*,Key && value) 
	{
		fdb_probe_actor_enter("getRangeValue", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValue", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetRangeValueActor, 1, Key >*,Error err) 
	{
		fdb_probe_actor_enter("getRangeValue", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValue", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont1loopBody1cont10(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont15(RangeResult const& snapshot_read,int loopDepth) 
	{
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		KeyRangeRef range = getKnownKeyRange(snapshot_read, read_begin, read_end, ryw->arena);
															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (ryw->cache.insert(range, snapshot_read))
															#line 3443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->arena.dependsOn(snapshot_read.arena());
															#line 3447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache( begin, it, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualBeginOffset);
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache( end, itEnd, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualEndOffset);
															#line 3453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont15(RangeResult && snapshot_read,int loopDepth) 
	{
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		KeyRangeRef range = getKnownKeyRange(snapshot_read, read_begin, read_end, ryw->arena);
															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (ryw->cache.insert(range, snapshot_read))
															#line 3464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->arena.dependsOn(snapshot_read.arena());
															#line 3468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache( begin, it, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualBeginOffset);
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache( end, itEnd, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualEndOffset);
															#line 3474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1when1(RangeResult const& snapshot_read,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont15(snapshot_read, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1when1(RangeResult && snapshot_read,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont15(std::move(snapshot_read), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetRangeValueActor*>(this)->actor_wait_state > 0) static_cast<GetRangeValueActor*>(this)->actor_wait_state = 0;
		static_cast<GetRangeValueActor*>(this)->ActorCallback< GetRangeValueActor, 2, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetRangeValueActor, 2, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getRangeValue", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValue", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetRangeValueActor, 2, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getRangeValue", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValue", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetRangeValueActor, 2, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getRangeValue", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValue", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	KeySelector begin;
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	KeySelector end;
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetRangeLimits limits;
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Iter* pit;
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Iter& it;
															#line 600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Iter itEnd;
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	RangeResult result;
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	int64_t additionalRows;
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	int itemsPastEnd;
															#line 604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	int requestCount;
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	bool readToBegin;
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	bool readThroughEnd;
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	int actualBeginOffset;
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	int actualEndOffset;
															#line 705 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	KeySelector read_end;
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	KeySelector read_begin;
															#line 748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetRangeLimits requestLimit;
															#line 3578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via getRangeValue()
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Iter>
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class GetRangeValueActor final : public Actor<RangeResult>, public ActorCallback< GetRangeValueActor<Iter>, 0, Key >, public ActorCallback< GetRangeValueActor<Iter>, 1, Key >, public ActorCallback< GetRangeValueActor<Iter>, 2, RangeResult >, public FastAllocated<GetRangeValueActor<Iter>>, public GetRangeValueActorState<Iter, GetRangeValueActor<Iter>> {
															#line 3585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<GetRangeValueActor<Iter>>::operator new;
	using FastAllocated<GetRangeValueActor<Iter>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetRangeValueActor<Iter>, 0, Key >;
friend struct ActorCallback< GetRangeValueActor<Iter>, 1, Key >;
friend struct ActorCallback< GetRangeValueActor<Iter>, 2, RangeResult >;
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetRangeValueActor(ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Iter* const& pit) 
															#line 3598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<RangeResult>(),
		   GetRangeValueActorState<Iter, GetRangeValueActor<Iter>>(ryw, begin, end, limits, pit)
	{
		fdb_probe_actor_enter("getRangeValue", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getRangeValue");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getRangeValue", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetRangeValueActor<Iter>, 0, Key >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetRangeValueActor<Iter>, 1, Key >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GetRangeValueActor<Iter>, 2, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Iter>
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<RangeResult> getRangeValue( ReadYourWritesTransaction* const& ryw, KeySelector const& begin, KeySelector const& end, GetRangeLimits const& limits, Iter* const& pit ) {
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<RangeResult>(new GetRangeValueActor<Iter>(ryw, begin, end, limits, pit));
															#line 3629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"

	static KeyRangeRef getKnownKeyRangeBack(RangeResultRef data, KeySelector begin, KeySelector end, Arena& arena) {
		StringRef beginKey = !data.more && begin.offset <= 1 ? begin.getKey() : allKeys.end;
		ExtStringRef endKey = end.offset >= 1 ? end.getKey() : allKeys.begin;

		if (data.readToBegin)
			beginKey = allKeys.begin;
		if (data.readThroughEnd)
			endKey = allKeys.end;

		if (data.size()) {
			if (data.readThrough.present()) {
				beginKey = std::min(data.readThrough.get(), beginKey);
			} else {
				beginKey = !data.more && data.end()[-1].key > beginKey ? beginKey : data.end()[-1].key;
			}

			endKey = data[0].key < endKey ? endKey : ExtStringRef(data[0].key, 1);
		}
		if (beginKey >= endKey)
			return KeyRangeRef();

		return KeyRangeRef(StringRef(arena, beginKey), endKey.toArena(arena));
	}

	// Pre: it points to an unknown range
	// Decrements it to point to the unknown range just before the last nontrivial known range (skips over trivial known
	// ranges), but not more than iterationLimit ranges away Returns the number of single-key empty ranges skipped
	template <class Iter>
	static int skipUncachedBack(Iter& it, Iter const& end, int iterationLimit) {
		ExtStringRef b = it.beginKey();
		ExtStringRef e = it.endKey();
		int singleEmpty = 0;
		ASSERT(!it.is_unreadable() && it.is_unknown_range());

		// b == it.beginKey()
		// e is the end of the contiguous empty range containing it
		while (it != end && --iterationLimit >= 0) {
			if (it.is_unreadable() || it.is_empty_range()) {
				if (it.is_unreadable() || !e.isKeyAfter(b)) { // Assumes no degenerate ranges
					while (it.is_unreadable() || !it.is_unknown_range())
						++it;
					return singleEmpty;
				}
				singleEmpty++;
			} else
				e = b;
			--it;
			b = it.beginKey();
		}
		while (it.is_unreadable() || !it.is_unknown_range())
			++it;
		return singleEmpty;
	}

	// Pre: it points to an unknown range
	// Returns the number of preceding empty single-key known ranges between it and the previous nontrivial known range,
	// but no more than maxClears Leaves it in an indeterminate state
	template <class Iter>
	static int countUncachedBack(Iter&& it, int maxClears) {
		if (maxClears <= 0)
			return 0;
		ExtStringRef b = it.beginKey();
		ExtStringRef e = it.endKey();
		int singleEmpty = 0;
		while (b > allKeys.begin) {
			if (it.is_unreadable() || it.is_empty_range()) {
				if (it.is_unreadable() || !e.isKeyAfter(b)) { // Assumes no degenerate ranges
					return singleEmpty;
				}
				singleEmpty++;
				if (singleEmpty >= maxClears)
					return maxClears;
			} else
				e = b;
			--it;
			b = it.beginKey();
		}
		return singleEmpty;
	}

																#line 3714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via getRangeValueBack()
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Iter, class GetRangeValueBackActor>
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class GetRangeValueBackActorState {
															#line 3720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetRangeValueBackActorState(ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Iter* const& pit) 
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw(ryw),
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   begin(begin),
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   end(end),
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   limits(limits),
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   pit(pit),
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   it(*pit),
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   itEnd(*pit),
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   result(),
															#line 905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   additionalRows(0),
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   itemsPastBegin(0),
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   requestCount(0),
															#line 908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   readToBegin(false),
															#line 909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   readThroughEnd(false),
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   actualBeginOffset(begin.offset),
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   actualEndOffset(end.offset)
															#line 3755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("getRangeValueBack", reinterpret_cast<unsigned long>(this));

	}
	~GetRangeValueBackActorState() 
	{
		fdb_probe_actor_destroy("getRangeValueBack", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			resolveKeySelectorFromCache(end, it, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualEndOffset);
															#line 915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			resolveKeySelectorFromCache( begin, itEnd, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualBeginOffset);
															#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (actualBeginOffset >= actualEndOffset && begin.getKey() >= end.getKey())
															#line 3774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (!static_cast<GetRangeValueBackActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(false, false)); this->~GetRangeValueBackActorState(); static_cast<GetRangeValueBackActor*>(this)->destroy(); return 0; }
															#line 3778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				new (&static_cast<GetRangeValueBackActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(false, false));
				this->~GetRangeValueBackActorState();
				static_cast<GetRangeValueBackActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if ((begin.isFirstGreaterOrEqual() && begin.getKey() == ryw->getMaxReadKey()) || (end.isFirstGreaterOrEqual() && end.getKey() == allKeys.begin))
															#line 3788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					if (!static_cast<GetRangeValueBackActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(readToBegin, readThroughEnd)); this->~GetRangeValueBackActorState(); static_cast<GetRangeValueBackActor*>(this)->destroy(); return 0; }
															#line 3792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					new (&static_cast<GetRangeValueBackActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(readToBegin, readThroughEnd));
					this->~GetRangeValueBackActorState();
					static_cast<GetRangeValueBackActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!begin.isFirstGreaterOrEqual() && begin.getKey() > end.getKey())
															#line 3801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				StrictFuture<Key> __when_expr_0 = read(ryw, GetKeyReq(begin), pit);
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (static_cast<GetRangeValueBackActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetRangeValueBackActor*>(this)->actor_wait_state = 1;
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetRangeValueBackActor, 0, Key >*>(static_cast<GetRangeValueBackActor*>(this)));
															#line 3812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~GetRangeValueBackActorState();
		static_cast<GetRangeValueBackActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		;
															#line 3840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Key const& resolvedBegin,int loopDepth) 
	{
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedBegin == allKeys.begin)
															#line 3849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readToBegin = true;
															#line 3853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedBegin == ryw->getMaxReadKey())
															#line 3857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readThroughEnd = true;
															#line 3861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedBegin >= end.getKey() && end.offset <= 1)
															#line 3865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<GetRangeValueBackActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(false, false)); this->~GetRangeValueBackActorState(); static_cast<GetRangeValueBackActor*>(this)->destroy(); return 0; }
															#line 3869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<GetRangeValueBackActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(false, false));
			this->~GetRangeValueBackActorState();
			static_cast<GetRangeValueBackActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (resolvedBegin == ryw->getMaxReadKey())
															#line 3879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (!static_cast<GetRangeValueBackActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(readToBegin, readThroughEnd)); this->~GetRangeValueBackActorState(); static_cast<GetRangeValueBackActor*>(this)->destroy(); return 0; }
															#line 3883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				new (&static_cast<GetRangeValueBackActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(readToBegin, readThroughEnd));
				this->~GetRangeValueBackActorState();
				static_cast<GetRangeValueBackActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache(end, it, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualEndOffset);
															#line 939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache( begin, itEnd, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualBeginOffset);
															#line 3894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Key && resolvedBegin,int loopDepth) 
	{
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedBegin == allKeys.begin)
															#line 3903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readToBegin = true;
															#line 3907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedBegin == ryw->getMaxReadKey())
															#line 3911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readThroughEnd = true;
															#line 3915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedBegin >= end.getKey() && end.offset <= 1)
															#line 3919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<GetRangeValueBackActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(false, false)); this->~GetRangeValueBackActorState(); static_cast<GetRangeValueBackActor*>(this)->destroy(); return 0; }
															#line 3923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<GetRangeValueBackActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(false, false));
			this->~GetRangeValueBackActorState();
			static_cast<GetRangeValueBackActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (resolvedBegin == ryw->getMaxReadKey())
															#line 3933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (!static_cast<GetRangeValueBackActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(readToBegin, readThroughEnd)); this->~GetRangeValueBackActorState(); static_cast<GetRangeValueBackActor*>(this)->destroy(); return 0; }
															#line 3937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				new (&static_cast<GetRangeValueBackActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(readToBegin, readThroughEnd));
				this->~GetRangeValueBackActorState();
				static_cast<GetRangeValueBackActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache(end, it, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualEndOffset);
															#line 939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache( begin, itEnd, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualBeginOffset);
															#line 3948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Key const& resolvedBegin,int loopDepth) 
	{
		loopDepth = a_body1cont5(resolvedBegin, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Key && resolvedBegin,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(resolvedBegin), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetRangeValueBackActor*>(this)->actor_wait_state > 0) static_cast<GetRangeValueBackActor*>(this)->actor_wait_state = 0;
		static_cast<GetRangeValueBackActor*>(this)->ActorCallback< GetRangeValueBackActor, 0, Key >::remove();

	}
	void a_callback_fire(ActorCallback< GetRangeValueBackActor, 0, Key >*,Key const& value) 
	{
		fdb_probe_actor_enter("getRangeValueBack", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValueBack", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetRangeValueBackActor, 0, Key >*,Key && value) 
	{
		fdb_probe_actor_enter("getRangeValueBack", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValueBack", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetRangeValueBackActor, 0, Key >*,Error err) 
	{
		fdb_probe_actor_enter("getRangeValueBack", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValueBack", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont12(int loopDepth) 
	{
															#line 1111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		result.more = result.more || limits.isReached();
															#line 1113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (begin.isFirstGreaterOrEqual())
															#line 4022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			int keepItems = result.rend() - std::lower_bound(result.rbegin(), result.rend(), begin.getKey(), KeyValueRef::OrderByKey());
															#line 1116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (keepItems < result.size())
															#line 4028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				result.more = false;
															#line 4032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
															#line 1119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			result.resize(result.arena(), keepItems);
															#line 4036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		result.readToBegin = !result.more && readToBegin;
															#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		result.readThroughEnd = readThroughEnd;
															#line 1124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		result.arena().dependsOn(ryw->arena);
															#line 1126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<GetRangeValueBackActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetRangeValueBackActorState(); static_cast<GetRangeValueBackActor*>(this)->destroy(); return 0; }
															#line 4046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<GetRangeValueBackActor*>(this)->SAV< RangeResult >::value()) RangeResult(std::move(result)); // state_var_RVO
		this->~GetRangeValueBackActorState();
		static_cast<GetRangeValueBackActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!result.size() && actualBeginOffset >= actualEndOffset && begin.getKey() >= end.getKey())
															#line 4065 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<GetRangeValueBackActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(false, false)); this->~GetRangeValueBackActorState(); static_cast<GetRangeValueBackActor*>(this)->destroy(); return 0; }
															#line 4069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<GetRangeValueBackActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(false, false));
			this->~GetRangeValueBackActorState();
			static_cast<GetRangeValueBackActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!begin.isBackward() && begin.getKey() >= ryw->getMaxReadKey())
															#line 4077 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<GetRangeValueBackActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(readToBegin, readThroughEnd)); this->~GetRangeValueBackActorState(); static_cast<GetRangeValueBackActor*>(this)->destroy(); return 0; }
															#line 4081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<GetRangeValueBackActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(readToBegin, readThroughEnd));
			this->~GetRangeValueBackActorState();
			static_cast<GetRangeValueBackActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if ((begin.offset >= end.offset && begin.getKey() >= end.getKey()) || (end.offset <= 1 && end.getKey() == allKeys.begin))
															#line 4089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (begin.isFirstGreaterOrEqual())
															#line 4093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
				return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
			}
															#line 969 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!result.size())
															#line 4099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
				return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
			}
															#line 971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<Key> __when_expr_1 = read(ryw, GetKeyReq(begin), pit);
															#line 971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (static_cast<GetRangeValueBackActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetRangeValueBackActor*>(this)->actor_wait_state = 2;
															#line 971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetRangeValueBackActor, 1, Key >*>(static_cast<GetRangeValueBackActor*>(this)));
															#line 4112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont1loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1break1(int loopDepth) 
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
	int a_body1cont1loopBody1cont1(int loopDepth) 
	{
															#line 983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (itemsPastBegin >= begin.offset - 1 && !it.is_unreadable() && !it.is_unknown_range() && it.beginKey() < itEnd.beginKey())
															#line 4139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (begin.isFirstGreaterOrEqual())
															#line 4143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
				return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
			}
															#line 987 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<GetRangeValueBackActor*>(this)->SAV<RangeResult>::futures) { (void)(RangeResultRef(readToBegin, readThroughEnd)); this->~GetRangeValueBackActorState(); static_cast<GetRangeValueBackActor*>(this)->destroy(); return 0; }
															#line 4149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<GetRangeValueBackActor*>(this)->SAV< RangeResult >::value()) RangeResult(RangeResultRef(readToBegin, readThroughEnd));
			this->~GetRangeValueBackActorState();
			static_cast<GetRangeValueBackActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (limits.isReached() && itemsPastBegin >= begin.offset - 1)
															#line 4157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 993 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (end.isFirstGreaterOrEqual() && end.getKey() == it.beginKey())
															#line 4163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (itemsPastBegin >= begin.offset - 1 && it == itEnd)
															#line 4167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
				return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
			}
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			--it;
															#line 4173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (it.is_unknown_range())
															#line 4177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (limits.hasByteLimit() && result.size() && itemsPastBegin >= begin.offset - 1)
															#line 4181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				result.more = true;
															#line 4185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
			}
															#line 1005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			Iter ucEnd(it);
															#line 1006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			int singleClears = 0;
															#line 1007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			int clearLimit = requestCount ? 1 << std::min(requestCount, 20) : 0;
															#line 1008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (it.beginKey() > itEnd.beginKey())
															#line 4196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				singleClears = std::min(skipUncachedBack(ucEnd, itEnd, BUGGIFY ? 0 : clearLimit + 100), clearLimit);
															#line 4200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
															#line 1011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			read_begin = KeySelector();
															#line 1012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (ucEnd != itEnd)
															#line 4206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				Key k = ucEnd.beginKey().toStandaloneStringRef();
															#line 1014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				read_begin = KeySelector(firstGreaterOrEqual(k), k.arena());
															#line 1015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (begin.offset > 1)
															#line 4214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 1016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					additionalRows += begin.offset - 1;
															#line 4218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
			}
			else
			{
															#line 1017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (begin.offset > 1)
															#line 4225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 1018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					read_begin = KeySelector(firstGreaterOrEqual(begin.getKey()), begin.arena());
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					additionalRows += begin.offset - 1;
															#line 4231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
				else
				{
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					read_begin = begin;
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					if (begin.offset < 1)
															#line 4239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					{
															#line 1023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
						singleClears += countUncachedBack(std::move(ucEnd), clearLimit - singleClears);
															#line 1024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
						read_begin.offset -= singleClears;
															#line 4245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					}
				}
			}
															#line 1028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			additionalRows += singleClears;
															#line 1030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			read_end = KeySelector();
															#line 1031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (end.isFirstGreaterOrEqual())
															#line 4255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				Key k = it.endKey() < end.getKey() ? it.endKey().toStandaloneStringRef() : end.getKey();
															#line 1033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				end = KeySelector(firstGreaterOrEqual(k), k.arena());
															#line 1034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				read_end = end;
															#line 4263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
			else
			{
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (end.offset < 1)
															#line 4269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					read_end = KeySelector(firstGreaterOrEqual(end.getKey()), end.arena());
															#line 1037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					additionalRows += 1 - end.offset;
															#line 4275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
				else
				{
															#line 1039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					read_end = end;
															#line 1040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					ucEnd = it;
															#line 1042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					singleClears = countUncached(std::move(ucEnd), ryw->getMaxReadKey(), clearLimit);
															#line 1043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					read_end.offset += singleClears;
															#line 1044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					additionalRows += singleClears;
															#line 4289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
			}
															#line 1047 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (read_begin.getKey() > read_end.getKey())
															#line 4294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				read_begin.setKey(read_end.getKey());
															#line 1049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				read_begin.arena().dependsOn(read_end.arena());
															#line 4300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
															#line 1052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			requestLimit = limits;
															#line 1053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			setRequestLimits(requestLimit, additionalRows, read_end.offset, requestCount);
															#line 1054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			requestCount++;
															#line 1056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ASSERT(!requestLimit.hasRowLimit() || requestLimit.rows > 0);
															#line 1057 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ASSERT(requestLimit.hasRowLimit() || requestLimit.hasByteLimit());
															#line 1061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			additionalRows = 0;
															#line 1062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<RangeResult> __when_expr_2 = ryw->tr.getRange(read_begin, read_end, requestLimit, Snapshot::True, Reverse::True);
															#line 1062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (static_cast<GetRangeValueBackActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
			static_cast<GetRangeValueBackActor*>(this)->actor_wait_state = 3;
															#line 1062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetRangeValueBackActor, 2, RangeResult >*>(static_cast<GetRangeValueBackActor*>(this)));
															#line 4323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 1083 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			KeyValueRef const* end = it.is_kv() ? it.kv(ryw->arena) : nullptr;
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (end != nullptr)
															#line 4332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				it.skipContiguousBack(begin.isFirstGreaterOrEqual() ? begin.getKey() : allKeys.begin);
															#line 1086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				KeyValueRef const* start = it.kv(ryw->arena);
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				ASSERT(start != nullptr);
															#line 1089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				int maxCount = end - start + 1;
															#line 1090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				int count = 0;
															#line 1091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				for(;count < maxCount && !limits.isReached();count++) {
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					limits.decrement(start[maxCount - count - 1]);
															#line 4348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
															#line 1095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				itemsPastBegin += maxCount - count;
															#line 1097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (count)
															#line 4354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 1098 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					int size = result.size();
															#line 1099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					result.resize(result.arena(), size + count);
															#line 1100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					for(int i = 0;i < count;i++) {
															#line 1101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
						result[size + i] = start[maxCount - i - 1];
															#line 4364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					}
				}
			}
															#line 1105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (it == itEnd)
															#line 4370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
				return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
			}
															#line 1107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			--it;
															#line 4376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			loopDepth = a_body1cont1loopBody1cont10(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont4(Key const& resolvedBegin,int loopDepth) 
	{
															#line 975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedBegin == allKeys.begin)
															#line 4386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readToBegin = true;
															#line 4390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedBegin == ryw->getMaxReadKey())
															#line 4394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readThroughEnd = true;
															#line 4398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		begin = firstGreaterOrEqual(resolvedBegin);
															#line 4402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1cont1loopBody1cont4(Key && resolvedBegin,int loopDepth) 
	{
															#line 975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedBegin == allKeys.begin)
															#line 4411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readToBegin = true;
															#line 4415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (resolvedBegin == ryw->getMaxReadKey())
															#line 4419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			readThroughEnd = true;
															#line 4423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		begin = firstGreaterOrEqual(resolvedBegin);
															#line 4427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Key const& resolvedBegin,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont4(resolvedBegin, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Key && resolvedBegin,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont4(std::move(resolvedBegin), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetRangeValueBackActor*>(this)->actor_wait_state > 0) static_cast<GetRangeValueBackActor*>(this)->actor_wait_state = 0;
		static_cast<GetRangeValueBackActor*>(this)->ActorCallback< GetRangeValueBackActor, 1, Key >::remove();

	}
	void a_callback_fire(ActorCallback< GetRangeValueBackActor, 1, Key >*,Key const& value) 
	{
		fdb_probe_actor_enter("getRangeValueBack", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValueBack", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetRangeValueBackActor, 1, Key >*,Key && value) 
	{
		fdb_probe_actor_enter("getRangeValueBack", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValueBack", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetRangeValueBackActor, 1, Key >*,Error err) 
	{
		fdb_probe_actor_enter("getRangeValueBack", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValueBack", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont1loopBody1cont10(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont16(RangeResult const& snapshot_read,int loopDepth) 
	{
															#line 1064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		KeyRangeRef range = getKnownKeyRangeBack(snapshot_read, read_begin, read_end, ryw->arena);
															#line 1068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		RangeResultRef reversed;
															#line 1069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		reversed.resize(ryw->arena, snapshot_read.size());
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		for(int i = 0;i < snapshot_read.size();i++) {
															#line 1071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			reversed[snapshot_read.size() - i - 1] = snapshot_read[i];
															#line 4513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (ryw->cache.insert(range, reversed))
															#line 4517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->arena.dependsOn(snapshot_read.arena());
															#line 4521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1078 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache( end, it, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualEndOffset);
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache( begin, itEnd, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualBeginOffset);
															#line 4527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont16(RangeResult && snapshot_read,int loopDepth) 
	{
															#line 1064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		KeyRangeRef range = getKnownKeyRangeBack(snapshot_read, read_begin, read_end, ryw->arena);
															#line 1068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		RangeResultRef reversed;
															#line 1069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		reversed.resize(ryw->arena, snapshot_read.size());
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		for(int i = 0;i < snapshot_read.size();i++) {
															#line 1071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			reversed[snapshot_read.size() - i - 1] = snapshot_read[i];
															#line 4544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (ryw->cache.insert(range, reversed))
															#line 4548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->arena.dependsOn(snapshot_read.arena());
															#line 4552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1078 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache( end, it, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualEndOffset);
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		resolveKeySelectorFromCache( begin, itEnd, ryw->getMaxReadKey(), &readToBegin, &readThroughEnd, &actualBeginOffset);
															#line 4558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = a_body1cont1loopBody1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1when1(RangeResult const& snapshot_read,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont16(snapshot_read, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1when1(RangeResult && snapshot_read,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont16(std::move(snapshot_read), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetRangeValueBackActor*>(this)->actor_wait_state > 0) static_cast<GetRangeValueBackActor*>(this)->actor_wait_state = 0;
		static_cast<GetRangeValueBackActor*>(this)->ActorCallback< GetRangeValueBackActor, 2, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetRangeValueBackActor, 2, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getRangeValueBack", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValueBack", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetRangeValueBackActor, 2, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getRangeValueBack", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValueBack", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetRangeValueBackActor, 2, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getRangeValueBack", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeValueBack", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	KeySelector begin;
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	KeySelector end;
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetRangeLimits limits;
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Iter* pit;
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Iter& it;
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Iter itEnd;
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	RangeResult result;
															#line 905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	int64_t additionalRows;
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	int itemsPastBegin;
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	int requestCount;
															#line 908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	bool readToBegin;
															#line 909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	bool readThroughEnd;
															#line 910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	int actualBeginOffset;
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	int actualEndOffset;
															#line 1011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	KeySelector read_begin;
															#line 1030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	KeySelector read_end;
															#line 1052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetRangeLimits requestLimit;
															#line 4662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via getRangeValueBack()
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Iter>
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class GetRangeValueBackActor final : public Actor<RangeResult>, public ActorCallback< GetRangeValueBackActor<Iter>, 0, Key >, public ActorCallback< GetRangeValueBackActor<Iter>, 1, Key >, public ActorCallback< GetRangeValueBackActor<Iter>, 2, RangeResult >, public FastAllocated<GetRangeValueBackActor<Iter>>, public GetRangeValueBackActorState<Iter, GetRangeValueBackActor<Iter>> {
															#line 4669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<GetRangeValueBackActor<Iter>>::operator new;
	using FastAllocated<GetRangeValueBackActor<Iter>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetRangeValueBackActor<Iter>, 0, Key >;
friend struct ActorCallback< GetRangeValueBackActor<Iter>, 1, Key >;
friend struct ActorCallback< GetRangeValueBackActor<Iter>, 2, RangeResult >;
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetRangeValueBackActor(ReadYourWritesTransaction* const& ryw,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Iter* const& pit) 
															#line 4682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<RangeResult>(),
		   GetRangeValueBackActorState<Iter, GetRangeValueBackActor<Iter>>(ryw, begin, end, limits, pit)
	{
		fdb_probe_actor_enter("getRangeValueBack", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getRangeValueBack");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getRangeValueBack", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetRangeValueBackActor<Iter>, 0, Key >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetRangeValueBackActor<Iter>, 1, Key >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GetRangeValueBackActor<Iter>, 2, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class Iter>
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<RangeResult> getRangeValueBack( ReadYourWritesTransaction* const& ryw, KeySelector const& begin, KeySelector const& end, GetRangeLimits const& limits, Iter* const& pit ) {
															#line 896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<RangeResult>(new GetRangeValueBackActor<Iter>(ryw, begin, end, limits, pit));
															#line 4713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 1128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"

#ifndef __INTEL_COMPILER
#pragma region GetMappedRange
#endif

	template <class Iter>
	static Future<MappedRangeResult> read(ReadYourWritesTransaction* ryw, GetMappedRangeReq<false> read, Iter* it) {
		return getMappedRangeValue(ryw, read.begin, read.end, read.mapper, read.limits, it);
	};

	template <class Iter>
	static Future<MappedRangeResult> read(ReadYourWritesTransaction* ryw, GetMappedRangeReq<true> read, Iter* it) {
		throw unsupported_operation();
		// TODO: Support reverse. return getMappedRangeValueBack(ryw, read.begin, read.end, read.mapper,
		// read.limits, it);
	};

																#line 4734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via readThrough()
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <bool backwards, class ReadThroughActor2>
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadThroughActor2State {
															#line 4740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadThroughActor2State(ReadYourWritesTransaction* const& ryw,GetMappedRangeReq<backwards> const& read,Snapshot const& snapshot) 
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw(ryw),
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   read(read),
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   snapshot(snapshot)
															#line 4751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("readThrough", reinterpret_cast<unsigned long>(this));

	}
	~ReadThroughActor2State() 
	{
		fdb_probe_actor_destroy("readThrough", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (backwards && read.end.offset > 1)
															#line 4766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				StrictFuture<Key> __when_expr_0 = ryw->tr.getKey(read.end, snapshot);
															#line 1152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (static_cast<ReadThroughActor2*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ReadThroughActor2*>(this)->actor_wait_state = 1;
															#line 1152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadThroughActor2, 0, Key >*>(static_cast<ReadThroughActor2*>(this)));
															#line 4777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~ReadThroughActor2State();
		static_cast<ReadThroughActor2*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		StrictFuture<MappedRangeResult> __when_expr_1 = ryw->tr.getMappedRange( read.begin, read.end, read.mapper, read.limits, snapshot, backwards ? Reverse::True : Reverse::False);
															#line 1158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (static_cast<ReadThroughActor2*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadThroughActor2*>(this)->actor_wait_state = 2;
															#line 1158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadThroughActor2, 1, MappedRangeResult >*>(static_cast<ReadThroughActor2*>(this)));
															#line 4812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Key const& key,int loopDepth) 
	{
															#line 1153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (key > ryw->getMaxReadKey())
															#line 4821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			read.end = firstGreaterOrEqual(ryw->getMaxReadKey());
															#line 4825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
		else
		{
															#line 1156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			read.end = KeySelector(firstGreaterOrEqual(key), key.arena());
															#line 4831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Key && key,int loopDepth) 
	{
															#line 1153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (key > ryw->getMaxReadKey())
															#line 4841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			read.end = firstGreaterOrEqual(ryw->getMaxReadKey());
															#line 4845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
		else
		{
															#line 1156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			read.end = KeySelector(firstGreaterOrEqual(key), key.arena());
															#line 4851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Key const& key,int loopDepth) 
	{
		loopDepth = a_body1cont2(key, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Key && key,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(key), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadThroughActor2*>(this)->actor_wait_state > 0) static_cast<ReadThroughActor2*>(this)->actor_wait_state = 0;
		static_cast<ReadThroughActor2*>(this)->ActorCallback< ReadThroughActor2, 0, Key >::remove();

	}
	void a_callback_fire(ActorCallback< ReadThroughActor2, 0, Key >*,Key const& value) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadThroughActor2, 0, Key >*,Key && value) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadThroughActor2, 0, Key >*,Error err) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont6(MappedRangeResult const& v,int loopDepth) 
	{
															#line 1160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadThroughActor2*>(this)->SAV<MappedRangeResult>::futures) { (void)(v); this->~ReadThroughActor2State(); static_cast<ReadThroughActor2*>(this)->destroy(); return 0; }
															#line 4924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadThroughActor2*>(this)->SAV< MappedRangeResult >::value()) MappedRangeResult(v);
		this->~ReadThroughActor2State();
		static_cast<ReadThroughActor2*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(MappedRangeResult && v,int loopDepth) 
	{
															#line 1160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadThroughActor2*>(this)->SAV<MappedRangeResult>::futures) { (void)(v); this->~ReadThroughActor2State(); static_cast<ReadThroughActor2*>(this)->destroy(); return 0; }
															#line 4936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadThroughActor2*>(this)->SAV< MappedRangeResult >::value()) MappedRangeResult(v);
		this->~ReadThroughActor2State();
		static_cast<ReadThroughActor2*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(MappedRangeResult const& v,int loopDepth) 
	{
		loopDepth = a_body1cont6(v, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(MappedRangeResult && v,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(v), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReadThroughActor2*>(this)->actor_wait_state > 0) static_cast<ReadThroughActor2*>(this)->actor_wait_state = 0;
		static_cast<ReadThroughActor2*>(this)->ActorCallback< ReadThroughActor2, 1, MappedRangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ReadThroughActor2, 1, MappedRangeResult >*,MappedRangeResult const& value) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadThroughActor2, 1, MappedRangeResult >*,MappedRangeResult && value) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadThroughActor2, 1, MappedRangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetMappedRangeReq<backwards> read;
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Snapshot snapshot;
															#line 5013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via readThrough()
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <bool backwards>
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadThroughActor2 final : public Actor<MappedRangeResult>, public ActorCallback< ReadThroughActor2<backwards>, 0, Key >, public ActorCallback< ReadThroughActor2<backwards>, 1, MappedRangeResult >, public FastAllocated<ReadThroughActor2<backwards>>, public ReadThroughActor2State<backwards, ReadThroughActor2<backwards>> {
															#line 5020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<ReadThroughActor2<backwards>>::operator new;
	using FastAllocated<ReadThroughActor2<backwards>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<MappedRangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadThroughActor2<backwards>, 0, Key >;
friend struct ActorCallback< ReadThroughActor2<backwards>, 1, MappedRangeResult >;
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadThroughActor2(ReadYourWritesTransaction* const& ryw,GetMappedRangeReq<backwards> const& read,Snapshot const& snapshot) 
															#line 5032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<MappedRangeResult>(),
		   ReadThroughActor2State<backwards, ReadThroughActor2<backwards>>(ryw, read, snapshot)
	{
		fdb_probe_actor_enter("readThrough", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readThrough");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readThrough", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadThroughActor2<backwards>, 0, Key >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReadThroughActor2<backwards>, 1, MappedRangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <bool backwards>
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<MappedRangeResult> readThrough( ReadYourWritesTransaction* const& ryw, GetMappedRangeReq<backwards> const& read, Snapshot const& snapshot ) {
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<MappedRangeResult>(new ReadThroughActor2<backwards>(ryw, read, snapshot));
															#line 5062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 1162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"

	template <bool backwards>
	static void addConflictRangeAndMustUnmodified(ReadYourWritesTransaction* ryw,
	                                              GetMappedRangeReq<backwards> read,
	                                              WriteMap::iterator& it,
	                                              MappedRangeResult result) {
		// Primary getRange.
		addConflictRange<true, MappedRangeResult>(
		    ryw, GetRangeReq<backwards>(read.begin, read.end, read.limits), it, result);

		// Secondary getValue/getRanges.
		for (const auto& mappedKeyValue : result) {
			const auto& reqAndResult = mappedKeyValue.reqAndResult;
			if (std::holds_alternative<GetValueReqAndResultRef>(reqAndResult)) {
				auto getValue = std::get<GetValueReqAndResultRef>(reqAndResult);
				// GetValueReq variation of addConflictRange require it to point at the right segment.
				it.skip(getValue.key);
				// The result is not used in GetValueReq variation of addConflictRange. Let's just pass in a
				// placeholder.
				addConflictRange<true>(ryw, GetValueReq(getValue.key), it, Optional<Value>());
			} else if (std::holds_alternative<GetRangeReqAndResultRef>(reqAndResult)) {
				auto getRange = std::get<GetRangeReqAndResultRef>(reqAndResult);
				// We only support forward scan for secondary getRange requests.
				// The limits are not used in addConflictRange. Let's just pass in a placeholder.
				addConflictRange<true>(
				    ryw, GetRangeReq<false>(getRange.begin, getRange.end, GetRangeLimits()), it, getRange.result);
			} else {
				throw internal_error();
			}
		}
	}

	// For Snapshot::True and NOT readYourWritesDisabled.
																#line 5099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via readWithConflictRangeRYW()
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <bool backwards, class ReadWithConflictRangeRYWActor1>
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadWithConflictRangeRYWActor1State {
															#line 5105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadWithConflictRangeRYWActor1State(ReadYourWritesTransaction* const& ryw,GetMappedRangeReq<backwards> const& req,Snapshot const& snapshot) 
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw(ryw),
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   req(req),
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   snapshot(snapshot)
															#line 5116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this));

	}
	~ReadWithConflictRangeRYWActor1State() 
	{
		fdb_probe_actor_destroy("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<MappedRangeResult> __when_expr_0 = readThrough(ryw, req, Snapshot::True);
															#line 1199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (static_cast<ReadWithConflictRangeRYWActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ryw->resetPromise.getFuture();
															#line 5137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<ReadWithConflictRangeRYWActor1*>(this)->actor_wait_state = 1;
															#line 1200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadWithConflictRangeRYWActor1, 0, MappedRangeResult >*>(static_cast<ReadWithConflictRangeRYWActor1*>(this)));
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadWithConflictRangeRYWActor1, 1, Void >*>(static_cast<ReadWithConflictRangeRYWActor1*>(this)));
															#line 5144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~ReadWithConflictRangeRYWActor1State();
		static_cast<ReadWithConflictRangeRYWActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(MappedRangeResult const& result,int loopDepth) 
	{
															#line 1204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		WriteMap::iterator writes(&ryw->writes);
															#line 1205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		addConflictRangeAndMustUnmodified<backwards>(ryw, req, writes, result);
															#line 1206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadWithConflictRangeRYWActor1*>(this)->SAV<MappedRangeResult>::futures) { (void)(result); this->~ReadWithConflictRangeRYWActor1State(); static_cast<ReadWithConflictRangeRYWActor1*>(this)->destroy(); return 0; }
															#line 5171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadWithConflictRangeRYWActor1*>(this)->SAV< MappedRangeResult >::value()) MappedRangeResult(result);
		this->~ReadWithConflictRangeRYWActor1State();
		static_cast<ReadWithConflictRangeRYWActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(MappedRangeResult && result,int loopDepth) 
	{
															#line 1204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		WriteMap::iterator writes(&ryw->writes);
															#line 1205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		addConflictRangeAndMustUnmodified<backwards>(ryw, req, writes, result);
															#line 1206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<ReadWithConflictRangeRYWActor1*>(this)->SAV<MappedRangeResult>::futures) { (void)(result); this->~ReadWithConflictRangeRYWActor1State(); static_cast<ReadWithConflictRangeRYWActor1*>(this)->destroy(); return 0; }
															#line 5187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<ReadWithConflictRangeRYWActor1*>(this)->SAV< MappedRangeResult >::value()) MappedRangeResult(result);
		this->~ReadWithConflictRangeRYWActor1State();
		static_cast<ReadWithConflictRangeRYWActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 1209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 5199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 1209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 5207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadWithConflictRangeRYWActor1*>(this)->actor_wait_state > 0) static_cast<ReadWithConflictRangeRYWActor1*>(this)->actor_wait_state = 0;
		static_cast<ReadWithConflictRangeRYWActor1*>(this)->ActorCallback< ReadWithConflictRangeRYWActor1, 0, MappedRangeResult >::remove();
		static_cast<ReadWithConflictRangeRYWActor1*>(this)->ActorCallback< ReadWithConflictRangeRYWActor1, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeRYWActor1, 0, MappedRangeResult >*,MappedRangeResult const& value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeRYWActor1, 0, MappedRangeResult >*,MappedRangeResult && value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadWithConflictRangeRYWActor1, 0, MappedRangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeRYWActor1, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadWithConflictRangeRYWActor1, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadWithConflictRangeRYWActor1, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetMappedRangeReq<backwards> req;
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Snapshot snapshot;
															#line 5314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via readWithConflictRangeRYW()
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <bool backwards>
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class ReadWithConflictRangeRYWActor1 final : public Actor<MappedRangeResult>, public ActorCallback< ReadWithConflictRangeRYWActor1<backwards>, 0, MappedRangeResult >, public ActorCallback< ReadWithConflictRangeRYWActor1<backwards>, 1, Void >, public FastAllocated<ReadWithConflictRangeRYWActor1<backwards>>, public ReadWithConflictRangeRYWActor1State<backwards, ReadWithConflictRangeRYWActor1<backwards>> {
															#line 5321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<ReadWithConflictRangeRYWActor1<backwards>>::operator new;
	using FastAllocated<ReadWithConflictRangeRYWActor1<backwards>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<MappedRangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadWithConflictRangeRYWActor1<backwards>, 0, MappedRangeResult >;
friend struct ActorCallback< ReadWithConflictRangeRYWActor1<backwards>, 1, Void >;
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadWithConflictRangeRYWActor1(ReadYourWritesTransaction* const& ryw,GetMappedRangeReq<backwards> const& req,Snapshot const& snapshot) 
															#line 5333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<MappedRangeResult>(),
		   ReadWithConflictRangeRYWActor1State<backwards, ReadWithConflictRangeRYWActor1<backwards>>(ryw, req, snapshot)
	{
		fdb_probe_actor_enter("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readWithConflictRangeRYW");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readWithConflictRangeRYW", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadWithConflictRangeRYWActor1<backwards>, 0, MappedRangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <bool backwards>
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<MappedRangeResult> readWithConflictRangeRYW( ReadYourWritesTransaction* const& ryw, GetMappedRangeReq<backwards> const& req, Snapshot const& snapshot ) {
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<MappedRangeResult>(new ReadWithConflictRangeRYWActor1<backwards>(ryw, req, snapshot));
															#line 5362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 1213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"

	template <bool backwards>
	static inline Future<MappedRangeResult> readWithConflictRangeForGetMappedRange(
	    ReadYourWritesTransaction* ryw,
	    GetMappedRangeReq<backwards> const& req,
	    Snapshot snapshot) {
		// For now, getMappedRange requires serializable isolation. (Technically it is trivial to add snapshot
		// isolation support. But it is not default and is rarely used. So we disallow it until we have thorough test
		// coverage for it.)
		if (snapshot) {
			CODE_PROBE(true, "getMappedRange not supported for snapshot.", probe::decoration::rare);
			throw unsupported_operation();
		}
		// For now, getMappedRange requires read-your-writes being NOT disabled. But the support of RYW is limited
		// to throwing get_mapped_range_reads_your_writes error when getMappedRange actually reads your own writes.
		// Applications should fall back in their own ways. This is different from what is usually expected from RYW,
		// which returns the written value transparently. In another word, it makes sure not break RYW semantics without
		// actually implementing reading from the writes.
		if (ryw->options.readYourWritesDisabled) {
			CODE_PROBE(true, "getMappedRange not supported for read-your-writes disabled.", probe::decoration::rare);
			throw unsupported_operation();
		}

		return readWithConflictRangeRYW(ryw, req, snapshot);
	}

#ifndef __INTEL_COMPILER
#pragma endregion
#endif

	static void triggerWatches(ReadYourWritesTransaction* ryw,
	                           KeyRangeRef range,
	                           Optional<ValueRef> val,
	                           bool valueKnown = true) {
		for (auto it = ryw->watchMap.lower_bound(range.begin); it != ryw->watchMap.end() && it->key < range.end;) {
			auto itCopy = it;
			++it;

			ASSERT(itCopy->value.size());
			CODE_PROBE(itCopy->value.size() > 1, "Multiple watches on the same key triggered by RYOW");

			for (int i = 0; i < itCopy->value.size(); i++) {
				if (itCopy->value[i]->onChangeTrigger.isSet()) {
					swapAndPop(&itCopy->value, i--);
				} else if (!valueKnown ||
				           (itCopy->value[i]->setPresent &&
				            (itCopy->value[i]->setValue.present() != val.present() ||
				             (val.present() && itCopy->value[i]->setValue.get() != val.get()))) ||
				           (itCopy->value[i]->valuePresent &&
				            (itCopy->value[i]->value.present() != val.present() ||
				             (val.present() && itCopy->value[i]->value.get() != val.get())))) {
					itCopy->value[i]->onChangeTrigger.send(Void());
					swapAndPop(&itCopy->value, i--);
				} else {
					itCopy->value[i]->setPresent = true;
					itCopy->value[i]->setValue = val.castTo<Value>();
				}
			}

			if (itCopy->value.size() == 0)
				ryw->watchMap.erase(itCopy);
		}
	}

	static void triggerWatches(ReadYourWritesTransaction* ryw,
	                           KeyRef key,
	                           Optional<ValueRef> val,
	                           bool valueKnown = true) {
		triggerWatches(ryw, singleKeyRange(key), val, valueKnown);
	}

																#line 5437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via watch()
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class WatchActor>
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class WatchActorState {
															#line 5443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	WatchActorState(ReadYourWritesTransaction* const& ryw,Key const& key) 
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw(ryw),
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   key(key),
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   val(),
															#line 1286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   watchFuture(),
															#line 1287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   watch(new Watch(key)),
															#line 1288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   done()
															#line 5460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("watch", reinterpret_cast<unsigned long>(this));

	}
	~WatchActorState() 
	{
		fdb_probe_actor_destroy("watch", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->reading.add(done.getFuture());
															#line 1292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!ryw->options.readYourWritesDisabled)
															#line 5477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				ryw->watchMap[key].push_back(watch);
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				val = readWithConflictRange(ryw, GetValueReq(key), Snapshot::False);
															#line 5483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
			else
			{
															#line 1296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				ryw->approximateSize += 2 * key.expectedSize() + 1;
															#line 1297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				val = ryw->tr.get(key);
															#line 5491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
			try {
															#line 1301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				StrictFuture<Void> __when_expr_0 = ryw->resetPromise.getFuture() || success(val) || watch->onChangeTrigger.getFuture();
															#line 1301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (static_cast<WatchActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 5498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<WatchActor*>(this)->actor_wait_state = 1;
															#line 1301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WatchActor, 0, Void >*>(static_cast<WatchActor*>(this)));
															#line 5503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~WatchActorState();
		static_cast<WatchActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (watch->onChangeTrigger.getFuture().isReady())
															#line 5532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			done.send(Void());
															#line 1309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (watch->onChangeTrigger.getFuture().isError())
															#line 5538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				return a_body1Catch1(watch->onChangeTrigger.getFuture().getError(), loopDepth);
															#line 5542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
															#line 1311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<WatchActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WatchActorState(); static_cast<WatchActor*>(this)->destroy(); return 0; }
															#line 5546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<WatchActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WatchActorState();
			static_cast<WatchActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		watch->valuePresent = true;
															#line 1315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		watch->value = val.get();
															#line 1317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (watch->setPresent && (watch->setValue.present() != watch->value.present() || (watch->value.present() && watch->setValue.get() != watch->value.get())))
															#line 5558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			watch->onChangeTrigger.send(Void());
															#line 1320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			done.send(Void());
															#line 1321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!static_cast<WatchActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WatchActorState(); static_cast<WatchActor*>(this)->destroy(); return 0; }
															#line 5566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			new (&static_cast<WatchActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WatchActorState();
			static_cast<WatchActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		try {
															#line 1325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			watchFuture = ryw->tr.watch(watch);
															#line 5575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			loopDepth = a_body1cont11(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			done.send(Void());
															#line 1304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 5593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

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
		if (static_cast<WatchActor*>(this)->actor_wait_state > 0) static_cast<WatchActor*>(this)->actor_wait_state = 0;
		static_cast<WatchActor*>(this)->ActorCallback< WatchActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WatchActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("watch", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watch", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WatchActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("watch", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watch", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WatchActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("watch", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watch", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont5(int loopDepth) 
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
	int a_body1cont6(int loopDepth) 
	{
															#line 1330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		done.send(Void());
															#line 1332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		StrictFuture<Void> __when_expr_1 = watchFuture;
															#line 1332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (static_cast<WatchActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont6when1(__when_expr_1.get(), loopDepth); };
		static_cast<WatchActor*>(this)->actor_wait_state = 2;
															#line 1332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WatchActor, 1, Void >*>(static_cast<WatchActor*>(this)));
															#line 5704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			done.send(Void());
															#line 1328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 5716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont11(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont6(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont12(Void const& _,int loopDepth) 
	{
															#line 1334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<WatchActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WatchActorState(); static_cast<WatchActor*>(this)->destroy(); return 0; }
															#line 5743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<WatchActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WatchActorState();
		static_cast<WatchActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont12(Void && _,int loopDepth) 
	{
															#line 1334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<WatchActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WatchActorState(); static_cast<WatchActor*>(this)->destroy(); return 0; }
															#line 5755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<WatchActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~WatchActorState();
		static_cast<WatchActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont12(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont12(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<WatchActor*>(this)->actor_wait_state > 0) static_cast<WatchActor*>(this)->actor_wait_state = 0;
		static_cast<WatchActor*>(this)->ActorCallback< WatchActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WatchActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("watch", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watch", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WatchActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("watch", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watch", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WatchActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("watch", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watch", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Key key;
															#line 1285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Future<Optional<Value>> val;
															#line 1286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Future<Void> watchFuture;
															#line 1287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Reference<Watch> watch;
															#line 1288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Promise<Void> done;
															#line 5838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via watch()
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class WatchActor final : public Actor<Void>, public ActorCallback< WatchActor, 0, Void >, public ActorCallback< WatchActor, 1, Void >, public FastAllocated<WatchActor>, public WatchActorState<WatchActor> {
															#line 5843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<WatchActor>::operator new;
	using FastAllocated<WatchActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WatchActor, 0, Void >;
friend struct ActorCallback< WatchActor, 1, Void >;
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	WatchActor(ReadYourWritesTransaction* const& ryw,Key const& key) 
															#line 5855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<Void>(),
		   WatchActorState<WatchActor>(ryw, key)
	{
		fdb_probe_actor_enter("watch", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("watch");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("watch", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WatchActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WatchActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<Void> watch( ReadYourWritesTransaction* const& ryw, Key const& key ) {
															#line 1284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<Void>(new WatchActor(ryw, key));
															#line 5883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 1336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"

																#line 5888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via simulateTimeoutInFlightCommit()
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class SimulateTimeoutInFlightCommitActor>
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class SimulateTimeoutInFlightCommitActorState {
															#line 5894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	SimulateTimeoutInFlightCommitActorState(ReadYourWritesTransaction* const& ryw_) 
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw_(ryw_),
															#line 1338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   ryw(Reference<ReadYourWritesTransaction>::addRef(ryw_))
															#line 5903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this));

	}
	~SimulateTimeoutInFlightCommitActorState() 
	{
		fdb_probe_actor_destroy("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ASSERT(ryw->options.timeoutInSeconds > 0);
															#line 1344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<Void> __when_expr_0 = success(ryw->getReadVersion());
															#line 1344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 5922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			static_cast<SimulateTimeoutInFlightCommitActor*>(this)->actor_wait_state = 1;
															#line 1344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SimulateTimeoutInFlightCommitActor, 0, Void >*>(static_cast<SimulateTimeoutInFlightCommitActor*>(this)));
															#line 5926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		delete static_cast<SimulateTimeoutInFlightCommitActor*>(this);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 1345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!ryw->resetPromise.isSet())
															#line 5948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->resetPromise.sendError(transaction_timed_out());
															#line 5952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(deterministicRandom()->random01() * 5);
															#line 1347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 5958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		static_cast<SimulateTimeoutInFlightCommitActor*>(this)->actor_wait_state = 2;
															#line 1347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SimulateTimeoutInFlightCommitActor, 1, Void >*>(static_cast<SimulateTimeoutInFlightCommitActor*>(this)));
															#line 5962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 1345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!ryw->resetPromise.isSet())
															#line 5971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->resetPromise.sendError(transaction_timed_out());
															#line 5975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		StrictFuture<Void> __when_expr_1 = delay(deterministicRandom()->random01() * 5);
															#line 1347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
															#line 5981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		static_cast<SimulateTimeoutInFlightCommitActor*>(this)->actor_wait_state = 2;
															#line 1347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< SimulateTimeoutInFlightCommitActor, 1, Void >*>(static_cast<SimulateTimeoutInFlightCommitActor*>(this)));
															#line 5985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		if (static_cast<SimulateTimeoutInFlightCommitActor*>(this)->actor_wait_state > 0) static_cast<SimulateTimeoutInFlightCommitActor*>(this)->actor_wait_state = 0;
		static_cast<SimulateTimeoutInFlightCommitActor*>(this)->ActorCallback< SimulateTimeoutInFlightCommitActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SimulateTimeoutInFlightCommitActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SimulateTimeoutInFlightCommitActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SimulateTimeoutInFlightCommitActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 1348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		TraceEvent("ClientBuggifyInFlightCommit").log();
															#line 1349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		StrictFuture<Void> __when_expr_2 = ryw->tr.commit();
															#line 1349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
															#line 6061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		static_cast<SimulateTimeoutInFlightCommitActor*>(this)->actor_wait_state = 3;
															#line 1349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SimulateTimeoutInFlightCommitActor, 2, Void >*>(static_cast<SimulateTimeoutInFlightCommitActor*>(this)));
															#line 6065 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		TraceEvent("ClientBuggifyInFlightCommit").log();
															#line 1349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		StrictFuture<Void> __when_expr_2 = ryw->tr.commit();
															#line 1349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
															#line 6078 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		static_cast<SimulateTimeoutInFlightCommitActor*>(this)->actor_wait_state = 3;
															#line 1349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< SimulateTimeoutInFlightCommitActor, 2, Void >*>(static_cast<SimulateTimeoutInFlightCommitActor*>(this)));
															#line 6082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		if (static_cast<SimulateTimeoutInFlightCommitActor*>(this)->actor_wait_state > 0) static_cast<SimulateTimeoutInFlightCommitActor*>(this)->actor_wait_state = 0;
		static_cast<SimulateTimeoutInFlightCommitActor*>(this)->ActorCallback< SimulateTimeoutInFlightCommitActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SimulateTimeoutInFlightCommitActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< SimulateTimeoutInFlightCommitActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< SimulateTimeoutInFlightCommitActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<SimulateTimeoutInFlightCommitActor*>(this)->actor_wait_state > 0) static_cast<SimulateTimeoutInFlightCommitActor*>(this)->actor_wait_state = 0;
		static_cast<SimulateTimeoutInFlightCommitActor*>(this)->ActorCallback< SimulateTimeoutInFlightCommitActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SimulateTimeoutInFlightCommitActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< SimulateTimeoutInFlightCommitActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< SimulateTimeoutInFlightCommitActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont5(int loopDepth) 
	{
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		delete static_cast<SimulateTimeoutInFlightCommitActor*>(this);
															#line 6229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		return 0;

		return loopDepth;
	}
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw_;
															#line 1338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Reference<ReadYourWritesTransaction> ryw;
															#line 6238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via simulateTimeoutInFlightCommit()
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class SimulateTimeoutInFlightCommitActor final : public Actor<void>, public ActorCallback< SimulateTimeoutInFlightCommitActor, 0, Void >, public ActorCallback< SimulateTimeoutInFlightCommitActor, 1, Void >, public ActorCallback< SimulateTimeoutInFlightCommitActor, 2, Void >, public FastAllocated<SimulateTimeoutInFlightCommitActor>, public SimulateTimeoutInFlightCommitActorState<SimulateTimeoutInFlightCommitActor> {
															#line 6243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<SimulateTimeoutInFlightCommitActor>::operator new;
	using FastAllocated<SimulateTimeoutInFlightCommitActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() {{ ((Actor<void>*)this)->~Actor(); operator delete(this); }}
#pragma clang diagnostic pop
friend struct ActorCallback< SimulateTimeoutInFlightCommitActor, 0, Void >;
friend struct ActorCallback< SimulateTimeoutInFlightCommitActor, 1, Void >;
friend struct ActorCallback< SimulateTimeoutInFlightCommitActor, 2, Void >;
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	SimulateTimeoutInFlightCommitActor(ReadYourWritesTransaction* const& ryw_) 
															#line 6256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<void>(),
		   SimulateTimeoutInFlightCommitActorState<SimulateTimeoutInFlightCommitActor>(ryw_)
	{
		fdb_probe_actor_enter("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("simulateTimeoutInFlightCommit");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("simulateTimeoutInFlightCommit", reinterpret_cast<unsigned long>(this), -1);

	}
};
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
static void simulateTimeoutInFlightCommit( ReadYourWritesTransaction* const& ryw_ ) {
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	new SimulateTimeoutInFlightCommitActor(ryw_);
															#line 6274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 1351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"

																#line 6279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via commit()
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class CommitActor>
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class CommitActorState {
															#line 6285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	CommitActorState(ReadYourWritesTransaction* const& ryw) 
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw(ryw)
															#line 6292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("commit", reinterpret_cast<unsigned long>(this));

	}
	~CommitActorState() 
	{
		fdb_probe_actor_destroy("commit", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				ryw->commitStarted = true;
															#line 1356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (ryw->options.specialKeySpaceChangeConfiguration)
															#line 6310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 1357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					StrictFuture<Void> __when_expr_0 = ryw->getDatabase()->specialKeySpace->commit(ryw);
															#line 1357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 6316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<CommitActor*>(this)->actor_wait_state = 1;
															#line 1357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 0, Void >*>(static_cast<CommitActor*>(this)));
															#line 6321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					loopDepth = 0;
				}
				else
				{
					loopDepth = a_body1cont2(loopDepth);
				}
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
		this->~CommitActorState();
		static_cast<CommitActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!ryw->tr.apiVersionAtLeast(410))
															#line 6356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				ryw->commitStarted = false;
															#line 1416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (!ryw->resetPromise.isSet())
															#line 6362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 1417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					ryw->tr.reset();
															#line 1418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					ryw->resetRyow();
															#line 6368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
			}
															#line 1422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 6373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
															#line 1359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		Future<Void> ready = ryw->reading;
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		StrictFuture<Void> __when_expr_1 = ryw->resetPromise.getFuture() || ready;
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 6391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<CommitActor*>(this)->actor_wait_state = 2;
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 1, Void >*>(static_cast<CommitActor*>(this)));
															#line 6396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CommitActor*>(this)->actor_wait_state > 0) static_cast<CommitActor*>(this)->actor_wait_state = 0;
		static_cast<CommitActor*>(this)->ActorCallback< CommitActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CommitActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CommitActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CommitActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
															#line 1362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (ryw->options.readYourWritesDisabled)
															#line 6480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->nativeReadRanges = ryw->tr.readConflictRanges();
															#line 1366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->nativeWriteRanges = ryw->tr.writeConflictRanges();
															#line 1367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			for( const auto& f : ryw->tr.getExtraReadConflictRanges() ) {
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (f.isReady() && f.get().first < f.get().second)
															#line 6490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 1369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					ryw->nativeReadRanges.push_back( ryw->nativeReadRanges.arena(), KeyRangeRef(f.get().first, f.get().second) .withPrefix(readConflictRangeKeysRange.begin, ryw->nativeReadRanges.arena()));
															#line 6494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
			}
															#line 1375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (ryw->resetPromise.isSet())
															#line 6499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				return a_body1Catch2(ryw->resetPromise.getFuture().getError(), loopDepth);
															#line 6503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
															#line 1377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (CLIENT_BUGGIFY && ryw->options.timeoutInSeconds > 0)
															#line 6507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				simulateTimeoutInFlightCommit(ryw);
															#line 1379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				return a_body1Catch2(transaction_timed_out(), loopDepth);
															#line 6513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
															#line 1381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<Void> __when_expr_2 = ryw->resetPromise.getFuture() || ryw->tr.commit();
															#line 1381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 6519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), loopDepth); else return a_body1cont4when1(__when_expr_2.get(), loopDepth); };
			static_cast<CommitActor*>(this)->actor_wait_state = 3;
															#line 1381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 2, Void >*>(static_cast<CommitActor*>(this)));
															#line 6524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont5(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
															#line 1362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (ryw->options.readYourWritesDisabled)
															#line 6538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->nativeReadRanges = ryw->tr.readConflictRanges();
															#line 1366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->nativeWriteRanges = ryw->tr.writeConflictRanges();
															#line 1367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			for( const auto& f : ryw->tr.getExtraReadConflictRanges() ) {
															#line 1368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (f.isReady() && f.get().first < f.get().second)
															#line 6548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 1369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					ryw->nativeReadRanges.push_back( ryw->nativeReadRanges.arena(), KeyRangeRef(f.get().first, f.get().second) .withPrefix(readConflictRangeKeysRange.begin, ryw->nativeReadRanges.arena()));
															#line 6552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
			}
															#line 1375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (ryw->resetPromise.isSet())
															#line 6557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				return a_body1Catch2(ryw->resetPromise.getFuture().getError(), loopDepth);
															#line 6561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
															#line 1377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (CLIENT_BUGGIFY && ryw->options.timeoutInSeconds > 0)
															#line 6565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				simulateTimeoutInFlightCommit(ryw);
															#line 1379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				return a_body1Catch2(transaction_timed_out(), loopDepth);
															#line 6571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
															#line 1381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<Void> __when_expr_2 = ryw->resetPromise.getFuture() || ryw->tr.commit();
															#line 1381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 6577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), loopDepth); else return a_body1cont4when1(__when_expr_2.get(), loopDepth); };
			static_cast<CommitActor*>(this)->actor_wait_state = 3;
															#line 1381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 2, Void >*>(static_cast<CommitActor*>(this)));
															#line 6582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont5(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CommitActor*>(this)->actor_wait_state > 0) static_cast<CommitActor*>(this)->actor_wait_state = 0;
		static_cast<CommitActor*>(this)->ActorCallback< CommitActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CommitActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CommitActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CommitActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont5(int loopDepth) 
	{
															#line 1392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		ryw->writeRangeToNativeTransaction(KeyRangeRef(StringRef(), allKeys.end));
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		auto conflictRanges = ryw->readConflicts.ranges();
															#line 1395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		for(auto iter = conflictRanges.begin();iter != conflictRanges.end();++iter) {
															#line 1396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (iter->value())
															#line 6665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				ryw->tr.addReadConflictRange(iter->range());
															#line 6669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
		}
															#line 1401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (CLIENT_BUGGIFY && ryw->options.timeoutInSeconds > 0)
															#line 6674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			simulateTimeoutInFlightCommit(ryw);
															#line 1403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			return a_body1Catch2(transaction_timed_out(), loopDepth);
															#line 6680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		StrictFuture<Void> __when_expr_3 = ryw->resetPromise.getFuture() || ryw->tr.commit();
															#line 1405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 6686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), loopDepth); else return a_body1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<CommitActor*>(this)->actor_wait_state = 4;
															#line 1405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 3, Void >*>(static_cast<CommitActor*>(this)));
															#line 6691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
															#line 1383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		ryw->debugLogRetries();
															#line 1385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!ryw->tr.apiVersionAtLeast(410))
															#line 6702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->reset();
															#line 6706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<CommitActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CommitActorState(); static_cast<CommitActor*>(this)->destroy(); return 0; }
															#line 6710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<CommitActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CommitActorState();
		static_cast<CommitActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
															#line 1383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		ryw->debugLogRetries();
															#line 1385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!ryw->tr.apiVersionAtLeast(410))
															#line 6724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->reset();
															#line 6728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<CommitActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CommitActorState(); static_cast<CommitActor*>(this)->destroy(); return 0; }
															#line 6732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<CommitActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CommitActorState();
		static_cast<CommitActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<CommitActor*>(this)->actor_wait_state > 0) static_cast<CommitActor*>(this)->actor_wait_state = 0;
		static_cast<CommitActor*>(this)->ActorCallback< CommitActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CommitActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CommitActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CommitActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont13(Void const& _,int loopDepth) 
	{
															#line 1407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		ryw->debugLogRetries();
															#line 1408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!ryw->tr.apiVersionAtLeast(410))
															#line 6809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->reset();
															#line 6813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<CommitActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CommitActorState(); static_cast<CommitActor*>(this)->destroy(); return 0; }
															#line 6817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<CommitActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CommitActorState();
		static_cast<CommitActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont13(Void && _,int loopDepth) 
	{
															#line 1407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		ryw->debugLogRetries();
															#line 1408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!ryw->tr.apiVersionAtLeast(410))
															#line 6831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			ryw->reset();
															#line 6835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<CommitActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CommitActorState(); static_cast<CommitActor*>(this)->destroy(); return 0; }
															#line 6839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<CommitActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CommitActorState();
		static_cast<CommitActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont13(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont13(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<CommitActor*>(this)->actor_wait_state > 0) static_cast<CommitActor*>(this)->actor_wait_state = 0;
		static_cast<CommitActor*>(this)->ActorCallback< CommitActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CommitActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< CommitActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< CommitActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 6912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via commit()
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class CommitActor final : public Actor<Void>, public ActorCallback< CommitActor, 0, Void >, public ActorCallback< CommitActor, 1, Void >, public ActorCallback< CommitActor, 2, Void >, public ActorCallback< CommitActor, 3, Void >, public FastAllocated<CommitActor>, public CommitActorState<CommitActor> {
															#line 6917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<CommitActor>::operator new;
	using FastAllocated<CommitActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CommitActor, 0, Void >;
friend struct ActorCallback< CommitActor, 1, Void >;
friend struct ActorCallback< CommitActor, 2, Void >;
friend struct ActorCallback< CommitActor, 3, Void >;
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	CommitActor(ReadYourWritesTransaction* const& ryw) 
															#line 6931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<Void>(),
		   CommitActorState<CommitActor>(ryw)
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("commit");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CommitActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CommitActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CommitActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< CommitActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<Void> commit( ReadYourWritesTransaction* const& ryw ) {
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<Void>(new CommitActor(ryw));
															#line 6961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 1425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"

	// This function must not block unless a non-empty commitFuture is passed in
	// If commitFuture is specified, this will wait for the future and report the result of
	// the future in the output. If commitFuture isn't specified, then the transaction will
	// be reported uncommitted. In that case, an optional error can be provided to indicate
	// why the transaction was uncommitted.
																#line 6971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via printDebugMessages()
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class PrintDebugMessagesActor>
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class PrintDebugMessagesActorState {
															#line 6977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	PrintDebugMessagesActorState(ReadYourWritesTransaction* const& self,Optional<Future<Void>> const& commitFuture,Optional<Error> const& error = Optional<Error>()) 
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : self(self),
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   commitFuture(commitFuture),
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   error(error),
															#line 1434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   prefix(),
															#line 1435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   commitResult(),
															#line 1436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   result(),
															#line 1437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   debugTraces(std::move(self->debugTraces)),
															#line 1438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   debugMessages(std::move(self->debugMessages))
															#line 6998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("printDebugMessages", reinterpret_cast<unsigned long>(this));

	}
	~PrintDebugMessagesActorState() 
	{
		fdb_probe_actor_destroy("printDebugMessages", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			self->debugTraces.clear();
															#line 1441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			self->debugMessages.clear();
															#line 1443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (commitFuture.present())
															#line 7017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
				try {
															#line 1445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					StrictFuture<Void> __when_expr_0 = store(result, errorOr(commitFuture.get()));
															#line 1445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					if (static_cast<PrintDebugMessagesActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 7024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<PrintDebugMessagesActor*>(this)->actor_wait_state = 1;
															#line 1445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< PrintDebugMessagesActor, 0, Void >*>(static_cast<PrintDebugMessagesActor*>(this)));
															#line 7029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~PrintDebugMessagesActorState();
		static_cast<PrintDebugMessagesActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		Version readVersion = self->getReadVersion().canGet() ? self->getReadVersion().get() : -1;
															#line 1452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		Version commitVersion = result.present() ? self->getCommittedVersion() : -1;
															#line 1454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (result.present())
															#line 7067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			commitResult = "Committed";
															#line 7071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
		else
		{
															#line 1456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (result.getError().code() == error_code_commit_unknown_result || result.getError().code() == error_code_operation_cancelled || result.getError().code() == error_code_transaction_timed_out)
															#line 7077 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				commitResult = "Maybe committed";
															#line 7081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
			else
			{
															#line 1460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (commitFuture.present())
															#line 7087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 1461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					commitResult = "Not committed";
															#line 7091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
				else
				{
															#line 1463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					commitResult = "Uncommitted";
															#line 7097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
			}
		}
															#line 1466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		for( auto& event : debugTraces ) {
															#line 1467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			event.detail("CommitResult", commitResult).detail("ReadVersion", readVersion);
															#line 1469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (result.present())
															#line 7107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				event.detail("CommitVersion", commitVersion);
															#line 7111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
			else
			{
															#line 1471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (commitFuture.present())
															#line 7117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 1472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					event.errorUnsuppressed(result.getError());
															#line 7121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
				else
				{
															#line 1473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					if (error.present())
															#line 7127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					{
															#line 1474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
						event.errorUnsuppressed(error.get());
															#line 7131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
					}
				}
			}
															#line 1477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			event.log();
															#line 7137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		for( auto message : debugMessages ) {
															#line 1481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			std::string cvString = result.present() ? fmt::format(" cv={}", commitVersion) : "";
															#line 1482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			std::string errorString;
															#line 1483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (commitFuture.present() && result.isError())
															#line 7147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				errorString = fmt::format(" error={}", result.getError().name());
															#line 7151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
			else
			{
															#line 1485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (error.present())
															#line 7157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 1486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					errorString = fmt::format(" error={}", error.get().name());
															#line 7161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
			}
															#line 1489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			fmt::print("[{} rv={}{}{}] {}\n", commitResult, readVersion, cvString, errorString, message);
															#line 7166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (result.isError())
															#line 7170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			return a_body1Catch1(result.getError(), loopDepth);
															#line 7174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1496 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<PrintDebugMessagesActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~PrintDebugMessagesActorState(); static_cast<PrintDebugMessagesActor*>(this)->destroy(); return 0; }
															#line 7178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<PrintDebugMessagesActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~PrintDebugMessagesActorState();
		static_cast<PrintDebugMessagesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			result = e;
															#line 7197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			loopDepth = a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<PrintDebugMessagesActor*>(this)->actor_wait_state > 0) static_cast<PrintDebugMessagesActor*>(this)->actor_wait_state = 0;
		static_cast<PrintDebugMessagesActor*>(this)->ActorCallback< PrintDebugMessagesActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PrintDebugMessagesActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("printDebugMessages", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("printDebugMessages", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< PrintDebugMessagesActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("printDebugMessages", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("printDebugMessages", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< PrintDebugMessagesActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("printDebugMessages", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("printDebugMessages", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(int loopDepth) 
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
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* self;
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Optional<Future<Void>> commitFuture;
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Optional<Error> error;
															#line 1434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	std::string prefix;
															#line 1435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	std::string commitResult;
															#line 1436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ErrorOr<Void> result;
															#line 1437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	std::vector<BaseTraceEvent> debugTraces;
															#line 1438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	std::vector<std::string> debugMessages;
															#line 7312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via printDebugMessages()
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class PrintDebugMessagesActor final : public Actor<Void>, public ActorCallback< PrintDebugMessagesActor, 0, Void >, public FastAllocated<PrintDebugMessagesActor>, public PrintDebugMessagesActorState<PrintDebugMessagesActor> {
															#line 7317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<PrintDebugMessagesActor>::operator new;
	using FastAllocated<PrintDebugMessagesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< PrintDebugMessagesActor, 0, Void >;
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	PrintDebugMessagesActor(ReadYourWritesTransaction* const& self,Optional<Future<Void>> const& commitFuture,Optional<Error> const& error = Optional<Error>()) 
															#line 7328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<Void>(),
		   PrintDebugMessagesActorState<PrintDebugMessagesActor>(self, commitFuture, error)
	{
		fdb_probe_actor_enter("printDebugMessages", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("printDebugMessages");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("printDebugMessages", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< PrintDebugMessagesActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<Void> printDebugMessages( ReadYourWritesTransaction* const& self, Optional<Future<Void>> const& commitFuture, Optional<Error> const& error = Optional<Error>() ) {
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<Void>(new PrintDebugMessagesActor(self, commitFuture, error));
															#line 7355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 1498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"

																#line 7360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via onError()
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class OnErrorActor>
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class OnErrorActorState {
															#line 7366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	OnErrorActorState(ReadYourWritesTransaction* const& ryw,Error const& e) 
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw(ryw),
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   e(e)
															#line 7375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("onError", reinterpret_cast<unsigned long>(this));

	}
	~OnErrorActorState() 
	{
		fdb_probe_actor_destroy("onError", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (ryw->debugTraces.size() > 0 || ryw->debugMessages.size() > 0)
															#line 7390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				ASSERT(printDebugMessages(ryw, {}, e).isReady());
															#line 7394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
			try {
															#line 1506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (ryw->resetPromise.isSet())
															#line 7399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 1507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					return a_body1Catch2(ryw->resetPromise.getFuture().getError(), loopDepth);
															#line 7403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
															#line 1510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				bool retry_limit_hit = ryw->options.maxRetries != -1 && ryw->retries >= ryw->options.maxRetries;
															#line 1511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (ryw->retries < std::numeric_limits<int>::max())
															#line 7409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 1512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					ryw->retries++;
															#line 7413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
															#line 1513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (retry_limit_hit)
															#line 7417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 1514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					return a_body1Catch2(e, loopDepth);
															#line 7421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
															#line 1517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				StrictFuture<Void> __when_expr_0 = ryw->resetPromise.getFuture() || ryw->tr.onError(e);
															#line 1517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (static_cast<OnErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 7427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<OnErrorActor*>(this)->actor_wait_state = 1;
															#line 1517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnErrorActor, 0, Void >*>(static_cast<OnErrorActor*>(this)));
															#line 7432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~OnErrorActorState();
		static_cast<OnErrorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (!ryw->resetPromise.isSet())
															#line 7462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				if (ryw->tr.apiVersionAtLeast(610))
															#line 7466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				{
															#line 1526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					ryw->resetPromise.sendError(transaction_cancelled());
															#line 7470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
				else
				{
															#line 1528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
					ryw->resetRyow();
															#line 7476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
				}
			}
															#line 1531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (e.code() == error_code_broken_promise)
															#line 7481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			{
															#line 1532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
				return a_body1Catch1(transaction_cancelled(), loopDepth);
															#line 7485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			}
															#line 1533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 7489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 1519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		ryw->debugLogRetries(e);
															#line 1521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		ryw->resetRyow();
															#line 1522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<OnErrorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnErrorActorState(); static_cast<OnErrorActor*>(this)->destroy(); return 0; }
															#line 7507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<OnErrorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnErrorActorState();
		static_cast<OnErrorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 1519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		ryw->debugLogRetries(e);
															#line 1521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		ryw->resetRyow();
															#line 1522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<OnErrorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnErrorActorState(); static_cast<OnErrorActor*>(this)->destroy(); return 0; }
															#line 7523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<OnErrorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnErrorActorState();
		static_cast<OnErrorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<OnErrorActor*>(this)->actor_wait_state > 0) static_cast<OnErrorActor*>(this)->actor_wait_state = 0;
		static_cast<OnErrorActor*>(this)->ActorCallback< OnErrorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< OnErrorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< OnErrorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OnErrorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onError", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Error e;
															#line 7598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via onError()
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class OnErrorActor final : public Actor<Void>, public ActorCallback< OnErrorActor, 0, Void >, public FastAllocated<OnErrorActor>, public OnErrorActorState<OnErrorActor> {
															#line 7603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<OnErrorActor>::operator new;
	using FastAllocated<OnErrorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnErrorActor, 0, Void >;
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	OnErrorActor(ReadYourWritesTransaction* const& ryw,Error const& e) 
															#line 7614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<Void>(),
		   OnErrorActorState<OnErrorActor>(ryw, e)
	{
		fdb_probe_actor_enter("onError", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("onError");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("onError", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< OnErrorActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<Void> onError( ReadYourWritesTransaction* const& ryw, Error const& e ) {
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<Void>(new OnErrorActor(ryw, e));
															#line 7641 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 1536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"

																#line 7646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
// This generated class is to be used only via getReadVersion()
															#line 1537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class GetReadVersionActor>
															#line 1537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class GetReadVersionActorState {
															#line 7652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 1537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetReadVersionActorState(ReadYourWritesTransaction* const& ryw) 
															#line 1537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 1537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : ryw(ryw)
															#line 7659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("getReadVersion", reinterpret_cast<unsigned long>(this));

	}
	~GetReadVersionActorState() 
	{
		fdb_probe_actor_destroy("getReadVersion", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<Version> __when_expr_0 = ryw->tr.getReadVersion();
															#line 1538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (static_cast<GetReadVersionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 1543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ryw->resetPromise.getFuture();
															#line 7680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<GetReadVersionActor*>(this)->actor_wait_state = 1;
															#line 1539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetReadVersionActor, 0, Version >*>(static_cast<GetReadVersionActor*>(this)));
															#line 1543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetReadVersionActor, 1, Void >*>(static_cast<GetReadVersionActor*>(this)));
															#line 7687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~GetReadVersionActorState();
		static_cast<GetReadVersionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Version const& v,int loopDepth) 
	{
															#line 1540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<GetReadVersionActor*>(this)->SAV<Version>::futures) { (void)(v); this->~GetReadVersionActorState(); static_cast<GetReadVersionActor*>(this)->destroy(); return 0; }
															#line 7710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<GetReadVersionActor*>(this)->SAV< Version >::value()) Version(v);
		this->~GetReadVersionActorState();
		static_cast<GetReadVersionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Version && v,int loopDepth) 
	{
															#line 1540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<GetReadVersionActor*>(this)->SAV<Version>::futures) { (void)(v); this->~GetReadVersionActorState(); static_cast<GetReadVersionActor*>(this)->destroy(); return 0; }
															#line 7722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<GetReadVersionActor*>(this)->SAV< Version >::value()) Version(v);
		this->~GetReadVersionActorState();
		static_cast<GetReadVersionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 1544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 7734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 1544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		return a_body1Catch1(internal_error(), loopDepth);
															#line 7742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetReadVersionActor*>(this)->actor_wait_state > 0) static_cast<GetReadVersionActor*>(this)->actor_wait_state = 0;
		static_cast<GetReadVersionActor*>(this)->ActorCallback< GetReadVersionActor, 0, Version >::remove();
		static_cast<GetReadVersionActor*>(this)->ActorCallback< GetReadVersionActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetReadVersionActor, 0, Version >*,Version const& value) 
	{
		fdb_probe_actor_enter("getReadVersion", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getReadVersion", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetReadVersionActor, 0, Version >*,Version && value) 
	{
		fdb_probe_actor_enter("getReadVersion", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getReadVersion", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetReadVersionActor, 0, Version >*,Error err) 
	{
		fdb_probe_actor_enter("getReadVersion", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getReadVersion", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetReadVersionActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getReadVersion", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getReadVersion", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetReadVersionActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getReadVersion", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getReadVersion", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetReadVersionActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getReadVersion", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getReadVersion", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	ReadYourWritesTransaction* ryw;
															#line 7845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via getReadVersion()
															#line 1537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class GetReadVersionActor final : public Actor<Version>, public ActorCallback< GetReadVersionActor, 0, Version >, public ActorCallback< GetReadVersionActor, 1, Void >, public FastAllocated<GetReadVersionActor>, public GetReadVersionActorState<GetReadVersionActor> {
															#line 7850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<GetReadVersionActor>::operator new;
	using FastAllocated<GetReadVersionActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Version>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetReadVersionActor, 0, Version >;
friend struct ActorCallback< GetReadVersionActor, 1, Void >;
															#line 1537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetReadVersionActor(ReadYourWritesTransaction* const& ryw) 
															#line 7862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<Version>(),
		   GetReadVersionActorState<GetReadVersionActor>(ryw)
	{
		fdb_probe_actor_enter("getReadVersion", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getReadVersion");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getReadVersion", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetReadVersionActor, 0, Version >*)0, actor_cancelled()); break;
		}

	}
};
															#line 1537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] static Future<Version> getReadVersion( ReadYourWritesTransaction* const& ryw ) {
															#line 1537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<Version>(new GetReadVersionActor(ryw));
															#line 7889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 1548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
};

ReadYourWritesTransaction::ReadYourWritesTransaction(Database const& cx, Optional<Reference<Tenant>> const& tenant)
  : ISingleThreadTransaction(cx->deferredError), tr(cx, tenant), cache(&arena), writes(&arena), retries(0),
    approximateSize(0), creationTime(now()), commitStarted(false), versionStampFuture(tr.getVersionstamp()),
    specialKeySpaceWriteMap(std::make_pair(false, Optional<Value>()), specialKeys.end), options(tr) {
	std::copy(
	    cx.getTransactionDefaults().begin(), cx.getTransactionDefaults().end(), std::back_inserter(persistentOptions));
	applyPersistentOptions();
}

void ReadYourWritesTransaction::construct(Database const& cx) {
	*this = ReadYourWritesTransaction(cx);
}

void ReadYourWritesTransaction::construct(Database const& cx, Reference<Tenant> const& tenant) {
	*this = ReadYourWritesTransaction(cx, tenant);
}

															#line 7912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
namespace {
// This generated class is to be used only via timebomb()
															#line 1567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class TimebombActor>
															#line 1567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class TimebombActorState {
															#line 7919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 1567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	TimebombActorState(double const& endTime,Promise<Void> const& resetPromise) 
															#line 1567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 1567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : endTime(endTime),
															#line 1567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   resetPromise(resetPromise)
															#line 7928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("timebomb", reinterpret_cast<unsigned long>(this));

	}
	~TimebombActorState() 
	{
		fdb_probe_actor_destroy("timebomb", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			;
															#line 7943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~TimebombActorState();
		static_cast<TimebombActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!resetPromise.isSet())
															#line 7966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
															#line 1572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			resetPromise.sendError(transaction_timed_out());
															#line 7970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		return a_body1Catch1(transaction_timed_out(), loopDepth);
															#line 7974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"

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
															#line 1568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!(now() < endTime))
															#line 7989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delayUntil(std::min(endTime + 0.0001, now() + CLIENT_KNOBS->TRANSACTION_TIMEOUT_DELAY_INTERVAL));
															#line 1569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (static_cast<TimebombActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 7997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<TimebombActor*>(this)->actor_wait_state = 1;
															#line 1569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TimebombActor, 0, Void >*>(static_cast<TimebombActor*>(this)));
															#line 8002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
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
		if (static_cast<TimebombActor*>(this)->actor_wait_state > 0) static_cast<TimebombActor*>(this)->actor_wait_state = 0;
		static_cast<TimebombActor*>(this)->ActorCallback< TimebombActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TimebombActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("timebomb", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timebomb", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TimebombActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("timebomb", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timebomb", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TimebombActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("timebomb", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timebomb", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	double endTime;
															#line 1567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Promise<Void> resetPromise;
															#line 8099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via timebomb()
															#line 1567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class TimebombActor final : public Actor<Void>, public ActorCallback< TimebombActor, 0, Void >, public FastAllocated<TimebombActor>, public TimebombActorState<TimebombActor> {
															#line 8104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<TimebombActor>::operator new;
	using FastAllocated<TimebombActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TimebombActor, 0, Void >;
															#line 1567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	TimebombActor(double const& endTime,Promise<Void> const& resetPromise) 
															#line 8115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<Void>(),
		   TimebombActorState<TimebombActor>(endTime, resetPromise)
	{
		fdb_probe_actor_enter("timebomb", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("timebomb");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("timebomb", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TimebombActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] Future<Void> timebomb( double const& endTime, Promise<Void> const& resetPromise ) {
															#line 1567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<Void>(new TimebombActor(endTime, resetPromise));
															#line 8143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 1575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"

void ReadYourWritesTransaction::resetTimeout() {
	timeoutActor =
	    options.timeoutInSeconds == 0.0 ? Void() : timebomb(options.timeoutInSeconds + creationTime, resetPromise);
}

Future<Version> ReadYourWritesTransaction::getReadVersion() {
	if (tr.apiVersionAtLeast(101)) {
		if (resetPromise.isSet())
			return resetPromise.getFuture().getError();
		return RYWImpl::getReadVersion(this);
	}
	return tr.getReadVersion();
}

Optional<Value> getValueFromJSON(StatusObject statusObj) {
	try {
		Value output =
		    StringRef(json_spirit::write_string(json_spirit::mValue(statusObj), json_spirit::Output_options::none));
		return output;
	} catch (std::exception& e) {
		TraceEvent(SevError, "UnableToUnparseStatusJSON").detail("What", e.what());
		throw internal_error();
	}
}

															#line 8173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
namespace {
// This generated class is to be used only via getJSON()
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class GetJSONActor>
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class GetJSONActorState {
															#line 8180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetJSONActorState(Database const& db) 
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : db(db)
															#line 8187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("getJSON", reinterpret_cast<unsigned long>(this));

	}
	~GetJSONActorState() 
	{
		fdb_probe_actor_destroy("getJSON", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			StrictFuture<StatusObject> __when_expr_0 = StatusClient::statusFetcher(db);
															#line 1602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			if (static_cast<GetJSONActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetJSONActor*>(this)->actor_wait_state = 1;
															#line 1602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetJSONActor, 0, StatusObject >*>(static_cast<GetJSONActor*>(this)));
															#line 8209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~GetJSONActorState();
		static_cast<GetJSONActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(StatusObject const& statusObj,int loopDepth) 
	{
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<GetJSONActor*>(this)->SAV<Optional<Value>>::futures) { (void)(getValueFromJSON(statusObj)); this->~GetJSONActorState(); static_cast<GetJSONActor*>(this)->destroy(); return 0; }
															#line 8232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<GetJSONActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(getValueFromJSON(statusObj));
		this->~GetJSONActorState();
		static_cast<GetJSONActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(StatusObject && statusObj,int loopDepth) 
	{
															#line 1603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<GetJSONActor*>(this)->SAV<Optional<Value>>::futures) { (void)(getValueFromJSON(statusObj)); this->~GetJSONActorState(); static_cast<GetJSONActor*>(this)->destroy(); return 0; }
															#line 8244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<GetJSONActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(getValueFromJSON(statusObj));
		this->~GetJSONActorState();
		static_cast<GetJSONActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(StatusObject const& statusObj,int loopDepth) 
	{
		loopDepth = a_body1cont1(statusObj, loopDepth);

		return loopDepth;
	}
	int a_body1when1(StatusObject && statusObj,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(statusObj), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetJSONActor*>(this)->actor_wait_state > 0) static_cast<GetJSONActor*>(this)->actor_wait_state = 0;
		static_cast<GetJSONActor*>(this)->ActorCallback< GetJSONActor, 0, StatusObject >::remove();

	}
	void a_callback_fire(ActorCallback< GetJSONActor, 0, StatusObject >*,StatusObject const& value) 
	{
		fdb_probe_actor_enter("getJSON", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getJSON", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetJSONActor, 0, StatusObject >*,StatusObject && value) 
	{
		fdb_probe_actor_enter("getJSON", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getJSON", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetJSONActor, 0, StatusObject >*,Error err) 
	{
		fdb_probe_actor_enter("getJSON", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getJSON", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Database db;
															#line 8317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via getJSON()
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class GetJSONActor final : public Actor<Optional<Value>>, public ActorCallback< GetJSONActor, 0, StatusObject >, public FastAllocated<GetJSONActor>, public GetJSONActorState<GetJSONActor> {
															#line 8322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<GetJSONActor>::operator new;
	using FastAllocated<GetJSONActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<Value>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetJSONActor, 0, StatusObject >;
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetJSONActor(Database const& db) 
															#line 8333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<Optional<Value>>(),
		   GetJSONActorState<GetJSONActor>(db)
	{
		fdb_probe_actor_enter("getJSON", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getJSON");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getJSON", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetJSONActor, 0, StatusObject >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] Future<Optional<Value>> getJSON( Database const& db ) {
															#line 1601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<Optional<Value>>(new GetJSONActor(db));
															#line 8361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 1605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"

															#line 8366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
namespace {
// This generated class is to be used only via getWorkerInterfaces()
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
template <class GetWorkerInterfacesActor>
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class GetWorkerInterfacesActorState {
															#line 8373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetWorkerInterfacesActorState(Reference<IClusterConnectionRecord> const& connRecord) 
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		 : connRecord(connRecord),
															#line 1607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   clusterInterface(new AsyncVar<Optional<ClusterInterface>>),
															#line 1608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		   leaderMon(monitorLeader<ClusterInterface>(connRecord, clusterInterface))
															#line 8384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
	{
		fdb_probe_actor_create("getWorkerInterfaces", reinterpret_cast<unsigned long>(this));

	}
	~GetWorkerInterfacesActorState() 
	{
		fdb_probe_actor_destroy("getWorkerInterfaces", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			;
															#line 8399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
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
		this->~GetWorkerInterfacesActorState();
		static_cast<GetWorkerInterfacesActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		StrictFuture<std::vector<ClientWorkerInterface>> __when_expr_0 = clusterInterface->get().present() ? brokenPromiseToNever( clusterInterface->get().get().getClientWorkers.getReply(GetClientWorkersRequest())) : Never();
															#line 1611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (static_cast<GetWorkerInterfacesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 8431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 1626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		StrictFuture<Void> __when_expr_1 = clusterInterface->onChange();
															#line 8435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<GetWorkerInterfacesActor*>(this)->actor_wait_state = 1;
															#line 1612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetWorkerInterfacesActor, 0, std::vector<ClientWorkerInterface> >*>(static_cast<GetWorkerInterfacesActor*>(this)));
															#line 1626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetWorkerInterfacesActor, 1, Void >*>(static_cast<GetWorkerInterfacesActor*>(this)));
															#line 8442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<ClientWorkerInterface> const& workers,int loopDepth) 
	{
															#line 1617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		RangeResult result;
															#line 1618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		for( auto& it : workers ) {
															#line 1619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			result.push_back_deep( result.arena(), KeyValueRef(it.address().toString(), BinaryWriter::toValue(it, IncludeVersion())));
															#line 8461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<GetWorkerInterfacesActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetWorkerInterfacesActorState(); static_cast<GetWorkerInterfacesActor*>(this)->destroy(); return 0; }
															#line 8465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<GetWorkerInterfacesActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetWorkerInterfacesActorState();
		static_cast<GetWorkerInterfacesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<ClientWorkerInterface> && workers,int loopDepth) 
	{
															#line 1617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		RangeResult result;
															#line 1618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		for( auto& it : workers ) {
															#line 1619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
			result.push_back_deep( result.arena(), KeyValueRef(it.address().toString(), BinaryWriter::toValue(it, IncludeVersion())));
															#line 8481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		}
															#line 1624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
		if (!static_cast<GetWorkerInterfacesActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetWorkerInterfacesActorState(); static_cast<GetWorkerInterfacesActor*>(this)->destroy(); return 0; }
															#line 8485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		new (&static_cast<GetWorkerInterfacesActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetWorkerInterfacesActorState();
		static_cast<GetWorkerInterfacesActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<GetWorkerInterfacesActor*>(this)->actor_wait_state > 0) static_cast<GetWorkerInterfacesActor*>(this)->actor_wait_state = 0;
		static_cast<GetWorkerInterfacesActor*>(this)->ActorCallback< GetWorkerInterfacesActor, 0, std::vector<ClientWorkerInterface> >::remove();
		static_cast<GetWorkerInterfacesActor*>(this)->ActorCallback< GetWorkerInterfacesActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetWorkerInterfacesActor, 0, std::vector<ClientWorkerInterface> >*,std::vector<ClientWorkerInterface> const& value) 
	{
		fdb_probe_actor_enter("getWorkerInterfaces", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkerInterfaces", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetWorkerInterfacesActor, 0, std::vector<ClientWorkerInterface> >*,std::vector<ClientWorkerInterface> && value) 
	{
		fdb_probe_actor_enter("getWorkerInterfaces", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkerInterfaces", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetWorkerInterfacesActor, 0, std::vector<ClientWorkerInterface> >*,Error err) 
	{
		fdb_probe_actor_enter("getWorkerInterfaces", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkerInterfaces", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetWorkerInterfacesActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getWorkerInterfaces", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkerInterfaces", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetWorkerInterfacesActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getWorkerInterfaces", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkerInterfaces", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetWorkerInterfacesActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getWorkerInterfaces", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkerInterfaces", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Reference<IClusterConnectionRecord> connRecord;
															#line 1607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Reference<AsyncVar<Optional<ClusterInterface>>> clusterInterface;
															#line 1608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	Future<Void> leaderMon;
															#line 8608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
};
// This generated class is to be used only via getWorkerInterfaces()
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
class GetWorkerInterfacesActor final : public Actor<RangeResult>, public ActorCallback< GetWorkerInterfacesActor, 0, std::vector<ClientWorkerInterface> >, public ActorCallback< GetWorkerInterfacesActor, 1, Void >, public FastAllocated<GetWorkerInterfacesActor>, public GetWorkerInterfacesActorState<GetWorkerInterfacesActor> {
															#line 8613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
public:
	using FastAllocated<GetWorkerInterfacesActor>::operator new;
	using FastAllocated<GetWorkerInterfacesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetWorkerInterfacesActor, 0, std::vector<ClientWorkerInterface> >;
friend struct ActorCallback< GetWorkerInterfacesActor, 1, Void >;
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	GetWorkerInterfacesActor(Reference<IClusterConnectionRecord> const& connRecord) 
															#line 8625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
		 : Actor<RangeResult>(),
		   GetWorkerInterfacesActorState<GetWorkerInterfacesActor>(connRecord)
	{
		fdb_probe_actor_enter("getWorkerInterfaces", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getWorkerInterfaces");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getWorkerInterfaces", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetWorkerInterfacesActor, 0, std::vector<ClientWorkerInterface> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
[[nodiscard]] Future<RangeResult> getWorkerInterfaces( Reference<IClusterConnectionRecord> const& connRecord ) {
															#line 1606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"
	return Future<RangeResult>(new GetWorkerInterfacesActor(connRecord));
															#line 8653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.g.cpp"
}

#line 1630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/ReadYourWrites.actor.cpp"

Future<Optional<Value>> ReadYourWritesTransaction::get(const Key& key, Snapshot snapshot) {
	CODE_PROBE(true, "ReadYourWritesTransaction::get");

	if (getDatabase()->apiVersionAtLeast(630)) {
		if (specialKeys.contains(key)) {
			CODE_PROBE(true, "Special keys get");
			return getDatabase()->specialKeySpace->get(this, key);
		}
	} else {
		if (key == "\xff\xff/status/json"_sr) {
			if (tr.getDatabase().getPtr() && tr.getDatabase()->getConnectionRecord()) {
				++tr.getDatabase()->transactionStatusRequests;
				return getJSON(tr.getDatabase());
			} else {
				return Optional<Value>();
			}
		}

		if (key == "\xff\xff/cluster_file_path"_sr) {
			try {
				if (tr.getDatabase().getPtr() && tr.getDatabase()->getConnectionRecord()) {
					Optional<Value> output = StringRef(tr.getDatabase()->getConnectionRecord()->getLocation());
					return output;
				}
			} catch (Error& e) {
				return e;
			}
			return Optional<Value>();
		}

		if (key == "\xff\xff/connection_string"_sr) {
			try {
				if (tr.getDatabase().getPtr() && tr.getDatabase()->getConnectionRecord()) {
					Reference<IClusterConnectionRecord> f = tr.getDatabase()->getConnectionRecord();
					Optional<Value> output = StringRef(f->getConnectionString().toString());
					return output;
				}
			} catch (Error& e) {
				return e;
			}
			return Optional<Value>();
		}
	}

	if (checkUsedDuringCommit()) {
		return used_during_commit();
	}

	if (resetPromise.isSet())
		return resetPromise.getFuture().getError();

	if (key >= getMaxReadKey() && key != metadataVersionKey)
		return key_outside_legal_range();

	// There are no keys in the database with size greater than the max key size
	if (key.size() > getMaxReadKeySize(key)) {
		return Optional<Value>();
	}

	Future<Optional<Value>> result = RYWImpl::readWithConflictRange(this, RYWImpl::GetValueReq(key), snapshot);
	reading.add(success(result));
	return result;
}

Future<Key> ReadYourWritesTransaction::getKey(const KeySelector& key, Snapshot snapshot) {
	if (checkUsedDuringCommit()) {
		return used_during_commit();
	}

	if (resetPromise.isSet())
		return resetPromise.getFuture().getError();

	if (key.getKey() > getMaxReadKey())
		return key_outside_legal_range();

	Future<Key> result = RYWImpl::readWithConflictRange(this, RYWImpl::GetKeyReq(key), snapshot);
	reading.add(success(result));
	return result;
}

Future<RangeResult> ReadYourWritesTransaction::getRange(KeySelector begin,
                                                        KeySelector end,
                                                        GetRangeLimits limits,
                                                        Snapshot snapshot,
                                                        Reverse reverse) {
	if (getDatabase()->apiVersionAtLeast(630)) {
		if (specialKeys.contains(begin.getKey()) && specialKeys.begin <= end.getKey() &&
		    end.getKey() <= specialKeys.end) {
			CODE_PROBE(true, "Special key space get range");
			return getDatabase()->specialKeySpace->getRange(this, begin, end, limits, reverse);
		}
	} else {
		if (begin.getKey() == "\xff\xff/worker_interfaces"_sr) {
			if (tr.getDatabase().getPtr() && tr.getDatabase()->getConnectionRecord()) {
				return getWorkerInterfaces(tr.getDatabase()->getConnectionRecord());
			} else {
				return RangeResult();
			}
		}
	}

	if (checkUsedDuringCommit()) {
		return used_during_commit();
	}

	if (resetPromise.isSet())
		return resetPromise.getFuture().getError();

	KeyRef maxKey = getMaxReadKey();
	if (begin.getKey() > maxKey || end.getKey() > maxKey)
		return key_outside_legal_range();

	// This optimization prevents nullptr operations from being added to the conflict range
	if (limits.isReached()) {
		CODE_PROBE(true, "RYW range read limit 0");
		return RangeResult();
	}

	if (!limits.isValid())
		return range_limits_invalid();

	if (begin.orEqual)
		begin.removeOrEqual(begin.arena());

	if (end.orEqual)
		end.removeOrEqual(end.arena());

	if (begin.offset >= end.offset && begin.getKey() >= end.getKey()) {
		CODE_PROBE(true, "RYW range inverted");
		return RangeResult();
	}

	Future<RangeResult> result =
	    reverse ? RYWImpl::readWithConflictRange(this, RYWImpl::GetRangeReq<true>(begin, end, limits), snapshot)
	            : RYWImpl::readWithConflictRange(this, RYWImpl::GetRangeReq<false>(begin, end, limits), snapshot);

	reading.add(success(result));
	return result;
}

Future<RangeResult> ReadYourWritesTransaction::getRange(const KeySelector& begin,
                                                        const KeySelector& end,
                                                        int limit,
                                                        Snapshot snapshot,
                                                        Reverse reverse) {
	return getRange(begin, end, GetRangeLimits(limit), snapshot, reverse);
}

Future<MappedRangeResult> ReadYourWritesTransaction::getMappedRange(KeySelector begin,
                                                                    KeySelector end,
                                                                    Key mapper,
                                                                    GetRangeLimits limits,
                                                                    Snapshot snapshot,
                                                                    Reverse reverse) {
	if (getDatabase()->apiVersionAtLeast(630)) {
		if (specialKeys.contains(begin.getKey()) && specialKeys.begin <= end.getKey() &&
		    end.getKey() <= specialKeys.end) {
			CODE_PROBE(true, "Special key space get range (getMappedRange)", probe::decoration::rare);
			throw client_invalid_operation(); // Not support special keys.
		}
	} else {
		if (begin.getKey() == "\xff\xff/worker_interfaces"_sr) {
			throw client_invalid_operation(); // Not support special keys.
		}
	}

	if (checkUsedDuringCommit()) {
		return used_during_commit();
	}

	if (resetPromise.isSet())
		return resetPromise.getFuture().getError();

	KeyRef maxKey = getMaxReadKey();
	if (begin.getKey() > maxKey || end.getKey() > maxKey)
		return key_outside_legal_range();

	// This optimization prevents nullptr operations from being added to the conflict range
	if (limits.isReached()) {
		CODE_PROBE(true, "RYW range read limit 0 (getMappedRange)", probe::decoration::rare);
		return MappedRangeResult();
	}

	if (!limits.isValid())
		return range_limits_invalid();

	if (begin.orEqual)
		begin.removeOrEqual(begin.arena());

	if (end.orEqual)
		end.removeOrEqual(end.arena());

	if (begin.offset >= end.offset && begin.getKey() >= end.getKey()) {
		CODE_PROBE(true, "RYW range inverted (getMappedRange)", probe::decoration::rare);
		return MappedRangeResult();
	}

	Future<MappedRangeResult> result =
	    reverse ? RYWImpl::readWithConflictRangeForGetMappedRange(
	                  this, RYWImpl::GetMappedRangeReq<true>(begin, end, mapper, limits), snapshot)
	            : RYWImpl::readWithConflictRangeForGetMappedRange(
	                  this, RYWImpl::GetMappedRangeReq<false>(begin, end, mapper, limits), snapshot);

	return result;
}

Future<Standalone<VectorRef<const char*>>> ReadYourWritesTransaction::getAddressesForKey(const Key& key) {
	if (checkUsedDuringCommit()) {
		return used_during_commit();
	}

	if (resetPromise.isSet())
		return resetPromise.getFuture().getError();

	// If key >= allKeys.end, then our resulting address vector will be empty.

	Future<Standalone<VectorRef<const char*>>> result =
	    waitOrError(tr.getAddressesForKey(key), resetPromise.getFuture());
	reading.add(success(result));
	return result;
}

Future<int64_t> ReadYourWritesTransaction::getEstimatedRangeSizeBytes(const KeyRange& keys) {
	if (checkUsedDuringCommit()) {
		throw used_during_commit();
	}
	if (resetPromise.isSet())
		return resetPromise.getFuture().getError();

	// Pass in the TransactionState only if tenant is present
	Optional<Reference<TransactionState>> trState =
	    tr.trState->hasTenant() ? tr.trState : Optional<Reference<TransactionState>>();
	return map(waitOrError(tr.getDatabase()->getStorageMetrics(keys, -1, trState), resetPromise.getFuture()),
	           [](const StorageMetrics& m) { return m.bytes; });
}

Future<Standalone<VectorRef<KeyRef>>> ReadYourWritesTransaction::getRangeSplitPoints(const KeyRange& range,
                                                                                     int64_t chunkSize) {
	if (checkUsedDuringCommit()) {
		return used_during_commit();
	}
	if (resetPromise.isSet())
		return resetPromise.getFuture().getError();

	KeyRef maxKey = getMaxReadKey();
	if (range.begin > maxKey || range.end > maxKey)
		return key_outside_legal_range();

	return waitOrError(tr.getRangeSplitPoints(range, chunkSize), resetPromise.getFuture());
}

Future<Standalone<VectorRef<KeyRangeRef>>> ReadYourWritesTransaction::getBlobGranuleRanges(const KeyRange& range,
                                                                                           int rangeLimit) {
	if (checkUsedDuringCommit()) {
		return used_during_commit();
	}
	if (resetPromise.isSet())
		return resetPromise.getFuture().getError();

	KeyRef maxKey = getMaxReadKey();
	if (range.begin > maxKey || range.end > maxKey)
		return key_outside_legal_range();

	return waitOrError(tr.getBlobGranuleRanges(range, rangeLimit), resetPromise.getFuture());
}

Future<Standalone<VectorRef<BlobGranuleChunkRef>>> ReadYourWritesTransaction::readBlobGranules(
    const KeyRange& range,
    Version begin,
    Optional<Version> readVersion,
    Version* readVersionOut) {
	if (!options.readYourWritesDisabled) {
		return blob_granule_no_ryw();
	}

	if (checkUsedDuringCommit()) {
		return used_during_commit();
	}

	if (resetPromise.isSet())
		return resetPromise.getFuture().getError();

	KeyRef maxKey = getMaxReadKey();
	if (range.begin > maxKey || range.end > maxKey)
		return key_outside_legal_range();

	return waitOrError(tr.readBlobGranules(range, begin, readVersion, readVersionOut), resetPromise.getFuture());
}

Future<Standalone<VectorRef<BlobGranuleSummaryRef>>> ReadYourWritesTransaction::summarizeBlobGranules(
    const KeyRange& range,
    Optional<Version> summaryVersion,
    int rangeLimit) {
	if (checkUsedDuringCommit()) {
		return used_during_commit();
	}

	if (resetPromise.isSet())
		return resetPromise.getFuture().getError();

	KeyRef maxKey = getMaxReadKey();
	if (range.begin > maxKey || range.end > maxKey)
		return key_outside_legal_range();

	return waitOrError(tr.summarizeBlobGranules(range, summaryVersion, rangeLimit), resetPromise.getFuture());
}

void ReadYourWritesTransaction::addGranuleMaterializeStats(const GranuleMaterializeStats& stats) {
	if (checkUsedDuringCommit()) {
		throw used_during_commit();
	}
	tr.addGranuleMaterializeStats(stats);
}

void ReadYourWritesTransaction::addReadConflictRange(KeyRangeRef const& keys) {
	if (checkUsedDuringCommit()) {
		throw used_during_commit();
	}

	if (tr.apiVersionAtLeast(300)) {
		if ((keys.begin > getMaxReadKey() || keys.end > getMaxReadKey()) &&
		    (keys.begin != metadataVersionKey || keys.end != metadataVersionKeyEnd)) {
			throw key_outside_legal_range();
		}
	}

	// There aren't any keys in the database with size larger than max key size, so if range contains large keys
	// we can translate it to an equivalent one with smaller keys
	KeyRef begin = keys.begin;
	KeyRef end = keys.end;

	int64_t beginMaxSize = getMaxReadKeySize(begin);
	int64_t endMaxSize = getMaxReadKeySize(end);
	if (begin.size() > beginMaxSize) {
		begin = begin.substr(0, beginMaxSize + 1);
	}
	if (end.size() > endMaxSize) {
		end = end.substr(0, endMaxSize + 1);
	}

	KeyRangeRef r = KeyRangeRef(begin, end);

	if (r.empty()) {
		return;
	}

	if (options.readYourWritesDisabled) {
		approximateSize += r.expectedSize() + sizeof(KeyRangeRef);
		tr.addReadConflictRange(r);
		return;
	}

	WriteMap::iterator it(&writes);
	KeyRangeRef readRange(arena, r);
	it.skip(readRange.begin);
	updateConflictMap(readRange, it);
}

void ReadYourWritesTransaction::updateConflictMap(KeyRef const& key, WriteMap::iterator& it) {
	RYWImpl::updateConflictMap(this, key, it);
}

void ReadYourWritesTransaction::updateConflictMap(KeyRangeRef const& keys, WriteMap::iterator& it) {
	RYWImpl::updateConflictMap(this, keys, it);
}

void ReadYourWritesTransaction::writeRangeToNativeTransaction(KeyRangeRef const& keys) {
	WriteMap::iterator it(&writes);
	it.skip(keys.begin);

	bool inClearRange = false;
	ExtStringRef clearBegin;

	// Clear ranges must be done first because of keys that are both cleared and set to a new value
	for (; it.beginKey() < keys.end; ++it) {
		if (it.is_cleared_range() && !inClearRange) {
			clearBegin = std::max(ExtStringRef(keys.begin), it.beginKey());
			inClearRange = true;
		} else if (!it.is_cleared_range() && inClearRange) {
			tr.clear(KeyRangeRef(clearBegin.toArenaOrRef(arena), it.beginKey().toArenaOrRef(arena)),
			         AddConflictRange::False);
			inClearRange = false;
		}
	}

	if (inClearRange) {
		tr.clear(KeyRangeRef(clearBegin.toArenaOrRef(arena), keys.end), AddConflictRange::False);
	}

	it.skip(keys.begin);

	bool inConflictRange = false;
	ExtStringRef conflictBegin;

	for (; it.beginKey() < keys.end; ++it) {
		if (it.is_conflict_range() && !inConflictRange) {
			conflictBegin = std::max(ExtStringRef(keys.begin), it.beginKey());
			inConflictRange = true;
		} else if (!it.is_conflict_range() && inConflictRange) {
			tr.addWriteConflictRange(KeyRangeRef(conflictBegin.toArenaOrRef(arena), it.beginKey().toArenaOrRef(arena)));
			inConflictRange = false;
		}

		// SOMEDAY: make atomicOp take set to avoid switch
		if (it.is_operation()) {
			auto op = it.op();
			for (int i = 0; i < op.size(); ++i) {
				switch (op[i].type) {
				case MutationRef::SetValue:
					if (op[i].value.present()) {
						tr.set(it.beginKey().assertRef(), op[i].value.get(), AddConflictRange::False);
					} else {
						tr.clear(it.beginKey().assertRef(), AddConflictRange::False);
					}
					break;
				case MutationRef::AddValue:
				case MutationRef::AppendIfFits:
				case MutationRef::And:
				case MutationRef::Or:
				case MutationRef::Xor:
				case MutationRef::Max:
				case MutationRef::Min:
				case MutationRef::SetVersionstampedKey:
				case MutationRef::SetVersionstampedValue:
				case MutationRef::ByteMin:
				case MutationRef::ByteMax:
				case MutationRef::MinV2:
				case MutationRef::AndV2:
				case MutationRef::CompareAndClear:
					tr.atomicOp(it.beginKey().assertRef(), op[i].value.get(), op[i].type, AddConflictRange::False);
					break;
				default:
					break;
				}
			}
		}
	}

	if (inConflictRange) {
		tr.addWriteConflictRange(KeyRangeRef(conflictBegin.toArenaOrRef(arena), keys.end));
	}
}

ReadYourWritesTransactionOptions::ReadYourWritesTransactionOptions(Transaction const& tr) {
	reset(tr);
}

void ReadYourWritesTransactionOptions::reset(Transaction const& tr) {
	memset(this, 0, sizeof(*this));
	timeoutInSeconds = 0.0;
	maxRetries = -1;
	snapshotRywEnabled = tr.getDatabase()->snapshotRywEnabled;
}

bool ReadYourWritesTransactionOptions::getAndResetWriteConflictDisabled() {
	bool disabled = nextWriteDisableConflictRange;
	nextWriteDisableConflictRange = false;
	return disabled;
}

void ReadYourWritesTransaction::getWriteConflicts(KeyRangeMap<bool>* result) {
	WriteMap::iterator it(&writes);
	it.skip(allKeys.begin);

	bool inConflictRange = false;
	ExtStringRef conflictBegin;

	for (; it.beginKey() < getMaxWriteKey(); ++it) {
		if (it.is_conflict_range() && !inConflictRange) {
			conflictBegin = it.beginKey();
			inConflictRange = true;
		} else if (!it.is_conflict_range() && inConflictRange) {
			result->insert(KeyRangeRef(conflictBegin.toArenaOrRef(arena), it.beginKey().toArenaOrRef(arena)), true);
			inConflictRange = false;
		}
	}

	if (inConflictRange) {
		result->insert(KeyRangeRef(conflictBegin.toArenaOrRef(arena), getMaxWriteKey()), true);
	}
}

void ReadYourWritesTransaction::setTransactionID(UID id) {
	tr.setTransactionID(id);
}

void ReadYourWritesTransaction::setToken(uint64_t token) {
	tr.setToken(token);
}

RangeResult ReadYourWritesTransaction::getReadConflictRangeIntersecting(KeyRangeRef kr) {
	CODE_PROBE(true, "Special keys read conflict range");
	ASSERT(readConflictRangeKeysRange.contains(kr));
	ASSERT(!tr.trState->options.checkWritesEnabled);
	RangeResult result;
	if (!options.readYourWritesDisabled) {
		kr = kr.removePrefix(readConflictRangeKeysRange.begin);
		auto iter = readConflicts.rangeContainingKeyBefore(kr.begin);
		if (iter->begin() == allKeys.begin && !iter->value()) {
			++iter; // Conventionally '' is missing from the result range if it's not part of a read conflict
		}
		for (; iter->begin() < kr.end; ++iter) {
			if (kr.begin <= iter->begin() && iter->begin() < kr.end) {
				result.push_back(result.arena(),
				                 KeyValueRef(iter->begin().withPrefix(readConflictRangeKeysRange.begin, result.arena()),
				                             iter->value() ? "1"_sr : "0"_sr));
			}
		}
	} else {
		CoalescedKeyRefRangeMap<ValueRef> readConflicts{ "0"_sr, specialKeys.end };
		for (const auto& range : tr.readConflictRanges())
			readConflicts.insert(range.withPrefix(readConflictRangeKeysRange.begin, result.arena()), "1"_sr);
		for (const auto& range : nativeReadRanges)
			readConflicts.insert(range.withPrefix(readConflictRangeKeysRange.begin, result.arena()), "1"_sr);
		for (const auto& f : tr.getExtraReadConflictRanges()) {
			if (f.isReady() && f.get().first < f.get().second)
				readConflicts.insert(KeyRangeRef(f.get().first, f.get().second)
				                         .withPrefix(readConflictRangeKeysRange.begin, result.arena()),
				                     "1"_sr);
		}
		auto beginIter = readConflicts.rangeContaining(kr.begin);
		if (beginIter->begin() != kr.begin)
			++beginIter;
		for (auto it = beginIter; it->begin() < kr.end; ++it) {
			result.push_back(result.arena(), KeyValueRef(it->begin(), it->value()));
		}
	}
	return result;
}

RangeResult ReadYourWritesTransaction::getWriteConflictRangeIntersecting(KeyRangeRef kr) {
	CODE_PROBE(true, "Special keys write conflict range");
	ASSERT(writeConflictRangeKeysRange.contains(kr));
	RangeResult result;

	// Memory owned by result
	CoalescedKeyRefRangeMap<ValueRef> writeConflicts{ "0"_sr, specialKeys.end };

	if (!options.readYourWritesDisabled) {
		KeyRangeRef strippedWriteRangePrefix = kr.removePrefix(writeConflictRangeKeysRange.begin);
		WriteMap::iterator it(&writes);
		it.skip(strippedWriteRangePrefix.begin);
		if (it.beginKey() > allKeys.begin)
			--it;
		for (; it.beginKey() < strippedWriteRangePrefix.end; ++it) {
			if (it.is_conflict_range())
				writeConflicts.insert(
				    KeyRangeRef(it.beginKey().toArena(result.arena()), it.endKey().toArena(result.arena()))
				        .withPrefix(writeConflictRangeKeysRange.begin, result.arena()),
				    "1"_sr);
		}
	} else {
		for (const auto& range : tr.writeConflictRanges())
			writeConflicts.insert(range.withPrefix(writeConflictRangeKeysRange.begin, result.arena()), "1"_sr);
		for (const auto& range : nativeWriteRanges)
			writeConflicts.insert(range.withPrefix(writeConflictRangeKeysRange.begin, result.arena()), "1"_sr);
	}

	for (const auto& k : versionStampKeys) {
		KeyRange range;
		if (versionStampFuture.isValid() && versionStampFuture.isReady() && !versionStampFuture.isError()) {
			const auto& stamp = versionStampFuture.get();
			StringRef key(range.arena(), k); // Copy
			ASSERT(k.size() >= 4);
			int32_t pos;
			memcpy(&pos, k.end() - sizeof(int32_t), sizeof(int32_t));
			pos = littleEndian32(pos);
			ASSERT(pos >= 0 && pos + stamp.size() <= key.size());
			memcpy(mutateString(key) + pos, stamp.begin(), stamp.size());
			*(mutateString(key) + key.size() - 4) = '\x00';
			// singleKeyRange, but share begin and end's memory
			range = KeyRangeRef(key.substr(0, key.size() - 4), key.substr(0, key.size() - 3));
		} else {
			range = getVersionstampKeyRange(result.arena(), k, tr.getCachedReadVersion().orDefault(0), getMaxReadKey());
		}
		writeConflicts.insert(range.withPrefix(writeConflictRangeKeysRange.begin, result.arena()), "1"_sr);
	}

	auto beginIter = writeConflicts.rangeContaining(kr.begin);
	if (beginIter->begin() != kr.begin)
		++beginIter;
	for (auto it = beginIter; it->begin() < kr.end; ++it) {
		result.push_back(result.arena(), KeyValueRef(it->begin(), it->value()));
	}

	return result;
}

void ReadYourWritesTransaction::atomicOp(const KeyRef& key, const ValueRef& operand, uint32_t operationType) {
	AddConflictRange addWriteConflict{ !options.getAndResetWriteConflictDisabled() };

	if (checkUsedDuringCommit()) {
		throw used_during_commit();
	}

	if (key == metadataVersionKey) {
		if (operationType != MutationRef::SetVersionstampedValue || operand != metadataVersionRequiredValue) {
			throw client_invalid_operation();
		}
	} else if (key >= getMaxWriteKey()) {
		throw key_outside_legal_range();
	}

	if (!isValidMutationType(operationType) || !isAtomicOp((MutationRef::Type)operationType))
		throw invalid_mutation_type();

	if (key.size() > getMaxWriteKeySize(key, getTransactionState()->options.rawAccess)) {
		throw key_too_large();
	}
	if (operand.size() > CLIENT_KNOBS->VALUE_SIZE_LIMIT)
		throw value_too_large();

	if (tr.apiVersionAtLeast(510)) {
		if (operationType == MutationRef::Min)
			operationType = MutationRef::MinV2;
		else if (operationType == MutationRef::And)
			operationType = MutationRef::AndV2;
	}

	KeyRef k;
	if (!tr.apiVersionAtLeast(520) && operationType == MutationRef::SetVersionstampedKey) {
		k = key.withSuffix("\x00\x00"_sr, arena);
	} else {
		k = KeyRef(arena, key);
	}
	ValueRef v;
	if (!tr.apiVersionAtLeast(520) && operationType == MutationRef::SetVersionstampedValue) {
		v = operand.withSuffix("\x00\x00\x00\x00"_sr, arena);
	} else {
		v = ValueRef(arena, operand);
	}

	if (operationType == MutationRef::SetVersionstampedKey) {
		CODE_PROBE(options.readYourWritesDisabled, "SetVersionstampedKey without ryw enabled");
		// this does validation of the key and needs to be performed before the readYourWritesDisabled path
		KeyRangeRef range = getVersionstampKeyRange(arena, k, tr.getCachedReadVersion().orDefault(0), getMaxReadKey());
		versionStampKeys.push_back(arena, k);
		addWriteConflict = AddConflictRange::False;
		if (!options.readYourWritesDisabled) {
			writeRangeToNativeTransaction(range);
			writes.addUnmodifiedAndUnreadableRange(range);
		}
		// k is the unversionstamped key provided by the user.  If we've filled in a minimum bound
		// for the versionstamp, we need to make sure that's reflected when we insert it into the
		// WriteMap below.
		transformVersionstampKey(k, tr.getCachedReadVersion().orDefault(0), 0);
	}

	if (operationType == MutationRef::SetVersionstampedValue) {
		if (v.size() < 4)
			throw client_invalid_operation();
		int32_t pos;
		memcpy(&pos, v.end() - sizeof(int32_t), sizeof(int32_t));
		pos = littleEndian32(pos);
		if (pos < 0 || pos + 10 > v.size() - 4)
			throw client_invalid_operation();
	}

	approximateSize += k.expectedSize() + v.expectedSize() + sizeof(MutationRef) +
	                   (addWriteConflict ? sizeof(KeyRangeRef) + 2 * key.expectedSize() + 1 : 0);
	if (options.readYourWritesDisabled) {
		return tr.atomicOp(k, v, (MutationRef::Type)operationType, addWriteConflict);
	}

	writes.mutate(k, (MutationRef::Type)operationType, v, addWriteConflict);
	RYWImpl::triggerWatches(this, k, Optional<ValueRef>(), false);
}

void ReadYourWritesTransaction::set(const KeyRef& key, const ValueRef& value) {
	if (key == metadataVersionKey) {
		throw client_invalid_operation();
	}

	if (specialKeys.contains(key)) {
		if (getDatabase()->apiVersionAtLeast(700)) {
			return getDatabase()->specialKeySpace->set(this, key, value);
		} else {
			// These three special keys are deprecated in 7.0 and an alternative C API is added
			// TODO : Rewrite related code using C api
			if (key == "\xff\xff/reboot_worker"_sr) {
				BinaryReader::fromStringRef<ClientWorkerInterface>(value, IncludeVersion())
				    .reboot.send(RebootRequest());
				return;
			}
			if (key == "\xff\xff/suspend_worker"_sr) {
				BinaryReader::fromStringRef<ClientWorkerInterface>(value, IncludeVersion())
				    .reboot.send(RebootRequest(false, false, options.timeoutInSeconds));
				return;
			}
			if (key == "\xff\xff/reboot_and_check_worker"_sr) {
				BinaryReader::fromStringRef<ClientWorkerInterface>(value, IncludeVersion())
				    .reboot.send(RebootRequest(false, true));
				return;
			}
		}
	}

	AddConflictRange addWriteConflict{ !options.getAndResetWriteConflictDisabled() };

	if (checkUsedDuringCommit()) {
		throw used_during_commit();
	}

	if (key >= getMaxWriteKey())
		throw key_outside_legal_range();

	approximateSize += key.expectedSize() + value.expectedSize() + sizeof(MutationRef) +
	                   (addWriteConflict ? sizeof(KeyRangeRef) + 2 * key.expectedSize() + 1 : 0);
	if (options.readYourWritesDisabled) {
		return tr.set(key, value, addWriteConflict);
	}

	// TODO: check transaction size here
	if (key.size() > getMaxWriteKeySize(key, getTransactionState()->options.rawAccess)) {
		throw key_too_large();
	}
	if (value.size() > CLIENT_KNOBS->VALUE_SIZE_LIMIT)
		throw value_too_large();

	KeyRef k = KeyRef(arena, key);
	ValueRef v = ValueRef(arena, value);

	writes.mutate(k, MutationRef::SetValue, v, addWriteConflict);
	RYWImpl::triggerWatches(this, key, value);
}

void ReadYourWritesTransaction::clear(const KeyRangeRef& range) {
	AddConflictRange addWriteConflict{ !options.getAndResetWriteConflictDisabled() };

	if (checkUsedDuringCommit()) {
		throw used_during_commit();
	}

	if (specialKeys.contains(range)) {
		if (getDatabase()->apiVersionAtLeast(700)) {
			return getDatabase()->specialKeySpace->clear(this, range);
		}
	}

	KeyRef maxKey = getMaxWriteKey();
	if (range.begin > maxKey || range.end > maxKey)
		throw key_outside_legal_range();

	approximateSize += range.expectedSize() + sizeof(MutationRef) +
	                   (addWriteConflict ? sizeof(KeyRangeRef) + range.expectedSize() : 0);
	if (options.readYourWritesDisabled) {
		return tr.clear(range, addWriteConflict);
	}

	// There aren't any keys in the database with size larger than the max key size, so if range contains large keys
	// we can translate it to an equivalent one with smaller keys
	KeyRef begin = range.begin;
	KeyRef end = range.end;

	int64_t beginMaxSize = getMaxClearKeySize(begin);
	int64_t endMaxSize = getMaxClearKeySize(end);
	if (begin.size() > beginMaxSize) {
		begin = begin.substr(0, beginMaxSize + 1);
	}
	if (end.size() > endMaxSize) {
		end = end.substr(0, endMaxSize + 1);
	}

	KeyRangeRef r = KeyRangeRef(begin, end);

	if (r.empty()) {
		return;
	}

	r = KeyRangeRef(arena, r);

	writes.clear(r, addWriteConflict);
	RYWImpl::triggerWatches(this, r, Optional<ValueRef>());
}

void ReadYourWritesTransaction::clear(const KeyRef& key) {
	AddConflictRange addWriteConflict{ !options.getAndResetWriteConflictDisabled() };

	if (checkUsedDuringCommit()) {
		throw used_during_commit();
	}

	if (specialKeys.contains(key)) {
		if (getDatabase()->apiVersionAtLeast(700)) {
			return getDatabase()->specialKeySpace->clear(this, key);
		}
	}

	if (key >= getMaxWriteKey())
		throw key_outside_legal_range();

	if (key.size() > getMaxClearKeySize(key)) {
		return;
	}

	if (options.readYourWritesDisabled) {
		return tr.clear(key, addWriteConflict);
	}

	KeyRangeRef r = singleKeyRange(key, arena);
	approximateSize +=
	    r.expectedSize() + sizeof(KeyRangeRef) + (addWriteConflict ? sizeof(KeyRangeRef) + r.expectedSize() : 0);

	// SOMEDAY: add an optimized single key clear to write map
	writes.clear(r, addWriteConflict);

	RYWImpl::triggerWatches(this, r, Optional<ValueRef>());
}

Future<Void> ReadYourWritesTransaction::watch(const Key& key) {
	if (checkUsedDuringCommit()) {
		return used_during_commit();
	}

	if (resetPromise.isSet())
		return resetPromise.getFuture().getError();

	if (options.readYourWritesDisabled)
		return watches_disabled();

	if (key >= allKeys.end || (key >= getMaxReadKey() && key != metadataVersionKey && tr.apiVersionAtLeast(300)))
		return key_outside_legal_range();

	if (key.size() > getMaxWriteKeySize(key, getTransactionState()->options.rawAccess)) {
		return key_too_large();
	}

	return RYWImpl::watch(this, key);
}

void ReadYourWritesTransaction::addWriteConflictRange(KeyRangeRef const& keys) {
	if (checkUsedDuringCommit()) {
		throw used_during_commit();
	}

	if (tr.apiVersionAtLeast(300)) {
		if (keys.begin > getMaxWriteKey() || keys.end > getMaxWriteKey()) {
			throw key_outside_legal_range();
		}
	}

	// There aren't any keys in the database with size larger than the max key size, so if range contains large keys
	// we can translate it to an equivalent one with smaller keys
	KeyRef begin = keys.begin;
	KeyRef end = keys.end;

	int64_t beginMaxSize = getMaxKeySize(begin);
	int64_t endMaxSize = getMaxKeySize(end);
	if (begin.size() > beginMaxSize) {
		begin = begin.substr(0, beginMaxSize + 1);
	}
	if (end.size() > endMaxSize) {
		end = end.substr(0, endMaxSize + 1);
	}

	KeyRangeRef r = KeyRangeRef(begin, end);

	if (r.empty()) {
		return;
	}

	approximateSize += r.expectedSize() + sizeof(KeyRangeRef);
	if (options.readYourWritesDisabled) {
		tr.addWriteConflictRange(r);
		return;
	}

	r = KeyRangeRef(arena, r);
	writes.addConflictRange(r);
}

Future<Void> ReadYourWritesTransaction::commit() {
	Future<Void> result;
	if (checkUsedDuringCommit()) {
		result = used_during_commit();
	} else if (resetPromise.isSet()) {
		result = resetPromise.getFuture().getError();
	} else {
		result = RYWImpl::commit(this);
	}

	return debugMessages.size() > 0 || debugTraces.size() > 0 ? RYWImpl::printDebugMessages(this, result) : result;
}

Future<Standalone<StringRef>> ReadYourWritesTransaction::getVersionstamp() {
	if (checkUsedDuringCommit()) {
		return used_during_commit();
	}

	return waitOrError(tr.getVersionstamp(), resetPromise.getFuture());
}

void ReadYourWritesTransaction::setOption(FDBTransactionOptions::Option option, Optional<StringRef> value) {
	setOptionImpl(option, value);
	auto const& opt = FDBTransactionOptions::optionInfo.getMustExist(option);
	if (opt.persistent) {
		if (opt.sensitive)
			sensitivePersistentOptions.emplace_back(option, value.castTo<WipedString>());
		else
			persistentOptions.emplace_back(option, value.castTo<Standalone<StringRef>>());
	}
}

void ReadYourWritesTransaction::setOptionImpl(FDBTransactionOptions::Option option, Optional<StringRef> value) {
	switch (option) {
	case FDBTransactionOptions::READ_YOUR_WRITES_DISABLE:
		validateOptionValueNotPresent(value);

		if (reading.getFutureCount() > 0 || !cache.empty() || !writes.empty())
			throw client_invalid_operation();

		options.readYourWritesDisabled = true;
		break;

	case FDBTransactionOptions::READ_AHEAD_DISABLE:
		validateOptionValueNotPresent(value);

		options.readAheadDisabled = true;
		break;

	case FDBTransactionOptions::NEXT_WRITE_NO_WRITE_CONFLICT_RANGE:
		validateOptionValueNotPresent(value);

		options.nextWriteDisableConflictRange = true;
		break;

	case FDBTransactionOptions::ACCESS_SYSTEM_KEYS:
		validateOptionValueNotPresent(value);

		options.readSystemKeys = true;
		options.writeSystemKeys = true;
		break;

	case FDBTransactionOptions::READ_SYSTEM_KEYS:
		validateOptionValueNotPresent(value);

		options.readSystemKeys = true;
		break;

	case FDBTransactionOptions::TIMEOUT:
		options.timeoutInSeconds = extractIntOption(value, 0, std::numeric_limits<int>::max()) / 1000.0;
		resetTimeout();
		break;

	case FDBTransactionOptions::RETRY_LIMIT:
		options.maxRetries = (int)extractIntOption(value, -1, std::numeric_limits<int>::max());
		break;

	case FDBTransactionOptions::DEBUG_RETRY_LOGGING:
		options.debugRetryLogging = true;
		if (!transactionDebugInfo) {
			transactionDebugInfo = Reference<TransactionDebugInfo>::addRef(new TransactionDebugInfo());
			transactionDebugInfo->lastRetryLogTime = creationTime;
		}

		transactionDebugInfo->transactionName = value.present() ? value.get().toString() : "";
		break;
	case FDBTransactionOptions::SNAPSHOT_RYW_ENABLE:
		validateOptionValueNotPresent(value);

		options.snapshotRywEnabled++;
		break;
	case FDBTransactionOptions::SNAPSHOT_RYW_DISABLE:
		validateOptionValueNotPresent(value);

		options.snapshotRywEnabled--;
		break;
	case FDBTransactionOptions::USED_DURING_COMMIT_PROTECTION_DISABLE:
		validateOptionValueNotPresent(value);

		options.disableUsedDuringCommitProtection = true;
		break;
	case FDBTransactionOptions::SPECIAL_KEY_SPACE_RELAXED:
		validateOptionValueNotPresent(value);
		options.specialKeySpaceRelaxed = true;
		break;
	case FDBTransactionOptions::SPECIAL_KEY_SPACE_ENABLE_WRITES:
		validateOptionValueNotPresent(value);
		options.specialKeySpaceChangeConfiguration = true;
		break;
	case FDBTransactionOptions::BYPASS_UNREADABLE:
		validateOptionValueNotPresent(value);
		options.bypassUnreadable = true;
		break;
	default:
		break;
	}

	tr.setOption(option, value);
}

void ReadYourWritesTransaction::operator=(ReadYourWritesTransaction&& r) noexcept {
	cache = std::move(r.cache);
	writes = std::move(r.writes);
	arena = std::move(r.arena);
	tr = std::move(r.tr);
	readConflicts = std::move(r.readConflicts);
	watchMap = std::move(r.watchMap);
	reading = std::move(r.reading);
	resetPromise = std::move(r.resetPromise);
	r.resetPromise = Promise<Void>();
	deferredError = std::move(r.deferredError);
	retries = r.retries;
	approximateSize = r.approximateSize;
	timeoutActor = r.timeoutActor;
	creationTime = r.creationTime;
	commitStarted = r.commitStarted;
	options = r.options;
	transactionDebugInfo = r.transactionDebugInfo;
	cache.arena = &arena;
	writes.arena = &arena;
	persistentOptions = std::move(r.persistentOptions);
	sensitivePersistentOptions = std::move(r.sensitivePersistentOptions);
	nativeReadRanges = std::move(r.nativeReadRanges);
	nativeWriteRanges = std::move(r.nativeWriteRanges);
	versionStampKeys = std::move(r.versionStampKeys);
	specialKeySpaceWriteMap = std::move(r.specialKeySpaceWriteMap);
	debugTraces = std::move(r.debugTraces);
	debugMessages = std::move(r.debugMessages);
}

ReadYourWritesTransaction::ReadYourWritesTransaction(ReadYourWritesTransaction&& r) noexcept
  : ISingleThreadTransaction(std::move(r.deferredError)), arena(std::move(r.arena)), cache(std::move(r.cache)),
    writes(std::move(r.writes)), resetPromise(std::move(r.resetPromise)), reading(std::move(r.reading)),
    retries(r.retries), approximateSize(r.approximateSize), timeoutActor(std::move(r.timeoutActor)),
    creationTime(r.creationTime), commitStarted(r.commitStarted), transactionDebugInfo(r.transactionDebugInfo),
    options(r.options) {
	cache.arena = &arena;
	writes.arena = &arena;
	tr = std::move(r.tr);
	readConflicts = std::move(r.readConflicts);
	watchMap = std::move(r.watchMap);
	r.resetPromise = Promise<Void>();
	persistentOptions = std::move(r.persistentOptions);
	sensitivePersistentOptions = std::move(r.sensitivePersistentOptions);
	nativeReadRanges = std::move(r.nativeReadRanges);
	nativeWriteRanges = std::move(r.nativeWriteRanges);
	versionStampKeys = std::move(r.versionStampKeys);
	specialKeySpaceWriteMap = std::move(r.specialKeySpaceWriteMap);
	debugTraces = std::move(r.debugTraces);
	debugMessages = std::move(r.debugMessages);
}

Future<Void> ReadYourWritesTransaction::onError(Error const& e) {
	return RYWImpl::onError(this, e);
}

void ReadYourWritesTransaction::applyPersistentOptions() {
	Optional<StringRef> timeout;
	for (auto const& option : persistentOptions) {
		if (option.first == FDBTransactionOptions::TIMEOUT) {
			timeout = option.second.castTo<StringRef>();
		} else {
			setOptionImpl(option.first, option.second.castTo<StringRef>());
		}
	}
	for (auto const& option : sensitivePersistentOptions) {
		setOptionImpl(option.first, option.second.castTo<StringRef>());
	}

	// Setting a timeout can immediately cause a transaction to fail. The only timeout
	// that matters is the one most recently set, so we ignore any earlier set timeouts
	// that might inadvertently fail the transaction.
	if (timeout.present()) {
		setOptionImpl(FDBTransactionOptions::TIMEOUT, timeout);
	}
}

void ReadYourWritesTransaction::resetRyow() {
	Promise<Void> oldReset = resetPromise;
	resetPromise = Promise<Void>();

	timeoutActor.cancel();
	arena = Arena();
	cache = SnapshotCache(&arena);
	writes = WriteMap(&arena);
	readConflicts = CoalescedKeyRefRangeMap<bool>();
	versionStampKeys = VectorRef<KeyRef>();
	nativeReadRanges = Standalone<VectorRef<KeyRangeRef>>();
	nativeWriteRanges = Standalone<VectorRef<KeyRangeRef>>();
	specialKeySpaceWriteMap =
	    KeyRangeMap<std::pair<bool, Optional<Value>>>(std::make_pair(false, Optional<Value>()), specialKeys.end);
	specialKeySpaceErrorMsg.reset();
	watchMap.clear();
	reading = AndFuture();
	approximateSize = 0;
	commitStarted = false;

	deferredError = Error();

	if (tr.apiVersionAtLeast(16)) {
		options.reset(tr);
		applyPersistentOptions();
	}

	if (!oldReset.isSet())
		oldReset.sendError(transaction_cancelled());
}

void ReadYourWritesTransaction::cancel() {
	if (!resetPromise.isSet())
		resetPromise.sendError(transaction_cancelled());
}

void ReadYourWritesTransaction::reset() {
	if (debugTraces.size() > 0 || debugMessages.size() > 0) {
		// printDebugMessages returns a future but will not block if called with an empty second argument
		ASSERT(RYWImpl::printDebugMessages(this, {}).isReady());
	}

	retries = 0;
	approximateSize = 0;
	creationTime = now();
	timeoutActor.cancel();
	persistentOptions.clear();
	sensitivePersistentOptions.clear();
	options.reset(tr);
	transactionDebugInfo.clear();
	tr.fullReset();
	versionStampFuture = tr.getVersionstamp();
	std::copy(tr.getDatabase().getTransactionDefaults().begin(),
	          tr.getDatabase().getTransactionDefaults().end(),
	          std::back_inserter(persistentOptions));
	resetRyow();
}

KeyRef ReadYourWritesTransaction::getMaxReadKey() {
	if (options.readSystemKeys)
		return systemKeys.end;
	else
		return normalKeys.end;
}

KeyRef ReadYourWritesTransaction::getMaxWriteKey() {
	if (options.writeSystemKeys)
		return systemKeys.end;
	else
		return normalKeys.end;
}

ReadYourWritesTransaction::~ReadYourWritesTransaction() {
	if (!resetPromise.isSet())
		resetPromise.sendError(transaction_cancelled());

	if (debugTraces.size() || debugMessages.size()) {
		// printDebugMessages returns a future but will not block if called with an empty second argument
		[[maybe_unused]] Future<Void> f = RYWImpl::printDebugMessages(this, {});
	}
}

bool ReadYourWritesTransaction::checkUsedDuringCommit() {
	if (commitStarted && !resetPromise.isSet() && !options.disableUsedDuringCommitProtection) {
		resetPromise.sendError(used_during_commit());
	}

	return commitStarted;
}

void ReadYourWritesTransaction::debugLogRetries(Optional<Error> error) {
	bool committed = !error.present();
	if (options.debugRetryLogging) {
		double timeSinceLastLog = now() - transactionDebugInfo->lastRetryLogTime;
		double elapsed = now() - creationTime;
		if (timeSinceLastLog >= 1 || (committed && elapsed > 1)) {
			std::string transactionNameStr = "";
			if (!transactionDebugInfo->transactionName.empty())
				transactionNameStr =
				    format(" in transaction '%s'", printable(StringRef(transactionDebugInfo->transactionName)).c_str());
			if (!g_network->isSimulated()) // Fuzz workload turns this on, but we do not want stderr output in
			                               // simulation
				fprintf(stderr,
				        "fdb WARNING: long transaction (%.2fs elapsed%s, %d retries, %s)\n",
				        elapsed,
				        transactionNameStr.c_str(),
				        retries,
				        committed ? "committed" : error.get().what());
			{
				TraceEvent trace = TraceEvent("LongTransaction");
				if (error.present())
					trace.errorUnsuppressed(error.get());
				if (!transactionDebugInfo->transactionName.empty())
					trace.detail("TransactionName", transactionDebugInfo->transactionName);
				trace.detail("Elapsed", elapsed).detail("Retries", retries).detail("Committed", committed);
			}
			transactionDebugInfo->lastRetryLogTime = now();
		}
	}
}

void ReadYourWritesTransaction::debugTrace(BaseTraceEvent&& event) {
	if (event.isEnabled()) {
		debugTraces.emplace_back(std::move(event));
	}
}

void ReadYourWritesTransaction::debugPrint(std::string const& message) {
	debugMessages.push_back(message);
}
