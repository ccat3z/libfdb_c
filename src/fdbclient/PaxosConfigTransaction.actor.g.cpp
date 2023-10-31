#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
/*
 * PaxosConfigTransaction.actor.cpp
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
#include "fdbclient/MonitorLeader.h"
#include "fdbclient/PaxosConfigTransaction.h"
#include "flow/actorcompiler.h" // must be last include

using ConfigTransactionInfo = ModelInterface<ConfigTransactionInterface>;

class CommitQuorum {
	ActorCollection actors{ false };
	std::vector<ConfigTransactionInterface> ctis;
	size_t failed{ 0 };
	size_t successful{ 0 };
	size_t maybeCommitted{ 0 };
	Promise<Void> result;
	Standalone<VectorRef<ConfigMutationRef>> mutations;
	ConfigCommitAnnotation annotation;

	ConfigTransactionCommitRequest getCommitRequest(ConfigGeneration generation,
	                                                CoordinatorsHash coordinatorsHash) const {
		return ConfigTransactionCommitRequest(coordinatorsHash, generation, mutations, annotation);
	}

	void updateResult() {
		if (successful >= ctis.size() / 2 + 1 && result.canBeSet()) {
			// Calling send could delete this
			auto local = this->result;
			local.send(Void());
		} else if (failed >= ctis.size() / 2 + 1 && result.canBeSet()) {
			// Rollforwards could cause a version that didn't have quorum to
			// commit, so send commit_unknown_result instead of commit_failed.

			// Calling sendError could delete this
			auto local = this->result;
			local.sendError(commit_unknown_result());
		} else {
			// Check if it is possible to ever receive quorum agreement
			auto totalRequestsOutstanding = ctis.size() - (failed + successful + maybeCommitted);
			if ((failed + totalRequestsOutstanding < ctis.size() / 2 + 1) &&
			    (successful + totalRequestsOutstanding < ctis.size() / 2 + 1) && result.canBeSet()) {
				// Calling sendError could delete this
				auto local = this->result;
				local.sendError(commit_unknown_result());
			}
		}
	}

																#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via addRequestActor()
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class AddRequestActorActor>
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class AddRequestActorActorState {
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	AddRequestActorActorState(CommitQuorum* const& self,ConfigGeneration const& generation,CoordinatorsHash const& coordinatorsHash,ConfigTransactionInterface const& cti) 
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self),
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   generation(generation),
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   coordinatorsHash(coordinatorsHash),
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   cti(cti)
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
	{
		fdb_probe_actor_create("addRequestActor", reinterpret_cast<unsigned long>(this));

	}
	~AddRequestActorActorState() 
	{
		fdb_probe_actor_destroy("addRequestActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				if (cti.hostname.present())
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				{
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					StrictFuture<Void> __when_expr_0 = timeoutError(retryGetReplyFromHostname(self->getCommitRequest(generation, coordinatorsHash), cti.hostname.get(), WLTOKEN_CONFIGTXN_COMMIT), CLIENT_KNOBS->COMMIT_QUORUM_TIMEOUT);
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					if (static_cast<AddRequestActorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<AddRequestActorActor*>(this)->actor_wait_state = 1;
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AddRequestActorActor, 0, Void >*>(static_cast<AddRequestActorActor*>(this)));
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
					loopDepth = 0;
				}
				else
				{
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					StrictFuture<Void> __when_expr_1 = timeoutError(cti.commit.getReply(self->getCommitRequest(generation, coordinatorsHash)), CLIENT_KNOBS->COMMIT_QUORUM_TIMEOUT);
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					if (static_cast<AddRequestActorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
					if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
					static_cast<AddRequestActorActor*>(this)->actor_wait_state = 2;
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AddRequestActorActor, 1, Void >*>(static_cast<AddRequestActorActor*>(this)));
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
					loopDepth = 0;
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
		this->~AddRequestActorActorState();
		static_cast<AddRequestActorActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->updateResult();
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<AddRequestActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AddRequestActorActorState(); static_cast<AddRequestActorActor*>(this)->destroy(); return 0; }
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<AddRequestActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~AddRequestActorActorState();
		static_cast<AddRequestActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (e.code() == error_code_not_committed || e.code() == error_code_timed_out)
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				++self->failed;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
			else
			{
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				++self->maybeCommitted;
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
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
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		++self->successful;
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = a_body1cont6(loopDepth);

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
		if (static_cast<AddRequestActorActor*>(this)->actor_wait_state > 0) static_cast<AddRequestActorActor*>(this)->actor_wait_state = 0;
		static_cast<AddRequestActorActor*>(this)->ActorCallback< AddRequestActorActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AddRequestActorActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("addRequestActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addRequestActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AddRequestActorActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("addRequestActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addRequestActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AddRequestActorActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("addRequestActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addRequestActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

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
		if (static_cast<AddRequestActorActor*>(this)->actor_wait_state > 0) static_cast<AddRequestActorActor*>(this)->actor_wait_state = 0;
		static_cast<AddRequestActorActor*>(this)->ActorCallback< AddRequestActorActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AddRequestActorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("addRequestActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addRequestActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< AddRequestActorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("addRequestActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addRequestActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< AddRequestActorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("addRequestActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addRequestActor", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont6(int loopDepth) 
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
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	CommitQuorum* self;
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigGeneration generation;
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	CoordinatorsHash coordinatorsHash;
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigTransactionInterface cti;
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via addRequestActor()
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class AddRequestActorActor final : public Actor<Void>, public ActorCallback< AddRequestActorActor, 0, Void >, public ActorCallback< AddRequestActorActor, 1, Void >, public FastAllocated<AddRequestActorActor>, public AddRequestActorActorState<AddRequestActorActor> {
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<AddRequestActorActor>::operator new;
	using FastAllocated<AddRequestActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AddRequestActorActor, 0, Void >;
friend struct ActorCallback< AddRequestActorActor, 1, Void >;
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	AddRequestActorActor(CommitQuorum* const& self,ConfigGeneration const& generation,CoordinatorsHash const& coordinatorsHash,ConfigTransactionInterface const& cti) 
															#line 401 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		 : Actor<Void>(),
		   AddRequestActorActorState<AddRequestActorActor>(self, generation, coordinatorsHash, cti)
	{
		fdb_probe_actor_enter("addRequestActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("addRequestActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("addRequestActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AddRequestActorActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< AddRequestActorActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<Void> addRequestActor( CommitQuorum* const& self, ConfigGeneration const& generation, CoordinatorsHash const& coordinatorsHash, ConfigTransactionInterface const& cti ) {
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<Void>(new AddRequestActorActor(self, generation, coordinatorsHash, cti));
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

public:
	CommitQuorum() = default;
	explicit CommitQuorum(std::vector<ConfigTransactionInterface> const& ctis) : ctis(ctis) {}
	void set(KeyRef key, ValueRef value) {
		if (key == configTransactionDescriptionKey) {
			annotation.description = ValueRef(annotation.arena(), value);
		} else {
			mutations.push_back_deep(mutations.arena(),
			                         IKnobCollection::createSetMutation(mutations.arena(), key, value));
		}
	}
	void clear(KeyRef key) {
		if (key == configTransactionDescriptionKey) {
			annotation.description = ""_sr;
		} else {
			mutations.push_back_deep(mutations.arena(), IKnobCollection::createClearMutation(mutations.arena(), key));
		}
	}
	void setTimestamp() { annotation.timestamp = now(); }
	size_t expectedSize() const { return annotation.expectedSize() + mutations.expectedSize(); }
	Future<Void> commit(ConfigGeneration generation, CoordinatorsHash coordinatorsHash) {
		// Send commit message to all replicas, even those that did not return the used replica.
		// This way, slow replicas are kept up date.
		for (const auto& cti : ctis) {
			actors.add(addRequestActor(this, generation, coordinatorsHash, cti));
		}
		return result.getFuture();
	}
	bool committed() const { return result.isSet() && !result.isError(); }
};

class GetGenerationQuorum {
	ActorCollection actors{ false };
	CoordinatorsHash coordinatorsHash{ 0 };
	std::vector<ConfigTransactionInterface> ctis;
	std::map<ConfigGeneration, std::vector<ConfigTransactionInterface>> seenGenerations;
	Promise<ConfigGeneration> result;
	size_t totalRepliesReceived{ 0 };
	size_t maxAgreement{ 0 };
	Future<Void> coordinatorsChangedFuture;
	Optional<Version> lastSeenLiveVersion;
	Future<ConfigGeneration> getGenerationFuture;

																#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via addRequestActor()
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class AddRequestActorActor1>
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class AddRequestActorActor1State {
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	AddRequestActorActor1State(GetGenerationQuorum* const& self,ConfigTransactionInterface const& cti) 
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self),
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   cti(cti)
															#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
	{
		fdb_probe_actor_create("addRequestActor", reinterpret_cast<unsigned long>(this));

	}
	~AddRequestActorActor1State() 
	{
		fdb_probe_actor_destroy("addRequestActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			;
															#line 507 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
		this->~AddRequestActorActor1State();
		static_cast<AddRequestActorActor1*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<AddRequestActorActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AddRequestActorActor1State(); static_cast<AddRequestActorActor1*>(this)->destroy(); return 0; }
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<AddRequestActorActor1*>(this)->SAV< Void >::value()) Void(Void());
		this->~AddRequestActorActor1State();
		static_cast<AddRequestActorActor1*>(this)->finishSendAndDelPromiseRef();
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
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			reply = ConfigTransactionGetGenerationReply();
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (cti.hostname.present())
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				StrictFuture<Void> __when_expr_0 = timeoutError(store(reply, retryGetReplyFromHostname( ConfigTransactionGetGenerationRequest{ self->coordinatorsHash, self->lastSeenLiveVersion }, cti.hostname.get(), WLTOKEN_CONFIGTXN_GETGENERATION)), CLIENT_KNOBS->GET_GENERATION_QUORUM_TIMEOUT);
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				if (static_cast<AddRequestActorActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<AddRequestActorActor1*>(this)->actor_wait_state = 1;
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AddRequestActorActor1, 0, Void >*>(static_cast<AddRequestActorActor1*>(this)));
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				StrictFuture<Void> __when_expr_1 = timeoutError(store(reply, cti.getGeneration.getReply(ConfigTransactionGetGenerationRequest{ self->coordinatorsHash, self->lastSeenLiveVersion })), CLIENT_KNOBS->GET_GENERATION_QUORUM_TIMEOUT);
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				if (static_cast<AddRequestActorActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
				static_cast<AddRequestActorActor1*>(this)->actor_wait_state = 2;
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AddRequestActorActor1, 1, Void >*>(static_cast<AddRequestActorActor1*>(this)));
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				loopDepth = 0;
			}
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
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (e.code() == error_code_broken_promise)
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
				return a_body1loopHead1(loopDepth); // continue
			}
			else
			{
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				if (e.code() == error_code_timed_out)
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				{
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					++self->totalRepliesReceived;
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					if (self->totalRepliesReceived == self->ctis.size() && self->result.canBeSet() && !self->result.isError())
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
					{
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
						auto local = self->result;
															#line 195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
						local.sendError(failed_to_reach_quorum());
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
					}
					return a_body1break1(loopDepth==0?0:loopDepth-1); // break
				}
				else
				{
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				}
			}
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
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		++self->totalRepliesReceived;
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		auto gen = reply.generation;
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->lastSeenLiveVersion = std::max(gen.liveVersion, self->lastSeenLiveVersion.orDefault(::invalidVersion));
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		auto& replicas = self->seenGenerations[gen];
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		replicas.push_back(cti);
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->maxAgreement = std::max(replicas.size(), self->maxAgreement);
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (replicas.size() >= self->ctis.size() / 2 + 1 && !self->result.isSet())
															#line 663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		{
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			self->result.send(gen);
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		}
		else
		{
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (self->maxAgreement + (self->ctis.size() - self->totalRepliesReceived) < (self->ctis.size() / 2 + 1))
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				if (!self->result.isError())
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				{
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					auto local = self->result;
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					local.sendError(failed_to_reach_quorum());
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				}
			}
		}
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
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
		if (static_cast<AddRequestActorActor1*>(this)->actor_wait_state > 0) static_cast<AddRequestActorActor1*>(this)->actor_wait_state = 0;
		static_cast<AddRequestActorActor1*>(this)->ActorCallback< AddRequestActorActor1, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AddRequestActorActor1, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("addRequestActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addRequestActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AddRequestActorActor1, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("addRequestActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addRequestActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AddRequestActorActor1, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("addRequestActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addRequestActor", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<AddRequestActorActor1*>(this)->actor_wait_state > 0) static_cast<AddRequestActorActor1*>(this)->actor_wait_state = 0;
		static_cast<AddRequestActorActor1*>(this)->ActorCallback< AddRequestActorActor1, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AddRequestActorActor1, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("addRequestActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addRequestActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< AddRequestActorActor1, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("addRequestActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addRequestActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< AddRequestActorActor1, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("addRequestActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("addRequestActor", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetGenerationQuorum* self;
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigTransactionInterface cti;
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigTransactionGetGenerationReply reply;
															#line 847 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via addRequestActor()
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class AddRequestActorActor1 final : public Actor<Void>, public ActorCallback< AddRequestActorActor1, 0, Void >, public ActorCallback< AddRequestActorActor1, 1, Void >, public FastAllocated<AddRequestActorActor1>, public AddRequestActorActor1State<AddRequestActorActor1> {
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<AddRequestActorActor1>::operator new;
	using FastAllocated<AddRequestActorActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AddRequestActorActor1, 0, Void >;
friend struct ActorCallback< AddRequestActorActor1, 1, Void >;
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	AddRequestActorActor1(GetGenerationQuorum* const& self,ConfigTransactionInterface const& cti) 
															#line 864 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		 : Actor<Void>(),
		   AddRequestActorActor1State<AddRequestActorActor1>(self, cti)
	{
		fdb_probe_actor_enter("addRequestActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("addRequestActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("addRequestActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AddRequestActorActor1, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< AddRequestActorActor1, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<Void> addRequestActor( GetGenerationQuorum* const& self, ConfigTransactionInterface const& cti ) {
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<Void>(new AddRequestActorActor1(self, cti));
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

																#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via getGenerationActor()
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class GetGenerationActorActor>
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetGenerationActorActorState {
															#line 903 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetGenerationActorActorState(GetGenerationQuorum* const& self) 
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self),
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   retries(0)
															#line 912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
	{
		fdb_probe_actor_create("getGenerationActor", reinterpret_cast<unsigned long>(this));

	}
	~GetGenerationActorActorState() 
	{
		fdb_probe_actor_destroy("getGenerationActor", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			;
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
		this->~GetGenerationActorActorState();
		static_cast<GetGenerationActorActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( const auto& cti : self->ctis ) {
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			self->actors.add(addRequestActor(self, cti));
															#line 959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		}
		try {
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			StrictFuture<ConfigGeneration> __when_expr_0 = self->result.getFuture();
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (static_cast<GetGenerationActorActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 966 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_1 = self->actors.getResult();
															#line 970 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
			static_cast<GetGenerationActorActor*>(this)->actor_wait_state = 1;
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetGenerationActorActor, 0, ConfigGeneration >*>(static_cast<GetGenerationActorActor*>(this)));
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetGenerationActorActor, 1, Void >*>(static_cast<GetGenerationActorActor*>(this)));
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (e.code() == error_code_failed_to_reach_quorum)
															#line 999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				CODE_PROBE(true, "Failed to reach quorum getting generation");
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				if (self->coordinatorsChangedFuture.isReady())
															#line 1005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				{
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					return a_body1Catch1(coordinators_changed(), std::max(0, loopDepth - 1));
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				}
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				if (deterministicRandom()->random01() < 0.95)
															#line 1013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				{
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					StrictFuture<Void> __when_expr_2 = delayJittered(std::clamp( 0.006 * (1 << std::min(retries, 30)), 0.0, CLIENT_KNOBS->TIMEOUT_RETRY_UPPER_BOUND));
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					if (static_cast<GetGenerationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
					if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
					static_cast<GetGenerationActorActor*>(this)->actor_wait_state = 2;
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetGenerationActorActor, 2, Void >*>(static_cast<GetGenerationActorActor*>(this)));
															#line 1024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
					loopDepth = 0;
				}
				else
				{
					loopDepth = a_body1loopBody1Catch1cont2(loopDepth);
				}
			}
			else
			{
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
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
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(ConfigGeneration const& generation,int loopDepth) 
	{
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<GetGenerationActorActor*>(this)->SAV<ConfigGeneration>::futures) { (void)(generation); this->~GetGenerationActorActorState(); static_cast<GetGenerationActorActor*>(this)->destroy(); return 0; }
															#line 1057 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<GetGenerationActorActor*>(this)->SAV< ConfigGeneration >::value()) ConfigGeneration(generation);
		this->~GetGenerationActorActorState();
		static_cast<GetGenerationActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(ConfigGeneration && generation,int loopDepth) 
	{
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<GetGenerationActorActor*>(this)->SAV<ConfigGeneration>::futures) { (void)(generation); this->~GetGenerationActorActorState(); static_cast<GetGenerationActorActor*>(this)->destroy(); return 0; }
															#line 1069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<GetGenerationActorActor*>(this)->SAV< ConfigGeneration >::value()) ConfigGeneration(generation);
		this->~GetGenerationActorActorState();
		static_cast<GetGenerationActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		ASSERT(false);
															#line 1081 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		ASSERT(false);
															#line 1090 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetGenerationActorActor*>(this)->actor_wait_state > 0) static_cast<GetGenerationActorActor*>(this)->actor_wait_state = 0;
		static_cast<GetGenerationActorActor*>(this)->ActorCallback< GetGenerationActorActor, 0, ConfigGeneration >::remove();
		static_cast<GetGenerationActorActor*>(this)->ActorCallback< GetGenerationActorActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetGenerationActorActor, 0, ConfigGeneration >*,ConfigGeneration const& value) 
	{
		fdb_probe_actor_enter("getGenerationActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGenerationActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetGenerationActorActor, 0, ConfigGeneration >*,ConfigGeneration && value) 
	{
		fdb_probe_actor_enter("getGenerationActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGenerationActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetGenerationActorActor, 0, ConfigGeneration >*,Error err) 
	{
		fdb_probe_actor_enter("getGenerationActor", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGenerationActor", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetGenerationActorActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getGenerationActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGenerationActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetGenerationActorActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getGenerationActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGenerationActor", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetGenerationActorActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getGenerationActor", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGenerationActor", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont4(int loopDepth) 
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
	int a_body1loopBody1Catch1cont1(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont2(int loopDepth) 
	{
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (deterministicRandom()->random01() < 0.05)
															#line 1215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		{
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_3 = delay(CLIENT_KNOBS->GET_GENERATION_QUORUM_TIMEOUT * (deterministicRandom()->random01() + 1.0));
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (static_cast<GetGenerationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1cont2when1(__when_expr_3.get(), loopDepth); };
			static_cast<GetGenerationActorActor*>(this)->actor_wait_state = 3;
															#line 237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< GetGenerationActorActor, 3, Void >*>(static_cast<GetGenerationActorActor*>(this)));
															#line 1226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1Catch1cont5(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont4(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont4(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont2(loopDepth);

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
	void a_exitChoose2() 
	{
		if (static_cast<GetGenerationActorActor*>(this)->actor_wait_state > 0) static_cast<GetGenerationActorActor*>(this)->actor_wait_state = 0;
		static_cast<GetGenerationActorActor*>(this)->ActorCallback< GetGenerationActorActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetGenerationActorActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getGenerationActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGenerationActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetGenerationActorActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getGenerationActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGenerationActor", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetGenerationActorActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getGenerationActor", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGenerationActor", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1Catch1cont5(int loopDepth) 
	{
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		++retries;
															#line 241 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->actors.clear(false);
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->seenGenerations.clear();
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->result.reset();
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->totalRepliesReceived = 0;
															#line 245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->maxAgreement = 0;
															#line 1325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = a_body1loopBody1Catch1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont6(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont6(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetGenerationActorActor*>(this)->actor_wait_state > 0) static_cast<GetGenerationActorActor*>(this)->actor_wait_state = 0;
		static_cast<GetGenerationActorActor*>(this)->ActorCallback< GetGenerationActorActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetGenerationActorActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getGenerationActor", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGenerationActor", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< GetGenerationActorActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getGenerationActor", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGenerationActor", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< GetGenerationActorActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getGenerationActor", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getGenerationActor", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetGenerationQuorum* self;
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	int retries;
															#line 1409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via getGenerationActor()
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetGenerationActorActor final : public Actor<ConfigGeneration>, public ActorCallback< GetGenerationActorActor, 0, ConfigGeneration >, public ActorCallback< GetGenerationActorActor, 1, Void >, public ActorCallback< GetGenerationActorActor, 2, Void >, public ActorCallback< GetGenerationActorActor, 3, Void >, public FastAllocated<GetGenerationActorActor>, public GetGenerationActorActorState<GetGenerationActorActor> {
															#line 1414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<GetGenerationActorActor>::operator new;
	using FastAllocated<GetGenerationActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<ConfigGeneration>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetGenerationActorActor, 0, ConfigGeneration >;
friend struct ActorCallback< GetGenerationActorActor, 1, Void >;
friend struct ActorCallback< GetGenerationActorActor, 2, Void >;
friend struct ActorCallback< GetGenerationActorActor, 3, Void >;
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetGenerationActorActor(GetGenerationQuorum* const& self) 
															#line 1428 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		 : Actor<ConfigGeneration>(),
		   GetGenerationActorActorState<GetGenerationActorActor>(self)
	{
		fdb_probe_actor_enter("getGenerationActor", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getGenerationActor");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getGenerationActor", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetGenerationActorActor, 0, ConfigGeneration >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< GetGenerationActorActor, 2, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< GetGenerationActorActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<ConfigGeneration> getGenerationActor( GetGenerationQuorum* const& self ) {
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<ConfigGeneration>(new GetGenerationActorActor(self));
															#line 1457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

public:
	GetGenerationQuorum() = default;
	explicit GetGenerationQuorum(CoordinatorsHash coordinatorsHash,
	                             std::vector<ConfigTransactionInterface> const& ctis,
	                             Future<Void> coordinatorsChangedFuture,
	                             Optional<Version> const& lastSeenLiveVersion = {})
	  : coordinatorsHash(coordinatorsHash), ctis(ctis), coordinatorsChangedFuture(coordinatorsChangedFuture),
	    lastSeenLiveVersion(lastSeenLiveVersion) {}
	Future<ConfigGeneration> getGeneration() {
		if (!getGenerationFuture.isValid()) {
			getGenerationFuture = getGenerationActor(this);
		}
		return getGenerationFuture;
	}
	bool isReady() const {
		return getGenerationFuture.isValid() && getGenerationFuture.isReady() && !getGenerationFuture.isError();
	}
	Optional<ConfigGeneration> getCachedGeneration() const {
		return isReady() ? getGenerationFuture.get() : Optional<ConfigGeneration>{};
	}
	std::vector<ConfigTransactionInterface> getReadReplicas() const {
		ASSERT(isReady());
		return seenGenerations.at(getGenerationFuture.get());
	}
	Optional<Version> getLastSeenLiveVersion() const { return lastSeenLiveVersion; }
};

class PaxosConfigTransactionImpl {
	CoordinatorsHash coordinatorsHash{ 0 };
	std::vector<ConfigTransactionInterface> ctis;
	GetGenerationQuorum getGenerationQuorum;
	CommitQuorum commitQuorum;
	int numRetries{ 0 };
	Optional<UID> dID;
	Database cx;
	Future<Void> watchClusterFileFuture;

																#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via get()
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class GetActor>
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetActorState {
															#line 1505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetActorState(PaxosConfigTransactionImpl* const& self,Key const& key) 
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self),
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   key(key),
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   configKey(ConfigKey::decodeKey(key))
															#line 1516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			;
															#line 1531 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
		this->~GetActorState();
		static_cast<GetActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			StrictFuture<ConfigGeneration> __when_expr_0 = self->getGenerationQuorum.getGeneration();
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (static_cast<GetActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetActor*>(this)->actor_wait_state = 1;
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetActor, 0, ConfigGeneration >*>(static_cast<GetActor*>(this)));
															#line 1569 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (e.code() != error_code_timed_out && e.code() != error_code_broken_promise && e.code() != error_code_coordinators_changed)
															#line 1591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 1595 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			self->reset();
															#line 1599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			loopDepth = a_body1loopBody1cont1(loopDepth);
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
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		readReplicas = self->getGenerationQuorum.getReadReplicas();
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		std::vector<Future<Void>> fs;
															#line 298 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( ConfigTransactionInterface& readReplica : readReplicas ) {
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (readReplica.hostname.present())
															#line 1620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				fs.push_back(tryInitializeRequestStream( &readReplica.get, readReplica.hostname.get(), WLTOKEN_CONFIGTXN_GET));
															#line 1624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
		}
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<Void> __when_expr_1 = waitForAll(fs);
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetActor*>(this)->actor_wait_state = 2;
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetActor, 1, Void >*>(static_cast<GetActor*>(this)));
															#line 1636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(ConfigGeneration const& __generation,int loopDepth) 
	{
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		generation = __generation;
															#line 1645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(ConfigGeneration && __generation,int loopDepth) 
	{
		generation = std::move(__generation);
		loopDepth = a_body1loopBody1cont2(loopDepth);

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
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetActor, 0, ConfigGeneration >*,ConfigGeneration && value) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetActor, 0, ConfigGeneration >*,Error err) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		configNodes = Reference<ConfigTransactionInfo>(new ConfigTransactionInfo(readReplicas));
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<ConfigTransactionGetReply> __when_expr_2 = timeoutError( basicLoadBalance(configNodes, &ConfigTransactionInterface::get, ConfigTransactionGetRequest{ self->coordinatorsHash, generation, configKey }), CLIENT_KNOBS->GET_KNOB_TIMEOUT);
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetActor*>(this)->actor_wait_state = 3;
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetActor, 2, ConfigTransactionGetReply >*>(static_cast<GetActor*>(this)));
															#line 1721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		configNodes = Reference<ConfigTransactionInfo>(new ConfigTransactionInfo(readReplicas));
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<ConfigTransactionGetReply> __when_expr_2 = timeoutError( basicLoadBalance(configNodes, &ConfigTransactionInterface::get, ConfigTransactionGetRequest{ self->coordinatorsHash, generation, configKey }), CLIENT_KNOBS->GET_KNOB_TIMEOUT);
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1734 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetActor*>(this)->actor_wait_state = 3;
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetActor, 2, ConfigTransactionGetReply >*>(static_cast<GetActor*>(this)));
															#line 1739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
		if (static_cast<GetActor*>(this)->actor_wait_state > 0) static_cast<GetActor*>(this)->actor_wait_state = 0;
		static_cast<GetActor*>(this)->ActorCallback< GetActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont6(ConfigTransactionGetReply const& reply,int loopDepth) 
	{
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (reply.value.present())
															#line 1811 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		{
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (!static_cast<GetActor*>(this)->SAV<Optional<Value>>::futures) { (void)(reply.value.get().toValue()); this->~GetActorState(); static_cast<GetActor*>(this)->destroy(); return 0; }
															#line 1815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			new (&static_cast<GetActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(reply.value.get().toValue());
			this->~GetActorState();
			static_cast<GetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (!static_cast<GetActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>{}); this->~GetActorState(); static_cast<GetActor*>(this)->destroy(); return 0; }
															#line 1825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			new (&static_cast<GetActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>{});
			this->~GetActorState();
			static_cast<GetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1loopBody1cont6(ConfigTransactionGetReply && reply,int loopDepth) 
	{
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (reply.value.present())
															#line 1838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		{
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (!static_cast<GetActor*>(this)->SAV<Optional<Value>>::futures) { (void)(reply.value.get().toValue()); this->~GetActorState(); static_cast<GetActor*>(this)->destroy(); return 0; }
															#line 1842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			new (&static_cast<GetActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(reply.value.get().toValue());
			this->~GetActorState();
			static_cast<GetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (!static_cast<GetActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>{}); this->~GetActorState(); static_cast<GetActor*>(this)->destroy(); return 0; }
															#line 1852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			new (&static_cast<GetActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>{});
			this->~GetActorState();
			static_cast<GetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(ConfigTransactionGetReply const& reply,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(reply, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(ConfigTransactionGetReply && reply,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(reply), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetActor*>(this)->actor_wait_state > 0) static_cast<GetActor*>(this)->actor_wait_state = 0;
		static_cast<GetActor*>(this)->ActorCallback< GetActor, 2, ConfigTransactionGetReply >::remove();

	}
	void a_callback_fire(ActorCallback< GetActor, 2, ConfigTransactionGetReply >*,ConfigTransactionGetReply const& value) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetActor, 2, ConfigTransactionGetReply >*,ConfigTransactionGetReply && value) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetActor, 2, ConfigTransactionGetReply >*,Error err) 
	{
		fdb_probe_actor_enter("get", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("get", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	PaxosConfigTransactionImpl* self;
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	Key key;
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigKey configKey;
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigGeneration generation;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	std::vector<ConfigTransactionInterface> readReplicas;
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	Reference<ConfigTransactionInfo> configNodes;
															#line 1936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via get()
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetActor final : public Actor<Optional<Value>>, public ActorCallback< GetActor, 0, ConfigGeneration >, public ActorCallback< GetActor, 1, Void >, public ActorCallback< GetActor, 2, ConfigTransactionGetReply >, public FastAllocated<GetActor>, public GetActorState<GetActor> {
															#line 1941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<GetActor>::operator new;
	using FastAllocated<GetActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<Value>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetActor, 0, ConfigGeneration >;
friend struct ActorCallback< GetActor, 1, Void >;
friend struct ActorCallback< GetActor, 2, ConfigTransactionGetReply >;
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetActor(PaxosConfigTransactionImpl* const& self,Key const& key) 
															#line 1954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
		case 3: this->a_callback_error((ActorCallback< GetActor, 2, ConfigTransactionGetReply >*)0, actor_cancelled()); break;
		}

	}
};
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<Optional<Value>> get( PaxosConfigTransactionImpl* const& self, Key const& key ) {
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<Optional<Value>>(new GetActor(self, key));
															#line 1983 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

																#line 1988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via getConfigClasses()
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class GetConfigClassesActor>
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetConfigClassesActorState {
															#line 1994 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetConfigClassesActorState(PaxosConfigTransactionImpl* const& self) 
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self)
															#line 2001 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			;
															#line 2016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
		this->~GetConfigClassesActorState();
		static_cast<GetConfigClassesActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			StrictFuture<ConfigGeneration> __when_expr_0 = self->getGenerationQuorum.getGeneration();
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 1;
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetConfigClassesActor, 0, ConfigGeneration >*>(static_cast<GetConfigClassesActor*>(this)));
															#line 2054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (e.code() != error_code_coordinators_changed)
															#line 2076 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 2080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			self->reset();
															#line 2084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			loopDepth = a_body1loopBody1cont1(loopDepth);
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
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		readReplicas = self->getGenerationQuorum.getReadReplicas();
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		std::vector<Future<Void>> fs;
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( ConfigTransactionInterface& readReplica : readReplicas ) {
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (readReplica.hostname.present())
															#line 2105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				fs.push_back(tryInitializeRequestStream( &readReplica.getClasses, readReplica.hostname.get(), WLTOKEN_CONFIGTXN_GETCLASSES));
															#line 2109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
		}
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<Void> __when_expr_1 = waitForAll(fs);
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 2;
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetConfigClassesActor, 1, Void >*>(static_cast<GetConfigClassesActor*>(this)));
															#line 2121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(ConfigGeneration const& __generation,int loopDepth) 
	{
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		generation = __generation;
															#line 2130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(ConfigGeneration && __generation,int loopDepth) 
	{
		generation = std::move(__generation);
		loopDepth = a_body1loopBody1cont2(loopDepth);

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
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetConfigClassesActor, 0, ConfigGeneration >*,ConfigGeneration && value) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetConfigClassesActor, 0, ConfigGeneration >*,Error err) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		configNodes = Reference<ConfigTransactionInfo>(new ConfigTransactionInfo(readReplicas));
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<ConfigTransactionGetConfigClassesReply> __when_expr_2 = basicLoadBalance(configNodes, &ConfigTransactionInterface::getClasses, ConfigTransactionGetConfigClassesRequest{ self->coordinatorsHash, generation });
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 3;
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetConfigClassesActor, 2, ConfigTransactionGetConfigClassesReply >*>(static_cast<GetConfigClassesActor*>(this)));
															#line 2206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		configNodes = Reference<ConfigTransactionInfo>(new ConfigTransactionInfo(readReplicas));
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<ConfigTransactionGetConfigClassesReply> __when_expr_2 = basicLoadBalance(configNodes, &ConfigTransactionInterface::getClasses, ConfigTransactionGetConfigClassesRequest{ self->coordinatorsHash, generation });
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 3;
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetConfigClassesActor, 2, ConfigTransactionGetConfigClassesReply >*>(static_cast<GetConfigClassesActor*>(this)));
															#line 2224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
		if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state > 0) static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 0;
		static_cast<GetConfigClassesActor*>(this)->ActorCallback< GetConfigClassesActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetConfigClassesActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetConfigClassesActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetConfigClassesActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont6(ConfigTransactionGetConfigClassesReply const& reply,int loopDepth) 
	{
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		RangeResult result;
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		result.reserve(result.arena(), reply.configClasses.size());
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( const auto& configClass : reply.configClasses ) {
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(configClass, ""_sr));
															#line 2302 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		}
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<GetConfigClassesActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetConfigClassesActorState(); static_cast<GetConfigClassesActor*>(this)->destroy(); return 0; }
															#line 2306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<GetConfigClassesActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetConfigClassesActorState();
		static_cast<GetConfigClassesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(ConfigTransactionGetConfigClassesReply && reply,int loopDepth) 
	{
															#line 345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		RangeResult result;
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		result.reserve(result.arena(), reply.configClasses.size());
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( const auto& configClass : reply.configClasses ) {
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(configClass, ""_sr));
															#line 2324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		}
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<GetConfigClassesActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetConfigClassesActorState(); static_cast<GetConfigClassesActor*>(this)->destroy(); return 0; }
															#line 2328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<GetConfigClassesActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetConfigClassesActorState();
		static_cast<GetConfigClassesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(ConfigTransactionGetConfigClassesReply const& reply,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(reply, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(ConfigTransactionGetConfigClassesReply && reply,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(reply), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state > 0) static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 0;
		static_cast<GetConfigClassesActor*>(this)->ActorCallback< GetConfigClassesActor, 2, ConfigTransactionGetConfigClassesReply >::remove();

	}
	void a_callback_fire(ActorCallback< GetConfigClassesActor, 2, ConfigTransactionGetConfigClassesReply >*,ConfigTransactionGetConfigClassesReply const& value) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetConfigClassesActor, 2, ConfigTransactionGetConfigClassesReply >*,ConfigTransactionGetConfigClassesReply && value) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetConfigClassesActor, 2, ConfigTransactionGetConfigClassesReply >*,Error err) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	PaxosConfigTransactionImpl* self;
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigGeneration generation;
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	std::vector<ConfigTransactionInterface> readReplicas;
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	Reference<ConfigTransactionInfo> configNodes;
															#line 2407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via getConfigClasses()
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetConfigClassesActor final : public Actor<RangeResult>, public ActorCallback< GetConfigClassesActor, 0, ConfigGeneration >, public ActorCallback< GetConfigClassesActor, 1, Void >, public ActorCallback< GetConfigClassesActor, 2, ConfigTransactionGetConfigClassesReply >, public FastAllocated<GetConfigClassesActor>, public GetConfigClassesActorState<GetConfigClassesActor> {
															#line 2412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<GetConfigClassesActor>::operator new;
	using FastAllocated<GetConfigClassesActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetConfigClassesActor, 0, ConfigGeneration >;
friend struct ActorCallback< GetConfigClassesActor, 1, Void >;
friend struct ActorCallback< GetConfigClassesActor, 2, ConfigTransactionGetConfigClassesReply >;
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetConfigClassesActor(PaxosConfigTransactionImpl* const& self) 
															#line 2425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
		case 3: this->a_callback_error((ActorCallback< GetConfigClassesActor, 2, ConfigTransactionGetConfigClassesReply >*)0, actor_cancelled()); break;
		}

	}
};
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<RangeResult> getConfigClasses( PaxosConfigTransactionImpl* const& self ) {
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<RangeResult>(new GetConfigClassesActor(self));
															#line 2454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

																#line 2459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via getKnobs()
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class GetKnobsActor>
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetKnobsActorState {
															#line 2465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetKnobsActorState(PaxosConfigTransactionImpl* const& self,Optional<Key> const& configClass) 
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self),
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   configClass(configClass)
															#line 2474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			;
															#line 2489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
		this->~GetKnobsActorState();
		static_cast<GetKnobsActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			StrictFuture<ConfigGeneration> __when_expr_0 = self->getGenerationQuorum.getGeneration();
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (static_cast<GetKnobsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetKnobsActor*>(this)->actor_wait_state = 1;
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetKnobsActor, 0, ConfigGeneration >*>(static_cast<GetKnobsActor*>(this)));
															#line 2527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (e.code() != error_code_coordinators_changed)
															#line 2549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 2553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			self->reset();
															#line 2557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			loopDepth = a_body1loopBody1cont1(loopDepth);
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
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		readReplicas = self->getGenerationQuorum.getReadReplicas();
															#line 366 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		std::vector<Future<Void>> fs;
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( ConfigTransactionInterface& readReplica : readReplicas ) {
															#line 368 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (readReplica.hostname.present())
															#line 2578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				fs.push_back(tryInitializeRequestStream( &readReplica.getKnobs, readReplica.hostname.get(), WLTOKEN_CONFIGTXN_GETKNOBS));
															#line 2582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
		}
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<Void> __when_expr_1 = waitForAll(fs);
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetKnobsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetKnobsActor*>(this)->actor_wait_state = 2;
															#line 373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetKnobsActor, 1, Void >*>(static_cast<GetKnobsActor*>(this)));
															#line 2594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(ConfigGeneration const& __generation,int loopDepth) 
	{
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		generation = __generation;
															#line 2603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(ConfigGeneration && __generation,int loopDepth) 
	{
		generation = std::move(__generation);
		loopDepth = a_body1loopBody1cont2(loopDepth);

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
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetKnobsActor, 0, ConfigGeneration >*,ConfigGeneration && value) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetKnobsActor, 0, ConfigGeneration >*,Error err) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		configNodes = Reference<ConfigTransactionInfo>(new ConfigTransactionInfo(readReplicas));
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<ConfigTransactionGetKnobsReply> __when_expr_2 = basicLoadBalance( configNodes, &ConfigTransactionInterface::getKnobs, ConfigTransactionGetKnobsRequest{ self->coordinatorsHash, generation, configClass });
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetKnobsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetKnobsActor*>(this)->actor_wait_state = 3;
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetKnobsActor, 2, ConfigTransactionGetKnobsReply >*>(static_cast<GetKnobsActor*>(this)));
															#line 2679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		configNodes = Reference<ConfigTransactionInfo>(new ConfigTransactionInfo(readReplicas));
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<ConfigTransactionGetKnobsReply> __when_expr_2 = basicLoadBalance( configNodes, &ConfigTransactionInterface::getKnobs, ConfigTransactionGetKnobsRequest{ self->coordinatorsHash, generation, configClass });
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetKnobsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetKnobsActor*>(this)->actor_wait_state = 3;
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetKnobsActor, 2, ConfigTransactionGetKnobsReply >*>(static_cast<GetKnobsActor*>(this)));
															#line 2697 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
		if (static_cast<GetKnobsActor*>(this)->actor_wait_state > 0) static_cast<GetKnobsActor*>(this)->actor_wait_state = 0;
		static_cast<GetKnobsActor*>(this)->ActorCallback< GetKnobsActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetKnobsActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< GetKnobsActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< GetKnobsActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont6(ConfigTransactionGetKnobsReply const& reply,int loopDepth) 
	{
															#line 379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		RangeResult result;
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		result.reserve(result.arena(), reply.knobNames.size());
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( const auto& knobName : reply.knobNames ) {
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(knobName, ""_sr));
															#line 2775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		}
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<GetKnobsActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetKnobsActorState(); static_cast<GetKnobsActor*>(this)->destroy(); return 0; }
															#line 2779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<GetKnobsActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetKnobsActorState();
		static_cast<GetKnobsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(ConfigTransactionGetKnobsReply && reply,int loopDepth) 
	{
															#line 379 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		RangeResult result;
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		result.reserve(result.arena(), reply.knobNames.size());
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( const auto& knobName : reply.knobNames ) {
															#line 382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(knobName, ""_sr));
															#line 2797 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		}
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<GetKnobsActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetKnobsActorState(); static_cast<GetKnobsActor*>(this)->destroy(); return 0; }
															#line 2801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<GetKnobsActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetKnobsActorState();
		static_cast<GetKnobsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(ConfigTransactionGetKnobsReply const& reply,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(reply, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(ConfigTransactionGetKnobsReply && reply,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(reply), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<GetKnobsActor*>(this)->actor_wait_state > 0) static_cast<GetKnobsActor*>(this)->actor_wait_state = 0;
		static_cast<GetKnobsActor*>(this)->ActorCallback< GetKnobsActor, 2, ConfigTransactionGetKnobsReply >::remove();

	}
	void a_callback_fire(ActorCallback< GetKnobsActor, 2, ConfigTransactionGetKnobsReply >*,ConfigTransactionGetKnobsReply const& value) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetKnobsActor, 2, ConfigTransactionGetKnobsReply >*,ConfigTransactionGetKnobsReply && value) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetKnobsActor, 2, ConfigTransactionGetKnobsReply >*,Error err) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	PaxosConfigTransactionImpl* self;
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	Optional<Key> configClass;
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigGeneration generation;
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	std::vector<ConfigTransactionInterface> readReplicas;
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	Reference<ConfigTransactionInfo> configNodes;
															#line 2882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via getKnobs()
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetKnobsActor final : public Actor<RangeResult>, public ActorCallback< GetKnobsActor, 0, ConfigGeneration >, public ActorCallback< GetKnobsActor, 1, Void >, public ActorCallback< GetKnobsActor, 2, ConfigTransactionGetKnobsReply >, public FastAllocated<GetKnobsActor>, public GetKnobsActorState<GetKnobsActor> {
															#line 2887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<GetKnobsActor>::operator new;
	using FastAllocated<GetKnobsActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<RangeResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetKnobsActor, 0, ConfigGeneration >;
friend struct ActorCallback< GetKnobsActor, 1, Void >;
friend struct ActorCallback< GetKnobsActor, 2, ConfigTransactionGetKnobsReply >;
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetKnobsActor(PaxosConfigTransactionImpl* const& self,Optional<Key> const& configClass) 
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
		case 3: this->a_callback_error((ActorCallback< GetKnobsActor, 2, ConfigTransactionGetKnobsReply >*)0, actor_cancelled()); break;
		}

	}
};
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<RangeResult> getKnobs( PaxosConfigTransactionImpl* const& self, Optional<Key> const& configClass ) {
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<RangeResult>(new GetKnobsActor(self, configClass));
															#line 2929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

																#line 2934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via commit()
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class CommitActor>
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class CommitActorState {
															#line 2940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	CommitActorState(PaxosConfigTransactionImpl* const& self) 
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self)
															#line 2947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 395 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			;
															#line 2962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
		this->~CommitActorState();
		static_cast<CommitActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			StrictFuture<ConfigGeneration> __when_expr_0 = self->getGenerationQuorum.getGeneration();
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CommitActor*>(this)->actor_wait_state = 1;
															#line 397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 0, ConfigGeneration >*>(static_cast<CommitActor*>(this)));
															#line 3000 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (e.code() != error_code_coordinators_changed)
															#line 3022 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 3026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
															#line 405 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			self->reset();
															#line 3030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(ConfigGeneration const& generation,int loopDepth) 
	{
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->commitQuorum.setTimestamp();
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<Void> __when_expr_1 = self->commitQuorum.commit(generation, self->coordinatorsHash);
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<CommitActor*>(this)->actor_wait_state = 2;
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 1, Void >*>(static_cast<CommitActor*>(this)));
															#line 3054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(ConfigGeneration && generation,int loopDepth) 
	{
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->commitQuorum.setTimestamp();
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<Void> __when_expr_1 = self->commitQuorum.commit(generation, self->coordinatorsHash);
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3067 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<CommitActor*>(this)->actor_wait_state = 2;
															#line 399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 1, Void >*>(static_cast<CommitActor*>(this)));
															#line 3072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(ConfigGeneration const& generation,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(generation, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(ConfigGeneration && generation,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(generation), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CommitActor*>(this)->actor_wait_state > 0) static_cast<CommitActor*>(this)->actor_wait_state = 0;
		static_cast<CommitActor*>(this)->ActorCallback< CommitActor, 0, ConfigGeneration >::remove();

	}
	void a_callback_fire(ActorCallback< CommitActor, 0, ConfigGeneration >*,ConfigGeneration const& value) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CommitActor, 0, ConfigGeneration >*,ConfigGeneration && value) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CommitActor, 0, ConfigGeneration >*,Error err) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<CommitActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CommitActorState(); static_cast<CommitActor*>(this)->destroy(); return 0; }
															#line 3144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<CommitActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CommitActorState();
		static_cast<CommitActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<CommitActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CommitActorState(); static_cast<CommitActor*>(this)->destroy(); return 0; }
															#line 3156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<CommitActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CommitActorState();
		static_cast<CommitActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<CommitActor*>(this)->actor_wait_state > 0) static_cast<CommitActor*>(this)->actor_wait_state = 0;
		static_cast<CommitActor*>(this)->ActorCallback< CommitActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CommitActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CommitActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CommitActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("commit", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("commit", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	PaxosConfigTransactionImpl* self;
															#line 3229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via commit()
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class CommitActor final : public Actor<Void>, public ActorCallback< CommitActor, 0, ConfigGeneration >, public ActorCallback< CommitActor, 1, Void >, public FastAllocated<CommitActor>, public CommitActorState<CommitActor> {
															#line 3234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<CommitActor>::operator new;
	using FastAllocated<CommitActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CommitActor, 0, ConfigGeneration >;
friend struct ActorCallback< CommitActor, 1, Void >;
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	CommitActor(PaxosConfigTransactionImpl* const& self) 
															#line 3246 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
		case 1: this->a_callback_error((ActorCallback< CommitActor, 0, ConfigGeneration >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CommitActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<Void> commit( PaxosConfigTransactionImpl* const& self ) {
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<Void>(new CommitActor(self));
															#line 3274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 409 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

																#line 3279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via onError()
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class OnErrorActor>
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class OnErrorActorState {
															#line 3285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	OnErrorActorState(PaxosConfigTransactionImpl* const& self,Error const& e) 
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self),
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   e(e)
															#line 3294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			TraceEvent("ConfigIncrementOnError").error(e).detail("NumRetries", self->numRetries);
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (e.code() == error_code_transaction_too_old || e.code() == error_code_not_committed)
															#line 3311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				StrictFuture<Void> __when_expr_0 = delay(std::clamp((1 << self->numRetries++) * 0.01 * deterministicRandom()->random01(), 0.0, CLIENT_KNOBS->TIMEOUT_RETRY_UPPER_BOUND));
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				if (static_cast<OnErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<OnErrorActor*>(this)->actor_wait_state = 1;
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnErrorActor, 0, Void >*>(static_cast<OnErrorActor*>(this)));
															#line 3322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		return a_body1Catch1(e, loopDepth);
															#line 3350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->reset();
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<OnErrorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnErrorActorState(); static_cast<OnErrorActor*>(this)->destroy(); return 0; }
															#line 3360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<OnErrorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnErrorActorState();
		static_cast<OnErrorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->reset();
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<OnErrorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnErrorActorState(); static_cast<OnErrorActor*>(this)->destroy(); return 0; }
															#line 3374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	PaxosConfigTransactionImpl* self;
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	Error e;
															#line 3449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via onError()
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class OnErrorActor final : public Actor<Void>, public ActorCallback< OnErrorActor, 0, Void >, public FastAllocated<OnErrorActor>, public OnErrorActorState<OnErrorActor> {
															#line 3454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<OnErrorActor>::operator new;
	using FastAllocated<OnErrorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnErrorActor, 0, Void >;
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	OnErrorActor(PaxosConfigTransactionImpl* const& self,Error const& e) 
															#line 3465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<Void> onError( PaxosConfigTransactionImpl* const& self, Error const& e ) {
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<Void>(new OnErrorActor(self, e));
															#line 3492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

	// Returns when the cluster interface updates with a new connection string.
																#line 3498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via watchClusterFile()
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class WatchClusterFileActor>
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class WatchClusterFileActorState {
															#line 3504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	WatchClusterFileActorState(Database const& cx) 
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : cx(cx),
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   leaderMonitor(monitorLeader<ClusterInterface>(cx->getConnectionRecord(), cx->statusClusterInterface)),
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   connectionString(cx->getConnectionRecord()->getConnectionString().toString())
															#line 3515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
	{
		fdb_probe_actor_create("watchClusterFile", reinterpret_cast<unsigned long>(this));

	}
	~WatchClusterFileActorState() 
	{
		fdb_probe_actor_destroy("watchClusterFile", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			;
															#line 3530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
		this->~WatchClusterFileActorState();
		static_cast<WatchClusterFileActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<Void> __when_expr_0 = cx->statusClusterInterface->onChange();
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<WatchClusterFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<WatchClusterFileActor*>(this)->actor_wait_state = 1;
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< WatchClusterFileActor, 0, Void >*>(static_cast<WatchClusterFileActor*>(this)));
															#line 3567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (cx->getConnectionRecord()->getConnectionString().toString() != connectionString)
															#line 3576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		{
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (!static_cast<WatchClusterFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WatchClusterFileActorState(); static_cast<WatchClusterFileActor*>(this)->destroy(); return 0; }
															#line 3580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			new (&static_cast<WatchClusterFileActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WatchClusterFileActorState();
			static_cast<WatchClusterFileActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (cx->getConnectionRecord()->getConnectionString().toString() != connectionString)
															#line 3594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		{
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (!static_cast<WatchClusterFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~WatchClusterFileActorState(); static_cast<WatchClusterFileActor*>(this)->destroy(); return 0; }
															#line 3598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			new (&static_cast<WatchClusterFileActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~WatchClusterFileActorState();
			static_cast<WatchClusterFileActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
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
		if (static_cast<WatchClusterFileActor*>(this)->actor_wait_state > 0) static_cast<WatchClusterFileActor*>(this)->actor_wait_state = 0;
		static_cast<WatchClusterFileActor*>(this)->ActorCallback< WatchClusterFileActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< WatchClusterFileActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("watchClusterFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchClusterFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< WatchClusterFileActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("watchClusterFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchClusterFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< WatchClusterFileActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("watchClusterFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("watchClusterFile", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	Database cx;
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	Future<Void> leaderMonitor;
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	std::string connectionString;
															#line 3677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via watchClusterFile()
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class WatchClusterFileActor final : public Actor<Void>, public ActorCallback< WatchClusterFileActor, 0, Void >, public FastAllocated<WatchClusterFileActor>, public WatchClusterFileActorState<WatchClusterFileActor> {
															#line 3682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<WatchClusterFileActor>::operator new;
	using FastAllocated<WatchClusterFileActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< WatchClusterFileActor, 0, Void >;
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	WatchClusterFileActor(Database const& cx) 
															#line 3693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		 : Actor<Void>(),
		   WatchClusterFileActorState<WatchClusterFileActor>(cx)
	{
		fdb_probe_actor_enter("watchClusterFile", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("watchClusterFile");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("watchClusterFile", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< WatchClusterFileActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<Void> watchClusterFile( Database const& cx ) {
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<Void>(new WatchClusterFileActor(cx));
															#line 3720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

public:
	Future<Version> getReadVersion() {
		return map(getGenerationQuorum.getGeneration(), [](auto const& gen) { return gen.committedVersion; });
	}

	Optional<Version> getCachedReadVersion() const {
		auto gen = getGenerationQuorum.getCachedGeneration();
		if (gen.present()) {
			return gen.get().committedVersion;
		} else {
			return {};
		}
	}

	Version getCommittedVersion() const {
		return commitQuorum.committed() ? getGenerationQuorum.getCachedGeneration().get().liveVersion
		                                : ::invalidVersion;
	}

	int64_t getApproximateSize() const { return commitQuorum.expectedSize(); }

	void set(KeyRef key, ValueRef value) { commitQuorum.set(key, value); }

	void clear(KeyRef key) { commitQuorum.clear(key); }

	Future<Optional<Value>> get(Key const& key) { return get(this, key); }

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

	Future<Void> onError(Error const& e) { return onError(this, e); }

	void debugTransaction(UID dID) { this->dID = dID; }

	void reset() {
		ctis.clear();
		// Re-read connection string. If the cluster file changed, this will
		// return the updated value.
		const ClusterConnectionString& cs = cx->getConnectionRecord()->getConnectionString();
		ctis.reserve(cs.hostnames.size() + cs.coords.size());
		for (const auto& h : cs.hostnames) {
			ctis.emplace_back(h);
		}
		for (const auto& c : cs.coords) {
			ctis.emplace_back(c);
		}
		coordinatorsHash = std::hash<std::string>()(cx->getConnectionRecord()->getConnectionString().toString());
		if (!cx->statusLeaderMon.isValid() || cx->statusLeaderMon.isReady()) {
			cx->statusClusterInterface = makeReference<AsyncVar<Optional<ClusterInterface>>>();
			cx->statusLeaderMon = watchClusterFile(cx);
		}
		getGenerationQuorum = GetGenerationQuorum{
			coordinatorsHash, ctis, cx->statusLeaderMon, getGenerationQuorum.getLastSeenLiveVersion()
		};
		commitQuorum = CommitQuorum{ ctis };
	}

	void fullReset() {
		numRetries = 0;
		dID = {};
		reset();
	}

	void checkDeferredError(Error const& deferredError) const {
		if (deferredError.code() != invalid_error_code) {
			throw deferredError;
		}
		if (cx.getPtr()) {
			cx->checkDeferredError();
		}
	}

	Future<Void> commit() { return commit(this); }

	PaxosConfigTransactionImpl(Database const& cx) : cx(cx) { reset(); }

	PaxosConfigTransactionImpl(std::vector<ConfigTransactionInterface> const& ctis)
	  : ctis(ctis), getGenerationQuorum(0, ctis, Future<Void>()), commitQuorum(ctis) {}
};

Future<Version> PaxosConfigTransaction::getReadVersion() {
	return impl->getReadVersion();
}

Optional<Version> PaxosConfigTransaction::getCachedReadVersion() const {
	return impl->getCachedReadVersion();
}

Future<Optional<Value>> PaxosConfigTransaction::get(Key const& key, Snapshot) {
	return impl->get(key);
}

Future<RangeResult> PaxosConfigTransaction::getRange(KeySelector const& begin,
                                                     KeySelector const& end,
                                                     int limit,
                                                     Snapshot snapshot,
                                                     Reverse reverse) {
	if (reverse) {
		throw client_invalid_operation();
	}
	return impl->getRange(KeyRangeRef(begin.getKey(), end.getKey()));
}

Future<RangeResult> PaxosConfigTransaction::getRange(KeySelector begin,
                                                     KeySelector end,
                                                     GetRangeLimits limits,
                                                     Snapshot snapshot,
                                                     Reverse reverse) {
	if (reverse) {
		throw client_invalid_operation();
	}
	return impl->getRange(KeyRangeRef(begin.getKey(), end.getKey()));
}

void PaxosConfigTransaction::set(KeyRef const& key, ValueRef const& value) {
	return impl->set(key, value);
}

void PaxosConfigTransaction::clear(KeyRef const& key) {
	return impl->clear(key);
}

Future<Void> PaxosConfigTransaction::commit() {
	return impl->commit();
}

Version PaxosConfigTransaction::getCommittedVersion() const {
	return impl->getCommittedVersion();
}

double PaxosConfigTransaction::getTagThrottledDuration() const {
	return 0.0;
}

int64_t PaxosConfigTransaction::getTotalCost() const {
	return 0;
}

int64_t PaxosConfigTransaction::getApproximateSize() const {
	return impl->getApproximateSize();
}

void PaxosConfigTransaction::setOption(FDBTransactionOptions::Option option, Optional<StringRef> value) {
	// TODO: Support using this option to determine atomicity
}

Future<Void> PaxosConfigTransaction::onError(Error const& e) {
	return impl->onError(e);
}

void PaxosConfigTransaction::cancel() {
	// TODO: Implement someday
	throw client_invalid_operation();
}

void PaxosConfigTransaction::reset() {
	impl->reset();
}

void PaxosConfigTransaction::fullReset() {
	impl->fullReset();
}

void PaxosConfigTransaction::debugTransaction(UID dID) {
	impl->debugTransaction(dID);
}

void PaxosConfigTransaction::checkDeferredError() const {
	impl->checkDeferredError(deferredError);
}

PaxosConfigTransaction::PaxosConfigTransaction(std::vector<ConfigTransactionInterface> const& ctis)
  : impl(PImpl<PaxosConfigTransactionImpl>::create(ctis)) {}

PaxosConfigTransaction::PaxosConfigTransaction() = default;

PaxosConfigTransaction::~PaxosConfigTransaction() = default;

void PaxosConfigTransaction::construct(Database const& cx) {
	impl = PImpl<PaxosConfigTransactionImpl>::create(cx);
}
