#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
/*
 * SimpleConfigTransaction.actor.cpp
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

#include <algorithm>

#include "fdbclient/CommitTransaction.h"
#include "fdbclient/DatabaseContext.h"
#include "fdbclient/IKnobCollection.h"
#include "fdbclient/SimpleConfigTransaction.h"
#include "flow/Arena.h"
#include "flow/actorcompiler.h" // This must be the last #include.

class SimpleConfigTransactionImpl {
	ConfigTransactionCommitRequest toCommit;
	Future<ConfigGeneration> getGenerationFuture;
	ConfigTransactionInterface cti;
	int numRetries{ 0 };
	bool committed{ false };
	Optional<UID> dID;
	Database cx;

																#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
// This generated class is to be used only via getGeneration()
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
template <class GetGenerationActor>
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
class GetGenerationActorState {
															#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
public:
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	GetGenerationActorState(SimpleConfigTransactionImpl* const& self) 
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		 : self(self)
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
	{
		fdb_probe_actor_create("getGeneration", reinterpret_cast<unsigned long>(this));

	}
	~GetGenerationActorState() 
	{
		fdb_probe_actor_destroy("getGeneration", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (self->dID.present())
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			{
															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
				TraceEvent("SimpleConfigTransactionGettingReadVersion", self->dID.get());
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			}
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			reply = ConfigTransactionGetGenerationReply();
															#line 44 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (self->cti.hostname.present())
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			{
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
				StrictFuture<Void> __when_expr_0 = store(reply, retryGetReplyFromHostname(ConfigTransactionGetGenerationRequest{ 0, Optional<Version>() }, self->cti.hostname.get(), WLTOKEN_CONFIGTXN_GETGENERATION));
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
				if (static_cast<GetGenerationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<GetGenerationActor*>(this)->actor_wait_state = 1;
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetGenerationActor, 0, Void >*>(static_cast<GetGenerationActor*>(this)));
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
				StrictFuture<Void> __when_expr_1 = store(reply, retryBrokenPromise(self->cti.getGeneration, ConfigTransactionGetGenerationRequest{ 0, Optional<Version>() }));
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
				if (static_cast<GetGenerationActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
				static_cast<GetGenerationActor*>(this)->actor_wait_state = 2;
															#line 50 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetGenerationActor, 1, Void >*>(static_cast<GetGenerationActor*>(this)));
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
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
		this->~GetGenerationActorState();
		static_cast<GetGenerationActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (self->dID.present())
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		{
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			TraceEvent("SimpleConfigTransactionGotReadVersion", self->dID.get()) .detail("Version", reply.generation.liveVersion);
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		}
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (!static_cast<GetGenerationActor*>(this)->SAV<ConfigGeneration>::futures) { (void)(reply.generation); this->~GetGenerationActorState(); static_cast<GetGenerationActor*>(this)->destroy(); return 0; }
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		new (&static_cast<GetGenerationActor*>(this)->SAV< ConfigGeneration >::value()) ConfigGeneration(reply.generation);
		this->~GetGenerationActorState();
		static_cast<GetGenerationActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

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
		if (static_cast<GetGenerationActor*>(this)->actor_wait_state > 0) static_cast<GetGenerationActor*>(this)->actor_wait_state = 0;
		static_cast<GetGenerationActor*>(this)->ActorCallback< GetGenerationActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetGenerationActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getGeneration", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGeneration", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetGenerationActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getGeneration", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGeneration", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetGenerationActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getGeneration", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGeneration", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetGenerationActor*>(this)->actor_wait_state > 0) static_cast<GetGenerationActor*>(this)->actor_wait_state = 0;
		static_cast<GetGenerationActor*>(this)->ActorCallback< GetGenerationActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetGenerationActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getGeneration", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGeneration", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetGenerationActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getGeneration", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGeneration", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetGenerationActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getGeneration", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGeneration", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	SimpleConfigTransactionImpl* self;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	ConfigTransactionGetGenerationReply reply;
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via getGeneration()
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
class GetGenerationActor final : public Actor<ConfigGeneration>, public ActorCallback< GetGenerationActor, 0, Void >, public ActorCallback< GetGenerationActor, 1, Void >, public FastAllocated<GetGenerationActor>, public GetGenerationActorState<GetGenerationActor> {
															#line 303 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<GetGenerationActor>::operator new;
	using FastAllocated<GetGenerationActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<ConfigGeneration>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetGenerationActor, 0, Void >;
friend struct ActorCallback< GetGenerationActor, 1, Void >;
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	GetGenerationActor(SimpleConfigTransactionImpl* const& self) 
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		 : Actor<ConfigGeneration>(),
		   GetGenerationActorState<GetGenerationActor>(self)
	{
		fdb_probe_actor_enter("getGeneration", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getGeneration");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getGeneration", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetGenerationActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetGenerationActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
[[nodiscard]] static Future<ConfigGeneration> getGeneration( SimpleConfigTransactionImpl* const& self ) {
															#line 39 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	return Future<ConfigGeneration>(new GetGenerationActor(self));
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
}

#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"

																#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
// This generated class is to be used only via get()
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
template <class GetActor>
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
class GetActorState {
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
public:
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	GetActorState(SimpleConfigTransactionImpl* const& self,KeyRef const& key) 
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		 : self(self),
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		   key(key)
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
	{
		fdb_probe_actor_create("get", reinterpret_cast<unsigned long>(this));

	}
	~GetActorState() 
	{
		fdb_probe_actor_destroy("get", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 62 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (!self->getGenerationFuture.isValid())
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			{
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
				self->getGenerationFuture = getGeneration(self);
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			}
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			configKey = ConfigKey::decodeKey(key);
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<ConfigGeneration> __when_expr_0 = self->getGenerationFuture;
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<GetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetActor*>(this)->actor_wait_state = 1;
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetActor, 0, ConfigGeneration >*>(static_cast<GetActor*>(this)));
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
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
		this->~GetActorState();
		static_cast<GetActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(ConfigGeneration const& generation,int loopDepth) 
	{
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (self->dID.present())
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		{
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			TraceEvent("SimpleConfigTransactionGettingValue", self->dID.get()) .detail("ConfigClass", configKey.configClass) .detail("KnobName", configKey.knobName);
															#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		}
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		reply = ConfigTransactionGetReply();
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (self->cti.hostname.present())
															#line 428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		{
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_1 = store(reply, retryGetReplyFromHostname(ConfigTransactionGetRequest{ 0, generation, configKey }, self->cti.hostname.get(), WLTOKEN_CONFIGTXN_GET));
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<GetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 434 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetActor*>(this)->actor_wait_state = 2;
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetActor, 1, Void >*>(static_cast<GetActor*>(this)));
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_2 = store(reply, retryBrokenPromise(self->cti.get, ConfigTransactionGetRequest{ 0, generation, configKey }));
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<GetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 448 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<GetActor*>(this)->actor_wait_state = 3;
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetActor, 2, Void >*>(static_cast<GetActor*>(this)));
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont1(ConfigGeneration && generation,int loopDepth) 
	{
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (self->dID.present())
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		{
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			TraceEvent("SimpleConfigTransactionGettingValue", self->dID.get()) .detail("ConfigClass", configKey.configClass) .detail("KnobName", configKey.knobName);
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		}
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		reply = ConfigTransactionGetReply();
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (self->cti.hostname.present())
															#line 473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		{
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_1 = store(reply, retryGetReplyFromHostname(ConfigTransactionGetRequest{ 0, generation, configKey }, self->cti.hostname.get(), WLTOKEN_CONFIGTXN_GET));
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<GetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetActor*>(this)->actor_wait_state = 2;
															#line 74 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetActor, 1, Void >*>(static_cast<GetActor*>(this)));
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_2 = store(reply, retryBrokenPromise(self->cti.get, ConfigTransactionGetRequest{ 0, generation, configKey }));
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<GetActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<GetActor*>(this)->actor_wait_state = 3;
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetActor, 2, Void >*>(static_cast<GetActor*>(this)));
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1when1(ConfigGeneration const& generation,int loopDepth) 
	{
		loopDepth = a_body1cont1(generation, loopDepth);

		return loopDepth;
	}
	int a_body1when1(ConfigGeneration && generation,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(generation), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetActor*>(this)->actor_wait_state > 0) static_cast<GetActor*>(this)->actor_wait_state = 0;
		static_cast<GetActor*>(this)->ActorCallback< GetActor, 0, ConfigGeneration >::remove();

	}
	void a_callback_fire(ActorCallback< GetActor, 0, ConfigGeneration >*,ConfigGeneration const& value) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetActor, 0, ConfigGeneration >*,ConfigGeneration && value) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetActor, 0, ConfigGeneration >*,Error err) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (self->dID.present())
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		{
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			TraceEvent("SimpleConfigTransactionGotValue", self->dID.get()) .detail("Value", reply.value.get().toString());
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		}
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (reply.value.present())
															#line 579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		{
															#line 87 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (!static_cast<GetActor*>(this)->SAV<Optional<Value>>::futures) { (void)(reply.value.get().toValue()); this->~GetActorState(); static_cast<GetActor*>(this)->destroy(); return 0; }
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			new (&static_cast<GetActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(reply.value.get().toValue());
			this->~GetActorState();
			static_cast<GetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (!static_cast<GetActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>{}); this->~GetActorState(); static_cast<GetActor*>(this)->destroy(); return 0; }
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			new (&static_cast<GetActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>{});
			this->~GetActorState();
			static_cast<GetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<GetActor*>(this)->actor_wait_state > 0) static_cast<GetActor*>(this)->actor_wait_state = 0;
		static_cast<GetActor*>(this)->ActorCallback< GetActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont6(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont6(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetActor*>(this)->actor_wait_state > 0) static_cast<GetActor*>(this)->actor_wait_state = 0;
		static_cast<GetActor*>(this)->ActorCallback< GetActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	SimpleConfigTransactionImpl* self;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	KeyRef key;
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	ConfigKey configKey;
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	ConfigTransactionGetReply reply;
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via get()
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
class GetActor final : public Actor<Optional<Value>>, public ActorCallback< GetActor, 0, ConfigGeneration >, public ActorCallback< GetActor, 1, Void >, public ActorCallback< GetActor, 2, Void >, public FastAllocated<GetActor>, public GetActorState<GetActor> {
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<GetActor>::operator new;
	using FastAllocated<GetActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<Value>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetActor, 0, ConfigGeneration >;
friend struct ActorCallback< GetActor, 1, Void >;
friend struct ActorCallback< GetActor, 2, Void >;
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	GetActor(SimpleConfigTransactionImpl* const& self,KeyRef const& key) 
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		 : Actor<Optional<Value>>(),
		   GetActorState<GetActor>(self, key)
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("get");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetActor, 0, ConfigGeneration >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GetActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
[[nodiscard]] static Future<Optional<Value>> get( SimpleConfigTransactionImpl* const& self, KeyRef const& key ) {
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	return Future<Optional<Value>>(new GetActor(self, key));
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
}

#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"

																#line 812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
// This generated class is to be used only via getConfigClasses()
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
template <class GetConfigClassesActor>
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
class GetConfigClassesActorState {
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
public:
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	GetConfigClassesActorState(SimpleConfigTransactionImpl* const& self) 
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		 : self(self)
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
	{
		fdb_probe_actor_create("getConfigClasses", reinterpret_cast<unsigned long>(this));

	}
	~GetConfigClassesActorState() 
	{
		fdb_probe_actor_destroy("getConfigClasses", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (!self->getGenerationFuture.isValid())
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			{
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
				self->getGenerationFuture = getGeneration(self);
															#line 844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			}
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<ConfigGeneration> __when_expr_0 = self->getGenerationFuture;
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 1;
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetConfigClassesActor, 0, ConfigGeneration >*>(static_cast<GetConfigClassesActor*>(this)));
															#line 855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
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
		this->~GetConfigClassesActorState();
		static_cast<GetConfigClassesActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(ConfigGeneration const& generation,int loopDepth) 
	{
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		reply = ConfigTransactionGetConfigClassesReply();
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (self->cti.hostname.present())
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		{
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_1 = store(reply, retryGetReplyFromHostname(ConfigTransactionGetConfigClassesRequest{ 0, generation }, self->cti.hostname.get(), WLTOKEN_CONFIGTXN_GETCLASSES));
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 2;
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetConfigClassesActor, 1, Void >*>(static_cast<GetConfigClassesActor*>(this)));
															#line 891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_2 = store( reply, retryBrokenPromise(self->cti.getClasses, ConfigTransactionGetConfigClassesRequest{ 0, generation }));
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 3;
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetConfigClassesActor, 2, Void >*>(static_cast<GetConfigClassesActor*>(this)));
															#line 905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont1(ConfigGeneration && generation,int loopDepth) 
	{
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		reply = ConfigTransactionGetConfigClassesReply();
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (self->cti.hostname.present())
															#line 917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		{
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_1 = store(reply, retryGetReplyFromHostname(ConfigTransactionGetConfigClassesRequest{ 0, generation }, self->cti.hostname.get(), WLTOKEN_CONFIGTXN_GETCLASSES));
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 2;
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetConfigClassesActor, 1, Void >*>(static_cast<GetConfigClassesActor*>(this)));
															#line 928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_2 = store( reply, retryBrokenPromise(self->cti.getClasses, ConfigTransactionGetConfigClassesRequest{ 0, generation }));
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 3;
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetConfigClassesActor, 2, Void >*>(static_cast<GetConfigClassesActor*>(this)));
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1when1(ConfigGeneration const& generation,int loopDepth) 
	{
		loopDepth = a_body1cont1(generation, loopDepth);

		return loopDepth;
	}
	int a_body1when1(ConfigGeneration && generation,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(generation), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state > 0) static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 0;
		static_cast<GetConfigClassesActor*>(this)->ActorCallback< GetConfigClassesActor, 0, ConfigGeneration >::remove();

	}
	void a_callback_fire(ActorCallback< GetConfigClassesActor, 0, ConfigGeneration >*,ConfigGeneration const& value) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetConfigClassesActor, 0, ConfigGeneration >*,ConfigGeneration && value) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetConfigClassesActor, 0, ConfigGeneration >*,Error err) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		RangeResult result;
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		for( const auto& configClass : reply.configClasses ) {
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(configClass, ""_sr));
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		}
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (!static_cast<GetConfigClassesActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetConfigClassesActorState(); static_cast<GetConfigClassesActor*>(this)->destroy(); return 0; }
															#line 1023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		new (&static_cast<GetConfigClassesActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetConfigClassesActorState();
		static_cast<GetConfigClassesActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state > 0) static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 0;
		static_cast<GetConfigClassesActor*>(this)->ActorCallback< GetConfigClassesActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetConfigClassesActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetConfigClassesActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetConfigClassesActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state > 0) static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 0;
		static_cast<GetConfigClassesActor*>(this)->ActorCallback< GetConfigClassesActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetConfigClassesActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetConfigClassesActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetConfigClassesActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	SimpleConfigTransactionImpl* self;
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	ConfigTransactionGetConfigClassesReply reply;
															#line 1185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via getConfigClasses()
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
class GetConfigClassesActor final : public Actor<RangeResult>, public ActorCallback< GetConfigClassesActor, 0, ConfigGeneration >, public ActorCallback< GetConfigClassesActor, 1, Void >, public ActorCallback< GetConfigClassesActor, 2, Void >, public FastAllocated<GetConfigClassesActor>, public GetConfigClassesActorState<GetConfigClassesActor> {
															#line 1190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<GetConfigClassesActor>::operator new;
	using FastAllocated<GetConfigClassesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetConfigClassesActor, 0, ConfigGeneration >;
friend struct ActorCallback< GetConfigClassesActor, 1, Void >;
friend struct ActorCallback< GetConfigClassesActor, 2, Void >;
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	GetConfigClassesActor(SimpleConfigTransactionImpl* const& self) 
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		 : Actor<RangeResult>(),
		   GetConfigClassesActorState<GetConfigClassesActor>(self)
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getConfigClasses");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetConfigClassesActor, 0, ConfigGeneration >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetConfigClassesActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GetConfigClassesActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
[[nodiscard]] static Future<RangeResult> getConfigClasses( SimpleConfigTransactionImpl* const& self ) {
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	return Future<RangeResult>(new GetConfigClassesActor(self));
															#line 1232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
}

#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"

																#line 1237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
// This generated class is to be used only via getKnobs()
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
template <class GetKnobsActor>
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
class GetKnobsActorState {
															#line 1243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
public:
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	GetKnobsActorState(SimpleConfigTransactionImpl* const& self,Optional<Key> const& configClass) 
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		 : self(self),
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		   configClass(configClass)
															#line 1252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
	{
		fdb_probe_actor_create("getKnobs", reinterpret_cast<unsigned long>(this));

	}
	~GetKnobsActorState() 
	{
		fdb_probe_actor_destroy("getKnobs", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (!self->getGenerationFuture.isValid())
															#line 1267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			{
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
				self->getGenerationFuture = getGeneration(self);
															#line 1271 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			}
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<ConfigGeneration> __when_expr_0 = self->getGenerationFuture;
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<GetKnobsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetKnobsActor*>(this)->actor_wait_state = 1;
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetKnobsActor, 0, ConfigGeneration >*>(static_cast<GetKnobsActor*>(this)));
															#line 1282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
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
		this->~GetKnobsActorState();
		static_cast<GetKnobsActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(ConfigGeneration const& generation,int loopDepth) 
	{
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		reply = ConfigTransactionGetKnobsReply();
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (self->cti.hostname.present())
															#line 1307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		{
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_1 = store(reply, retryGetReplyFromHostname(ConfigTransactionGetKnobsRequest{ 0, generation, configClass }, self->cti.hostname.get(), WLTOKEN_CONFIGTXN_GETKNOBS));
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<GetKnobsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetKnobsActor*>(this)->actor_wait_state = 2;
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetKnobsActor, 1, Void >*>(static_cast<GetKnobsActor*>(this)));
															#line 1318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_2 = store(reply, retryBrokenPromise(self->cti.getKnobs, ConfigTransactionGetKnobsRequest{ 0, generation, configClass }));
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<GetKnobsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<GetKnobsActor*>(this)->actor_wait_state = 3;
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetKnobsActor, 2, Void >*>(static_cast<GetKnobsActor*>(this)));
															#line 1332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont1(ConfigGeneration && generation,int loopDepth) 
	{
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		reply = ConfigTransactionGetKnobsReply();
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (self->cti.hostname.present())
															#line 1344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		{
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_1 = store(reply, retryGetReplyFromHostname(ConfigTransactionGetKnobsRequest{ 0, generation, configClass }, self->cti.hostname.get(), WLTOKEN_CONFIGTXN_GETKNOBS));
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<GetKnobsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<GetKnobsActor*>(this)->actor_wait_state = 2;
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetKnobsActor, 1, Void >*>(static_cast<GetKnobsActor*>(this)));
															#line 1355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_2 = store(reply, retryBrokenPromise(self->cti.getKnobs, ConfigTransactionGetKnobsRequest{ 0, generation, configClass }));
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<GetKnobsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<GetKnobsActor*>(this)->actor_wait_state = 3;
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetKnobsActor, 2, Void >*>(static_cast<GetKnobsActor*>(this)));
															#line 1369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1when1(ConfigGeneration const& generation,int loopDepth) 
	{
		loopDepth = a_body1cont1(generation, loopDepth);

		return loopDepth;
	}
	int a_body1when1(ConfigGeneration && generation,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(generation), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetKnobsActor*>(this)->actor_wait_state > 0) static_cast<GetKnobsActor*>(this)->actor_wait_state = 0;
		static_cast<GetKnobsActor*>(this)->ActorCallback< GetKnobsActor, 0, ConfigGeneration >::remove();

	}
	void a_callback_fire(ActorCallback< GetKnobsActor, 0, ConfigGeneration >*,ConfigGeneration const& value) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetKnobsActor, 0, ConfigGeneration >*,ConfigGeneration && value) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetKnobsActor, 0, ConfigGeneration >*,Error err) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		RangeResult result;
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		for( const auto& knobName : reply.knobNames ) {
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(knobName, ""_sr));
															#line 1446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		}
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (!static_cast<GetKnobsActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetKnobsActorState(); static_cast<GetKnobsActor*>(this)->destroy(); return 0; }
															#line 1450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		new (&static_cast<GetKnobsActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetKnobsActorState();
		static_cast<GetKnobsActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<GetKnobsActor*>(this)->actor_wait_state > 0) static_cast<GetKnobsActor*>(this)->actor_wait_state = 0;
		static_cast<GetKnobsActor*>(this)->ActorCallback< GetKnobsActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetKnobsActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetKnobsActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetKnobsActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetKnobsActor*>(this)->actor_wait_state > 0) static_cast<GetKnobsActor*>(this)->actor_wait_state = 0;
		static_cast<GetKnobsActor*>(this)->ActorCallback< GetKnobsActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetKnobsActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetKnobsActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetKnobsActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	SimpleConfigTransactionImpl* self;
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	Optional<Key> configClass;
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	ConfigTransactionGetKnobsReply reply;
															#line 1614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via getKnobs()
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
class GetKnobsActor final : public Actor<RangeResult>, public ActorCallback< GetKnobsActor, 0, ConfigGeneration >, public ActorCallback< GetKnobsActor, 1, Void >, public ActorCallback< GetKnobsActor, 2, Void >, public FastAllocated<GetKnobsActor>, public GetKnobsActorState<GetKnobsActor> {
															#line 1619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<GetKnobsActor>::operator new;
	using FastAllocated<GetKnobsActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetKnobsActor, 0, ConfigGeneration >;
friend struct ActorCallback< GetKnobsActor, 1, Void >;
friend struct ActorCallback< GetKnobsActor, 2, Void >;
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	GetKnobsActor(SimpleConfigTransactionImpl* const& self,Optional<Key> const& configClass) 
															#line 1632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		 : Actor<RangeResult>(),
		   GetKnobsActorState<GetKnobsActor>(self, configClass)
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getKnobs");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetKnobsActor, 0, ConfigGeneration >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetKnobsActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GetKnobsActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
[[nodiscard]] static Future<RangeResult> getKnobs( SimpleConfigTransactionImpl* const& self, Optional<Key> const& configClass ) {
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	return Future<RangeResult>(new GetKnobsActor(self, configClass));
															#line 1661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
}

#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"

																#line 1666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
// This generated class is to be used only via commit()
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
template <class CommitActor>
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
class CommitActorState {
															#line 1672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
public:
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	CommitActorState(SimpleConfigTransactionImpl* const& self) 
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		 : self(self)
															#line 1679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
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
															#line 140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (!self->getGenerationFuture.isValid())
															#line 1694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			{
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
				self->getGenerationFuture = getGeneration(self);
															#line 1698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			}
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			self->toCommit.coordinatorsHash = 0;
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_0 = store(self->toCommit.generation, self->getGenerationFuture);
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CommitActor*>(this)->actor_wait_state = 1;
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 0, Void >*>(static_cast<CommitActor*>(this)));
															#line 1711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
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
		this->~CommitActorState();
		static_cast<CommitActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		self->toCommit.annotation.timestamp = now();
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (self->cti.hostname.present())
															#line 1736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		{
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_1 = retryGetReplyFromHostname(self->toCommit, self->cti.hostname.get(), WLTOKEN_CONFIGTXN_COMMIT);
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<CommitActor*>(this)->actor_wait_state = 2;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 1, Void >*>(static_cast<CommitActor*>(this)));
															#line 1747 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_2 = retryBrokenPromise(self->cti.commit, self->toCommit);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1756 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<CommitActor*>(this)->actor_wait_state = 3;
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 2, Void >*>(static_cast<CommitActor*>(this)));
															#line 1761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		self->toCommit.annotation.timestamp = now();
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (self->cti.hostname.present())
															#line 1773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		{
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_1 = retryGetReplyFromHostname(self->toCommit, self->cti.hostname.get(), WLTOKEN_CONFIGTXN_COMMIT);
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
			static_cast<CommitActor*>(this)->actor_wait_state = 2;
															#line 147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 1, Void >*>(static_cast<CommitActor*>(this)));
															#line 1784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_2 = retryBrokenPromise(self->cti.commit, self->toCommit);
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when2(__when_expr_2.get(), loopDepth); };
			static_cast<CommitActor*>(this)->actor_wait_state = 3;
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 2, Void >*>(static_cast<CommitActor*>(this)));
															#line 1798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			loopDepth = 0;
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
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
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
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CommitActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		self->committed = true;
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (!static_cast<CommitActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CommitActorState(); static_cast<CommitActor*>(this)->destroy(); return 0; }
															#line 1873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		new (&static_cast<CommitActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CommitActorState();
		static_cast<CommitActor*>(this)->finishSendAndDelPromiseRef();
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
		if (static_cast<CommitActor*>(this)->actor_wait_state > 0) static_cast<CommitActor*>(this)->actor_wait_state = 0;
		static_cast<CommitActor*>(this)->ActorCallback< CommitActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CommitActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CommitActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CommitActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont5(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont5(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont5(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(_), loopDepth);

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
			a_body1cont1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CommitActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CommitActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	SimpleConfigTransactionImpl* self;
															#line 2033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via commit()
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
class CommitActor final : public Actor<Void>, public ActorCallback< CommitActor, 0, Void >, public ActorCallback< CommitActor, 1, Void >, public ActorCallback< CommitActor, 2, Void >, public FastAllocated<CommitActor>, public CommitActorState<CommitActor> {
															#line 2038 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
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
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	CommitActor(SimpleConfigTransactionImpl* const& self) 
															#line 2051 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		 : Actor<Void>(),
		   CommitActorState<CommitActor>(self)
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
		}

	}
};
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
[[nodiscard]] static Future<Void> commit( SimpleConfigTransactionImpl* const& self ) {
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	return Future<Void>(new CommitActor(self));
															#line 2080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
}

#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"

																#line 2085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
// This generated class is to be used only via onError()
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
template <class OnErrorActor>
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
class OnErrorActorState {
															#line 2091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
public:
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	OnErrorActorState(SimpleConfigTransactionImpl* const& self,Error const& e) 
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		 : self(self),
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		   e(e)
															#line 2100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
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
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
			if (e.code() == error_code_transaction_too_old || e.code() == error_code_not_committed)
															#line 2115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
			{
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
				StrictFuture<Void> __when_expr_0 = delay((1 << self->numRetries++) * 0.01 * deterministicRandom()->random01());
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
				if (static_cast<OnErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<OnErrorActor*>(this)->actor_wait_state = 1;
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnErrorActor, 0, Void >*>(static_cast<OnErrorActor*>(this)));
															#line 2126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
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
		this->~OnErrorActorState();
		static_cast<OnErrorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		return a_body1Catch1(e, loopDepth);
															#line 2154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		self->reset();
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (!static_cast<OnErrorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnErrorActorState(); static_cast<OnErrorActor*>(this)->destroy(); return 0; }
															#line 2164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		new (&static_cast<OnErrorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnErrorActorState();
		static_cast<OnErrorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		self->reset();
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
		if (!static_cast<OnErrorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnErrorActorState(); static_cast<OnErrorActor*>(this)->destroy(); return 0; }
															#line 2178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		new (&static_cast<OnErrorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnErrorActorState();
		static_cast<OnErrorActor*>(this)->finishSendAndDelPromiseRef();
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
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
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
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onError", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< OnErrorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("onError", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("onError", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	SimpleConfigTransactionImpl* self;
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	Error e;
															#line 2253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via onError()
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
class OnErrorActor final : public Actor<Void>, public ActorCallback< OnErrorActor, 0, Void >, public FastAllocated<OnErrorActor>, public OnErrorActorState<OnErrorActor> {
															#line 2258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<OnErrorActor>::operator new;
	using FastAllocated<OnErrorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnErrorActor, 0, Void >;
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	OnErrorActor(SimpleConfigTransactionImpl* const& self,Error const& e) 
															#line 2269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
		 : Actor<Void>(),
		   OnErrorActorState<OnErrorActor>(self, e)
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
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
[[nodiscard]] static Future<Void> onError( SimpleConfigTransactionImpl* const& self, Error const& e ) {
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"
	return Future<Void>(new OnErrorActor(self, e));
															#line 2296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.g.cpp"
}

#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/SimpleConfigTransaction.actor.cpp"

public:
	SimpleConfigTransactionImpl(Database const& cx) : cx(cx) {
		const ClusterConnectionString& cs = cx->getConnectionRecord()->getConnectionString();
		if (cs.coords.size()) {
			std::vector<NetworkAddress> coordinators = cs.coords;
			std::sort(coordinators.begin(), coordinators.end());
			cti = ConfigTransactionInterface(coordinators[0]);
		} else {
			cti = ConfigTransactionInterface(cs.hostnames[0]);
		}
	}

	SimpleConfigTransactionImpl(ConfigTransactionInterface const& cti) : cti(cti) {}

	void set(KeyRef key, ValueRef value) {
		toCommit.mutations.push_back_deep(toCommit.arena,
		                                  IKnobCollection::createSetMutation(toCommit.arena, key, value));
	}

	void clear(KeyRef key) {
		toCommit.mutations.push_back_deep(toCommit.arena, IKnobCollection::createClearMutation(toCommit.arena, key));
	}

	Future<Optional<Value>> get(KeyRef key) { return get(this, key); }

	Future<RangeResult> getRange(KeyRangeRef keys) {
		if (keys == configClassKeys) {
			return getConfigClasses(this);
		} else if (keys == globalConfigKnobKeys) {
			return getKnobs(this, {});
		} else if (configKnobKeys.contains(keys) && keys.singleKeyRange()) {
			const auto configClass = keys.begin.removePrefix(configKnobKeys.begin);
			return getKnobs(this, configClass);
		} else {
			throw invalid_config_db_range_read();
		}
	}

	Future<Void> commit() { return commit(this); }

	Future<Void> onError(Error const& e) { return onError(this, e); }

	Future<Version> getReadVersion() {
		if (!getGenerationFuture.isValid())
			getGenerationFuture = getGeneration(this);
		return map(getGenerationFuture, [](auto const& gen) { return gen.committedVersion; });
	}

	Optional<Version> getCachedReadVersion() const {
		if (getGenerationFuture.isValid() && getGenerationFuture.isReady() && !getGenerationFuture.isError()) {
			return getGenerationFuture.get().committedVersion;
		} else {
			return {};
		}
	}

	Version getCommittedVersion() const { return committed ? getGenerationFuture.get().liveVersion : ::invalidVersion; }

	void reset() {
		getGenerationFuture = Future<ConfigGeneration>{};
		toCommit = {};
		committed = false;
	}

	void fullReset() {
		numRetries = 0;
		dID = {};
		reset();
	}

	size_t getApproximateSize() const { return toCommit.expectedSize(); }

	void debugTransaction(UID dID) { this->dID = dID; }

	void checkDeferredError(Error const& deferredError) const {
		if (deferredError.code() != invalid_error_code) {
			throw deferredError;
		}
		if (cx.getPtr()) {
			cx->checkDeferredError();
		}
	}
}; // SimpleConfigTransactionImpl

Future<Version> SimpleConfigTransaction::getReadVersion() {
	return impl->getReadVersion();
}

Optional<Version> SimpleConfigTransaction::getCachedReadVersion() const {
	return impl->getCachedReadVersion();
}

Future<Optional<Value>> SimpleConfigTransaction::get(Key const& key, Snapshot snapshot) {
	return impl->get(key);
}

Future<RangeResult> SimpleConfigTransaction::getRange(KeySelector const& begin,
                                                      KeySelector const& end,
                                                      int limit,
                                                      Snapshot snapshot,
                                                      Reverse reverse) {
	if (reverse) {
		throw client_invalid_operation();
	}
	return impl->getRange(KeyRangeRef(begin.getKey(), end.getKey()));
}

Future<RangeResult> SimpleConfigTransaction::getRange(KeySelector begin,
                                                      KeySelector end,
                                                      GetRangeLimits limits,
                                                      Snapshot snapshot,
                                                      Reverse reverse) {
	if (reverse) {
		throw client_invalid_operation();
	}
	return impl->getRange(KeyRangeRef(begin.getKey(), end.getKey()));
}

void SimpleConfigTransaction::set(KeyRef const& key, ValueRef const& value) {
	impl->set(key, value);
}

void SimpleConfigTransaction::clear(KeyRef const& key) {
	impl->clear(key);
}

Future<Void> SimpleConfigTransaction::commit() {
	return impl->commit();
}

Version SimpleConfigTransaction::getCommittedVersion() const {
	return impl->getCommittedVersion();
}

double SimpleConfigTransaction::getTagThrottledDuration() const {
	return 0.0;
}

int64_t SimpleConfigTransaction::getTotalCost() const {
	return 0;
}

int64_t SimpleConfigTransaction::getApproximateSize() const {
	return impl->getApproximateSize();
}

void SimpleConfigTransaction::setOption(FDBTransactionOptions::Option option, Optional<StringRef> value) {
	// TODO: Support using this option to determine atomicity
}

Future<Void> SimpleConfigTransaction::onError(Error const& e) {
	return impl->onError(e);
}

void SimpleConfigTransaction::cancel() {
	// TODO: Implement someday
	throw client_invalid_operation();
}

void SimpleConfigTransaction::reset() {
	return impl->reset();
}

void SimpleConfigTransaction::fullReset() {
	return impl->fullReset();
}

void SimpleConfigTransaction::debugTransaction(UID dID) {
	impl->debugTransaction(dID);
}

void SimpleConfigTransaction::checkDeferredError() const {
	impl->checkDeferredError(deferredError);
}

void SimpleConfigTransaction::construct(Database const& cx) {
	impl = PImpl<SimpleConfigTransactionImpl>::create(cx);
}

SimpleConfigTransaction::SimpleConfigTransaction(ConfigTransactionInterface const& cti)
  : impl(PImpl<SimpleConfigTransactionImpl>::create(cti)) {}

SimpleConfigTransaction::SimpleConfigTransaction() = default;

SimpleConfigTransaction::~SimpleConfigTransaction() = default;
