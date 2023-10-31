#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
/*
 * LoadBalance.actor.h
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

// When actually compiled (NO_INTELLISENSE), include the generated version of this file.  In intellisense use the source
// version.
#if defined(NO_INTELLISENSE) && !defined(FLOW_LOADBALANCE_ACTOR_G_H)
#define FLOW_LOADBALANCE_ACTOR_G_H
#include "fdbrpc/LoadBalance.actor.g.h"
#elif !defined(FLOW_LOADBALANCE_ACTOR_H)
#define FLOW_LOADBALANCE_ACTOR_H

#include "flow/BooleanParam.h"
#include "flow/flow.h"
#include "flow/Knobs.h"

#include "fdbrpc/FailureMonitor.h"
#include "fdbrpc/fdbrpc.h"
#include "fdbrpc/Locality.h"
#include "fdbrpc/QueueModel.h"
#include "fdbrpc/MultiInterface.h"
#include "fdbrpc/simulator.h" // for checking tss simulation mode
#include "fdbrpc/TSSComparison.h"
#include "flow/actorcompiler.h" // This must be the last #include.

															#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
[[nodiscard]] Future<Void> allAlternativesFailedDelay( Future<Void> const& okFuture );

#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"

struct ModelHolder : NonCopyable, public ReferenceCounted<ModelHolder> {
	QueueModel* model;
	bool released;
	double startTime;
	double delta;
	uint64_t token;

	ModelHolder(QueueModel* model, uint64_t token) : model(model), released(false), startTime(now()), token(token) {
		if (model) {
			delta = model->addRequest(token);
		}
	}

	void release(bool clean, bool futureVersion, double penalty, bool measureLatency = true) {
		if (model && !released) {
			released = true;
			double latency = (clean || measureLatency) ? now() - startTime : 0.0;
			model->endRequest(token, latency, penalty, delta, clean, futureVersion);
		}
	}

	~ModelHolder() { release(false, false, -1.0, false); }
};

// Subclasses must initialize all members in their default constructors
// Subclasses must serialize all members
struct LoadBalancedReply {
	double penalty;
	Optional<Error> error;
	LoadBalancedReply() : penalty(1.0) {}
};

Optional<LoadBalancedReply> getLoadBalancedReply(const LoadBalancedReply* reply);
Optional<LoadBalancedReply> getLoadBalancedReply(const void*);

															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
namespace {
// This generated class is to be used only via tssComparison()
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
template <class Req, class Resp, class Interface, class Multi, bool P, class TssComparisonActor>
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
class TssComparisonActorState {
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
public:
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	TssComparisonActorState(Req const& req,Future<ErrorOr<Resp>> const& fSource,Future<ErrorOr<Resp>> const& fTss,TSSEndpointData const& tssData,uint64_t const& srcEndpointId,Reference<MultiInterface<Multi>> const& ssTeam,RequestStream<Req, P> Interface::* const& channel) 
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		 : req(req),
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   fSource(fSource),
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   fTss(fTss),
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   tssData(tssData),
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   srcEndpointId(srcEndpointId),
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   ssTeam(ssTeam),
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   channel(channel),
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   startTime(now()),
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   fTssWithTimeout(timeout(fTss, FLOW_KNOBS->LOAD_BALANCE_TSS_TIMEOUT)),
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   finished(0),
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   srcEndTime(),
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   tssEndTime(),
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   srcErrorCode(error_code_success),
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   tssErrorCode(error_code_success),
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   src(),
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   tss()
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
	{
		fdb_probe_actor_create("tssComparison", reinterpret_cast<unsigned long>(this));

	}
	~TssComparisonActorState() 
	{
		fdb_probe_actor_destroy("tssComparison", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			;
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
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
		this->~TssComparisonActorState();
		static_cast<TssComparisonActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		++tssData.metrics->requests;
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (src.isError())
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			srcErrorCode = src.getError().code();
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			tssData.metrics->ssError(srcErrorCode);
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (!tss.present())
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			++tssData.metrics->tssTimeouts;
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
		else
		{
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (tss.get().isError())
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				tssErrorCode = tss.get().getError().code();
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				tssData.metrics->tssError(tssErrorCode);
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			}
		}
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (!src.isError() && tss.present() && !tss.get().isError())
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			Optional<LoadBalancedReply> srcLB = getLoadBalancedReply(&src.get());
															#line 134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			Optional<LoadBalancedReply> tssLB = getLoadBalancedReply(&tss.get().get());
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			ASSERT(srcLB.present() == tssLB.present());
															#line 139 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!srcLB.present() || (!srcLB.get().error.present() && !tssLB.get().error.present()))
															#line 211 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				tssData.metrics->recordLatency(req, srcEndTime - startTime, tssEndTime - startTime);
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				if (!TSS_doCompare(src.get(), tss.get().get()))
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					CODE_PROBE(true, "TSS Mismatch");
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					mismatchEvent = TraceEvent((g_network->isSimulated() && g_simulator->tssMode == ISimulator::TSSMode::EnabledDropMutations) ? SevWarnAlways : SevError, TSS_mismatchTraceName(req));
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					mismatchEvent.setMaxEventLength(FLOW_KNOBS->TSS_LARGE_TRACE_SIZE);
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					mismatchEvent.detail("TSSID", tssData.tssId);
															#line 154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					if (FLOW_KNOBS->LOAD_BALANCE_TSS_MISMATCH_VERIFY_SS && ssTeam->size() > 1)
															#line 229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					{
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						CODE_PROBE(true, "checking TSS mismatch against rest of storage team");
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						restOfTeamFutures = std::vector<Future<ErrorOr<Resp>>>();
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						restOfTeamFutures.reserve(ssTeam->size() - 1);
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						for(int i = 0;i < ssTeam->size();i++) {
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
							RequestStream<Req, P> const* si = &ssTeam->get(i, channel);
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
							if (si->getEndpoint().token.first() != srcEndpointId)
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
							{
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
								resetReply(req);
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
								restOfTeamFutures.push_back(si->tryGetReply(req));
															#line 249 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
							}
						}
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						StrictFuture<Void> __when_expr_2 = waitForAllReady(restOfTeamFutures);
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						if (static_cast<TssComparisonActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
						if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when1(__when_expr_2.get(), loopDepth); };
						static_cast<TssComparisonActor*>(this)->actor_wait_state = 2;
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TssComparisonActor, 2, Void >*>(static_cast<TssComparisonActor*>(this)));
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
						loopDepth = 0;
					}
					else
					{
						loopDepth = a_body1cont9(loopDepth);
					}
				}
				else
				{
					loopDepth = a_body1cont8(loopDepth);
				}
			}
			else
			{
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				if (tssLB.present() && tssLB.get().error.present())
															#line 278 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					tssErrorCode = tssLB.get().error.get().code();
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					tssData.metrics->tssError(tssErrorCode);
															#line 284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				}
				else
				{
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					if (srcLB.present() && srcLB.get().error.present())
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					{
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						srcErrorCode = srcLB.get().error.get().code();
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						tssData.metrics->ssError(srcErrorCode);
															#line 296 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					}
				}
				loopDepth = a_body1cont7(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
		}

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
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		StrictFuture<Void> __when_expr_0 = store(src, fSource);
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (static_cast<TssComparisonActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		StrictFuture<Void> __when_expr_1 = store(tss, fTssWithTimeout);
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<TssComparisonActor*>(this)->actor_wait_state = 1;
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TssComparisonActor, 0, Void >*>(static_cast<TssComparisonActor*>(this)));
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TssComparisonActor, 1, Void >*>(static_cast<TssComparisonActor*>(this)));
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
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
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		srcEndTime = now();
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		fSource = Never();
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		finished++;
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (finished == 2)
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		srcEndTime = now();
															#line 104 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		fSource = Never();
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		finished++;
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (finished == 2)
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		tssEndTime = now();
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		fTssWithTimeout = Never();
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		finished++;
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (finished == 2)
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		tssEndTime = now();
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		fTssWithTimeout = Never();
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		finished++;
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (finished == 2)
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TssComparisonActor*>(this)->actor_wait_state > 0) static_cast<TssComparisonActor*>(this)->actor_wait_state = 0;
		static_cast<TssComparisonActor*>(this)->ActorCallback< TssComparisonActor, 0, Void >::remove();
		static_cast<TssComparisonActor*>(this)->ActorCallback< TssComparisonActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TssComparisonActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TssComparisonActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TssComparisonActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TssComparisonActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TssComparisonActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TssComparisonActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (srcErrorCode != error_code_success && tssErrorCode != error_code_success && srcErrorCode != tssErrorCode)
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			TraceEvent("TSSErrorMismatch") .suppressFor(1.0) .detail("TSSID", tssData.tssId) .detail("SSError", srcErrorCode) .detail("TSSError", tssErrorCode);
															#line 534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (!static_cast<TssComparisonActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TssComparisonActorState(); static_cast<TssComparisonActor*>(this)->destroy(); return 0; }
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		new (&static_cast<TssComparisonActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TssComparisonActorState();
		static_cast<TssComparisonActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont8(int loopDepth) 
	{
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont9(int loopDepth) 
	{
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (tssData.metrics->shouldRecordDetailedMismatch())
															#line 562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 198 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			TSS_traceMismatch(mismatchEvent, req, src.get(), tss.get().get());
															#line 200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			CODE_PROBE(FLOW_KNOBS->LOAD_BALANCE_TSS_MISMATCH_TRACE_FULL, "Tracing Full TSS Mismatch");
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			CODE_PROBE(!FLOW_KNOBS->LOAD_BALANCE_TSS_MISMATCH_TRACE_FULL, "Tracing Partial TSS Mismatch and storing the rest in FDB");
															#line 204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!FLOW_KNOBS->LOAD_BALANCE_TSS_MISMATCH_TRACE_FULL)
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				mismatchEvent.disable();
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				UID mismatchUID = deterministicRandom()->randomUniqueID();
															#line 207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				tssData.metrics->recordDetailedMismatchData(mismatchUID, mismatchEvent.getFields().toString());
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				TraceEvent summaryEvent((g_network->isSimulated() && g_simulator->tssMode == ISimulator::TSSMode::EnabledDropMutations) ? SevWarnAlways : SevError, TSS_mismatchTraceName(req));
															#line 215 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				summaryEvent.detail("TSSID", tssData.tssId).detail("MismatchId", mismatchUID);
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			}
		}
		else
		{
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			mismatchEvent.disable();
															#line 591 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
		loopDepth = a_body1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1cont10(Void const& _,int loopDepth) 
	{
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		int numError = 0;
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		int numMatchSS = 0;
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		int numMatchTSS = 0;
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		int numMatchNeither = 0;
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		for( Future<ErrorOr<Resp>> f : restOfTeamFutures ) {
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!f.canGet() || f.get().isError())
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				numError++;
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			}
			else
			{
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				Optional<LoadBalancedReply> fLB = getLoadBalancedReply(&f.get().get());
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				if (fLB.present() && fLB.get().error.present())
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					numError++;
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				}
				else
				{
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					if (TSS_doCompare(src.get(), f.get().get()))
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					{
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						numMatchSS++;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					}
					else
					{
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						if (TSS_doCompare(tss.get().get(), f.get().get()))
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
						{
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
							numMatchTSS++;
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
						}
						else
						{
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
							numMatchNeither++;
															#line 653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
						}
					}
				}
			}
		}
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		mismatchEvent.detail("TeamCheckErrors", numError) .detail("TeamCheckMatchSS", numMatchSS) .detail("TeamCheckMatchTSS", numMatchTSS) .detail("TeamCheckMatchNeither", numMatchNeither);
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont10(Void && _,int loopDepth) 
	{
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		int numError = 0;
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		int numMatchSS = 0;
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		int numMatchTSS = 0;
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		int numMatchNeither = 0;
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		for( Future<ErrorOr<Resp>> f : restOfTeamFutures ) {
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!f.canGet() || f.get().isError())
															#line 680 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				numError++;
															#line 684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			}
			else
			{
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				Optional<LoadBalancedReply> fLB = getLoadBalancedReply(&f.get().get());
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				if (fLB.present() && fLB.get().error.present())
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					numError++;
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				}
				else
				{
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					if (TSS_doCompare(src.get(), f.get().get()))
															#line 702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					{
															#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						numMatchSS++;
															#line 706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					}
					else
					{
															#line 185 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						if (TSS_doCompare(tss.get().get(), f.get().get()))
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
						{
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
							numMatchTSS++;
															#line 716 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
						}
						else
						{
															#line 188 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
							numMatchNeither++;
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
						}
					}
				}
			}
		}
															#line 192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		mismatchEvent.detail("TeamCheckErrors", numError) .detail("TeamCheckMatchSS", numMatchSS) .detail("TeamCheckMatchTSS", numMatchTSS) .detail("TeamCheckMatchNeither", numMatchNeither);
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<TssComparisonActor*>(this)->actor_wait_state > 0) static_cast<TssComparisonActor*>(this)->actor_wait_state = 0;
		static_cast<TssComparisonActor*>(this)->ActorCallback< TssComparisonActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TssComparisonActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< TssComparisonActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< TssComparisonActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	Req req;
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	Future<ErrorOr<Resp>> fSource;
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	Future<ErrorOr<Resp>> fTss;
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	TSSEndpointData tssData;
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	uint64_t srcEndpointId;
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	Reference<MultiInterface<Multi>> ssTeam;
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	RequestStream<Req, P> Interface::* channel;
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	double startTime;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	Future<Optional<ErrorOr<Resp>>> fTssWithTimeout;
															#line 91 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	int finished;
															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	double srcEndTime;
															#line 93 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	double tssEndTime;
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	int srcErrorCode;
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	int tssErrorCode;
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	ErrorOr<Resp> src;
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	Optional<ErrorOr<Resp>> tss;
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	TraceEvent mismatchEvent;
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	std::vector<Future<ErrorOr<Resp>>> restOfTeamFutures;
															#line 834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
};
// This generated class is to be used only via tssComparison()
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
template <class Req, class Resp, class Interface, class Multi, bool P>
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
class TssComparisonActor final : public Actor<Void>, public ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi, P>, 0, Void >, public ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi, P>, 1, Void >, public ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi, P>, 2, Void >, public FastAllocated<TssComparisonActor<Req, Resp, Interface, Multi, P>>, public TssComparisonActorState<Req, Resp, Interface, Multi, P, TssComparisonActor<Req, Resp, Interface, Multi, P>> {
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
public:
	using FastAllocated<TssComparisonActor<Req, Resp, Interface, Multi, P>>::operator new;
	using FastAllocated<TssComparisonActor<Req, Resp, Interface, Multi, P>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi, P>, 0, Void >;
friend struct ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi, P>, 1, Void >;
friend struct ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi, P>, 2, Void >;
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	TssComparisonActor(Req const& req,Future<ErrorOr<Resp>> const& fSource,Future<ErrorOr<Resp>> const& fTss,TSSEndpointData const& tssData,uint64_t const& srcEndpointId,Reference<MultiInterface<Multi>> const& ssTeam,RequestStream<Req, P> Interface::* const& channel) 
															#line 854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		 : Actor<Void>(),
		   TssComparisonActorState<Req, Resp, Interface, Multi, P, TssComparisonActor<Req, Resp, Interface, Multi, P>>(req, fSource, fTss, tssData, srcEndpointId, ssTeam, channel)
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tssComparison");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi, P>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi, P>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
template <class Req, class Resp, class Interface, class Multi, bool P>
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
[[nodiscard]] Future<Void> tssComparison( Req const& req, Future<ErrorOr<Resp>> const& fSource, Future<ErrorOr<Resp>> const& fTss, TSSEndpointData const& tssData, uint64_t const& srcEndpointId, Reference<MultiInterface<Multi>> const& ssTeam, RequestStream<Req, P> Interface::* const& channel ) {
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	return Future<Void>(new TssComparisonActor<Req, Resp, Interface, Multi, P>(req, fSource, fTss, tssData, srcEndpointId, ssTeam, channel));
															#line 885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
}

#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"

FDB_BOOLEAN_PARAM(AtMostOnce);
FDB_BOOLEAN_PARAM(TriedAllOptions);

// Stores state for a request made by the load balancer
template <class Request, class Interface, class Multi, bool P>
struct RequestData : NonCopyable {
	typedef ErrorOr<REPLY_TYPE(Request)> Reply;

	Future<Reply> response;
	Reference<ModelHolder> modelHolder;
	TriedAllOptions triedAllOptions{ false };

	bool requestStarted = false; // true once the request has been sent to an alternative
	bool requestProcessed = false; // true once a response has been received and handled by checkAndProcessResult

	// Whether or not the response future is valid
	// This is true once setupRequest is called, even though at that point the response is Never().
	bool isValid() { return response.isValid(); }

	static void maybeDuplicateTSSRequest(RequestStream<Request, P> const* stream,
	                                     Request& request,
	                                     QueueModel* model,
	                                     Future<Reply> ssResponse,
	                                     Reference<MultiInterface<Multi>> alternatives,
	                                     RequestStream<Request, P> Interface::*channel) {
		if (model) {
			// Send parallel request to TSS pair, if it exists
			Optional<TSSEndpointData> tssData = model->getTssData(stream->getEndpoint().token.first());

			if (tssData.present()) {
				CODE_PROBE(true, "duplicating request to TSS");
				resetReply(request);
				// FIXME: optimize to avoid creating new netNotifiedQueue for each message
				RequestStream<Request, P> tssRequestStream(tssData.get().endpoint);
				Future<ErrorOr<REPLY_TYPE(Request)>> fTssResult = tssRequestStream.tryGetReply(request);
				model->addActor.send(tssComparison(request,
				                                   ssResponse,
				                                   fTssResult,
				                                   tssData.get(),
				                                   stream->getEndpoint().token.first(),
				                                   alternatives,
				                                   channel));
			}
		}
	}

	// Initializes the request state and starts it, possibly after a backoff delay
	void startRequest(
	    double backoff,
	    TriedAllOptions triedAllOptions,
	    RequestStream<Request, P> const* stream,
	    Request& request,
	    QueueModel* model,
	    Reference<MultiInterface<Multi>> alternatives, // alternatives and channel passed through for TSS check
	    RequestStream<Request, P> Interface::*channel) {
		modelHolder = Reference<ModelHolder>();
		requestStarted = false;

		if (backoff > 0) {
			response = mapAsync(delay(backoff), [this, stream, &request, model, alternatives, channel](Void _) {
				requestStarted = true;
				modelHolder = Reference<ModelHolder>(new ModelHolder(model, stream->getEndpoint().token.first()));
				Future<Reply> resp = stream->tryGetReply(request);
				maybeDuplicateTSSRequest(stream, request, model, resp, alternatives, channel);
				return resp;
			});
		} else {
			requestStarted = true;
			modelHolder = Reference<ModelHolder>(new ModelHolder(model, stream->getEndpoint().token.first()));
			response = stream->tryGetReply(request);
			maybeDuplicateTSSRequest(stream, request, model, response, alternatives, channel);
		}

		requestProcessed = false;
		this->triedAllOptions = triedAllOptions;
	}

	// Implementation of the logic to handle a response.
	// Checks the state of the response, updates the queue model, and returns one of the following outcomes:
	// A return value of true means that the request completed successfully
	// A return value of false means that the request failed but should be retried
	// A return value with an error means that the error should be thrown back to original caller
	static ErrorOr<bool> checkAndProcessResultImpl(Reply const& result,
	                                               Reference<ModelHolder> modelHolder,
	                                               AtMostOnce atMostOnce,
	                                               TriedAllOptions triedAllOptions) {
		ASSERT(modelHolder);

		Optional<LoadBalancedReply> loadBalancedReply;
		if (!result.isError()) {
			loadBalancedReply = getLoadBalancedReply(&result.get());
		}

		int errCode;
		if (loadBalancedReply.present()) {
			errCode = loadBalancedReply.get().error.present() ? loadBalancedReply.get().error.get().code()
			                                                  : error_code_success;
		} else {
			errCode = result.isError() ? result.getError().code() : error_code_success;
		}

		bool maybeDelivered = errCode == error_code_broken_promise || errCode == error_code_request_maybe_delivered;
		bool receivedResponse =
		    loadBalancedReply.present() ? !loadBalancedReply.get().error.present() : result.present();
		receivedResponse = receivedResponse || (!maybeDelivered && errCode != error_code_process_behind);
		bool futureVersion = errCode == error_code_future_version || errCode == error_code_process_behind;

		modelHolder->release(
		    receivedResponse, futureVersion, loadBalancedReply.present() ? loadBalancedReply.get().penalty : -1.0);

		if (errCode == error_code_server_overloaded) {
			return false;
		}

		if (loadBalancedReply.present() && !loadBalancedReply.get().error.present()) {
			return true;
		}

		if (!loadBalancedReply.present() && result.present()) {
			return true;
		}

		if (receivedResponse) {
			return loadBalancedReply.present() ? loadBalancedReply.get().error.get() : result.getError();
		}

		if (atMostOnce && maybeDelivered) {
			return request_maybe_delivered();
		}

		if (triedAllOptions && errCode == error_code_process_behind) {
			return process_behind();
		}

		return false;
	}

	// Checks the state of the response, updates the queue model, and returns one of the following outcomes:
	// A return value of true means that the request completed successfully
	// A return value of false means that the request failed but should be retried
	// In the event of a non-retryable failure, an error is thrown indicating the failure
	bool checkAndProcessResult(AtMostOnce atMostOnce) {
		ASSERT(response.isReady());
		requestProcessed = true;

		ErrorOr<bool> outcome =
		    checkAndProcessResultImpl(response.get(), std::move(modelHolder), atMostOnce, triedAllOptions);

		if (outcome.isError()) {
			throw outcome.getError();
		} else if (!outcome.get()) {
			response = Future<Reply>();
		}

		return outcome.get();
	}

	// Convert this request to a lagging request. Such a request is no longer being waited on, but it still needs to be
	// processed so we can update the queue model.
	void makeLaggingRequest() {
		ASSERT(response.isValid());
		ASSERT(!response.isReady());
		ASSERT(modelHolder);
		ASSERT(modelHolder->model);

		QueueModel* model = modelHolder->model;
		if (model->laggingRequestCount > FLOW_KNOBS->MAX_LAGGING_REQUESTS_OUTSTANDING ||
		    model->laggingRequests.isReady()) {
			model->laggingRequests.cancel();
			model->laggingRequestCount = 0;
			model->addActor = PromiseStream<Future<Void>>();
			model->laggingRequests = actorCollection(model->addActor.getFuture(), &model->laggingRequestCount);
		}

		// We need to process the lagging request in order to update the queue model
		Reference<ModelHolder> holderCapture = std::move(modelHolder);
		auto triedAllOptionsCapture = triedAllOptions;
		Future<Void> updateModel = map(response, [holderCapture, triedAllOptionsCapture](Reply result) {
			checkAndProcessResultImpl(result, holderCapture, AtMostOnce::False, triedAllOptionsCapture);
			return Void();
		});
		model->addActor.send(updateModel);
	}

	~RequestData() {
		// If the request has been started but hasn't completed, mark it as a lagging request
		if (requestStarted && !requestProcessed && modelHolder && modelHolder->model) {
			makeLaggingRequest();
		}
	}
};

// Try to get a reply from one of the alternatives until success, cancellation, or certain errors.
// Load balancing has a budget to race requests to a second alternative if the first request is slow.
// Tries to take into account failMon's information for load balancing and avoiding failed servers.
// If ALL the servers are failed and the list of servers is not fresh, throws an exception to let the caller refresh the
// list of servers.
// When model is set, load balance among alternatives in the same DC aims to balance request queue length on these
// interfaces. If too many interfaces in the same DC are bad, try remote interfaces.
															#line 1089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
namespace {
// This generated class is to be used only via loadBalance()
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
template <class Interface, class Request, class Multi, bool P, class LoadBalanceActor>
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
class LoadBalanceActorState {
															#line 1096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
public:
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	LoadBalanceActorState(Reference<MultiInterface<Multi>> const& alternatives,RequestStream<Request, P> Interface::* const& channel,Request const& request = Request(),TaskPriority const& taskID = TaskPriority::DefaultPromiseEndpoint,AtMostOnce const& atMostOnce = AtMostOnce::False,QueueModel* const& model = nullptr) 
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		 : alternatives(alternatives),
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   channel(channel),
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   request(request),
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   taskID(taskID),
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   atMostOnce(atMostOnce),
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   model(model),
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   firstRequestData(),
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   secondRequestData(),
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   firstRequestEndpoint(),
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   secondDelay(Never()),
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   requestFinished(),
															#line 459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   startTime(now()),
															#line 461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   triedAllOptions(TriedAllOptions::False)
															#line 1127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
	{
		fdb_probe_actor_create("loadBalance", reinterpret_cast<unsigned long>(this));

	}
	~LoadBalanceActorState() 
	{
		fdb_probe_actor_destroy("loadBalance", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			setReplyPriority(request, taskID);
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!alternatives)
															#line 1144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				this->~LoadBalanceActorState();
															#line 1148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				static_cast<LoadBalanceActor*>(this)->sendAndDelPromiseRef(Never());
				return 0;
			}
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			ASSERT(alternatives->size());
															#line 469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			bestAlt = deterministicRandom()->randomInt(0, alternatives->countBest());
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			nextAlt = deterministicRandom()->randomInt(0, std::max(alternatives->size() - 1, 1));
															#line 471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (nextAlt >= bestAlt)
															#line 1160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				nextAlt++;
															#line 1164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			}
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (model)
															#line 1168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 475 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				double bestMetric = 1e9;
															#line 476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				double nextMetric = 1e9;
															#line 477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				double bestTime = 1e9;
															#line 478 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				double nextTime = 1e9;
															#line 479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				int badServers = 0;
															#line 481 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				for(int i = 0;i < alternatives->size();i++) {
															#line 484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					if (badServers < std::min(i, FLOW_KNOBS->LOAD_BALANCE_MAX_BAD_OPTIONS + 1) && i == alternatives->countBest())
															#line 1184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					{
						break;
					}
					else
					{
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						if (badServers == alternatives->countBest() && i == badServers)
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
						{
															#line 491 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
							TraceEvent("AllLocalAlternativesFailed") .suppressFor(1.0) .detail("Alternatives", alternatives->description()) .detail("Total", alternatives->size()) .detail("Best", alternatives->countBest());
															#line 1196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
						}
					}
															#line 498 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					RequestStream<Request, P> const* thisStream = &alternatives->get(i, channel);
															#line 499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					if (!IFailureMonitor::failureMonitor().getState(thisStream->getEndpoint()).failed)
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					{
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						auto const& qd = model->getMeasurement(thisStream->getEndpoint().token.first());
															#line 501 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						if (now() > qd.failedUntil)
															#line 1209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
						{
															#line 502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
							double thisMetric = qd.smoothOutstanding.smoothTotal();
															#line 503 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
							double thisTime = qd.latency;
															#line 504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
							if (FLOW_KNOBS->LOAD_BALANCE_PENALTY_IS_BAD && qd.penalty > 1.001)
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
							{
															#line 508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
								++badServers;
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
							}
															#line 511 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
							if (thisMetric < bestMetric)
															#line 1225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
							{
															#line 512 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
								if (i != bestAlt)
															#line 1229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
								{
															#line 513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
									nextAlt = bestAlt;
															#line 514 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
									nextMetric = bestMetric;
															#line 515 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
									nextTime = bestTime;
															#line 1237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
								}
															#line 517 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
								bestAlt = i;
															#line 518 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
								bestMetric = thisMetric;
															#line 519 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
								bestTime = thisTime;
															#line 1245 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
							}
							else
							{
															#line 520 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
								if (thisMetric < nextMetric)
															#line 1251 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
								{
															#line 521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
									nextAlt = i;
															#line 522 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
									nextMetric = thisMetric;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
									nextTime = thisTime;
															#line 1259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
								}
							}
						}
						else
						{
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
							++badServers;
															#line 1267 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
						}
					}
					else
					{
															#line 529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						++badServers;
															#line 1274 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					}
				}
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				if (nextMetric > 1e8)
															#line 1279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					for(int i = alternatives->countBest();i < alternatives->size();i++) {
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						RequestStream<Request, P> const* thisStream = &alternatives->get(i, channel);
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						if (!IFailureMonitor::failureMonitor().getState(thisStream->getEndpoint()).failed)
															#line 1287 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
						{
															#line 539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
							auto const& qd = model->getMeasurement(thisStream->getEndpoint().token.first());
															#line 540 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
							if (now() > qd.failedUntil)
															#line 1293 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
							{
															#line 541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
								double thisMetric = qd.smoothOutstanding.smoothTotal();
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
								double thisTime = qd.latency;
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
								if (thisMetric < nextMetric)
															#line 1301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
								{
															#line 545 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
									nextAlt = i;
															#line 546 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
									nextMetric = thisMetric;
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
									nextTime = thisTime;
															#line 1309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
								}
							}
						}
					}
				}
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				if (nextTime < 1e9)
															#line 1317 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					if (bestTime > FLOW_KNOBS->INSTANT_SECOND_REQUEST_MULTIPLIER * (model->secondMultiplier * (nextTime) + FLOW_KNOBS->BASE_SECOND_REQUEST_TIME))
															#line 1321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					{
															#line 558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						secondDelay = Void();
															#line 1325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					}
					else
					{
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						secondDelay = delay(model->secondMultiplier * nextTime + FLOW_KNOBS->BASE_SECOND_REQUEST_TIME);
															#line 1331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					}
				}
				else
				{
															#line 563 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					secondDelay = Never();
															#line 1338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				}
			}
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			startAlt = nextAlt;
															#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			startDistance = (bestAlt + alternatives->size() - startAlt) % alternatives->size();
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			numAttempts = 0;
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			backoff = 0;
															#line 573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			;
															#line 1351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
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
		this->~LoadBalanceActorState();
		static_cast<LoadBalanceActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (now() - startTime > (g_network->isSimulated() ? 30.0 : 600.0))
															#line 1381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			TraceEvent ev(g_network->isSimulated() ? SevWarn : SevWarnAlways, "LoadBalanceTooLong");
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			ev.suppressFor(1.0);
															#line 577 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			ev.detail("Duration", now() - startTime);
															#line 578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			ev.detail("NumAttempts", numAttempts);
															#line 579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			ev.detail("Backoff", backoff);
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			ev.detail("TriedAllOptions", triedAllOptions);
															#line 581 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (ev.isEnabled())
															#line 1397 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				ev.log();
															#line 583 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				for(int alternativeNum = 0;alternativeNum < alternatives->size();alternativeNum++) {
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					RequestStream<Request, P> const* thisStream = &alternatives->get(alternativeNum, channel);
															#line 585 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					TraceEvent(SevWarn, "LoadBalanceTooLongEndpoint") .detail("Addr", thisStream->getEndpoint().getPrimaryAddress()) .detail("Token", thisStream->getEndpoint().token) .detail("Failed", IFailureMonitor::failureMonitor().getState(thisStream->getEndpoint()).failed);
															#line 1407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				}
			}
		}
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		stream = nullptr;
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		distance = LBDistance::Type();
															#line 598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		for(int alternativeNum = 0;alternativeNum < alternatives->size();alternativeNum++) {
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			int useAlt = nextAlt;
															#line 600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (nextAlt == startAlt)
															#line 1421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 601 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				useAlt = bestAlt;
															#line 1425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			}
			else
			{
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				if ((nextAlt + alternatives->size() - startAlt) % alternatives->size() <= startDistance)
															#line 1431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					useAlt = (nextAlt + alternatives->size() - 1) % alternatives->size();
															#line 1435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				}
			}
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			stream = &alternatives->get(useAlt, channel);
															#line 606 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			distance = alternatives->getDistance(useAlt);
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!IFailureMonitor::failureMonitor().getState(stream->getEndpoint()).failed && (!firstRequestEndpoint.present() || stream->getEndpoint().token.first() != firstRequestEndpoint.get()))
															#line 1444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
				break;
			}
															#line 610 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			nextAlt = (nextAlt + 1) % alternatives->size();
															#line 611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (nextAlt == startAlt)
															#line 1452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				triedAllOptions = TriedAllOptions::True;
															#line 1456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			}
															#line 613 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			stream = nullptr;
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			distance = LBDistance::DISTANT;
															#line 1462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (!stream && !firstRequestData.isValid())
															#line 1466 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			std::vector<Future<Void>> ok(alternatives->size());
															#line 621 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			for(int i = 0;i < ok.size();i++) {
															#line 622 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				ok[i] = IFailureMonitor::failureMonitor().onStateEqual(alternatives->get(i, channel).getEndpoint(), FailureStatus(false));
															#line 1474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			}
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			Future<Void> okFuture = quorum(ok, 1);
															#line 628 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!alternatives->alwaysFresh())
															#line 1480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				if (now() - g_network->networkInfo.newestAlternativesFailure > 1 || deterministicRandom()->random01() < 0.01)
															#line 1484 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					TraceEvent("AllAlternativesFailed").detail("Alternatives", alternatives->description());
															#line 1488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				}
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				StrictFuture<Void> __when_expr_0 = allAlternativesFailedDelay(okFuture);
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				if (static_cast<LoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1494 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<LoadBalanceActor*>(this)->actor_wait_state = 1;
															#line 634 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< LoadBalanceActor, 0, Void >*>(static_cast<LoadBalanceActor*>(this)));
															#line 1499 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				loopDepth = 0;
			}
			else
			{
															#line 636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				StrictFuture<Void> __when_expr_1 = okFuture;
															#line 636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				if (static_cast<LoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1508 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
				static_cast<LoadBalanceActor*>(this)->actor_wait_state = 2;
															#line 636 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< LoadBalanceActor, 1, Void >*>(static_cast<LoadBalanceActor*>(this)));
															#line 1513 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				loopDepth = 0;
			}
		}
		else
		{
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!stream)
															#line 1521 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				StrictFuture<ErrorOr<REPLY_TYPE(Request)>> __when_expr_2 = firstRequestData.response;
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				if (static_cast<LoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1527 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when3(__when_expr_2.get(), loopDepth); };
				static_cast<LoadBalanceActor*>(this)->actor_wait_state = 3;
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< LoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*>(static_cast<LoadBalanceActor*>(this)));
															#line 1532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				loopDepth = 0;
			}
			else
			{
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				if (firstRequestData.isValid())
															#line 1539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 650 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					if (distance == LBDistance::DISTANT)
															#line 1543 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					{
															#line 651 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
						TraceEvent("LBDistant2nd") .suppressFor(0.1) .detail("Distance", (int)distance) .detail("BackOff", backoff) .detail("TriedAllOptions", triedAllOptions) .detail("Alternatives", alternatives->description()) .detail("Token", stream->getEndpoint().token) .detail("Total", alternatives->size()) .detail("Best", alternatives->countBest()) .detail("Attempts", numAttempts);
															#line 1547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					}
															#line 662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					secondRequestData.startRequest(backoff, triedAllOptions, stream, request, model, alternatives, channel);
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					;
															#line 1553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					loopDepth = a_body1loopBody1loopHead1(loopDepth);
				}
				else
				{
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					firstRequestData.startRequest(backoff, triedAllOptions, stream, request, model, alternatives, channel);
															#line 690 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					firstRequestEndpoint = stream->getEndpoint().token.first();
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					;
															#line 1564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
					loopDepth = a_body1loopBody1loopHead2(loopDepth);
				}
			}
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		nextAlt = (nextAlt + 1) % alternatives->size();
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (nextAlt == startAlt)
															#line 1578 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			triedAllOptions = TriedAllOptions::True;
															#line 1582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 731 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		resetReply(request, taskID);
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		secondDelay = Never();
															#line 1588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont11(int loopDepth) 
	{
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		numAttempts = 0;
															#line 1597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont13(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont13(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont13(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont13(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<LoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<LoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<LoadBalanceActor*>(this)->ActorCallback< LoadBalanceActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< LoadBalanceActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont15(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont15(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont15(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont15(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<LoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<LoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<LoadBalanceActor*>(this)->ActorCallback< LoadBalanceActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< LoadBalanceActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont16(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont17(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (firstRequestData.checkAndProcessResult(atMostOnce))
															#line 1762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 1766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		firstRequestEndpoint = Optional<uint64_t>();
															#line 1774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1loopBody1cont16(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont17(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (firstRequestData.checkAndProcessResult(atMostOnce))
															#line 1783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 644 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 1787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		firstRequestEndpoint = Optional<uint64_t>();
															#line 1795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1loopBody1cont16(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when3(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont17(result, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when3(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont17(std::move(result), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<LoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<LoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<LoadBalanceActor*>(this)->ActorCallback< LoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >::remove();

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> const& value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1when3(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> && value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1when3(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< LoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*,Error err) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont18(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont16(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont19(int loopDepth) 
	{
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (++numAttempts >= alternatives->size())
															#line 1873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			backoff = std::min( FLOW_KNOBS->LOAD_BALANCE_MAX_BACKOFF, std::max(FLOW_KNOBS->LOAD_BALANCE_START_BACKOFF, backoff * FLOW_KNOBS->LOAD_BALANCE_BACKOFF_RATE));
															#line 1877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont18(loopDepth);

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
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Request)>> __when_expr_3 = firstRequestData.response.isValid() ? firstRequestData.response : Never();
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (static_cast<LoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 1896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1when1(__when_expr_3.get(), loopDepth); };
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Request)>> __when_expr_4 = secondRequestData.response;
															#line 1900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1when2(__when_expr_4.get(), loopDepth); };
		static_cast<LoadBalanceActor*>(this)->actor_wait_state = 4;
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< LoadBalanceActor, 3, ErrorOr<REPLY_TYPE(Request)> >*>(static_cast<LoadBalanceActor*>(this)));
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< LoadBalanceActor, 4, ErrorOr<REPLY_TYPE(Request)> >*>(static_cast<LoadBalanceActor*>(this)));
															#line 1907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont19(loopDepth);
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
	int a_body1loopBody1loopBody1when1(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (firstRequestData.checkAndProcessResult(atMostOnce))
															#line 1935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 1939 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		firstRequestEndpoint = Optional<uint64_t>();
															#line 1947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1loopBody1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
															#line 667 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (firstRequestData.checkAndProcessResult(atMostOnce))
															#line 1956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 1960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		firstRequestEndpoint = Optional<uint64_t>();
															#line 1968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1loopBody1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when2(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (secondRequestData.checkAndProcessResult(atMostOnce))
															#line 1977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 1981 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when2(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (secondRequestData.checkAndProcessResult(atMostOnce))
															#line 1995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 675 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 1999 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<LoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<LoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<LoadBalanceActor*>(this)->ActorCallback< LoadBalanceActor, 3, ErrorOr<REPLY_TYPE(Request)> >::remove();
		static_cast<LoadBalanceActor*>(this)->ActorCallback< LoadBalanceActor, 4, ErrorOr<REPLY_TYPE(Request)> >::remove();

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 3, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> const& value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 3, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> && value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< LoadBalanceActor, 3, ErrorOr<REPLY_TYPE(Request)> >*,Error err) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 4, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> const& value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose4();
		try {
			a_body1loopBody1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 4, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> && value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose4();
		try {
			a_body1loopBody1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< LoadBalanceActor, 4, ErrorOr<REPLY_TYPE(Request)> >*,Error err) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1loopBody1cont21(int loopDepth) 
	{
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (++numAttempts >= alternatives->size())
															#line 2110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			backoff = std::min( FLOW_KNOBS->LOAD_BALANCE_MAX_BACKOFF, std::max(FLOW_KNOBS->LOAD_BALANCE_START_BACKOFF, backoff * FLOW_KNOBS->LOAD_BALANCE_BACKOFF_RATE));
															#line 2114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont18(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopHead2(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1loopBody2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody2(int loopDepth) 
	{
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Request)>> __when_expr_5 = firstRequestData.response;
															#line 693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (static_cast<LoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 2133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody2when1(__when_expr_5.get(), loopDepth); };
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		StrictFuture<Void> __when_expr_6 = secondDelay;
															#line 2137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody2when2(__when_expr_6.get(), loopDepth); };
		static_cast<LoadBalanceActor*>(this)->actor_wait_state = 5;
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< LoadBalanceActor, 5, ErrorOr<REPLY_TYPE(Request)> >*>(static_cast<LoadBalanceActor*>(this)));
															#line 710 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< LoadBalanceActor, 6, Void >*>(static_cast<LoadBalanceActor*>(this)));
															#line 2144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1break2(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont21(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody2cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1loopHead2(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody2when1(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (model)
															#line 2172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			model->secondMultiplier = std::max(model->secondMultiplier - FLOW_KNOBS->SECOND_REQUEST_MULTIPLIER_DECAY, 1.0);
															#line 698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			model->secondBudget = std::min(model->secondBudget + FLOW_KNOBS->SECOND_REQUEST_BUDGET_GROWTH, FLOW_KNOBS->SECOND_REQUEST_MAX_BUDGET);
															#line 2178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (firstRequestData.checkAndProcessResult(atMostOnce))
															#line 2182 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 2186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		firstRequestEndpoint = Optional<uint64_t>();
															#line 2194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		return a_body1loopBody1break2(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1loopBody2when1(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
															#line 695 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (model)
															#line 2203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			model->secondMultiplier = std::max(model->secondMultiplier - FLOW_KNOBS->SECOND_REQUEST_MULTIPLIER_DECAY, 1.0);
															#line 698 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			model->secondBudget = std::min(model->secondBudget + FLOW_KNOBS->SECOND_REQUEST_BUDGET_GROWTH, FLOW_KNOBS->SECOND_REQUEST_MAX_BUDGET);
															#line 2209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 703 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (firstRequestData.checkAndProcessResult(atMostOnce))
															#line 2213 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 704 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 2217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		firstRequestEndpoint = Optional<uint64_t>();
															#line 2225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		return a_body1loopBody1break2(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1loopBody2when2(Void const& _,int loopDepth) 
	{
															#line 711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		secondDelay = Never();
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (model && model->secondBudget >= 1.0)
															#line 2236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			model->secondMultiplier += FLOW_KNOBS->SECOND_REQUEST_MULTIPLIER_GROWTH;
															#line 714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			model->secondBudget -= 1.0;
															#line 2242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			return a_body1loopBody1break2(loopDepth==0?0:loopDepth-1); // break
		}
		loopDepth = a_body1loopBody1loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody2when2(Void && _,int loopDepth) 
	{
															#line 711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		secondDelay = Never();
															#line 712 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (model && model->secondBudget >= 1.0)
															#line 2255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 713 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			model->secondMultiplier += FLOW_KNOBS->SECOND_REQUEST_MULTIPLIER_GROWTH;
															#line 714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			model->secondBudget -= 1.0;
															#line 2261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			return a_body1loopBody1break2(loopDepth==0?0:loopDepth-1); // break
		}
		loopDepth = a_body1loopBody1loopBody2cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<LoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<LoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<LoadBalanceActor*>(this)->ActorCallback< LoadBalanceActor, 5, ErrorOr<REPLY_TYPE(Request)> >::remove();
		static_cast<LoadBalanceActor*>(this)->ActorCallback< LoadBalanceActor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 5, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> const& value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose5();
		try {
			a_body1loopBody1loopBody2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 5, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> && value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose5();
		try {
			a_body1loopBody1loopBody2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< LoadBalanceActor, 5, ErrorOr<REPLY_TYPE(Request)> >*,Error err) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose5();
		try {
			a_body1loopBody1loopBody2when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose5();
		try {
			a_body1loopBody1loopBody2when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< LoadBalanceActor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 6);

	}
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	Reference<MultiInterface<Multi>> alternatives;
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	RequestStream<Request, P> Interface::* channel;
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	Request request;
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	TaskPriority taskID;
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	AtMostOnce atMostOnce;
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	QueueModel* model;
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	RequestData<Request, Interface, Multi, P> firstRequestData;
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	RequestData<Request, Interface, Multi, P> secondRequestData;
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	Optional<uint64_t> firstRequestEndpoint;
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	Future<Void> secondDelay;
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	Promise<Void> requestFinished;
															#line 459 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	double startTime;
															#line 461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	TriedAllOptions triedAllOptions;
															#line 469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	int bestAlt;
															#line 470 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	int nextAlt;
															#line 567 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	int startAlt;
															#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	int startDistance;
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	int numAttempts;
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	double backoff;
															#line 596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	RequestStream<Request, P> const* stream;
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	LBDistance::Type distance;
															#line 2407 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
};
// This generated class is to be used only via loadBalance()
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
template <class Interface, class Request, class Multi, bool P>
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
class LoadBalanceActor final : public Actor<REPLY_TYPE(Request)>, public ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 0, Void >, public ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 1, Void >, public ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 2, ErrorOr<REPLY_TYPE(Request)> >, public ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 3, ErrorOr<REPLY_TYPE(Request)> >, public ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 4, ErrorOr<REPLY_TYPE(Request)> >, public ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 5, ErrorOr<REPLY_TYPE(Request)> >, public ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 6, Void >, public FastAllocated<LoadBalanceActor<Interface, Request, Multi, P>>, public LoadBalanceActorState<Interface, Request, Multi, P, LoadBalanceActor<Interface, Request, Multi, P>> {
															#line 2414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
public:
	using FastAllocated<LoadBalanceActor<Interface, Request, Multi, P>>::operator new;
	using FastAllocated<LoadBalanceActor<Interface, Request, Multi, P>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<REPLY_TYPE(Request)>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 0, Void >;
friend struct ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 1, Void >;
friend struct ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 2, ErrorOr<REPLY_TYPE(Request)> >;
friend struct ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 3, ErrorOr<REPLY_TYPE(Request)> >;
friend struct ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 4, ErrorOr<REPLY_TYPE(Request)> >;
friend struct ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 5, ErrorOr<REPLY_TYPE(Request)> >;
friend struct ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 6, Void >;
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	LoadBalanceActor(Reference<MultiInterface<Multi>> const& alternatives,RequestStream<Request, P> Interface::* const& channel,Request const& request = Request(),TaskPriority const& taskID = TaskPriority::DefaultPromiseEndpoint,AtMostOnce const& atMostOnce = AtMostOnce::False,QueueModel* const& model = nullptr) 
															#line 2431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		 : Actor<REPLY_TYPE(Request)>(),
		   LoadBalanceActorState<Interface, Request, Multi, P, LoadBalanceActor<Interface, Request, Multi, P>>(alternatives, channel, request, taskID, atMostOnce, model)
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("loadBalance");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 2, ErrorOr<REPLY_TYPE(Request)> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 3, ErrorOr<REPLY_TYPE(Request)> >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< LoadBalanceActor<Interface, Request, Multi, P>, 5, ErrorOr<REPLY_TYPE(Request)> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
template <class Interface, class Request, class Multi, bool P>
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
[[nodiscard]] Future<REPLY_TYPE(Request)> loadBalance( Reference<MultiInterface<Multi>> const& alternatives, RequestStream<Request, P> Interface::* const& channel, Request const& request = Request(), TaskPriority const& taskID = TaskPriority::DefaultPromiseEndpoint, AtMostOnce const& atMostOnce = AtMostOnce::False, QueueModel* const& model = nullptr ) {
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	return Future<REPLY_TYPE(Request)>(new LoadBalanceActor<Interface, Request, Multi, P>(alternatives, channel, request, taskID, atMostOnce, model));
															#line 2465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
}

#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"

// Subclasses must initialize all members in their default constructors
// Subclasses must serialize all members
struct BasicLoadBalancedReply {
	int processBusyTime;
	BasicLoadBalancedReply() : processBusyTime(0) {}
};

Optional<BasicLoadBalancedReply> getBasicLoadBalancedReply(const BasicLoadBalancedReply* reply);
Optional<BasicLoadBalancedReply> getBasicLoadBalancedReply(const void*);

// A simpler version of LoadBalance that does not send second requests where the list of servers are always fresh
//
// If |alternativeChosen| is not null, then atMostOnce must be True, and if the returned future completes successfully
// then *alternativeChosen will be the alternative to which the message was sent. *alternativeChosen must outlive the
// returned future.
															#line 2485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
namespace {
// This generated class is to be used only via basicLoadBalance()
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
template <class Interface, class Request, class Multi, bool P, class BasicLoadBalanceActor>
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
class BasicLoadBalanceActorState {
															#line 2492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
public:
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	BasicLoadBalanceActorState(Reference<ModelInterface<Multi>> const& alternatives,RequestStream<Request, P> Interface::* const& channel,Request const& request = Request(),TaskPriority const& taskID = TaskPriority::DefaultPromiseEndpoint,AtMostOnce const& atMostOnce = AtMostOnce::False,int* const& alternativeChosen = nullptr) 
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		 : alternatives(alternatives),
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   channel(channel),
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   request(request),
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   taskID(taskID),
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   atMostOnce(atMostOnce),
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		   alternativeChosen(alternativeChosen)
															#line 2509 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
	{
		fdb_probe_actor_create("basicLoadBalance", reinterpret_cast<unsigned long>(this));

	}
	~BasicLoadBalanceActorState() 
	{
		fdb_probe_actor_destroy("basicLoadBalance", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			ASSERT(alternativeChosen == nullptr || atMostOnce == AtMostOnce::True);
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			setReplyPriority(request, taskID);
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!alternatives)
															#line 2528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				this->~BasicLoadBalanceActorState();
															#line 2532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				static_cast<BasicLoadBalanceActor*>(this)->sendAndDelPromiseRef(Never());
				return 0;
			}
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			ASSERT(alternatives->size() && alternatives->alwaysFresh());
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			bestAlt = alternatives->getBest();
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			nextAlt = deterministicRandom()->randomInt(0, std::max(alternatives->size() - 1, 1));
															#line 767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (nextAlt >= bestAlt)
															#line 2544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 768 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				nextAlt++;
															#line 2548 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			}
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			startAlt = nextAlt;
															#line 771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			startDistance = (bestAlt + alternatives->size() - startAlt) % alternatives->size();
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			numAttempts = 0;
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			backoff = 0;
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			useAlt = int();
															#line 776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			;
															#line 2562 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
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
		this->~BasicLoadBalanceActorState();
		static_cast<BasicLoadBalanceActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		stream = nullptr;
															#line 779 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		for(int alternativeNum = 0;alternativeNum < alternatives->size();alternativeNum++) {
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			useAlt = nextAlt;
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (nextAlt == startAlt)
															#line 2598 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 782 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				useAlt = bestAlt;
															#line 2602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			}
			else
			{
															#line 783 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				if ((nextAlt + alternatives->size() - startAlt) % alternatives->size() <= startDistance)
															#line 2608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 784 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
					useAlt = (nextAlt + alternatives->size() - 1) % alternatives->size();
															#line 2612 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				}
			}
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			stream = &alternatives->get(useAlt, channel);
															#line 787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (alternativeChosen != nullptr)
															#line 2619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				*alternativeChosen = useAlt;
															#line 2623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			}
															#line 790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!IFailureMonitor::failureMonitor().getState(stream->getEndpoint()).failed)
															#line 2627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
				break;
			}
															#line 792 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			nextAlt = (nextAlt + 1) % alternatives->size();
															#line 793 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			stream = nullptr;
															#line 2635 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 796 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (!stream)
															#line 2639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			std::vector<Future<Void>> ok(alternatives->size());
															#line 800 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			for(int i = 0;i < ok.size();i++) {
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				ok[i] = IFailureMonitor::failureMonitor().onStateEqual(alternatives->get(i, channel).getEndpoint(), FailureStatus(false));
															#line 2647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			}
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			StrictFuture<Void> __when_expr_0 = quorum(ok, 1);
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2653 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state = 1;
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< BasicLoadBalanceActor, 0, Void >*>(static_cast<BasicLoadBalanceActor*>(this)));
															#line 2658 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			loopDepth = 0;
		}
		else
		{
															#line 808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (backoff > 0.0)
															#line 2665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				StrictFuture<Void> __when_expr_1 = delay(backoff);
															#line 809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				if (static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2671 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
				static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state = 2;
															#line 809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< BasicLoadBalanceActor, 1, Void >*>(static_cast<BasicLoadBalanceActor*>(this)));
															#line 2676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont11(loopDepth);
			}
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		nextAlt = (nextAlt + 1) % alternatives->size();
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		resetReply(request, taskID);
															#line 2693 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void const& _,int loopDepth) 
	{
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		numAttempts = 0;
															#line 2702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void && _,int loopDepth) 
	{
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		numAttempts = 0;
															#line 2711 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<BasicLoadBalanceActor*>(this)->ActorCallback< BasicLoadBalanceActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< BasicLoadBalanceActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< BasicLoadBalanceActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< BasicLoadBalanceActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont11(int loopDepth) 
	{
															#line 812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Request)>> __when_expr_2 = stream->tryGetReply(request);
															#line 812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2785 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont11when1(__when_expr_2.get(), loopDepth); };
		static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state = 3;
															#line 812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< BasicLoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*>(static_cast<BasicLoadBalanceActor*>(this)));
															#line 2790 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont12(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont12(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont12(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont12(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<BasicLoadBalanceActor*>(this)->ActorCallback< BasicLoadBalanceActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< BasicLoadBalanceActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< BasicLoadBalanceActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< BasicLoadBalanceActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont11cont1(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (result.present())
															#line 2874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			Optional<BasicLoadBalancedReply> loadBalancedReply = getBasicLoadBalancedReply(&result.get());
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (loadBalancedReply.present())
															#line 2880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				alternatives->updateRecent(useAlt, loadBalancedReply.get().processBusyTime);
															#line 2884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			}
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<BasicLoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~BasicLoadBalanceActorState(); static_cast<BasicLoadBalanceActor*>(this)->destroy(); return 0; }
															#line 2888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<BasicLoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~BasicLoadBalanceActorState();
			static_cast<BasicLoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (result.getError().code() != error_code_broken_promise && result.getError().code() != error_code_request_maybe_delivered)
															#line 2896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			return a_body1Catch1(result.getError(), std::max(0, loopDepth - 1));
															#line 2900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (atMostOnce)
															#line 2904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			return a_body1Catch1(request_maybe_delivered(), std::max(0, loopDepth - 1));
															#line 2908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (++numAttempts >= alternatives->size())
															#line 2912 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			backoff = std::min( FLOW_KNOBS->LOAD_BALANCE_MAX_BACKOFF, std::max(FLOW_KNOBS->LOAD_BALANCE_START_BACKOFF, backoff * FLOW_KNOBS->LOAD_BALANCE_BACKOFF_RATE));
															#line 2916 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11cont1(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
															#line 814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (result.present())
															#line 2926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 815 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			Optional<BasicLoadBalancedReply> loadBalancedReply = getBasicLoadBalancedReply(&result.get());
															#line 816 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (loadBalancedReply.present())
															#line 2932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
				alternatives->updateRecent(useAlt, loadBalancedReply.get().processBusyTime);
															#line 2936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			}
															#line 820 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<BasicLoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~BasicLoadBalanceActorState(); static_cast<BasicLoadBalanceActor*>(this)->destroy(); return 0; }
															#line 2940 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<BasicLoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~BasicLoadBalanceActorState();
			static_cast<BasicLoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (result.getError().code() != error_code_broken_promise && result.getError().code() != error_code_request_maybe_delivered)
															#line 2948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			return a_body1Catch1(result.getError(), std::max(0, loopDepth - 1));
															#line 2952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (atMostOnce)
															#line 2956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			return a_body1Catch1(request_maybe_delivered(), std::max(0, loopDepth - 1));
															#line 2960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 832 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
		if (++numAttempts >= alternatives->size())
															#line 2964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 833 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
			backoff = std::min( FLOW_KNOBS->LOAD_BALANCE_MAX_BACKOFF, std::max(FLOW_KNOBS->LOAD_BALANCE_START_BACKOFF, backoff * FLOW_KNOBS->LOAD_BALANCE_BACKOFF_RATE));
															#line 2968 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11when1(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11cont1(result, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11when1(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11cont1(std::move(result), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<BasicLoadBalanceActor*>(this)->ActorCallback< BasicLoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >::remove();

	}
	void a_callback_fire(ActorCallback< BasicLoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> const& value) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont11when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< BasicLoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> && value) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont11when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< BasicLoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*,Error err) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	Reference<ModelInterface<Multi>> alternatives;
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	RequestStream<Request, P> Interface::* channel;
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	Request request;
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	TaskPriority taskID;
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	AtMostOnce atMostOnce;
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	int* alternativeChosen;
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	int bestAlt;
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	int nextAlt;
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	int startAlt;
															#line 771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	int startDistance;
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	int numAttempts;
															#line 774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	double backoff;
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	int useAlt;
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	RequestStream<Request, P> const* stream;
															#line 3065 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
};
// This generated class is to be used only via basicLoadBalance()
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
template <class Interface, class Request, class Multi, bool P>
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
class BasicLoadBalanceActor final : public Actor<REPLY_TYPE(Request)>, public ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi, P>, 0, Void >, public ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi, P>, 1, Void >, public ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi, P>, 2, ErrorOr<REPLY_TYPE(Request)> >, public FastAllocated<BasicLoadBalanceActor<Interface, Request, Multi, P>>, public BasicLoadBalanceActorState<Interface, Request, Multi, P, BasicLoadBalanceActor<Interface, Request, Multi, P>> {
															#line 3072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
public:
	using FastAllocated<BasicLoadBalanceActor<Interface, Request, Multi, P>>::operator new;
	using FastAllocated<BasicLoadBalanceActor<Interface, Request, Multi, P>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<REPLY_TYPE(Request)>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi, P>, 0, Void >;
friend struct ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi, P>, 1, Void >;
friend struct ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi, P>, 2, ErrorOr<REPLY_TYPE(Request)> >;
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	BasicLoadBalanceActor(Reference<ModelInterface<Multi>> const& alternatives,RequestStream<Request, P> Interface::* const& channel,Request const& request = Request(),TaskPriority const& taskID = TaskPriority::DefaultPromiseEndpoint,AtMostOnce const& atMostOnce = AtMostOnce::False,int* const& alternativeChosen = nullptr) 
															#line 3085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
		 : Actor<REPLY_TYPE(Request)>(),
		   BasicLoadBalanceActorState<Interface, Request, Multi, P, BasicLoadBalanceActor<Interface, Request, Multi, P>>(alternatives, channel, request, taskID, atMostOnce, alternativeChosen)
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("basicLoadBalance");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi, P>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi, P>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi, P>, 2, ErrorOr<REPLY_TYPE(Request)> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
template <class Interface, class Request, class Multi, bool P>
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
[[nodiscard]] Future<REPLY_TYPE(Request)> basicLoadBalance( Reference<ModelInterface<Multi>> const& alternatives, RequestStream<Request, P> Interface::* const& channel, Request const& request = Request(), TaskPriority const& taskID = TaskPriority::DefaultPromiseEndpoint, AtMostOnce const& atMostOnce = AtMostOnce::False, int* const& alternativeChosen = nullptr ) {
															#line 751 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"
	return Future<REPLY_TYPE(Request)>(new BasicLoadBalanceActor<Interface, Request, Multi, P>(alternatives, channel, request, taskID, atMostOnce, alternativeChosen));
															#line 3117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.g.h"
}

#line 843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/include/fdbrpc/LoadBalance.actor.h"

#include "flow/unactorcompiler.h"

#endif
