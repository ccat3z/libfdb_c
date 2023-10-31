#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
/*
 * KeyBackedTypes.actor.h
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

#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_KEYBACKEDTYPES_ACTOR_G_H)
#define FDBCLIENT_KEYBACKEDTYPES_ACTOR_G_H
#include "fdbclient/KeyBackedTypes.actor.g.h"
#elif !defined(FDBCLIENT_KEYBACKEDTYPES_ACTOR_H)
#define FDBCLIENT_KEYBACKEDTYPES_ACTOR_H

#include <utility>
#include <vector>
#include <ranges>

#include "fdbclient/ClientBooleanParams.h"
#include "fdbclient/CommitTransaction.h"
#include "fdbclient/RunTransaction.actor.h"
#include "fdbclient/FDBOptions.g.h"
#include "fdbclient/FDBTypes.h"
#include "fdbclient/GenericTransactionHelper.h"
#include "fdbclient/Subspace.h"
#include "flow/ObjectSerializer.h"
#include "flow/Platform.h"
#include "flow/genericactors.actor.h"
#include "flow/serialize.h"
#include "flow/ThreadHelper.actor.h"

#include "flow/actorcompiler.h" // This must be the last #include.

#define KEYBACKEDTYPES_DEBUG 0
#if KEYBACKEDTYPES_DEBUG || !defined(NO_INTELLISENSE)
#define kbt_debug fmt::print
#else
#define kbt_debug(...)
#endif

// TupleCodec is a utility struct to convert a type to and from a value using Tuple encoding.
// It is used by the template classes below like KeyBackedProperty and KeyBackedMap to convert
// key parts and values from various types to Value strings and back.
// New types can be supported either by writing a new specialization or adding these
// methods to the type so that the default specialization can be used:
//   static T T::unpack(Standalone<StringRef> const& val)
//   Standalone<StringRef> T::pack(T const& val) const
// Since TupleCodec is a struct, partial specialization can be used, such as the std::pair
// partial specialization below allowing any std::pair<T1,T2> where T1 and T2 are already
// supported by TupleCodec.
template <typename T>
struct TupleCodec {
	static inline Standalone<StringRef> pack(T const& val) { return val.pack().pack(); }
	static inline T unpack(Standalone<StringRef> const& val) { return T::unpack(Tuple::unpack(val)); }
};

// If T is Tuple then conversion is simple.
template <>
inline Standalone<StringRef> TupleCodec<Tuple>::pack(Tuple const& val) {
	return val.pack();
}
template <>
inline Tuple TupleCodec<Tuple>::unpack(Standalone<StringRef> const& val) {
	return Tuple::unpack(val);
}

template <>
inline Standalone<StringRef> TupleCodec<int64_t>::pack(int64_t const& val) {
	return Tuple::makeTuple(val).pack();
}
template <>
inline int64_t TupleCodec<int64_t>::unpack(Standalone<StringRef> const& val) {
	return Tuple::unpack(val).getInt(0);
}

template <>
inline Standalone<StringRef> TupleCodec<bool>::pack(bool const& val) {
	return Tuple::makeTuple(val ? 1 : 0).pack();
}
template <>
inline bool TupleCodec<bool>::unpack(Standalone<StringRef> const& val) {
	return Tuple::unpack(val).getInt(0) == 1;
}

template <>
inline Standalone<StringRef> TupleCodec<Standalone<StringRef>>::pack(Standalone<StringRef> const& val) {
	return Tuple::makeTuple(val).pack();
}
template <>
inline Standalone<StringRef> TupleCodec<Standalone<StringRef>>::unpack(Standalone<StringRef> const& val) {
	return Tuple::unpack(val).getString(0);
}

template <>
inline Standalone<StringRef> TupleCodec<UID>::pack(UID const& val) {
	return TupleCodec<Standalone<StringRef>>::pack(BinaryWriter::toValue<UID>(val, Unversioned()));
}
template <>
inline UID TupleCodec<UID>::unpack(Standalone<StringRef> const& val) {
	return BinaryReader::fromStringRef<UID>(TupleCodec<Standalone<StringRef>>::unpack(val), Unversioned());
}

// This is backward compatible with TupleCodec<Standalone<StringRef>>
template <>
inline Standalone<StringRef> TupleCodec<std::string>::pack(std::string const& val) {
	return Tuple::makeTuple(val).pack();
}
template <>
inline std::string TupleCodec<std::string>::unpack(Standalone<StringRef> const& val) {
	return Tuple::unpack(val).getString(0).toString();
}

// Partial specialization to cover all std::pairs as long as the component types are TupleCodec compatible
template <typename First, typename Second>
struct TupleCodec<std::pair<First, Second>> {
	static Standalone<StringRef> pack(typename std::pair<First, Second> const& val) {
		// Packing a concatenated tuple is the same as concatenating two packed tuples
		return TupleCodec<First>::pack(val.first).withSuffix(TupleCodec<Second>::pack(val.second));
	}
	static std::pair<First, Second> unpack(Standalone<StringRef> const& val) {
		Tuple t = Tuple::unpack(val);
		ASSERT(t.size() == 2);
		return { TupleCodec<First>::unpack(t.subTupleRawString(0)),
			     TupleCodec<Second>::unpack(t.subTupleRawString(1)) };
	}
};

template <typename T>
struct TupleCodec<std::vector<T>> {
	static Standalone<StringRef> pack(typename std::vector<T> const& val) {
		Tuple t;
		for (T item : val) {
			// fdbclient doesn't support nested tuples yet. For now, flatten the tuple into StringRef
			t.append(TupleCodec<T>::pack(item));
		}
		return t.pack();
	}

	static std::vector<T> unpack(Standalone<StringRef> const& val) {
		Tuple t = Tuple::unpack(val);
		std::vector<T> v;

		for (int i = 0; i < t.size(); i++) {
			v.push_back(TupleCodec<T>::unpack(t.getString(i)));
		}

		return v;
	}
};

template <>
inline Standalone<StringRef> TupleCodec<KeyRange>::pack(KeyRange const& val) {
	return Tuple::makeTuple(val.begin, val.end).pack();
}
template <>
inline KeyRange TupleCodec<KeyRange>::unpack(Standalone<StringRef> const& val) {
	Tuple t = Tuple::unpack(val);
	return KeyRangeRef(t.getString(0), t.getString(1));
}

struct NullCodec {
	static Standalone<StringRef> pack(Standalone<StringRef> val) { return val; }
	static Standalone<StringRef> unpack(Standalone<StringRef> val) { return val; }
};

template <class T>
struct BinaryCodec {
	static Standalone<StringRef> pack(T val) { return BinaryWriter::toValue<T>(val, Unversioned()); }
	static T unpack(Standalone<StringRef> val) { return BinaryReader::fromStringRef<T>(val, Unversioned()); }
};

// Codec for using Flatbuffer compatible types via ObjectWriter/ObjectReader
template <typename T, typename VersionOptions>
struct ObjectCodec {
	ObjectCodec(VersionOptions vo) : vo(vo) {}
	VersionOptions vo;

	inline Standalone<StringRef> pack(T const& val) const { return ObjectWriter::toValue<T>(val, vo); }
	inline T unpack(Standalone<StringRef> const& val) const { return ObjectReader::fromStringRef<T>(val, vo); }
};

template <typename ResultType>
struct KeyBackedRangeResult {
	std::vector<ResultType> results;
	bool more;

	bool operator==(KeyBackedRangeResult const& other) const { return results == other.results && more == other.more; }
	bool operator!=(KeyBackedRangeResult const& other) const { return !(*this == other); }
};

class WatchableTrigger {
private:
	Key key;

public:
	WatchableTrigger(Key k) : key(k) {}

	template <class Transaction>
	void update(Transaction tr, AddConflictRange conflict = AddConflictRange::False) {
		std::array<uint8_t, 14> value;
		value.fill(0);
		tr->atomicOp(key, StringRef(value.data(), value.size()), MutationRef::SetVersionstampedValue);
		if (conflict) {
			tr->addReadConflictRange(singleKeyRange(key));
		}
	}

	template <class Transaction>
	Future<Optional<Versionstamp>> get(Transaction tr, Snapshot snapshot = Snapshot::False) const {
		typename transaction_future_type<Transaction, Optional<Value>>::type getFuture = tr->get(key, snapshot);

		return holdWhile(
		    getFuture,
		    map(safeThreadFutureToFuture(getFuture), [](Optional<Value> const& val) -> Optional<Versionstamp> {
			    if (val.present()) {
				    return BinaryReader::fromStringRef<Versionstamp>(*val, Unversioned());
			    }
			    return {};
		    }));
	}

	template <class Transaction>
	Future<Void> watch(Transaction tr) const {
		typename transaction_future_type<Transaction, Void>::type watchFuture = tr->watch(key);
		return holdWhile(watchFuture, safeThreadFutureToFuture(watchFuture));
	}

// Forward declaration of this static template actor does not work correctly, so this actor is forward declared
// in a way that works for both compiling and in IDEs.
#if defined(NO_INTELLISENSE)
	template <class DB>
	static Future<Version> onChangeActor(WatchableTrigger const& self,
	                                     Reference<DB> const& db,
	                                     Optional<Version> const& initialVersion,
	                                     Promise<Version> const& watching);
#else
																#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
[[nodiscard]] static Future<Version> onChangeActor( WatchableTrigger const& self, Reference<DB> const& db, Optional<Version> const& initialVersion, Promise<Version> const& watching );
template <class> friend class WatchableTrigger_OnChangeActorActorState;

#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
#endif

	// Watch the trigger until it changes.  The result will be ready when it is observed that the trigger value's
	// version is greater than initialVersion, and the observed trigger value version will be returned.
	//
	// If initialVersion is not present it will be initialized internally to the first read version successfully
	// obtained from the db.
	//
	// initialVersion can be thought of as a "last known version" but it could also be used to indiciate some future
	// version after which you want to be notifified if the trigger's value changes.
	//
	// If watching can be set, the initialized value of initialVersion will be sent to it once known.
	template <class DB>
	Future<Version> onChange(Reference<DB> db,
	                         Optional<Version> initialVersion = {},
	                         Promise<Version> watching = {}) const {
		return onChangeActor(*this, db, initialVersion, watching);
	}
};

															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
// This generated class is to be used only via onChangeActor()
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
template <class DB, class WatchableTrigger_OnChangeActorActor>
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
class WatchableTrigger_OnChangeActorActorState {
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
public:
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	WatchableTrigger_OnChangeActorActorState(WatchableTrigger const& self,Reference<DB> const& db,Optional<Version> const& initialVersion,Promise<Version> const& watching) 
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		 : self(self),
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   db(db),
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   initialVersion(initialVersion),
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   watching(watching),
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   tr(db->createTransaction())
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
	{
		fdb_probe_actor_create("onChangeActor", reinterpret_cast<unsigned long>(this));

	}
	~WatchableTrigger_OnChangeActorActorState() 
	{
		fdb_probe_actor_destroy("onChangeActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			;
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
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
		this->~WatchableTrigger_OnChangeActorActorState();
		static_cast<WatchableTrigger_OnChangeActorActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if constexpr (can_set_transaction_options<DB>)
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			db->setOptions(tr);
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		}
		try {
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (!initialVersion.present())
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			{
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				StrictFuture<Void> __when_expr_0 = store(initialVersion, safeThreadFutureToFuture(tr->getReadVersion()));
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				if (static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state = 1;
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WatchableTrigger_OnChangeActorActor, 0, Void >*>(static_cast<WatchableTrigger_OnChangeActorActor*>(this)));
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont3(loopDepth);
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
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			StrictFuture<Void> __when_expr_4 = safeThreadFutureToFuture(tr->onError(e));
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_4.get(), loopDepth); };
			static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state = 5;
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< WatchableTrigger_OnChangeActorActor, 4, Void >*>(static_cast<WatchableTrigger_OnChangeActorActor*>(this)));
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (watching.canBeSet())
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			watching.send(*initialVersion);
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		}
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		StrictFuture<Optional<Versionstamp>> __when_expr_1 = self.get(tr);
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
		static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state = 2;
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< WatchableTrigger_OnChangeActorActor, 1, Optional<Versionstamp> >*>(static_cast<WatchableTrigger_OnChangeActorActor*>(this)));
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		loopDepth = 0;

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
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state > 0) static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<WatchableTrigger_OnChangeActorActor*>(this)->ActorCallback< WatchableTrigger_OnChangeActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WatchableTrigger_OnChangeActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WatchableTrigger_OnChangeActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WatchableTrigger_OnChangeActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont5(Optional<Versionstamp> const& currentVal,int loopDepth) 
	{
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (currentVal.present() && currentVal->version > *initialVersion)
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (!static_cast<WatchableTrigger_OnChangeActorActor*>(this)->SAV<Version>::futures) { (void)(currentVal->version); this->~WatchableTrigger_OnChangeActorActorState(); static_cast<WatchableTrigger_OnChangeActorActor*>(this)->destroy(); return 0; }
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			new (&static_cast<WatchableTrigger_OnChangeActorActor*>(this)->SAV< Version >::value()) Version(currentVal->version);
			this->~WatchableTrigger_OnChangeActorActorState();
			static_cast<WatchableTrigger_OnChangeActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		watch = self.watch(tr);
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->commit());
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_2.get(), loopDepth); };
		static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state = 3;
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WatchableTrigger_OnChangeActorActor, 2, Void >*>(static_cast<WatchableTrigger_OnChangeActorActor*>(this)));
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont5(Optional<Versionstamp> && currentVal,int loopDepth) 
	{
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (currentVal.present() && currentVal->version > *initialVersion)
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (!static_cast<WatchableTrigger_OnChangeActorActor*>(this)->SAV<Version>::futures) { (void)(currentVal->version); this->~WatchableTrigger_OnChangeActorActorState(); static_cast<WatchableTrigger_OnChangeActorActor*>(this)->destroy(); return 0; }
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			new (&static_cast<WatchableTrigger_OnChangeActorActor*>(this)->SAV< Version >::value()) Version(currentVal->version);
			this->~WatchableTrigger_OnChangeActorActorState();
			static_cast<WatchableTrigger_OnChangeActorActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		watch = self.watch(tr);
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->commit());
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_2.get(), loopDepth); };
		static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state = 3;
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< WatchableTrigger_OnChangeActorActor, 2, Void >*>(static_cast<WatchableTrigger_OnChangeActorActor*>(this)));
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Optional<Versionstamp> const& currentVal,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(currentVal, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Optional<Versionstamp> && currentVal,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(std::move(currentVal), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state > 0) static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<WatchableTrigger_OnChangeActorActor*>(this)->ActorCallback< WatchableTrigger_OnChangeActorActor, 1, Optional<Versionstamp> >::remove();

	}
	void a_callback_fire(ActorCallback< WatchableTrigger_OnChangeActorActor, 1, Optional<Versionstamp> >*,Optional<Versionstamp> const& value) 
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< WatchableTrigger_OnChangeActorActor, 1, Optional<Versionstamp> >*,Optional<Versionstamp> && value) 
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< WatchableTrigger_OnChangeActorActor, 1, Optional<Versionstamp> >*,Error err) 
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont7(Void const& _,int loopDepth) 
	{
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		StrictFuture<Void> __when_expr_3 = watch;
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont7when1(__when_expr_3.get(), loopDepth); };
		static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state = 4;
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WatchableTrigger_OnChangeActorActor, 3, Void >*>(static_cast<WatchableTrigger_OnChangeActorActor*>(this)));
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void && _,int loopDepth) 
	{
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		StrictFuture<Void> __when_expr_3 = watch;
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont7when1(__when_expr_3.get(), loopDepth); };
		static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state = 4;
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< WatchableTrigger_OnChangeActorActor, 3, Void >*>(static_cast<WatchableTrigger_OnChangeActorActor*>(this)));
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state > 0) static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<WatchableTrigger_OnChangeActorActor*>(this)->ActorCallback< WatchableTrigger_OnChangeActorActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WatchableTrigger_OnChangeActorActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< WatchableTrigger_OnChangeActorActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< WatchableTrigger_OnChangeActorActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont9(Void const& _,int loopDepth) 
	{
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		tr->reset();
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void && _,int loopDepth) 
	{
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		tr->reset();
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state > 0) static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<WatchableTrigger_OnChangeActorActor*>(this)->ActorCallback< WatchableTrigger_OnChangeActorActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WatchableTrigger_OnChangeActorActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont7when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< WatchableTrigger_OnChangeActorActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont7when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< WatchableTrigger_OnChangeActorActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), 3);

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
		if (static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state > 0) static_cast<WatchableTrigger_OnChangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<WatchableTrigger_OnChangeActorActor*>(this)->ActorCallback< WatchableTrigger_OnChangeActorActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WatchableTrigger_OnChangeActorActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< WatchableTrigger_OnChangeActorActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< WatchableTrigger_OnChangeActorActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	WatchableTrigger self;
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	Reference<DB> db;
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	Optional<Version> initialVersion;
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	Promise<Version> watching;
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	Future<Void> watch;
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
};
// This generated class is to be used only via onChangeActor()
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
template <class DB>
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
class WatchableTrigger_OnChangeActorActor final : public Actor<Version>, public ActorCallback< WatchableTrigger_OnChangeActorActor<DB>, 0, Void >, public ActorCallback< WatchableTrigger_OnChangeActorActor<DB>, 1, Optional<Versionstamp> >, public ActorCallback< WatchableTrigger_OnChangeActorActor<DB>, 2, Void >, public ActorCallback< WatchableTrigger_OnChangeActorActor<DB>, 3, Void >, public ActorCallback< WatchableTrigger_OnChangeActorActor<DB>, 4, Void >, public FastAllocated<WatchableTrigger_OnChangeActorActor<DB>>, public WatchableTrigger_OnChangeActorActorState<DB, WatchableTrigger_OnChangeActorActor<DB>> {
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
public:
	using FastAllocated<WatchableTrigger_OnChangeActorActor<DB>>::operator new;
	using FastAllocated<WatchableTrigger_OnChangeActorActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Version>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WatchableTrigger_OnChangeActorActor<DB>, 0, Void >;
friend struct ActorCallback< WatchableTrigger_OnChangeActorActor<DB>, 1, Optional<Versionstamp> >;
friend struct ActorCallback< WatchableTrigger_OnChangeActorActor<DB>, 2, Void >;
friend struct ActorCallback< WatchableTrigger_OnChangeActorActor<DB>, 3, Void >;
friend struct ActorCallback< WatchableTrigger_OnChangeActorActor<DB>, 4, Void >;
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	WatchableTrigger_OnChangeActorActor(WatchableTrigger const& self,Reference<DB> const& db,Optional<Version> const& initialVersion,Promise<Version> const& watching) 
															#line 929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		 : Actor<Version>(),
		   WatchableTrigger_OnChangeActorActorState<DB, WatchableTrigger_OnChangeActorActor<DB>>(self, db, initialVersion, watching)
	{
		fdb_probe_actor_enter("onChangeActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("onChangeActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("onChangeActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WatchableTrigger_OnChangeActorActor<DB>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< WatchableTrigger_OnChangeActorActor<DB>, 1, Optional<Versionstamp> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< WatchableTrigger_OnChangeActorActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< WatchableTrigger_OnChangeActorActor<DB>, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< WatchableTrigger_OnChangeActorActor<DB>, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
template <class DB>
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
[[nodiscard]] Future<Version> WatchableTrigger::onChangeActor( WatchableTrigger const& self, Reference<DB> const& db, Optional<Version> const& initialVersion, Promise<Version> const& watching ) {
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	return Future<Version>(new WatchableTrigger_OnChangeActorActor<DB>(self, db, initialVersion, watching));
															#line 962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
}

#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"

// Convenient read/write access to a single value of type T stored at key
// Even though 'this' is not actually mutated, methods that change the db key are not const.
template <typename T, typename Codec = TupleCodec<T>>
class KeyBackedProperty {
public:
	KeyBackedProperty(KeyRef key = invalidKey, Optional<WatchableTrigger> trigger = {}, Codec codec = {})
	  : key(key), trigger(trigger), codec(codec) {}

	template <class Transaction>
	Future<Optional<T>> get(Transaction tr, Snapshot snapshot = Snapshot::False) const {

		if constexpr (is_transaction_creator<Transaction>) {
			return runTransaction(tr, [=, self = *this](decltype(tr->createTransaction()) tr) {
				tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
				tr->setOption(FDBTransactionOptions::LOCK_AWARE);
				return self.get(tr, snapshot);
			});
		} else {
			typename transaction_future_type<Transaction, Optional<Value>>::type getFuture = tr->get(key, snapshot);

			return holdWhile(
			    getFuture,
			    map(safeThreadFutureToFuture(getFuture), [codec = codec](Optional<Value> const& val) -> Optional<T> {
				    if (val.present())
					    return codec.unpack(val.get());
				    return {};
			    }));
		}
	}

	// Get property's value or defaultValue if it doesn't exist
	template <class Transaction>
	Future<T> getD(Transaction tr, Snapshot snapshot = Snapshot::False, T defaultValue = T()) const {
		if constexpr (is_transaction_creator<Transaction>) {
			return runTransaction(tr, [=, self = *this](decltype(tr->createTransaction()) tr) {
				tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
				tr->setOption(FDBTransactionOptions::LOCK_AWARE);
				return self.getD(tr, snapshot, defaultValue);
			});
		} else {
			return map(get(tr, snapshot),
			           [=](Optional<T> val) -> T { return val.present() ? val.get() : defaultValue; });
		}
	}

	// Get property's value or throw error if it doesn't exist
	template <class Transaction>
	Future<T> getOrThrow(Transaction tr, Snapshot snapshot = Snapshot::False, Error err = key_not_found()) const {
		if constexpr (is_transaction_creator<Transaction>) {
			return runTransaction(tr, [=, self = *this](decltype(tr->createTransaction()) tr) {
				tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
				tr->setOption(FDBTransactionOptions::LOCK_AWARE);
				return self.getOrThrow(tr, snapshot, err);
			});
		} else {
			return map(get(tr, snapshot), [=](Optional<T> val) -> T {
				if (!val.present()) {
					throw err;
				}

				return val.get();
			});
		}
	}

	template <class Transaction>
	void set(Transaction tr, T const& val) {
		if constexpr (is_transaction_creator<Transaction>) {
			return runTransaction(tr, [=, self = *this](decltype(tr->createTransaction()) tr) {
				tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
				tr->setOption(FDBTransactionOptions::LOCK_AWARE);
				return self.set(tr, val);
			});
		} else {
			tr->set(key, packValue(val));
			if (trigger.present()) {
				trigger->update(tr);
			}
		}
	}

	template <class Transaction>
	void clear(Transaction tr) {
		if constexpr (is_transaction_creator<Transaction>) {
			return runTransaction(tr, [=, self = *this](decltype(tr->createTransaction()) tr) {
				tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
				tr->setOption(FDBTransactionOptions::LOCK_AWARE);
				return self.clear(tr);
			});
		} else {
			tr->clear(key);
			if (trigger.present()) {
				trigger->update(tr);
			}
		}
	}

	template <class Transaction>
	Future<Void> watch(Transaction tr) {
		return tr->watch(key);
	}

	Value packValue(T const& value) const { return codec.pack(value); }
	T unpackValue(ValueRef const& value) const { return codec.unpack(value); }

	Key key;
	Optional<WatchableTrigger> trigger;
	Codec codec;
};

// KeyBackedObjectProperty is a convenience wrapper of KeyBackedProperty which uses ObjectCodec<T, VersionOptions> as
// the codec
template <typename T, typename VersionOptions>
class KeyBackedObjectProperty : public KeyBackedProperty<T, ObjectCodec<T, VersionOptions>> {
	typedef ObjectCodec<T, VersionOptions> TCodec;
	typedef KeyBackedProperty<T, TCodec> Base;

public:
	KeyBackedObjectProperty(KeyRef key, VersionOptions vo, Optional<WatchableTrigger> trigger = {})
	  : Base(key, trigger, TCodec(vo)) {}
};

// KeyBackedBinaryValue is a convenience wrapper of KeyBackedProperty but using BinaryCodec<T> as the codec and adds
// atomic ops and version stamp operations.
template <typename T>
class KeyBackedBinaryValue : public KeyBackedProperty<T, BinaryCodec<T>> {
	typedef KeyBackedProperty<T, BinaryCodec<T>> Base;

public:
	KeyBackedBinaryValue(KeyRef key = invalidKey, Optional<WatchableTrigger> trigger = {}) : Base(key, trigger) {}

	template <class Transaction>
	void atomicOp(Transaction tr, T const& val, MutationRef::Type type) {
		tr->atomicOp(this->key, BinaryWriter::toValue<T>(val, Unversioned()), type);
		if (this->trigger.present()) {
			this->trigger->update(tr);
		}
	}

	template <class Transaction>
	void setVersionstamp(Transaction tr, T const& val, int offset) {
		tr->atomicOp(
		    this->key,
		    BinaryWriter::toValue<T>(val, Unversioned()).withSuffix(StringRef(reinterpret_cast<uint8_t*>(&offset), 4)),
		    MutationRef::SetVersionstampedValue);
		if (this->trigger.present()) {
			this->trigger->update(tr);
		}
	}
};

template <typename KeyType, typename ValueType>
struct TypedKVPair {
	KeyType key;
	ValueType value;
};

template <typename KeyType>
struct TypedRange {
	KeyType begin;
	KeyType end;
};

template <typename KeyType, typename KeyCodec>
struct TypedKeySelector {
	KeyType key;
	bool orEqual;
	int offset;

	TypedKeySelector operator+(int delta) { return { key, orEqual, offset + delta }; }

	TypedKeySelector operator-(int delta) { return { key, orEqual, offset - delta }; }

	KeySelector pack(const KeyRef& prefix) const {
		Key packed = KeyCodec::pack(key).withPrefix(prefix);
		return KeySelector(KeySelectorRef(packed, orEqual, offset), packed.arena());
	}

	static TypedKeySelector lastLessThan(const KeyType& k) { return { k, false, 0 }; }

	static TypedKeySelector lastLessOrEqual(const KeyType& k) { return { k, true, 0 }; }

	static TypedKeySelector firstGreaterThan(const KeyType& k) { return { k, true, +1 }; }

	static TypedKeySelector firstGreaterOrEqual(const KeyType& k) { return { k, false, +1 }; }
};

// Convenient read/write access to a sorted map of KeyType to ValueType under prefix
// Even though 'this' is not actually mutated, methods that change db keys are not const.
template <typename _KeyType,
          typename _ValueType,
          typename KeyCodec = TupleCodec<_KeyType>,
          typename ValueCodec = TupleCodec<_ValueType>>
class KeyBackedMap {
public:
	KeyBackedMap(KeyRef prefix = invalidKey, Optional<WatchableTrigger> trigger = {}, ValueCodec valueCodec = {})
	  : subspace(prefixRange(prefix)), trigger(trigger), valueCodec(valueCodec) {}

	typedef _KeyType KeyType;
	typedef _ValueType ValueType;
	typedef std::pair<KeyType, ValueType> PairType;
	typedef KeyBackedRangeResult<PairType> RangeResultType;
	typedef TypedKVPair<KeyType, ValueType> KVType;
	typedef KeyBackedProperty<ValueType, ValueCodec> SingleRecordProperty;
	typedef TypedKeySelector<KeyType, KeyCodec> KeySelector;

	// If end is not present one key past the end of the map is used.
	template <class Transaction>
	Future<RangeResultType> getRange(Transaction tr,
	                                 Optional<KeyType> const& begin,
	                                 Optional<KeyType> const& end,
	                                 int limit,
	                                 Snapshot snapshot = Snapshot::False,
	                                 Reverse reverse = Reverse::False) const {
		Key beginKey = begin.present() ? packKey(begin.get()) : subspace.begin;
		Key endKey = end.present() ? packKey(end.get()) : subspace.end;

		typename transaction_future_type<Transaction, RangeResult>::type getRangeFuture =
		    tr->getRange(KeyRangeRef(beginKey, endKey), GetRangeLimits(limit), snapshot, reverse);

		return holdWhile(
		    getRangeFuture,
		    map(safeThreadFutureToFuture(getRangeFuture),
		        [prefix = subspace.begin, valueCodec = valueCodec](RangeResult const& kvs) -> RangeResultType {
			        RangeResultType rangeResult;
			        for (int i = 0; i < kvs.size(); ++i) {
				        KeyType key = KeyCodec::unpack(kvs[i].key.removePrefix(prefix));
				        ValueType val = valueCodec.unpack(kvs[i].value);
				        rangeResult.results.push_back(PairType(key, val));
			        }
			        rangeResult.more = kvs.more;
			        return rangeResult;
		        }));
	}

																#line 1202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
// This generated class is to be used only via getRangeActor()
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
template <class Transaction, class GetRangeActorActor>
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
class GetRangeActorActorState {
															#line 1208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
public:
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	GetRangeActorActorState(KeyBackedMap const& self,Transaction const& tr,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Snapshot const& snapshot,Reverse const& reverse) 
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		 : self(self),
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   tr(tr),
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   begin(begin),
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   end(end),
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   limits(limits),
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   snapshot(snapshot),
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   reverse(reverse)
															#line 1227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
	{
		fdb_probe_actor_create("getRangeActor", reinterpret_cast<unsigned long>(this));

	}
	~GetRangeActorActorState() 
	{
		fdb_probe_actor_destroy("getRangeActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			kbt_debug("MAP GETRANGE KeySelectors {} - {}\n", begin.pack(self.subspace.begin).toString(), end.pack(self.subspace.begin).toString());
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			ksBegin = begin.pack(self.subspace.begin);
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			ksEnd = end.pack(self.subspace.begin);
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			getRangeFuture = tr->getRange(ksBegin, ksEnd, limits, snapshot, reverse);
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			rangeResult = RangeResultType();
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			;
															#line 1252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
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
		this->~GetRangeActorActorState();
		static_cast<GetRangeActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (!static_cast<GetRangeActorActor*>(this)->SAV<RangeResultType>::futures) { (void)(rangeResult); this->~GetRangeActorActorState(); static_cast<GetRangeActorActor*>(this)->destroy(); return 0; }
															#line 1275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		new (&static_cast<GetRangeActorActor*>(this)->SAV< RangeResultType >::value()) RangeResultType(std::move(rangeResult)); // state_var_RVO
		this->~GetRangeActorActorState();
		static_cast<GetRangeActorActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		StrictFuture<RangeResult> __when_expr_0 = safeThreadFutureToFuture(getRangeFuture);
															#line 573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (static_cast<GetRangeActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<GetRangeActorActor*>(this)->actor_wait_state = 1;
															#line 573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetRangeActorActor, 0, RangeResult >*>(static_cast<GetRangeActorActor*>(this)));
															#line 1301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
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
	int a_body1loopBody1cont1(RangeResult const& kvs,int loopDepth) 
	{
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		kbt_debug("MAP GETRANGE KeySelectors {} - {} results={} more={}\n", begin.pack(self.subspace.begin).toString(), end.pack(self.subspace.begin).toString(), kvs.size(), kvs.more);
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		for( auto const& kv : kvs ) {
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			kbt_debug("   {} -> {}\n", kv.key.printable(), kv.value.printable());
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (self.subspace.contains(kv.key))
															#line 1329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			{
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				KeyType key = self.unpackKey(kv.key);
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				ValueType val = self.unpackValue(kv.value);
															#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				rangeResult.results.push_back(PairType(key, val));
															#line 1337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			}
		}
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (!kvs.more)
															#line 1342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (!rangeResult.results.empty())
															#line 1348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			rangeResult.more = self.subspace.contains(kvs.back().key);
															#line 1352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (reverse)
															#line 1357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			ksEnd = ::firstGreaterOrEqual(kvs.back().key);
															#line 1361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		}
		else
		{
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			ksBegin = ::firstGreaterThan(kvs.back().key);
															#line 1367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		}
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		getRangeFuture = tr->getRange(ksBegin, ksEnd, limits, snapshot, reverse);
															#line 1371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(RangeResult && kvs,int loopDepth) 
	{
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		kbt_debug("MAP GETRANGE KeySelectors {} - {} results={} more={}\n", begin.pack(self.subspace.begin).toString(), end.pack(self.subspace.begin).toString(), kvs.size(), kvs.more);
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		for( auto const& kv : kvs ) {
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			kbt_debug("   {} -> {}\n", kv.key.printable(), kv.value.printable());
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (self.subspace.contains(kv.key))
															#line 1386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			{
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				KeyType key = self.unpackKey(kv.key);
															#line 586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				ValueType val = self.unpackValue(kv.value);
															#line 587 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				rangeResult.results.push_back(PairType(key, val));
															#line 1394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			}
		}
															#line 592 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (!kvs.more)
															#line 1399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (!rangeResult.results.empty())
															#line 1405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			rangeResult.more = self.subspace.contains(kvs.back().key);
															#line 1409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (reverse)
															#line 1414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			ksEnd = ::firstGreaterOrEqual(kvs.back().key);
															#line 1418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		}
		else
		{
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			ksBegin = ::firstGreaterThan(kvs.back().key);
															#line 1424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		}
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		getRangeFuture = tr->getRange(ksBegin, ksEnd, limits, snapshot, reverse);
															#line 1428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& kvs,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(kvs, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && kvs,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(kvs), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetRangeActorActor*>(this)->actor_wait_state > 0) static_cast<GetRangeActorActor*>(this)->actor_wait_state = 0;
		static_cast<GetRangeActorActor*>(this)->ActorCallback< GetRangeActorActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetRangeActorActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetRangeActorActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetRangeActorActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	KeyBackedMap self;
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	Transaction tr;
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	KeySelector begin;
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	KeySelector end;
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	GetRangeLimits limits;
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	Snapshot snapshot;
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	Reverse reverse;
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	::KeySelector ksBegin;
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	::KeySelector ksEnd;
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	typename transaction_future_type<Transaction, RangeResult>::type getRangeFuture;
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	RangeResultType rangeResult;
															#line 1518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
};
// This generated class is to be used only via getRangeActor()
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
template <class Transaction>
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
class GetRangeActorActor final : public Actor<RangeResultType>, public ActorCallback< GetRangeActorActor<Transaction>, 0, RangeResult >, public FastAllocated<GetRangeActorActor<Transaction>>, public GetRangeActorActorState<Transaction, GetRangeActorActor<Transaction>> {
															#line 1525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
public:
	using FastAllocated<GetRangeActorActor<Transaction>>::operator new;
	using FastAllocated<GetRangeActorActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResultType>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetRangeActorActor<Transaction>, 0, RangeResult >;
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	GetRangeActorActor(KeyBackedMap const& self,Transaction const& tr,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Snapshot const& snapshot,Reverse const& reverse) 
															#line 1536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		 : Actor<RangeResultType>(),
		   GetRangeActorActorState<Transaction, GetRangeActorActor<Transaction>>(self, tr, begin, end, limits, snapshot, reverse)
	{
		fdb_probe_actor_enter("getRangeActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getRangeActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getRangeActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetRangeActorActor<Transaction>, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
template <class Transaction>
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
[[nodiscard]] static Future<RangeResultType> getRangeActor( KeyBackedMap const& self, Transaction const& tr, KeySelector const& begin, KeySelector const& end, GetRangeLimits const& limits, Snapshot const& snapshot, Reverse const& reverse ) {
															#line 550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	return Future<RangeResultType>(new GetRangeActorActor<Transaction>(self, tr, begin, end, limits, snapshot, reverse));
															#line 1565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
}

#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"

	// GetRange with typed KeySelectors
	template <class Transaction>
	Future<RangeResultType> getRange(Transaction tr,
	                                 KeySelector begin,
	                                 KeySelector end,
	                                 GetRangeLimits limits,
	                                 Snapshot snapshot = Snapshot::False,
	                                 Reverse reverse = Reverse::False) const {
		return getRangeActor(*this, tr, begin, end, limits, snapshot, reverse);
	}

	// Find the closest key which is <, <=, >, or >= query
	// These operation can be accomplished using KeySelectors however they run the risk of touching keys outside of
	// map subspace, which can cause problems if this touches an offline range or a key which is unreadable by range
	// read operations due to having been modified with a version stamp operation in the current transaction.
																#line 1585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
// This generated class is to be used only via seek()
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
template <class Transaction, class SeekActor>
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
class SeekActorState {
															#line 1591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
public:
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	SeekActorState(KeyBackedMap const& self,Transaction const& tr,KeyType const& query,bool const& lessThan,bool const& orEqual,Snapshot const& snapshot) 
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		 : self(self),
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   tr(tr),
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   query(query),
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   lessThan(lessThan),
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   orEqual(orEqual),
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   snapshot(snapshot)
															#line 1608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
	{
		fdb_probe_actor_create("seek", reinterpret_cast<unsigned long>(this));

	}
	~SeekActorState() 
	{
		fdb_probe_actor_destroy("seek", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			Key begin;
															#line 650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			Key end;
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (lessThan)
															#line 1627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			{
															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				begin = self.subspace.begin;
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				end = self.packKey(query);
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				if (orEqual)
															#line 1635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
				{
															#line 656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
					end = keyAfter(end);
															#line 1639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
				}
			}
			else
			{
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				begin = self.packKey(query);
															#line 660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				if (!orEqual)
															#line 1648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
				{
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
					begin = keyAfter(begin);
															#line 1652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
				}
															#line 663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				end = self.subspace.end;
															#line 1656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			}
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			getRangeFuture = tr->getRange(KeyRangeRef(begin, end), 1, snapshot, Reverse{ lessThan });
															#line 669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			StrictFuture<RangeResult> __when_expr_0 = safeThreadFutureToFuture(getRangeFuture);
															#line 669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (static_cast<SeekActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SeekActor*>(this)->actor_wait_state = 1;
															#line 669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SeekActor, 0, RangeResult >*>(static_cast<SeekActor*>(this)));
															#line 1669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
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
		this->~SeekActorState();
		static_cast<SeekActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& kvs,int loopDepth) 
	{
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (kvs.empty())
															#line 1692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (!static_cast<SeekActor*>(this)->SAV<Optional<KVType>>::futures) { (void)(Optional<KVType>()); this->~SeekActorState(); static_cast<SeekActor*>(this)->destroy(); return 0; }
															#line 1696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			new (&static_cast<SeekActor*>(this)->SAV< Optional<KVType> >::value()) Optional<KVType>(Optional<KVType>());
			this->~SeekActorState();
			static_cast<SeekActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (!static_cast<SeekActor*>(this)->SAV<Optional<KVType>>::futures) { (void)(self.unpackKV(kvs.front())); this->~SeekActorState(); static_cast<SeekActor*>(this)->destroy(); return 0; }
															#line 1704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		new (&static_cast<SeekActor*>(this)->SAV< Optional<KVType> >::value()) Optional<KVType>(self.unpackKV(kvs.front()));
		this->~SeekActorState();
		static_cast<SeekActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && kvs,int loopDepth) 
	{
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (kvs.empty())
															#line 1716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (!static_cast<SeekActor*>(this)->SAV<Optional<KVType>>::futures) { (void)(Optional<KVType>()); this->~SeekActorState(); static_cast<SeekActor*>(this)->destroy(); return 0; }
															#line 1720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			new (&static_cast<SeekActor*>(this)->SAV< Optional<KVType> >::value()) Optional<KVType>(Optional<KVType>());
			this->~SeekActorState();
			static_cast<SeekActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (!static_cast<SeekActor*>(this)->SAV<Optional<KVType>>::futures) { (void)(self.unpackKV(kvs.front())); this->~SeekActorState(); static_cast<SeekActor*>(this)->destroy(); return 0; }
															#line 1728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		new (&static_cast<SeekActor*>(this)->SAV< Optional<KVType> >::value()) Optional<KVType>(self.unpackKV(kvs.front()));
		this->~SeekActorState();
		static_cast<SeekActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& kvs,int loopDepth) 
	{
		loopDepth = a_body1cont1(kvs, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && kvs,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(kvs), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SeekActor*>(this)->actor_wait_state > 0) static_cast<SeekActor*>(this)->actor_wait_state = 0;
		static_cast<SeekActor*>(this)->ActorCallback< SeekActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< SeekActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("seek", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("seek", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SeekActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("seek", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("seek", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SeekActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("seek", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("seek", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	KeyBackedMap self;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	Transaction tr;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	KeyType query;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	bool lessThan;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	bool orEqual;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	Snapshot snapshot;
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	typename transaction_future_type<Transaction, RangeResult>::type getRangeFuture;
															#line 1813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
};
// This generated class is to be used only via seek()
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
template <class Transaction>
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
class SeekActor final : public Actor<Optional<KVType>>, public ActorCallback< SeekActor<Transaction>, 0, RangeResult >, public FastAllocated<SeekActor<Transaction>>, public SeekActorState<Transaction, SeekActor<Transaction>> {
															#line 1820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
public:
	using FastAllocated<SeekActor<Transaction>>::operator new;
	using FastAllocated<SeekActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<KVType>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SeekActor<Transaction>, 0, RangeResult >;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	SeekActor(KeyBackedMap const& self,Transaction const& tr,KeyType const& query,bool const& lessThan,bool const& orEqual,Snapshot const& snapshot) 
															#line 1831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		 : Actor<Optional<KVType>>(),
		   SeekActorState<Transaction, SeekActor<Transaction>>(self, tr, query, lessThan, orEqual, snapshot)
	{
		fdb_probe_actor_enter("seek", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("seek");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("seek", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SeekActor<Transaction>, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
template <class Transaction>
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
[[nodiscard]] static Future<Optional<KVType>> seek( KeyBackedMap const& self, Transaction const& tr, KeyType const& query, bool const& lessThan, bool const& orEqual, Snapshot const& snapshot ) {
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	return Future<Optional<KVType>>(new SeekActor<Transaction>(self, tr, query, lessThan, orEqual, snapshot));
															#line 1860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
}

#line 676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"

	template <class Transaction>
	Future<Optional<KVType>> seekLessThan(Transaction tr, KeyType query, Snapshot snapshot = Snapshot::False) const {
		return seek(*this, tr, query, true, false, snapshot);
	}

	template <class Transaction>
	Future<Optional<KVType>> seekLessOrEqual(Transaction tr, KeyType query, Snapshot snapshot = Snapshot::False) const {
		return seek(*this, tr, query, true, true, snapshot);
	}

	template <class Transaction>
	Future<Optional<KVType>> seekGreaterThan(Transaction tr, KeyType query, Snapshot snapshot = Snapshot::False) const {
		return seek(*this, tr, query, false, false, snapshot);
	}

	template <class Transaction>
	Future<Optional<KVType>> seekGreaterOrEqual(Transaction tr,
	                                            KeyType query,
	                                            Snapshot snapshot = Snapshot::False) const {
		return seek(*this, tr, query, false, true, snapshot);
	}

	template <class Transaction>
	Future<Optional<ValueType>> get(Transaction tr, KeyType const& key, Snapshot snapshot = Snapshot::False) const {
		typename transaction_future_type<Transaction, Optional<Value>>::type getFuture =
		    tr->get(packKey(key), snapshot);

		return holdWhile(getFuture,
		                 map(safeThreadFutureToFuture(getFuture),
		                     [valueCodec = valueCodec](Optional<Value> const& val) -> Optional<ValueType> {
			                     if (val.present())
				                     return valueCodec.unpack(val.get());
			                     return {};
		                     }));
	}

	// Get key's value or defaultValue if it doesn't exist
	template <class Transaction>
	Future<ValueType> getD(Transaction tr,
	                       KeyType const& key,
	                       Snapshot snapshot = Snapshot::False,
	                       ValueType defaultValue = ValueType()) const {
		return map(get(tr, key, snapshot),
		           [=](Optional<ValueType> val) -> ValueType { return val.orDefault(defaultValue); });
	}

	// Returns a Property that can be get/set that represents key's entry in this this.
	SingleRecordProperty getProperty(KeyType const& key) const { return { packKey(key), trigger, valueCodec }; }

	// Returns the expectedSize of the set key
	template <class Transaction>
	int set(Transaction tr, KeyType const& key, ValueType const& val) {
		Key k = packKey(key);
		Value v = packValue(val);
		kbt_debug("MAP SET {} -> {}\n", k.printable(), v.printable());
		tr->set(k, v);
		if (trigger.present()) {
			trigger->update(tr);
		}
		return k.expectedSize() + v.expectedSize();
	}

	template <class Transaction>
	void atomicOp(Transaction tr, KeyType const& key, ValueType const& val, MutationRef::Type type) {
		Key k = packKey(key);
		Value v = packValue(val);
		tr->atomicOp(k, v, type);
		if (trigger.present()) {
			trigger->update(tr);
		}
	}

	template <class Transaction>
	void erase(Transaction tr, KeyType const& key) {
		kbt_debug("MAP ERASE {}\n", packKey(key).printable());
		tr->clear(packKey(key));
		if (trigger.present()) {
			trigger->update(tr);
		}
	}

	template <class Transaction>
	void erase(Transaction tr, KeyType const& begin, KeyType const& end) {
		tr->clear(KeyRangeRef(packKey(begin), packKey(end)));
		if (trigger.present()) {
			trigger->update(tr);
		}
	}

	template <class Transaction>
	void clear(Transaction tr) {
		tr->clear(subspace);
		if (trigger.present()) {
			trigger->update(tr);
		}
	}

	template <class Transaction>
	void addReadConflictKey(Transaction tr, KeyType const& key) {
		Key k = packKey(key);
		tr->addReadConflictRange(singleKeyRange(k));
	}

	template <class Transaction>
	void addReadConflictRange(Transaction tr, KeyType const& begin, KeyType const& end) {
		tr->addReadConflictRange(packKey(begin), packKey(end));
	}

	template <class Transaction>
	void addWriteConflictKey(Transaction tr, KeyType const& key) {
		Key k = packKey(key);
		tr->addWriteConflictRange(singleKeyRange(k));
	}

	template <class Transaction>
	void addWriteConflictRange(Transaction tr, KeyType const& begin, KeyType const& end) {
		tr->addWriteConflictRange(packKey(begin), packKey(end));
	}

	KeyRange subspace;
	Optional<WatchableTrigger> trigger;
	ValueCodec valueCodec;

	Key packKey(KeyType const& key) const { return subspace.begin.withSuffix(KeyCodec::pack(key)); }
	KeyType unpackKey(KeyRef const& key) const { return KeyCodec::unpack(key.removePrefix(subspace.begin)); }

	Value packValue(ValueType const& value) const { return valueCodec.pack(value); }
	ValueType unpackValue(ValueRef const& value) const { return valueCodec.unpack(value); }

	KVType unpackKV(KeyValueRef const& kv) const { return { unpackKey(kv.key), unpackValue(kv.value) }; }
};

// KeyBackedObjectMap is a convenience wrapper of KeyBackedMap which uses ObjectCodec<_ValueType, VersionOptions> as
// the value codec
template <typename _KeyType, typename _ValueType, typename VersionOptions, typename KeyCodec = TupleCodec<_KeyType>>
class KeyBackedObjectMap
  : public KeyBackedMap<_KeyType, _ValueType, KeyCodec, ObjectCodec<_ValueType, VersionOptions>> {
	typedef ObjectCodec<_ValueType, VersionOptions> ValueCodec;
	typedef KeyBackedMap<_KeyType, _ValueType, KeyCodec, ValueCodec> Base;

public:
	KeyBackedObjectMap(KeyRef key, VersionOptions vo, Optional<WatchableTrigger> trigger = {})
	  : Base(key, trigger, ValueCodec(vo)) {}
};

template <typename _ValueType, typename Codec = TupleCodec<_ValueType>>
class KeyBackedSet {
public:
	KeyBackedSet(KeyRef key = invalidKey, Optional<WatchableTrigger> trigger = {})
	  : subspace(prefixRange(key)), trigger(trigger) {}

	typedef _ValueType ValueType;
	typedef KeyBackedRangeResult<ValueType> RangeResultType;
	typedef TypedKeySelector<ValueType, Codec> KeySelector;

	template <class Transaction>
	Future<RangeResultType> getRange(Transaction tr,
	                                 Optional<ValueType> const& begin,
	                                 Optional<ValueType> const& end,
	                                 int limit,
	                                 Snapshot snapshot = Snapshot::False,
	                                 Reverse reverse = Reverse::False) const {
		Key beginKey = begin.present() ? packKey(begin.get()) : subspace.begin;
		Key endKey = end.present() ? packKey(end.get()) : subspace.end;

		typename transaction_future_type<Transaction, RangeResult>::type getRangeFuture =
		    tr->getRange(KeyRangeRef(beginKey, endKey), GetRangeLimits(limit), snapshot, reverse);

		return holdWhile(getRangeFuture,
		                 map(safeThreadFutureToFuture(getRangeFuture),
		                     [prefix = subspace.begin](RangeResult const& kvs) -> RangeResultType {
			                     RangeResultType rangeResult;
			                     for (auto const& kv : kvs) {
				                     rangeResult.results.push_back(Codec::unpack(kv.key.removePrefix(prefix)));
			                     }
			                     rangeResult.more = kvs.more;
			                     return rangeResult;
		                     }));
	}

																#line 2045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
// This generated class is to be used only via getRangeActor()
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
template <class Transaction, class GetRangeActorActor1>
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
class GetRangeActorActor1State {
															#line 2051 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
public:
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	GetRangeActorActor1State(KeyBackedSet const& self,Transaction const& tr,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Snapshot const& snapshot,Reverse const& reverse) 
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		 : self(self),
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   tr(tr),
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   begin(begin),
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   end(end),
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   limits(limits),
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   snapshot(snapshot),
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   reverse(reverse)
															#line 2070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
	{
		fdb_probe_actor_create("getRangeActor", reinterpret_cast<unsigned long>(this));

	}
	~GetRangeActorActor1State() 
	{
		fdb_probe_actor_destroy("getRangeActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			kbt_debug("MAP GETRANGE KeySelectors {} - {}\n", begin.pack(self.subspace.begin).toString(), end.pack(self.subspace.begin).toString());
															#line 869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			ksBegin = begin.pack(self.subspace.begin);
															#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			ksEnd = end.pack(self.subspace.begin);
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			getRangeFuture = tr->getRange(ksBegin, ksEnd, limits, snapshot, reverse);
															#line 878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			rangeResult = RangeResultType();
															#line 879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			;
															#line 2095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
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
		this->~GetRangeActorActor1State();
		static_cast<GetRangeActorActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (!static_cast<GetRangeActorActor1*>(this)->SAV<RangeResultType>::futures) { (void)(rangeResult); this->~GetRangeActorActor1State(); static_cast<GetRangeActorActor1*>(this)->destroy(); return 0; }
															#line 2118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		new (&static_cast<GetRangeActorActor1*>(this)->SAV< RangeResultType >::value()) RangeResultType(std::move(rangeResult)); // state_var_RVO
		this->~GetRangeActorActor1State();
		static_cast<GetRangeActorActor1*>(this)->finishSendAndDelPromiseRef();
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
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		StrictFuture<RangeResult> __when_expr_0 = safeThreadFutureToFuture(getRangeFuture);
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (static_cast<GetRangeActorActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<GetRangeActorActor1*>(this)->actor_wait_state = 1;
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetRangeActorActor1, 0, RangeResult >*>(static_cast<GetRangeActorActor1*>(this)));
															#line 2144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
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
	int a_body1loopBody1cont1(RangeResult const& kvs,int loopDepth) 
	{
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		kbt_debug("MAP GETRANGE KeySelectors {} - {} results={} more={}\n", begin.pack(self.subspace.begin).toString(), end.pack(self.subspace.begin).toString(), kvs.size(), kvs.more);
															#line 887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		for( auto const& kv : kvs ) {
															#line 888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			kbt_debug("   {} -> {}\n", kv.key.printable(), kv.value.printable());
															#line 891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (self.subspace.contains(kv.key))
															#line 2172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			{
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				rangeResult.results.push_back(self.unpackKey(kv.key));
															#line 2176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			}
		}
															#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (!kvs.more)
															#line 2181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (!rangeResult.results.empty())
															#line 2187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			rangeResult.more = self.subspace.contains(kvs.back().key);
															#line 2191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (reverse)
															#line 2196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			ksEnd = ::firstGreaterOrEqual(kvs.back().key);
															#line 2200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		}
		else
		{
															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			ksBegin = ::firstGreaterThan(kvs.back().key);
															#line 2206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		}
															#line 921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		getRangeFuture = tr->getRange(ksBegin, ksEnd, limits, snapshot, reverse);
															#line 2210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(RangeResult && kvs,int loopDepth) 
	{
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		kbt_debug("MAP GETRANGE KeySelectors {} - {} results={} more={}\n", begin.pack(self.subspace.begin).toString(), end.pack(self.subspace.begin).toString(), kvs.size(), kvs.more);
															#line 887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		for( auto const& kv : kvs ) {
															#line 888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			kbt_debug("   {} -> {}\n", kv.key.printable(), kv.value.printable());
															#line 891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (self.subspace.contains(kv.key))
															#line 2225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			{
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				rangeResult.results.push_back(self.unpackKey(kv.key));
															#line 2229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			}
		}
															#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (!kvs.more)
															#line 2234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (!rangeResult.results.empty())
															#line 2240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			rangeResult.more = self.subspace.contains(kvs.back().key);
															#line 2244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (reverse)
															#line 2249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			ksEnd = ::firstGreaterOrEqual(kvs.back().key);
															#line 2253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		}
		else
		{
															#line 919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			ksBegin = ::firstGreaterThan(kvs.back().key);
															#line 2259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		}
															#line 921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		getRangeFuture = tr->getRange(ksBegin, ksEnd, limits, snapshot, reverse);
															#line 2263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& kvs,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(kvs, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && kvs,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(kvs), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetRangeActorActor1*>(this)->actor_wait_state > 0) static_cast<GetRangeActorActor1*>(this)->actor_wait_state = 0;
		static_cast<GetRangeActorActor1*>(this)->ActorCallback< GetRangeActorActor1, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetRangeActorActor1, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetRangeActorActor1, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetRangeActorActor1, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getRangeActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getRangeActor", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	KeyBackedSet self;
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	Transaction tr;
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	KeySelector begin;
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	KeySelector end;
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	GetRangeLimits limits;
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	Snapshot snapshot;
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	Reverse reverse;
															#line 869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	::KeySelector ksBegin;
															#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	::KeySelector ksEnd;
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	typename transaction_future_type<Transaction, RangeResult>::type getRangeFuture;
															#line 878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	RangeResultType rangeResult;
															#line 2353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
};
// This generated class is to be used only via getRangeActor()
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
template <class Transaction>
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
class GetRangeActorActor1 final : public Actor<RangeResultType>, public ActorCallback< GetRangeActorActor1<Transaction>, 0, RangeResult >, public FastAllocated<GetRangeActorActor1<Transaction>>, public GetRangeActorActor1State<Transaction, GetRangeActorActor1<Transaction>> {
															#line 2360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
public:
	using FastAllocated<GetRangeActorActor1<Transaction>>::operator new;
	using FastAllocated<GetRangeActorActor1<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResultType>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetRangeActorActor1<Transaction>, 0, RangeResult >;
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	GetRangeActorActor1(KeyBackedSet const& self,Transaction const& tr,KeySelector const& begin,KeySelector const& end,GetRangeLimits const& limits,Snapshot const& snapshot,Reverse const& reverse) 
															#line 2371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		 : Actor<RangeResultType>(),
		   GetRangeActorActor1State<Transaction, GetRangeActorActor1<Transaction>>(self, tr, begin, end, limits, snapshot, reverse)
	{
		fdb_probe_actor_enter("getRangeActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getRangeActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getRangeActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetRangeActorActor1<Transaction>, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
template <class Transaction>
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
[[nodiscard]] static Future<RangeResultType> getRangeActor( KeyBackedSet const& self, Transaction const& tr, KeySelector const& begin, KeySelector const& end, GetRangeLimits const& limits, Snapshot const& snapshot, Reverse const& reverse ) {
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	return Future<RangeResultType>(new GetRangeActorActor1<Transaction>(self, tr, begin, end, limits, snapshot, reverse));
															#line 2400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
}

#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"

	// GetRange with typed KeySelectors
	template <class Transaction>
	Future<RangeResultType> getRange(Transaction tr,
	                                 KeySelector begin,
	                                 KeySelector end,
	                                 GetRangeLimits limits,
	                                 Snapshot snapshot = Snapshot::False,
	                                 Reverse reverse = Reverse::False) const {
		return getRangeActor(*this, tr, begin, end, limits, snapshot, reverse);
	}

	// Find the closest key which is <, <=, >, or >= query
	// These operation can be accomplished using KeySelectors however they run the risk of touching keys outside of
	// map subspace, which can cause problems if this touches an offline range or a key which is unreadable by range
	// read operations due to having been modified with a version stamp operation in the current transaction.
																#line 2420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
// This generated class is to be used only via seek()
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
template <class Transaction, class SeekActor1>
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
class SeekActor1State {
															#line 2426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
public:
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	SeekActor1State(KeyBackedSet const& self,Transaction const& tr,ValueType const& query,bool const& lessThan,bool const& orEqual,Snapshot const& snapshot) 
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		 : self(self),
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   tr(tr),
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   query(query),
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   lessThan(lessThan),
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   orEqual(orEqual),
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		   snapshot(snapshot)
															#line 2443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
	{
		fdb_probe_actor_create("seek", reinterpret_cast<unsigned long>(this));

	}
	~SeekActor1State() 
	{
		fdb_probe_actor_destroy("seek", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			Key begin;
															#line 955 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			Key end;
															#line 957 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (lessThan)
															#line 2462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			{
															#line 958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				begin = self.subspace.begin;
															#line 959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				end = self.packKey(query);
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				if (orEqual)
															#line 2470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
				{
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
					end = keyAfter(end);
															#line 2474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
				}
			}
			else
			{
															#line 964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				begin = self.packKey(query);
															#line 965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				if (!orEqual)
															#line 2483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
				{
															#line 966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
					begin = keyAfter(begin);
															#line 2487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
				}
															#line 968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
				end = self.subspace.end;
															#line 2491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			}
															#line 971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			getRangeFuture = tr->getRange(KeyRangeRef(begin, end), 1, snapshot, Reverse{ lessThan });
															#line 974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			StrictFuture<RangeResult> __when_expr_0 = safeThreadFutureToFuture(getRangeFuture);
															#line 974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (static_cast<SeekActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SeekActor1*>(this)->actor_wait_state = 1;
															#line 974 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SeekActor1, 0, RangeResult >*>(static_cast<SeekActor1*>(this)));
															#line 2504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
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
		this->~SeekActor1State();
		static_cast<SeekActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& kvs,int loopDepth) 
	{
															#line 975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (kvs.empty())
															#line 2527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (!static_cast<SeekActor1*>(this)->SAV<Optional<ValueType>>::futures) { (void)(Optional<ValueType>()); this->~SeekActor1State(); static_cast<SeekActor1*>(this)->destroy(); return 0; }
															#line 2531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			new (&static_cast<SeekActor1*>(this)->SAV< Optional<ValueType> >::value()) Optional<ValueType>(Optional<ValueType>());
			this->~SeekActor1State();
			static_cast<SeekActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (!static_cast<SeekActor1*>(this)->SAV<Optional<ValueType>>::futures) { (void)(self.unpackKey(kvs.front())); this->~SeekActor1State(); static_cast<SeekActor1*>(this)->destroy(); return 0; }
															#line 2539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		new (&static_cast<SeekActor1*>(this)->SAV< Optional<ValueType> >::value()) Optional<ValueType>(self.unpackKey(kvs.front()));
		this->~SeekActor1State();
		static_cast<SeekActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && kvs,int loopDepth) 
	{
															#line 975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (kvs.empty())
															#line 2551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		{
															#line 976 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
			if (!static_cast<SeekActor1*>(this)->SAV<Optional<ValueType>>::futures) { (void)(Optional<ValueType>()); this->~SeekActor1State(); static_cast<SeekActor1*>(this)->destroy(); return 0; }
															#line 2555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
			new (&static_cast<SeekActor1*>(this)->SAV< Optional<ValueType> >::value()) Optional<ValueType>(Optional<ValueType>());
			this->~SeekActor1State();
			static_cast<SeekActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
		if (!static_cast<SeekActor1*>(this)->SAV<Optional<ValueType>>::futures) { (void)(self.unpackKey(kvs.front())); this->~SeekActor1State(); static_cast<SeekActor1*>(this)->destroy(); return 0; }
															#line 2563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		new (&static_cast<SeekActor1*>(this)->SAV< Optional<ValueType> >::value()) Optional<ValueType>(self.unpackKey(kvs.front()));
		this->~SeekActor1State();
		static_cast<SeekActor1*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& kvs,int loopDepth) 
	{
		loopDepth = a_body1cont1(kvs, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && kvs,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(kvs), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<SeekActor1*>(this)->actor_wait_state > 0) static_cast<SeekActor1*>(this)->actor_wait_state = 0;
		static_cast<SeekActor1*>(this)->ActorCallback< SeekActor1, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< SeekActor1, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("seek", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("seek", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SeekActor1, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("seek", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("seek", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SeekActor1, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("seek", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("seek", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	KeyBackedSet self;
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	Transaction tr;
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	ValueType query;
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	bool lessThan;
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	bool orEqual;
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	Snapshot snapshot;
															#line 971 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	typename transaction_future_type<Transaction, RangeResult>::type getRangeFuture;
															#line 2648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
};
// This generated class is to be used only via seek()
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
template <class Transaction>
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
class SeekActor1 final : public Actor<Optional<ValueType>>, public ActorCallback< SeekActor1<Transaction>, 0, RangeResult >, public FastAllocated<SeekActor1<Transaction>>, public SeekActor1State<Transaction, SeekActor1<Transaction>> {
															#line 2655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
public:
	using FastAllocated<SeekActor1<Transaction>>::operator new;
	using FastAllocated<SeekActor1<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<ValueType>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SeekActor1<Transaction>, 0, RangeResult >;
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	SeekActor1(KeyBackedSet const& self,Transaction const& tr,ValueType const& query,bool const& lessThan,bool const& orEqual,Snapshot const& snapshot) 
															#line 2666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
		 : Actor<Optional<ValueType>>(),
		   SeekActor1State<Transaction, SeekActor1<Transaction>>(self, tr, query, lessThan, orEqual, snapshot)
	{
		fdb_probe_actor_enter("seek", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("seek");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("seek", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SeekActor1<Transaction>, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
template <class Transaction>
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
[[nodiscard]] static Future<Optional<ValueType>> seek( KeyBackedSet const& self, Transaction const& tr, ValueType const& query, bool const& lessThan, bool const& orEqual, Snapshot const& snapshot ) {
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"
	return Future<Optional<ValueType>>(new SeekActor1<Transaction>(self, tr, query, lessThan, orEqual, snapshot));
															#line 2695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.g.h"
}

#line 981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/KeyBackedTypes.actor.h"

	template <class Transaction>
	Future<Optional<ValueType>> seekLessThan(Transaction tr,
	                                         ValueType query,
	                                         Snapshot snapshot = Snapshot::False) const {
		return seek(*this, tr, query, true, false, snapshot);
	}

	template <class Transaction>
	Future<Optional<ValueType>> seekLessOrEqual(Transaction tr,
	                                            ValueType query,
	                                            Snapshot snapshot = Snapshot::False) const {
		return seek(*this, tr, query, true, true, snapshot);
	}

	template <class Transaction>
	Future<Optional<ValueType>> seekGreaterThan(Transaction tr,
	                                            ValueType query,
	                                            Snapshot snapshot = Snapshot::False) const {
		return seek(*this, tr, query, false, false, snapshot);
	}

	template <class Transaction>
	Future<Optional<ValueType>> seekGreaterOrEqual(Transaction tr,
	                                               ValueType query,
	                                               Snapshot snapshot = Snapshot::False) const {
		return seek(*this, tr, query, false, true, snapshot);
	}

	template <class Transaction>
	Future<bool> exists(Transaction tr, ValueType const& val, Snapshot snapshot = Snapshot::False) const {
		typename transaction_future_type<Transaction, Optional<Value>>::type getFuture =
		    tr->get(packKey(val), snapshot);

		return holdWhile(getFuture, map(safeThreadFutureToFuture(getFuture), [](Optional<Value> const& val) -> bool {
			                 return val.present();
		                 }));
	}

	// Returns the expectedSize of the set key
	template <class Transaction>
	int insert(Transaction tr, ValueType const& val) {
		Key k = packKey(val);
		tr->set(k, StringRef());
		if (trigger.present()) {
			trigger->update(tr);
		}
		return k.expectedSize();
	}

	template <class Transaction>
	void erase(Transaction tr, ValueType const& val) {
		tr->clear(packKey(val));
		if (trigger.present()) {
			trigger->update(tr);
		}
	}

	template <class Transaction>
	void erase(Transaction tr, ValueType const& begin, ValueType const& end) {
		tr->clear(KeyRangeRef(packKey(begin), packKey(end)));
		if (trigger.present()) {
			trigger->update(tr);
		}
	}

	template <class Transaction>
	void clear(Transaction tr) {
		tr->clear(subspace);
		if (trigger.present()) {
			trigger->update(tr);
		}
	}

	KeyRange subspace;
	Optional<WatchableTrigger> trigger;

	Key packKey(ValueType const& value) const { return subspace.begin.withSuffix(Codec::pack(value)); }
	ValueType unpackKey(KeyRef const& key) const { return Codec::unpack(key.removePrefix(subspace.begin)); }
};

// KeyBackedClass is a convenient base class for a set of related KeyBacked types that exist
// under a single key prefix and other help functions relevant to the concepts that the class
// represent.
//
// A WatchableTrigger called trigger is provided, which a default key which is under the struct's
// root space.  Alternatively, a custom WatchableTrigger can be provided to the constructor
// to use any other database key instead.

class KeyBackedClass {
public:
	KeyBackedClass(StringRef prefix, Optional<Key> triggerOverride = {})
	  : subspace(prefix), trigger(triggerOverride.orDefault(subspace.pack("_changeTrigger"_sr))) {}

	Subspace subspace;
	WatchableTrigger trigger;
};

#include "flow/unactorcompiler.h"

#endif
