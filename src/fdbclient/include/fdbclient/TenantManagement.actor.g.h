#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
/*
 * TenantManagement.actor.h
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
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_TENANT_MANAGEMENT_ACTOR_G_H)
#define FDBCLIENT_TENANT_MANAGEMENT_ACTOR_G_H
#include "fdbclient/TenantManagement.actor.g.h"
#elif !defined(FDBCLIENT_TENANT_MANAGEMENT_ACTOR_H)
#define FDBCLIENT_TENANT_MANAGEMENT_ACTOR_H

#include <algorithm>
#include <string>
#include <map>
#include "fdbclient/ClientBooleanParams.h"
#include "fdbclient/GenericTransactionHelper.h"
#include "fdbclient/Knobs.h"
#include "fdbclient/MetaclusterRegistration.h"
#include "fdbclient/SystemData.h"
#include "fdbclient/Tenant.h"
#include "flow/IRandom.h"
#include "flow/ThreadHelper.actor.h"
#include "flow/actorcompiler.h" // has to be last include

namespace TenantAPI {

static const int TENANT_ID_PREFIX_MIN_VALUE = 0;
static const int TENANT_ID_PREFIX_MAX_VALUE = 32767;

template <class Transaction>
Future<Optional<TenantMapEntry>> tryGetTenantTransaction(Transaction tr, int64_t tenantId) {
	tr->setOption(FDBTransactionOptions::RAW_ACCESS);
	return TenantMetadata::tenantMap().get(tr, tenantId);
}

															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via tryGetTenantTransaction()
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class TryGetTenantTransactionActor>
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class TryGetTenantTransactionActorState {
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TryGetTenantTransactionActorState(Transaction const& tr,TenantName const& name) 
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : tr(tr),
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   name(name)
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	~TryGetTenantTransactionActorState() 
	{
		fdb_probe_actor_destroy("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Optional<int64_t>> __when_expr_0 = TenantMetadata::tenantNameIndex().get(tr, name);
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<TryGetTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TryGetTenantTransactionActor*>(this)->actor_wait_state = 1;
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TryGetTenantTransactionActor, 0, Optional<int64_t> >*>(static_cast<TryGetTenantTransactionActor*>(this)));
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~TryGetTenantTransactionActorState();
		static_cast<TryGetTenantTransactionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<int64_t> const& tenantId,int loopDepth) 
	{
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tenantId.present())
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Optional<TenantMapEntry>> __when_expr_1 = TenantMetadata::tenantMap().get(tr, tenantId.get());
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<TryGetTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TryGetTenantTransactionActor*>(this)->actor_wait_state = 2;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TryGetTenantTransactionActor, 1, Optional<TenantMapEntry> >*>(static_cast<TryGetTenantTransactionActor*>(this)));
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (!static_cast<TryGetTenantTransactionActor*>(this)->SAV<Optional<TenantMapEntry>>::futures) { (void)(Optional<TenantMapEntry>()); this->~TryGetTenantTransactionActorState(); static_cast<TryGetTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			new (&static_cast<TryGetTenantTransactionActor*>(this)->SAV< Optional<TenantMapEntry> >::value()) Optional<TenantMapEntry>(Optional<TenantMapEntry>());
			this->~TryGetTenantTransactionActorState();
			static_cast<TryGetTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1cont1(Optional<int64_t> && tenantId,int loopDepth) 
	{
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tenantId.present())
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Optional<TenantMapEntry>> __when_expr_1 = TenantMetadata::tenantMap().get(tr, tenantId.get());
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<TryGetTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TryGetTenantTransactionActor*>(this)->actor_wait_state = 2;
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TryGetTenantTransactionActor, 1, Optional<TenantMapEntry> >*>(static_cast<TryGetTenantTransactionActor*>(this)));
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (!static_cast<TryGetTenantTransactionActor*>(this)->SAV<Optional<TenantMapEntry>>::futures) { (void)(Optional<TenantMapEntry>()); this->~TryGetTenantTransactionActorState(); static_cast<TryGetTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			new (&static_cast<TryGetTenantTransactionActor*>(this)->SAV< Optional<TenantMapEntry> >::value()) Optional<TenantMapEntry>(Optional<TenantMapEntry>());
			this->~TryGetTenantTransactionActorState();
			static_cast<TryGetTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1when1(Optional<int64_t> const& tenantId,int loopDepth) 
	{
		loopDepth = a_body1cont1(tenantId, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<int64_t> && tenantId,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(tenantId), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TryGetTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<TryGetTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<TryGetTenantTransactionActor*>(this)->ActorCallback< TryGetTenantTransactionActor, 0, Optional<int64_t> >::remove();

	}
	void a_callback_fire(ActorCallback< TryGetTenantTransactionActor, 0, Optional<int64_t> >*,Optional<int64_t> const& value) 
	{
		fdb_probe_actor_enter("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TryGetTenantTransactionActor, 0, Optional<int64_t> >*,Optional<int64_t> && value) 
	{
		fdb_probe_actor_enter("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TryGetTenantTransactionActor, 0, Optional<int64_t> >*,Error err) 
	{
		fdb_probe_actor_enter("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<TryGetTenantTransactionActor*>(this)->SAV<Optional<TenantMapEntry>>::futures) { (void)(entry); this->~TryGetTenantTransactionActorState(); static_cast<TryGetTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<TryGetTenantTransactionActor*>(this)->SAV< Optional<TenantMapEntry> >::value()) Optional<TenantMapEntry>(entry);
		this->~TryGetTenantTransactionActorState();
		static_cast<TryGetTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<TryGetTenantTransactionActor*>(this)->SAV<Optional<TenantMapEntry>>::futures) { (void)(entry); this->~TryGetTenantTransactionActorState(); static_cast<TryGetTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<TryGetTenantTransactionActor*>(this)->SAV< Optional<TenantMapEntry> >::value()) Optional<TenantMapEntry>(entry);
		this->~TryGetTenantTransactionActorState();
		static_cast<TryGetTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
		loopDepth = a_body1cont3(entry, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(entry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<TryGetTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<TryGetTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<TryGetTenantTransactionActor*>(this)->ActorCallback< TryGetTenantTransactionActor, 1, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< TryGetTenantTransactionActor, 1, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TryGetTenantTransactionActor, 1, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TryGetTenantTransactionActor, 1, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Transaction tr;
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantName name;
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via tryGetTenantTransaction()
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class TryGetTenantTransactionActor final : public Actor<Optional<TenantMapEntry>>, public ActorCallback< TryGetTenantTransactionActor<Transaction>, 0, Optional<int64_t> >, public ActorCallback< TryGetTenantTransactionActor<Transaction>, 1, Optional<TenantMapEntry> >, public FastAllocated<TryGetTenantTransactionActor<Transaction>>, public TryGetTenantTransactionActorState<Transaction, TryGetTenantTransactionActor<Transaction>> {
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<TryGetTenantTransactionActor<Transaction>>::operator new;
	using FastAllocated<TryGetTenantTransactionActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<TenantMapEntry>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TryGetTenantTransactionActor<Transaction>, 0, Optional<int64_t> >;
friend struct ActorCallback< TryGetTenantTransactionActor<Transaction>, 1, Optional<TenantMapEntry> >;
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TryGetTenantTransactionActor(Transaction const& tr,TenantName const& name) 
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<Optional<TenantMapEntry>>(),
		   TryGetTenantTransactionActorState<Transaction, TryGetTenantTransactionActor<Transaction>>(tr, name)
	{
		fdb_probe_actor_enter("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tryGetTenantTransaction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TryGetTenantTransactionActor<Transaction>, 0, Optional<int64_t> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TryGetTenantTransactionActor<Transaction>, 1, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<Optional<TenantMapEntry>> tryGetTenantTransaction( Transaction const& tr, TenantName const& name ) {
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<Optional<TenantMapEntry>>(new TryGetTenantTransactionActor<Transaction>(tr, name));
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via tryGetTenant()
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB, class Tenant, class TryGetTenantActor>
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class TryGetTenantActorState {
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TryGetTenantActorState(Reference<DB> const& db,Tenant const& tenant) 
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : db(db),
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tenant(tenant),
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tr(db->createTransaction())
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("tryGetTenant", reinterpret_cast<unsigned long>(this));

	}
	~TryGetTenantActorState() 
	{
		fdb_probe_actor_destroy("tryGetTenant", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			;
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~TryGetTenantActorState();
		static_cast<TryGetTenantActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::READ_LOCK_AWARE);
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Optional<TenantMapEntry>> __when_expr_0 = tryGetTenantTransaction(tr, tenant);
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<TryGetTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TryGetTenantActor*>(this)->actor_wait_state = 1;
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TryGetTenantActor, 0, Optional<TenantMapEntry> >*>(static_cast<TryGetTenantActor*>(this)));
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->onError(e));
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<TryGetTenantActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TryGetTenantActor*>(this)->actor_wait_state = 2;
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TryGetTenantActor, 1, Void >*>(static_cast<TryGetTenantActor*>(this)));
															#line 485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<TryGetTenantActor*>(this)->SAV<Optional<TenantMapEntry>>::futures) { (void)(entry); this->~TryGetTenantActorState(); static_cast<TryGetTenantActor*>(this)->destroy(); return 0; }
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<TryGetTenantActor*>(this)->SAV< Optional<TenantMapEntry> >::value()) Optional<TenantMapEntry>(entry);
		this->~TryGetTenantActorState();
		static_cast<TryGetTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<TryGetTenantActor*>(this)->SAV<Optional<TenantMapEntry>>::futures) { (void)(entry); this->~TryGetTenantActorState(); static_cast<TryGetTenantActor*>(this)->destroy(); return 0; }
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<TryGetTenantActor*>(this)->SAV< Optional<TenantMapEntry> >::value()) Optional<TenantMapEntry>(entry);
		this->~TryGetTenantActorState();
		static_cast<TryGetTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(entry, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(entry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TryGetTenantActor*>(this)->actor_wait_state > 0) static_cast<TryGetTenantActor*>(this)->actor_wait_state = 0;
		static_cast<TryGetTenantActor*>(this)->ActorCallback< TryGetTenantActor, 0, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< TryGetTenantActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("tryGetTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TryGetTenantActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("tryGetTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TryGetTenantActor, 0, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("tryGetTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenant", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<TryGetTenantActor*>(this)->actor_wait_state > 0) static_cast<TryGetTenantActor*>(this)->actor_wait_state = 0;
		static_cast<TryGetTenantActor*>(this)->ActorCallback< TryGetTenantActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TryGetTenantActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tryGetTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TryGetTenantActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tryGetTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TryGetTenantActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tryGetTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenant", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Reference<DB> db;
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Tenant tenant;
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via tryGetTenant()
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB, class Tenant>
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class TryGetTenantActor final : public Actor<Optional<TenantMapEntry>>, public ActorCallback< TryGetTenantActor<DB, Tenant>, 0, Optional<TenantMapEntry> >, public ActorCallback< TryGetTenantActor<DB, Tenant>, 1, Void >, public FastAllocated<TryGetTenantActor<DB, Tenant>>, public TryGetTenantActorState<DB, Tenant, TryGetTenantActor<DB, Tenant>> {
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<TryGetTenantActor<DB, Tenant>>::operator new;
	using FastAllocated<TryGetTenantActor<DB, Tenant>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<TenantMapEntry>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TryGetTenantActor<DB, Tenant>, 0, Optional<TenantMapEntry> >;
friend struct ActorCallback< TryGetTenantActor<DB, Tenant>, 1, Void >;
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TryGetTenantActor(Reference<DB> const& db,Tenant const& tenant) 
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<Optional<TenantMapEntry>>(),
		   TryGetTenantActorState<DB, Tenant, TryGetTenantActor<DB, Tenant>>(db, tenant)
	{
		fdb_probe_actor_enter("tryGetTenant", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tryGetTenant");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tryGetTenant", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TryGetTenantActor<DB, Tenant>, 0, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TryGetTenantActor<DB, Tenant>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB, class Tenant>
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<Optional<TenantMapEntry>> tryGetTenant( Reference<DB> const& db, Tenant const& tenant ) {
															#line 64 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<Optional<TenantMapEntry>>(new TryGetTenantActor<DB, Tenant>(db, tenant));
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via getTenantTransaction()
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class Tenant, class GetTenantTransactionActor>
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class GetTenantTransactionActorState {
															#line 724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	GetTenantTransactionActorState(Transaction const& tr,Tenant const& tenant) 
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : tr(tr),
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tenant(tenant)
															#line 733 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("getTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	~GetTenantTransactionActorState() 
	{
		fdb_probe_actor_destroy("getTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Optional<TenantMapEntry>> __when_expr_0 = tryGetTenantTransaction(tr, tenant);
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<GetTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 750 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetTenantTransactionActor*>(this)->actor_wait_state = 1;
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetTenantTransactionActor, 0, Optional<TenantMapEntry> >*>(static_cast<GetTenantTransactionActor*>(this)));
															#line 755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~GetTenantTransactionActorState();
		static_cast<GetTenantTransactionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!entry.present())
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_not_found(), loopDepth);
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<GetTenantTransactionActor*>(this)->SAV<TenantMapEntry>::futures) { (void)(entry.get()); this->~GetTenantTransactionActorState(); static_cast<GetTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<GetTenantTransactionActor*>(this)->SAV< TenantMapEntry >::value()) TenantMapEntry(entry.get());
		this->~GetTenantTransactionActorState();
		static_cast<GetTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!entry.present())
															#line 798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_not_found(), loopDepth);
															#line 802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<GetTenantTransactionActor*>(this)->SAV<TenantMapEntry>::futures) { (void)(entry.get()); this->~GetTenantTransactionActorState(); static_cast<GetTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<GetTenantTransactionActor*>(this)->SAV< TenantMapEntry >::value()) TenantMapEntry(entry.get());
		this->~GetTenantTransactionActorState();
		static_cast<GetTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
		loopDepth = a_body1cont1(entry, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(entry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<GetTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<GetTenantTransactionActor*>(this)->ActorCallback< GetTenantTransactionActor, 0, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< GetTenantTransactionActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("getTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetTenantTransactionActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("getTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetTenantTransactionActor, 0, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("getTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Transaction tr;
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Tenant tenant;
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via getTenantTransaction()
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class Tenant>
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class GetTenantTransactionActor final : public Actor<TenantMapEntry>, public ActorCallback< GetTenantTransactionActor<Transaction, Tenant>, 0, Optional<TenantMapEntry> >, public FastAllocated<GetTenantTransactionActor<Transaction, Tenant>>, public GetTenantTransactionActorState<Transaction, Tenant, GetTenantTransactionActor<Transaction, Tenant>> {
															#line 888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<GetTenantTransactionActor<Transaction, Tenant>>::operator new;
	using FastAllocated<GetTenantTransactionActor<Transaction, Tenant>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<TenantMapEntry>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetTenantTransactionActor<Transaction, Tenant>, 0, Optional<TenantMapEntry> >;
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	GetTenantTransactionActor(Transaction const& tr,Tenant const& tenant) 
															#line 899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<TenantMapEntry>(),
		   GetTenantTransactionActorState<Transaction, Tenant, GetTenantTransactionActor<Transaction, Tenant>>(tr, tenant)
	{
		fdb_probe_actor_enter("getTenantTransaction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getTenantTransaction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getTenantTransaction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetTenantTransactionActor<Transaction, Tenant>, 0, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class Tenant>
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<TenantMapEntry> getTenantTransaction( Transaction const& tr, Tenant const& tenant ) {
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<TenantMapEntry>(new GetTenantTransactionActor<Transaction, Tenant>(tr, tenant));
															#line 928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

															#line 933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via getTenant()
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB, class Tenant, class GetTenantActor>
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class GetTenantActorState {
															#line 939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	GetTenantActorState(Reference<DB> const& db,Tenant const& tenant) 
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : db(db),
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tenant(tenant)
															#line 948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("getTenant", reinterpret_cast<unsigned long>(this));

	}
	~GetTenantActorState() 
	{
		fdb_probe_actor_destroy("getTenant", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Optional<TenantMapEntry>> __when_expr_0 = tryGetTenant(db, tenant);
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<GetTenantActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 965 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetTenantActor*>(this)->actor_wait_state = 1;
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetTenantActor, 0, Optional<TenantMapEntry> >*>(static_cast<GetTenantActor*>(this)));
															#line 970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~GetTenantActorState();
		static_cast<GetTenantActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!entry.present())
															#line 993 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_not_found(), loopDepth);
															#line 997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<GetTenantActor*>(this)->SAV<TenantMapEntry>::futures) { (void)(entry.get()); this->~GetTenantActorState(); static_cast<GetTenantActor*>(this)->destroy(); return 0; }
															#line 1001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<GetTenantActor*>(this)->SAV< TenantMapEntry >::value()) TenantMapEntry(entry.get());
		this->~GetTenantActorState();
		static_cast<GetTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!entry.present())
															#line 1013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_not_found(), loopDepth);
															#line 1017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<GetTenantActor*>(this)->SAV<TenantMapEntry>::futures) { (void)(entry.get()); this->~GetTenantActorState(); static_cast<GetTenantActor*>(this)->destroy(); return 0; }
															#line 1021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<GetTenantActor*>(this)->SAV< TenantMapEntry >::value()) TenantMapEntry(entry.get());
		this->~GetTenantActorState();
		static_cast<GetTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
		loopDepth = a_body1cont1(entry, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(entry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetTenantActor*>(this)->actor_wait_state > 0) static_cast<GetTenantActor*>(this)->actor_wait_state = 0;
		static_cast<GetTenantActor*>(this)->ActorCallback< GetTenantActor, 0, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< GetTenantActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("getTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetTenantActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("getTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetTenantActor, 0, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("getTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenant", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Reference<DB> db;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Tenant tenant;
															#line 1096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via getTenant()
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB, class Tenant>
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class GetTenantActor final : public Actor<TenantMapEntry>, public ActorCallback< GetTenantActor<DB, Tenant>, 0, Optional<TenantMapEntry> >, public FastAllocated<GetTenantActor<DB, Tenant>>, public GetTenantActorState<DB, Tenant, GetTenantActor<DB, Tenant>> {
															#line 1103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<GetTenantActor<DB, Tenant>>::operator new;
	using FastAllocated<GetTenantActor<DB, Tenant>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<TenantMapEntry>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetTenantActor<DB, Tenant>, 0, Optional<TenantMapEntry> >;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	GetTenantActor(Reference<DB> const& db,Tenant const& tenant) 
															#line 1114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<TenantMapEntry>(),
		   GetTenantActorState<DB, Tenant, GetTenantActor<DB, Tenant>>(db, tenant)
	{
		fdb_probe_actor_enter("getTenant", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getTenant");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getTenant", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetTenantActor<DB, Tenant>, 0, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB, class Tenant>
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<TenantMapEntry> getTenant( Reference<DB> const& db, Tenant const& tenant ) {
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<TenantMapEntry>(new GetTenantActor<DB, Tenant>(db, tenant));
															#line 1143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

															#line 1148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via getClusterType()
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class GetClusterTypeActor>
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class GetClusterTypeActorState {
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	GetClusterTypeActorState(Transaction const& tr) 
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : tr(tr)
															#line 1161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("getClusterType", reinterpret_cast<unsigned long>(this));

	}
	~GetClusterTypeActorState() 
	{
		fdb_probe_actor_destroy("getClusterType", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Optional<MetaclusterRegistrationEntry>> __when_expr_0 = metacluster::metadata::metaclusterRegistration().get(tr);
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<GetClusterTypeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetClusterTypeActor*>(this)->actor_wait_state = 1;
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetClusterTypeActor, 0, Optional<MetaclusterRegistrationEntry> >*>(static_cast<GetClusterTypeActor*>(this)));
															#line 1183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~GetClusterTypeActorState();
		static_cast<GetClusterTypeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<MetaclusterRegistrationEntry> const& metaclusterRegistration,int loopDepth) 
	{
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<GetClusterTypeActor*>(this)->SAV<ClusterType>::futures) { (void)(metaclusterRegistration.present() ? metaclusterRegistration.get().clusterType : ClusterType::STANDALONE); this->~GetClusterTypeActorState(); static_cast<GetClusterTypeActor*>(this)->destroy(); return 0; }
															#line 1206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<GetClusterTypeActor*>(this)->SAV< ClusterType >::value()) ClusterType(metaclusterRegistration.present() ? metaclusterRegistration.get().clusterType : ClusterType::STANDALONE);
		this->~GetClusterTypeActorState();
		static_cast<GetClusterTypeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<MetaclusterRegistrationEntry> && metaclusterRegistration,int loopDepth) 
	{
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<GetClusterTypeActor*>(this)->SAV<ClusterType>::futures) { (void)(metaclusterRegistration.present() ? metaclusterRegistration.get().clusterType : ClusterType::STANDALONE); this->~GetClusterTypeActorState(); static_cast<GetClusterTypeActor*>(this)->destroy(); return 0; }
															#line 1218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<GetClusterTypeActor*>(this)->SAV< ClusterType >::value()) ClusterType(metaclusterRegistration.present() ? metaclusterRegistration.get().clusterType : ClusterType::STANDALONE);
		this->~GetClusterTypeActorState();
		static_cast<GetClusterTypeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<MetaclusterRegistrationEntry> const& metaclusterRegistration,int loopDepth) 
	{
		loopDepth = a_body1cont1(metaclusterRegistration, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<MetaclusterRegistrationEntry> && metaclusterRegistration,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(metaclusterRegistration), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetClusterTypeActor*>(this)->actor_wait_state > 0) static_cast<GetClusterTypeActor*>(this)->actor_wait_state = 0;
		static_cast<GetClusterTypeActor*>(this)->ActorCallback< GetClusterTypeActor, 0, Optional<MetaclusterRegistrationEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< GetClusterTypeActor, 0, Optional<MetaclusterRegistrationEntry> >*,Optional<MetaclusterRegistrationEntry> const& value) 
	{
		fdb_probe_actor_enter("getClusterType", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getClusterType", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetClusterTypeActor, 0, Optional<MetaclusterRegistrationEntry> >*,Optional<MetaclusterRegistrationEntry> && value) 
	{
		fdb_probe_actor_enter("getClusterType", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getClusterType", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetClusterTypeActor, 0, Optional<MetaclusterRegistrationEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("getClusterType", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getClusterType", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Transaction tr;
															#line 1291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via getClusterType()
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class GetClusterTypeActor final : public Actor<ClusterType>, public ActorCallback< GetClusterTypeActor<Transaction>, 0, Optional<MetaclusterRegistrationEntry> >, public FastAllocated<GetClusterTypeActor<Transaction>>, public GetClusterTypeActorState<Transaction, GetClusterTypeActor<Transaction>> {
															#line 1298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<GetClusterTypeActor<Transaction>>::operator new;
	using FastAllocated<GetClusterTypeActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<ClusterType>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetClusterTypeActor<Transaction>, 0, Optional<MetaclusterRegistrationEntry> >;
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	GetClusterTypeActor(Transaction const& tr) 
															#line 1309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<ClusterType>(),
		   GetClusterTypeActorState<Transaction, GetClusterTypeActor<Transaction>>(tr)
	{
		fdb_probe_actor_enter("getClusterType", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getClusterType");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getClusterType", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetClusterTypeActor<Transaction>, 0, Optional<MetaclusterRegistrationEntry> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<ClusterType> getClusterType( Transaction const& tr ) {
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<ClusterType>(new GetClusterTypeActor<Transaction>(tr));
															#line 1338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

															#line 1343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via checkTenantMode()
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class CheckTenantModeActor>
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class CheckTenantModeActorState {
															#line 1349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	CheckTenantModeActorState(Transaction const& tr,ClusterType const& expectedClusterType) 
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : tr(tr),
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   expectedClusterType(expectedClusterType),
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tenantModeFuture(tr->get(configKeysPrefix.withSuffix("tenant_mode"_sr)))
															#line 1360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("checkTenantMode", reinterpret_cast<unsigned long>(this));

	}
	~CheckTenantModeActorState() 
	{
		fdb_probe_actor_destroy("checkTenantMode", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<ClusterType> __when_expr_0 = getClusterType(tr);
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<CheckTenantModeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheckTenantModeActor*>(this)->actor_wait_state = 1;
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckTenantModeActor, 0, ClusterType >*>(static_cast<CheckTenantModeActor*>(this)));
															#line 1382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~CheckTenantModeActorState();
		static_cast<CheckTenantModeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Optional<Value>> __when_expr_1 = safeThreadFutureToFuture(tenantModeFuture);
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<CheckTenantModeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CheckTenantModeActor*>(this)->actor_wait_state = 2;
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckTenantModeActor, 1, Optional<Value> >*>(static_cast<CheckTenantModeActor*>(this)));
															#line 1412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(ClusterType const& __actualClusterType,int loopDepth) 
	{
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		actualClusterType = __actualClusterType;
															#line 1421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(ClusterType && __actualClusterType,int loopDepth) 
	{
		actualClusterType = std::move(__actualClusterType);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CheckTenantModeActor*>(this)->actor_wait_state > 0) static_cast<CheckTenantModeActor*>(this)->actor_wait_state = 0;
		static_cast<CheckTenantModeActor*>(this)->ActorCallback< CheckTenantModeActor, 0, ClusterType >::remove();

	}
	void a_callback_fire(ActorCallback< CheckTenantModeActor, 0, ClusterType >*,ClusterType const& value) 
	{
		fdb_probe_actor_enter("checkTenantMode", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkTenantMode", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckTenantModeActor, 0, ClusterType >*,ClusterType && value) 
	{
		fdb_probe_actor_enter("checkTenantMode", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkTenantMode", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckTenantModeActor, 0, ClusterType >*,Error err) 
	{
		fdb_probe_actor_enter("checkTenantMode", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkTenantMode", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Optional<Value> const& tenantModeValue,int loopDepth) 
	{
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMode tenantMode = TenantMode::fromValue(tenantModeValue.castTo<ValueRef>());
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (actualClusterType != expectedClusterType)
															#line 1490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(invalid_metacluster_operation(), loopDepth);
															#line 1494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
		else
		{
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (actualClusterType == ClusterType::STANDALONE && tenantMode == TenantMode::DISABLED)
															#line 1500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				return a_body1Catch1(tenants_disabled(), loopDepth);
															#line 1504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			}
		}
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<CheckTenantModeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckTenantModeActorState(); static_cast<CheckTenantModeActor*>(this)->destroy(); return 0; }
															#line 1509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<CheckTenantModeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheckTenantModeActorState();
		static_cast<CheckTenantModeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Optional<Value> && tenantModeValue,int loopDepth) 
	{
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMode tenantMode = TenantMode::fromValue(tenantModeValue.castTo<ValueRef>());
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (actualClusterType != expectedClusterType)
															#line 1523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(invalid_metacluster_operation(), loopDepth);
															#line 1527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
		else
		{
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (actualClusterType == ClusterType::STANDALONE && tenantMode == TenantMode::DISABLED)
															#line 1533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				return a_body1Catch1(tenants_disabled(), loopDepth);
															#line 1537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			}
		}
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<CheckTenantModeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CheckTenantModeActorState(); static_cast<CheckTenantModeActor*>(this)->destroy(); return 0; }
															#line 1542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<CheckTenantModeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CheckTenantModeActorState();
		static_cast<CheckTenantModeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(Optional<Value> const& tenantModeValue,int loopDepth) 
	{
		loopDepth = a_body1cont2(tenantModeValue, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Optional<Value> && tenantModeValue,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(tenantModeValue), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CheckTenantModeActor*>(this)->actor_wait_state > 0) static_cast<CheckTenantModeActor*>(this)->actor_wait_state = 0;
		static_cast<CheckTenantModeActor*>(this)->ActorCallback< CheckTenantModeActor, 1, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< CheckTenantModeActor, 1, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("checkTenantMode", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkTenantMode", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CheckTenantModeActor, 1, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("checkTenantMode", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkTenantMode", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CheckTenantModeActor, 1, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("checkTenantMode", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkTenantMode", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Transaction tr;
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ClusterType expectedClusterType;
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	typename transaction_future_type<Transaction, Optional<Value>>::type tenantModeFuture;
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ClusterType actualClusterType;
															#line 1621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via checkTenantMode()
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class CheckTenantModeActor final : public Actor<Void>, public ActorCallback< CheckTenantModeActor<Transaction>, 0, ClusterType >, public ActorCallback< CheckTenantModeActor<Transaction>, 1, Optional<Value> >, public FastAllocated<CheckTenantModeActor<Transaction>>, public CheckTenantModeActorState<Transaction, CheckTenantModeActor<Transaction>> {
															#line 1628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<CheckTenantModeActor<Transaction>>::operator new;
	using FastAllocated<CheckTenantModeActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckTenantModeActor<Transaction>, 0, ClusterType >;
friend struct ActorCallback< CheckTenantModeActor<Transaction>, 1, Optional<Value> >;
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	CheckTenantModeActor(Transaction const& tr,ClusterType const& expectedClusterType) 
															#line 1640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<Void>(),
		   CheckTenantModeActorState<Transaction, CheckTenantModeActor<Transaction>>(tr, expectedClusterType)
	{
		fdb_probe_actor_enter("checkTenantMode", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkTenantMode");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkTenantMode", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckTenantModeActor<Transaction>, 0, ClusterType >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CheckTenantModeActor<Transaction>, 1, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<Void> checkTenantMode( Transaction const& tr, ClusterType const& expectedClusterType ) {
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<Void>(new CheckTenantModeActor<Transaction>(tr, expectedClusterType));
															#line 1670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

TenantMode tenantModeForClusterType(ClusterType clusterType, TenantMode tenantMode);
int64_t extractTenantIdFromMutation(MutationRef m);
int64_t extractTenantIdFromKeyRef(StringRef s);
bool tenantMapChanging(MutationRef const& mutation, KeyRangeRef const& tenantMapRange);
int64_t computeNextTenantId(int64_t tenantId, int64_t delta);
int64_t getMaxAllowableTenantId(int64_t curTenantId);
int64_t getTenantIdPrefix(int64_t tenantId);

// Returns true if the specified ID has already been deleted and false if not. If the ID is old enough
// that we no longer keep tombstones for it, an error is thrown.
															#line 1685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via checkTombstone()
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class CheckTombstoneActor>
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class CheckTombstoneActorState {
															#line 1691 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	CheckTombstoneActorState(Transaction const& tr,int64_t const& id) 
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : tr(tr),
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   id(id),
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tombstoneFuture(TenantMetadata::tenantTombstones().exists(tr, id))
															#line 1702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("checkTombstone", reinterpret_cast<unsigned long>(this));

	}
	~CheckTombstoneActorState() 
	{
		fdb_probe_actor_destroy("checkTombstone", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Optional<TenantTombstoneCleanupData>> __when_expr_0 = TenantMetadata::tombstoneCleanupData().get(tr);
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<CheckTombstoneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CheckTombstoneActor*>(this)->actor_wait_state = 1;
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CheckTombstoneActor, 0, Optional<TenantTombstoneCleanupData> >*>(static_cast<CheckTombstoneActor*>(this)));
															#line 1724 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~CheckTombstoneActorState();
		static_cast<CheckTombstoneActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<TenantTombstoneCleanupData> const& tombstoneCleanupData,int loopDepth) 
	{
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tombstoneCleanupData.present() && tombstoneCleanupData.get().tombstonesErasedThrough >= id)
															#line 1747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_creation_permanently_failed(), loopDepth);
															#line 1751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<bool> __when_expr_1 = tombstoneFuture;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<CheckTombstoneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CheckTombstoneActor*>(this)->actor_wait_state = 2;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckTombstoneActor, 1, bool >*>(static_cast<CheckTombstoneActor*>(this)));
															#line 1762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<TenantTombstoneCleanupData> && tombstoneCleanupData,int loopDepth) 
	{
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tombstoneCleanupData.present() && tombstoneCleanupData.get().tombstonesErasedThrough >= id)
															#line 1771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_creation_permanently_failed(), loopDepth);
															#line 1775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<bool> __when_expr_1 = tombstoneFuture;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<CheckTombstoneActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CheckTombstoneActor*>(this)->actor_wait_state = 2;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CheckTombstoneActor, 1, bool >*>(static_cast<CheckTombstoneActor*>(this)));
															#line 1786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Optional<TenantTombstoneCleanupData> const& tombstoneCleanupData,int loopDepth) 
	{
		loopDepth = a_body1cont1(tombstoneCleanupData, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<TenantTombstoneCleanupData> && tombstoneCleanupData,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(tombstoneCleanupData), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CheckTombstoneActor*>(this)->actor_wait_state > 0) static_cast<CheckTombstoneActor*>(this)->actor_wait_state = 0;
		static_cast<CheckTombstoneActor*>(this)->ActorCallback< CheckTombstoneActor, 0, Optional<TenantTombstoneCleanupData> >::remove();

	}
	void a_callback_fire(ActorCallback< CheckTombstoneActor, 0, Optional<TenantTombstoneCleanupData> >*,Optional<TenantTombstoneCleanupData> const& value) 
	{
		fdb_probe_actor_enter("checkTombstone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkTombstone", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CheckTombstoneActor, 0, Optional<TenantTombstoneCleanupData> >*,Optional<TenantTombstoneCleanupData> && value) 
	{
		fdb_probe_actor_enter("checkTombstone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkTombstone", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CheckTombstoneActor, 0, Optional<TenantTombstoneCleanupData> >*,Error err) 
	{
		fdb_probe_actor_enter("checkTombstone", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkTombstone", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<CheckTombstoneActor*>(this)->SAV<bool>::futures) { (void)(hasTombstone); this->~CheckTombstoneActorState(); static_cast<CheckTombstoneActor*>(this)->destroy(); return 0; }
															#line 1858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<CheckTombstoneActor*>(this)->SAV< bool >::value()) bool(std::move(hasTombstone)); // state_var_RVO
		this->~CheckTombstoneActorState();
		static_cast<CheckTombstoneActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(bool const& __hasTombstone,int loopDepth) 
	{
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		hasTombstone = __hasTombstone;
															#line 1870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(bool && __hasTombstone,int loopDepth) 
	{
		hasTombstone = std::move(__hasTombstone);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CheckTombstoneActor*>(this)->actor_wait_state > 0) static_cast<CheckTombstoneActor*>(this)->actor_wait_state = 0;
		static_cast<CheckTombstoneActor*>(this)->ActorCallback< CheckTombstoneActor, 1, bool >::remove();

	}
	void a_callback_fire(ActorCallback< CheckTombstoneActor, 1, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("checkTombstone", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkTombstone", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CheckTombstoneActor, 1, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("checkTombstone", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkTombstone", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CheckTombstoneActor, 1, bool >*,Error err) 
	{
		fdb_probe_actor_enter("checkTombstone", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("checkTombstone", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Transaction tr;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	int64_t id;
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Future<bool> tombstoneFuture;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	bool hasTombstone;
															#line 1941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via checkTombstone()
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class CheckTombstoneActor final : public Actor<bool>, public ActorCallback< CheckTombstoneActor<Transaction>, 0, Optional<TenantTombstoneCleanupData> >, public ActorCallback< CheckTombstoneActor<Transaction>, 1, bool >, public FastAllocated<CheckTombstoneActor<Transaction>>, public CheckTombstoneActorState<Transaction, CheckTombstoneActor<Transaction>> {
															#line 1948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<CheckTombstoneActor<Transaction>>::operator new;
	using FastAllocated<CheckTombstoneActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<bool>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CheckTombstoneActor<Transaction>, 0, Optional<TenantTombstoneCleanupData> >;
friend struct ActorCallback< CheckTombstoneActor<Transaction>, 1, bool >;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	CheckTombstoneActor(Transaction const& tr,int64_t const& id) 
															#line 1960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<bool>(),
		   CheckTombstoneActorState<Transaction, CheckTombstoneActor<Transaction>>(tr, id)
	{
		fdb_probe_actor_enter("checkTombstone", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("checkTombstone");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("checkTombstone", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CheckTombstoneActor<Transaction>, 0, Optional<TenantTombstoneCleanupData> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CheckTombstoneActor<Transaction>, 1, bool >*)0, actor_cancelled()); break;
		}

	}
};
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<bool> checkTombstone( Transaction const& tr, int64_t const& id ) {
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<bool>(new CheckTombstoneActor<Transaction>(tr, id));
															#line 1990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

// Creates a tenant. If the tenant already exists, the boolean return parameter will be false
// and the existing entry will be returned. If the tenant cannot be created, then the optional will be empty.
															#line 1997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via createTenantTransaction()
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class CreateTenantTransactionActor>
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class CreateTenantTransactionActorState {
															#line 2003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	CreateTenantTransactionActorState(Transaction const& tr,TenantMapEntry const& tenantEntry,ClusterType const& clusterType = ClusterType::STANDALONE) 
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : tr(tr),
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tenantEntry(tenantEntry),
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   clusterType(clusterType)
															#line 2014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("createTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	~CreateTenantTransactionActorState() 
	{
		fdb_probe_actor_destroy("createTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			ASSERT(clusterType != ClusterType::METACLUSTER_MANAGEMENT);
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			ASSERT(tenantEntry.id >= 0);
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (tenantEntry.tenantName.startsWith("\xff"_sr))
															#line 2033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				return a_body1Catch1(invalid_tenant_name(), loopDepth);
															#line 2037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			}
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (tenantEntry.tenantGroup.present() && tenantEntry.tenantGroup.get().startsWith("\xff"_sr))
															#line 2041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				return a_body1Catch1(invalid_tenant_group_name(), loopDepth);
															#line 2045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			}
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			existingEntryFuture = tryGetTenantTransaction(tr, tenantEntry.tenantName);
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tenantModeCheck = checkTenantMode(tr, clusterType);
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tombstoneFuture = (clusterType == ClusterType::STANDALONE) ? false : checkTombstone(tr, tenantEntry.id);
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			existingTenantGroupEntryFuture = Future<Optional<TenantGroupEntry>>();
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (tenantEntry.tenantGroup.present())
															#line 2059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				existingTenantGroupEntryFuture = TenantMetadata::tenantGroupMap().get(tr, tenantEntry.tenantGroup.get());
															#line 2063 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			}
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_0 = tenantModeCheck;
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 1;
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 0, Void >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 2074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~CreateTenantTransactionActorState();
		static_cast<CreateTenantTransactionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Optional<TenantMapEntry>> __when_expr_1 = existingEntryFuture;
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2099 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 2;
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 1, Optional<TenantMapEntry> >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 2104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Optional<TenantMapEntry>> __when_expr_1 = existingEntryFuture;
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 2;
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 1, Optional<TenantMapEntry> >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 2120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantTransactionActor*>(this)->ActorCallback< CreateTenantTransactionActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CreateTenantTransactionActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont5(Optional<TenantMapEntry> const& existingEntry,int loopDepth) 
	{
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (existingEntry.present())
															#line 2192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (!static_cast<CreateTenantTransactionActor*>(this)->SAV<std::pair<Optional<TenantMapEntry>, bool>>::futures) { (void)(std::make_pair(existingEntry.get(), false)); this->~CreateTenantTransactionActorState(); static_cast<CreateTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 2196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			new (&static_cast<CreateTenantTransactionActor*>(this)->SAV< std::pair<Optional<TenantMapEntry>, bool> >::value()) std::pair<Optional<TenantMapEntry>, bool>(std::make_pair(existingEntry.get(), false));
			this->~CreateTenantTransactionActorState();
			static_cast<CreateTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<bool> __when_expr_2 = tombstoneFuture;
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont5when1(__when_expr_2.get(), loopDepth); };
		static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 3;
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 2, bool >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 2211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5(Optional<TenantMapEntry> && existingEntry,int loopDepth) 
	{
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (existingEntry.present())
															#line 2220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (!static_cast<CreateTenantTransactionActor*>(this)->SAV<std::pair<Optional<TenantMapEntry>, bool>>::futures) { (void)(std::make_pair(existingEntry.get(), false)); this->~CreateTenantTransactionActorState(); static_cast<CreateTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 2224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			new (&static_cast<CreateTenantTransactionActor*>(this)->SAV< std::pair<Optional<TenantMapEntry>, bool> >::value()) std::pair<Optional<TenantMapEntry>, bool>(std::make_pair(existingEntry.get(), false));
			this->~CreateTenantTransactionActorState();
			static_cast<CreateTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<bool> __when_expr_2 = tombstoneFuture;
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont5when1(__when_expr_2.get(), loopDepth); };
		static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 3;
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 2, bool >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 2239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(Optional<TenantMapEntry> const& existingEntry,int loopDepth) 
	{
		loopDepth = a_body1cont5(existingEntry, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Optional<TenantMapEntry> && existingEntry,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(existingEntry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantTransactionActor*>(this)->ActorCallback< CreateTenantTransactionActor, 1, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 1, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 1, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CreateTenantTransactionActor, 1, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont6(int loopDepth) 
	{
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (hasTombstone)
															#line 2311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (!static_cast<CreateTenantTransactionActor*>(this)->SAV<std::pair<Optional<TenantMapEntry>, bool>>::futures) { (void)(std::make_pair(Optional<TenantMapEntry>(), false)); this->~CreateTenantTransactionActorState(); static_cast<CreateTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 2315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			new (&static_cast<CreateTenantTransactionActor*>(this)->SAV< std::pair<Optional<TenantMapEntry>, bool> >::value()) std::pair<Optional<TenantMapEntry>, bool>(std::make_pair(Optional<TenantMapEntry>(), false));
			this->~CreateTenantTransactionActorState();
			static_cast<CreateTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		prefixRangeFuture = tr->getRange(prefixRange(tenantEntry.prefix), 1);
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<RangeResult> __when_expr_3 = safeThreadFutureToFuture(prefixRangeFuture);
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 4;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 3, RangeResult >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 2332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5when1(bool const& __hasTombstone,int loopDepth) 
	{
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		hasTombstone = __hasTombstone;
															#line 2341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(bool && __hasTombstone,int loopDepth) 
	{
		hasTombstone = std::move(__hasTombstone);
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantTransactionActor*>(this)->ActorCallback< CreateTenantTransactionActor, 2, bool >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 2, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 2, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CreateTenantTransactionActor, 2, bool >*,Error err) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont8(RangeResult const& contents,int loopDepth) 
	{
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!contents.empty())
															#line 2408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_prefix_allocator_conflict(), loopDepth);
															#line 2412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantMap().set(tr, tenantEntry.id, tenantEntry);
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantNameIndex().set(tr, tenantEntry.tenantName, tenantEntry.id);
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::lastTenantModification().setVersionstamp(tr, Versionstamp(), 0);
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tenantEntry.tenantGroup.present())
															#line 2422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::tenantGroupTenantIndex().insert( tr, Tuple::makeTuple(tenantEntry.tenantGroup.get(), tenantEntry.id));
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Optional<TenantGroupEntry>> __when_expr_4 = existingTenantGroupEntryFuture;
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont8when1(__when_expr_4.get(), loopDepth); };
			static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 5;
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 4, Optional<TenantGroupEntry> >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 2435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont10(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont8(RangeResult && contents,int loopDepth) 
	{
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!contents.empty())
															#line 2449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_prefix_allocator_conflict(), loopDepth);
															#line 2453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantMap().set(tr, tenantEntry.id, tenantEntry);
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantNameIndex().set(tr, tenantEntry.tenantName, tenantEntry.id);
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::lastTenantModification().setVersionstamp(tr, Versionstamp(), 0);
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tenantEntry.tenantGroup.present())
															#line 2463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::tenantGroupTenantIndex().insert( tr, Tuple::makeTuple(tenantEntry.tenantGroup.get(), tenantEntry.id));
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Optional<TenantGroupEntry>> __when_expr_4 = existingTenantGroupEntryFuture;
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont8when1(__when_expr_4.get(), loopDepth); };
			static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 5;
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 4, Optional<TenantGroupEntry> >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 2476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont10(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont6when1(RangeResult const& contents,int loopDepth) 
	{
		loopDepth = a_body1cont8(contents, loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(RangeResult && contents,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(contents), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantTransactionActor*>(this)->ActorCallback< CreateTenantTransactionActor, 3, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 3, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 3, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< CreateTenantTransactionActor, 3, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont10(int loopDepth) 
	{
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantCount().atomicOp(tr, 1, MutationRef::AddValue);
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<int64_t> __when_expr_5 = TenantMetadata::tenantCount().getD(tr, Snapshot::False, 0);
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1cont10when1(__when_expr_5.get(), loopDepth); };
		static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 6;
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 5, int64_t >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 2562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont12(Optional<TenantGroupEntry> const& existingTenantGroup,int loopDepth) 
	{
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!existingTenantGroup.present())
															#line 2571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::tenantGroupMap().set(tr, tenantEntry.tenantGroup.get(), TenantGroupEntry());
															#line 2575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont12(Optional<TenantGroupEntry> && existingTenantGroup,int loopDepth) 
	{
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!existingTenantGroup.present())
															#line 2585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::tenantGroupMap().set(tr, tenantEntry.tenantGroup.get(), TenantGroupEntry());
															#line 2589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont8when1(Optional<TenantGroupEntry> const& existingTenantGroup,int loopDepth) 
	{
		loopDepth = a_body1cont12(existingTenantGroup, loopDepth);

		return loopDepth;
	}
	int a_body1cont8when1(Optional<TenantGroupEntry> && existingTenantGroup,int loopDepth) 
	{
		loopDepth = a_body1cont12(std::move(existingTenantGroup), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantTransactionActor*>(this)->ActorCallback< CreateTenantTransactionActor, 4, Optional<TenantGroupEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 4, Optional<TenantGroupEntry> >*,Optional<TenantGroupEntry> const& value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont8when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 4, Optional<TenantGroupEntry> >*,Optional<TenantGroupEntry> && value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont8when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< CreateTenantTransactionActor, 4, Optional<TenantGroupEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1cont10cont1(int64_t const& tenantCount,int loopDepth) 
	{
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tenantCount > CLIENT_KNOBS->MAX_TENANTS_PER_CLUSTER)
															#line 2662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(cluster_no_capacity(), loopDepth);
															#line 2666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<CreateTenantTransactionActor*>(this)->SAV<std::pair<Optional<TenantMapEntry>, bool>>::futures) { (void)(std::make_pair(tenantEntry, true)); this->~CreateTenantTransactionActorState(); static_cast<CreateTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 2670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<CreateTenantTransactionActor*>(this)->SAV< std::pair<Optional<TenantMapEntry>, bool> >::value()) std::pair<Optional<TenantMapEntry>, bool>(std::make_pair(tenantEntry, true));
		this->~CreateTenantTransactionActorState();
		static_cast<CreateTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10cont1(int64_t && tenantCount,int loopDepth) 
	{
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tenantCount > CLIENT_KNOBS->MAX_TENANTS_PER_CLUSTER)
															#line 2682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(cluster_no_capacity(), loopDepth);
															#line 2686 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<CreateTenantTransactionActor*>(this)->SAV<std::pair<Optional<TenantMapEntry>, bool>>::futures) { (void)(std::make_pair(tenantEntry, true)); this->~CreateTenantTransactionActorState(); static_cast<CreateTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 2690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<CreateTenantTransactionActor*>(this)->SAV< std::pair<Optional<TenantMapEntry>, bool> >::value()) std::pair<Optional<TenantMapEntry>, bool>(std::make_pair(tenantEntry, true));
		this->~CreateTenantTransactionActorState();
		static_cast<CreateTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont10when1(int64_t const& tenantCount,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1(tenantCount, loopDepth);

		return loopDepth;
	}
	int a_body1cont10when1(int64_t && tenantCount,int loopDepth) 
	{
		loopDepth = a_body1cont10cont1(std::move(tenantCount), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantTransactionActor*>(this)->ActorCallback< CreateTenantTransactionActor, 5, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 5, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont10when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 5, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1cont10when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< CreateTenantTransactionActor, 5, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 5);

	}
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Transaction tr;
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantMapEntry tenantEntry;
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ClusterType clusterType;
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Future<Optional<TenantMapEntry>> existingEntryFuture;
															#line 169 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Future<Void> tenantModeCheck;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Future<bool> tombstoneFuture;
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Future<Optional<TenantGroupEntry>> existingTenantGroupEntryFuture;
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	bool hasTombstone;
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	typename transaction_future_type<Transaction, RangeResult>::type prefixRangeFuture;
															#line 2779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via createTenantTransaction()
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class CreateTenantTransactionActor final : public Actor<std::pair<Optional<TenantMapEntry>, bool>>, public ActorCallback< CreateTenantTransactionActor<Transaction>, 0, Void >, public ActorCallback< CreateTenantTransactionActor<Transaction>, 1, Optional<TenantMapEntry> >, public ActorCallback< CreateTenantTransactionActor<Transaction>, 2, bool >, public ActorCallback< CreateTenantTransactionActor<Transaction>, 3, RangeResult >, public ActorCallback< CreateTenantTransactionActor<Transaction>, 4, Optional<TenantGroupEntry> >, public ActorCallback< CreateTenantTransactionActor<Transaction>, 5, int64_t >, public FastAllocated<CreateTenantTransactionActor<Transaction>>, public CreateTenantTransactionActorState<Transaction, CreateTenantTransactionActor<Transaction>> {
															#line 2786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<CreateTenantTransactionActor<Transaction>>::operator new;
	using FastAllocated<CreateTenantTransactionActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::pair<Optional<TenantMapEntry>, bool>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CreateTenantTransactionActor<Transaction>, 0, Void >;
friend struct ActorCallback< CreateTenantTransactionActor<Transaction>, 1, Optional<TenantMapEntry> >;
friend struct ActorCallback< CreateTenantTransactionActor<Transaction>, 2, bool >;
friend struct ActorCallback< CreateTenantTransactionActor<Transaction>, 3, RangeResult >;
friend struct ActorCallback< CreateTenantTransactionActor<Transaction>, 4, Optional<TenantGroupEntry> >;
friend struct ActorCallback< CreateTenantTransactionActor<Transaction>, 5, int64_t >;
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	CreateTenantTransactionActor(Transaction const& tr,TenantMapEntry const& tenantEntry,ClusterType const& clusterType = ClusterType::STANDALONE) 
															#line 2802 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<std::pair<Optional<TenantMapEntry>, bool>>(),
		   CreateTenantTransactionActorState<Transaction, CreateTenantTransactionActor<Transaction>>(tr, tenantEntry, clusterType)
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("createTenantTransaction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CreateTenantTransactionActor<Transaction>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CreateTenantTransactionActor<Transaction>, 1, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CreateTenantTransactionActor<Transaction>, 2, bool >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< CreateTenantTransactionActor<Transaction>, 3, RangeResult >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< CreateTenantTransactionActor<Transaction>, 4, Optional<TenantGroupEntry> >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< CreateTenantTransactionActor<Transaction>, 5, int64_t >*)0, actor_cancelled()); break;
		}

	}
};
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<std::pair<Optional<TenantMapEntry>, bool>> createTenantTransaction( Transaction const& tr, TenantMapEntry const& tenantEntry, ClusterType const& clusterType = ClusterType::STANDALONE ) {
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<std::pair<Optional<TenantMapEntry>, bool>>(new CreateTenantTransactionActor<Transaction>(tr, tenantEntry, clusterType));
															#line 2836 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

															#line 2841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via getNextTenantId()
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class GetNextTenantIdActor>
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class GetNextTenantIdActorState {
															#line 2847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	GetNextTenantIdActorState(Transaction const& tr) 
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : tr(tr)
															#line 2854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("getNextTenantId", reinterpret_cast<unsigned long>(this));

	}
	~GetNextTenantIdActorState() 
	{
		fdb_probe_actor_destroy("getNextTenantId", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Optional<int64_t>> __when_expr_0 = TenantMetadata::lastTenantId().get(tr);
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<GetNextTenantIdActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetNextTenantIdActor*>(this)->actor_wait_state = 1;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetNextTenantIdActor, 0, Optional<int64_t> >*>(static_cast<GetNextTenantIdActor*>(this)));
															#line 2876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~GetNextTenantIdActorState();
		static_cast<GetNextTenantIdActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!lastId.present())
															#line 2899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<int64_t> __when_expr_1 = TenantMetadata::tenantIdPrefix().getD(tr, Snapshot::False, 0);
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<GetNextTenantIdActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetNextTenantIdActor*>(this)->actor_wait_state = 2;
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetNextTenantIdActor, 1, int64_t >*>(static_cast<GetNextTenantIdActor*>(this)));
															#line 2910 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1when1(Optional<int64_t> const& __lastId,int loopDepth) 
	{
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		lastId = __lastId;
															#line 2924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<int64_t> && __lastId,int loopDepth) 
	{
		lastId = std::move(__lastId);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetNextTenantIdActor*>(this)->actor_wait_state > 0) static_cast<GetNextTenantIdActor*>(this)->actor_wait_state = 0;
		static_cast<GetNextTenantIdActor*>(this)->ActorCallback< GetNextTenantIdActor, 0, Optional<int64_t> >::remove();

	}
	void a_callback_fire(ActorCallback< GetNextTenantIdActor, 0, Optional<int64_t> >*,Optional<int64_t> const& value) 
	{
		fdb_probe_actor_enter("getNextTenantId", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNextTenantId", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetNextTenantIdActor, 0, Optional<int64_t> >*,Optional<int64_t> && value) 
	{
		fdb_probe_actor_enter("getNextTenantId", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNextTenantId", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetNextTenantIdActor, 0, Optional<int64_t> >*,Error err) 
	{
		fdb_probe_actor_enter("getNextTenantId", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNextTenantId", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		int64_t delta = 1;
															#line 235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (BUGGIFY)
															#line 2993 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			delta += deterministicRandom()->randomSkewedUInt32(1, 1e9);
															#line 2997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<GetNextTenantIdActor*>(this)->SAV<int64_t>::futures) { (void)(TenantAPI::computeNextTenantId(lastId.get(), delta)); this->~GetNextTenantIdActorState(); static_cast<GetNextTenantIdActor*>(this)->destroy(); return 0; }
															#line 3001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<GetNextTenantIdActor*>(this)->SAV< int64_t >::value()) int64_t(TenantAPI::computeNextTenantId(lastId.get(), delta));
		this->~GetNextTenantIdActorState();
		static_cast<GetNextTenantIdActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(int64_t const& tenantIdPrefix,int loopDepth) 
	{
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		lastId = tenantIdPrefix << 48;
															#line 3013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(int64_t && tenantIdPrefix,int loopDepth) 
	{
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		lastId = tenantIdPrefix << 48;
															#line 3022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(int64_t const& tenantIdPrefix,int loopDepth) 
	{
		loopDepth = a_body1cont3(tenantIdPrefix, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(int64_t && tenantIdPrefix,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(tenantIdPrefix), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetNextTenantIdActor*>(this)->actor_wait_state > 0) static_cast<GetNextTenantIdActor*>(this)->actor_wait_state = 0;
		static_cast<GetNextTenantIdActor*>(this)->ActorCallback< GetNextTenantIdActor, 1, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< GetNextTenantIdActor, 1, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("getNextTenantId", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNextTenantId", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetNextTenantIdActor, 1, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("getNextTenantId", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNextTenantId", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetNextTenantIdActor, 1, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("getNextTenantId", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNextTenantId", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Transaction tr;
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Optional<int64_t> lastId;
															#line 3094 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via getNextTenantId()
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class GetNextTenantIdActor final : public Actor<int64_t>, public ActorCallback< GetNextTenantIdActor<Transaction>, 0, Optional<int64_t> >, public ActorCallback< GetNextTenantIdActor<Transaction>, 1, int64_t >, public FastAllocated<GetNextTenantIdActor<Transaction>>, public GetNextTenantIdActorState<Transaction, GetNextTenantIdActor<Transaction>> {
															#line 3101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<GetNextTenantIdActor<Transaction>>::operator new;
	using FastAllocated<GetNextTenantIdActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int64_t>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetNextTenantIdActor<Transaction>, 0, Optional<int64_t> >;
friend struct ActorCallback< GetNextTenantIdActor<Transaction>, 1, int64_t >;
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	GetNextTenantIdActor(Transaction const& tr) 
															#line 3113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<int64_t>(),
		   GetNextTenantIdActorState<Transaction, GetNextTenantIdActor<Transaction>>(tr)
	{
		fdb_probe_actor_enter("getNextTenantId", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getNextTenantId");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getNextTenantId", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetNextTenantIdActor<Transaction>, 0, Optional<int64_t> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetNextTenantIdActor<Transaction>, 1, int64_t >*)0, actor_cancelled()); break;
		}

	}
};
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<int64_t> getNextTenantId( Transaction const& tr ) {
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<int64_t>(new GetNextTenantIdActor<Transaction>(tr));
															#line 3143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

															#line 3148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via createTenant()
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB, class CreateTenantActor>
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class CreateTenantActorState {
															#line 3154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	CreateTenantActorState(Reference<DB> const& db,TenantName const& name,TenantMapEntry const& tenantEntry = TenantMapEntry(),ClusterType const& clusterType = ClusterType::STANDALONE) 
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : db(db),
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   name(name),
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tenantEntry(tenantEntry),
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   clusterType(clusterType),
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tr(db->createTransaction()),
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   checkExistence(clusterType != ClusterType::METACLUSTER_DATA),
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   generateTenantId(tenantEntry.id < 0)
															#line 3173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			ASSERT(clusterType == ClusterType::STANDALONE || !generateTenantId);
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tenantEntry.tenantName = name;
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			;
															#line 3192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~CreateTenantActorState();
		static_cast<CreateTenantActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tenantIdFuture = Future<int64_t>();
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (generateTenantId)
															#line 3229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				tenantIdFuture = getNextTenantId(tr);
															#line 3233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			}
															#line 266 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (checkExistence)
															#line 3237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				StrictFuture<Optional<int64_t>> __when_expr_0 = TenantMetadata::tenantNameIndex().get(tr, name);
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				if (static_cast<CreateTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<CreateTenantActor*>(this)->actor_wait_state = 1;
															#line 267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CreateTenantActor, 0, Optional<int64_t> >*>(static_cast<CreateTenantActor*>(this)));
															#line 3248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_4 = safeThreadFutureToFuture(tr->onError(e));
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<CreateTenantActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_4.get(), loopDepth); };
			static_cast<CreateTenantActor*>(this)->actor_wait_state = 5;
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< CreateTenantActor, 4, Void >*>(static_cast<CreateTenantActor*>(this)));
															#line 3282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (generateTenantId)
															#line 3297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<int64_t> __when_expr_1 = tenantIdFuture;
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<CreateTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<CreateTenantActor*>(this)->actor_wait_state = 2;
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CreateTenantActor, 1, int64_t >*>(static_cast<CreateTenantActor*>(this)));
															#line 3308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont7(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont4(Optional<int64_t> const& existingId,int loopDepth) 
	{
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (existingId.present())
															#line 3322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1loopBody1Catch1(tenant_already_exists(), loopDepth);
															#line 3326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		checkExistence = false;
															#line 3330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Optional<int64_t> && existingId,int loopDepth) 
	{
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (existingId.present())
															#line 3339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1loopBody1Catch1(tenant_already_exists(), loopDepth);
															#line 3343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 272 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		checkExistence = false;
															#line 3347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<int64_t> const& existingId,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(existingId, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<int64_t> && existingId,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(existingId), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CreateTenantActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantActor*>(this)->ActorCallback< CreateTenantActor, 0, Optional<int64_t> >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 0, Optional<int64_t> >*,Optional<int64_t> const& value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 0, Optional<int64_t> >*,Optional<int64_t> && value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CreateTenantActor, 0, Optional<int64_t> >*,Error err) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont7(int loopDepth) 
	{
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<std::pair<Optional<TenantMapEntry>, bool>> __when_expr_2 = createTenantTransaction(tr, tenantEntry, clusterType);
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<CreateTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont7when1(__when_expr_2.get(), loopDepth); };
		static_cast<CreateTenantActor*>(this)->actor_wait_state = 3;
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CreateTenantActor, 2, std::pair<Optional<TenantMapEntry>, bool> >*>(static_cast<CreateTenantActor*>(this)));
															#line 3426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont8(int64_t const& tenantId,int loopDepth) 
	{
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		tenantEntry.setId(tenantId);
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::lastTenantId().set(tr, tenantId);
															#line 3437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8(int64_t && tenantId,int loopDepth) 
	{
															#line 277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		tenantEntry.setId(tenantId);
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::lastTenantId().set(tr, tenantId);
															#line 3448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(int64_t const& tenantId,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(tenantId, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(int64_t && tenantId,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(std::move(tenantId), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CreateTenantActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantActor*>(this)->ActorCallback< CreateTenantActor, 1, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 1, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 1, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CreateTenantActor, 1, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont10(int loopDepth) 
	{
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (newTenant.second)
															#line 3520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			ASSERT(newTenant.first.present());
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_3 = buggifiedCommit(tr, BUGGIFY_WITH_PROB(0.1));
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<CreateTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont10when1(__when_expr_3.get(), loopDepth); };
			static_cast<CreateTenantActor*>(this)->actor_wait_state = 4;
															#line 286 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CreateTenantActor, 3, Void >*>(static_cast<CreateTenantActor*>(this)));
															#line 3533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont10cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont7when1(std::pair<Optional<TenantMapEntry>, bool> const& __newTenant,int loopDepth) 
	{
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		newTenant = __newTenant;
															#line 3547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1loopBody1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7when1(std::pair<Optional<TenantMapEntry>, bool> && __newTenant,int loopDepth) 
	{
		newTenant = std::move(__newTenant);
		loopDepth = a_body1loopBody1cont10(loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<CreateTenantActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantActor*>(this)->ActorCallback< CreateTenantActor, 2, std::pair<Optional<TenantMapEntry>, bool> >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 2, std::pair<Optional<TenantMapEntry>, bool> >*,std::pair<Optional<TenantMapEntry>, bool> const& value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont7when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 2, std::pair<Optional<TenantMapEntry>, bool> >*,std::pair<Optional<TenantMapEntry>, bool> && value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont7when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CreateTenantActor, 2, std::pair<Optional<TenantMapEntry>, bool> >*,Error err) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont10cont1(int loopDepth) 
	{
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<CreateTenantActor*>(this)->SAV<Optional<TenantMapEntry>>::futures) { (void)(newTenant.first); this->~CreateTenantActorState(); static_cast<CreateTenantActor*>(this)->destroy(); return 0; }
															#line 3614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<CreateTenantActor*>(this)->SAV< Optional<TenantMapEntry> >::value()) Optional<TenantMapEntry>(newTenant.first);
		this->~CreateTenantActorState();
		static_cast<CreateTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont10cont2(Void const& _,int loopDepth) 
	{
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TraceEvent("CreatedTenant") .detail("Tenant", name) .detail("TenantId", newTenant.first.get().id) .detail("Prefix", newTenant.first.get().prefix) .detail("TenantGroup", tenantEntry.tenantGroup) .detail("Version", tr->getCommittedVersion());
															#line 3626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1loopBody1cont10cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont10cont2(Void && _,int loopDepth) 
	{
															#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TraceEvent("CreatedTenant") .detail("Tenant", name) .detail("TenantId", newTenant.first.get().id) .detail("Prefix", newTenant.first.get().prefix) .detail("TenantGroup", tenantEntry.tenantGroup) .detail("Version", tr->getCommittedVersion());
															#line 3635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1loopBody1cont10cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont10when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont10when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<CreateTenantActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantActor*>(this)->ActorCallback< CreateTenantActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont10when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont10when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< CreateTenantActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 3);

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
		if (static_cast<CreateTenantActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantActor*>(this)->ActorCallback< CreateTenantActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< CreateTenantActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Reference<DB> db;
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantName name;
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantMapEntry tenantEntry;
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ClusterType clusterType;
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	bool checkExistence;
															#line 250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	bool generateTenantId;
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Future<int64_t> tenantIdFuture;
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	std::pair<Optional<TenantMapEntry>, bool> newTenant;
															#line 3796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via createTenant()
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB>
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class CreateTenantActor final : public Actor<Optional<TenantMapEntry>>, public ActorCallback< CreateTenantActor<DB>, 0, Optional<int64_t> >, public ActorCallback< CreateTenantActor<DB>, 1, int64_t >, public ActorCallback< CreateTenantActor<DB>, 2, std::pair<Optional<TenantMapEntry>, bool> >, public ActorCallback< CreateTenantActor<DB>, 3, Void >, public ActorCallback< CreateTenantActor<DB>, 4, Void >, public FastAllocated<CreateTenantActor<DB>>, public CreateTenantActorState<DB, CreateTenantActor<DB>> {
															#line 3803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<CreateTenantActor<DB>>::operator new;
	using FastAllocated<CreateTenantActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<TenantMapEntry>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CreateTenantActor<DB>, 0, Optional<int64_t> >;
friend struct ActorCallback< CreateTenantActor<DB>, 1, int64_t >;
friend struct ActorCallback< CreateTenantActor<DB>, 2, std::pair<Optional<TenantMapEntry>, bool> >;
friend struct ActorCallback< CreateTenantActor<DB>, 3, Void >;
friend struct ActorCallback< CreateTenantActor<DB>, 4, Void >;
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	CreateTenantActor(Reference<DB> const& db,TenantName const& name,TenantMapEntry const& tenantEntry = TenantMapEntry(),ClusterType const& clusterType = ClusterType::STANDALONE) 
															#line 3818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<Optional<TenantMapEntry>>(),
		   CreateTenantActorState<DB, CreateTenantActor<DB>>(db, name, tenantEntry, clusterType)
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
		case 1: this->a_callback_error((ActorCallback< CreateTenantActor<DB>, 0, Optional<int64_t> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CreateTenantActor<DB>, 1, int64_t >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CreateTenantActor<DB>, 2, std::pair<Optional<TenantMapEntry>, bool> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< CreateTenantActor<DB>, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< CreateTenantActor<DB>, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB>
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<Optional<TenantMapEntry>> createTenant( Reference<DB> const& db, TenantName const& name, TenantMapEntry const& tenantEntry = TenantMapEntry(), ClusterType const& clusterType = ClusterType::STANDALONE ) {
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<Optional<TenantMapEntry>>(new CreateTenantActor<DB>(db, name, tenantEntry, clusterType));
															#line 3851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

															#line 3856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via markTenantTombstones()
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class MarkTenantTombstonesActor>
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class MarkTenantTombstonesActorState {
															#line 3862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	MarkTenantTombstonesActorState(Transaction const& tr,int64_t const& tenantId) 
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : tr(tr),
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tenantId(tenantId),
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   latestTombstoneFuture(TenantMetadata::tenantTombstones().getRange(tr, {}, {}, 1, Snapshot::False, Reverse::True))
															#line 3873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("markTenantTombstones", reinterpret_cast<unsigned long>(this));

	}
	~MarkTenantTombstonesActorState() 
	{
		fdb_probe_actor_destroy("markTenantTombstones", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Optional<TenantTombstoneCleanupData>> __when_expr_0 = TenantMetadata::tombstoneCleanupData().get(tr);
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<MarkTenantTombstonesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3890 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<MarkTenantTombstonesActor*>(this)->actor_wait_state = 1;
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MarkTenantTombstonesActor, 0, Optional<TenantTombstoneCleanupData> >*>(static_cast<MarkTenantTombstonesActor*>(this)));
															#line 3895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~MarkTenantTombstonesActorState();
		static_cast<MarkTenantTombstonesActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Version> __when_expr_1 = safeThreadFutureToFuture(tr->getReadVersion());
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<MarkTenantTombstonesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3920 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<MarkTenantTombstonesActor*>(this)->actor_wait_state = 2;
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< MarkTenantTombstonesActor, 1, Version >*>(static_cast<MarkTenantTombstonesActor*>(this)));
															#line 3925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Optional<TenantTombstoneCleanupData> const& __cleanupData,int loopDepth) 
	{
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		cleanupData = __cleanupData;
															#line 3934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<TenantTombstoneCleanupData> && __cleanupData,int loopDepth) 
	{
		cleanupData = std::move(__cleanupData);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<MarkTenantTombstonesActor*>(this)->actor_wait_state > 0) static_cast<MarkTenantTombstonesActor*>(this)->actor_wait_state = 0;
		static_cast<MarkTenantTombstonesActor*>(this)->ActorCallback< MarkTenantTombstonesActor, 0, Optional<TenantTombstoneCleanupData> >::remove();

	}
	void a_callback_fire(ActorCallback< MarkTenantTombstonesActor, 0, Optional<TenantTombstoneCleanupData> >*,Optional<TenantTombstoneCleanupData> const& value) 
	{
		fdb_probe_actor_enter("markTenantTombstones", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("markTenantTombstones", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MarkTenantTombstonesActor, 0, Optional<TenantTombstoneCleanupData> >*,Optional<TenantTombstoneCleanupData> && value) 
	{
		fdb_probe_actor_enter("markTenantTombstones", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("markTenantTombstones", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MarkTenantTombstonesActor, 0, Optional<TenantTombstoneCleanupData> >*,Error err) 
	{
		fdb_probe_actor_enter("markTenantTombstones", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("markTenantTombstones", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!cleanupData.present() || cleanupData.get().nextTombstoneEraseVersion <= transactionReadVersion)
															#line 4001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			deleteThroughId = cleanupData.present() ? cleanupData.get().nextTombstoneEraseId : -1;
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (deleteThroughId >= 0)
															#line 4007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				TenantMetadata::tenantTombstones().erase(tr, 0, deleteThroughId + 1);
															#line 4011 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			}
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<KeyBackedRangeResult<int64_t>> __when_expr_2 = latestTombstoneFuture;
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<MarkTenantTombstonesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<MarkTenantTombstonesActor*>(this)->actor_wait_state = 3;
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< MarkTenantTombstonesActor, 2, KeyBackedRangeResult<int64_t> >*>(static_cast<MarkTenantTombstonesActor*>(this)));
															#line 4022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (tenantId > cleanupData.get().tombstonesErasedThrough)
															#line 4029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				TenantMetadata::tenantTombstones().insert(tr, tenantId);
															#line 4033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			}
			loopDepth = a_body1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1when1(Version const& __transactionReadVersion,int loopDepth) 
	{
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		transactionReadVersion = __transactionReadVersion;
															#line 4044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Version && __transactionReadVersion,int loopDepth) 
	{
		transactionReadVersion = std::move(__transactionReadVersion);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<MarkTenantTombstonesActor*>(this)->actor_wait_state > 0) static_cast<MarkTenantTombstonesActor*>(this)->actor_wait_state = 0;
		static_cast<MarkTenantTombstonesActor*>(this)->ActorCallback< MarkTenantTombstonesActor, 1, Version >::remove();

	}
	void a_callback_fire(ActorCallback< MarkTenantTombstonesActor, 1, Version >*,Version const& value) 
	{
		fdb_probe_actor_enter("markTenantTombstones", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("markTenantTombstones", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< MarkTenantTombstonesActor, 1, Version >*,Version && value) 
	{
		fdb_probe_actor_enter("markTenantTombstones", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("markTenantTombstones", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< MarkTenantTombstonesActor, 1, Version >*,Error err) 
	{
		fdb_probe_actor_enter("markTenantTombstones", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("markTenantTombstones", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<MarkTenantTombstonesActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MarkTenantTombstonesActorState(); static_cast<MarkTenantTombstonesActor*>(this)->destroy(); return 0; }
															#line 4111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<MarkTenantTombstonesActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~MarkTenantTombstonesActorState();
		static_cast<MarkTenantTombstonesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(KeyBackedRangeResult<int64_t> const& latestTombstone,int loopDepth) 
	{
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		int64_t nextDeleteThroughId = std::max(deleteThroughId, tenantId);
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!latestTombstone.results.empty())
															#line 4125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			nextDeleteThroughId = std::max(nextDeleteThroughId, latestTombstone.results[0]);
															#line 4129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantTombstoneCleanupData updatedCleanupData;
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		updatedCleanupData.tombstonesErasedThrough = deleteThroughId;
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		updatedCleanupData.nextTombstoneEraseId = nextDeleteThroughId;
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		updatedCleanupData.nextTombstoneEraseVersion = transactionReadVersion + CLIENT_KNOBS->TENANT_TOMBSTONE_CLEANUP_INTERVAL * CLIENT_KNOBS->VERSIONS_PER_SECOND;
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tombstoneCleanupData().set(tr, updatedCleanupData);
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tenantId > updatedCleanupData.tombstonesErasedThrough)
															#line 4143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::tenantTombstones().insert(tr, tenantId);
															#line 4147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(KeyBackedRangeResult<int64_t> && latestTombstone,int loopDepth) 
	{
															#line 320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		int64_t nextDeleteThroughId = std::max(deleteThroughId, tenantId);
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!latestTombstone.results.empty())
															#line 4159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			nextDeleteThroughId = std::max(nextDeleteThroughId, latestTombstone.results[0]);
															#line 4163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantTombstoneCleanupData updatedCleanupData;
															#line 328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		updatedCleanupData.tombstonesErasedThrough = deleteThroughId;
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		updatedCleanupData.nextTombstoneEraseId = nextDeleteThroughId;
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		updatedCleanupData.nextTombstoneEraseVersion = transactionReadVersion + CLIENT_KNOBS->TENANT_TOMBSTONE_CLEANUP_INTERVAL * CLIENT_KNOBS->VERSIONS_PER_SECOND;
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tombstoneCleanupData().set(tr, updatedCleanupData);
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tenantId > updatedCleanupData.tombstonesErasedThrough)
															#line 4177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::tenantTombstones().insert(tr, tenantId);
															#line 4181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(KeyBackedRangeResult<int64_t> const& latestTombstone,int loopDepth) 
	{
		loopDepth = a_body1cont4(latestTombstone, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(KeyBackedRangeResult<int64_t> && latestTombstone,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(latestTombstone), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<MarkTenantTombstonesActor*>(this)->actor_wait_state > 0) static_cast<MarkTenantTombstonesActor*>(this)->actor_wait_state = 0;
		static_cast<MarkTenantTombstonesActor*>(this)->ActorCallback< MarkTenantTombstonesActor, 2, KeyBackedRangeResult<int64_t> >::remove();

	}
	void a_callback_fire(ActorCallback< MarkTenantTombstonesActor, 2, KeyBackedRangeResult<int64_t> >*,KeyBackedRangeResult<int64_t> const& value) 
	{
		fdb_probe_actor_enter("markTenantTombstones", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("markTenantTombstones", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< MarkTenantTombstonesActor, 2, KeyBackedRangeResult<int64_t> >*,KeyBackedRangeResult<int64_t> && value) 
	{
		fdb_probe_actor_enter("markTenantTombstones", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("markTenantTombstones", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< MarkTenantTombstonesActor, 2, KeyBackedRangeResult<int64_t> >*,Error err) 
	{
		fdb_probe_actor_enter("markTenantTombstones", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("markTenantTombstones", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Transaction tr;
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	int64_t tenantId;
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Future<KeyBackedRangeResult<int64_t>> latestTombstoneFuture;
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Optional<TenantTombstoneCleanupData> cleanupData;
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Version transactionReadVersion;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	int64_t deleteThroughId;
															#line 4262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via markTenantTombstones()
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class MarkTenantTombstonesActor final : public Actor<Void>, public ActorCallback< MarkTenantTombstonesActor<Transaction>, 0, Optional<TenantTombstoneCleanupData> >, public ActorCallback< MarkTenantTombstonesActor<Transaction>, 1, Version >, public ActorCallback< MarkTenantTombstonesActor<Transaction>, 2, KeyBackedRangeResult<int64_t> >, public FastAllocated<MarkTenantTombstonesActor<Transaction>>, public MarkTenantTombstonesActorState<Transaction, MarkTenantTombstonesActor<Transaction>> {
															#line 4269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<MarkTenantTombstonesActor<Transaction>>::operator new;
	using FastAllocated<MarkTenantTombstonesActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MarkTenantTombstonesActor<Transaction>, 0, Optional<TenantTombstoneCleanupData> >;
friend struct ActorCallback< MarkTenantTombstonesActor<Transaction>, 1, Version >;
friend struct ActorCallback< MarkTenantTombstonesActor<Transaction>, 2, KeyBackedRangeResult<int64_t> >;
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	MarkTenantTombstonesActor(Transaction const& tr,int64_t const& tenantId) 
															#line 4282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<Void>(),
		   MarkTenantTombstonesActorState<Transaction, MarkTenantTombstonesActor<Transaction>>(tr, tenantId)
	{
		fdb_probe_actor_enter("markTenantTombstones", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("markTenantTombstones");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("markTenantTombstones", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MarkTenantTombstonesActor<Transaction>, 0, Optional<TenantTombstoneCleanupData> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< MarkTenantTombstonesActor<Transaction>, 1, Version >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< MarkTenantTombstonesActor<Transaction>, 2, KeyBackedRangeResult<int64_t> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<Void> markTenantTombstones( Transaction const& tr, int64_t const& tenantId ) {
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<Void>(new MarkTenantTombstonesActor<Transaction>(tr, tenantId));
															#line 4313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

// Deletes a tenant with the given ID. If no matching tenant is found, this function returns without deleting anything.
// This behavior allows the function to be used idempotently: if the transaction is retried after having succeeded, it
// will see that the tenant is absent and do nothing.
															#line 4321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via deleteTenantTransaction()
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class DeleteTenantTransactionActor>
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class DeleteTenantTransactionActorState {
															#line 4327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	DeleteTenantTransactionActorState(Transaction const& tr,int64_t const& tenantId,ClusterType const& clusterType = ClusterType::STANDALONE) 
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : tr(tr),
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tenantId(tenantId),
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   clusterType(clusterType)
															#line 4338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("deleteTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	~DeleteTenantTransactionActorState() 
	{
		fdb_probe_actor_destroy("deleteTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			ASSERT(tenantId != TenantInfo::INVALID_TENANT);
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			ASSERT(clusterType != ClusterType::METACLUSTER_MANAGEMENT);
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tenantEntryFuture = tryGetTenantTransaction(tr, tenantId);
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_0 = checkTenantMode(tr, clusterType);
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 1;
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantTransactionActor, 0, Void >*>(static_cast<DeleteTenantTransactionActor*>(this)));
															#line 4368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~DeleteTenantTransactionActorState();
		static_cast<DeleteTenantTransactionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Optional<TenantMapEntry>> __when_expr_1 = tenantEntryFuture;
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 2;
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantTransactionActor, 1, Optional<TenantMapEntry> >*>(static_cast<DeleteTenantTransactionActor*>(this)));
															#line 4398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Optional<TenantMapEntry>> __when_expr_1 = tenantEntryFuture;
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 2;
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantTransactionActor, 1, Optional<TenantMapEntry> >*>(static_cast<DeleteTenantTransactionActor*>(this)));
															#line 4414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantTransactionActor*>(this)->ActorCallback< DeleteTenantTransactionActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantTransactionActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DeleteTenantTransactionActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DeleteTenantTransactionActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tenantEntry.present())
															#line 4486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			prefixRangeFuture = tr->getRange(prefixRange(tenantEntry.get().prefix), 1);
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<RangeResult> __when_expr_2 = safeThreadFutureToFuture(prefixRangeFuture);
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 3;
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantTransactionActor, 2, RangeResult >*>(static_cast<DeleteTenantTransactionActor*>(this)));
															#line 4499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1when1(Optional<TenantMapEntry> const& __tenantEntry,int loopDepth) 
	{
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		tenantEntry = __tenantEntry;
															#line 4513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Optional<TenantMapEntry> && __tenantEntry,int loopDepth) 
	{
		tenantEntry = std::move(__tenantEntry);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantTransactionActor*>(this)->ActorCallback< DeleteTenantTransactionActor, 1, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantTransactionActor, 1, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DeleteTenantTransactionActor, 1, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DeleteTenantTransactionActor, 1, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (clusterType == ClusterType::METACLUSTER_DATA)
															#line 4580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_4 = markTenantTombstones(tr, tenantId);
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4586 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont3when1(__when_expr_4.get(), loopDepth); };
			static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 5;
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantTransactionActor, 4, Void >*>(static_cast<DeleteTenantTransactionActor*>(this)));
															#line 4591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont10(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont4(RangeResult const& contents,int loopDepth) 
	{
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!contents.empty())
															#line 4605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_not_empty(), loopDepth);
															#line 4609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantMap().erase(tr, tenantId);
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantNameIndex().erase(tr, tenantEntry.get().tenantName);
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantCount().atomicOp(tr, -1, MutationRef::AddValue);
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::lastTenantModification().setVersionstamp(tr, Versionstamp(), 0);
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tenantEntry.get().tenantGroup.present())
															#line 4621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::tenantGroupTenantIndex().erase( tr, Tuple::makeTuple(tenantEntry.get().tenantGroup.get(), tenantId));
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<KeyBackedSet<Tuple>::RangeResultType> __when_expr_3 = TenantMetadata::tenantGroupTenantIndex().getRange( tr, Tuple::makeTuple(tenantEntry.get().tenantGroup.get()), Tuple::makeTuple(keyAfter(tenantEntry.get().tenantGroup.get())), 2);
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
			static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 4;
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantTransactionActor, 3, KeyBackedSet<Tuple>::RangeResultType >*>(static_cast<DeleteTenantTransactionActor*>(this)));
															#line 4634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont5(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont4(RangeResult && contents,int loopDepth) 
	{
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!contents.empty())
															#line 4648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_not_empty(), loopDepth);
															#line 4652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantMap().erase(tr, tenantId);
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantNameIndex().erase(tr, tenantEntry.get().tenantName);
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantCount().atomicOp(tr, -1, MutationRef::AddValue);
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::lastTenantModification().setVersionstamp(tr, Versionstamp(), 0);
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tenantEntry.get().tenantGroup.present())
															#line 4664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::tenantGroupTenantIndex().erase( tr, Tuple::makeTuple(tenantEntry.get().tenantGroup.get(), tenantId));
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<KeyBackedSet<Tuple>::RangeResultType> __when_expr_3 = TenantMetadata::tenantGroupTenantIndex().getRange( tr, Tuple::makeTuple(tenantEntry.get().tenantGroup.get()), Tuple::makeTuple(keyAfter(tenantEntry.get().tenantGroup.get())), 2);
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont4when1(__when_expr_3.get(), loopDepth); };
			static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 4;
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantTransactionActor, 3, KeyBackedSet<Tuple>::RangeResultType >*>(static_cast<DeleteTenantTransactionActor*>(this)));
															#line 4677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont5(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2when1(RangeResult const& contents,int loopDepth) 
	{
		loopDepth = a_body1cont4(contents, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(RangeResult && contents,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(contents), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantTransactionActor*>(this)->ActorCallback< DeleteTenantTransactionActor, 2, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantTransactionActor, 2, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< DeleteTenantTransactionActor, 2, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< DeleteTenantTransactionActor, 2, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont5(int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(KeyBackedSet<Tuple>::RangeResultType const& tenantsInGroup,int loopDepth) 
	{
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tenantsInGroup.results.empty() || (tenantsInGroup.results.size() == 1 && tenantsInGroup.results[0].getInt(1) == tenantId))
															#line 4760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::tenantGroupMap().erase(tr, tenantEntry.get().tenantGroup.get());
															#line 4764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(KeyBackedSet<Tuple>::RangeResultType && tenantsInGroup,int loopDepth) 
	{
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tenantsInGroup.results.empty() || (tenantsInGroup.results.size() == 1 && tenantsInGroup.results[0].getInt(1) == tenantId))
															#line 4774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::tenantGroupMap().erase(tr, tenantEntry.get().tenantGroup.get());
															#line 4778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(KeyBackedSet<Tuple>::RangeResultType const& tenantsInGroup,int loopDepth) 
	{
		loopDepth = a_body1cont7(tenantsInGroup, loopDepth);

		return loopDepth;
	}
	int a_body1cont4when1(KeyBackedSet<Tuple>::RangeResultType && tenantsInGroup,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(tenantsInGroup), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantTransactionActor*>(this)->ActorCallback< DeleteTenantTransactionActor, 3, KeyBackedSet<Tuple>::RangeResultType >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantTransactionActor, 3, KeyBackedSet<Tuple>::RangeResultType >*,KeyBackedSet<Tuple>::RangeResultType const& value) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< DeleteTenantTransactionActor, 3, KeyBackedSet<Tuple>::RangeResultType >*,KeyBackedSet<Tuple>::RangeResultType && value) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont4when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< DeleteTenantTransactionActor, 3, KeyBackedSet<Tuple>::RangeResultType >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont10(int loopDepth) 
	{
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<DeleteTenantTransactionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteTenantTransactionActorState(); static_cast<DeleteTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 4851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<DeleteTenantTransactionActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteTenantTransactionActorState();
		static_cast<DeleteTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont11(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont11(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont11(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont11(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantTransactionActor*>(this)->ActorCallback< DeleteTenantTransactionActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantTransactionActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< DeleteTenantTransactionActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< DeleteTenantTransactionActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Transaction tr;
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	int64_t tenantId;
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ClusterType clusterType;
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Future<Optional<TenantMapEntry>> tenantEntryFuture;
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Optional<TenantMapEntry> tenantEntry;
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	typename transaction_future_type<Transaction, RangeResult>::type prefixRangeFuture;
															#line 4946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via deleteTenantTransaction()
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class DeleteTenantTransactionActor final : public Actor<Void>, public ActorCallback< DeleteTenantTransactionActor<Transaction>, 0, Void >, public ActorCallback< DeleteTenantTransactionActor<Transaction>, 1, Optional<TenantMapEntry> >, public ActorCallback< DeleteTenantTransactionActor<Transaction>, 2, RangeResult >, public ActorCallback< DeleteTenantTransactionActor<Transaction>, 3, KeyBackedSet<Tuple>::RangeResultType >, public ActorCallback< DeleteTenantTransactionActor<Transaction>, 4, Void >, public FastAllocated<DeleteTenantTransactionActor<Transaction>>, public DeleteTenantTransactionActorState<Transaction, DeleteTenantTransactionActor<Transaction>> {
															#line 4953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<DeleteTenantTransactionActor<Transaction>>::operator new;
	using FastAllocated<DeleteTenantTransactionActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DeleteTenantTransactionActor<Transaction>, 0, Void >;
friend struct ActorCallback< DeleteTenantTransactionActor<Transaction>, 1, Optional<TenantMapEntry> >;
friend struct ActorCallback< DeleteTenantTransactionActor<Transaction>, 2, RangeResult >;
friend struct ActorCallback< DeleteTenantTransactionActor<Transaction>, 3, KeyBackedSet<Tuple>::RangeResultType >;
friend struct ActorCallback< DeleteTenantTransactionActor<Transaction>, 4, Void >;
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	DeleteTenantTransactionActor(Transaction const& tr,int64_t const& tenantId,ClusterType const& clusterType = ClusterType::STANDALONE) 
															#line 4968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<Void>(),
		   DeleteTenantTransactionActorState<Transaction, DeleteTenantTransactionActor<Transaction>>(tr, tenantId, clusterType)
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("deleteTenantTransaction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DeleteTenantTransactionActor<Transaction>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DeleteTenantTransactionActor<Transaction>, 1, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< DeleteTenantTransactionActor<Transaction>, 2, RangeResult >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< DeleteTenantTransactionActor<Transaction>, 3, KeyBackedSet<Tuple>::RangeResultType >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< DeleteTenantTransactionActor<Transaction>, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<Void> deleteTenantTransaction( Transaction const& tr, int64_t const& tenantId, ClusterType const& clusterType = ClusterType::STANDALONE ) {
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<Void>(new DeleteTenantTransactionActor<Transaction>(tr, tenantId, clusterType));
															#line 5001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

// Deletes the tenant with the given name. If tenantId is specified, the tenant being deleted must also have the same
// ID.
															#line 5008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via deleteTenant()
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB, class DeleteTenantActor>
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class DeleteTenantActorState {
															#line 5014 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	DeleteTenantActorState(Reference<DB> const& db,TenantName const& name,Optional<int64_t> const& tenantId = Optional<int64_t>(),ClusterType const& clusterType = ClusterType::STANDALONE) 
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : db(db),
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   name(name),
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tenantId(tenantId),
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   clusterType(clusterType),
															#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tr(db->createTransaction()),
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   checkExistence(clusterType == ClusterType::STANDALONE)
															#line 5031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("deleteTenant", reinterpret_cast<unsigned long>(this));

	}
	~DeleteTenantActorState() 
	{
		fdb_probe_actor_destroy("deleteTenant", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			;
															#line 5046 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~DeleteTenantActorState();
		static_cast<DeleteTenantActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (checkExistence)
															#line 5081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				StrictFuture<Optional<int64_t>> __when_expr_0 = TenantMetadata::tenantNameIndex().get(tr, name);
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				if (static_cast<DeleteTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<DeleteTenantActor*>(this)->actor_wait_state = 1;
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantActor, 0, Optional<int64_t> >*>(static_cast<DeleteTenantActor*>(this)));
															#line 5092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_3 = safeThreadFutureToFuture(tr->onError(e));
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<DeleteTenantActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<DeleteTenantActor*>(this)->actor_wait_state = 4;
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantActor, 3, Void >*>(static_cast<DeleteTenantActor*>(this)));
															#line 5126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Void> __when_expr_1 = deleteTenantTransaction(tr, tenantId.get(), clusterType);
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<DeleteTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<DeleteTenantActor*>(this)->actor_wait_state = 2;
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantActor, 1, Void >*>(static_cast<DeleteTenantActor*>(this)));
															#line 5148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Optional<int64_t> const& actualId,int loopDepth) 
	{
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!actualId.present() || (tenantId.present() && tenantId != actualId))
															#line 5157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1loopBody1Catch1(tenant_not_found(), loopDepth);
															#line 5161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		tenantId = actualId;
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		checkExistence = false;
															#line 5167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Optional<int64_t> && actualId,int loopDepth) 
	{
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!actualId.present() || (tenantId.present() && tenantId != actualId))
															#line 5176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1loopBody1Catch1(tenant_not_found(), loopDepth);
															#line 5180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		tenantId = actualId;
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		checkExistence = false;
															#line 5186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<int64_t> const& actualId,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(actualId, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<int64_t> && actualId,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(actualId), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DeleteTenantActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantActor*>(this)->ActorCallback< DeleteTenantActor, 0, Optional<int64_t> >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 0, Optional<int64_t> >*,Optional<int64_t> const& value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 0, Optional<int64_t> >*,Optional<int64_t> && value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DeleteTenantActor, 0, Optional<int64_t> >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Void> __when_expr_2 = buggifiedCommit(tr, BUGGIFY_WITH_PROB(0.1));
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<DeleteTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont6when1(__when_expr_2.get(), loopDepth); };
		static_cast<DeleteTenantActor*>(this)->actor_wait_state = 3;
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantActor, 2, Void >*>(static_cast<DeleteTenantActor*>(this)));
															#line 5265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Void> __when_expr_2 = buggifiedCommit(tr, BUGGIFY_WITH_PROB(0.1));
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<DeleteTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont6when1(__when_expr_2.get(), loopDepth); };
		static_cast<DeleteTenantActor*>(this)->actor_wait_state = 3;
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantActor, 2, Void >*>(static_cast<DeleteTenantActor*>(this)));
															#line 5281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DeleteTenantActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantActor*>(this)->ActorCallback< DeleteTenantActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DeleteTenantActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont7(Void const& _,int loopDepth) 
	{
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TraceEvent("DeletedTenant") .detail("Tenant", name) .detail("TenantId", tenantId) .detail("Version", tr->getCommittedVersion());
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<DeleteTenantActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteTenantActorState(); static_cast<DeleteTenantActor*>(this)->destroy(); return 0; }
															#line 5355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<DeleteTenantActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteTenantActorState();
		static_cast<DeleteTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void && _,int loopDepth) 
	{
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TraceEvent("DeletedTenant") .detail("Tenant", name) .detail("TenantId", tenantId) .detail("Version", tr->getCommittedVersion());
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<DeleteTenantActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteTenantActorState(); static_cast<DeleteTenantActor*>(this)->destroy(); return 0; }
															#line 5369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<DeleteTenantActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteTenantActorState();
		static_cast<DeleteTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<DeleteTenantActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantActor*>(this)->ActorCallback< DeleteTenantActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< DeleteTenantActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 2);

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
		if (static_cast<DeleteTenantActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantActor*>(this)->ActorCallback< DeleteTenantActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< DeleteTenantActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Reference<DB> db;
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantName name;
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Optional<int64_t> tenantId;
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ClusterType clusterType;
															#line 408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	bool checkExistence;
															#line 5527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via deleteTenant()
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB>
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class DeleteTenantActor final : public Actor<Void>, public ActorCallback< DeleteTenantActor<DB>, 0, Optional<int64_t> >, public ActorCallback< DeleteTenantActor<DB>, 1, Void >, public ActorCallback< DeleteTenantActor<DB>, 2, Void >, public ActorCallback< DeleteTenantActor<DB>, 3, Void >, public FastAllocated<DeleteTenantActor<DB>>, public DeleteTenantActorState<DB, DeleteTenantActor<DB>> {
															#line 5534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<DeleteTenantActor<DB>>::operator new;
	using FastAllocated<DeleteTenantActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DeleteTenantActor<DB>, 0, Optional<int64_t> >;
friend struct ActorCallback< DeleteTenantActor<DB>, 1, Void >;
friend struct ActorCallback< DeleteTenantActor<DB>, 2, Void >;
friend struct ActorCallback< DeleteTenantActor<DB>, 3, Void >;
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	DeleteTenantActor(Reference<DB> const& db,TenantName const& name,Optional<int64_t> const& tenantId = Optional<int64_t>(),ClusterType const& clusterType = ClusterType::STANDALONE) 
															#line 5548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<Void>(),
		   DeleteTenantActorState<DB, DeleteTenantActor<DB>>(db, name, tenantId, clusterType)
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("deleteTenant");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DeleteTenantActor<DB>, 0, Optional<int64_t> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DeleteTenantActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< DeleteTenantActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< DeleteTenantActor<DB>, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB>
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<Void> deleteTenant( Reference<DB> const& db, TenantName const& name, Optional<int64_t> const& tenantId = Optional<int64_t>(), ClusterType const& clusterType = ClusterType::STANDALONE ) {
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<Void>(new DeleteTenantActor<DB>(db, name, tenantId, clusterType));
															#line 5580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

// This should only be called from a transaction that has already confirmed that the tenant entry
// is present. The tenantEntry should start with the existing entry and modify only those fields that need
// to be changed. This must only be called on a non-management cluster.
															#line 5588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via configureTenantTransaction()
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class ConfigureTenantTransactionActor>
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class ConfigureTenantTransactionActorState {
															#line 5594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ConfigureTenantTransactionActorState(Transaction const& tr,TenantMapEntry const& originalEntry,TenantMapEntry const& updatedTenantEntry) 
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : tr(tr),
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   originalEntry(originalEntry),
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   updatedTenantEntry(updatedTenantEntry)
															#line 5605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("configureTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	~ConfigureTenantTransactionActorState() 
	{
		fdb_probe_actor_destroy("configureTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			ASSERT(updatedTenantEntry.id == originalEntry.id);
															#line 449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::tenantMap().set(tr, updatedTenantEntry.id, updatedTenantEntry);
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::lastTenantModification().setVersionstamp(tr, Versionstamp(), 0);
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (originalEntry.tenantGroup != updatedTenantEntry.tenantGroup)
															#line 5628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				if (updatedTenantEntry.tenantGroup.present() && updatedTenantEntry.tenantGroup.get().startsWith("\xff"_sr))
															#line 5632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
				{
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
					return a_body1Catch1(invalid_tenant_group_name(), loopDepth);
															#line 5636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
				}
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				if (originalEntry.tenantGroup.present())
															#line 5640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
				{
															#line 460 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
					TenantMetadata::tenantGroupTenantIndex().erase( tr, Tuple::makeTuple(originalEntry.tenantGroup.get(), updatedTenantEntry.id));
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
					StrictFuture<KeyBackedSet<Tuple>::RangeResultType> __when_expr_0 = TenantMetadata::tenantGroupTenantIndex().getRange( tr, Tuple::makeTuple(originalEntry.tenantGroup.get()), Tuple::makeTuple(keyAfter(originalEntry.tenantGroup.get())), 2);
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
					if (static_cast<ConfigureTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<ConfigureTenantTransactionActor*>(this)->actor_wait_state = 1;
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ConfigureTenantTransactionActor, 0, KeyBackedSet<Tuple>::RangeResultType >*>(static_cast<ConfigureTenantTransactionActor*>(this)));
															#line 5653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~ConfigureTenantTransactionActorState();
		static_cast<ConfigureTenantTransactionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		ASSERT_EQ(updatedTenantEntry.tenantLockId.present(), updatedTenantEntry.tenantLockState != TenantLockState::UNLOCKED);
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<ConfigureTenantTransactionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ConfigureTenantTransactionActorState(); static_cast<ConfigureTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 5688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<ConfigureTenantTransactionActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ConfigureTenantTransactionActorState();
		static_cast<ConfigureTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(int loopDepth) 
	{
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (updatedTenantEntry.tenantGroup.present())
															#line 5700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Optional<TenantGroupEntry>> __when_expr_1 = TenantMetadata::tenantGroupMap().get(tr, updatedTenantEntry.tenantGroup.get());
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<ConfigureTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont2when1(__when_expr_1.get(), loopDepth); };
			static_cast<ConfigureTenantTransactionActor*>(this)->actor_wait_state = 2;
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ConfigureTenantTransactionActor, 1, Optional<TenantGroupEntry> >*>(static_cast<ConfigureTenantTransactionActor*>(this)));
															#line 5711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont7(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont4(KeyBackedSet<Tuple>::RangeResultType const& tenants,int loopDepth) 
	{
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tenants.results.empty() || (tenants.results.size() == 1 && tenants.results[0].getInt(1) == updatedTenantEntry.id))
															#line 5725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::tenantGroupMap().erase(tr, originalEntry.tenantGroup.get());
															#line 5729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(KeyBackedSet<Tuple>::RangeResultType && tenants,int loopDepth) 
	{
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (tenants.results.empty() || (tenants.results.size() == 1 && tenants.results[0].getInt(1) == updatedTenantEntry.id))
															#line 5739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::tenantGroupMap().erase(tr, originalEntry.tenantGroup.get());
															#line 5743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(KeyBackedSet<Tuple>::RangeResultType const& tenants,int loopDepth) 
	{
		loopDepth = a_body1cont4(tenants, loopDepth);

		return loopDepth;
	}
	int a_body1when1(KeyBackedSet<Tuple>::RangeResultType && tenants,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(tenants), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ConfigureTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<ConfigureTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<ConfigureTenantTransactionActor*>(this)->ActorCallback< ConfigureTenantTransactionActor, 0, KeyBackedSet<Tuple>::RangeResultType >::remove();

	}
	void a_callback_fire(ActorCallback< ConfigureTenantTransactionActor, 0, KeyBackedSet<Tuple>::RangeResultType >*,KeyBackedSet<Tuple>::RangeResultType const& value) 
	{
		fdb_probe_actor_enter("configureTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("configureTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ConfigureTenantTransactionActor, 0, KeyBackedSet<Tuple>::RangeResultType >*,KeyBackedSet<Tuple>::RangeResultType && value) 
	{
		fdb_probe_actor_enter("configureTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("configureTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ConfigureTenantTransactionActor, 0, KeyBackedSet<Tuple>::RangeResultType >*,Error err) 
	{
		fdb_probe_actor_enter("configureTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("configureTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont7(int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont8(Optional<TenantGroupEntry> const& entry,int loopDepth) 
	{
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!entry.present())
															#line 5822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::tenantGroupMap().set(tr, updatedTenantEntry.tenantGroup.get(), TenantGroupEntry());
															#line 5826 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantGroupTenantIndex().insert( tr, Tuple::makeTuple(updatedTenantEntry.tenantGroup.get(), updatedTenantEntry.id));
															#line 5830 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont8(Optional<TenantGroupEntry> && entry,int loopDepth) 
	{
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!entry.present())
															#line 5839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMetadata::tenantGroupMap().set(tr, updatedTenantEntry.tenantGroup.get(), TenantGroupEntry());
															#line 5843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantGroupTenantIndex().insert( tr, Tuple::makeTuple(updatedTenantEntry.tenantGroup.get(), updatedTenantEntry.id));
															#line 5847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Optional<TenantGroupEntry> const& entry,int loopDepth) 
	{
		loopDepth = a_body1cont8(entry, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Optional<TenantGroupEntry> && entry,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(entry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ConfigureTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<ConfigureTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<ConfigureTenantTransactionActor*>(this)->ActorCallback< ConfigureTenantTransactionActor, 1, Optional<TenantGroupEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< ConfigureTenantTransactionActor, 1, Optional<TenantGroupEntry> >*,Optional<TenantGroupEntry> const& value) 
	{
		fdb_probe_actor_enter("configureTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("configureTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ConfigureTenantTransactionActor, 1, Optional<TenantGroupEntry> >*,Optional<TenantGroupEntry> && value) 
	{
		fdb_probe_actor_enter("configureTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("configureTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ConfigureTenantTransactionActor, 1, Optional<TenantGroupEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("configureTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("configureTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Transaction tr;
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantMapEntry originalEntry;
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantMapEntry updatedTenantEntry;
															#line 5921 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via configureTenantTransaction()
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class ConfigureTenantTransactionActor final : public Actor<Void>, public ActorCallback< ConfigureTenantTransactionActor<Transaction>, 0, KeyBackedSet<Tuple>::RangeResultType >, public ActorCallback< ConfigureTenantTransactionActor<Transaction>, 1, Optional<TenantGroupEntry> >, public FastAllocated<ConfigureTenantTransactionActor<Transaction>>, public ConfigureTenantTransactionActorState<Transaction, ConfigureTenantTransactionActor<Transaction>> {
															#line 5928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<ConfigureTenantTransactionActor<Transaction>>::operator new;
	using FastAllocated<ConfigureTenantTransactionActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ConfigureTenantTransactionActor<Transaction>, 0, KeyBackedSet<Tuple>::RangeResultType >;
friend struct ActorCallback< ConfigureTenantTransactionActor<Transaction>, 1, Optional<TenantGroupEntry> >;
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ConfigureTenantTransactionActor(Transaction const& tr,TenantMapEntry const& originalEntry,TenantMapEntry const& updatedTenantEntry) 
															#line 5940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<Void>(),
		   ConfigureTenantTransactionActorState<Transaction, ConfigureTenantTransactionActor<Transaction>>(tr, originalEntry, updatedTenantEntry)
	{
		fdb_probe_actor_enter("configureTenantTransaction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("configureTenantTransaction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("configureTenantTransaction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ConfigureTenantTransactionActor<Transaction>, 0, KeyBackedSet<Tuple>::RangeResultType >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ConfigureTenantTransactionActor<Transaction>, 1, Optional<TenantGroupEntry> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<Void> configureTenantTransaction( Transaction const& tr, TenantMapEntry const& originalEntry, TenantMapEntry const& updatedTenantEntry ) {
															#line 443 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<Void>(new ConfigureTenantTransactionActor<Transaction>(tr, originalEntry, updatedTenantEntry));
															#line 5970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

template <class TenantMapEntryT>
bool checkLockState(TenantMapEntryT entry, TenantLockState desiredLockState, UID lockId) {
	if (entry.tenantLockId == lockId && entry.tenantLockState == desiredLockState) {
		return true;
	}

	if (entry.tenantLockId.present() && entry.tenantLockId.get() != lockId) {
		throw tenant_locked();
	}

	return false;
}

															#line 5988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via changeLockState()
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class ChangeLockStateActor>
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class ChangeLockStateActorState {
															#line 5994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ChangeLockStateActorState(Transaction const& tr,int64_t const& tenant,TenantLockState const& desiredLockState,UID const& lockId) 
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : tr(tr),
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tenant(tenant),
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   desiredLockState(desiredLockState),
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   lockId(lockId),
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tenantModeCheck(TenantAPI::checkTenantMode(tr, ClusterType::STANDALONE))
															#line 6009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("changeLockState", reinterpret_cast<unsigned long>(this));

	}
	~ChangeLockStateActorState() 
	{
		fdb_probe_actor_destroy("changeLockState", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<TenantMapEntry> __when_expr_0 = TenantAPI::getTenantTransaction(tr, tenant);
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<ChangeLockStateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ChangeLockStateActor*>(this)->actor_wait_state = 1;
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ChangeLockStateActor, 0, TenantMapEntry >*>(static_cast<ChangeLockStateActor*>(this)));
															#line 6031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~ChangeLockStateActorState();
		static_cast<ChangeLockStateActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Void> __when_expr_1 = tenantModeCheck;
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<ChangeLockStateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ChangeLockStateActor*>(this)->actor_wait_state = 2;
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChangeLockStateActor, 1, Void >*>(static_cast<ChangeLockStateActor*>(this)));
															#line 6061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(TenantMapEntry const& __entry,int loopDepth) 
	{
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		entry = __entry;
															#line 6070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(TenantMapEntry && __entry,int loopDepth) 
	{
		entry = std::move(__entry);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ChangeLockStateActor*>(this)->actor_wait_state > 0) static_cast<ChangeLockStateActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeLockStateActor*>(this)->ActorCallback< ChangeLockStateActor, 0, TenantMapEntry >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeLockStateActor, 0, TenantMapEntry >*,TenantMapEntry const& value) 
	{
		fdb_probe_actor_enter("changeLockState", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeLockState", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ChangeLockStateActor, 0, TenantMapEntry >*,TenantMapEntry && value) 
	{
		fdb_probe_actor_enter("changeLockState", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeLockState", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ChangeLockStateActor, 0, TenantMapEntry >*,Error err) 
	{
		fdb_probe_actor_enter("changeLockState", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeLockState", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!checkLockState(entry, desiredLockState, lockId))
															#line 6137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMapEntry newState = entry;
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			newState.tenantLockState = desiredLockState;
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			newState.tenantLockId = (desiredLockState == TenantLockState::UNLOCKED) ? Optional<UID>() : lockId;
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_2 = configureTenantTransaction(tr, entry, newState);
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<ChangeLockStateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<ChangeLockStateActor*>(this)->actor_wait_state = 3;
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ChangeLockStateActor, 2, Void >*>(static_cast<ChangeLockStateActor*>(this)));
															#line 6154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!checkLockState(entry, desiredLockState, lockId))
															#line 6168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			TenantMapEntry newState = entry;
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			newState.tenantLockState = desiredLockState;
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			newState.tenantLockId = (desiredLockState == TenantLockState::UNLOCKED) ? Optional<UID>() : lockId;
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_2 = configureTenantTransaction(tr, entry, newState);
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<ChangeLockStateActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<ChangeLockStateActor*>(this)->actor_wait_state = 3;
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ChangeLockStateActor, 2, Void >*>(static_cast<ChangeLockStateActor*>(this)));
															#line 6185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont3(loopDepth);
		}

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
		if (static_cast<ChangeLockStateActor*>(this)->actor_wait_state > 0) static_cast<ChangeLockStateActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeLockStateActor*>(this)->ActorCallback< ChangeLockStateActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeLockStateActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeLockState", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeLockState", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ChangeLockStateActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeLockState", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeLockState", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ChangeLockStateActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeLockState", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeLockState", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<ChangeLockStateActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ChangeLockStateActorState(); static_cast<ChangeLockStateActor*>(this)->destroy(); return 0; }
															#line 6262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<ChangeLockStateActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ChangeLockStateActorState();
		static_cast<ChangeLockStateActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

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
		if (static_cast<ChangeLockStateActor*>(this)->actor_wait_state > 0) static_cast<ChangeLockStateActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeLockStateActor*>(this)->ActorCallback< ChangeLockStateActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeLockStateActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeLockState", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeLockState", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ChangeLockStateActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeLockState", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeLockState", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ChangeLockStateActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeLockState", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeLockState", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Transaction tr;
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	int64_t tenant;
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantLockState desiredLockState;
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	UID lockId;
															#line 510 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Future<Void> tenantModeCheck;
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantMapEntry entry;
															#line 6357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via changeLockState()
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class ChangeLockStateActor final : public Actor<Void>, public ActorCallback< ChangeLockStateActor<Transaction>, 0, TenantMapEntry >, public ActorCallback< ChangeLockStateActor<Transaction>, 1, Void >, public ActorCallback< ChangeLockStateActor<Transaction>, 2, Void >, public FastAllocated<ChangeLockStateActor<Transaction>>, public ChangeLockStateActorState<Transaction, ChangeLockStateActor<Transaction>> {
															#line 6364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<ChangeLockStateActor<Transaction>>::operator new;
	using FastAllocated<ChangeLockStateActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ChangeLockStateActor<Transaction>, 0, TenantMapEntry >;
friend struct ActorCallback< ChangeLockStateActor<Transaction>, 1, Void >;
friend struct ActorCallback< ChangeLockStateActor<Transaction>, 2, Void >;
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ChangeLockStateActor(Transaction const& tr,int64_t const& tenant,TenantLockState const& desiredLockState,UID const& lockId) 
															#line 6377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<Void>(),
		   ChangeLockStateActorState<Transaction, ChangeLockStateActor<Transaction>>(tr, tenant, desiredLockState, lockId)
	{
		fdb_probe_actor_enter("changeLockState", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("changeLockState");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("changeLockState", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ChangeLockStateActor<Transaction>, 0, TenantMapEntry >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ChangeLockStateActor<Transaction>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ChangeLockStateActor<Transaction>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<Void> changeLockState( Transaction const& tr, int64_t const& tenant, TenantLockState const& desiredLockState, UID const& lockId ) {
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<Void>(new ChangeLockStateActor<Transaction>(tr, tenant, desiredLockState, lockId));
															#line 6408 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

template <class Transaction>
Future<std::vector<std::pair<TenantName, int64_t>>> listTenantsTransaction(Transaction tr,
                                                                           TenantName begin,
                                                                           TenantName end,
                                                                           int limit) {
	tr->setOption(FDBTransactionOptions::RAW_ACCESS);
	auto future = TenantMetadata::tenantNameIndex().getRange(tr, begin, end, limit);
	return fmap([](auto f) -> std::vector<std::pair<TenantName, int64_t>> { return f.results; }, future);
}

template <class DB>
Future<std::vector<std::pair<TenantName, int64_t>>> listTenants(Reference<DB> db,
                                                                TenantName begin,
                                                                TenantName end,
                                                                int limit) {
	return runTransaction(db, [=](Reference<typename DB::TransactionT> tr) {
		tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
		tr->setOption(FDBTransactionOptions::LOCK_AWARE);
		return listTenantsTransaction(tr, begin, end, limit);
	});
}

															#line 6435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via listTenantMetadataTransaction()
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class ListTenantMetadataTransactionActor>
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class ListTenantMetadataTransactionActorState {
															#line 6441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ListTenantMetadataTransactionActorState(Transaction const& tr,TenantName const& begin,TenantName const& end,int const& limit) 
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : tr(tr),
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   begin(begin),
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   end(end),
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   limit(limit)
															#line 6454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this));

	}
	~ListTenantMetadataTransactionActorState() 
	{
		fdb_probe_actor_destroy("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<std::vector<std::pair<TenantName, int64_t>>> __when_expr_0 = listTenantsTransaction(tr, begin, end, limit);
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<ListTenantMetadataTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ListTenantMetadataTransactionActor*>(this)->actor_wait_state = 1;
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ListTenantMetadataTransactionActor, 0, std::vector<std::pair<TenantName, int64_t>> >*>(static_cast<ListTenantMetadataTransactionActor*>(this)));
															#line 6476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~ListTenantMetadataTransactionActorState();
		static_cast<ListTenantMetadataTransactionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<std::pair<TenantName, int64_t>> const& matchingTenants,int loopDepth) 
	{
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		tenantEntryFutures = std::vector<Future<TenantMapEntry>>();
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		for( auto const& [name, id] : matchingTenants ) {
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tenantEntryFutures.push_back(getTenantTransaction(tr, id));
															#line 6503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Void> __when_expr_1 = waitForAll(tenantEntryFutures);
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<ListTenantMetadataTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ListTenantMetadataTransactionActor*>(this)->actor_wait_state = 2;
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ListTenantMetadataTransactionActor, 1, Void >*>(static_cast<ListTenantMetadataTransactionActor*>(this)));
															#line 6514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(std::vector<std::pair<TenantName, int64_t>> && matchingTenants,int loopDepth) 
	{
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		tenantEntryFutures = std::vector<Future<TenantMapEntry>>();
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		for( auto const& [name, id] : matchingTenants ) {
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tenantEntryFutures.push_back(getTenantTransaction(tr, id));
															#line 6527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Void> __when_expr_1 = waitForAll(tenantEntryFutures);
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<ListTenantMetadataTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ListTenantMetadataTransactionActor*>(this)->actor_wait_state = 2;
															#line 559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ListTenantMetadataTransactionActor, 1, Void >*>(static_cast<ListTenantMetadataTransactionActor*>(this)));
															#line 6538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(std::vector<std::pair<TenantName, int64_t>> const& matchingTenants,int loopDepth) 
	{
		loopDepth = a_body1cont1(matchingTenants, loopDepth);

		return loopDepth;
	}
	int a_body1when1(std::vector<std::pair<TenantName, int64_t>> && matchingTenants,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(matchingTenants), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ListTenantMetadataTransactionActor*>(this)->actor_wait_state > 0) static_cast<ListTenantMetadataTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<ListTenantMetadataTransactionActor*>(this)->ActorCallback< ListTenantMetadataTransactionActor, 0, std::vector<std::pair<TenantName, int64_t>> >::remove();

	}
	void a_callback_fire(ActorCallback< ListTenantMetadataTransactionActor, 0, std::vector<std::pair<TenantName, int64_t>> >*,std::vector<std::pair<TenantName, int64_t>> const& value) 
	{
		fdb_probe_actor_enter("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ListTenantMetadataTransactionActor, 0, std::vector<std::pair<TenantName, int64_t>> >*,std::vector<std::pair<TenantName, int64_t>> && value) 
	{
		fdb_probe_actor_enter("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ListTenantMetadataTransactionActor, 0, std::vector<std::pair<TenantName, int64_t>> >*,Error err) 
	{
		fdb_probe_actor_enter("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		std::vector<std::pair<TenantName, TenantMapEntry>> results;
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		for( auto const& f : tenantEntryFutures ) {
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			results.emplace_back(f.get().tenantName, f.get());
															#line 6614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<ListTenantMetadataTransactionActor*>(this)->SAV<std::vector<std::pair<TenantName, TenantMapEntry>>>::futures) { (void)(results); this->~ListTenantMetadataTransactionActorState(); static_cast<ListTenantMetadataTransactionActor*>(this)->destroy(); return 0; }
															#line 6618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<ListTenantMetadataTransactionActor*>(this)->SAV< std::vector<std::pair<TenantName, TenantMapEntry>> >::value()) std::vector<std::pair<TenantName, TenantMapEntry>>(results);
		this->~ListTenantMetadataTransactionActorState();
		static_cast<ListTenantMetadataTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		std::vector<std::pair<TenantName, TenantMapEntry>> results;
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		for( auto const& f : tenantEntryFutures ) {
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			results.emplace_back(f.get().tenantName, f.get());
															#line 6634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<ListTenantMetadataTransactionActor*>(this)->SAV<std::vector<std::pair<TenantName, TenantMapEntry>>>::futures) { (void)(results); this->~ListTenantMetadataTransactionActorState(); static_cast<ListTenantMetadataTransactionActor*>(this)->destroy(); return 0; }
															#line 6638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<ListTenantMetadataTransactionActor*>(this)->SAV< std::vector<std::pair<TenantName, TenantMapEntry>> >::value()) std::vector<std::pair<TenantName, TenantMapEntry>>(results);
		this->~ListTenantMetadataTransactionActorState();
		static_cast<ListTenantMetadataTransactionActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<ListTenantMetadataTransactionActor*>(this)->actor_wait_state > 0) static_cast<ListTenantMetadataTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<ListTenantMetadataTransactionActor*>(this)->ActorCallback< ListTenantMetadataTransactionActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ListTenantMetadataTransactionActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ListTenantMetadataTransactionActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ListTenantMetadataTransactionActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Transaction tr;
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantName begin;
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantName end;
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	int limit;
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	std::vector<Future<TenantMapEntry>> tenantEntryFutures;
															#line 6719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via listTenantMetadataTransaction()
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class ListTenantMetadataTransactionActor final : public Actor<std::vector<std::pair<TenantName, TenantMapEntry>>>, public ActorCallback< ListTenantMetadataTransactionActor<Transaction>, 0, std::vector<std::pair<TenantName, int64_t>> >, public ActorCallback< ListTenantMetadataTransactionActor<Transaction>, 1, Void >, public FastAllocated<ListTenantMetadataTransactionActor<Transaction>>, public ListTenantMetadataTransactionActorState<Transaction, ListTenantMetadataTransactionActor<Transaction>> {
															#line 6726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<ListTenantMetadataTransactionActor<Transaction>>::operator new;
	using FastAllocated<ListTenantMetadataTransactionActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<std::pair<TenantName, TenantMapEntry>>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ListTenantMetadataTransactionActor<Transaction>, 0, std::vector<std::pair<TenantName, int64_t>> >;
friend struct ActorCallback< ListTenantMetadataTransactionActor<Transaction>, 1, Void >;
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ListTenantMetadataTransactionActor(Transaction const& tr,TenantName const& begin,TenantName const& end,int const& limit) 
															#line 6738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<std::vector<std::pair<TenantName, TenantMapEntry>>>(),
		   ListTenantMetadataTransactionActorState<Transaction, ListTenantMetadataTransactionActor<Transaction>>(tr, begin, end, limit)
	{
		fdb_probe_actor_enter("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("listTenantMetadataTransaction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("listTenantMetadataTransaction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ListTenantMetadataTransactionActor<Transaction>, 0, std::vector<std::pair<TenantName, int64_t>> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ListTenantMetadataTransactionActor<Transaction>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<std::vector<std::pair<TenantName, TenantMapEntry>>> listTenantMetadataTransaction( Transaction const& tr, TenantName const& begin, TenantName const& end, int const& limit ) {
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<std::vector<std::pair<TenantName, TenantMapEntry>>>(new ListTenantMetadataTransactionActor<Transaction>(tr, begin, end, limit));
															#line 6768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

template <class DB>
Future<std::vector<std::pair<TenantName, TenantMapEntry>>> listTenantMetadata(Reference<DB> db,
                                                                              TenantName begin,
                                                                              TenantName end,
                                                                              int limit) {
	return runTransaction(db, [=](Reference<typename DB::TransactionT> tr) {
		tr->setOption(FDBTransactionOptions::LOCK_AWARE);
		tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
		return listTenantMetadataTransaction(tr, begin, end, limit);
	});
}

															#line 6785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via renameTenantTransaction()
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class RenameTenantTransactionActor>
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class RenameTenantTransactionActorState {
															#line 6791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	RenameTenantTransactionActorState(Transaction const& tr,TenantName const& oldName,TenantName const& newName,Optional<int64_t> const& tenantId = Optional<int64_t>(),ClusterType const& clusterType = ClusterType::STANDALONE,Optional<int64_t> const& configureSequenceNum = Optional<int64_t>()) 
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : tr(tr),
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   oldName(oldName),
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   newName(newName),
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tenantId(tenantId),
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   clusterType(clusterType),
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   configureSequenceNum(configureSequenceNum)
															#line 6808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("renameTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	~RenameTenantTransactionActorState() 
	{
		fdb_probe_actor_destroy("renameTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			ASSERT(clusterType == ClusterType::STANDALONE || (tenantId.present() && configureSequenceNum.present()));
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			ASSERT(clusterType != ClusterType::METACLUSTER_MANAGEMENT);
															#line 591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tenantModeCheck = checkTenantMode(tr, clusterType);
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			oldNameIdFuture = tenantId.present() ? Future<Optional<int64_t>>() : TenantMetadata::tenantNameIndex().get(tr, oldName);
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			newNameIdFuture = TenantMetadata::tenantNameIndex().get(tr, newName);
															#line 598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_0 = tenantModeCheck;
															#line 598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state = 1;
															#line 598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RenameTenantTransactionActor, 0, Void >*>(static_cast<RenameTenantTransactionActor*>(this)));
															#line 6842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~RenameTenantTransactionActorState();
		static_cast<RenameTenantTransactionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!tenantId.present())
															#line 6865 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_1 = store(tenantId, oldNameIdFuture);
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state = 2;
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RenameTenantTransactionActor, 1, Void >*>(static_cast<RenameTenantTransactionActor*>(this)));
															#line 6876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!tenantId.present())
															#line 6890 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_1 = store(tenantId, oldNameIdFuture);
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state = 2;
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RenameTenantTransactionActor, 1, Void >*>(static_cast<RenameTenantTransactionActor*>(this)));
															#line 6901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
		}

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
		if (static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<RenameTenantTransactionActor*>(this)->ActorCallback< RenameTenantTransactionActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RenameTenantTransactionActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RenameTenantTransactionActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RenameTenantTransactionActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<TenantMapEntry> __when_expr_2 = getTenantTransaction(tr, tenantId.get());
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state = 3;
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RenameTenantTransactionActor, 2, TenantMapEntry >*>(static_cast<RenameTenantTransactionActor*>(this)));
															#line 6985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!tenantId.present())
															#line 6994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_not_found(), loopDepth);
															#line 6998 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!tenantId.present())
															#line 7008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_not_found(), loopDepth);
															#line 7012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		if (static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<RenameTenantTransactionActor*>(this)->ActorCallback< RenameTenantTransactionActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RenameTenantTransactionActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RenameTenantTransactionActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RenameTenantTransactionActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont6(int loopDepth) 
	{
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Optional<int64_t>> __when_expr_3 = newNameIdFuture;
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state = 4;
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< RenameTenantTransactionActor, 3, Optional<int64_t> >*>(static_cast<RenameTenantTransactionActor*>(this)));
															#line 7092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2when1(TenantMapEntry const& __entry,int loopDepth) 
	{
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		entry = __entry;
															#line 7101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(TenantMapEntry && __entry,int loopDepth) 
	{
		entry = std::move(__entry);
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<RenameTenantTransactionActor*>(this)->ActorCallback< RenameTenantTransactionActor, 2, TenantMapEntry >::remove();

	}
	void a_callback_fire(ActorCallback< RenameTenantTransactionActor, 2, TenantMapEntry >*,TenantMapEntry const& value) 
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< RenameTenantTransactionActor, 2, TenantMapEntry >*,TenantMapEntry && value) 
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< RenameTenantTransactionActor, 2, TenantMapEntry >*,Error err) 
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont7(Optional<int64_t> const& newNameId,int loopDepth) 
	{
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (entry.tenantName != oldName)
															#line 7168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_not_found(), loopDepth);
															#line 7172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (newNameId.present())
															#line 7176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_already_exists(), loopDepth);
															#line 7180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (configureSequenceNum.present())
															#line 7184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (entry.configurationSequenceNum > configureSequenceNum.get())
															#line 7188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				if (!static_cast<RenameTenantTransactionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RenameTenantTransactionActorState(); static_cast<RenameTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 7192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
				new (&static_cast<RenameTenantTransactionActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~RenameTenantTransactionActorState();
				static_cast<RenameTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			entry.configurationSequenceNum = configureSequenceNum.get();
															#line 7200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		entry.tenantName = newName;
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantMap().set(tr, tenantId.get(), entry);
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantNameIndex().set(tr, newName, tenantId.get());
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantNameIndex().erase(tr, oldName);
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::lastTenantModification().setVersionstamp(tr, Versionstamp(), 0);
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (clusterType == ClusterType::METACLUSTER_DATA)
															#line 7214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_4 = markTenantTombstones(tr, tenantId.get());
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont7when1(__when_expr_4.get(), loopDepth); };
			static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state = 5;
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< RenameTenantTransactionActor, 4, Void >*>(static_cast<RenameTenantTransactionActor*>(this)));
															#line 7225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont8(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont7(Optional<int64_t> && newNameId,int loopDepth) 
	{
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (entry.tenantName != oldName)
															#line 7239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_not_found(), loopDepth);
															#line 7243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (newNameId.present())
															#line 7247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1Catch1(tenant_already_exists(), loopDepth);
															#line 7251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (configureSequenceNum.present())
															#line 7255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (entry.configurationSequenceNum > configureSequenceNum.get())
															#line 7259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 618 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				if (!static_cast<RenameTenantTransactionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RenameTenantTransactionActorState(); static_cast<RenameTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 7263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
				new (&static_cast<RenameTenantTransactionActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~RenameTenantTransactionActorState();
				static_cast<RenameTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			entry.configurationSequenceNum = configureSequenceNum.get();
															#line 7271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		entry.tenantName = newName;
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantMap().set(tr, tenantId.get(), entry);
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantNameIndex().set(tr, newName, tenantId.get());
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::tenantNameIndex().erase(tr, oldName);
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TenantMetadata::lastTenantModification().setVersionstamp(tr, Versionstamp(), 0);
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (clusterType == ClusterType::METACLUSTER_DATA)
															#line 7285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_4 = markTenantTombstones(tr, tenantId.get());
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont7when1(__when_expr_4.get(), loopDepth); };
			static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state = 5;
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< RenameTenantTransactionActor, 4, Void >*>(static_cast<RenameTenantTransactionActor*>(this)));
															#line 7296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont8(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont6when1(Optional<int64_t> const& newNameId,int loopDepth) 
	{
		loopDepth = a_body1cont7(newNameId, loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(Optional<int64_t> && newNameId,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(newNameId), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<RenameTenantTransactionActor*>(this)->ActorCallback< RenameTenantTransactionActor, 3, Optional<int64_t> >::remove();

	}
	void a_callback_fire(ActorCallback< RenameTenantTransactionActor, 3, Optional<int64_t> >*,Optional<int64_t> const& value) 
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< RenameTenantTransactionActor, 3, Optional<int64_t> >*,Optional<int64_t> && value) 
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< RenameTenantTransactionActor, 3, Optional<int64_t> >*,Error err) 
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont8(int loopDepth) 
	{
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<RenameTenantTransactionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RenameTenantTransactionActorState(); static_cast<RenameTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 7373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<RenameTenantTransactionActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RenameTenantTransactionActorState();
		static_cast<RenameTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont13(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1cont13(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1cont7when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont13(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont7when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont13(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<RenameTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<RenameTenantTransactionActor*>(this)->ActorCallback< RenameTenantTransactionActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RenameTenantTransactionActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont7when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< RenameTenantTransactionActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont7when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< RenameTenantTransactionActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Transaction tr;
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantName oldName;
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantName newName;
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Optional<int64_t> tenantId;
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ClusterType clusterType;
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Optional<int64_t> configureSequenceNum;
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Future<Void> tenantModeCheck;
															#line 594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Future<Optional<int64_t>> oldNameIdFuture;
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Future<Optional<int64_t>> newNameIdFuture;
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantMapEntry entry;
															#line 7476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via renameTenantTransaction()
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class RenameTenantTransactionActor final : public Actor<Void>, public ActorCallback< RenameTenantTransactionActor<Transaction>, 0, Void >, public ActorCallback< RenameTenantTransactionActor<Transaction>, 1, Void >, public ActorCallback< RenameTenantTransactionActor<Transaction>, 2, TenantMapEntry >, public ActorCallback< RenameTenantTransactionActor<Transaction>, 3, Optional<int64_t> >, public ActorCallback< RenameTenantTransactionActor<Transaction>, 4, Void >, public FastAllocated<RenameTenantTransactionActor<Transaction>>, public RenameTenantTransactionActorState<Transaction, RenameTenantTransactionActor<Transaction>> {
															#line 7483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<RenameTenantTransactionActor<Transaction>>::operator new;
	using FastAllocated<RenameTenantTransactionActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RenameTenantTransactionActor<Transaction>, 0, Void >;
friend struct ActorCallback< RenameTenantTransactionActor<Transaction>, 1, Void >;
friend struct ActorCallback< RenameTenantTransactionActor<Transaction>, 2, TenantMapEntry >;
friend struct ActorCallback< RenameTenantTransactionActor<Transaction>, 3, Optional<int64_t> >;
friend struct ActorCallback< RenameTenantTransactionActor<Transaction>, 4, Void >;
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	RenameTenantTransactionActor(Transaction const& tr,TenantName const& oldName,TenantName const& newName,Optional<int64_t> const& tenantId = Optional<int64_t>(),ClusterType const& clusterType = ClusterType::STANDALONE,Optional<int64_t> const& configureSequenceNum = Optional<int64_t>()) 
															#line 7498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<Void>(),
		   RenameTenantTransactionActorState<Transaction, RenameTenantTransactionActor<Transaction>>(tr, oldName, newName, tenantId, clusterType, configureSequenceNum)
	{
		fdb_probe_actor_enter("renameTenantTransaction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("renameTenantTransaction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("renameTenantTransaction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RenameTenantTransactionActor<Transaction>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RenameTenantTransactionActor<Transaction>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< RenameTenantTransactionActor<Transaction>, 2, TenantMapEntry >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< RenameTenantTransactionActor<Transaction>, 3, Optional<int64_t> >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< RenameTenantTransactionActor<Transaction>, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<Void> renameTenantTransaction( Transaction const& tr, TenantName const& oldName, TenantName const& newName, Optional<int64_t> const& tenantId = Optional<int64_t>(), ClusterType const& clusterType = ClusterType::STANDALONE, Optional<int64_t> const& configureSequenceNum = Optional<int64_t>() ) {
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<Void>(new RenameTenantTransactionActor<Transaction>(tr, oldName, newName, tenantId, clusterType, configureSequenceNum));
															#line 7531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

															#line 7536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via renameTenant()
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB, class RenameTenantActor>
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class RenameTenantActorState {
															#line 7542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	RenameTenantActorState(Reference<DB> const& db,TenantName const& oldName,TenantName const& newName,Optional<int64_t> const& tenantId = Optional<int64_t>(),ClusterType const& clusterType = ClusterType::STANDALONE) 
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : db(db),
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   oldName(oldName),
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   newName(newName),
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tenantId(tenantId),
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   clusterType(clusterType),
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tr(db->createTransaction())
															#line 7559 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("renameTenant", reinterpret_cast<unsigned long>(this));

	}
	~RenameTenantActorState() 
	{
		fdb_probe_actor_destroy("renameTenant", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			ASSERT(clusterType == ClusterType::STANDALONE || tenantId.present());
															#line 646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			firstTry = true;
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			;
															#line 7578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~RenameTenantActorState();
		static_cast<RenameTenantActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (!tenantId.present())
															#line 7611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				StrictFuture<Void> __when_expr_0 = store(tenantId, TenantMetadata::tenantNameIndex().get(tr, oldName));
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				if (static_cast<RenameTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<RenameTenantActor*>(this)->actor_wait_state = 1;
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< RenameTenantActor, 0, Void >*>(static_cast<RenameTenantActor*>(this)));
															#line 7622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_5 = safeThreadFutureToFuture(tr->onError(e));
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<RenameTenantActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 7651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_5.get(), loopDepth); };
			static_cast<RenameTenantActor*>(this)->actor_wait_state = 6;
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< RenameTenantActor, 5, Void >*>(static_cast<RenameTenantActor*>(this)));
															#line 7656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		newNameIdFuture = TenantMetadata::tenantNameIndex().get(tr, newName);
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<TenantMapEntry> __when_expr_1 = getTenantTransaction(tr, tenantId.get());
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<RenameTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<RenameTenantActor*>(this)->actor_wait_state = 2;
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< RenameTenantActor, 1, TenantMapEntry >*>(static_cast<RenameTenantActor*>(this)));
															#line 7680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!tenantId.present())
															#line 7689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1loopBody1Catch1(tenant_not_found(), loopDepth);
															#line 7693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 652 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!tenantId.present())
															#line 7703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			return a_body1loopBody1Catch1(tenant_not_found(), loopDepth);
															#line 7707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont2(loopDepth);

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
		if (static_cast<RenameTenantActor*>(this)->actor_wait_state > 0) static_cast<RenameTenantActor*>(this)->actor_wait_state = 0;
		static_cast<RenameTenantActor*>(this)->ActorCallback< RenameTenantActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RenameTenantActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< RenameTenantActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< RenameTenantActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont6(int loopDepth) 
	{
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Optional<int64_t>> __when_expr_2 = newNameIdFuture;
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<RenameTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont6when1(__when_expr_2.get(), loopDepth); };
		static_cast<RenameTenantActor*>(this)->actor_wait_state = 3;
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< RenameTenantActor, 2, Optional<int64_t> >*>(static_cast<RenameTenantActor*>(this)));
															#line 7787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(TenantMapEntry const& __entry,int loopDepth) 
	{
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		entry = __entry;
															#line 7796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(TenantMapEntry && __entry,int loopDepth) 
	{
		entry = std::move(__entry);
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<RenameTenantActor*>(this)->actor_wait_state > 0) static_cast<RenameTenantActor*>(this)->actor_wait_state = 0;
		static_cast<RenameTenantActor*>(this)->ActorCallback< RenameTenantActor, 1, TenantMapEntry >::remove();

	}
	void a_callback_fire(ActorCallback< RenameTenantActor, 1, TenantMapEntry >*,TenantMapEntry const& value) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< RenameTenantActor, 1, TenantMapEntry >*,TenantMapEntry && value) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< RenameTenantActor, 1, TenantMapEntry >*,Error err) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont7(Optional<int64_t> const& newNameId,int loopDepth) 
	{
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!firstTry && entry.tenantName == newName)
															#line 7863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (!static_cast<RenameTenantActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RenameTenantActorState(); static_cast<RenameTenantActor*>(this)->destroy(); return 0; }
															#line 7867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			new (&static_cast<RenameTenantActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~RenameTenantActorState();
			static_cast<RenameTenantActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (entry.tenantName != oldName)
															#line 7877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				return a_body1loopBody1Catch1(tenant_not_found(), loopDepth);
															#line 7881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			}
			else
			{
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				if (newNameId.present() && newNameId.get() != tenantId.get())
															#line 7887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
				{
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
					return a_body1loopBody1Catch1(tenant_already_exists(), loopDepth);
															#line 7891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
				}
			}
		}
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		firstTry = false;
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Void> __when_expr_3 = renameTenantTransaction(tr, oldName, newName, tenantId, clusterType);
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<RenameTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont7when1(__when_expr_3.get(), loopDepth); };
		static_cast<RenameTenantActor*>(this)->actor_wait_state = 4;
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< RenameTenantActor, 3, Void >*>(static_cast<RenameTenantActor*>(this)));
															#line 7906 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont7(Optional<int64_t> && newNameId,int loopDepth) 
	{
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!firstTry && entry.tenantName == newName)
															#line 7915 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		{
															#line 663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (!static_cast<RenameTenantActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RenameTenantActorState(); static_cast<RenameTenantActor*>(this)->destroy(); return 0; }
															#line 7919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			new (&static_cast<RenameTenantActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~RenameTenantActorState();
			static_cast<RenameTenantActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (entry.tenantName != oldName)
															#line 7929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			{
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				return a_body1loopBody1Catch1(tenant_not_found(), loopDepth);
															#line 7933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			}
			else
			{
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
				if (newNameId.present() && newNameId.get() != tenantId.get())
															#line 7939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
				{
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
					return a_body1loopBody1Catch1(tenant_already_exists(), loopDepth);
															#line 7943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
				}
			}
		}
															#line 670 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		firstTry = false;
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Void> __when_expr_3 = renameTenantTransaction(tr, oldName, newName, tenantId, clusterType);
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<RenameTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont7when1(__when_expr_3.get(), loopDepth); };
		static_cast<RenameTenantActor*>(this)->actor_wait_state = 4;
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< RenameTenantActor, 3, Void >*>(static_cast<RenameTenantActor*>(this)));
															#line 7958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Optional<int64_t> const& newNameId,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(newNameId, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Optional<int64_t> && newNameId,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(std::move(newNameId), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<RenameTenantActor*>(this)->actor_wait_state > 0) static_cast<RenameTenantActor*>(this)->actor_wait_state = 0;
		static_cast<RenameTenantActor*>(this)->ActorCallback< RenameTenantActor, 2, Optional<int64_t> >::remove();

	}
	void a_callback_fire(ActorCallback< RenameTenantActor, 2, Optional<int64_t> >*,Optional<int64_t> const& value) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< RenameTenantActor, 2, Optional<int64_t> >*,Optional<int64_t> && value) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< RenameTenantActor, 2, Optional<int64_t> >*,Error err) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont8(Void const& _,int loopDepth) 
	{
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Void> __when_expr_4 = buggifiedCommit(tr, BUGGIFY_WITH_PROB(0.1));
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<RenameTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8032 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1loopBody1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1loopBody1cont8when1(__when_expr_4.get(), loopDepth); };
		static_cast<RenameTenantActor*>(this)->actor_wait_state = 5;
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< RenameTenantActor, 4, Void >*>(static_cast<RenameTenantActor*>(this)));
															#line 8037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void && _,int loopDepth) 
	{
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		StrictFuture<Void> __when_expr_4 = buggifiedCommit(tr, BUGGIFY_WITH_PROB(0.1));
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (static_cast<RenameTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1loopBody1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1loopBody1cont8when1(__when_expr_4.get(), loopDepth); };
		static_cast<RenameTenantActor*>(this)->actor_wait_state = 5;
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< RenameTenantActor, 4, Void >*>(static_cast<RenameTenantActor*>(this)));
															#line 8053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont7when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<RenameTenantActor*>(this)->actor_wait_state > 0) static_cast<RenameTenantActor*>(this)->actor_wait_state = 0;
		static_cast<RenameTenantActor*>(this)->ActorCallback< RenameTenantActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RenameTenantActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont7when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< RenameTenantActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont7when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< RenameTenantActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1cont14(Void const& _,int loopDepth) 
	{
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TraceEvent("TenantRenamed") .detail("OldName", oldName) .detail("NewName", newName) .detail("TenantId", tenantId.get());
															#line 679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<RenameTenantActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RenameTenantActorState(); static_cast<RenameTenantActor*>(this)->destroy(); return 0; }
															#line 8127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<RenameTenantActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RenameTenantActorState();
		static_cast<RenameTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont14(Void && _,int loopDepth) 
	{
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		TraceEvent("TenantRenamed") .detail("OldName", oldName) .detail("NewName", newName) .detail("TenantId", tenantId.get());
															#line 679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<RenameTenantActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~RenameTenantActorState(); static_cast<RenameTenantActor*>(this)->destroy(); return 0; }
															#line 8141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<RenameTenantActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~RenameTenantActorState();
		static_cast<RenameTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont8when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont14(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont14(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<RenameTenantActor*>(this)->actor_wait_state > 0) static_cast<RenameTenantActor*>(this)->actor_wait_state = 0;
		static_cast<RenameTenantActor*>(this)->ActorCallback< RenameTenantActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RenameTenantActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont8when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< RenameTenantActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont8when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< RenameTenantActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 4);

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
		if (static_cast<RenameTenantActor*>(this)->actor_wait_state > 0) static_cast<RenameTenantActor*>(this)->actor_wait_state = 0;
		static_cast<RenameTenantActor*>(this)->ActorCallback< RenameTenantActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< RenameTenantActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< RenameTenantActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< RenameTenantActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), 5);

	}
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Reference<DB> db;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantName oldName;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantName newName;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Optional<int64_t> tenantId;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ClusterType clusterType;
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	bool firstTry;
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Future<Optional<int64_t>> newNameIdFuture;
															#line 658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantMapEntry entry;
															#line 8305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via renameTenant()
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB>
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class RenameTenantActor final : public Actor<Void>, public ActorCallback< RenameTenantActor<DB>, 0, Void >, public ActorCallback< RenameTenantActor<DB>, 1, TenantMapEntry >, public ActorCallback< RenameTenantActor<DB>, 2, Optional<int64_t> >, public ActorCallback< RenameTenantActor<DB>, 3, Void >, public ActorCallback< RenameTenantActor<DB>, 4, Void >, public ActorCallback< RenameTenantActor<DB>, 5, Void >, public FastAllocated<RenameTenantActor<DB>>, public RenameTenantActorState<DB, RenameTenantActor<DB>> {
															#line 8312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<RenameTenantActor<DB>>::operator new;
	using FastAllocated<RenameTenantActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< RenameTenantActor<DB>, 0, Void >;
friend struct ActorCallback< RenameTenantActor<DB>, 1, TenantMapEntry >;
friend struct ActorCallback< RenameTenantActor<DB>, 2, Optional<int64_t> >;
friend struct ActorCallback< RenameTenantActor<DB>, 3, Void >;
friend struct ActorCallback< RenameTenantActor<DB>, 4, Void >;
friend struct ActorCallback< RenameTenantActor<DB>, 5, Void >;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	RenameTenantActor(Reference<DB> const& db,TenantName const& oldName,TenantName const& newName,Optional<int64_t> const& tenantId = Optional<int64_t>(),ClusterType const& clusterType = ClusterType::STANDALONE) 
															#line 8328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<Void>(),
		   RenameTenantActorState<DB, RenameTenantActor<DB>>(db, oldName, newName, tenantId, clusterType)
	{
		fdb_probe_actor_enter("renameTenant", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("renameTenant");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("renameTenant", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< RenameTenantActor<DB>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< RenameTenantActor<DB>, 1, TenantMapEntry >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< RenameTenantActor<DB>, 2, Optional<int64_t> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< RenameTenantActor<DB>, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< RenameTenantActor<DB>, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< RenameTenantActor<DB>, 5, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB>
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<Void> renameTenant( Reference<DB> const& db, TenantName const& oldName, TenantName const& newName, Optional<int64_t> const& tenantId = Optional<int64_t>(), ClusterType const& clusterType = ClusterType::STANDALONE ) {
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<Void>(new RenameTenantActor<DB>(db, oldName, newName, tenantId, clusterType));
															#line 8362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

template <class Transaction>
Future<Optional<TenantGroupEntry>> tryGetTenantGroupTransaction(Transaction tr, TenantGroupName name) {
	tr->setOption(FDBTransactionOptions::RAW_ACCESS);
	return TenantMetadata::tenantGroupMap().get(tr, name);
}

															#line 8373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via tryGetTenantGroup()
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB, class TryGetTenantGroupActor>
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class TryGetTenantGroupActorState {
															#line 8379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TryGetTenantGroupActorState(Reference<DB> const& db,TenantGroupName const& name) 
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : db(db),
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   name(name),
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tr(db->createTransaction())
															#line 8390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("tryGetTenantGroup", reinterpret_cast<unsigned long>(this));

	}
	~TryGetTenantGroupActorState() 
	{
		fdb_probe_actor_destroy("tryGetTenantGroup", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			;
															#line 8405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~TryGetTenantGroupActorState();
		static_cast<TryGetTenantGroupActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::READ_LOCK_AWARE);
															#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Optional<TenantGroupEntry>> __when_expr_0 = tryGetTenantGroupTransaction(tr, name);
															#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<TryGetTenantGroupActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TryGetTenantGroupActor*>(this)->actor_wait_state = 1;
															#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TryGetTenantGroupActor, 0, Optional<TenantGroupEntry> >*>(static_cast<TryGetTenantGroupActor*>(this)));
															#line 8447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->onError(e));
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<TryGetTenantGroupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 8471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TryGetTenantGroupActor*>(this)->actor_wait_state = 2;
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TryGetTenantGroupActor, 1, Void >*>(static_cast<TryGetTenantGroupActor*>(this)));
															#line 8476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<TenantGroupEntry> const& entry,int loopDepth) 
	{
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<TryGetTenantGroupActor*>(this)->SAV<Optional<TenantGroupEntry>>::futures) { (void)(entry); this->~TryGetTenantGroupActorState(); static_cast<TryGetTenantGroupActor*>(this)->destroy(); return 0; }
															#line 8491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<TryGetTenantGroupActor*>(this)->SAV< Optional<TenantGroupEntry> >::value()) Optional<TenantGroupEntry>(entry);
		this->~TryGetTenantGroupActorState();
		static_cast<TryGetTenantGroupActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<TenantGroupEntry> && entry,int loopDepth) 
	{
															#line 701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<TryGetTenantGroupActor*>(this)->SAV<Optional<TenantGroupEntry>>::futures) { (void)(entry); this->~TryGetTenantGroupActorState(); static_cast<TryGetTenantGroupActor*>(this)->destroy(); return 0; }
															#line 8503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<TryGetTenantGroupActor*>(this)->SAV< Optional<TenantGroupEntry> >::value()) Optional<TenantGroupEntry>(entry);
		this->~TryGetTenantGroupActorState();
		static_cast<TryGetTenantGroupActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<TenantGroupEntry> const& entry,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(entry, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<TenantGroupEntry> && entry,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(entry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TryGetTenantGroupActor*>(this)->actor_wait_state > 0) static_cast<TryGetTenantGroupActor*>(this)->actor_wait_state = 0;
		static_cast<TryGetTenantGroupActor*>(this)->ActorCallback< TryGetTenantGroupActor, 0, Optional<TenantGroupEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< TryGetTenantGroupActor, 0, Optional<TenantGroupEntry> >*,Optional<TenantGroupEntry> const& value) 
	{
		fdb_probe_actor_enter("tryGetTenantGroup", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenantGroup", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TryGetTenantGroupActor, 0, Optional<TenantGroupEntry> >*,Optional<TenantGroupEntry> && value) 
	{
		fdb_probe_actor_enter("tryGetTenantGroup", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenantGroup", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TryGetTenantGroupActor, 0, Optional<TenantGroupEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("tryGetTenantGroup", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenantGroup", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<TryGetTenantGroupActor*>(this)->actor_wait_state > 0) static_cast<TryGetTenantGroupActor*>(this)->actor_wait_state = 0;
		static_cast<TryGetTenantGroupActor*>(this)->ActorCallback< TryGetTenantGroupActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TryGetTenantGroupActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tryGetTenantGroup", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenantGroup", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TryGetTenantGroupActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tryGetTenantGroup", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenantGroup", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TryGetTenantGroupActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tryGetTenantGroup", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenantGroup", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Reference<DB> db;
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantGroupName name;
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 8655 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via tryGetTenantGroup()
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB>
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class TryGetTenantGroupActor final : public Actor<Optional<TenantGroupEntry>>, public ActorCallback< TryGetTenantGroupActor<DB>, 0, Optional<TenantGroupEntry> >, public ActorCallback< TryGetTenantGroupActor<DB>, 1, Void >, public FastAllocated<TryGetTenantGroupActor<DB>>, public TryGetTenantGroupActorState<DB, TryGetTenantGroupActor<DB>> {
															#line 8662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<TryGetTenantGroupActor<DB>>::operator new;
	using FastAllocated<TryGetTenantGroupActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<TenantGroupEntry>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TryGetTenantGroupActor<DB>, 0, Optional<TenantGroupEntry> >;
friend struct ActorCallback< TryGetTenantGroupActor<DB>, 1, Void >;
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TryGetTenantGroupActor(Reference<DB> const& db,TenantGroupName const& name) 
															#line 8674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<Optional<TenantGroupEntry>>(),
		   TryGetTenantGroupActorState<DB, TryGetTenantGroupActor<DB>>(db, name)
	{
		fdb_probe_actor_enter("tryGetTenantGroup", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tryGetTenantGroup");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tryGetTenantGroup", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TryGetTenantGroupActor<DB>, 0, Optional<TenantGroupEntry> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TryGetTenantGroupActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB>
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<Optional<TenantGroupEntry>> tryGetTenantGroup( Reference<DB> const& db, TenantGroupName const& name ) {
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<Optional<TenantGroupEntry>>(new TryGetTenantGroupActor<DB>(db, name));
															#line 8704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

															#line 8709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via listTenantGroupsTransaction()
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction, class ListTenantGroupsTransactionActor>
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class ListTenantGroupsTransactionActorState {
															#line 8715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ListTenantGroupsTransactionActorState(Transaction const& tr,TenantGroupName const& begin,TenantGroupName const& end,int const& limit) 
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : tr(tr),
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   begin(begin),
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   end(end),
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   limit(limit)
															#line 8728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("listTenantGroupsTransaction", reinterpret_cast<unsigned long>(this));

	}
	~ListTenantGroupsTransactionActorState() 
	{
		fdb_probe_actor_destroy("listTenantGroupsTransaction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<KeyBackedRangeResult<std::pair<TenantGroupName, TenantGroupEntry>>> __when_expr_0 = TenantMetadata::tenantGroupMap().getRange(tr, begin, end, limit);
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<ListTenantGroupsTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 8747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ListTenantGroupsTransactionActor*>(this)->actor_wait_state = 1;
															#line 715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ListTenantGroupsTransactionActor, 0, KeyBackedRangeResult<std::pair<TenantGroupName, TenantGroupEntry>> >*>(static_cast<ListTenantGroupsTransactionActor*>(this)));
															#line 8752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~ListTenantGroupsTransactionActorState();
		static_cast<ListTenantGroupsTransactionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(KeyBackedRangeResult<std::pair<TenantGroupName, TenantGroupEntry>> const& results,int loopDepth) 
	{
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<ListTenantGroupsTransactionActor*>(this)->SAV<std::vector<std::pair<TenantGroupName, TenantGroupEntry>>>::futures) { (void)(results.results); this->~ListTenantGroupsTransactionActorState(); static_cast<ListTenantGroupsTransactionActor*>(this)->destroy(); return 0; }
															#line 8775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<ListTenantGroupsTransactionActor*>(this)->SAV< std::vector<std::pair<TenantGroupName, TenantGroupEntry>> >::value()) std::vector<std::pair<TenantGroupName, TenantGroupEntry>>(results.results);
		this->~ListTenantGroupsTransactionActorState();
		static_cast<ListTenantGroupsTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(KeyBackedRangeResult<std::pair<TenantGroupName, TenantGroupEntry>> && results,int loopDepth) 
	{
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<ListTenantGroupsTransactionActor*>(this)->SAV<std::vector<std::pair<TenantGroupName, TenantGroupEntry>>>::futures) { (void)(results.results); this->~ListTenantGroupsTransactionActorState(); static_cast<ListTenantGroupsTransactionActor*>(this)->destroy(); return 0; }
															#line 8787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<ListTenantGroupsTransactionActor*>(this)->SAV< std::vector<std::pair<TenantGroupName, TenantGroupEntry>> >::value()) std::vector<std::pair<TenantGroupName, TenantGroupEntry>>(results.results);
		this->~ListTenantGroupsTransactionActorState();
		static_cast<ListTenantGroupsTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(KeyBackedRangeResult<std::pair<TenantGroupName, TenantGroupEntry>> const& results,int loopDepth) 
	{
		loopDepth = a_body1cont1(results, loopDepth);

		return loopDepth;
	}
	int a_body1when1(KeyBackedRangeResult<std::pair<TenantGroupName, TenantGroupEntry>> && results,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(results), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ListTenantGroupsTransactionActor*>(this)->actor_wait_state > 0) static_cast<ListTenantGroupsTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<ListTenantGroupsTransactionActor*>(this)->ActorCallback< ListTenantGroupsTransactionActor, 0, KeyBackedRangeResult<std::pair<TenantGroupName, TenantGroupEntry>> >::remove();

	}
	void a_callback_fire(ActorCallback< ListTenantGroupsTransactionActor, 0, KeyBackedRangeResult<std::pair<TenantGroupName, TenantGroupEntry>> >*,KeyBackedRangeResult<std::pair<TenantGroupName, TenantGroupEntry>> const& value) 
	{
		fdb_probe_actor_enter("listTenantGroupsTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantGroupsTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ListTenantGroupsTransactionActor, 0, KeyBackedRangeResult<std::pair<TenantGroupName, TenantGroupEntry>> >*,KeyBackedRangeResult<std::pair<TenantGroupName, TenantGroupEntry>> && value) 
	{
		fdb_probe_actor_enter("listTenantGroupsTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantGroupsTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ListTenantGroupsTransactionActor, 0, KeyBackedRangeResult<std::pair<TenantGroupName, TenantGroupEntry>> >*,Error err) 
	{
		fdb_probe_actor_enter("listTenantGroupsTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantGroupsTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Transaction tr;
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantGroupName begin;
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantGroupName end;
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	int limit;
															#line 8866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via listTenantGroupsTransaction()
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class ListTenantGroupsTransactionActor final : public Actor<std::vector<std::pair<TenantGroupName, TenantGroupEntry>>>, public ActorCallback< ListTenantGroupsTransactionActor<Transaction>, 0, KeyBackedRangeResult<std::pair<TenantGroupName, TenantGroupEntry>> >, public FastAllocated<ListTenantGroupsTransactionActor<Transaction>>, public ListTenantGroupsTransactionActorState<Transaction, ListTenantGroupsTransactionActor<Transaction>> {
															#line 8873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<ListTenantGroupsTransactionActor<Transaction>>::operator new;
	using FastAllocated<ListTenantGroupsTransactionActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<std::pair<TenantGroupName, TenantGroupEntry>>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ListTenantGroupsTransactionActor<Transaction>, 0, KeyBackedRangeResult<std::pair<TenantGroupName, TenantGroupEntry>> >;
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ListTenantGroupsTransactionActor(Transaction const& tr,TenantGroupName const& begin,TenantGroupName const& end,int const& limit) 
															#line 8884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<std::vector<std::pair<TenantGroupName, TenantGroupEntry>>>(),
		   ListTenantGroupsTransactionActorState<Transaction, ListTenantGroupsTransactionActor<Transaction>>(tr, begin, end, limit)
	{
		fdb_probe_actor_enter("listTenantGroupsTransaction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("listTenantGroupsTransaction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("listTenantGroupsTransaction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ListTenantGroupsTransactionActor<Transaction>, 0, KeyBackedRangeResult<std::pair<TenantGroupName, TenantGroupEntry>> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class Transaction>
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<std::vector<std::pair<TenantGroupName, TenantGroupEntry>>> listTenantGroupsTransaction( Transaction const& tr, TenantGroupName const& begin, TenantGroupName const& end, int const& limit ) {
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<std::vector<std::pair<TenantGroupName, TenantGroupEntry>>>(new ListTenantGroupsTransactionActor<Transaction>(tr, begin, end, limit));
															#line 8913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

															#line 8918 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
// This generated class is to be used only via listTenantGroups()
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB, class ListTenantGroupsActor>
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class ListTenantGroupsActorState {
															#line 8924 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ListTenantGroupsActorState(Reference<DB> const& db,TenantGroupName const& begin,TenantGroupName const& end,int const& limit) 
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		 : db(db),
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   begin(begin),
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   end(end),
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   limit(limit),
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		   tr(db->createTransaction())
															#line 8939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
	{
		fdb_probe_actor_create("listTenantGroups", reinterpret_cast<unsigned long>(this));

	}
	~ListTenantGroupsActorState() 
	{
		fdb_probe_actor_destroy("listTenantGroups", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			;
															#line 8954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
		this->~ListTenantGroupsActorState();
		static_cast<ListTenantGroupsActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			tr->setOption(FDBTransactionOptions::READ_LOCK_AWARE);
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<std::vector<std::pair<TenantGroupName, TenantGroupEntry>>> __when_expr_0 = listTenantGroupsTransaction(tr, begin, end, limit);
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<ListTenantGroupsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 8991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ListTenantGroupsActor*>(this)->actor_wait_state = 1;
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ListTenantGroupsActor, 0, std::vector<std::pair<TenantGroupName, TenantGroupEntry>> >*>(static_cast<ListTenantGroupsActor*>(this)));
															#line 8996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
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
															#line 736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->onError(e));
															#line 736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			if (static_cast<ListTenantGroupsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 9020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ListTenantGroupsActor*>(this)->actor_wait_state = 2;
															#line 736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ListTenantGroupsActor, 1, Void >*>(static_cast<ListTenantGroupsActor*>(this)));
															#line 9025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::vector<std::pair<TenantGroupName, TenantGroupEntry>> const& tenantGroups,int loopDepth) 
	{
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<ListTenantGroupsActor*>(this)->SAV<std::vector<std::pair<TenantGroupName, TenantGroupEntry>>>::futures) { (void)(tenantGroups); this->~ListTenantGroupsActorState(); static_cast<ListTenantGroupsActor*>(this)->destroy(); return 0; }
															#line 9040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<ListTenantGroupsActor*>(this)->SAV< std::vector<std::pair<TenantGroupName, TenantGroupEntry>> >::value()) std::vector<std::pair<TenantGroupName, TenantGroupEntry>>(tenantGroups);
		this->~ListTenantGroupsActorState();
		static_cast<ListTenantGroupsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::vector<std::pair<TenantGroupName, TenantGroupEntry>> && tenantGroups,int loopDepth) 
	{
															#line 734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
		if (!static_cast<ListTenantGroupsActor*>(this)->SAV<std::vector<std::pair<TenantGroupName, TenantGroupEntry>>>::futures) { (void)(tenantGroups); this->~ListTenantGroupsActorState(); static_cast<ListTenantGroupsActor*>(this)->destroy(); return 0; }
															#line 9052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		new (&static_cast<ListTenantGroupsActor*>(this)->SAV< std::vector<std::pair<TenantGroupName, TenantGroupEntry>> >::value()) std::vector<std::pair<TenantGroupName, TenantGroupEntry>>(tenantGroups);
		this->~ListTenantGroupsActorState();
		static_cast<ListTenantGroupsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<std::pair<TenantGroupName, TenantGroupEntry>> const& tenantGroups,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(tenantGroups, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<std::pair<TenantGroupName, TenantGroupEntry>> && tenantGroups,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(tenantGroups), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ListTenantGroupsActor*>(this)->actor_wait_state > 0) static_cast<ListTenantGroupsActor*>(this)->actor_wait_state = 0;
		static_cast<ListTenantGroupsActor*>(this)->ActorCallback< ListTenantGroupsActor, 0, std::vector<std::pair<TenantGroupName, TenantGroupEntry>> >::remove();

	}
	void a_callback_fire(ActorCallback< ListTenantGroupsActor, 0, std::vector<std::pair<TenantGroupName, TenantGroupEntry>> >*,std::vector<std::pair<TenantGroupName, TenantGroupEntry>> const& value) 
	{
		fdb_probe_actor_enter("listTenantGroups", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantGroups", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ListTenantGroupsActor, 0, std::vector<std::pair<TenantGroupName, TenantGroupEntry>> >*,std::vector<std::pair<TenantGroupName, TenantGroupEntry>> && value) 
	{
		fdb_probe_actor_enter("listTenantGroups", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantGroups", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ListTenantGroupsActor, 0, std::vector<std::pair<TenantGroupName, TenantGroupEntry>> >*,Error err) 
	{
		fdb_probe_actor_enter("listTenantGroups", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantGroups", reinterpret_cast<unsigned long>(this), 0);

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
		if (static_cast<ListTenantGroupsActor*>(this)->actor_wait_state > 0) static_cast<ListTenantGroupsActor*>(this)->actor_wait_state = 0;
		static_cast<ListTenantGroupsActor*>(this)->ActorCallback< ListTenantGroupsActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ListTenantGroupsActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("listTenantGroups", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantGroups", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ListTenantGroupsActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("listTenantGroups", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantGroups", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ListTenantGroupsActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("listTenantGroups", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantGroups", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Reference<DB> db;
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantGroupName begin;
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	TenantGroupName end;
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	int limit;
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 9208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
};
// This generated class is to be used only via listTenantGroups()
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB>
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
class ListTenantGroupsActor final : public Actor<std::vector<std::pair<TenantGroupName, TenantGroupEntry>>>, public ActorCallback< ListTenantGroupsActor<DB>, 0, std::vector<std::pair<TenantGroupName, TenantGroupEntry>> >, public ActorCallback< ListTenantGroupsActor<DB>, 1, Void >, public FastAllocated<ListTenantGroupsActor<DB>>, public ListTenantGroupsActorState<DB, ListTenantGroupsActor<DB>> {
															#line 9215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
public:
	using FastAllocated<ListTenantGroupsActor<DB>>::operator new;
	using FastAllocated<ListTenantGroupsActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<std::pair<TenantGroupName, TenantGroupEntry>>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ListTenantGroupsActor<DB>, 0, std::vector<std::pair<TenantGroupName, TenantGroupEntry>> >;
friend struct ActorCallback< ListTenantGroupsActor<DB>, 1, Void >;
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	ListTenantGroupsActor(Reference<DB> const& db,TenantGroupName const& begin,TenantGroupName const& end,int const& limit) 
															#line 9227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
		 : Actor<std::vector<std::pair<TenantGroupName, TenantGroupEntry>>>(),
		   ListTenantGroupsActorState<DB, ListTenantGroupsActor<DB>>(db, begin, end, limit)
	{
		fdb_probe_actor_enter("listTenantGroups", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("listTenantGroups");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("listTenantGroups", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ListTenantGroupsActor<DB>, 0, std::vector<std::pair<TenantGroupName, TenantGroupEntry>> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ListTenantGroupsActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
template <class DB>
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
[[nodiscard]] Future<std::vector<std::pair<TenantGroupName, TenantGroupEntry>>> listTenantGroups( Reference<DB> const& db, TenantGroupName const& begin, TenantGroupName const& end, int const& limit ) {
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"
	return Future<std::vector<std::pair<TenantGroupName, TenantGroupEntry>>>(new ListTenantGroupsActor<DB>(db, begin, end, limit));
															#line 9257 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.g.h"
}

#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/TenantManagement.actor.h"

} // namespace TenantAPI

#include "flow/unactorcompiler.h"
#endif