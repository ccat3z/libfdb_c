#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
/*
 * AuditUtils.actor.cpp
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

#include "fdbclient/AuditUtils.actor.h"

#include "fdbclient/Audit.h"
#include "fdbclient/FDBTypes.h"
#include "fdbclient/NativeAPI.actor.h"
#include "fdbclient/ReadYourWrites.h"
#include "fdbclient/ClientKnobs.h"
#include <fmt/format.h>

#include "flow/actorcompiler.h" // has to be last include

void clearAuditProgressMetadata(Transaction* tr, AuditType auditType, UID auditId) {
	// There are two possible places to store AuditProgressMetadata:
	// (1) auditServerBasedProgressRangeFor or (2) auditRangeBasedProgressRangeFor
	// Which place stores the progress metadata is decided by DDAudit design
	// This function enforces the DDAudit design when clear the progress metadata
	// Design: for replica/ha/locationMetadata, the audit always writes to RangeBased space
	// for SSShard, the audit always writes to ServerBased space
	// This function clears the progress metadata accordingly
	if (auditType == AuditType::ValidateStorageServerShard) {
		tr->clear(auditServerBasedProgressRangeFor(auditType, auditId));
	} else if (auditType == AuditType::ValidateHA) {
		tr->clear(auditRangeBasedProgressRangeFor(auditType, auditId));
	} else if (auditType == AuditType::ValidateReplica) {
		tr->clear(auditRangeBasedProgressRangeFor(auditType, auditId));
	} else if (auditType == AuditType::ValidateLocationMetadata) {
		tr->clear(auditRangeBasedProgressRangeFor(auditType, auditId));
	} else {
		UNREACHABLE();
	}
	return;
}

															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via checkStorageServerRemoved()
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class CheckStorageServerRemovedActor>
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class CheckStorageServerRemovedActorState {
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	CheckStorageServerRemovedActorState(Database const& cx,UID const& ssid) 
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : cx(cx),
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   ssid(ssid),
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   res(false),
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   tr(cx)
															#line 76 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this));

	}
	~CheckStorageServerRemovedActorState() 
	{
		fdb_probe_actor_destroy("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevDebug, "AuditUtilStorageServerRemovedStart").detail("StorageServer", ssid);
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			;
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~CheckStorageServerRemovedActorState();
		static_cast<CheckStorageServerRemovedActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevDebug, "AuditUtilStorageServerRemovedEnd").detail("StorageServer", ssid).detail("Removed", res);
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<CheckStorageServerRemovedActor*>(this)->SAV<bool>::futures) { (void)(res); this->~CheckStorageServerRemovedActorState(); static_cast<CheckStorageServerRemovedActor*>(this)->destroy(); return 0; }
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<CheckStorageServerRemovedActor*>(this)->SAV< bool >::value()) bool(std::move(res)); // state_var_RVO
		this->~CheckStorageServerRemovedActorState();
		static_cast<CheckStorageServerRemovedActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(serverListKeyFor(ssid));
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<CheckStorageServerRemovedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheckStorageServerRemovedActor*>(this)->actor_wait_state = 1;
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckStorageServerRemovedActor, 0, Optional<Value> >*>(static_cast<CheckStorageServerRemovedActor*>(this)));
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevDebug, "AuditUtilStorageServerRemovedError") .errorUnsuppressed(e) .detail("StorageServer", ssid);
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.onError(e);
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<CheckStorageServerRemovedActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<CheckStorageServerRemovedActor*>(this)->actor_wait_state = 2;
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckStorageServerRemovedActor, 1, Void >*>(static_cast<CheckStorageServerRemovedActor*>(this)));
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& serverListValue,int loopDepth) 
	{
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!serverListValue.present())
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			res = true;
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && serverListValue,int loopDepth) 
	{
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!serverListValue.present())
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			res = true;
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& serverListValue,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(serverListValue, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && serverListValue,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(serverListValue), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CheckStorageServerRemovedActor*>(this)->actor_wait_state > 0) static_cast<CheckStorageServerRemovedActor*>(this)->actor_wait_state = 0;
		static_cast<CheckStorageServerRemovedActor*>(this)->ActorCallback< CheckStorageServerRemovedActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< CheckStorageServerRemovedActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckStorageServerRemovedActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckStorageServerRemovedActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<CheckStorageServerRemovedActor*>(this)->actor_wait_state > 0) static_cast<CheckStorageServerRemovedActor*>(this)->actor_wait_state = 0;
		static_cast<CheckStorageServerRemovedActor*>(this)->ActorCallback< CheckStorageServerRemovedActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CheckStorageServerRemovedActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CheckStorageServerRemovedActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CheckStorageServerRemovedActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Database cx;
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	UID ssid;
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	bool res;
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Transaction tr;
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via checkStorageServerRemoved()
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class CheckStorageServerRemovedActor final : public Actor<bool>, public ActorCallback< CheckStorageServerRemovedActor, 0, Optional<Value> >, public ActorCallback< CheckStorageServerRemovedActor, 1, Void >, public FastAllocated<CheckStorageServerRemovedActor>, public CheckStorageServerRemovedActorState<CheckStorageServerRemovedActor> {
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<CheckStorageServerRemovedActor>::operator new;
	using FastAllocated<CheckStorageServerRemovedActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckStorageServerRemovedActor, 0, Optional<Value> >;
friend struct ActorCallback< CheckStorageServerRemovedActor, 1, Void >;
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	CheckStorageServerRemovedActor(Database const& cx,UID const& ssid) 
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<bool>(),
		   CheckStorageServerRemovedActorState<CheckStorageServerRemovedActor>(cx, ssid)
	{
		fdb_probe_actor_enter("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkStorageServerRemoved");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkStorageServerRemoved", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckStorageServerRemovedActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CheckStorageServerRemovedActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<bool> checkStorageServerRemoved( Database const& cx, UID const& ssid ) {
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<bool>(new CheckStorageServerRemovedActor(cx, ssid));
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via cancelAuditMetadata()
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class CancelAuditMetadataActor>
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class CancelAuditMetadataActorState {
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	CancelAuditMetadataActorState(Database const& cx,AuditType const& auditType,UID const& auditId) 
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : cx(cx),
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditType(auditType),
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditId(auditId)
															#line 449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("cancelAuditMetadata", reinterpret_cast<unsigned long>(this));

	}
	~CancelAuditMetadataActorState() 
	{
		fdb_probe_actor_destroy("cancelAuditMetadata", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				tr = Transaction(cx);
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				TraceEvent(SevInfo, "AuditUtilCancelAuditMetadataStart", auditId) .detail("AuditKey", auditKey(auditType, auditId));
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				;
															#line 469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~CancelAuditMetadataActorState();
		static_cast<CancelAuditMetadataActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<CancelAuditMetadataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CancelAuditMetadataActorState(); static_cast<CancelAuditMetadataActor*>(this)->destroy(); return 0; }
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<CancelAuditMetadataActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CancelAuditMetadataActorState();
		static_cast<CancelAuditMetadataActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1Catch1(cancel_audit_storage_failed(), loopDepth);
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		try {
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(auditKey(auditType, auditId));
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<CancelAuditMetadataActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CancelAuditMetadataActor*>(this)->actor_wait_state = 1;
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CancelAuditMetadataActor, 0, Optional<Value> >*>(static_cast<CancelAuditMetadataActor*>(this)));
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevWarn, "AuditUtilCancelAuditMetadataError", auditId) .detail("AuditKey", auditKey(auditType, auditId));
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_2 = tr.onError(e);
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<CancelAuditMetadataActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<CancelAuditMetadataActor*>(this)->actor_wait_state = 3;
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CancelAuditMetadataActor, 2, Void >*>(static_cast<CancelAuditMetadataActor*>(this)));
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& res_,int loopDepth) 
	{
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!res_.present())
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		toCancelState = decodeAuditStorageState(res_.get());
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		ASSERT(toCancelState.id == auditId && toCancelState.getType() == auditType);
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		toCancelState.setPhase(AuditPhase::Failed);
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		tr.set(auditKey(toCancelState.getType(), toCancelState.id), auditStorageStateValue(toCancelState));
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		clearAuditProgressMetadata(&tr, toCancelState.getType(), toCancelState.id);
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<CancelAuditMetadataActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<CancelAuditMetadataActor*>(this)->actor_wait_state = 2;
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CancelAuditMetadataActor, 1, Void >*>(static_cast<CancelAuditMetadataActor*>(this)));
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && res_,int loopDepth) 
	{
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!res_.present())
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		toCancelState = decodeAuditStorageState(res_.get());
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		ASSERT(toCancelState.id == auditId && toCancelState.getType() == auditType);
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		toCancelState.setPhase(AuditPhase::Failed);
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		tr.set(auditKey(toCancelState.getType(), toCancelState.id), auditStorageStateValue(toCancelState));
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		clearAuditProgressMetadata(&tr, toCancelState.getType(), toCancelState.id);
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<CancelAuditMetadataActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<CancelAuditMetadataActor*>(this)->actor_wait_state = 2;
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CancelAuditMetadataActor, 1, Void >*>(static_cast<CancelAuditMetadataActor*>(this)));
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& res_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(res_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && res_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(res_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CancelAuditMetadataActor*>(this)->actor_wait_state > 0) static_cast<CancelAuditMetadataActor*>(this)->actor_wait_state = 0;
		static_cast<CancelAuditMetadataActor*>(this)->ActorCallback< CancelAuditMetadataActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< CancelAuditMetadataActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CancelAuditMetadataActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CancelAuditMetadataActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevInfo, "AuditUtilCancelAuditMetadataEnd", auditId) .detail("AuditKey", auditKey(auditType, auditId));
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevInfo, "AuditUtilCancelAuditMetadataEnd", auditId) .detail("AuditKey", auditKey(auditType, auditId));
															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

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
		if (static_cast<CancelAuditMetadataActor*>(this)->actor_wait_state > 0) static_cast<CancelAuditMetadataActor*>(this)->actor_wait_state = 0;
		static_cast<CancelAuditMetadataActor*>(this)->ActorCallback< CancelAuditMetadataActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CancelAuditMetadataActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CancelAuditMetadataActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CancelAuditMetadataActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<CancelAuditMetadataActor*>(this)->actor_wait_state > 0) static_cast<CancelAuditMetadataActor*>(this)->actor_wait_state = 0;
		static_cast<CancelAuditMetadataActor*>(this)->ActorCallback< CancelAuditMetadataActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CancelAuditMetadataActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CancelAuditMetadataActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CancelAuditMetadataActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), 2);

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
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Database cx;
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditType auditType;
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	UID auditId;
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Transaction tr;
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditStorageState toCancelState;
															#line 913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via cancelAuditMetadata()
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class CancelAuditMetadataActor final : public Actor<Void>, public ActorCallback< CancelAuditMetadataActor, 0, Optional<Value> >, public ActorCallback< CancelAuditMetadataActor, 1, Void >, public ActorCallback< CancelAuditMetadataActor, 2, Void >, public FastAllocated<CancelAuditMetadataActor>, public CancelAuditMetadataActorState<CancelAuditMetadataActor> {
															#line 918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<CancelAuditMetadataActor>::operator new;
	using FastAllocated<CancelAuditMetadataActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CancelAuditMetadataActor, 0, Optional<Value> >;
friend struct ActorCallback< CancelAuditMetadataActor, 1, Void >;
friend struct ActorCallback< CancelAuditMetadataActor, 2, Void >;
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	CancelAuditMetadataActor(Database const& cx,AuditType const& auditType,UID const& auditId) 
															#line 931 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<Void>(),
		   CancelAuditMetadataActorState<CancelAuditMetadataActor>(cx, auditType, auditId)
	{
		fdb_probe_actor_enter("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("cancelAuditMetadata");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("cancelAuditMetadata", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CancelAuditMetadataActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CancelAuditMetadataActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CancelAuditMetadataActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<Void> cancelAuditMetadata( Database const& cx, AuditType const& auditType, UID const& auditId ) {
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<Void>(new CancelAuditMetadataActor(cx, auditType, auditId));
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

AuditPhase stringToAuditPhase(std::string auditPhaseStr) {
	// Convert chars of auditPhaseStr to lower case
	std::transform(auditPhaseStr.begin(), auditPhaseStr.end(), auditPhaseStr.begin(), [](unsigned char c) {
		return std::tolower(c);
	});
	if (auditPhaseStr == "running") {
		return AuditPhase::Running;
	} else if (auditPhaseStr == "complete") {
		return AuditPhase::Complete;
	} else if (auditPhaseStr == "failed") {
		return AuditPhase::Failed;
	} else if (auditPhaseStr == "error") {
		return AuditPhase::Error;
	} else {
		return AuditPhase::Invalid;
	}
}

// This is not transactional
															#line 985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via getAuditStates()
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class GetAuditStatesActor>
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class GetAuditStatesActorState {
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	GetAuditStatesActorState(Database const& cx,AuditType const& auditType,bool const& newFirst,Optional<int> const& num,Optional<AuditPhase> const& phase) 
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : cx(cx),
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditType(auditType),
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   newFirst(newFirst),
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   num(num),
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   phase(phase),
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   tr(cx),
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditStates(),
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   readBegin(),
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   readEnd(),
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   reverse(newFirst ? Reverse::True : Reverse::False)
															#line 1017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("getAuditStates", reinterpret_cast<unsigned long>(this));

	}
	~GetAuditStatesActorState() 
	{
		fdb_probe_actor_destroy("getAuditStates", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (num.present() && num.get() == 0)
															#line 1032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (!static_cast<GetAuditStatesActor*>(this)->SAV<std::vector<AuditStorageState>>::futures) { (void)(auditStates); this->~GetAuditStatesActorState(); static_cast<GetAuditStatesActor*>(this)->destroy(); return 0; }
															#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				new (&static_cast<GetAuditStatesActor*>(this)->SAV< std::vector<AuditStorageState> >::value()) std::vector<AuditStorageState>(std::move(auditStates)); // state_var_RVO
				this->~GetAuditStatesActorState();
				static_cast<GetAuditStatesActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			;
															#line 1044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~GetAuditStatesActorState();
		static_cast<GetAuditStatesActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<GetAuditStatesActor*>(this)->SAV<std::vector<AuditStorageState>>::futures) { (void)(auditStates); this->~GetAuditStatesActorState(); static_cast<GetAuditStatesActor*>(this)->destroy(); return 0; }
															#line 1067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<GetAuditStatesActor*>(this)->SAV< std::vector<AuditStorageState> >::value()) std::vector<AuditStorageState>(std::move(auditStates)); // state_var_RVO
		this->~GetAuditStatesActorState();
		static_cast<GetAuditStatesActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			readBegin = auditKeyRange(auditType).begin;
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			readEnd = auditKeyRange(auditType).end;
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			auditStates.clear();
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			;
															#line 1093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = a_body1loopBody1loopHead1(loopDepth);
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
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.onError(e);
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<GetAuditStatesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetAuditStatesActor*>(this)->actor_wait_state = 2;
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetAuditStatesActor, 1, Void >*>(static_cast<GetAuditStatesActor*>(this)));
															#line 1135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1(int loopDepth) 
	{
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		tr.setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		KeyRangeRef rangeToRead(readBegin, readEnd);
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<RangeResult> __when_expr_0 = tr.getRange(rangeToRead, num.present() ? GetRangeLimits(num.get()) : GetRangeLimits(), Snapshot::False, reverse);
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<GetAuditStatesActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<GetAuditStatesActor*>(this)->actor_wait_state = 1;
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetAuditStatesActor, 0, RangeResult >*>(static_cast<GetAuditStatesActor*>(this)));
															#line 1178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(int loopDepth) 
	{
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		for(int i = 0;i < res.size();++i) {
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			const AuditStorageState auditState = decodeAuditStorageState(res[i].value);
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (phase.present() && auditState.getPhase() != phase.get())
															#line 1204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
				continue;
			}
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			auditStates.push_back(auditState);
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (num.present() && auditStates.size() == num.get())
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (!static_cast<GetAuditStatesActor*>(this)->SAV<std::vector<AuditStorageState>>::futures) { (void)(auditStates); this->~GetAuditStatesActorState(); static_cast<GetAuditStatesActor*>(this)->destroy(); return 0; }
															#line 1216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				new (&static_cast<GetAuditStatesActor*>(this)->SAV< std::vector<AuditStorageState> >::value()) std::vector<AuditStorageState>(std::move(auditStates)); // state_var_RVO
				this->~GetAuditStatesActorState();
				static_cast<GetAuditStatesActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!res.more)
															#line 1225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (newFirst)
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			readEnd = res.front().key;
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
		else
		{
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			readBegin = keyAfter(res.back().key);
															#line 1241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		tr.reset();
															#line 1245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(RangeResult const& __res,int loopDepth) 
	{
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		res = __res;
															#line 1254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = a_body1loopBody1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(RangeResult && __res,int loopDepth) 
	{
		res = std::move(__res);
		loopDepth = a_body1loopBody1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetAuditStatesActor*>(this)->actor_wait_state > 0) static_cast<GetAuditStatesActor*>(this)->actor_wait_state = 0;
		static_cast<GetAuditStatesActor*>(this)->ActorCallback< GetAuditStatesActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetAuditStatesActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getAuditStates", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStates", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetAuditStatesActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getAuditStates", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStates", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetAuditStatesActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getAuditStates", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStates", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<GetAuditStatesActor*>(this)->actor_wait_state > 0) static_cast<GetAuditStatesActor*>(this)->actor_wait_state = 0;
		static_cast<GetAuditStatesActor*>(this)->ActorCallback< GetAuditStatesActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetAuditStatesActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getAuditStates", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStates", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetAuditStatesActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getAuditStates", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStates", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetAuditStatesActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getAuditStates", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStates", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Database cx;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditType auditType;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	bool newFirst;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Optional<int> num;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Optional<AuditPhase> phase;
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Transaction tr;
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	std::vector<AuditStorageState> auditStates;
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Key readBegin;
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Key readEnd;
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Reverse reverse;
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	RangeResult res;
															#line 1414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via getAuditStates()
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class GetAuditStatesActor final : public Actor<std::vector<AuditStorageState>>, public ActorCallback< GetAuditStatesActor, 0, RangeResult >, public ActorCallback< GetAuditStatesActor, 1, Void >, public FastAllocated<GetAuditStatesActor>, public GetAuditStatesActorState<GetAuditStatesActor> {
															#line 1419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<GetAuditStatesActor>::operator new;
	using FastAllocated<GetAuditStatesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<AuditStorageState>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetAuditStatesActor, 0, RangeResult >;
friend struct ActorCallback< GetAuditStatesActor, 1, Void >;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	GetAuditStatesActor(Database const& cx,AuditType const& auditType,bool const& newFirst,Optional<int> const& num,Optional<AuditPhase> const& phase) 
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<std::vector<AuditStorageState>>(),
		   GetAuditStatesActorState<GetAuditStatesActor>(cx, auditType, newFirst, num, phase)
	{
		fdb_probe_actor_enter("getAuditStates", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getAuditStates");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getAuditStates", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetAuditStatesActor, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetAuditStatesActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<std::vector<AuditStorageState>> getAuditStates( Database const& cx, AuditType const& auditType, bool const& newFirst, Optional<int> const& num, Optional<AuditPhase> const& phase ) {
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<std::vector<AuditStorageState>>(new GetAuditStatesActor(cx, auditType, newFirst, num, phase));
															#line 1460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

															#line 1465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via clearAuditMetadataForType()
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class ClearAuditMetadataForTypeActor>
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class ClearAuditMetadataForTypeActorState {
															#line 1472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	ClearAuditMetadataForTypeActorState(Database const& cx,AuditType const& auditType,UID const& maxAuditIdToClear,int const& numFinishAuditToKeep) 
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : cx(cx),
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditType(auditType),
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   maxAuditIdToClear(maxAuditIdToClear),
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   numFinishAuditToKeep(numFinishAuditToKeep),
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   tr(cx),
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   numFinishAuditCleaned(0)
															#line 1489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this));

	}
	~ClearAuditMetadataForTypeActorState() 
	{
		fdb_probe_actor_destroy("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevDebug, "AuditUtilClearAuditMetadataForTypeStart") .detail("AuditType", auditType) .detail("MaxAuditIdToClear", maxAuditIdToClear);
															#line 1504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			try {
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				;
															#line 1508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~ClearAuditMetadataForTypeActorState();
		static_cast<ClearAuditMetadataForTypeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<ClearAuditMetadataForTypeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ClearAuditMetadataForTypeActorState(); static_cast<ClearAuditMetadataForTypeActor*>(this)->destroy(); return 0; }
															#line 1537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<ClearAuditMetadataForTypeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ClearAuditMetadataForTypeActorState();
		static_cast<ClearAuditMetadataForTypeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevInfo, "AuditUtilClearAuditMetadataForTypeError") .detail("AuditType", auditType) .errorUnsuppressed(e);
															#line 1550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		try {
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<std::vector<AuditStorageState>> __when_expr_0 = getAuditStates(cx, auditType, false);
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<ClearAuditMetadataForTypeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ClearAuditMetadataForTypeActor*>(this)->actor_wait_state = 1;
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ClearAuditMetadataForTypeActor, 0, std::vector<AuditStorageState> >*>(static_cast<ClearAuditMetadataForTypeActor*>(this)));
															#line 1586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_2 = tr.onError(e);
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<ClearAuditMetadataForTypeActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<ClearAuditMetadataForTypeActor*>(this)->actor_wait_state = 3;
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ClearAuditMetadataForTypeActor, 2, Void >*>(static_cast<ClearAuditMetadataForTypeActor*>(this)));
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		int numFinishAudit = 0;
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		for( const auto& auditState : auditStates ) {
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (auditState.id.first() > maxAuditIdToClear.first())
															#line 1647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
				continue;
			}
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (auditState.getPhase() == AuditPhase::Complete || auditState.getPhase() == AuditPhase::Failed)
															#line 1653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				numFinishAudit++;
															#line 1657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
		}
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		const int numFinishAuditToClean = numFinishAudit - numFinishAuditToKeep;
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		numFinishAuditCleaned = 0;
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		for( const auto& auditState : auditStates ) {
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (auditState.id.first() > maxAuditIdToClear.first())
															#line 1674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
				continue;
			}
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			ASSERT(auditState.getType() == auditType);
															#line 228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (auditState.getPhase() == AuditPhase::Complete && numFinishAuditCleaned < numFinishAuditToClean)
															#line 1682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				tr.clear(auditKey(auditType, auditState.id));
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				numFinishAuditCleaned++;
															#line 1688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
			else
			{
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (auditState.getPhase() == AuditPhase::Failed && numFinishAuditCleaned < numFinishAuditToClean)
															#line 1694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				{
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					tr.clear(auditKey(auditType, auditState.id));
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					clearAuditProgressMetadata(&tr, auditType, auditState.id);
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					numFinishAuditCleaned++;
															#line 1702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				}
			}
		}
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_1 = tr.commit();
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<ClearAuditMetadataForTypeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ClearAuditMetadataForTypeActor*>(this)->actor_wait_state = 2;
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ClearAuditMetadataForTypeActor, 1, Void >*>(static_cast<ClearAuditMetadataForTypeActor*>(this)));
															#line 1715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<AuditStorageState> const& __auditStates,int loopDepth) 
	{
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		auditStates = __auditStates;
															#line 1724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<AuditStorageState> && __auditStates,int loopDepth) 
	{
		auditStates = std::move(__auditStates);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ClearAuditMetadataForTypeActor*>(this)->actor_wait_state > 0) static_cast<ClearAuditMetadataForTypeActor*>(this)->actor_wait_state = 0;
		static_cast<ClearAuditMetadataForTypeActor*>(this)->ActorCallback< ClearAuditMetadataForTypeActor, 0, std::vector<AuditStorageState> >::remove();

	}
	void a_callback_fire(ActorCallback< ClearAuditMetadataForTypeActor, 0, std::vector<AuditStorageState> >*,std::vector<AuditStorageState> const& value) 
	{
		fdb_probe_actor_enter("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ClearAuditMetadataForTypeActor, 0, std::vector<AuditStorageState> >*,std::vector<AuditStorageState> && value) 
	{
		fdb_probe_actor_enter("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ClearAuditMetadataForTypeActor, 0, std::vector<AuditStorageState> >*,Error err) 
	{
		fdb_probe_actor_enter("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevDebug, "AuditUtilClearAuditMetadataForTypeEnd") .detail("AuditType", auditType) .detail("NumCleanedFinishAudits", numFinishAuditCleaned);
															#line 1791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevDebug, "AuditUtilClearAuditMetadataForTypeEnd") .detail("AuditType", auditType) .detail("NumCleanedFinishAudits", numFinishAuditCleaned);
															#line 1800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

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
		if (static_cast<ClearAuditMetadataForTypeActor*>(this)->actor_wait_state > 0) static_cast<ClearAuditMetadataForTypeActor*>(this)->actor_wait_state = 0;
		static_cast<ClearAuditMetadataForTypeActor*>(this)->ActorCallback< ClearAuditMetadataForTypeActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ClearAuditMetadataForTypeActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ClearAuditMetadataForTypeActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ClearAuditMetadataForTypeActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<ClearAuditMetadataForTypeActor*>(this)->actor_wait_state > 0) static_cast<ClearAuditMetadataForTypeActor*>(this)->actor_wait_state = 0;
		static_cast<ClearAuditMetadataForTypeActor*>(this)->ActorCallback< ClearAuditMetadataForTypeActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ClearAuditMetadataForTypeActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ClearAuditMetadataForTypeActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ClearAuditMetadataForTypeActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), 2);

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
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Database cx;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditType auditType;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	UID maxAuditIdToClear;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	int numFinishAuditToKeep;
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Transaction tr;
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	int numFinishAuditCleaned;
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	std::vector<AuditStorageState> auditStates;
															#line 1970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via clearAuditMetadataForType()
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class ClearAuditMetadataForTypeActor final : public Actor<Void>, public ActorCallback< ClearAuditMetadataForTypeActor, 0, std::vector<AuditStorageState> >, public ActorCallback< ClearAuditMetadataForTypeActor, 1, Void >, public ActorCallback< ClearAuditMetadataForTypeActor, 2, Void >, public FastAllocated<ClearAuditMetadataForTypeActor>, public ClearAuditMetadataForTypeActorState<ClearAuditMetadataForTypeActor> {
															#line 1975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<ClearAuditMetadataForTypeActor>::operator new;
	using FastAllocated<ClearAuditMetadataForTypeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ClearAuditMetadataForTypeActor, 0, std::vector<AuditStorageState> >;
friend struct ActorCallback< ClearAuditMetadataForTypeActor, 1, Void >;
friend struct ActorCallback< ClearAuditMetadataForTypeActor, 2, Void >;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	ClearAuditMetadataForTypeActor(Database const& cx,AuditType const& auditType,UID const& maxAuditIdToClear,int const& numFinishAuditToKeep) 
															#line 1988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<Void>(),
		   ClearAuditMetadataForTypeActorState<ClearAuditMetadataForTypeActor>(cx, auditType, maxAuditIdToClear, numFinishAuditToKeep)
	{
		fdb_probe_actor_enter("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("clearAuditMetadataForType");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("clearAuditMetadataForType", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ClearAuditMetadataForTypeActor, 0, std::vector<AuditStorageState> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ClearAuditMetadataForTypeActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ClearAuditMetadataForTypeActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<Void> clearAuditMetadataForType( Database const& cx, AuditType const& auditType, UID const& maxAuditIdToClear, int const& numFinishAuditToKeep ) {
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<Void>(new ClearAuditMetadataForTypeActor(cx, auditType, maxAuditIdToClear, numFinishAuditToKeep));
															#line 2018 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

															#line 2023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via checkMoveKeysLockForAudit()
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class CheckMoveKeysLockForAuditActor>
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class CheckMoveKeysLockForAuditActorState {
															#line 2030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	CheckMoveKeysLockForAuditActorState(Transaction* const& tr,MoveKeyLockInfo const& lock,bool const& isDDEnabled,bool const& isWrite = true) 
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : tr(tr),
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   lock(lock),
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   isDDEnabled(isDDEnabled),
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   isWrite(isWrite)
															#line 2043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this));

	}
	~CheckMoveKeysLockForAuditActorState() 
	{
		fdb_probe_actor_destroy("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (!isDDEnabled)
															#line 2060 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				TraceEvent(SevDebug, "AuditUtilDisabledByInMemoryCheck").log();
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				return a_body1Catch1(movekeys_conflict(), loopDepth);
															#line 2066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr->get(moveKeysLockOwnerKey);
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<CheckMoveKeysLockForAuditActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheckMoveKeysLockForAuditActor*>(this)->actor_wait_state = 1;
															#line 274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckMoveKeysLockForAuditActor, 0, Optional<Value> >*>(static_cast<CheckMoveKeysLockForAuditActor*>(this)));
															#line 2077 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~CheckMoveKeysLockForAuditActorState();
		static_cast<CheckMoveKeysLockForAuditActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& readVal,int loopDepth) 
	{
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		UID currentOwner = readVal.present() ? BinaryReader::fromStringRef<UID>(readVal.get(), Unversioned()) : UID();
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (currentOwner == lock.prevOwner)
															#line 2102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_1 = tr->get(moveKeysLockWriteKey);
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<CheckMoveKeysLockForAuditActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<CheckMoveKeysLockForAuditActor*>(this)->actor_wait_state = 2;
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckMoveKeysLockForAuditActor, 1, Optional<Value> >*>(static_cast<CheckMoveKeysLockForAuditActor*>(this)));
															#line 2113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (currentOwner == lock.myOwner)
															#line 2120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (isWrite)
															#line 2124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				{
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					BinaryWriter wrLastWrite(Unversioned());
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					wrLastWrite << deterministicRandom()->randomUniqueID();
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					tr->set(moveKeysLockWriteKey, wrLastWrite.toValue());
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					tr->makeSelfConflicting();
															#line 2134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				}
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (!static_cast<CheckMoveKeysLockForAuditActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckMoveKeysLockForAuditActorState(); static_cast<CheckMoveKeysLockForAuditActor*>(this)->destroy(); return 0; }
															#line 2138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				new (&static_cast<CheckMoveKeysLockForAuditActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~CheckMoveKeysLockForAuditActorState();
				static_cast<CheckMoveKeysLockForAuditActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				TraceEvent(SevDebug, "AuditUtilConflictWithNewOwner") .detail("CurrentOwner", currentOwner.toString()) .detail("PrevOwner", lock.prevOwner.toString()) .detail("PrevWrite", lock.prevWrite.toString()) .detail("MyOwner", lock.myOwner.toString());
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				return a_body1Catch1(movekeys_conflict(), loopDepth);
															#line 2150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
		}

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && readVal,int loopDepth) 
	{
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		UID currentOwner = readVal.present() ? BinaryReader::fromStringRef<UID>(readVal.get(), Unversioned()) : UID();
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (currentOwner == lock.prevOwner)
															#line 2162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_1 = tr->get(moveKeysLockWriteKey);
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<CheckMoveKeysLockForAuditActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<CheckMoveKeysLockForAuditActor*>(this)->actor_wait_state = 2;
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckMoveKeysLockForAuditActor, 1, Optional<Value> >*>(static_cast<CheckMoveKeysLockForAuditActor*>(this)));
															#line 2173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (currentOwner == lock.myOwner)
															#line 2180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (isWrite)
															#line 2184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				{
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					BinaryWriter wrLastWrite(Unversioned());
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					wrLastWrite << deterministicRandom()->randomUniqueID();
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					tr->set(moveKeysLockWriteKey, wrLastWrite.toValue());
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					tr->makeSelfConflicting();
															#line 2194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				}
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (!static_cast<CheckMoveKeysLockForAuditActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckMoveKeysLockForAuditActorState(); static_cast<CheckMoveKeysLockForAuditActor*>(this)->destroy(); return 0; }
															#line 2198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				new (&static_cast<CheckMoveKeysLockForAuditActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~CheckMoveKeysLockForAuditActorState();
				static_cast<CheckMoveKeysLockForAuditActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				TraceEvent(SevDebug, "AuditUtilConflictWithNewOwner") .detail("CurrentOwner", currentOwner.toString()) .detail("PrevOwner", lock.prevOwner.toString()) .detail("PrevWrite", lock.prevWrite.toString()) .detail("MyOwner", lock.myOwner.toString());
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				return a_body1Catch1(movekeys_conflict(), loopDepth);
															#line 2210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
		}

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& readVal,int loopDepth) 
	{
		loopDepth = a_body1cont1(readVal, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && readVal,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(readVal), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CheckMoveKeysLockForAuditActor*>(this)->actor_wait_state > 0) static_cast<CheckMoveKeysLockForAuditActor*>(this)->actor_wait_state = 0;
		static_cast<CheckMoveKeysLockForAuditActor*>(this)->ActorCallback< CheckMoveKeysLockForAuditActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< CheckMoveKeysLockForAuditActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckMoveKeysLockForAuditActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckMoveKeysLockForAuditActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(Optional<Value> const& readVal,int loopDepth) 
	{
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		UID lastWrite = readVal.present() ? BinaryReader::fromStringRef<UID>(readVal.get(), Unversioned()) : UID();
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (lastWrite != lock.prevWrite)
															#line 2285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevDebug, "ConflictWithPreviousOwner");
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1Catch1(movekeys_conflict(), loopDepth);
															#line 2291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (isWrite)
															#line 2295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			BinaryWriter wrMyOwner(Unversioned());
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			wrMyOwner << lock.myOwner;
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr->set(moveKeysLockOwnerKey, wrMyOwner.toValue());
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			BinaryWriter wrLastWrite(Unversioned());
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			UID lastWriter = deterministicRandom()->randomUniqueID();
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			wrLastWrite << lastWriter;
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr->set(moveKeysLockWriteKey, wrLastWrite.toValue());
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent("AuditUtilCheckMoveKeysLock") .detail("PrevOwner", lock.prevOwner.toString()) .detail("PrevWrite", lock.prevWrite.toString()) .detail("MyOwner", lock.myOwner.toString()) .detail("Writer", lastWriter.toString());
															#line 2313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<CheckMoveKeysLockForAuditActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckMoveKeysLockForAuditActorState(); static_cast<CheckMoveKeysLockForAuditActor*>(this)->destroy(); return 0; }
															#line 2317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<CheckMoveKeysLockForAuditActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheckMoveKeysLockForAuditActorState();
		static_cast<CheckMoveKeysLockForAuditActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Optional<Value> && readVal,int loopDepth) 
	{
															#line 280 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		UID lastWrite = readVal.present() ? BinaryReader::fromStringRef<UID>(readVal.get(), Unversioned()) : UID();
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (lastWrite != lock.prevWrite)
															#line 2331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevDebug, "ConflictWithPreviousOwner");
															#line 283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1Catch1(movekeys_conflict(), loopDepth);
															#line 2337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (isWrite)
															#line 2341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			BinaryWriter wrMyOwner(Unversioned());
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			wrMyOwner << lock.myOwner;
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr->set(moveKeysLockOwnerKey, wrMyOwner.toValue());
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			BinaryWriter wrLastWrite(Unversioned());
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			UID lastWriter = deterministicRandom()->randomUniqueID();
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			wrLastWrite << lastWriter;
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr->set(moveKeysLockWriteKey, wrLastWrite.toValue());
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent("AuditUtilCheckMoveKeysLock") .detail("PrevOwner", lock.prevOwner.toString()) .detail("PrevWrite", lock.prevWrite.toString()) .detail("MyOwner", lock.myOwner.toString()) .detail("Writer", lastWriter.toString());
															#line 2359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<CheckMoveKeysLockForAuditActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckMoveKeysLockForAuditActorState(); static_cast<CheckMoveKeysLockForAuditActor*>(this)->destroy(); return 0; }
															#line 2363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<CheckMoveKeysLockForAuditActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheckMoveKeysLockForAuditActorState();
		static_cast<CheckMoveKeysLockForAuditActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Optional<Value> const& readVal,int loopDepth) 
	{
		loopDepth = a_body1cont4(readVal, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Optional<Value> && readVal,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(readVal), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CheckMoveKeysLockForAuditActor*>(this)->actor_wait_state > 0) static_cast<CheckMoveKeysLockForAuditActor*>(this)->actor_wait_state = 0;
		static_cast<CheckMoveKeysLockForAuditActor*>(this)->ActorCallback< CheckMoveKeysLockForAuditActor, 1, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< CheckMoveKeysLockForAuditActor, 1, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CheckMoveKeysLockForAuditActor, 1, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CheckMoveKeysLockForAuditActor, 1, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Transaction* tr;
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	MoveKeyLockInfo lock;
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	bool isDDEnabled;
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	bool isWrite;
															#line 2442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via checkMoveKeysLockForAudit()
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class CheckMoveKeysLockForAuditActor final : public Actor<Void>, public ActorCallback< CheckMoveKeysLockForAuditActor, 0, Optional<Value> >, public ActorCallback< CheckMoveKeysLockForAuditActor, 1, Optional<Value> >, public FastAllocated<CheckMoveKeysLockForAuditActor>, public CheckMoveKeysLockForAuditActorState<CheckMoveKeysLockForAuditActor> {
															#line 2447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<CheckMoveKeysLockForAuditActor>::operator new;
	using FastAllocated<CheckMoveKeysLockForAuditActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckMoveKeysLockForAuditActor, 0, Optional<Value> >;
friend struct ActorCallback< CheckMoveKeysLockForAuditActor, 1, Optional<Value> >;
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	CheckMoveKeysLockForAuditActor(Transaction* const& tr,MoveKeyLockInfo const& lock,bool const& isDDEnabled,bool const& isWrite = true) 
															#line 2459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<Void>(),
		   CheckMoveKeysLockForAuditActorState<CheckMoveKeysLockForAuditActor>(tr, lock, isDDEnabled, isWrite)
	{
		fdb_probe_actor_enter("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkMoveKeysLockForAudit");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkMoveKeysLockForAudit", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckMoveKeysLockForAuditActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CheckMoveKeysLockForAuditActor, 1, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] static Future<Void> checkMoveKeysLockForAudit( Transaction* const& tr, MoveKeyLockInfo const& lock, bool const& isDDEnabled, bool const& isWrite = true ) {
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<Void>(new CheckMoveKeysLockForAuditActor(tr, lock, isDDEnabled, isWrite));
															#line 2488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

															#line 2493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via persistNewAuditState()
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class PersistNewAuditStateActor>
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class PersistNewAuditStateActorState {
															#line 2500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	PersistNewAuditStateActorState(Database const& cx,AuditStorageState const& auditState,MoveKeyLockInfo const& lock,bool const& ddEnabled) 
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : cx(cx),
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditState(auditState),
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   lock(lock),
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   ddEnabled(ddEnabled)
															#line 2513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("persistNewAuditState", reinterpret_cast<unsigned long>(this));

	}
	~PersistNewAuditStateActorState() 
	{
		fdb_probe_actor_destroy("persistNewAuditState", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			ASSERT(!auditState.id.isValid());
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr = Transaction(cx);
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			auditId = UID();
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			latestExistingAuditState = AuditStorageState();
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevDebug, "AuditUtilPersistedNewAuditStateStart", auditId);
															#line 2536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			try {
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				;
															#line 2540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~PersistNewAuditStateActorState();
		static_cast<PersistNewAuditStateActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<PersistNewAuditStateActor*>(this)->SAV<UID>::futures) { (void)(auditId); this->~PersistNewAuditStateActorState(); static_cast<PersistNewAuditStateActor*>(this)->destroy(); return 0; }
															#line 2569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<PersistNewAuditStateActor*>(this)->SAV< UID >::value()) UID(std::move(auditId)); // state_var_RVO
		this->~PersistNewAuditStateActorState();
		static_cast<PersistNewAuditStateActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevWarn, "AuditUtilPersistedNewAuditStateUnretriableError", auditId) .errorUnsuppressed(e) .detail("AuditKey", auditKey(auditState.getType(), auditId));
															#line 379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			ASSERT_WE_THINK(e.code() == error_code_actor_cancelled || e.code() == error_code_movekeys_conflict);
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 2586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 2590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
			else
			{
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				return a_body1Catch1(persist_new_audit_metadata_error(), loopDepth);
															#line 2596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
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
		try {
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_0 = checkMoveKeysLockForAudit(&tr, lock, ddEnabled, true);
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state = 1;
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< PersistNewAuditStateActor, 0, Void >*>(static_cast<PersistNewAuditStateActor*>(this)));
															#line 2638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevDebug, "AuditUtilPersistedNewAuditStateError", auditId) .errorUnsuppressed(e) .detail("AuditKey", auditKey(auditState.getType(), auditId));
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_3 = tr.onError(e);
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state = 4;
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< PersistNewAuditStateActor, 3, Void >*>(static_cast<PersistNewAuditStateActor*>(this)));
															#line 2682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr.getRange(auditKeyRange(auditState.getType()), 1, Snapshot::False, Reverse::True);
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state = 2;
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PersistNewAuditStateActor, 1, RangeResult >*>(static_cast<PersistNewAuditStateActor*>(this)));
															#line 2704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr.getRange(auditKeyRange(auditState.getType()), 1, Snapshot::False, Reverse::True);
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state = 2;
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PersistNewAuditStateActor, 1, RangeResult >*>(static_cast<PersistNewAuditStateActor*>(this)));
															#line 2720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		if (static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state > 0) static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state = 0;
		static_cast<PersistNewAuditStateActor*>(this)->ActorCallback< PersistNewAuditStateActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PersistNewAuditStateActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("persistNewAuditState", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistNewAuditState", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< PersistNewAuditStateActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("persistNewAuditState", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistNewAuditState", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< PersistNewAuditStateActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("persistNewAuditState", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistNewAuditState", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(RangeResult const& res,int loopDepth) 
	{
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		ASSERT(res.size() == 0 || res.size() == 1);
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		uint64_t nextId = 1;
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!res.empty())
															#line 2796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			latestExistingAuditState = decodeAuditStorageState(res[0].value);
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (auditId.isValid())
															#line 2802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				ASSERT(latestExistingAuditState.id.first() <= auditId.first());
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (latestExistingAuditState.id.first() == auditId.first())
															#line 2808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				{
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					if (!static_cast<PersistNewAuditStateActor*>(this)->SAV<UID>::futures) { (void)(auditId); this->~PersistNewAuditStateActorState(); static_cast<PersistNewAuditStateActor*>(this)->destroy(); return 0; }
															#line 2812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
					new (&static_cast<PersistNewAuditStateActor*>(this)->SAV< UID >::value()) UID(std::move(auditId)); // state_var_RVO
					this->~PersistNewAuditStateActorState();
					static_cast<PersistNewAuditStateActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
				else
				{
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					ASSERT(auditId.first() == latestExistingAuditState.id.first() + 1);
															#line 2822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				}
			}
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			nextId = latestExistingAuditState.id.first() + 1;
															#line 2827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		auditId = UID(nextId, 0LL);
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		auditState.id = auditId;
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevVerbose, "AuditUtilPersistedNewAuditStateIdSelected", auditId) .detail("AuditKey", auditKey(auditState.getType(), auditId));
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		tr.set(auditKey(auditState.getType(), auditId), auditStorageStateValue(auditState));
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state = 3;
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< PersistNewAuditStateActor, 2, Void >*>(static_cast<PersistNewAuditStateActor*>(this)));
															#line 2846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(RangeResult && res,int loopDepth) 
	{
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		ASSERT(res.size() == 0 || res.size() == 1);
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		uint64_t nextId = 1;
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!res.empty())
															#line 2859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			latestExistingAuditState = decodeAuditStorageState(res[0].value);
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (auditId.isValid())
															#line 2865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				ASSERT(latestExistingAuditState.id.first() <= auditId.first());
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (latestExistingAuditState.id.first() == auditId.first())
															#line 2871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				{
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					if (!static_cast<PersistNewAuditStateActor*>(this)->SAV<UID>::futures) { (void)(auditId); this->~PersistNewAuditStateActorState(); static_cast<PersistNewAuditStateActor*>(this)->destroy(); return 0; }
															#line 2875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
					new (&static_cast<PersistNewAuditStateActor*>(this)->SAV< UID >::value()) UID(std::move(auditId)); // state_var_RVO
					this->~PersistNewAuditStateActorState();
					static_cast<PersistNewAuditStateActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
				else
				{
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					ASSERT(auditId.first() == latestExistingAuditState.id.first() + 1);
															#line 2885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				}
			}
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			nextId = latestExistingAuditState.id.first() + 1;
															#line 2890 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		auditId = UID(nextId, 0LL);
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		auditState.id = auditId;
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevVerbose, "AuditUtilPersistedNewAuditStateIdSelected", auditId) .detail("AuditKey", auditKey(auditState.getType(), auditId));
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		tr.set(auditKey(auditState.getType(), auditId), auditStorageStateValue(auditState));
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state = 3;
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< PersistNewAuditStateActor, 2, Void >*>(static_cast<PersistNewAuditStateActor*>(this)));
															#line 2909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(RangeResult const& res,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(res, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(RangeResult && res,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(res), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state > 0) static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state = 0;
		static_cast<PersistNewAuditStateActor*>(this)->ActorCallback< PersistNewAuditStateActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< PersistNewAuditStateActor, 1, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("persistNewAuditState", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistNewAuditState", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< PersistNewAuditStateActor, 1, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("persistNewAuditState", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistNewAuditState", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< PersistNewAuditStateActor, 1, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("persistNewAuditState", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistNewAuditState", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevDebug, "AuditUtilPersistedNewAuditState", auditId) .detail("AuditKey", auditKey(auditState.getType(), auditId));
															#line 2981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevDebug, "AuditUtilPersistedNewAuditState", auditId) .detail("AuditKey", auditKey(auditState.getType(), auditId));
															#line 2990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

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
	void a_exitChoose3() 
	{
		if (static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state > 0) static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state = 0;
		static_cast<PersistNewAuditStateActor*>(this)->ActorCallback< PersistNewAuditStateActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PersistNewAuditStateActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("persistNewAuditState", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistNewAuditState", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< PersistNewAuditStateActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("persistNewAuditState", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistNewAuditState", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< PersistNewAuditStateActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("persistNewAuditState", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistNewAuditState", reinterpret_cast<unsigned long>(this), 2);

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
		if (static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state > 0) static_cast<PersistNewAuditStateActor*>(this)->actor_wait_state = 0;
		static_cast<PersistNewAuditStateActor*>(this)->ActorCallback< PersistNewAuditStateActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PersistNewAuditStateActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("persistNewAuditState", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistNewAuditState", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< PersistNewAuditStateActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("persistNewAuditState", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistNewAuditState", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< PersistNewAuditStateActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("persistNewAuditState", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistNewAuditState", reinterpret_cast<unsigned long>(this), 3);

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
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Database cx;
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditStorageState auditState;
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	MoveKeyLockInfo lock;
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	bool ddEnabled;
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Transaction tr;
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	UID auditId;
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditStorageState latestExistingAuditState;
															#line 3160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via persistNewAuditState()
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class PersistNewAuditStateActor final : public Actor<UID>, public ActorCallback< PersistNewAuditStateActor, 0, Void >, public ActorCallback< PersistNewAuditStateActor, 1, RangeResult >, public ActorCallback< PersistNewAuditStateActor, 2, Void >, public ActorCallback< PersistNewAuditStateActor, 3, Void >, public FastAllocated<PersistNewAuditStateActor>, public PersistNewAuditStateActorState<PersistNewAuditStateActor> {
															#line 3165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<PersistNewAuditStateActor>::operator new;
	using FastAllocated<PersistNewAuditStateActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<UID>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< PersistNewAuditStateActor, 0, Void >;
friend struct ActorCallback< PersistNewAuditStateActor, 1, RangeResult >;
friend struct ActorCallback< PersistNewAuditStateActor, 2, Void >;
friend struct ActorCallback< PersistNewAuditStateActor, 3, Void >;
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	PersistNewAuditStateActor(Database const& cx,AuditStorageState const& auditState,MoveKeyLockInfo const& lock,bool const& ddEnabled) 
															#line 3179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<UID>(),
		   PersistNewAuditStateActorState<PersistNewAuditStateActor>(cx, auditState, lock, ddEnabled)
	{
		fdb_probe_actor_enter("persistNewAuditState", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("persistNewAuditState");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("persistNewAuditState", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< PersistNewAuditStateActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< PersistNewAuditStateActor, 1, RangeResult >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< PersistNewAuditStateActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< PersistNewAuditStateActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<UID> persistNewAuditState( Database const& cx, AuditStorageState const& auditState, MoveKeyLockInfo const& lock, bool const& ddEnabled ) {
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<UID>(new PersistNewAuditStateActor(cx, auditState, lock, ddEnabled));
															#line 3210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

															#line 3215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via persistAuditState()
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class PersistAuditStateActor>
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class PersistAuditStateActorState {
															#line 3222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	PersistAuditStateActorState(Database const& cx,AuditStorageState const& auditState,std::string const& context,MoveKeyLockInfo const& lock,bool const& ddEnabled) 
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : cx(cx),
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditState(auditState),
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   context(context),
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   lock(lock),
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   ddEnabled(ddEnabled),
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   tr(cx),
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditPhase(auditState.getPhase())
															#line 3241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("persistAuditState", reinterpret_cast<unsigned long>(this));

	}
	~PersistAuditStateActorState() 
	{
		fdb_probe_actor_destroy("persistAuditState", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			ASSERT(auditPhase == AuditPhase::Complete || auditPhase == AuditPhase::Failed || auditPhase == AuditPhase::Error);
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			;
															#line 3258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~PersistAuditStateActorState();
		static_cast<PersistAuditStateActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<PersistAuditStateActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~PersistAuditStateActorState(); static_cast<PersistAuditStateActor*>(this)->destroy(); return 0; }
															#line 3281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<PersistAuditStateActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~PersistAuditStateActorState();
		static_cast<PersistAuditStateActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_0 = checkMoveKeysLockForAudit(&tr, lock, ddEnabled, true);
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<PersistAuditStateActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<PersistAuditStateActor*>(this)->actor_wait_state = 1;
															#line 404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateActor, 0, Void >*>(static_cast<PersistAuditStateActor*>(this)));
															#line 3314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevWarn, "AuditUtilPersistAuditStateError", auditState.id) .errorUnsuppressed(e) .detail("AuditID", auditState.id) .detail("AuditType", auditState.getType()) .detail("AuditPhase", auditPhase) .detail("AuditKey", auditKey(auditState.getType(), auditState.id)) .detail("Context", context);
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_3 = tr.onError(e);
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<PersistAuditStateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<PersistAuditStateActor*>(this)->actor_wait_state = 4;
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateActor, 3, Void >*>(static_cast<PersistAuditStateActor*>(this)));
															#line 3358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (auditPhase == AuditPhase::Complete)
															#line 3373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			clearAuditProgressMetadata(&tr, auditState.getType(), auditState.id);
															#line 3377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Optional<Value>> __when_expr_1 = tr.get(auditKey(auditState.getType(), auditState.id));
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistAuditStateActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<PersistAuditStateActor*>(this)->actor_wait_state = 2;
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateActor, 1, Optional<Value> >*>(static_cast<PersistAuditStateActor*>(this)));
															#line 3388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (auditPhase == AuditPhase::Complete)
															#line 3397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			clearAuditProgressMetadata(&tr, auditState.getType(), auditState.id);
															#line 3401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Optional<Value>> __when_expr_1 = tr.get(auditKey(auditState.getType(), auditState.id));
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistAuditStateActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<PersistAuditStateActor*>(this)->actor_wait_state = 2;
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateActor, 1, Optional<Value> >*>(static_cast<PersistAuditStateActor*>(this)));
															#line 3412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		if (static_cast<PersistAuditStateActor*>(this)->actor_wait_state > 0) static_cast<PersistAuditStateActor*>(this)->actor_wait_state = 0;
		static_cast<PersistAuditStateActor*>(this)->ActorCallback< PersistAuditStateActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PersistAuditStateActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("persistAuditState", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditState", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< PersistAuditStateActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("persistAuditState", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditState", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< PersistAuditStateActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("persistAuditState", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditState", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Optional<Value> const& res_,int loopDepth) 
	{
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!res_.present())
															#line 3484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1loopBody1Catch1(audit_storage_cancelled(), loopDepth);
															#line 3488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
		else
		{
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			const AuditStorageState currentState = decodeAuditStorageState(res_.get());
															#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			ASSERT(currentState.id == auditState.id && currentState.getType() == auditState.getType());
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (currentState.getPhase() == AuditPhase::Failed)
															#line 3498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				return a_body1loopBody1Catch1(audit_storage_cancelled(), loopDepth);
															#line 3502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
		}
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		tr.set(auditKey(auditState.getType(), auditState.id), auditStorageStateValue(auditState));
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistAuditStateActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<PersistAuditStateActor*>(this)->actor_wait_state = 3;
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateActor, 2, Void >*>(static_cast<PersistAuditStateActor*>(this)));
															#line 3516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Optional<Value> && res_,int loopDepth) 
	{
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!res_.present())
															#line 3525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1loopBody1Catch1(audit_storage_cancelled(), loopDepth);
															#line 3529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
		else
		{
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			const AuditStorageState currentState = decodeAuditStorageState(res_.get());
															#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			ASSERT(currentState.id == auditState.id && currentState.getType() == auditState.getType());
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (currentState.getPhase() == AuditPhase::Failed)
															#line 3539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				return a_body1loopBody1Catch1(audit_storage_cancelled(), loopDepth);
															#line 3543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
		}
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		tr.set(auditKey(auditState.getType(), auditState.id), auditStorageStateValue(auditState));
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistAuditStateActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<PersistAuditStateActor*>(this)->actor_wait_state = 3;
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateActor, 2, Void >*>(static_cast<PersistAuditStateActor*>(this)));
															#line 3557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Optional<Value> const& res_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(res_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Optional<Value> && res_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(res_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<PersistAuditStateActor*>(this)->actor_wait_state > 0) static_cast<PersistAuditStateActor*>(this)->actor_wait_state = 0;
		static_cast<PersistAuditStateActor*>(this)->ActorCallback< PersistAuditStateActor, 1, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< PersistAuditStateActor, 1, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("persistAuditState", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditState", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< PersistAuditStateActor, 1, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("persistAuditState", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditState", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< PersistAuditStateActor, 1, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("persistAuditState", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditState", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont5(Void const& _,int loopDepth) 
	{
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevInfo, "AuditUtilPersistAuditState", auditState.id) .detail("AuditID", auditState.id) .detail("AuditType", auditState.getType()) .detail("AuditPhase", auditPhase) .detail("AuditKey", auditKey(auditState.getType(), auditState.id)) .detail("Context", context);
															#line 3629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont5(Void && _,int loopDepth) 
	{
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevInfo, "AuditUtilPersistAuditState", auditState.id) .detail("AuditID", auditState.id) .detail("AuditType", auditState.getType()) .detail("AuditPhase", auditPhase) .detail("AuditKey", auditKey(auditState.getType(), auditState.id)) .detail("Context", context);
															#line 3638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

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
		if (static_cast<PersistAuditStateActor*>(this)->actor_wait_state > 0) static_cast<PersistAuditStateActor*>(this)->actor_wait_state = 0;
		static_cast<PersistAuditStateActor*>(this)->ActorCallback< PersistAuditStateActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PersistAuditStateActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("persistAuditState", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditState", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< PersistAuditStateActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("persistAuditState", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditState", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< PersistAuditStateActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("persistAuditState", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditState", reinterpret_cast<unsigned long>(this), 2);

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
		if (static_cast<PersistAuditStateActor*>(this)->actor_wait_state > 0) static_cast<PersistAuditStateActor*>(this)->actor_wait_state = 0;
		static_cast<PersistAuditStateActor*>(this)->ActorCallback< PersistAuditStateActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PersistAuditStateActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("persistAuditState", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditState", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< PersistAuditStateActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("persistAuditState", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditState", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< PersistAuditStateActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("persistAuditState", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditState", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Database cx;
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditStorageState auditState;
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	std::string context;
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	MoveKeyLockInfo lock;
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	bool ddEnabled;
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Transaction tr;
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditPhase auditPhase;
															#line 3795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via persistAuditState()
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class PersistAuditStateActor final : public Actor<Void>, public ActorCallback< PersistAuditStateActor, 0, Void >, public ActorCallback< PersistAuditStateActor, 1, Optional<Value> >, public ActorCallback< PersistAuditStateActor, 2, Void >, public ActorCallback< PersistAuditStateActor, 3, Void >, public FastAllocated<PersistAuditStateActor>, public PersistAuditStateActorState<PersistAuditStateActor> {
															#line 3800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<PersistAuditStateActor>::operator new;
	using FastAllocated<PersistAuditStateActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< PersistAuditStateActor, 0, Void >;
friend struct ActorCallback< PersistAuditStateActor, 1, Optional<Value> >;
friend struct ActorCallback< PersistAuditStateActor, 2, Void >;
friend struct ActorCallback< PersistAuditStateActor, 3, Void >;
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	PersistAuditStateActor(Database const& cx,AuditStorageState const& auditState,std::string const& context,MoveKeyLockInfo const& lock,bool const& ddEnabled) 
															#line 3814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<Void>(),
		   PersistAuditStateActorState<PersistAuditStateActor>(cx, auditState, context, lock, ddEnabled)
	{
		fdb_probe_actor_enter("persistAuditState", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("persistAuditState");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("persistAuditState", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< PersistAuditStateActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< PersistAuditStateActor, 1, Optional<Value> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< PersistAuditStateActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< PersistAuditStateActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<Void> persistAuditState( Database const& cx, AuditStorageState const& auditState, std::string const& context, MoveKeyLockInfo const& lock, bool const& ddEnabled ) {
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<Void>(new PersistAuditStateActor(cx, auditState, context, lock, ddEnabled));
															#line 3845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

															#line 3850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via getAuditState()
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class GetAuditStateActor>
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class GetAuditStateActorState {
															#line 3857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	GetAuditStateActorState(Database const& cx,AuditType const& type,UID const& id) 
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : cx(cx),
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   type(type),
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   id(id),
															#line 446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   tr(cx),
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   res()
															#line 3872 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("getAuditState", reinterpret_cast<unsigned long>(this));

	}
	~GetAuditStateActorState() 
	{
		fdb_probe_actor_destroy("getAuditState", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			;
															#line 3887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~GetAuditStateActorState();
		static_cast<GetAuditStateActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!res.present())
															#line 3910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1Catch1(key_not_found(), loopDepth);
															#line 3914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<GetAuditStateActor*>(this)->SAV<AuditStorageState>::futures) { (void)(decodeAuditStorageState(res.get())); this->~GetAuditStateActorState(); static_cast<GetAuditStateActor*>(this)->destroy(); return 0; }
															#line 3918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<GetAuditStateActor*>(this)->SAV< AuditStorageState >::value()) AuditStorageState(decodeAuditStorageState(res.get()));
		this->~GetAuditStateActorState();
		static_cast<GetAuditStateActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(auditKey(type, id));
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<GetAuditStateActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetAuditStateActor*>(this)->actor_wait_state = 1;
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetAuditStateActor, 0, Optional<Value> >*>(static_cast<GetAuditStateActor*>(this)));
															#line 3951 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevDebug, "AuditUtilReadAuditStateError", id) .errorUnsuppressed(e) .detail("AuditID", id) .detail("AuditType", type) .detail("AuditKey", auditKey(type, id));
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.onError(e);
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<GetAuditStateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetAuditStateActor*>(this)->actor_wait_state = 2;
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetAuditStateActor, 1, Void >*>(static_cast<GetAuditStateActor*>(this)));
															#line 3995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& res_,int loopDepth) 
	{
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		res = res_;
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevDebug, "AuditUtilReadAuditState", id) .detail("AuditID", id) .detail("AuditType", type) .detail("AuditKey", auditKey(type, id));
															#line 4012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && res_,int loopDepth) 
	{
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		res = res_;
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevDebug, "AuditUtilReadAuditState", id) .detail("AuditID", id) .detail("AuditType", type) .detail("AuditKey", auditKey(type, id));
															#line 4023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& res_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(res_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && res_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(res_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetAuditStateActor*>(this)->actor_wait_state > 0) static_cast<GetAuditStateActor*>(this)->actor_wait_state = 0;
		static_cast<GetAuditStateActor*>(this)->ActorCallback< GetAuditStateActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< GetAuditStateActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("getAuditState", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditState", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetAuditStateActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("getAuditState", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditState", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetAuditStateActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("getAuditState", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditState", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<GetAuditStateActor*>(this)->actor_wait_state > 0) static_cast<GetAuditStateActor*>(this)->actor_wait_state = 0;
		static_cast<GetAuditStateActor*>(this)->ActorCallback< GetAuditStateActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetAuditStateActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getAuditState", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditState", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetAuditStateActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getAuditState", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditState", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetAuditStateActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getAuditState", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditState", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Database cx;
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditType type;
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	UID id;
															#line 446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Transaction tr;
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Optional<Value> res;
															#line 4176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via getAuditState()
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class GetAuditStateActor final : public Actor<AuditStorageState>, public ActorCallback< GetAuditStateActor, 0, Optional<Value> >, public ActorCallback< GetAuditStateActor, 1, Void >, public FastAllocated<GetAuditStateActor>, public GetAuditStateActorState<GetAuditStateActor> {
															#line 4181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<GetAuditStateActor>::operator new;
	using FastAllocated<GetAuditStateActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<AuditStorageState>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetAuditStateActor, 0, Optional<Value> >;
friend struct ActorCallback< GetAuditStateActor, 1, Void >;
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	GetAuditStateActor(Database const& cx,AuditType const& type,UID const& id) 
															#line 4193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<AuditStorageState>(),
		   GetAuditStateActorState<GetAuditStateActor>(cx, type, id)
	{
		fdb_probe_actor_enter("getAuditState", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getAuditState");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getAuditState", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetAuditStateActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetAuditStateActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<AuditStorageState> getAuditState( Database const& cx, AuditType const& type, UID const& id ) {
															#line 445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<AuditStorageState>(new GetAuditStateActor(cx, type, id));
															#line 4222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

															#line 4227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via persistAuditStateByRange()
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class PersistAuditStateByRangeActor>
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class PersistAuditStateByRangeActorState {
															#line 4234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	PersistAuditStateByRangeActorState(Database const& cx,AuditStorageState const& auditState) 
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : cx(cx),
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditState(auditState),
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   tr(cx)
															#line 4245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("persistAuditStateByRange", reinterpret_cast<unsigned long>(this));

	}
	~PersistAuditStateByRangeActorState() 
	{
		fdb_probe_actor_destroy("persistAuditStateByRange", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			;
															#line 4260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~PersistAuditStateByRangeActorState();
		static_cast<PersistAuditStateByRangeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<PersistAuditStateByRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~PersistAuditStateByRangeActorState(); static_cast<PersistAuditStateByRangeActor*>(this)->destroy(); return 0; }
															#line 4283 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<PersistAuditStateByRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~PersistAuditStateByRangeActorState();
		static_cast<PersistAuditStateByRangeActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(auditKey(auditState.getType(), auditState.id));
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state = 1;
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateByRangeActor, 0, Optional<Value> >*>(static_cast<PersistAuditStateByRangeActor*>(this)));
															#line 4316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevDebug, "AuditUtilPersistAuditStateByRangeError") .errorUnsuppressed(e) .detail("AuditID", auditState.id) .detail("AuditType", auditState.getType()) .detail("AuditPhase", auditState.getPhase());
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_3 = tr.onError(e);
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state = 4;
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateByRangeActor, 3, Void >*>(static_cast<PersistAuditStateByRangeActor*>(this)));
															#line 4360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& ddAuditState_,int loopDepth) 
	{
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!ddAuditState_.present())
															#line 4375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1loopBody1Catch1(audit_storage_cancelled(), loopDepth);
															#line 4379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		AuditStorageState ddAuditState = decodeAuditStorageState(ddAuditState_.get());
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		ASSERT(ddAuditState.ddId.isValid());
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (ddAuditState.ddId != auditState.ddId)
															#line 4387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1loopBody1Catch1(audit_storage_task_outdated(), loopDepth);
															#line 4391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (ddAuditState.getPhase() == AuditPhase::Complete)
															#line 4395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		ASSERT(ddAuditState.getPhase() == AuditPhase::Running || ddAuditState.getPhase() == AuditPhase::Failed);
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (ddAuditState.getPhase() == AuditPhase::Failed)
															#line 4403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1loopBody1Catch1(audit_storage_cancelled(), loopDepth);
															#line 4407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_1 = krmSetRange(&tr, auditRangeBasedProgressPrefixFor(auditState.getType(), auditState.id), auditState.range, auditStorageStateValue(auditState));
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state = 2;
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateByRangeActor, 1, Void >*>(static_cast<PersistAuditStateByRangeActor*>(this)));
															#line 4418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && ddAuditState_,int loopDepth) 
	{
															#line 487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!ddAuditState_.present())
															#line 4427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1loopBody1Catch1(audit_storage_cancelled(), loopDepth);
															#line 4431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		AuditStorageState ddAuditState = decodeAuditStorageState(ddAuditState_.get());
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		ASSERT(ddAuditState.ddId.isValid());
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (ddAuditState.ddId != auditState.ddId)
															#line 4439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1loopBody1Catch1(audit_storage_task_outdated(), loopDepth);
															#line 4443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (ddAuditState.getPhase() == AuditPhase::Complete)
															#line 4447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		ASSERT(ddAuditState.getPhase() == AuditPhase::Running || ddAuditState.getPhase() == AuditPhase::Failed);
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (ddAuditState.getPhase() == AuditPhase::Failed)
															#line 4455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1loopBody1Catch1(audit_storage_cancelled(), loopDepth);
															#line 4459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_1 = krmSetRange(&tr, auditRangeBasedProgressPrefixFor(auditState.getType(), auditState.id), auditState.range, auditStorageStateValue(auditState));
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state = 2;
															#line 506 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateByRangeActor, 1, Void >*>(static_cast<PersistAuditStateByRangeActor*>(this)));
															#line 4470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& ddAuditState_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(ddAuditState_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && ddAuditState_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(ddAuditState_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state > 0) static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state = 0;
		static_cast<PersistAuditStateByRangeActor*>(this)->ActorCallback< PersistAuditStateByRangeActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByRangeActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByRangeActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< PersistAuditStateByRangeActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state = 3;
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateByRangeActor, 2, Void >*>(static_cast<PersistAuditStateByRangeActor*>(this)));
															#line 4549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state = 3;
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateByRangeActor, 2, Void >*>(static_cast<PersistAuditStateByRangeActor*>(this)));
															#line 4565 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		if (static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state > 0) static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state = 0;
		static_cast<PersistAuditStateByRangeActor*>(this)->ActorCallback< PersistAuditStateByRangeActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByRangeActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByRangeActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< PersistAuditStateByRangeActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont8(Void const& _,int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void && _,int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

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
		if (static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state > 0) static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state = 0;
		static_cast<PersistAuditStateByRangeActor*>(this)->ActorCallback< PersistAuditStateByRangeActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByRangeActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByRangeActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< PersistAuditStateByRangeActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 2);

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
		if (static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state > 0) static_cast<PersistAuditStateByRangeActor*>(this)->actor_wait_state = 0;
		static_cast<PersistAuditStateByRangeActor*>(this)->ActorCallback< PersistAuditStateByRangeActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByRangeActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByRangeActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< PersistAuditStateByRangeActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Database cx;
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditStorageState auditState;
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Transaction tr;
															#line 4789 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via persistAuditStateByRange()
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class PersistAuditStateByRangeActor final : public Actor<Void>, public ActorCallback< PersistAuditStateByRangeActor, 0, Optional<Value> >, public ActorCallback< PersistAuditStateByRangeActor, 1, Void >, public ActorCallback< PersistAuditStateByRangeActor, 2, Void >, public ActorCallback< PersistAuditStateByRangeActor, 3, Void >, public FastAllocated<PersistAuditStateByRangeActor>, public PersistAuditStateByRangeActorState<PersistAuditStateByRangeActor> {
															#line 4794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<PersistAuditStateByRangeActor>::operator new;
	using FastAllocated<PersistAuditStateByRangeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< PersistAuditStateByRangeActor, 0, Optional<Value> >;
friend struct ActorCallback< PersistAuditStateByRangeActor, 1, Void >;
friend struct ActorCallback< PersistAuditStateByRangeActor, 2, Void >;
friend struct ActorCallback< PersistAuditStateByRangeActor, 3, Void >;
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	PersistAuditStateByRangeActor(Database const& cx,AuditStorageState const& auditState) 
															#line 4808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<Void>(),
		   PersistAuditStateByRangeActorState<PersistAuditStateByRangeActor>(cx, auditState)
	{
		fdb_probe_actor_enter("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("persistAuditStateByRange");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("persistAuditStateByRange", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< PersistAuditStateByRangeActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< PersistAuditStateByRangeActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< PersistAuditStateByRangeActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< PersistAuditStateByRangeActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<Void> persistAuditStateByRange( Database const& cx, AuditStorageState const& auditState ) {
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<Void>(new PersistAuditStateByRangeActor(cx, auditState));
															#line 4839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

															#line 4844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via getAuditStateByRange()
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class GetAuditStateByRangeActor>
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class GetAuditStateByRangeActorState {
															#line 4851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	GetAuditStateByRangeActorState(Database const& cx,AuditType const& type,UID const& auditId,KeyRange const& range) 
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : cx(cx),
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   type(type),
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditId(auditId),
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   range(range),
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditStates(),
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   tr(cx)
															#line 4868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("getAuditStateByRange", reinterpret_cast<unsigned long>(this));

	}
	~GetAuditStateByRangeActorState() 
	{
		fdb_probe_actor_destroy("getAuditStateByRange", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			;
															#line 4883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~GetAuditStateByRangeActorState();
		static_cast<GetAuditStateByRangeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		std::vector<AuditStorageState> res;
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		for(int i = 0;i < auditStates.size() - 1;++i) {
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			KeyRange currentRange = KeyRangeRef(auditStates[i].key, auditStates[i + 1].key);
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			AuditStorageState auditState(auditId, currentRange, type);
															#line 558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (!auditStates[i].value.empty())
															#line 4914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				AuditStorageState auditState_ = decodeAuditStorageState(auditStates[i].value);
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				auditState.setPhase(auditState_.getPhase());
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				auditState.error = auditState_.error;
															#line 4922 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			res.push_back(auditState);
															#line 4926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<GetAuditStateByRangeActor*>(this)->SAV<std::vector<AuditStorageState>>::futures) { (void)(res); this->~GetAuditStateByRangeActorState(); static_cast<GetAuditStateByRangeActor*>(this)->destroy(); return 0; }
															#line 4930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<GetAuditStateByRangeActor*>(this)->SAV< std::vector<AuditStorageState> >::value()) std::vector<AuditStorageState>(res);
		this->~GetAuditStateByRangeActorState();
		static_cast<GetAuditStateByRangeActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = krmGetRanges(&tr, auditRangeBasedProgressPrefixFor(type, auditId), range, CLIENT_KNOBS->KRM_GET_RANGE_LIMIT, CLIENT_KNOBS->KRM_GET_RANGE_LIMIT_BYTES);
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<GetAuditStateByRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetAuditStateByRangeActor*>(this)->actor_wait_state = 1;
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetAuditStateByRangeActor, 0, RangeResult >*>(static_cast<GetAuditStateByRangeActor*>(this)));
															#line 4963 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
															#line 545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevDebug, "AuditUtilGetAuditStateForRangeError").errorUnsuppressed(e).detail("AuditID", auditId);
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.onError(e);
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<GetAuditStateByRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetAuditStateByRangeActor*>(this)->actor_wait_state = 2;
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetAuditStateByRangeActor, 1, Void >*>(static_cast<GetAuditStateByRangeActor*>(this)));
															#line 5007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(RangeResult const& res_,int loopDepth) 
	{
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		auditStates = res_;
															#line 5022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont2(RangeResult && res_,int loopDepth) 
	{
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		auditStates = res_;
															#line 5031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& res_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(res_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && res_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(res_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetAuditStateByRangeActor*>(this)->actor_wait_state > 0) static_cast<GetAuditStateByRangeActor*>(this)->actor_wait_state = 0;
		static_cast<GetAuditStateByRangeActor*>(this)->ActorCallback< GetAuditStateByRangeActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetAuditStateByRangeActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getAuditStateByRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStateByRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetAuditStateByRangeActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getAuditStateByRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStateByRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetAuditStateByRangeActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getAuditStateByRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStateByRange", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<GetAuditStateByRangeActor*>(this)->actor_wait_state > 0) static_cast<GetAuditStateByRangeActor*>(this)->actor_wait_state = 0;
		static_cast<GetAuditStateByRangeActor*>(this)->ActorCallback< GetAuditStateByRangeActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetAuditStateByRangeActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getAuditStateByRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStateByRange", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetAuditStateByRangeActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getAuditStateByRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStateByRange", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetAuditStateByRangeActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getAuditStateByRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStateByRange", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Database cx;
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditType type;
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	UID auditId;
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	KeyRange range;
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	RangeResult auditStates;
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Transaction tr;
															#line 5186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via getAuditStateByRange()
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class GetAuditStateByRangeActor final : public Actor<std::vector<AuditStorageState>>, public ActorCallback< GetAuditStateByRangeActor, 0, RangeResult >, public ActorCallback< GetAuditStateByRangeActor, 1, Void >, public FastAllocated<GetAuditStateByRangeActor>, public GetAuditStateByRangeActorState<GetAuditStateByRangeActor> {
															#line 5191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<GetAuditStateByRangeActor>::operator new;
	using FastAllocated<GetAuditStateByRangeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<AuditStorageState>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetAuditStateByRangeActor, 0, RangeResult >;
friend struct ActorCallback< GetAuditStateByRangeActor, 1, Void >;
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	GetAuditStateByRangeActor(Database const& cx,AuditType const& type,UID const& auditId,KeyRange const& range) 
															#line 5203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<std::vector<AuditStorageState>>(),
		   GetAuditStateByRangeActorState<GetAuditStateByRangeActor>(cx, type, auditId, range)
	{
		fdb_probe_actor_enter("getAuditStateByRange", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getAuditStateByRange");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getAuditStateByRange", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetAuditStateByRangeActor, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetAuditStateByRangeActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<std::vector<AuditStorageState>> getAuditStateByRange( Database const& cx, AuditType const& type, UID const& auditId, KeyRange const& range ) {
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<std::vector<AuditStorageState>>(new GetAuditStateByRangeActor(cx, type, auditId, range));
															#line 5232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

															#line 5237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via persistAuditStateByServer()
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class PersistAuditStateByServerActor>
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class PersistAuditStateByServerActorState {
															#line 5244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	PersistAuditStateByServerActorState(Database const& cx,AuditStorageState const& auditState) 
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : cx(cx),
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditState(auditState),
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   tr(cx)
															#line 5255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("persistAuditStateByServer", reinterpret_cast<unsigned long>(this));

	}
	~PersistAuditStateByServerActorState() 
	{
		fdb_probe_actor_destroy("persistAuditStateByServer", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			;
															#line 5270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~PersistAuditStateByServerActorState();
		static_cast<PersistAuditStateByServerActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<PersistAuditStateByServerActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~PersistAuditStateByServerActorState(); static_cast<PersistAuditStateByServerActor*>(this)->destroy(); return 0; }
															#line 5293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<PersistAuditStateByServerActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~PersistAuditStateByServerActorState();
		static_cast<PersistAuditStateByServerActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Optional<Value>> __when_expr_0 = tr.get(auditKey(auditState.getType(), auditState.id));
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state = 1;
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateByServerActor, 0, Optional<Value> >*>(static_cast<PersistAuditStateByServerActor*>(this)));
															#line 5326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevDebug, "AuditUtilPersistAuditStateByRangeError") .errorUnsuppressed(e) .detail("AuditID", auditState.id) .detail("AuditType", auditState.getType()) .detail("AuditPhase", auditState.getPhase()) .detail("AuditServerID", auditState.auditServerId);
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_3 = tr.onError(e);
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state = 4;
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateByServerActor, 3, Void >*>(static_cast<PersistAuditStateByServerActor*>(this)));
															#line 5370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> const& ddAuditState_,int loopDepth) 
	{
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!ddAuditState_.present())
															#line 5385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1loopBody1Catch1(audit_storage_cancelled(), loopDepth);
															#line 5389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		AuditStorageState ddAuditState = decodeAuditStorageState(ddAuditState_.get());
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		ASSERT(ddAuditState.ddId.isValid());
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (ddAuditState.ddId != auditState.ddId)
															#line 5397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1loopBody1Catch1(audit_storage_task_outdated(), loopDepth);
															#line 5401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (ddAuditState.getPhase() == AuditPhase::Complete)
															#line 5405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		ASSERT(ddAuditState.getPhase() == AuditPhase::Running || ddAuditState.getPhase() == AuditPhase::Failed);
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (ddAuditState.getPhase() == AuditPhase::Failed)
															#line 5413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1loopBody1Catch1(audit_storage_cancelled(), loopDepth);
															#line 5417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_1 = krmSetRange( &tr, auditServerBasedProgressPrefixFor(auditState.getType(), auditState.id, auditState.auditServerId), auditState.range, auditStorageStateValue(auditState));
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state = 2;
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateByServerActor, 1, Void >*>(static_cast<PersistAuditStateByServerActor*>(this)));
															#line 5428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<Value> && ddAuditState_,int loopDepth) 
	{
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!ddAuditState_.present())
															#line 5437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1loopBody1Catch1(audit_storage_cancelled(), loopDepth);
															#line 5441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		AuditStorageState ddAuditState = decodeAuditStorageState(ddAuditState_.get());
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		ASSERT(ddAuditState.ddId.isValid());
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (ddAuditState.ddId != auditState.ddId)
															#line 5449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1loopBody1Catch1(audit_storage_task_outdated(), loopDepth);
															#line 5453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (ddAuditState.getPhase() == AuditPhase::Complete)
															#line 5457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		ASSERT(ddAuditState.getPhase() == AuditPhase::Running || ddAuditState.getPhase() == AuditPhase::Failed);
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (ddAuditState.getPhase() == AuditPhase::Failed)
															#line 5465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1loopBody1Catch1(audit_storage_cancelled(), loopDepth);
															#line 5469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_1 = krmSetRange( &tr, auditServerBasedProgressPrefixFor(auditState.getType(), auditState.id, auditState.auditServerId), auditState.range, auditStorageStateValue(auditState));
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state = 2;
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateByServerActor, 1, Void >*>(static_cast<PersistAuditStateByServerActor*>(this)));
															#line 5480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> const& ddAuditState_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(ddAuditState_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<Value> && ddAuditState_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(ddAuditState_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state > 0) static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state = 0;
		static_cast<PersistAuditStateByServerActor*>(this)->ActorCallback< PersistAuditStateByServerActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByServerActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByServerActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< PersistAuditStateByServerActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state = 3;
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateByServerActor, 2, Void >*>(static_cast<PersistAuditStateByServerActor*>(this)));
															#line 5559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state = 3;
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< PersistAuditStateByServerActor, 2, Void >*>(static_cast<PersistAuditStateByServerActor*>(this)));
															#line 5575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		if (static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state > 0) static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state = 0;
		static_cast<PersistAuditStateByServerActor*>(this)->ActorCallback< PersistAuditStateByServerActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByServerActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByServerActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< PersistAuditStateByServerActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont8(Void const& _,int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void && _,int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

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
		if (static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state > 0) static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state = 0;
		static_cast<PersistAuditStateByServerActor*>(this)->ActorCallback< PersistAuditStateByServerActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByServerActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByServerActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< PersistAuditStateByServerActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 2);

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
		if (static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state > 0) static_cast<PersistAuditStateByServerActor*>(this)->actor_wait_state = 0;
		static_cast<PersistAuditStateByServerActor*>(this)->ActorCallback< PersistAuditStateByServerActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByServerActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< PersistAuditStateByServerActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< PersistAuditStateByServerActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Database cx;
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditStorageState auditState;
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Transaction tr;
															#line 5799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via persistAuditStateByServer()
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class PersistAuditStateByServerActor final : public Actor<Void>, public ActorCallback< PersistAuditStateByServerActor, 0, Optional<Value> >, public ActorCallback< PersistAuditStateByServerActor, 1, Void >, public ActorCallback< PersistAuditStateByServerActor, 2, Void >, public ActorCallback< PersistAuditStateByServerActor, 3, Void >, public FastAllocated<PersistAuditStateByServerActor>, public PersistAuditStateByServerActorState<PersistAuditStateByServerActor> {
															#line 5804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<PersistAuditStateByServerActor>::operator new;
	using FastAllocated<PersistAuditStateByServerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< PersistAuditStateByServerActor, 0, Optional<Value> >;
friend struct ActorCallback< PersistAuditStateByServerActor, 1, Void >;
friend struct ActorCallback< PersistAuditStateByServerActor, 2, Void >;
friend struct ActorCallback< PersistAuditStateByServerActor, 3, Void >;
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	PersistAuditStateByServerActor(Database const& cx,AuditStorageState const& auditState) 
															#line 5818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<Void>(),
		   PersistAuditStateByServerActorState<PersistAuditStateByServerActor>(cx, auditState)
	{
		fdb_probe_actor_enter("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("persistAuditStateByServer");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("persistAuditStateByServer", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< PersistAuditStateByServerActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< PersistAuditStateByServerActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< PersistAuditStateByServerActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< PersistAuditStateByServerActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<Void> persistAuditStateByServer( Database const& cx, AuditStorageState const& auditState ) {
															#line 569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<Void>(new PersistAuditStateByServerActor(cx, auditState));
															#line 5849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

															#line 5854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via getAuditStateByServer()
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class GetAuditStateByServerActor>
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class GetAuditStateByServerActorState {
															#line 5861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	GetAuditStateByServerActorState(Database const& cx,AuditType const& type,UID const& auditId,UID const& auditServerId,KeyRange const& range) 
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : cx(cx),
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   type(type),
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditId(auditId),
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditServerId(auditServerId),
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   range(range),
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditStates(),
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   tr(cx)
															#line 5880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("getAuditStateByServer", reinterpret_cast<unsigned long>(this));

	}
	~GetAuditStateByServerActorState() 
	{
		fdb_probe_actor_destroy("getAuditStateByServer", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			;
															#line 5895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~GetAuditStateByServerActorState();
		static_cast<GetAuditStateByServerActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		std::vector<AuditStorageState> res;
															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		for(int i = 0;i < auditStates.size() - 1;++i) {
															#line 654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			KeyRange currentRange = KeyRangeRef(auditStates[i].key, auditStates[i + 1].key);
															#line 655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			AuditStorageState auditState(auditId, currentRange, type);
															#line 656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (!auditStates[i].value.empty())
															#line 5926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				AuditStorageState auditState_ = decodeAuditStorageState(auditStates[i].value);
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				auditState.setPhase(auditState_.getPhase());
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				auditState.error = auditState_.error;
															#line 5934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			res.push_back(auditState);
															#line 5938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<GetAuditStateByServerActor*>(this)->SAV<std::vector<AuditStorageState>>::futures) { (void)(res); this->~GetAuditStateByServerActorState(); static_cast<GetAuditStateByServerActor*>(this)->destroy(); return 0; }
															#line 5942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<GetAuditStateByServerActor*>(this)->SAV< std::vector<AuditStorageState> >::value()) std::vector<AuditStorageState>(res);
		this->~GetAuditStateByServerActorState();
		static_cast<GetAuditStateByServerActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = krmGetRanges(&tr, auditServerBasedProgressPrefixFor(type, auditId, auditServerId), range, CLIENT_KNOBS->KRM_GET_RANGE_LIMIT, CLIENT_KNOBS->KRM_GET_RANGE_LIMIT_BYTES);
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<GetAuditStateByServerActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetAuditStateByServerActor*>(this)->actor_wait_state = 1;
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetAuditStateByServerActor, 0, RangeResult >*>(static_cast<GetAuditStateByServerActor*>(this)));
															#line 5975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevDebug, "AuditUtilGetAuditStateForRangeError") .errorUnsuppressed(e) .detail("AuditID", auditId) .detail("AuditType", type) .detail("AuditServerID", auditServerId);
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_1 = tr.onError(e);
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<GetAuditStateByServerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetAuditStateByServerActor*>(this)->actor_wait_state = 2;
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetAuditStateByServerActor, 1, Void >*>(static_cast<GetAuditStateByServerActor*>(this)));
															#line 6019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(RangeResult const& res_,int loopDepth) 
	{
															#line 636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		auditStates = res_;
															#line 6034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont2(RangeResult && res_,int loopDepth) 
	{
															#line 636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		auditStates = res_;
															#line 6043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& res_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(res_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && res_,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(res_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetAuditStateByServerActor*>(this)->actor_wait_state > 0) static_cast<GetAuditStateByServerActor*>(this)->actor_wait_state = 0;
		static_cast<GetAuditStateByServerActor*>(this)->ActorCallback< GetAuditStateByServerActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< GetAuditStateByServerActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getAuditStateByServer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStateByServer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetAuditStateByServerActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getAuditStateByServer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStateByServer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetAuditStateByServerActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getAuditStateByServer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStateByServer", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<GetAuditStateByServerActor*>(this)->actor_wait_state > 0) static_cast<GetAuditStateByServerActor*>(this)->actor_wait_state = 0;
		static_cast<GetAuditStateByServerActor*>(this)->ActorCallback< GetAuditStateByServerActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetAuditStateByServerActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getAuditStateByServer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStateByServer", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetAuditStateByServerActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getAuditStateByServer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStateByServer", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetAuditStateByServerActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getAuditStateByServer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getAuditStateByServer", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Database cx;
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditType type;
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	UID auditId;
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	UID auditServerId;
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	KeyRange range;
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	RangeResult auditStates;
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Transaction tr;
															#line 6200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via getAuditStateByServer()
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class GetAuditStateByServerActor final : public Actor<std::vector<AuditStorageState>>, public ActorCallback< GetAuditStateByServerActor, 0, RangeResult >, public ActorCallback< GetAuditStateByServerActor, 1, Void >, public FastAllocated<GetAuditStateByServerActor>, public GetAuditStateByServerActorState<GetAuditStateByServerActor> {
															#line 6205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<GetAuditStateByServerActor>::operator new;
	using FastAllocated<GetAuditStateByServerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<AuditStorageState>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetAuditStateByServerActor, 0, RangeResult >;
friend struct ActorCallback< GetAuditStateByServerActor, 1, Void >;
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	GetAuditStateByServerActor(Database const& cx,AuditType const& type,UID const& auditId,UID const& auditServerId,KeyRange const& range) 
															#line 6217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<std::vector<AuditStorageState>>(),
		   GetAuditStateByServerActorState<GetAuditStateByServerActor>(cx, type, auditId, auditServerId, range)
	{
		fdb_probe_actor_enter("getAuditStateByServer", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getAuditStateByServer");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getAuditStateByServer", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetAuditStateByServerActor, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetAuditStateByServerActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<std::vector<AuditStorageState>> getAuditStateByServer( Database const& cx, AuditType const& type, UID const& auditId, UID const& auditServerId, KeyRange const& range ) {
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<std::vector<AuditStorageState>>(new GetAuditStateByServerActor(cx, type, auditId, auditServerId, range));
															#line 6246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

															#line 6251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via checkAuditProgressCompleteByRange()
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class CheckAuditProgressCompleteByRangeActor>
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class CheckAuditProgressCompleteByRangeActorState {
															#line 6258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	CheckAuditProgressCompleteByRangeActorState(Database const& cx,AuditType const& auditType,UID const& auditId,KeyRange const& auditRange) 
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : cx(cx),
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditType(auditType),
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditId(auditId),
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditRange(auditRange)
															#line 6271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this));

	}
	~CheckAuditProgressCompleteByRangeActorState() 
	{
		fdb_probe_actor_destroy("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			ASSERT(auditType == AuditType::ValidateHA || auditType == AuditType::ValidateReplica || auditType == AuditType::ValidateLocationMetadata);
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			rangeToRead = auditRange;
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			rangeToReadBegin = auditRange.begin;
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			retryCount = 0;
															#line 676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			;
															#line 6294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~CheckAuditProgressCompleteByRangeActorState();
		static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevInfo, "AuditUtilCheckAuditProgressFinish") .detail("AuditID", auditId) .detail("AuditRange", auditRange) .detail("AuditType", auditType);
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->SAV<bool>::futures) { (void)(true); this->~CheckAuditProgressCompleteByRangeActorState(); static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->destroy(); return 0; }
															#line 6319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->SAV< bool >::value()) bool(true);
		this->~CheckAuditProgressCompleteByRangeActorState();
		static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!(rangeToReadBegin < auditRange.end))
															#line 6338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		;
															#line 6344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = a_body1loopBody1loopHead1(loopDepth);

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
	int a_body1loopBody1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1(int loopDepth) 
	{
		try {
															#line 679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			rangeToRead = KeyRangeRef(rangeToReadBegin, auditRange.end);
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<std::vector<AuditStorageState>> __when_expr_0 = getAuditStateByRange(cx, auditType, auditId, rangeToRead);
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 6384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->actor_wait_state = 1;
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckAuditProgressCompleteByRangeActor, 0, std::vector<AuditStorageState> >*>(static_cast<CheckAuditProgressCompleteByRangeActor*>(this)));
															#line 6389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 6424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 2));
															#line 6428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (retryCount > 30)
															#line 6432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				TraceEvent(SevWarn, "AuditUtilCheckAuditProgressFailed") .detail("AuditID", auditId) .detail("AuditRange", auditRange) .detail("AuditType", auditType);
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				return a_body1Catch1(audit_storage_failed(), std::max(0, loopDepth - 2));
															#line 6438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_1 = delay(0.5);
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 6444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->actor_wait_state = 2;
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckAuditProgressCompleteByRangeActor, 1, Void >*>(static_cast<CheckAuditProgressCompleteByRangeActor*>(this)));
															#line 6449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 2));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 2));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont2(int loopDepth) 
	{
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		for(int i = 0;i < auditStates.size();i++) {
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			AuditPhase phase = auditStates[i].getPhase();
															#line 684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (phase == AuditPhase::Invalid)
															#line 6468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				TraceEvent(SevWarn, "AuditUtilCheckAuditProgressNotFinished") .detail("AuditID", auditId) .detail("AuditRange", auditRange) .detail("AuditType", auditType) .detail("UnfinishedRange", auditStates[i].range);
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (!static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckAuditProgressCompleteByRangeActorState(); static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->destroy(); return 0; }
															#line 6474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				new (&static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->SAV< bool >::value()) bool(false);
				this->~CheckAuditProgressCompleteByRangeActorState();
				static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		rangeToReadBegin = auditStates.back().range.end;
															#line 6483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(std::vector<AuditStorageState> const& __auditStates,int loopDepth) 
	{
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		auditStates = __auditStates;
															#line 6492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = a_body1loopBody1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(std::vector<AuditStorageState> && __auditStates,int loopDepth) 
	{
		auditStates = std::move(__auditStates);
		loopDepth = a_body1loopBody1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->actor_wait_state > 0) static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->actor_wait_state = 0;
		static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->ActorCallback< CheckAuditProgressCompleteByRangeActor, 0, std::vector<AuditStorageState> >::remove();

	}
	void a_callback_fire(ActorCallback< CheckAuditProgressCompleteByRangeActor, 0, std::vector<AuditStorageState> >*,std::vector<AuditStorageState> const& value) 
	{
		fdb_probe_actor_enter("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckAuditProgressCompleteByRangeActor, 0, std::vector<AuditStorageState> >*,std::vector<AuditStorageState> && value) 
	{
		fdb_probe_actor_enter("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckAuditProgressCompleteByRangeActor, 0, std::vector<AuditStorageState> >*,Error err) 
	{
		fdb_probe_actor_enter("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		retryCount++;
															#line 6559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = a_body1loopBody1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		retryCount++;
															#line 6568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = a_body1loopBody1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->actor_wait_state > 0) static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->actor_wait_state = 0;
		static_cast<CheckAuditProgressCompleteByRangeActor*>(this)->ActorCallback< CheckAuditProgressCompleteByRangeActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CheckAuditProgressCompleteByRangeActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CheckAuditProgressCompleteByRangeActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CheckAuditProgressCompleteByRangeActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Database cx;
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditType auditType;
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	UID auditId;
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	KeyRange auditRange;
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	KeyRange rangeToRead;
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Key rangeToReadBegin;
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	int retryCount;
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	std::vector<AuditStorageState> auditStates;
															#line 6652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via checkAuditProgressCompleteByRange()
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class CheckAuditProgressCompleteByRangeActor final : public Actor<bool>, public ActorCallback< CheckAuditProgressCompleteByRangeActor, 0, std::vector<AuditStorageState> >, public ActorCallback< CheckAuditProgressCompleteByRangeActor, 1, Void >, public FastAllocated<CheckAuditProgressCompleteByRangeActor>, public CheckAuditProgressCompleteByRangeActorState<CheckAuditProgressCompleteByRangeActor> {
															#line 6657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<CheckAuditProgressCompleteByRangeActor>::operator new;
	using FastAllocated<CheckAuditProgressCompleteByRangeActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckAuditProgressCompleteByRangeActor, 0, std::vector<AuditStorageState> >;
friend struct ActorCallback< CheckAuditProgressCompleteByRangeActor, 1, Void >;
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	CheckAuditProgressCompleteByRangeActor(Database const& cx,AuditType const& auditType,UID const& auditId,KeyRange const& auditRange) 
															#line 6669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<bool>(),
		   CheckAuditProgressCompleteByRangeActorState<CheckAuditProgressCompleteByRangeActor>(cx, auditType, auditId, auditRange)
	{
		fdb_probe_actor_enter("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkAuditProgressCompleteByRange");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkAuditProgressCompleteByRange", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckAuditProgressCompleteByRangeActor, 0, std::vector<AuditStorageState> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CheckAuditProgressCompleteByRangeActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<bool> checkAuditProgressCompleteByRange( Database const& cx, AuditType const& auditType, UID const& auditId, KeyRange const& auditRange ) {
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<bool>(new CheckAuditProgressCompleteByRangeActor(cx, auditType, auditId, auditRange));
															#line 6698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 717 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

															#line 6703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via checkAuditProgressCompleteByServer()
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class CheckAuditProgressCompleteByServerActor>
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class CheckAuditProgressCompleteByServerActorState {
															#line 6710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	CheckAuditProgressCompleteByServerActorState(Database const& cx,AuditType const& auditType,UID const& auditId,KeyRange const& auditRange,UID const& serverId,std::shared_ptr<AsyncVar<int>> const& checkProgressBudget) 
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : cx(cx),
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditType(auditType),
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditId(auditId),
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditRange(auditRange),
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   serverId(serverId),
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   checkProgressBudget(checkProgressBudget)
															#line 6727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this));

	}
	~CheckAuditProgressCompleteByServerActorState() 
	{
		fdb_probe_actor_destroy("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			ASSERT(auditType == AuditType::ValidateStorageServerShard);
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			rangeToRead = auditRange;
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			rangeToReadBegin = auditRange.begin;
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			retryCount = 0;
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			;
															#line 6750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~CheckAuditProgressCompleteByServerActorState();
		static_cast<CheckAuditProgressCompleteByServerActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		checkProgressBudget->set(checkProgressBudget->get() + 1);
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevInfo, "AuditUtilCheckAuditProgressFinish") .detail("ServerID", serverId) .detail("AuditID", auditId) .detail("AuditRange", auditRange) .detail("AuditType", auditType);
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<CheckAuditProgressCompleteByServerActor*>(this)->SAV<bool>::futures) { (void)(true); this->~CheckAuditProgressCompleteByServerActorState(); static_cast<CheckAuditProgressCompleteByServerActor*>(this)->destroy(); return 0; }
															#line 6777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<CheckAuditProgressCompleteByServerActor*>(this)->SAV< bool >::value()) bool(true);
		this->~CheckAuditProgressCompleteByServerActorState();
		static_cast<CheckAuditProgressCompleteByServerActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!(rangeToReadBegin < auditRange.end))
															#line 6796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		;
															#line 6802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = a_body1loopBody1loopHead1(loopDepth);

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
	int a_body1loopBody1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1(int loopDepth) 
	{
		try {
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			rangeToRead = KeyRangeRef(rangeToReadBegin, auditRange.end);
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<std::vector<AuditStorageState>> __when_expr_0 = getAuditStateByServer(cx, auditType, auditId, serverId, rangeToRead);
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<CheckAuditProgressCompleteByServerActor*>(this)->actor_wait_state < 0) return a_body1loopBody1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 6842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheckAuditProgressCompleteByServerActor*>(this)->actor_wait_state = 1;
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckAuditProgressCompleteByServerActor, 0, std::vector<AuditStorageState> >*>(static_cast<CheckAuditProgressCompleteByServerActor*>(this)));
															#line 6847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 6882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 2));
															#line 6886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
															#line 753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (retryCount > 30)
															#line 6890 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 754 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				TraceEvent(SevWarn, "AuditUtilCheckAuditProgressFailed") .detail("ServerID", serverId) .detail("AuditID", auditId) .detail("AuditRange", auditRange) .detail("AuditType", auditType);
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				checkProgressBudget->set(checkProgressBudget->get() + 1);
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				return a_body1Catch1(audit_storage_failed(), std::max(0, loopDepth - 2));
															#line 6898 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_1 = delay(0.5);
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<CheckAuditProgressCompleteByServerActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 6904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<CheckAuditProgressCompleteByServerActor*>(this)->actor_wait_state = 2;
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckAuditProgressCompleteByServerActor, 1, Void >*>(static_cast<CheckAuditProgressCompleteByServerActor*>(this)));
															#line 6909 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 2));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 2));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont2(int loopDepth) 
	{
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		for(int i = 0;i < auditStates.size();i++) {
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			AuditPhase phase = auditStates[i].getPhase();
															#line 736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (phase == AuditPhase::Invalid)
															#line 6928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 737 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				TraceEvent(SevWarn, "AuditUtilCheckAuditProgressNotFinished") .detail("ServerID", serverId) .detail("AuditID", auditId) .detail("AuditRange", auditRange) .detail("AuditType", auditType) .detail("UnfinishedRange", auditStates[i].range);
															#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				checkProgressBudget->set(checkProgressBudget->get() + 1);
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (!static_cast<CheckAuditProgressCompleteByServerActor*>(this)->SAV<bool>::futures) { (void)(false); this->~CheckAuditProgressCompleteByServerActorState(); static_cast<CheckAuditProgressCompleteByServerActor*>(this)->destroy(); return 0; }
															#line 6936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				new (&static_cast<CheckAuditProgressCompleteByServerActor*>(this)->SAV< bool >::value()) bool(false);
				this->~CheckAuditProgressCompleteByServerActorState();
				static_cast<CheckAuditProgressCompleteByServerActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		rangeToReadBegin = auditStates.back().range.end;
															#line 6945 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(std::vector<AuditStorageState> const& __auditStates,int loopDepth) 
	{
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		auditStates = __auditStates;
															#line 6954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = a_body1loopBody1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(std::vector<AuditStorageState> && __auditStates,int loopDepth) 
	{
		auditStates = std::move(__auditStates);
		loopDepth = a_body1loopBody1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CheckAuditProgressCompleteByServerActor*>(this)->actor_wait_state > 0) static_cast<CheckAuditProgressCompleteByServerActor*>(this)->actor_wait_state = 0;
		static_cast<CheckAuditProgressCompleteByServerActor*>(this)->ActorCallback< CheckAuditProgressCompleteByServerActor, 0, std::vector<AuditStorageState> >::remove();

	}
	void a_callback_fire(ActorCallback< CheckAuditProgressCompleteByServerActor, 0, std::vector<AuditStorageState> >*,std::vector<AuditStorageState> const& value) 
	{
		fdb_probe_actor_enter("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckAuditProgressCompleteByServerActor, 0, std::vector<AuditStorageState> >*,std::vector<AuditStorageState> && value) 
	{
		fdb_probe_actor_enter("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckAuditProgressCompleteByServerActor, 0, std::vector<AuditStorageState> >*,Error err) 
	{
		fdb_probe_actor_enter("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		retryCount++;
															#line 7021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = a_body1loopBody1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		retryCount++;
															#line 7030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = a_body1loopBody1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CheckAuditProgressCompleteByServerActor*>(this)->actor_wait_state > 0) static_cast<CheckAuditProgressCompleteByServerActor*>(this)->actor_wait_state = 0;
		static_cast<CheckAuditProgressCompleteByServerActor*>(this)->ActorCallback< CheckAuditProgressCompleteByServerActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CheckAuditProgressCompleteByServerActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CheckAuditProgressCompleteByServerActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CheckAuditProgressCompleteByServerActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Database cx;
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditType auditType;
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	UID auditId;
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	KeyRange auditRange;
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	UID serverId;
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	std::shared_ptr<AsyncVar<int>> checkProgressBudget;
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	KeyRange rangeToRead;
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Key rangeToReadBegin;
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	int retryCount;
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	std::vector<AuditStorageState> auditStates;
															#line 7118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via checkAuditProgressCompleteByServer()
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class CheckAuditProgressCompleteByServerActor final : public Actor<bool>, public ActorCallback< CheckAuditProgressCompleteByServerActor, 0, std::vector<AuditStorageState> >, public ActorCallback< CheckAuditProgressCompleteByServerActor, 1, Void >, public FastAllocated<CheckAuditProgressCompleteByServerActor>, public CheckAuditProgressCompleteByServerActorState<CheckAuditProgressCompleteByServerActor> {
															#line 7123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<CheckAuditProgressCompleteByServerActor>::operator new;
	using FastAllocated<CheckAuditProgressCompleteByServerActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckAuditProgressCompleteByServerActor, 0, std::vector<AuditStorageState> >;
friend struct ActorCallback< CheckAuditProgressCompleteByServerActor, 1, Void >;
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	CheckAuditProgressCompleteByServerActor(Database const& cx,AuditType const& auditType,UID const& auditId,KeyRange const& auditRange,UID const& serverId,std::shared_ptr<AsyncVar<int>> const& checkProgressBudget) 
															#line 7135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<bool>(),
		   CheckAuditProgressCompleteByServerActorState<CheckAuditProgressCompleteByServerActor>(cx, auditType, auditId, auditRange, serverId, checkProgressBudget)
	{
		fdb_probe_actor_enter("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkAuditProgressCompleteByServer");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkAuditProgressCompleteByServer", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckAuditProgressCompleteByServerActor, 0, std::vector<AuditStorageState> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CheckAuditProgressCompleteByServerActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<bool> checkAuditProgressCompleteByServer( Database const& cx, AuditType const& auditType, UID const& auditId, KeyRange const& auditRange, UID const& serverId, std::shared_ptr<AsyncVar<int>> const& checkProgressBudget ) {
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<bool>(new CheckAuditProgressCompleteByServerActor(cx, auditType, auditId, auditRange, serverId, checkProgressBudget));
															#line 7164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

// Load RUNNING audit states to resume, clean up COMPLETE and FAILED audit states
// Update ddId for RUNNING audit states
															#line 7171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via initAuditMetadata()
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class InitAuditMetadataActor>
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class InitAuditMetadataActorState {
															#line 7178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	InitAuditMetadataActorState(Database const& cx,MoveKeyLockInfo const& lock,bool const& ddEnabled,UID const& dataDistributorId,int const& persistFinishAuditCount) 
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : cx(cx),
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   lock(lock),
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   ddEnabled(ddEnabled),
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   dataDistributorId(dataDistributorId),
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   persistFinishAuditCount(persistFinishAuditCount),
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   existingAuditStates(),
															#line 784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   auditStatesToResume(),
															#line 785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   tr(cx),
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   retryCount(0)
															#line 7201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("initAuditMetadata", reinterpret_cast<unsigned long>(this));

	}
	~InitAuditMetadataActorState() 
	{
		fdb_probe_actor_destroy("initAuditMetadata", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			;
															#line 7216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~InitAuditMetadataActorState();
		static_cast<InitAuditMetadataActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<InitAuditMetadataActor*>(this)->SAV<std::vector<AuditStorageState>>::futures) { (void)(auditStatesToResume); this->~InitAuditMetadataActorState(); static_cast<InitAuditMetadataActor*>(this)->destroy(); return 0; }
															#line 7239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<InitAuditMetadataActor*>(this)->SAV< std::vector<AuditStorageState> >::value()) std::vector<AuditStorageState>(std::move(auditStatesToResume)); // state_var_RVO
		this->~InitAuditMetadataActorState();
		static_cast<InitAuditMetadataActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			existingAuditStates.clear();
															#line 791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			auditStatesToResume.clear();
															#line 792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 794 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			StrictFuture<Void> __when_expr_0 = checkMoveKeysLockForAudit(&tr, lock, ddEnabled, true);
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (static_cast<InitAuditMetadataActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<InitAuditMetadataActor*>(this)->actor_wait_state = 1;
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< InitAuditMetadataActor, 0, Void >*>(static_cast<InitAuditMetadataActor*>(this)));
															#line 7276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
															#line 870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (e.code() == error_code_actor_cancelled || e.code() == error_code_movekeys_conflict)
															#line 7311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 7315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
															#line 873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (retryCount > 50)
															#line 7319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				TraceEvent(SevWarnAlways, "AuditUtilInitAuditMetadataExceedRetryMax", dataDistributorId) .errorUnsuppressed(e);
															#line 7323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				return a_body1break1(loopDepth==0?0:loopDepth-1); // break
			}
			try {
															#line 879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				StrictFuture<Void> __when_expr_3 = tr.onError(e);
															#line 879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (static_cast<InitAuditMetadataActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1Catch1(actor_cancelled(), loopDepth);
															#line 7331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
				static_cast<InitAuditMetadataActor*>(this)->actor_wait_state = 4;
															#line 879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< InitAuditMetadataActor, 3, Void >*>(static_cast<InitAuditMetadataActor*>(this)));
															#line 7336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				loopDepth = 0;
			}
			catch (Error& error) {
				loopDepth = a_body1loopBody1Catch1Catch1(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1loopBody1Catch1Catch1(unknown_error(), loopDepth);
			}
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
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr.getRange(auditKeys, CLIENT_KNOBS->TOO_MANY);
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<InitAuditMetadataActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<InitAuditMetadataActor*>(this)->actor_wait_state = 2;
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< InitAuditMetadataActor, 1, RangeResult >*>(static_cast<InitAuditMetadataActor*>(this)));
															#line 7364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr.getRange(auditKeys, CLIENT_KNOBS->TOO_MANY);
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<InitAuditMetadataActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<InitAuditMetadataActor*>(this)->actor_wait_state = 2;
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< InitAuditMetadataActor, 1, RangeResult >*>(static_cast<InitAuditMetadataActor*>(this)));
															#line 7380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		if (static_cast<InitAuditMetadataActor*>(this)->actor_wait_state > 0) static_cast<InitAuditMetadataActor*>(this)->actor_wait_state = 0;
		static_cast<InitAuditMetadataActor*>(this)->ActorCallback< InitAuditMetadataActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< InitAuditMetadataActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("initAuditMetadata", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("initAuditMetadata", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< InitAuditMetadataActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("initAuditMetadata", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("initAuditMetadata", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< InitAuditMetadataActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("initAuditMetadata", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("initAuditMetadata", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(RangeResult const& result,int loopDepth) 
	{
															#line 797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (result.more || result.size() >= CLIENT_KNOBS->TOO_MANY)
															#line 7452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(g_network->isSimulated() ? SevError : SevWarnAlways, "AuditUtilLoadMetadataIncomplete", dataDistributorId) .detail("ResMore", result.more) .detail("ResSize", result.size());
															#line 7456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		for(int i = 0;i < result.size();++i) {
															#line 805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			auto auditState = decodeAuditStorageState(result[i].value);
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevVerbose, "AuditUtilLoadMetadataEach", dataDistributorId) .detail("CurrentDDID", dataDistributorId) .detail("AuditDDID", auditState.ddId) .detail("AuditType", auditState.getType()) .detail("AuditID", auditState.id) .detail("AuditPhase", auditState.getPhase());
															#line 812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (auditState.getPhase() == AuditPhase::Running)
															#line 7466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				AuditStorageState toUpdate = auditState;
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				toUpdate.ddId = dataDistributorId;
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				tr.set(auditKey(toUpdate.getType(), toUpdate.id), auditStorageStateValue(toUpdate));
															#line 7474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			existingAuditStates[auditState.getType()].push_back(auditState);
															#line 7478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		for( const auto& [auditType, _] : existingAuditStates ) {
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			int numFinishAudit = 0;
															#line 822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			for( const auto& auditState : existingAuditStates[auditType] ) {
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (auditState.getPhase() == AuditPhase::Complete || auditState.getPhase() == AuditPhase::Failed)
															#line 7488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				{
															#line 824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					numFinishAudit++;
															#line 7492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				}
			}
															#line 827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			const int numFinishAuditsToClear = numFinishAudit - persistFinishAuditCount;
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			int numFinishAuditsCleared = 0;
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			std::sort(existingAuditStates[auditType].begin(), existingAuditStates[auditType].end(), [](AuditStorageState a, AuditStorageState b) { return a.id < b.id; });
															#line 834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			for( const auto& auditState : existingAuditStates[auditType] ) {
															#line 835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (auditState.getPhase() == AuditPhase::Failed)
															#line 7505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				{
															#line 836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					if (numFinishAuditsCleared < numFinishAuditsToClear)
															#line 7509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
					{
															#line 838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
						tr.clear(auditKey(auditState.getType(), auditState.id));
															#line 839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
						clearAuditProgressMetadata(&tr, auditState.getType(), auditState.id);
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
						numFinishAuditsCleared++;
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
						TraceEvent(SevInfo, "AuditUtilMetadataCleared", dataDistributorId) .detail("AuditID", auditState.id) .detail("AuditType", auditState.getType()) .detail("AuditRange", auditState.range);
															#line 7519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
					}
				}
				else
				{
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					if (auditState.getPhase() == AuditPhase::Complete)
															#line 7526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
					{
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
						if (numFinishAuditsCleared < numFinishAuditsToClear)
															#line 7530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
						{
															#line 851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
							tr.clear(auditKey(auditState.getType(), auditState.id));
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
							numFinishAuditsCleared++;
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
							TraceEvent(SevInfo, "AuditUtilMetadataCleared", dataDistributorId) .detail("AuditID", auditState.id) .detail("AuditType", auditState.getType()) .detail("AuditRange", auditState.range);
															#line 7538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
						}
					}
					else
					{
															#line 858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
						if (auditState.getPhase() == AuditPhase::Running)
															#line 7545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
						{
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
							auditStatesToResume.push_back(auditState);
															#line 860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
							TraceEvent(SevInfo, "AuditUtilMetadataAddedToResume", dataDistributorId) .detail("AuditID", auditState.id) .detail("AuditType", auditState.getType()) .detail("AuditRange", auditState.range);
															#line 7551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
						}
					}
				}
			}
		}
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<InitAuditMetadataActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<InitAuditMetadataActor*>(this)->actor_wait_state = 3;
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< InitAuditMetadataActor, 2, Void >*>(static_cast<InitAuditMetadataActor*>(this)));
															#line 7566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(RangeResult && result,int loopDepth) 
	{
															#line 797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (result.more || result.size() >= CLIENT_KNOBS->TOO_MANY)
															#line 7575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(g_network->isSimulated() ? SevError : SevWarnAlways, "AuditUtilLoadMetadataIncomplete", dataDistributorId) .detail("ResMore", result.more) .detail("ResSize", result.size());
															#line 7579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		for(int i = 0;i < result.size();++i) {
															#line 805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			auto auditState = decodeAuditStorageState(result[i].value);
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevVerbose, "AuditUtilLoadMetadataEach", dataDistributorId) .detail("CurrentDDID", dataDistributorId) .detail("AuditDDID", auditState.ddId) .detail("AuditType", auditState.getType()) .detail("AuditID", auditState.id) .detail("AuditPhase", auditState.getPhase());
															#line 812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (auditState.getPhase() == AuditPhase::Running)
															#line 7589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				AuditStorageState toUpdate = auditState;
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				toUpdate.ddId = dataDistributorId;
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				tr.set(auditKey(toUpdate.getType(), toUpdate.id), auditStorageStateValue(toUpdate));
															#line 7597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			existingAuditStates[auditState.getType()].push_back(auditState);
															#line 7601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		for( const auto& [auditType, _] : existingAuditStates ) {
															#line 821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			int numFinishAudit = 0;
															#line 822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			for( const auto& auditState : existingAuditStates[auditType] ) {
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (auditState.getPhase() == AuditPhase::Complete || auditState.getPhase() == AuditPhase::Failed)
															#line 7611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				{
															#line 824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					numFinishAudit++;
															#line 7615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				}
			}
															#line 827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			const int numFinishAuditsToClear = numFinishAudit - persistFinishAuditCount;
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			int numFinishAuditsCleared = 0;
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			std::sort(existingAuditStates[auditType].begin(), existingAuditStates[auditType].end(), [](AuditStorageState a, AuditStorageState b) { return a.id < b.id; });
															#line 834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			for( const auto& auditState : existingAuditStates[auditType] ) {
															#line 835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (auditState.getPhase() == AuditPhase::Failed)
															#line 7628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				{
															#line 836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					if (numFinishAuditsCleared < numFinishAuditsToClear)
															#line 7632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
					{
															#line 838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
						tr.clear(auditKey(auditState.getType(), auditState.id));
															#line 839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
						clearAuditProgressMetadata(&tr, auditState.getType(), auditState.id);
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
						numFinishAuditsCleared++;
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
						TraceEvent(SevInfo, "AuditUtilMetadataCleared", dataDistributorId) .detail("AuditID", auditState.id) .detail("AuditType", auditState.getType()) .detail("AuditRange", auditState.range);
															#line 7642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
					}
				}
				else
				{
															#line 846 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
					if (auditState.getPhase() == AuditPhase::Complete)
															#line 7649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
					{
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
						if (numFinishAuditsCleared < numFinishAuditsToClear)
															#line 7653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
						{
															#line 851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
							tr.clear(auditKey(auditState.getType(), auditState.id));
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
							numFinishAuditsCleared++;
															#line 853 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
							TraceEvent(SevInfo, "AuditUtilMetadataCleared", dataDistributorId) .detail("AuditID", auditState.id) .detail("AuditType", auditState.getType()) .detail("AuditRange", auditState.range);
															#line 7661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
						}
					}
					else
					{
															#line 858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
						if (auditState.getPhase() == AuditPhase::Running)
															#line 7668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
						{
															#line 859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
							auditStatesToResume.push_back(auditState);
															#line 860 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
							TraceEvent(SevInfo, "AuditUtilMetadataAddedToResume", dataDistributorId) .detail("AuditID", auditState.id) .detail("AuditType", auditState.getType()) .detail("AuditRange", auditState.range);
															#line 7674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
						}
					}
				}
			}
		}
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		StrictFuture<Void> __when_expr_2 = tr.commit();
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (static_cast<InitAuditMetadataActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<InitAuditMetadataActor*>(this)->actor_wait_state = 3;
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< InitAuditMetadataActor, 2, Void >*>(static_cast<InitAuditMetadataActor*>(this)));
															#line 7689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(RangeResult const& result,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(result, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(RangeResult && result,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(result), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<InitAuditMetadataActor*>(this)->actor_wait_state > 0) static_cast<InitAuditMetadataActor*>(this)->actor_wait_state = 0;
		static_cast<InitAuditMetadataActor*>(this)->ActorCallback< InitAuditMetadataActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< InitAuditMetadataActor, 1, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("initAuditMetadata", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("initAuditMetadata", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< InitAuditMetadataActor, 1, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("initAuditMetadata", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("initAuditMetadata", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< InitAuditMetadataActor, 1, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("initAuditMetadata", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("initAuditMetadata", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

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
	void a_exitChoose3() 
	{
		if (static_cast<InitAuditMetadataActor*>(this)->actor_wait_state > 0) static_cast<InitAuditMetadataActor*>(this)->actor_wait_state = 0;
		static_cast<InitAuditMetadataActor*>(this)->ActorCallback< InitAuditMetadataActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< InitAuditMetadataActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("initAuditMetadata", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("initAuditMetadata", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< InitAuditMetadataActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("initAuditMetadata", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("initAuditMetadata", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< InitAuditMetadataActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("initAuditMetadata", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("initAuditMetadata", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1Catch1cont1(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			retryCount++;
															#line 882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			tr.reset();
															#line 7845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			loopDepth = a_body1loopBody1Catch1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<InitAuditMetadataActor*>(this)->actor_wait_state > 0) static_cast<InitAuditMetadataActor*>(this)->actor_wait_state = 0;
		static_cast<InitAuditMetadataActor*>(this)->ActorCallback< InitAuditMetadataActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< InitAuditMetadataActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("initAuditMetadata", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("initAuditMetadata", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< InitAuditMetadataActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("initAuditMetadata", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("initAuditMetadata", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< InitAuditMetadataActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("initAuditMetadata", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("initAuditMetadata", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1Catch1cont5(int loopDepth) 
	{
		try {
			loopDepth = a_body1loopBody1Catch1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Database cx;
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	MoveKeyLockInfo lock;
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	bool ddEnabled;
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	UID dataDistributorId;
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	int persistFinishAuditCount;
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	std::unordered_map<AuditType, std::vector<AuditStorageState>> existingAuditStates;
															#line 784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	std::vector<AuditStorageState> auditStatesToResume;
															#line 785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Transaction tr;
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	int retryCount;
															#line 7962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via initAuditMetadata()
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class InitAuditMetadataActor final : public Actor<std::vector<AuditStorageState>>, public ActorCallback< InitAuditMetadataActor, 0, Void >, public ActorCallback< InitAuditMetadataActor, 1, RangeResult >, public ActorCallback< InitAuditMetadataActor, 2, Void >, public ActorCallback< InitAuditMetadataActor, 3, Void >, public FastAllocated<InitAuditMetadataActor>, public InitAuditMetadataActorState<InitAuditMetadataActor> {
															#line 7967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<InitAuditMetadataActor>::operator new;
	using FastAllocated<InitAuditMetadataActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<AuditStorageState>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< InitAuditMetadataActor, 0, Void >;
friend struct ActorCallback< InitAuditMetadataActor, 1, RangeResult >;
friend struct ActorCallback< InitAuditMetadataActor, 2, Void >;
friend struct ActorCallback< InitAuditMetadataActor, 3, Void >;
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	InitAuditMetadataActor(Database const& cx,MoveKeyLockInfo const& lock,bool const& ddEnabled,UID const& dataDistributorId,int const& persistFinishAuditCount) 
															#line 7981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<std::vector<AuditStorageState>>(),
		   InitAuditMetadataActorState<InitAuditMetadataActor>(cx, lock, ddEnabled, dataDistributorId, persistFinishAuditCount)
	{
		fdb_probe_actor_enter("initAuditMetadata", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("initAuditMetadata");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("initAuditMetadata", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< InitAuditMetadataActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< InitAuditMetadataActor, 1, RangeResult >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< InitAuditMetadataActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< InitAuditMetadataActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<std::vector<AuditStorageState>> initAuditMetadata( Database const& cx, MoveKeyLockInfo const& lock, bool const& ddEnabled, UID const& dataDistributorId, int const& persistFinishAuditCount ) {
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<std::vector<AuditStorageState>>(new InitAuditMetadataActor(cx, lock, ddEnabled, dataDistributorId, persistFinishAuditCount));
															#line 8012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

// Check if any pair of ranges are exclusive with each other
// This is not a part in consistency check of audit metadata
// This is used for checking the validity of inputs to rangesSame()
bool elementsAreExclusiveWithEachOther(std::vector<KeyRange> ranges) {
	ASSERT(std::is_sorted(ranges.begin(), ranges.end(), KeyRangeRef::ArbitraryOrder()));
	for (int i = 0; i < ranges.size() - 1; ++i) {
		if (ranges[i].end > ranges[i + 1].begin) {
			TraceEvent(SevError, "AuditUtilElementsAreNotExclusiveWithEachOther").detail("Ranges", describe(ranges));
			return false;
		}
	}
	return true;
}

// Check if any range is empty in the given list of ranges
// This is not a part in consistency check of audit metadata
// This is used for checking the validity of inputs to rangesSame()
bool noEmptyRangeInRanges(std::vector<KeyRange> ranges) {
	for (const auto& range : ranges) {
		if (range.empty()) {
			return false;
		}
	}
	return true;
}

// Given a list of ranges, where ranges can overlap with each other
// Return a list of exclusive ranges which covers the ranges exactly
// the same as the input list of ranges
std::vector<KeyRange> coalesceRangeList(std::vector<KeyRange> ranges) {
	std::sort(ranges.begin(), ranges.end(), [](KeyRange a, KeyRange b) { return a.begin < b.begin; });
	std::vector<KeyRange> res;
	for (const auto& range : ranges) {
		if (res.empty()) {
			res.push_back(range);
			continue;
		}
		if (range.begin <= res.back().end) {
			if (range.end > res.back().end) { // update res.back if current range extends the back range
				KeyRange newBack = Standalone(KeyRangeRef(res.back().begin, range.end));
				res.pop_back();
				res.push_back(newBack);
			}
		} else {
			res.push_back(range);
		}
	}
	return res;
}

// Given two lists of ranges --- rangesA and rangesB, check if two lists are identical
// If not, return the mismatched two ranges of rangeA and rangeB respectively
Optional<std::pair<KeyRange, KeyRange>> rangesSame(std::vector<KeyRange> rangesA, std::vector<KeyRange> rangesB) {
	if (g_network->isSimulated()) {
		ASSERT(noEmptyRangeInRanges(rangesA));
		ASSERT(noEmptyRangeInRanges(rangesB));
	}
	KeyRange emptyRange;
	if (rangesA.empty() && rangesB.empty()) { // no mismatch
		return Optional<std::pair<KeyRange, KeyRange>>();
	} else if (rangesA.empty() && !rangesB.empty()) { // rangesA is empty while rangesB has a range
		return std::make_pair(emptyRange, rangesB.front());
	} else if (!rangesA.empty() && rangesB.empty()) { // rangesB is empty while rangesA has a range
		return std::make_pair(rangesA.front(), emptyRange);
	}
	TraceEvent(SevVerbose, "AuditUtilRangesSameBeforeSort").detail("RangesA", rangesA).detail("Rangesb", rangesB);
	// sort in ascending order
	std::sort(rangesA.begin(), rangesA.end(), [](KeyRange a, KeyRange b) { return a.begin < b.begin; });
	std::sort(rangesB.begin(), rangesB.end(), [](KeyRange a, KeyRange b) { return a.begin < b.begin; });
	TraceEvent(SevVerbose, "AuditUtilRangesSameAfterSort").detail("RangesA", rangesA).detail("Rangesb", rangesB);
	if (g_network->isSimulated()) {
		ASSERT(elementsAreExclusiveWithEachOther(rangesA));
		ASSERT(elementsAreExclusiveWithEachOther(rangesB));
	}
	if (rangesA.front().begin != rangesB.front().begin) { // rangeList heads mismatch
		return std::make_pair(rangesA.front(), rangesB.front());
	} else if (rangesA.back().end != rangesB.back().end) { // rangeList backs mismatch
		return std::make_pair(rangesA.back(), rangesB.back());
	}
	int ia = 0;
	int ib = 0;
	KeyRangeRef rangeA = rangesA[0];
	KeyRangeRef rangeB = rangesB[0];
	KeyRange lastRangeA = Standalone(rangeA);
	KeyRange lastRangeB = Standalone(rangeB);
	while (true) {
		if (rangeA.begin == rangeB.begin) {
			if (rangeA.end == rangeB.end) {
				if (rangeA.end == rangesA.back().end) {
					break;
				}
				++ia;
				++ib;
				rangeA = rangesA[ia];
				rangeB = rangesB[ib];
				lastRangeA = Standalone(rangeA);
				lastRangeB = Standalone(rangeB);
			} else if (rangeA.end > rangeB.end) {
				rangeA = KeyRangeRef(rangeB.end, rangeA.end);
				++ib;
				rangeB = rangesB[ib];
				lastRangeB = Standalone(rangeB);
			} else {
				rangeB = KeyRangeRef(rangeA.end, rangeB.end);
				++ia;
				rangeA = rangesA[ia];
				lastRangeA = Standalone(rangeA);
			}
		} else {
			return std::make_pair(lastRangeA, lastRangeB);
		}
	}
	return Optional<std::pair<KeyRange, KeyRange>>();
}

// Given an input server, get ranges within the input range via the input transaction
// from the perspective of ServerKeys system key space
// Input: (1) SS id; (2) transaction tr; (3) within range
// Return AuditGetServerKeysRes, including: (1) complete range by a single read range;
// (2) verison of the read; (3) ranges of the input SS
															#line 8137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via getThisServerKeysFromServerKeys()
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class GetThisServerKeysFromServerKeysActor>
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class GetThisServerKeysFromServerKeysActorState {
															#line 8144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	GetThisServerKeysFromServerKeysActorState(UID const& serverID,Transaction* const& tr,KeyRange const& range) 
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : serverID(serverID),
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   tr(tr),
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   range(range),
															#line 1010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   readResult(),
															#line 1011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   res()
															#line 8159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("getThisServerKeysFromServerKeys", reinterpret_cast<unsigned long>(this));

	}
	~GetThisServerKeysFromServerKeysActorState() 
	{
		fdb_probe_actor_destroy("getThisServerKeysFromServerKeys", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				StrictFuture<Void> __when_expr_0 = store(readResult, krmGetRanges(tr, serverKeysPrefixFor(serverID), range, CLIENT_KNOBS->KRM_GET_RANGE_LIMIT, CLIENT_KNOBS->KRM_GET_RANGE_LIMIT_BYTES));
															#line 1014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (static_cast<GetThisServerKeysFromServerKeysActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 8177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetThisServerKeysFromServerKeysActor*>(this)->actor_wait_state = 1;
															#line 1014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetThisServerKeysFromServerKeysActor, 0, Void >*>(static_cast<GetThisServerKeysFromServerKeysActor*>(this)));
															#line 8182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~GetThisServerKeysFromServerKeysActorState();
		static_cast<GetThisServerKeysFromServerKeysActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1062 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<GetThisServerKeysFromServerKeysActor*>(this)->SAV<AuditGetServerKeysRes>::futures) { (void)(res); this->~GetThisServerKeysFromServerKeysActorState(); static_cast<GetThisServerKeysFromServerKeysActor*>(this)->destroy(); return 0; }
															#line 8211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<GetThisServerKeysFromServerKeysActor*>(this)->SAV< AuditGetServerKeysRes >::value()) AuditGetServerKeysRes(std::move(res)); // state_var_RVO
		this->~GetThisServerKeysFromServerKeysActorState();
		static_cast<GetThisServerKeysFromServerKeysActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevDebug, "AuditUtilGetThisServerKeysError", serverID) .errorUnsuppressed(e) .detail("AduitServerID", serverID);
															#line 1059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 8226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		Future<Version> grvF = tr->getReadVersion();
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!grvF.isReady())
															#line 8242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevWarnAlways, "AuditUtilReadServerKeysGRVError", serverID);
															#line 1023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1Catch2(audit_storage_cancelled(), loopDepth);
															#line 8248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 1025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		Version readAtVersion = grvF.get();
															#line 1027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevVerbose, "AuditUtilGetThisServerKeysFromServerKeysReadDone", serverID) .detail("Range", range) .detail("Prefix", serverKeysPrefixFor(serverID)) .detail("ResultSize", readResult.size()) .detail("AduitServerID", serverID);
															#line 1033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		std::vector<KeyRange> ownRanges;
															#line 1034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		for(int i = 0;i < readResult.size() - 1;++i) {
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevVerbose, "AuditUtilGetThisServerKeysFromServerKeysAddToResult", serverID) .detail("ValueIsServerKeysFalse", readResult[i].value == serverKeysFalse) .detail("ServerHasKey", serverHasKey(readResult[i].value)) .detail("Range", KeyRangeRef(readResult[i].key, readResult[i + 1].key)) .detail("AduitServerID", serverID);
															#line 1040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (serverHasKey(readResult[i].value))
															#line 8262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 1041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				KeyRange shardRange;
															#line 1042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				ownRanges.push_back(Standalone(KeyRangeRef(readResult[i].key, readResult[i + 1].key)));
															#line 8268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
		}
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		const KeyRange completeRange = Standalone(KeyRangeRef(range.begin, readResult.back().key));
															#line 1046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevVerbose, "AuditUtilGetThisServerKeysFromServerKeysEnd", serverID) .detail("AduitServerID", serverID) .detail("Range", range) .detail("Prefix", serverKeysPrefixFor(serverID)) .detail("ReadAtVersion", readAtVersion) .detail("CompleteRange", completeRange) .detail("ResultSize", ownRanges.size());
															#line 1053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		res = AuditGetServerKeysRes(completeRange, readAtVersion, serverID, ownRanges, readResult.logicalSize());
															#line 8277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		Future<Version> grvF = tr->getReadVersion();
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!grvF.isReady())
															#line 8288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 1022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevWarnAlways, "AuditUtilReadServerKeysGRVError", serverID);
															#line 1023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1Catch2(audit_storage_cancelled(), loopDepth);
															#line 8294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 1025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		Version readAtVersion = grvF.get();
															#line 1027 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevVerbose, "AuditUtilGetThisServerKeysFromServerKeysReadDone", serverID) .detail("Range", range) .detail("Prefix", serverKeysPrefixFor(serverID)) .detail("ResultSize", readResult.size()) .detail("AduitServerID", serverID);
															#line 1033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		std::vector<KeyRange> ownRanges;
															#line 1034 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		for(int i = 0;i < readResult.size() - 1;++i) {
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevVerbose, "AuditUtilGetThisServerKeysFromServerKeysAddToResult", serverID) .detail("ValueIsServerKeysFalse", readResult[i].value == serverKeysFalse) .detail("ServerHasKey", serverHasKey(readResult[i].value)) .detail("Range", KeyRangeRef(readResult[i].key, readResult[i + 1].key)) .detail("AduitServerID", serverID);
															#line 1040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (serverHasKey(readResult[i].value))
															#line 8308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 1041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				KeyRange shardRange;
															#line 1042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				ownRanges.push_back(Standalone(KeyRangeRef(readResult[i].key, readResult[i + 1].key)));
															#line 8314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
		}
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		const KeyRange completeRange = Standalone(KeyRangeRef(range.begin, readResult.back().key));
															#line 1046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevVerbose, "AuditUtilGetThisServerKeysFromServerKeysEnd", serverID) .detail("AduitServerID", serverID) .detail("Range", range) .detail("Prefix", serverKeysPrefixFor(serverID)) .detail("ReadAtVersion", readAtVersion) .detail("CompleteRange", completeRange) .detail("ResultSize", ownRanges.size());
															#line 1053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		res = AuditGetServerKeysRes(completeRange, readAtVersion, serverID, ownRanges, readResult.logicalSize());
															#line 8323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = a_body1cont7(loopDepth);

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
		if (static_cast<GetThisServerKeysFromServerKeysActor*>(this)->actor_wait_state > 0) static_cast<GetThisServerKeysFromServerKeysActor*>(this)->actor_wait_state = 0;
		static_cast<GetThisServerKeysFromServerKeysActor*>(this)->ActorCallback< GetThisServerKeysFromServerKeysActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetThisServerKeysFromServerKeysActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getThisServerKeysFromServerKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThisServerKeysFromServerKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetThisServerKeysFromServerKeysActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getThisServerKeysFromServerKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThisServerKeysFromServerKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetThisServerKeysFromServerKeysActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getThisServerKeysFromServerKeys", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getThisServerKeysFromServerKeys", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont7(int loopDepth) 
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
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	UID serverID;
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Transaction* tr;
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	KeyRange range;
															#line 1010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	RangeResult readResult;
															#line 1011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditGetServerKeysRes res;
															#line 8414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via getThisServerKeysFromServerKeys()
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class GetThisServerKeysFromServerKeysActor final : public Actor<AuditGetServerKeysRes>, public ActorCallback< GetThisServerKeysFromServerKeysActor, 0, Void >, public FastAllocated<GetThisServerKeysFromServerKeysActor>, public GetThisServerKeysFromServerKeysActorState<GetThisServerKeysFromServerKeysActor> {
															#line 8419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<GetThisServerKeysFromServerKeysActor>::operator new;
	using FastAllocated<GetThisServerKeysFromServerKeysActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<AuditGetServerKeysRes>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetThisServerKeysFromServerKeysActor, 0, Void >;
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	GetThisServerKeysFromServerKeysActor(UID const& serverID,Transaction* const& tr,KeyRange const& range) 
															#line 8430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<AuditGetServerKeysRes>(),
		   GetThisServerKeysFromServerKeysActorState<GetThisServerKeysFromServerKeysActor>(serverID, tr, range)
	{
		fdb_probe_actor_enter("getThisServerKeysFromServerKeys", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getThisServerKeysFromServerKeys");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getThisServerKeysFromServerKeys", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetThisServerKeysFromServerKeysActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<AuditGetServerKeysRes> getThisServerKeysFromServerKeys( UID const& serverID, Transaction* const& tr, KeyRange const& range ) {
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<AuditGetServerKeysRes>(new GetThisServerKeysFromServerKeysActor(serverID, tr, range));
															#line 8458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 1064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"

// Given an input server, get ranges within the input range via the input transaction
// from the perspective of KeyServers system key space
// Input: (1) Audit Server ID (for logging); (2) transaction tr; (3) within range
// Return AuditGetKeyServersRes, including : (1) complete range by a single read range; (2) verison of the read;
// (3) map between SSes and their ranges --- in KeyServers space, a range corresponds to multiple SSes
															#line 8468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
namespace {
// This generated class is to be used only via getShardMapFromKeyServers()
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
template <class GetShardMapFromKeyServersActor>
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class GetShardMapFromKeyServersActorState {
															#line 8475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	GetShardMapFromKeyServersActorState(UID const& auditServerId,Transaction* const& tr,KeyRange const& range) 
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		 : auditServerId(auditServerId),
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   tr(tr),
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   range(range),
															#line 1071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   res(),
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   actors(),
															#line 1073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   readResult(),
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   UIDtoTagMap(),
															#line 1075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   totalShardsCount(0),
															#line 1076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		   shardsInAnonymousPhysicalShardCount(0)
															#line 8498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
	{
		fdb_probe_actor_create("getShardMapFromKeyServers", reinterpret_cast<unsigned long>(this));

	}
	~GetShardMapFromKeyServersActorState() 
	{
		fdb_probe_actor_destroy("getShardMapFromKeyServers", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 1080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				actors.push_back(store(readResult, krmGetRanges(tr, keyServersPrefix, range, CLIENT_KNOBS->KRM_GET_RANGE_LIMIT, CLIENT_KNOBS->KRM_GET_RANGE_LIMIT_BYTES)));
															#line 1086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				actors.push_back(store(UIDtoTagMap, tr->getRange(serverTagKeys, CLIENT_KNOBS->TOO_MANY)));
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				StrictFuture<Void> __when_expr_0 = waitForAll(actors);
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				if (static_cast<GetShardMapFromKeyServersActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 8520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetShardMapFromKeyServersActor*>(this)->actor_wait_state = 1;
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetShardMapFromKeyServersActor, 0, Void >*>(static_cast<GetShardMapFromKeyServersActor*>(this)));
															#line 8525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
		this->~GetShardMapFromKeyServersActorState();
		static_cast<GetShardMapFromKeyServersActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!static_cast<GetShardMapFromKeyServersActor*>(this)->SAV<AuditGetKeyServersRes>::futures) { (void)(res); this->~GetShardMapFromKeyServersActorState(); static_cast<GetShardMapFromKeyServersActor*>(this)->destroy(); return 0; }
															#line 8554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		new (&static_cast<GetShardMapFromKeyServersActor*>(this)->SAV< AuditGetKeyServersRes >::value()) AuditGetKeyServersRes(std::move(res)); // state_var_RVO
		this->~GetShardMapFromKeyServersActorState();
		static_cast<GetShardMapFromKeyServersActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 1134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevDebug, "AuditUtilGetThisServerKeysFromKeyServersError", auditServerId) .errorUnsuppressed(e) .detail("AuditServerId", auditServerId);
															#line 1137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 8569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
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
															#line 1088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (UIDtoTagMap.more || UIDtoTagMap.size() >= CLIENT_KNOBS->TOO_MANY)
															#line 8583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 1089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(g_network->isSimulated() ? SevError : SevWarnAlways, "AuditUtilReadKeyServersReadTagError", auditServerId);
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1Catch2(audit_storage_cancelled(), loopDepth);
															#line 8589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 1094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		Future<Version> grvF = tr->getReadVersion();
															#line 1095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!grvF.isReady())
															#line 8595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 1096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevWarnAlways, "AuditUtilReadKeyServersGRVError", auditServerId);
															#line 1097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1Catch2(audit_storage_cancelled(), loopDepth);
															#line 8601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 1099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		Version readAtVersion = grvF.get();
															#line 1101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevVerbose, "AuditUtilGetThisServerKeysFromKeyServersReadDone", auditServerId) .detail("Range", range) .detail("ResultSize", readResult.size()) .detail("AduitServerID", auditServerId);
															#line 1107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		std::unordered_map<UID, std::vector<KeyRange>> serverOwnRanges;
															#line 1108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		for(int i = 0;i < readResult.size() - 1;++i) {
															#line 1109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			std::vector<UID> src;
															#line 1110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			std::vector<UID> dest;
															#line 1111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			UID srcID;
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			UID destID;
															#line 1113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			decodeKeyServersValue(UIDtoTagMap, readResult[i].value, src, dest, srcID, destID);
															#line 1114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (srcID == anonymousShardId)
															#line 8623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 1115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				shardsInAnonymousPhysicalShardCount++;
															#line 8627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
															#line 1117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			totalShardsCount++;
															#line 1118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			std::vector<UID> servers(src.size() + dest.size());
															#line 1119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			std::merge(src.begin(), src.end(), dest.begin(), dest.end(), servers.begin());
															#line 1120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			for( auto& ssid : servers ) {
															#line 1121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				serverOwnRanges[ssid].push_back(Standalone(KeyRangeRef(readResult[i].key, readResult[i + 1].key)));
															#line 8639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
		}
															#line 1124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		const KeyRange completeRange = Standalone(KeyRangeRef(range.begin, readResult.back().key));
															#line 1125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevInfo, "AuditUtilGetThisServerKeysFromKeyServersEnd", auditServerId) .detail("Range", range) .detail("CompleteRange", completeRange) .detail("AtVersion", readAtVersion) .detail("ShardsInAnonymousPhysicalShardCount", shardsInAnonymousPhysicalShardCount) .detail("TotalShardsCount", totalShardsCount);
															#line 1131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		res = AuditGetKeyServersRes(completeRange, readAtVersion, serverOwnRanges, readResult.logicalSize());
															#line 8648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 1088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (UIDtoTagMap.more || UIDtoTagMap.size() >= CLIENT_KNOBS->TOO_MANY)
															#line 8657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 1089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(g_network->isSimulated() ? SevError : SevWarnAlways, "AuditUtilReadKeyServersReadTagError", auditServerId);
															#line 1092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1Catch2(audit_storage_cancelled(), loopDepth);
															#line 8663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 1094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		Future<Version> grvF = tr->getReadVersion();
															#line 1095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		if (!grvF.isReady())
															#line 8669 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		{
															#line 1096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			TraceEvent(SevWarnAlways, "AuditUtilReadKeyServersGRVError", auditServerId);
															#line 1097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			return a_body1Catch2(audit_storage_cancelled(), loopDepth);
															#line 8675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		}
															#line 1099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		Version readAtVersion = grvF.get();
															#line 1101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevVerbose, "AuditUtilGetThisServerKeysFromKeyServersReadDone", auditServerId) .detail("Range", range) .detail("ResultSize", readResult.size()) .detail("AduitServerID", auditServerId);
															#line 1107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		std::unordered_map<UID, std::vector<KeyRange>> serverOwnRanges;
															#line 1108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		for(int i = 0;i < readResult.size() - 1;++i) {
															#line 1109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			std::vector<UID> src;
															#line 1110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			std::vector<UID> dest;
															#line 1111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			UID srcID;
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			UID destID;
															#line 1113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			decodeKeyServersValue(UIDtoTagMap, readResult[i].value, src, dest, srcID, destID);
															#line 1114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			if (srcID == anonymousShardId)
															#line 8697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			{
															#line 1115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				shardsInAnonymousPhysicalShardCount++;
															#line 8701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
															#line 1117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			totalShardsCount++;
															#line 1118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			std::vector<UID> servers(src.size() + dest.size());
															#line 1119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			std::merge(src.begin(), src.end(), dest.begin(), dest.end(), servers.begin());
															#line 1120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
			for( auto& ssid : servers ) {
															#line 1121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
				serverOwnRanges[ssid].push_back(Standalone(KeyRangeRef(readResult[i].key, readResult[i + 1].key)));
															#line 8713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
			}
		}
															#line 1124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		const KeyRange completeRange = Standalone(KeyRangeRef(range.begin, readResult.back().key));
															#line 1125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		TraceEvent(SevInfo, "AuditUtilGetThisServerKeysFromKeyServersEnd", auditServerId) .detail("Range", range) .detail("CompleteRange", completeRange) .detail("AtVersion", readAtVersion) .detail("ShardsInAnonymousPhysicalShardCount", shardsInAnonymousPhysicalShardCount) .detail("TotalShardsCount", totalShardsCount);
															#line 1131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
		res = AuditGetKeyServersRes(completeRange, readAtVersion, serverOwnRanges, readResult.logicalSize());
															#line 8722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		loopDepth = a_body1cont9(loopDepth);

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
		if (static_cast<GetShardMapFromKeyServersActor*>(this)->actor_wait_state > 0) static_cast<GetShardMapFromKeyServersActor*>(this)->actor_wait_state = 0;
		static_cast<GetShardMapFromKeyServersActor*>(this)->ActorCallback< GetShardMapFromKeyServersActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetShardMapFromKeyServersActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getShardMapFromKeyServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getShardMapFromKeyServers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetShardMapFromKeyServersActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getShardMapFromKeyServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getShardMapFromKeyServers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetShardMapFromKeyServersActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getShardMapFromKeyServers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getShardMapFromKeyServers", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont9(int loopDepth) 
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
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	UID auditServerId;
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	Transaction* tr;
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	KeyRange range;
															#line 1071 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	AuditGetKeyServersRes res;
															#line 1072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	std::vector<Future<Void>> actors;
															#line 1073 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	RangeResult readResult;
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	RangeResult UIDtoTagMap;
															#line 1075 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	int64_t totalShardsCount;
															#line 1076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	int64_t shardsInAnonymousPhysicalShardCount;
															#line 8821 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
};
// This generated class is to be used only via getShardMapFromKeyServers()
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
class GetShardMapFromKeyServersActor final : public Actor<AuditGetKeyServersRes>, public ActorCallback< GetShardMapFromKeyServersActor, 0, Void >, public FastAllocated<GetShardMapFromKeyServersActor>, public GetShardMapFromKeyServersActorState<GetShardMapFromKeyServersActor> {
															#line 8826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
public:
	using FastAllocated<GetShardMapFromKeyServersActor>::operator new;
	using FastAllocated<GetShardMapFromKeyServersActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<AuditGetKeyServersRes>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetShardMapFromKeyServersActor, 0, Void >;
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	GetShardMapFromKeyServersActor(UID const& auditServerId,Transaction* const& tr,KeyRange const& range) 
															#line 8837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
		 : Actor<AuditGetKeyServersRes>(),
		   GetShardMapFromKeyServersActorState<GetShardMapFromKeyServersActor>(auditServerId, tr, range)
	{
		fdb_probe_actor_enter("getShardMapFromKeyServers", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getShardMapFromKeyServers");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getShardMapFromKeyServers", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetShardMapFromKeyServersActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
[[nodiscard]] Future<AuditGetKeyServersRes> getShardMapFromKeyServers( UID const& auditServerId, Transaction* const& tr, KeyRange const& range ) {
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
	return Future<AuditGetKeyServersRes>(new GetShardMapFromKeyServersActor(auditServerId, tr, range));
															#line 8865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.g.cpp"
}

#line 1142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/AuditUtils.actor.cpp"
