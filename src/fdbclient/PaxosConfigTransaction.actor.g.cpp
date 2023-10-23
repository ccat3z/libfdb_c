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

	ConfigTransactionCommitRequest getCommitRequest(ConfigGeneration generation) const {
		return ConfigTransactionCommitRequest(generation, mutations, annotation);
	}

	void updateResult() {
		if (successful >= ctis.size() / 2 + 1 && result.canBeSet()) {
			result.send(Void());
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

																#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via addRequestActor()
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class AddRequestActorActor>
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class AddRequestActorActorState {
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	AddRequestActorActorState(CommitQuorum* const& self,ConfigGeneration const& generation,ConfigTransactionInterface const& cti) 
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self),
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   generation(generation),
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   cti(cti)
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				if (cti.hostname.present())
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				{
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					StrictFuture<Void> __when_expr_0 = timeoutError(retryGetReplyFromHostname( self->getCommitRequest(generation), cti.hostname.get(), WLTOKEN_CONFIGTXN_COMMIT), CLIENT_KNOBS->COMMIT_QUORUM_TIMEOUT);
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					if (static_cast<AddRequestActorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
					if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
					static_cast<AddRequestActorActor*>(this)->actor_wait_state = 1;
															#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AddRequestActorActor, 0, Void >*>(static_cast<AddRequestActorActor*>(this)));
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
					loopDepth = 0;
				}
				else
				{
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					StrictFuture<Void> __when_expr_1 = timeoutError(cti.commit.getReply(self->getCommitRequest(generation)), CLIENT_KNOBS->COMMIT_QUORUM_TIMEOUT);
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					if (static_cast<AddRequestActorActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
					if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
					static_cast<AddRequestActorActor*>(this)->actor_wait_state = 2;
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AddRequestActorActor, 1, Void >*>(static_cast<AddRequestActorActor*>(this)));
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->updateResult();
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<AddRequestActorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AddRequestActorActorState(); static_cast<AddRequestActorActor*>(this)->destroy(); return 0; }
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<AddRequestActorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~AddRequestActorActorState();
		static_cast<AddRequestActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 78 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
															#line 82 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (e.code() == error_code_not_committed || e.code() == error_code_timed_out)
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 83 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				++self->failed;
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
			else
			{
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				++self->maybeCommitted;
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 75 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		++self->successful;
															#line 202 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	CommitQuorum* self;
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigGeneration generation;
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigTransactionInterface cti;
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via addRequestActor()
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class AddRequestActorActor final : public Actor<Void>, public ActorCallback< AddRequestActorActor, 0, Void >, public ActorCallback< AddRequestActorActor, 1, Void >, public FastAllocated<AddRequestActorActor>, public AddRequestActorActorState<AddRequestActorActor> {
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<AddRequestActorActor>::operator new;
	using FastAllocated<AddRequestActorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AddRequestActorActor, 0, Void >;
friend struct ActorCallback< AddRequestActorActor, 1, Void >;
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	AddRequestActorActor(CommitQuorum* const& self,ConfigGeneration const& generation,ConfigTransactionInterface const& cti) 
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		 : Actor<Void>(),
		   AddRequestActorActorState<AddRequestActorActor>(self, generation, cti)
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
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<Void> addRequestActor( CommitQuorum* const& self, ConfigGeneration const& generation, ConfigTransactionInterface const& cti ) {
															#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<Void>(new AddRequestActorActor(self, generation, cti));
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

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
	Future<Void> commit(ConfigGeneration generation) {
		// Send commit message to all replicas, even those that did not return the used replica.
		// This way, slow replicas are kept up date.
		for (const auto& cti : ctis) {
			actors.add(addRequestActor(this, generation, cti));
		}
		return result.getFuture();
	}
	bool committed() const { return result.isSet() && !result.isError(); }
};

class GetGenerationQuorum {
	ActorCollection actors{ false };
	std::vector<ConfigTransactionInterface> ctis;
	std::map<ConfigGeneration, std::vector<ConfigTransactionInterface>> seenGenerations;
	Promise<ConfigGeneration> result;
	size_t totalRepliesReceived{ 0 };
	size_t maxAgreement{ 0 };
	Optional<Version> lastSeenLiveVersion;
	Future<ConfigGeneration> getGenerationFuture;

																#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via addRequestActor()
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class AddRequestActorActor1>
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class AddRequestActorActor1State {
															#line 473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	AddRequestActorActor1State(GetGenerationQuorum* const& self,ConfigTransactionInterface const& cti) 
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self),
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   cti(cti)
															#line 482 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			;
															#line 497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<AddRequestActorActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AddRequestActorActor1State(); static_cast<AddRequestActorActor1*>(this)->destroy(); return 0; }
															#line 520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			reply = ConfigTransactionGetGenerationReply();
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (cti.hostname.present())
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				StrictFuture<Void> __when_expr_0 = timeoutError(store(reply, retryGetReplyFromHostname( ConfigTransactionGetGenerationRequest{ self->lastSeenLiveVersion }, cti.hostname.get(), WLTOKEN_CONFIGTXN_GETGENERATION)), CLIENT_KNOBS->GET_GENERATION_QUORUM_TIMEOUT);
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				if (static_cast<AddRequestActorActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<AddRequestActorActor1*>(this)->actor_wait_state = 1;
															#line 138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AddRequestActorActor1, 0, Void >*>(static_cast<AddRequestActorActor1*>(this)));
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				StrictFuture<Void> __when_expr_1 = timeoutError(store(reply, cti.getGeneration.getReply( ConfigTransactionGetGenerationRequest{ self->lastSeenLiveVersion })), CLIENT_KNOBS->GET_GENERATION_QUORUM_TIMEOUT);
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				if (static_cast<AddRequestActorActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
				static_cast<AddRequestActorActor1*>(this)->actor_wait_state = 2;
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AddRequestActorActor1, 1, Void >*>(static_cast<AddRequestActorActor1*>(this)));
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (e.code() == error_code_broken_promise)
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
				return a_body1loopHead1(loopDepth); // continue
			}
			else
			{
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				if (e.code() == error_code_timed_out)
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				{
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					++self->totalRepliesReceived;
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					if (self->totalRepliesReceived == self->ctis.size() && self->result.canBeSet() && !self->result.isError())
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
					{
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
						auto local = self->result;
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
						local.sendError(failed_to_reach_quorum());
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
					}
					return a_body1break1(loopDepth==0?0:loopDepth-1); // break
				}
				else
				{
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 625 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		++self->totalRepliesReceived;
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		auto gen = reply.generation;
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->lastSeenLiveVersion = std::max(gen.liveVersion, self->lastSeenLiveVersion.orDefault(::invalidVersion));
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		auto& replicas = self->seenGenerations[gen];
															#line 156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		replicas.push_back(cti);
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->maxAgreement = std::max(replicas.size(), self->maxAgreement);
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (replicas.size() >= self->ctis.size() / 2 + 1 && !self->result.isSet())
															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		{
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			self->result.send(gen);
															#line 657 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		}
		else
		{
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (self->maxAgreement + (self->ctis.size() - self->totalRepliesReceived) < (self->ctis.size() / 2 + 1))
															#line 663 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				if (!self->result.isError())
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				{
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					auto local = self->result;
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
					local.sendError(failed_to_reach_quorum());
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetGenerationQuorum* self;
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigTransactionInterface cti;
															#line 136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigTransactionGetGenerationReply reply;
															#line 837 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via addRequestActor()
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class AddRequestActorActor1 final : public Actor<Void>, public ActorCallback< AddRequestActorActor1, 0, Void >, public ActorCallback< AddRequestActorActor1, 1, Void >, public FastAllocated<AddRequestActorActor1>, public AddRequestActorActor1State<AddRequestActorActor1> {
															#line 842 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<AddRequestActorActor1>::operator new;
	using FastAllocated<AddRequestActorActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AddRequestActorActor1, 0, Void >;
friend struct ActorCallback< AddRequestActorActor1, 1, Void >;
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	AddRequestActorActor1(GetGenerationQuorum* const& self,ConfigTransactionInterface const& cti) 
															#line 854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<Void> addRequestActor( GetGenerationQuorum* const& self, ConfigTransactionInterface const& cti ) {
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<Void>(new AddRequestActorActor1(self, cti));
															#line 882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

																#line 887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via getGenerationActor()
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class GetGenerationActorActor>
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetGenerationActorActorState {
															#line 893 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetGenerationActorActorState(GetGenerationQuorum* const& self) 
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self),
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   retries(0)
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			;
															#line 917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( const auto& cti : self->ctis ) {
															#line 193 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			self->actors.add(addRequestActor(self, cti));
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		}
		try {
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			StrictFuture<ConfigGeneration> __when_expr_0 = self->result.getFuture();
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (static_cast<GetGenerationActorActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			StrictFuture<Void> __when_expr_1 = self->actors.getResult();
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
			static_cast<GetGenerationActorActor*>(this)->actor_wait_state = 1;
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetGenerationActorActor, 0, ConfigGeneration >*>(static_cast<GetGenerationActorActor*>(this)));
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetGenerationActorActor, 1, Void >*>(static_cast<GetGenerationActorActor*>(this)));
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (e.code() == error_code_failed_to_reach_quorum)
															#line 989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				TEST(true);
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				StrictFuture<Void> __when_expr_2 = delayJittered( std::clamp(0.005 * (1 << retries), 0.0, CLIENT_KNOBS->TIMEOUT_RETRY_UPPER_BOUND));
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				if (static_cast<GetGenerationActorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 997 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
				static_cast<GetGenerationActorActor*>(this)->actor_wait_state = 2;
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetGenerationActorActor, 2, Void >*>(static_cast<GetGenerationActorActor*>(this)));
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<GetGenerationActorActor*>(this)->SAV<ConfigGeneration>::futures) { (void)(generation); this->~GetGenerationActorActorState(); static_cast<GetGenerationActorActor*>(this)->destroy(); return 0; }
															#line 1030 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<GetGenerationActorActor*>(this)->SAV< ConfigGeneration >::value()) ConfigGeneration(generation);
		this->~GetGenerationActorActorState();
		static_cast<GetGenerationActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(ConfigGeneration && generation,int loopDepth) 
	{
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<GetGenerationActorActor*>(this)->SAV<ConfigGeneration>::futures) { (void)(generation); this->~GetGenerationActorActorState(); static_cast<GetGenerationActorActor*>(this)->destroy(); return 0; }
															#line 1042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<GetGenerationActorActor*>(this)->SAV< ConfigGeneration >::value()) ConfigGeneration(generation);
		this->~GetGenerationActorActorState();
		static_cast<GetGenerationActorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		ASSERT(false);
															#line 1054 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = a_body1loopBody1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		ASSERT(false);
															#line 1063 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
	int a_body1loopBody1Catch1cont2(Void const& _,int loopDepth) 
	{
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		++retries;
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->actors.clear(false);
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->seenGenerations.clear();
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->result.reset();
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->totalRepliesReceived = 0;
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->maxAgreement = 0;
															#line 1198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = a_body1loopBody1Catch1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont2(Void && _,int loopDepth) 
	{
															#line 209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		++retries;
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->actors.clear(false);
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->seenGenerations.clear();
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->result.reset();
															#line 213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->totalRepliesReceived = 0;
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->maxAgreement = 0;
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = a_body1loopBody1Catch1cont1(loopDepth);

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
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetGenerationQuorum* self;
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	int retries;
															#line 1289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via getGenerationActor()
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetGenerationActorActor final : public Actor<ConfigGeneration>, public ActorCallback< GetGenerationActorActor, 0, ConfigGeneration >, public ActorCallback< GetGenerationActorActor, 1, Void >, public ActorCallback< GetGenerationActorActor, 2, Void >, public FastAllocated<GetGenerationActorActor>, public GetGenerationActorActorState<GetGenerationActorActor> {
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetGenerationActorActor(GetGenerationQuorum* const& self) 
															#line 1307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
		}

	}
};
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<ConfigGeneration> getGenerationActor( GetGenerationQuorum* const& self ) {
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<ConfigGeneration>(new GetGenerationActorActor(self));
															#line 1335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

public:
	GetGenerationQuorum() = default;
	explicit GetGenerationQuorum(std::vector<ConfigTransactionInterface> const& ctis,
	                             Optional<Version> const& lastSeenLiveVersion = {})
	  : ctis(ctis), lastSeenLiveVersion(lastSeenLiveVersion) {}
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
	std::vector<ConfigTransactionInterface> ctis;
	GetGenerationQuorum getGenerationQuorum;
	CommitQuorum commitQuorum;
	int numRetries{ 0 };
	Optional<UID> dID;
	Database cx;

																#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via get()
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class GetActor>
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetActorState {
															#line 1378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetActorState(PaxosConfigTransactionImpl* const& self,Key const& key) 
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self),
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   key(key),
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   configKey(ConfigKey::decodeKey(key))
															#line 1389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			;
															#line 1404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			StrictFuture<ConfigGeneration> __when_expr_0 = self->getGenerationQuorum.getGeneration();
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (static_cast<GetActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetActor*>(this)->actor_wait_state = 1;
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetActor, 0, ConfigGeneration >*>(static_cast<GetActor*>(this)));
															#line 1442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (e.code() != error_code_timed_out && e.code() != error_code_broken_promise)
															#line 1464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 282 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 1468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			self->reset();
															#line 1472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		readReplicas = self->getGenerationQuorum.getReadReplicas();
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		std::vector<Future<Void>> fs;
															#line 262 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( ConfigTransactionInterface& readReplica : readReplicas ) {
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (readReplica.hostname.present())
															#line 1493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				fs.push_back(tryInitializeRequestStream( &readReplica.get, readReplica.hostname.get(), WLTOKEN_CONFIGTXN_GET));
															#line 1497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
		}
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<Void> __when_expr_1 = waitForAll(fs);
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetActor*>(this)->actor_wait_state = 2;
															#line 268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetActor, 1, Void >*>(static_cast<GetActor*>(this)));
															#line 1509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(ConfigGeneration const& __generation,int loopDepth) 
	{
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		generation = __generation;
															#line 1518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		configNodes = Reference<ConfigTransactionInfo>(new ConfigTransactionInfo(readReplicas));
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<ConfigTransactionGetReply> __when_expr_2 = timeoutError(basicLoadBalance(configNodes, &ConfigTransactionInterface::get, ConfigTransactionGetRequest{ generation, configKey }), CLIENT_KNOBS->GET_KNOB_TIMEOUT);
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetActor*>(this)->actor_wait_state = 3;
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetActor, 2, ConfigTransactionGetReply >*>(static_cast<GetActor*>(this)));
															#line 1594 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		configNodes = Reference<ConfigTransactionInfo>(new ConfigTransactionInfo(readReplicas));
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<ConfigTransactionGetReply> __when_expr_2 = timeoutError(basicLoadBalance(configNodes, &ConfigTransactionInterface::get, ConfigTransactionGetRequest{ generation, configKey }), CLIENT_KNOBS->GET_KNOB_TIMEOUT);
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetActor*>(this)->actor_wait_state = 3;
															#line 270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetActor, 2, ConfigTransactionGetReply >*>(static_cast<GetActor*>(this)));
															#line 1612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (reply.value.present())
															#line 1684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		{
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (!static_cast<GetActor*>(this)->SAV<Optional<Value>>::futures) { (void)(reply.value.get().toValue()); this->~GetActorState(); static_cast<GetActor*>(this)->destroy(); return 0; }
															#line 1688 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			new (&static_cast<GetActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(reply.value.get().toValue());
			this->~GetActorState();
			static_cast<GetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (!static_cast<GetActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>{}); this->~GetActorState(); static_cast<GetActor*>(this)->destroy(); return 0; }
															#line 1698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			new (&static_cast<GetActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(Optional<Value>{});
			this->~GetActorState();
			static_cast<GetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}

		return loopDepth;
	}
	int a_body1loopBody1cont6(ConfigTransactionGetReply && reply,int loopDepth) 
	{
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (reply.value.present())
															#line 1711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		{
															#line 276 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (!static_cast<GetActor*>(this)->SAV<Optional<Value>>::futures) { (void)(reply.value.get().toValue()); this->~GetActorState(); static_cast<GetActor*>(this)->destroy(); return 0; }
															#line 1715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			new (&static_cast<GetActor*>(this)->SAV< Optional<Value> >::value()) Optional<Value>(reply.value.get().toValue());
			this->~GetActorState();
			static_cast<GetActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (!static_cast<GetActor*>(this)->SAV<Optional<Value>>::futures) { (void)(Optional<Value>{}); this->~GetActorState(); static_cast<GetActor*>(this)->destroy(); return 0; }
															#line 1725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	PaxosConfigTransactionImpl* self;
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	Key key;
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigKey configKey;
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigGeneration generation;
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	std::vector<ConfigTransactionInterface> readReplicas;
															#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	Reference<ConfigTransactionInfo> configNodes;
															#line 1809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via get()
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetActor final : public Actor<Optional<Value>>, public ActorCallback< GetActor, 0, ConfigGeneration >, public ActorCallback< GetActor, 1, Void >, public ActorCallback< GetActor, 2, ConfigTransactionGetReply >, public FastAllocated<GetActor>, public GetActorState<GetActor> {
															#line 1814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetActor(PaxosConfigTransactionImpl* const& self,Key const& key) 
															#line 1827 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<Optional<Value>> get( PaxosConfigTransactionImpl* const& self, Key const& key ) {
															#line 254 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<Optional<Value>>(new GetActor(self, key));
															#line 1856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

																#line 1861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via getConfigClasses()
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class GetConfigClassesActor>
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetConfigClassesActorState {
															#line 1867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetConfigClassesActorState(PaxosConfigTransactionImpl* const& self) 
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self)
															#line 1874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			StrictFuture<ConfigGeneration> __when_expr_0 = self->getGenerationQuorum.getGeneration();
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 1;
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetConfigClassesActor, 0, ConfigGeneration >*>(static_cast<GetConfigClassesActor*>(this)));
															#line 1896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
	int a_body1cont1(int loopDepth) 
	{
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		readReplicas = self->getGenerationQuorum.getReadReplicas();
															#line 292 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		std::vector<Future<Void>> fs;
															#line 293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( ConfigTransactionInterface& readReplica : readReplicas ) {
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (readReplica.hostname.present())
															#line 1925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				fs.push_back(tryInitializeRequestStream( &readReplica.getClasses, readReplica.hostname.get(), WLTOKEN_CONFIGTXN_GETCLASSES));
															#line 1929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
		}
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<Void> __when_expr_1 = waitForAll(fs);
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 2;
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetConfigClassesActor, 1, Void >*>(static_cast<GetConfigClassesActor*>(this)));
															#line 1941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(ConfigGeneration const& __generation,int loopDepth) 
	{
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		generation = __generation;
															#line 1950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(ConfigGeneration && __generation,int loopDepth) 
	{
		generation = std::move(__generation);
		loopDepth = a_body1cont1(loopDepth);

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
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		configNodes = Reference<ConfigTransactionInfo>(new ConfigTransactionInfo(readReplicas));
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<ConfigTransactionGetConfigClassesReply> __when_expr_2 = basicLoadBalance(configNodes, &ConfigTransactionInterface::getClasses, ConfigTransactionGetConfigClassesRequest{ generation });
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2021 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 3;
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetConfigClassesActor, 2, ConfigTransactionGetConfigClassesReply >*>(static_cast<GetConfigClassesActor*>(this)));
															#line 2026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		configNodes = Reference<ConfigTransactionInfo>(new ConfigTransactionInfo(readReplicas));
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<ConfigTransactionGetConfigClassesReply> __when_expr_2 = basicLoadBalance(configNodes, &ConfigTransactionInterface::getClasses, ConfigTransactionGetConfigClassesRequest{ generation });
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetConfigClassesActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetConfigClassesActor*>(this)->actor_wait_state = 3;
															#line 301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetConfigClassesActor, 2, ConfigTransactionGetConfigClassesReply >*>(static_cast<GetConfigClassesActor*>(this)));
															#line 2044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
	int a_body1cont5(ConfigTransactionGetConfigClassesReply const& reply,int loopDepth) 
	{
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		RangeResult result;
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		result.reserve(result.arena(), reply.configClasses.size());
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( const auto& configClass : reply.configClasses ) {
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(configClass, ""_sr));
															#line 2122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		}
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<GetConfigClassesActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetConfigClassesActorState(); static_cast<GetConfigClassesActor*>(this)->destroy(); return 0; }
															#line 2126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<GetConfigClassesActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetConfigClassesActorState();
		static_cast<GetConfigClassesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5(ConfigTransactionGetConfigClassesReply && reply,int loopDepth) 
	{
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		RangeResult result;
															#line 306 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		result.reserve(result.arena(), reply.configClasses.size());
															#line 307 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( const auto& configClass : reply.configClasses ) {
															#line 308 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(configClass, ""_sr));
															#line 2144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		}
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<GetConfigClassesActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetConfigClassesActorState(); static_cast<GetConfigClassesActor*>(this)->destroy(); return 0; }
															#line 2148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<GetConfigClassesActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetConfigClassesActorState();
		static_cast<GetConfigClassesActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(ConfigTransactionGetConfigClassesReply const& reply,int loopDepth) 
	{
		loopDepth = a_body1cont5(reply, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(ConfigTransactionGetConfigClassesReply && reply,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(reply), loopDepth);

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
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetConfigClassesActor, 2, ConfigTransactionGetConfigClassesReply >*,ConfigTransactionGetConfigClassesReply && value) 
	{
		fdb_probe_actor_enter("getConfigClasses", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getConfigClasses", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetConfigClassesActor, 2, ConfigTransactionGetConfigClassesReply >*,Error err) 
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
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	PaxosConfigTransactionImpl* self;
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigGeneration generation;
															#line 291 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	std::vector<ConfigTransactionInterface> readReplicas;
															#line 300 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	Reference<ConfigTransactionInfo> configNodes;
															#line 2227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via getConfigClasses()
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetConfigClassesActor final : public Actor<RangeResult>, public ActorCallback< GetConfigClassesActor, 0, ConfigGeneration >, public ActorCallback< GetConfigClassesActor, 1, Void >, public ActorCallback< GetConfigClassesActor, 2, ConfigTransactionGetConfigClassesReply >, public FastAllocated<GetConfigClassesActor>, public GetConfigClassesActorState<GetConfigClassesActor> {
															#line 2232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetConfigClassesActor(PaxosConfigTransactionImpl* const& self) 
															#line 2245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<RangeResult> getConfigClasses( PaxosConfigTransactionImpl* const& self ) {
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<RangeResult>(new GetConfigClassesActor(self));
															#line 2274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

																#line 2279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via getKnobs()
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class GetKnobsActor>
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetKnobsActorState {
															#line 2285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetKnobsActorState(PaxosConfigTransactionImpl* const& self,Optional<Key> const& configClass) 
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self),
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   configClass(configClass)
															#line 2294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			StrictFuture<ConfigGeneration> __when_expr_0 = self->getGenerationQuorum.getGeneration();
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (static_cast<GetKnobsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetKnobsActor*>(this)->actor_wait_state = 1;
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetKnobsActor, 0, ConfigGeneration >*>(static_cast<GetKnobsActor*>(this)));
															#line 2316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
	int a_body1cont1(int loopDepth) 
	{
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		readReplicas = self->getGenerationQuorum.getReadReplicas();
															#line 316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		std::vector<Future<Void>> fs;
															#line 317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( ConfigTransactionInterface& readReplica : readReplicas ) {
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (readReplica.hostname.present())
															#line 2345 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 319 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				fs.push_back(tryInitializeRequestStream( &readReplica.getKnobs, readReplica.hostname.get(), WLTOKEN_CONFIGTXN_GETKNOBS));
															#line 2349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			}
		}
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<Void> __when_expr_1 = waitForAll(fs);
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetKnobsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<GetKnobsActor*>(this)->actor_wait_state = 2;
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< GetKnobsActor, 1, Void >*>(static_cast<GetKnobsActor*>(this)));
															#line 2361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(ConfigGeneration const& __generation,int loopDepth) 
	{
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		generation = __generation;
															#line 2370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(ConfigGeneration && __generation,int loopDepth) 
	{
		generation = std::move(__generation);
		loopDepth = a_body1cont1(loopDepth);

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
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		configNodes = Reference<ConfigTransactionInfo>(new ConfigTransactionInfo(readReplicas));
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<ConfigTransactionGetKnobsReply> __when_expr_2 = basicLoadBalance(configNodes, &ConfigTransactionInterface::getKnobs, ConfigTransactionGetKnobsRequest{ generation, configClass });
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetKnobsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetKnobsActor*>(this)->actor_wait_state = 3;
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetKnobsActor, 2, ConfigTransactionGetKnobsReply >*>(static_cast<GetKnobsActor*>(this)));
															#line 2446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		configNodes = Reference<ConfigTransactionInfo>(new ConfigTransactionInfo(readReplicas));
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<ConfigTransactionGetKnobsReply> __when_expr_2 = basicLoadBalance(configNodes, &ConfigTransactionInterface::getKnobs, ConfigTransactionGetKnobsRequest{ generation, configClass });
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<GetKnobsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<GetKnobsActor*>(this)->actor_wait_state = 3;
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< GetKnobsActor, 2, ConfigTransactionGetKnobsReply >*>(static_cast<GetKnobsActor*>(this)));
															#line 2464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
	int a_body1cont5(ConfigTransactionGetKnobsReply const& reply,int loopDepth) 
	{
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		RangeResult result;
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		result.reserve(result.arena(), reply.knobNames.size());
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( const auto& knobName : reply.knobNames ) {
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(knobName, ""_sr));
															#line 2542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		}
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<GetKnobsActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetKnobsActorState(); static_cast<GetKnobsActor*>(this)->destroy(); return 0; }
															#line 2546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<GetKnobsActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetKnobsActorState();
		static_cast<GetKnobsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont5(ConfigTransactionGetKnobsReply && reply,int loopDepth) 
	{
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		RangeResult result;
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		result.reserve(result.arena(), reply.knobNames.size());
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		for( const auto& knobName : reply.knobNames ) {
															#line 332 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			result.push_back_deep(result.arena(), KeyValueRef(knobName, ""_sr));
															#line 2564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		}
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<GetKnobsActor*>(this)->SAV<RangeResult>::futures) { (void)(result); this->~GetKnobsActorState(); static_cast<GetKnobsActor*>(this)->destroy(); return 0; }
															#line 2568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<GetKnobsActor*>(this)->SAV< RangeResult >::value()) RangeResult(result);
		this->~GetKnobsActorState();
		static_cast<GetKnobsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(ConfigTransactionGetKnobsReply const& reply,int loopDepth) 
	{
		loopDepth = a_body1cont5(reply, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(ConfigTransactionGetKnobsReply && reply,int loopDepth) 
	{
		loopDepth = a_body1cont5(std::move(reply), loopDepth);

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
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< GetKnobsActor, 2, ConfigTransactionGetKnobsReply >*,ConfigTransactionGetKnobsReply && value) 
	{
		fdb_probe_actor_enter("getKnobs", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getKnobs", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< GetKnobsActor, 2, ConfigTransactionGetKnobsReply >*,Error err) 
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
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	PaxosConfigTransactionImpl* self;
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	Optional<Key> configClass;
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	ConfigGeneration generation;
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	std::vector<ConfigTransactionInterface> readReplicas;
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	Reference<ConfigTransactionInfo> configNodes;
															#line 2649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via getKnobs()
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class GetKnobsActor final : public Actor<RangeResult>, public ActorCallback< GetKnobsActor, 0, ConfigGeneration >, public ActorCallback< GetKnobsActor, 1, Void >, public ActorCallback< GetKnobsActor, 2, ConfigTransactionGetKnobsReply >, public FastAllocated<GetKnobsActor>, public GetKnobsActorState<GetKnobsActor> {
															#line 2654 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	GetKnobsActor(PaxosConfigTransactionImpl* const& self,Optional<Key> const& configClass) 
															#line 2667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<RangeResult> getKnobs( PaxosConfigTransactionImpl* const& self, Optional<Key> const& configClass ) {
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<RangeResult>(new GetKnobsActor(self, configClass));
															#line 2696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

																#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via commit()
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class CommitActor>
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class CommitActorState {
															#line 2707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	CommitActorState(PaxosConfigTransactionImpl* const& self) 
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self)
															#line 2714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			StrictFuture<ConfigGeneration> __when_expr_0 = self->getGenerationQuorum.getGeneration();
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CommitActor*>(this)->actor_wait_state = 1;
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 0, ConfigGeneration >*>(static_cast<CommitActor*>(this)));
															#line 2736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
	int a_body1cont1(ConfigGeneration const& generation,int loopDepth) 
	{
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->commitQuorum.setTimestamp();
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<Void> __when_expr_1 = self->commitQuorum.commit(generation);
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CommitActor*>(this)->actor_wait_state = 2;
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 1, Void >*>(static_cast<CommitActor*>(this)));
															#line 2768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(ConfigGeneration && generation,int loopDepth) 
	{
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->commitQuorum.setTimestamp();
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		StrictFuture<Void> __when_expr_1 = self->commitQuorum.commit(generation);
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (static_cast<CommitActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CommitActor*>(this)->actor_wait_state = 2;
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CommitActor, 1, Void >*>(static_cast<CommitActor*>(this)));
															#line 2786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		loopDepth = 0;

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
		if (static_cast<CommitActor*>(this)->actor_wait_state > 0) static_cast<CommitActor*>(this)->actor_wait_state = 0;
		static_cast<CommitActor*>(this)->ActorCallback< CommitActor, 0, ConfigGeneration >::remove();

	}
	void a_callback_fire(ActorCallback< CommitActor, 0, ConfigGeneration >*,ConfigGeneration const& value) 
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
	void a_callback_fire(ActorCallback< CommitActor, 0, ConfigGeneration >*,ConfigGeneration && value) 
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
	void a_callback_error(ActorCallback< CommitActor, 0, ConfigGeneration >*,Error err) 
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
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<CommitActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CommitActorState(); static_cast<CommitActor*>(this)->destroy(); return 0; }
															#line 2858 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<CommitActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CommitActorState();
		static_cast<CommitActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<CommitActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CommitActorState(); static_cast<CommitActor*>(this)->destroy(); return 0; }
															#line 2870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<CommitActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CommitActorState();
		static_cast<CommitActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	PaxosConfigTransactionImpl* self;
															#line 2943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via commit()
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class CommitActor final : public Actor<Void>, public ActorCallback< CommitActor, 0, ConfigGeneration >, public ActorCallback< CommitActor, 1, Void >, public FastAllocated<CommitActor>, public CommitActorState<CommitActor> {
															#line 2948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<CommitActor>::operator new;
	using FastAllocated<CommitActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CommitActor, 0, ConfigGeneration >;
friend struct ActorCallback< CommitActor, 1, Void >;
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	CommitActor(PaxosConfigTransactionImpl* const& self) 
															#line 2960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<Void> commit( PaxosConfigTransactionImpl* const& self ) {
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<Void>(new CommitActor(self));
															#line 2988 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

																#line 2993 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
// This generated class is to be used only via onError()
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
template <class OnErrorActor>
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class OnErrorActorState {
															#line 2999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	OnErrorActorState(PaxosConfigTransactionImpl* const& self,Error const& e) 
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		 : self(self),
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		   e(e)
															#line 3008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			TraceEvent("ConfigIncrementOnError").error(e).detail("NumRetries", self->numRetries);
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
			if (e.code() == error_code_transaction_too_old || e.code() == error_code_not_committed)
															#line 3025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
			{
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				StrictFuture<Void> __when_expr_0 = delay(std::clamp((1 << self->numRetries++) * 0.01 * deterministicRandom()->random01(), 0.0, CLIENT_KNOBS->TIMEOUT_RETRY_UPPER_BOUND));
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				if (static_cast<OnErrorActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<OnErrorActor*>(this)->actor_wait_state = 1;
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< OnErrorActor, 0, Void >*>(static_cast<OnErrorActor*>(this)));
															#line 3036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		return a_body1Catch1(e, loopDepth);
															#line 3064 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->reset();
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<OnErrorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnErrorActorState(); static_cast<OnErrorActor*>(this)->destroy(); return 0; }
															#line 3074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
		new (&static_cast<OnErrorActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~OnErrorActorState();
		static_cast<OnErrorActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		self->reset();
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
		if (!static_cast<OnErrorActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~OnErrorActorState(); static_cast<OnErrorActor*>(this)->destroy(); return 0; }
															#line 3088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	PaxosConfigTransactionImpl* self;
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	Error e;
															#line 3163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
};
// This generated class is to be used only via onError()
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
class OnErrorActor final : public Actor<Void>, public ActorCallback< OnErrorActor, 0, Void >, public FastAllocated<OnErrorActor>, public OnErrorActorState<OnErrorActor> {
															#line 3168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
public:
	using FastAllocated<OnErrorActor>::operator new;
	using FastAllocated<OnErrorActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< OnErrorActor, 0, Void >;
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	OnErrorActor(PaxosConfigTransactionImpl* const& self,Error const& e) 
															#line 3179 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
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
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
[[nodiscard]] static Future<Void> onError( PaxosConfigTransactionImpl* const& self, Error const& e ) {
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"
	return Future<Void>(new OnErrorActor(self, e));
															#line 3206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.g.cpp"
}

#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/PaxosConfigTransaction.actor.cpp"

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
		getGenerationQuorum = GetGenerationQuorum{ ctis };
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

	PaxosConfigTransactionImpl(Database const& cx) : cx(cx) {
		const ClusterConnectionString& cs = cx->getConnectionRecord()->getConnectionString();
		ctis.reserve(cs.hostnames.size() + cs.coords.size());
		for (const auto& h : cs.hostnames) {
			ctis.emplace_back(h);
		}
		for (const auto& c : cs.coords) {
			ctis.emplace_back(c);
		}
		getGenerationQuorum = GetGenerationQuorum{ ctis };
		commitQuorum = CommitQuorum{ ctis };
	}

	PaxosConfigTransactionImpl(std::vector<ConfigTransactionInterface> const& ctis)
	  : ctis(ctis), getGenerationQuorum(ctis), commitQuorum(ctis) {}
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
