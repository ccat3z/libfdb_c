#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
/*
 * BlobGranuleRequest.actor.h
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
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_BLOB_GRANULE_REQUEST_ACTOR_G_H)
#define FDBCLIENT_BLOB_GRANULE_REQUEST_ACTOR_G_H
#include "fdbclient/BlobGranuleRequest.actor.g.h"
#elif !defined(FDBCLIENT_BLOB_GRANULE_REQUEST_ACTOR_H)
#define FDBCLIENT_BLOB_GRANULE_REQUEST_ACTOR_H

#include "flow/flow.h"
#include "flow/Knobs.h"

// #include "fdbclient/NativeAPI.actor.h"
#include "flow/Arena.h"
#include "fdbclient/DatabaseContext.h"
#include "fdbclient/BlobWorkerInterface.h"

#include "flow/actorcompiler.h" // This must be the last #include.

#define BGR_DEBUG false

															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
namespace {
// This generated class is to be used only via txnDoBlobGranuleRequests()
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
template <class Request, bool P, class TxnDoBlobGranuleRequestsActor>
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
class TxnDoBlobGranuleRequestsActorState {
															#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
public:
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	TxnDoBlobGranuleRequestsActorState(Transaction* const& tr,Key* const& beginKey,Key const& endKey,Request const& request,RequestStream<Request, P> BlobWorkerInterface::* const& channel) 
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		 : tr(tr),
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		   beginKey(beginKey),
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		   endKey(endKey),
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		   request(request),
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		   channel(channel)
															#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
	{
		fdb_probe_actor_create("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this));

	}
	~TxnDoBlobGranuleRequestsActorState() 
	{
		fdb_probe_actor_destroy("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			StrictFuture<RangeResult> __when_expr_0 = krmGetRanges( tr, blobGranuleMappingKeys.begin, KeyRangeRef(*beginKey, endKey), 64, GetRangeLimits::BYTE_LIMIT_UNLIMITED);
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			if (static_cast<TxnDoBlobGranuleRequestsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TxnDoBlobGranuleRequestsActor*>(this)->actor_wait_state = 1;
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TxnDoBlobGranuleRequestsActor, 0, RangeResult >*>(static_cast<TxnDoBlobGranuleRequestsActor*>(this)));
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
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
		this->~TxnDoBlobGranuleRequestsActorState();
		static_cast<TxnDoBlobGranuleRequestsActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		i = 0;
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		requests = std::vector<Future<ErrorOr<REPLY_TYPE(Request)>>>();
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		results = Standalone<VectorRef<REPLY_TYPE(Request)>>();
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		;
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult const& __blobGranuleMapping,int loopDepth) 
	{
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		blobGranuleMapping = __blobGranuleMapping;
															#line 127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && __blobGranuleMapping,int loopDepth) 
	{
		blobGranuleMapping = std::move(__blobGranuleMapping);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TxnDoBlobGranuleRequestsActor*>(this)->actor_wait_state > 0) static_cast<TxnDoBlobGranuleRequestsActor*>(this)->actor_wait_state = 0;
		static_cast<TxnDoBlobGranuleRequestsActor*>(this)->ActorCallback< TxnDoBlobGranuleRequestsActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< TxnDoBlobGranuleRequestsActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TxnDoBlobGranuleRequestsActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TxnDoBlobGranuleRequestsActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		j = 0;
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		;
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		loopDepth = a_body1cont2loopHead1(loopDepth);

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
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		if (!(i < blobGranuleMapping.size() - 1))
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		{
			return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 59 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		if (!blobGranuleMapping[i].value.size())
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		{
															#line 60 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			if (BGR_DEBUG)
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			{
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
				fmt::print("ERROR: No valid granule data for range [{0} - {1}) \n", blobGranuleMapping[i].key.printable(), blobGranuleMapping[i + 1].key.printable());
															#line 226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			}
															#line 66 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			return a_body1Catch1(blob_granule_transaction_too_old(), std::max(0, loopDepth - 1));
															#line 230 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		}
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		workerId = decodeBlobGranuleMappingValue(blobGranuleMapping[i].value);
															#line 70 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		if (workerId == UID())
															#line 236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		{
															#line 71 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			if (BGR_DEBUG)
															#line 240 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			{
															#line 72 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
				fmt::print("ERROR: Invalid Blob Worker ID for range [{0} - {1}) \n", blobGranuleMapping[i].key.printable(), blobGranuleMapping[i + 1].key.printable());
															#line 244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			}
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			return a_body1Catch1(blob_granule_transaction_too_old(), std::max(0, loopDepth - 1));
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		}
															#line 80 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		if (!tr->trState->cx->blobWorker_interf.count(workerId))
															#line 252 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		{
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			StrictFuture<Optional<Value>> __when_expr_1 = tr->get(blobWorkerListKeyFor(workerId));
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			if (static_cast<TxnDoBlobGranuleRequestsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 258 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TxnDoBlobGranuleRequestsActor*>(this)->actor_wait_state = 2;
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TxnDoBlobGranuleRequestsActor, 1, Optional<Value> >*>(static_cast<TxnDoBlobGranuleRequestsActor*>(this)));
															#line 263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
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
			return a_body1cont2(loopDepth);
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
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		if (BGR_DEBUG)
															#line 290 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		{
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			fmt::print("Requesting range [{0} - {1}) from worker {2}!\n", blobGranuleMapping[i].key.printable(), blobGranuleMapping[i + 1].key.printable(), workerId.toString().substr(0, 5));
															#line 294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		}
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		KeyRangeRef range(blobGranuleMapping[i].key, blobGranuleMapping[i + 1].key);
															#line 107 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		request.reply.reset();
															#line 108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		request.setRange(range);
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		BlobWorkerInterface bwi = tr->trState->cx->blobWorker_interf[workerId];
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		RequestStream<Request, P> const* stream = &(bwi.*channel);
															#line 112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		Future<ErrorOr<REPLY_TYPE(Request)>> response = stream->tryGetReply(request);
															#line 113 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		requests.push_back(response);
															#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		i++;
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont7(Optional<Value> const& workerInterface,int loopDepth) 
	{
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		if (workerInterface.present())
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		{
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			tr->trState->cx->blobWorker_interf[workerId] = decodeBlobWorkerListValue(workerInterface.get());
															#line 325 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		}
		else
		{
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			if (BGR_DEBUG)
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			{
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
				fmt::print("ERROR: Worker  for range [{1} - {2}) does not exist!\n", workerId.toString().substr(0, 5), blobGranuleMapping[i].key.printable(), blobGranuleMapping[i + 1].key.printable());
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			}
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			return a_body1Catch1(blob_granule_request_failed(), std::max(0, loopDepth - 1));
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		}
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont7(Optional<Value> && workerInterface,int loopDepth) 
	{
															#line 85 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		if (workerInterface.present())
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		{
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			tr->trState->cx->blobWorker_interf[workerId] = decodeBlobWorkerListValue(workerInterface.get());
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		}
		else
		{
															#line 88 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			if (BGR_DEBUG)
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			{
															#line 89 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
				fmt::print("ERROR: Worker  for range [{1} - {2}) does not exist!\n", workerId.toString().substr(0, 5), blobGranuleMapping[i].key.printable(), blobGranuleMapping[i + 1].key.printable());
															#line 363 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			}
															#line 95 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			return a_body1Catch1(blob_granule_request_failed(), std::max(0, loopDepth - 1));
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		}
		loopDepth = a_body1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Optional<Value> const& workerInterface,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont7(workerInterface, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Optional<Value> && workerInterface,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont7(std::move(workerInterface), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<TxnDoBlobGranuleRequestsActor*>(this)->actor_wait_state > 0) static_cast<TxnDoBlobGranuleRequestsActor*>(this)->actor_wait_state = 0;
		static_cast<TxnDoBlobGranuleRequestsActor*>(this)->ActorCallback< TxnDoBlobGranuleRequestsActor, 1, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< TxnDoBlobGranuleRequestsActor, 1, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TxnDoBlobGranuleRequestsActor, 1, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TxnDoBlobGranuleRequestsActor, 1, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		if (i < blobGranuleMapping.size() - 1)
															#line 440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		{
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			*beginKey = blobGranuleMapping[i].key;
															#line 144 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			return a_body1Catch1(blob_granule_request_failed(), loopDepth);
															#line 446 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		}
		else
		{
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			if (blobGranuleMapping.more)
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			{
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
				*beginKey = blobGranuleMapping.back().key;
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			}
			else
			{
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
				*beginKey = endKey;
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			}
		}
															#line 151 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		if (!static_cast<TxnDoBlobGranuleRequestsActor*>(this)->SAV<Standalone<VectorRef<REPLY_TYPE(Request)>>>::futures) { (void)(results); this->~TxnDoBlobGranuleRequestsActorState(); static_cast<TxnDoBlobGranuleRequestsActor*>(this)->destroy(); return 0; }
															#line 467 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		new (&static_cast<TxnDoBlobGranuleRequestsActor*>(this)->SAV< Standalone<VectorRef<REPLY_TYPE(Request)>> >::value()) Standalone<VectorRef<REPLY_TYPE(Request)>>(std::move(results)); // state_var_RVO
		this->~TxnDoBlobGranuleRequestsActorState();
		static_cast<TxnDoBlobGranuleRequestsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont2loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1(int loopDepth) 
	{
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		if (!(j < requests.size()))
															#line 486 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		{
			return a_body1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
		try {
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			StrictFuture<ErrorOr<REPLY_TYPE(Request)>> __when_expr_2 = requests[j];
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			if (static_cast<TxnDoBlobGranuleRequestsActor*>(this)->actor_wait_state < 0) return a_body1cont2loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont2loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2loopBody1when1(__when_expr_2.get(), loopDepth); };
			static_cast<TxnDoBlobGranuleRequestsActor*>(this)->actor_wait_state = 3;
															#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TxnDoBlobGranuleRequestsActor, 2, ErrorOr<REPLY_TYPE(Request)> >*>(static_cast<TxnDoBlobGranuleRequestsActor*>(this)));
															#line 500 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1cont2loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont2loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2break1(int loopDepth) 
	{
		try {
			return a_body1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2loopBody1cont1(int loopDepth) 
	{
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		j++;
															#line 528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		if (loopDepth == 0) return a_body1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1cont2loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			if (e.code() == error_code_wrong_shard_server || e.code() == error_code_request_maybe_delivered || e.code() == error_code_broken_promise || e.code() == error_code_connection_failed)
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			{
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
				i = j;
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
				return a_body1cont2break1(loopDepth==0?0:loopDepth-1); // break
			}
			else
			{
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
				if (BGR_DEBUG)
															#line 549 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
				{
															#line 132 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
					fmt::print("ERROR: Error doing request for range [{0} - {1}): {2}!\n", blobGranuleMapping[j].key.printable(), blobGranuleMapping[j + 1].key.printable(), e.name());
															#line 553 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
				}
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1cont2loopBody1cont3(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		if (result.isError())
															#line 572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		{
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			return a_body1cont2loopBody1Catch1(result.getError(), loopDepth);
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		}
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		results.push_back(results.arena(), result.get());
															#line 580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		loopDepth = a_body1cont2loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont3(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		if (result.isError())
															#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		{
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			return a_body1cont2loopBody1Catch1(result.getError(), loopDepth);
															#line 593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		}
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		results.push_back(results.arena(), result.get());
															#line 597 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		loopDepth = a_body1cont2loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1when1(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont3(result, loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1when1(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont3(std::move(result), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<TxnDoBlobGranuleRequestsActor*>(this)->actor_wait_state > 0) static_cast<TxnDoBlobGranuleRequestsActor*>(this)->actor_wait_state = 0;
		static_cast<TxnDoBlobGranuleRequestsActor*>(this)->ActorCallback< TxnDoBlobGranuleRequestsActor, 2, ErrorOr<REPLY_TYPE(Request)> >::remove();

	}
	void a_callback_fire(ActorCallback< TxnDoBlobGranuleRequestsActor, 2, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> const& value) 
	{
		fdb_probe_actor_enter("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< TxnDoBlobGranuleRequestsActor, 2, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> && value) 
	{
		fdb_probe_actor_enter("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< TxnDoBlobGranuleRequestsActor, 2, ErrorOr<REPLY_TYPE(Request)> >*,Error err) 
	{
		fdb_probe_actor_enter("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont2loopBody1cont6(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont2loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	Transaction* tr;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	Key* beginKey;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	Key endKey;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	Request request;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	RequestStream<Request, P> BlobWorkerInterface::* channel;
															#line 51 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	RangeResult blobGranuleMapping;
															#line 54 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	int i;
															#line 55 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	std::vector<Future<ErrorOr<REPLY_TYPE(Request)>>> requests;
															#line 56 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	Standalone<VectorRef<REPLY_TYPE(Request)>> results;
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	UID workerId;
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	int j;
															#line 700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
};
// This generated class is to be used only via txnDoBlobGranuleRequests()
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
template <class Request, bool P>
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
class TxnDoBlobGranuleRequestsActor final : public Actor<Standalone<VectorRef<REPLY_TYPE(Request)>>>, public ActorCallback< TxnDoBlobGranuleRequestsActor<Request, P>, 0, RangeResult >, public ActorCallback< TxnDoBlobGranuleRequestsActor<Request, P>, 1, Optional<Value> >, public ActorCallback< TxnDoBlobGranuleRequestsActor<Request, P>, 2, ErrorOr<REPLY_TYPE(Request)> >, public FastAllocated<TxnDoBlobGranuleRequestsActor<Request, P>>, public TxnDoBlobGranuleRequestsActorState<Request, P, TxnDoBlobGranuleRequestsActor<Request, P>> {
															#line 707 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
public:
	using FastAllocated<TxnDoBlobGranuleRequestsActor<Request, P>>::operator new;
	using FastAllocated<TxnDoBlobGranuleRequestsActor<Request, P>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Standalone<VectorRef<REPLY_TYPE(Request)>>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TxnDoBlobGranuleRequestsActor<Request, P>, 0, RangeResult >;
friend struct ActorCallback< TxnDoBlobGranuleRequestsActor<Request, P>, 1, Optional<Value> >;
friend struct ActorCallback< TxnDoBlobGranuleRequestsActor<Request, P>, 2, ErrorOr<REPLY_TYPE(Request)> >;
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	TxnDoBlobGranuleRequestsActor(Transaction* const& tr,Key* const& beginKey,Key const& endKey,Request const& request,RequestStream<Request, P> BlobWorkerInterface::* const& channel) 
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		 : Actor<Standalone<VectorRef<REPLY_TYPE(Request)>>>(),
		   TxnDoBlobGranuleRequestsActorState<Request, P, TxnDoBlobGranuleRequestsActor<Request, P>>(tr, beginKey, endKey, request, channel)
	{
		fdb_probe_actor_enter("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("txnDoBlobGranuleRequests");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("txnDoBlobGranuleRequests", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TxnDoBlobGranuleRequestsActor<Request, P>, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TxnDoBlobGranuleRequestsActor<Request, P>, 1, Optional<Value> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< TxnDoBlobGranuleRequestsActor<Request, P>, 2, ErrorOr<REPLY_TYPE(Request)> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
template <class Request, bool P>
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
[[nodiscard]] Future<Standalone<VectorRef<REPLY_TYPE(Request)>>> txnDoBlobGranuleRequests( Transaction* const& tr, Key* const& beginKey, Key const& endKey, Request const& request, RequestStream<Request, P> BlobWorkerInterface::* const& channel ) {
															#line 43 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	return Future<Standalone<VectorRef<REPLY_TYPE(Request)>>>(new TxnDoBlobGranuleRequestsActor<Request, P>(tr, beginKey, endKey, request, channel));
															#line 752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
}

#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"

// FIXME: port other request types to this function
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
namespace {
// This generated class is to be used only via doBlobGranuleRequests()
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
template <class Request, bool P, class DoBlobGranuleRequestsActor>
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
class DoBlobGranuleRequestsActorState {
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
public:
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	DoBlobGranuleRequestsActorState(Database const& cx,KeyRange const& range,Request const& request,RequestStream<Request, P> BlobWorkerInterface::* const& channel) 
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		 : cx(cx),
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		   range(range),
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		   request(request),
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		   channel(channel),
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		   beginKey(range.begin),
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		   endKey(range.end),
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		   tr(cx),
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		   results()
															#line 786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
	{
		fdb_probe_actor_create("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this));

	}
	~DoBlobGranuleRequestsActorState() 
	{
		fdb_probe_actor_destroy("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			;
															#line 801 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
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
		this->~DoBlobGranuleRequestsActorState();
		static_cast<DoBlobGranuleRequestsActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		if (beginKey >= endKey)
															#line 831 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		{
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			if (!static_cast<DoBlobGranuleRequestsActor*>(this)->SAV<Standalone<VectorRef<REPLY_TYPE(Request)>>>::futures) { (void)(results); this->~DoBlobGranuleRequestsActorState(); static_cast<DoBlobGranuleRequestsActor*>(this)->destroy(); return 0; }
															#line 835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			new (&static_cast<DoBlobGranuleRequestsActor*>(this)->SAV< Standalone<VectorRef<REPLY_TYPE(Request)>> >::value()) Standalone<VectorRef<REPLY_TYPE(Request)>>(std::move(results)); // state_var_RVO
			this->~DoBlobGranuleRequestsActorState();
			static_cast<DoBlobGranuleRequestsActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		try {
															#line 170 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			tr.setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 172 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			StrictFuture<Standalone<VectorRef<REPLY_TYPE(Request)>>> __when_expr_0 = txnDoBlobGranuleRequests(&tr, &beginKey, endKey, request, channel);
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			if (static_cast<DoBlobGranuleRequestsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 852 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DoBlobGranuleRequestsActor*>(this)->actor_wait_state = 1;
															#line 173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoBlobGranuleRequestsActor, 0, Standalone<VectorRef<REPLY_TYPE(Request)>> >*>(static_cast<DoBlobGranuleRequestsActor*>(this)));
															#line 857 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
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
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			StrictFuture<Void> __when_expr_1 = tr.onError(e);
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			if (static_cast<DoBlobGranuleRequestsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<DoBlobGranuleRequestsActor*>(this)->actor_wait_state = 2;
															#line 180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoBlobGranuleRequestsActor, 1, Void >*>(static_cast<DoBlobGranuleRequestsActor*>(this)));
															#line 886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont3(Standalone<VectorRef<REPLY_TYPE(Request)>> const& partialResults,int loopDepth) 
	{
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		if (!partialResults.empty())
															#line 901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		{
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			results.arena().dependsOn(partialResults.arena());
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			results.append(results.arena(), partialResults.begin(), partialResults.size());
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Standalone<VectorRef<REPLY_TYPE(Request)>> && partialResults,int loopDepth) 
	{
															#line 175 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
		if (!partialResults.empty())
															#line 917 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		{
															#line 176 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			results.arena().dependsOn(partialResults.arena());
															#line 177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
			results.append(results.arena(), partialResults.begin(), partialResults.size());
															#line 923 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Standalone<VectorRef<REPLY_TYPE(Request)>> const& partialResults,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(partialResults, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Standalone<VectorRef<REPLY_TYPE(Request)>> && partialResults,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(partialResults), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DoBlobGranuleRequestsActor*>(this)->actor_wait_state > 0) static_cast<DoBlobGranuleRequestsActor*>(this)->actor_wait_state = 0;
		static_cast<DoBlobGranuleRequestsActor*>(this)->ActorCallback< DoBlobGranuleRequestsActor, 0, Standalone<VectorRef<REPLY_TYPE(Request)>> >::remove();

	}
	void a_callback_fire(ActorCallback< DoBlobGranuleRequestsActor, 0, Standalone<VectorRef<REPLY_TYPE(Request)>> >*,Standalone<VectorRef<REPLY_TYPE(Request)>> const& value) 
	{
		fdb_probe_actor_enter("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoBlobGranuleRequestsActor, 0, Standalone<VectorRef<REPLY_TYPE(Request)>> >*,Standalone<VectorRef<REPLY_TYPE(Request)>> && value) 
	{
		fdb_probe_actor_enter("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoBlobGranuleRequestsActor, 0, Standalone<VectorRef<REPLY_TYPE(Request)>> >*,Error err) 
	{
		fdb_probe_actor_enter("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont6(int loopDepth) 
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
	void a_exitChoose2() 
	{
		if (static_cast<DoBlobGranuleRequestsActor*>(this)->actor_wait_state > 0) static_cast<DoBlobGranuleRequestsActor*>(this)->actor_wait_state = 0;
		static_cast<DoBlobGranuleRequestsActor*>(this)->ActorCallback< DoBlobGranuleRequestsActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoBlobGranuleRequestsActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DoBlobGranuleRequestsActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DoBlobGranuleRequestsActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	Database cx;
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	KeyRange range;
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	Request request;
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	RequestStream<Request, P> BlobWorkerInterface::* channel;
															#line 161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	Key beginKey;
															#line 162 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	Key endKey;
															#line 163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	Transaction tr;
															#line 164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	Standalone<VectorRef<REPLY_TYPE(Request)>> results;
															#line 1096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
};
// This generated class is to be used only via doBlobGranuleRequests()
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
template <class Request, bool P>
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
class DoBlobGranuleRequestsActor final : public Actor<Standalone<VectorRef<REPLY_TYPE(Request)>>>, public ActorCallback< DoBlobGranuleRequestsActor<Request, P>, 0, Standalone<VectorRef<REPLY_TYPE(Request)>> >, public ActorCallback< DoBlobGranuleRequestsActor<Request, P>, 1, Void >, public FastAllocated<DoBlobGranuleRequestsActor<Request, P>>, public DoBlobGranuleRequestsActorState<Request, P, DoBlobGranuleRequestsActor<Request, P>> {
															#line 1103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
public:
	using FastAllocated<DoBlobGranuleRequestsActor<Request, P>>::operator new;
	using FastAllocated<DoBlobGranuleRequestsActor<Request, P>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Standalone<VectorRef<REPLY_TYPE(Request)>>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoBlobGranuleRequestsActor<Request, P>, 0, Standalone<VectorRef<REPLY_TYPE(Request)>> >;
friend struct ActorCallback< DoBlobGranuleRequestsActor<Request, P>, 1, Void >;
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	DoBlobGranuleRequestsActor(Database const& cx,KeyRange const& range,Request const& request,RequestStream<Request, P> BlobWorkerInterface::* const& channel) 
															#line 1115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
		 : Actor<Standalone<VectorRef<REPLY_TYPE(Request)>>>(),
		   DoBlobGranuleRequestsActorState<Request, P, DoBlobGranuleRequestsActor<Request, P>>(cx, range, request, channel)
	{
		fdb_probe_actor_enter("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doBlobGranuleRequests");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doBlobGranuleRequests", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoBlobGranuleRequestsActor<Request, P>, 0, Standalone<VectorRef<REPLY_TYPE(Request)>> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DoBlobGranuleRequestsActor<Request, P>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
template <class Request, bool P>
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
[[nodiscard]] Future<Standalone<VectorRef<REPLY_TYPE(Request)>>> doBlobGranuleRequests( Database const& cx, KeyRange const& range, Request const& request, RequestStream<Request, P> BlobWorkerInterface::* const& channel ) {
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"
	return Future<Standalone<VectorRef<REPLY_TYPE(Request)>>>(new DoBlobGranuleRequestsActor<Request, P>(cx, range, request, channel));
															#line 1146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.g.h"
}

#line 184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/BlobGranuleRequest.actor.h"

#include "flow/unactorcompiler.h"

#endif