#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
/*
 * RESTClient.actor.cpp
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

#include "fdbclient/RESTClient.h"

#include "fdbrpc/HTTP.h"
#include "flow/IRateControl.h"
#include "fdbclient/RESTUtils.h"
#include "flow/Arena.h"
#include "flow/Error.h"
#include "flow/FastRef.h"
#include "flow/Knobs.h"
#include "flow/Net2Packet.h"
#include "flow/flow.h"
#include "flow/network.h"
#include "flow/serialize.h"
#include "flow/Trace.h"
#include "flow/UnitTest.h"
#include "flow/IConnection.h"

#include <memory>
#include <unordered_map>

#include "flow/actorcompiler.h" // always the last include

#define TRACE_REST_OP(opName, url)                                                                                     \
	do {                                                                                                               \
		if (FLOW_KNOBS->REST_LOG_LEVEL >= RESTLogSeverity::DEBUG) {                                                    \
			const std::string urlStr = url.toString();                                                                 \
			TraceEvent("RESTClientOp")                                                                                 \
			    .detail("Op", #opName)                                                                                 \
			    .detail("Url", urlStr)                                                                                 \
			    .detail("IsSecure", url.connType.secure);                                                              \
		}                                                                                                              \
	} while (0);

json_spirit::mObject RESTClient::Stats::getJSON() {
	json_spirit::mObject o;

	o["host_service"] = host_service;
	o["requests_failed"] = requests_failed;
	o["requests_successful"] = requests_successful;
	o["bytes_sent"] = bytes_sent;

	return o;
}

RESTClient::Stats RESTClient::Stats::operator-(const Stats& rhs) {
	Stats r(host_service);

	r.requests_failed = requests_failed - rhs.requests_failed;
	r.requests_successful = requests_successful - rhs.requests_successful;
	r.bytes_sent = bytes_sent - rhs.bytes_sent;

	return r;
}

RESTClient::RESTClient() : conectionPool(makeReference<RESTConnectionPool>(knobs.connection_pool_size)) {}

RESTClient::RESTClient(std::unordered_map<std::string, int>& knobSettings)
  : conectionPool(makeReference<RESTConnectionPool>(knobs.connection_pool_size)) {
	knobs.set(knobSettings);
}

void RESTClient::setKnobs(const std::unordered_map<std::string, int>& knobSettings) {
	knobs.set(knobSettings);
}

std::unordered_map<std::string, int> RESTClient::getKnobs() const {
	return knobs.get();
}

															#line 92 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
namespace {
// This generated class is to be used only via doRequest_impl()
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
template <class DoRequest_implActor>
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
class DoRequest_implActorState {
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
public:
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	DoRequest_implActorState(Reference<RESTClient> const& client,std::string const& verb,HTTP::Headers const& headers,RESTUrl const& url,std::set<unsigned int> const& successCodes) 
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		 : client(client),
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		   verb(verb),
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		   headers(headers),
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		   url(url),
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		   successCodes(successCodes),
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		   req(makeReference<HTTP::OutgoingRequest>()),
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		   content()
															#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
	{
		fdb_probe_actor_create("doRequest_impl", reinterpret_cast<unsigned long>(this));

	}
	~DoRequest_implActorState() 
	{
		fdb_probe_actor_destroy("doRequest_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 98 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			req->data.content = &content;
															#line 99 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			req->data.contentLen = url.body.size();
															#line 100 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			req->data.headers = headers;
															#line 101 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			req->data.headers["Host"] = url.host;
															#line 102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			req->verb = verb;
															#line 103 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			req->resource = url.resource;
															#line 105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			if (FLOW_KNOBS->REST_LOG_LEVEL >= RESTLogSeverity::VERBOSE)
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			{
															#line 106 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				TraceEvent("RESTDoRequestImpl").detail("Url", url.toString());
															#line 149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			}
															#line 109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			if (url.body.size() > 0)
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			{
															#line 110 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				PacketWriter pw(req->data.content->getWriteBuffer(url.body.size()), nullptr, Unversioned());
															#line 111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				pw.serializeBytes(url.body);
															#line 159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			}
															#line 114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			std::string statsKey = RESTClient::getStatsKey(url.host, url.service);
															#line 115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			auto sItr = client->statsMap.find(statsKey);
															#line 116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			if (sItr == client->statsMap.end())
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			{
															#line 117 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				client->statsMap.emplace(statsKey, std::make_unique<RESTClient::Stats>(statsKey));
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			}
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			maxTries = std::min(client->knobs.request_tries, client->knobs.connect_tries);
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			thisTry = 1;
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			nextRetryDelay = 2.0;
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			sendReceiveRate = makeReference<Unlimited>();
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			reqTimeout = (client->knobs.request_timeout_secs * 1.0);
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			connectPoolKey = RESTConnectionPool::getConnectionPoolKey(url.host, url.service);
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			statsPtr = client->statsMap[statsKey].get();
															#line 128 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			;
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
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
		this->~DoRequest_implActorState();
		static_cast<DoRequest_implActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		err = Optional<Error>();
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		remoteAddress = Optional<NetworkAddress>();
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		connectionEstablished = false;
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		r = Reference<HTTP::IncomingResponse>();
															#line 225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		try {
															#line 137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			Future<RESTConnectionPool::ReusableConnection> frconn = client->conectionPool->connect(connectPoolKey, url.connType.secure, client->knobs.max_connection_life);
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			StrictFuture<RESTConnectionPool::ReusableConnection> __when_expr_0 = timeoutError(frconn, client->knobs.connect_timeout);
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			if (static_cast<DoRequest_implActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DoRequest_implActor*>(this)->actor_wait_state = 1;
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DoRequest_implActor, 0, RESTConnectionPool::ReusableConnection >*>(static_cast<DoRequest_implActor*>(this)));
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
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
															#line 165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		if (!err.present() && successCodes.count(r->code) != 0)
															#line 253 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		{
															#line 166 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			statsPtr->requests_successful++;
															#line 167 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			if (!static_cast<DoRequest_implActor*>(this)->SAV<Reference<HTTP::IncomingResponse>>::futures) { (void)(r); this->~DoRequest_implActorState(); static_cast<DoRequest_implActor*>(this)->destroy(); return 0; }
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			new (&static_cast<DoRequest_implActor*>(this)->SAV< Reference<HTTP::IncomingResponse> >::value()) Reference<HTTP::IncomingResponse>(std::move(r)); // state_var_RVO
			this->~DoRequest_implActorState();
			static_cast<DoRequest_implActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		statsPtr->requests_failed++;
															#line 174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		bool retryable = err.present() || r->code == HTTP::HTTP_STATUS_CODE_INTERNAL_SERVER_ERROR || r->code == HTTP::HTTP_STATUS_CODE_BAD_GATEWAY || r->code == HTTP::HTTP_STATUS_CODE_BAD_GATEWAY || r->code == HTTP::HTTP_STATUS_CODE_SERVICE_UNAVAILABLE || r->code == HTTP::HTTP_STATUS_CODE_TOO_MANY_REQUESTS || r->code == HTTP::HTTP_STATUS_CODE_TIMEOUT;
															#line 181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		retryable = retryable && (thisTry < maxTries);
															#line 183 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		TraceEvent event(SevWarn, retryable ? "RESTClientFailedRetryable" : "RESTClientRequestFailed");
															#line 186 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		if (err.present())
															#line 275 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		{
															#line 187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			event.errorUnsuppressed(err.get());
															#line 279 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		}
															#line 189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		event.suppressFor(60);
															#line 190 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		if (!err.present())
															#line 285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		{
															#line 191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			event.detail("ResponseCode", r->code);
															#line 289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		}
															#line 194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		event.detail("ConnectionEstablished", connectionEstablished);
															#line 196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		if (remoteAddress.present())
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		{
															#line 197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			event.detail("RemoteEndpoint", remoteAddress.get());
															#line 299 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		}
		else
		{
															#line 199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			event.detail("RemoteHost", url.host);
															#line 305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		}
															#line 201 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		event.detail("Verb", verb).detail("Resource", url.resource).detail("ThisTry", thisTry);
															#line 205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		if (!r || r->code != HTTP::HTTP_STATUS_CODE_TOO_MANY_REQUESTS)
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		{
															#line 206 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			++thisTry;
															#line 315 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		}
															#line 210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		double delay = nextRetryDelay;
															#line 212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		nextRetryDelay = std::min(nextRetryDelay * 2, 60.0);
															#line 214 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		if (retryable)
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		{
															#line 216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			if (r)
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			{
															#line 217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				auto iRetryAfter = r->data.headers.find("Retry-After");
															#line 218 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				if (iRetryAfter != r->data.headers.end())
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
				{
															#line 219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
					event.detail("RetryAfterHeader", iRetryAfter->second);
															#line 220 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
					char* pEnd;
															#line 221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
					double retryAfter = strtod(iRetryAfter->second.c_str(), &pEnd);
															#line 222 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
					if (*pEnd)
															#line 343 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
					{
															#line 224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
						retryAfter = HTTP::HTTP_RETRYAFTER_DELAY_SECS;
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
					}
															#line 227 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
					delay = std::max(delay, retryAfter);
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
				}
			}
															#line 232 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			event.detail("RetryDelay", delay);
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			StrictFuture<Void> __when_expr_2 = ::delay(delay);
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			if (static_cast<DoRequest_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
			static_cast<DoRequest_implActor*>(this)->actor_wait_state = 3;
															#line 233 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DoRequest_implActor, 2, Void >*>(static_cast<DoRequest_implActor*>(this)));
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
															#line 238 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			if (r && r->code == HTTP::HTTP_STATUS_CODE_NOT_ACCEPTABLE)
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			{
															#line 239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				return a_body1Catch1(http_not_accepted(), std::max(0, loopDepth - 1));
															#line 376 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			}
															#line 242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			if (r && r->code == HTTP::HTTP_STATUS_CODE_UNAUTHORIZED)
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			{
															#line 243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				return a_body1Catch1(http_auth_failed(), std::max(0, loopDepth - 1));
															#line 384 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			}
															#line 247 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			if (err.present())
															#line 388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			{
															#line 248 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				int code = err.get().code();
															#line 255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				if (code == error_code_timed_out && !connectionEstablished)
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
				{
															#line 256 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
					return a_body1Catch1(connection_failed(), std::max(0, loopDepth - 1));
															#line 398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
				}
															#line 259 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				if (code == error_code_timed_out || code == error_code_connection_failed || code == error_code_lookup_failed)
															#line 402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
				{
															#line 261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
					return a_body1Catch1(err.get(), std::max(0, loopDepth - 1));
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
				}
			}
															#line 265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			return a_body1Catch1(http_request_failed(), std::max(0, loopDepth - 1));
															#line 411 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 157 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			{
															#line 158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			}
															#line 160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			err = e;
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
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
															#line 143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		connectionEstablished = true;
															#line 145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		remoteAddress = rconn.conn->getPeerAddress();
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		StrictFuture<Reference<HTTP::IncomingResponse>> __when_expr_1 = timeoutError( HTTP::doRequest(rconn.conn, req, sendReceiveRate, &statsPtr->bytes_sent, sendReceiveRate), reqTimeout);
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		if (static_cast<DoRequest_implActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 450 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<DoRequest_implActor*>(this)->actor_wait_state = 2;
															#line 146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DoRequest_implActor, 1, Reference<HTTP::IncomingResponse> >*>(static_cast<DoRequest_implActor*>(this)));
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(RESTConnectionPool::ReusableConnection const& __rconn,int loopDepth) 
	{
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		rconn = __rconn;
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RESTConnectionPool::ReusableConnection && __rconn,int loopDepth) 
	{
		rconn = std::move(__rconn);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DoRequest_implActor*>(this)->actor_wait_state > 0) static_cast<DoRequest_implActor*>(this)->actor_wait_state = 0;
		static_cast<DoRequest_implActor*>(this)->ActorCallback< DoRequest_implActor, 0, RESTConnectionPool::ReusableConnection >::remove();

	}
	void a_callback_fire(ActorCallback< DoRequest_implActor, 0, RESTConnectionPool::ReusableConnection >*,RESTConnectionPool::ReusableConnection const& value) 
	{
		fdb_probe_actor_enter("doRequest_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DoRequest_implActor, 0, RESTConnectionPool::ReusableConnection >*,RESTConnectionPool::ReusableConnection && value) 
	{
		fdb_probe_actor_enter("doRequest_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DoRequest_implActor, 0, RESTConnectionPool::ReusableConnection >*,Error err) 
	{
		fdb_probe_actor_enter("doRequest_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Reference<HTTP::IncomingResponse> const& _r,int loopDepth) 
	{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		r = _r;
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		if (r->data.headers["Connection"] != "close")
															#line 533 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		{
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			client->conectionPool->returnConnection(connectPoolKey, rconn, client->knobs.connection_pool_size);
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		}
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		rconn.conn.clear();
															#line 541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Reference<HTTP::IncomingResponse> && _r,int loopDepth) 
	{
															#line 148 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		r = _r;
															#line 152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		if (r->data.headers["Connection"] != "close")
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		{
															#line 153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			client->conectionPool->returnConnection(connectPoolKey, rconn, client->knobs.connection_pool_size);
															#line 556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		}
															#line 155 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		rconn.conn.clear();
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Reference<HTTP::IncomingResponse> const& _r,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_r, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Reference<HTTP::IncomingResponse> && _r,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_r), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DoRequest_implActor*>(this)->actor_wait_state > 0) static_cast<DoRequest_implActor*>(this)->actor_wait_state = 0;
		static_cast<DoRequest_implActor*>(this)->ActorCallback< DoRequest_implActor, 1, Reference<HTTP::IncomingResponse> >::remove();

	}
	void a_callback_fire(ActorCallback< DoRequest_implActor, 1, Reference<HTTP::IncomingResponse> >*,Reference<HTTP::IncomingResponse> const& value) 
	{
		fdb_probe_actor_enter("doRequest_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DoRequest_implActor, 1, Reference<HTTP::IncomingResponse> >*,Reference<HTTP::IncomingResponse> && value) 
	{
		fdb_probe_actor_enter("doRequest_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DoRequest_implActor, 1, Reference<HTTP::IncomingResponse> >*,Error err) 
	{
		fdb_probe_actor_enter("doRequest_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest_impl", reinterpret_cast<unsigned long>(this), 1);

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
	int a_body1loopBody1cont7(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont14(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont14(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont14(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont14(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<DoRequest_implActor*>(this)->actor_wait_state > 0) static_cast<DoRequest_implActor*>(this)->actor_wait_state = 0;
		static_cast<DoRequest_implActor*>(this)->ActorCallback< DoRequest_implActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DoRequest_implActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("doRequest_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest_impl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< DoRequest_implActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("doRequest_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest_impl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< DoRequest_implActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("doRequest_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("doRequest_impl", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	Reference<RESTClient> client;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	std::string verb;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	HTTP::Headers headers;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	RESTUrl url;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	std::set<unsigned int> successCodes;
															#line 96 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	Reference<HTTP::OutgoingRequest> req;
															#line 97 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	UnsentPacketQueue content;
															#line 120 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	int maxTries;
															#line 121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	int thisTry;
															#line 122 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	double nextRetryDelay;
															#line 123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	Reference<IRateControl> sendReceiveRate;
															#line 124 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	double reqTimeout;
															#line 125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	RESTConnectionPoolKey connectPoolKey;
															#line 126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	RESTClient::Stats* statsPtr;
															#line 129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	Optional<Error> err;
															#line 130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	Optional<NetworkAddress> remoteAddress;
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	bool connectionEstablished;
															#line 133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	Reference<HTTP::IncomingResponse> r;
															#line 141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	RESTConnectionPool::ReusableConnection rconn;
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
};
// This generated class is to be used only via doRequest_impl()
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
class DoRequest_implActor final : public Actor<Reference<HTTP::IncomingResponse>>, public ActorCallback< DoRequest_implActor, 0, RESTConnectionPool::ReusableConnection >, public ActorCallback< DoRequest_implActor, 1, Reference<HTTP::IncomingResponse> >, public ActorCallback< DoRequest_implActor, 2, Void >, public FastAllocated<DoRequest_implActor>, public DoRequest_implActorState<DoRequest_implActor> {
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
public:
	using FastAllocated<DoRequest_implActor>::operator new;
	using FastAllocated<DoRequest_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Reference<HTTP::IncomingResponse>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DoRequest_implActor, 0, RESTConnectionPool::ReusableConnection >;
friend struct ActorCallback< DoRequest_implActor, 1, Reference<HTTP::IncomingResponse> >;
friend struct ActorCallback< DoRequest_implActor, 2, Void >;
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	DoRequest_implActor(Reference<RESTClient> const& client,std::string const& verb,HTTP::Headers const& headers,RESTUrl const& url,std::set<unsigned int> const& successCodes) 
															#line 778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		 : Actor<Reference<HTTP::IncomingResponse>>(),
		   DoRequest_implActorState<DoRequest_implActor>(client, verb, headers, url, successCodes)
	{
		fdb_probe_actor_enter("doRequest_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("doRequest_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("doRequest_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DoRequest_implActor, 0, RESTConnectionPool::ReusableConnection >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DoRequest_implActor, 1, Reference<HTTP::IncomingResponse> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< DoRequest_implActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
[[nodiscard]] Future<Reference<HTTP::IncomingResponse>> doRequest_impl( Reference<RESTClient> const& client, std::string const& verb, HTTP::Headers const& headers, RESTUrl const& url, std::set<unsigned int> const& successCodes ) {
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	return Future<Reference<HTTP::IncomingResponse>>(new DoRequest_implActor(client, verb, headers, url, successCodes));
															#line 808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
}

#line 269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"

Future<Reference<HTTP::IncomingResponse>> RESTClient::doPutOrPost(const std::string& verb,
                                                                  Optional<HTTP::Headers> optHeaders,
                                                                  RESTUrl& url,
                                                                  std::set<unsigned int> successCodes) {
	HTTP::Headers headers;
	if (optHeaders.present()) {
		headers = optHeaders.get();
	}

	return doRequest_impl(Reference<RESTClient>::addRef(this), verb, headers, url, successCodes);
}

Future<Reference<HTTP::IncomingResponse>> RESTClient::doPost(const std::string& fullUrl,
                                                             const std::string& requestBody,
                                                             Optional<HTTP::Headers> optHeaders) {
	RESTUrl url(fullUrl, requestBody);
	TRACE_REST_OP("DoPost", url);
	return doPutOrPost(HTTP::HTTP_VERB_POST, optHeaders, url, { HTTP::HTTP_STATUS_CODE_OK });
}

Future<Reference<HTTP::IncomingResponse>> RESTClient::doPut(const std::string& fullUrl,
                                                            const std::string& requestBody,
                                                            Optional<HTTP::Headers> optHeaders) {
	RESTUrl url(fullUrl, requestBody);
	TRACE_REST_OP("DoPut", url);
	return doPutOrPost(
	    HTTP::HTTP_VERB_PUT,
	    optHeaders,
	    url,
	    // 201 - on successful resource create
	    // 200 / 204 - if target resource representation was successfully modified with the desired state
	    { HTTP::HTTP_STATUS_CODE_OK, HTTP::HTTP_STATUS_CODE_CREATED, HTTP::HTTP_STATUS_CODE_NO_CONTENT });
}

Future<Reference<HTTP::IncomingResponse>> RESTClient::doGetHeadDeleteOrTrace(const std::string& verb,
                                                                             Optional<HTTP::Headers> optHeaders,
                                                                             RESTUrl& url,
                                                                             std::set<unsigned int> successCodes) {
	HTTP::Headers headers;
	if (optHeaders.present()) {
		headers = optHeaders.get();
	}

	return doRequest_impl(Reference<RESTClient>::addRef(this), HTTP::HTTP_VERB_GET, headers, url, successCodes);
}

Future<Reference<HTTP::IncomingResponse>> RESTClient::doGet(const std::string& fullUrl,
                                                            Optional<HTTP::Headers> optHeaders) {
	RESTUrl url(fullUrl);
	TRACE_REST_OP("DoGet", url);
	return doGetHeadDeleteOrTrace(HTTP::HTTP_VERB_GET, optHeaders, url, { HTTP::HTTP_STATUS_CODE_OK });
}

Future<Reference<HTTP::IncomingResponse>> RESTClient::doHead(const std::string& fullUrl,
                                                             Optional<HTTP::Headers> optHeaders) {
	RESTUrl url(fullUrl);
	TRACE_REST_OP("DoHead", url);
	return doGetHeadDeleteOrTrace(HTTP::HTTP_VERB_HEAD, optHeaders, url, { HTTP::HTTP_STATUS_CODE_OK });
}

Future<Reference<HTTP::IncomingResponse>> RESTClient::doDelete(const std::string& fullUrl,
                                                               Optional<HTTP::Headers> optHeaders) {
	RESTUrl url(fullUrl);
	TRACE_REST_OP("DoDelete", url);
	return doGetHeadDeleteOrTrace(
	    HTTP::HTTP_VERB_DELETE,
	    optHeaders,
	    url,
	    // 200 - action has been enacted.
	    // 202 - action will likely succeed, but, has not yet been enacted.
	    // 204 - action has been enated, no further information is to supplied.
	    { HTTP::HTTP_STATUS_CODE_OK, HTTP::HTTP_STATUS_CODE_NO_CONTENT, HTTP::HTTP_STATUS_CODE_ACCEPTED });
}

Future<Reference<HTTP::IncomingResponse>> RESTClient::doTrace(const std::string& fullUrl,
                                                              Optional<HTTP::Headers> optHeaders) {
	RESTUrl url(fullUrl);
	TRACE_REST_OP("DoTrace", url);
	return doGetHeadDeleteOrTrace(HTTP::HTTP_VERB_TRACE, optHeaders, url, { HTTP::HTTP_STATUS_CODE_OK });
}

// Only used to link unit tests
void forceLinkRESTClientTests() {}

															#line 897 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase354()
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
template <class FlowTestCase354Actor>
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
class FlowTestCase354ActorState {
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
public:
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	FlowTestCase354ActorState(UnitTestParameters const& params) 
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		 : params(params)
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase354", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase354ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase354", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 355 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			RESTClient r;
															#line 356 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			std::unordered_map<std::string, int> knobs = r.getKnobs();
															#line 357 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			ASSERT_EQ(knobs["connection_pool_size"], FLOW_KNOBS->RESTCLIENT_MAX_CONNECTIONPOOL_SIZE);
															#line 358 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			ASSERT_EQ(knobs["connect_tries"], FLOW_KNOBS->RESTCLIENT_CONNECT_TRIES);
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			ASSERT_EQ(knobs["connect_timeout"], FLOW_KNOBS->RESTCLIENT_CONNECT_TIMEOUT);
															#line 360 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			ASSERT_EQ(knobs["max_connection_life"], FLOW_KNOBS->RESTCLIENT_MAX_CONNECTION_LIFE);
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			ASSERT_EQ(knobs["request_tries"], FLOW_KNOBS->RESTCLIENT_REQUEST_TRIES);
															#line 362 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			ASSERT_EQ(knobs["request_timeout_secs"], FLOW_KNOBS->RESTCLIENT_REQUEST_TIMEOUT_SEC);
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			for( auto& itr : knobs ) {
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				itr.second++;
															#line 944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			}
															#line 367 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			r.setKnobs(knobs);
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			std::unordered_map<std::string, int> updated = r.getKnobs();
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			for( auto& itr : updated ) {
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				ASSERT_EQ(knobs[itr.first], itr.second);
															#line 954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			}
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			knobs["foo"] = 100;
															#line 958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			try {
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				r.setKnobs(knobs);
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				ASSERT(false);
															#line 964 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
				loopDepth = a_body1cont5(loopDepth);
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
		this->~FlowTestCase354ActorState();
		static_cast<FlowTestCase354Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
		if (!static_cast<FlowTestCase354Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase354ActorState(); static_cast<FlowTestCase354Actor*>(this)->destroy(); return 0; }
															#line 993 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		new (&static_cast<FlowTestCase354Actor*>(this)->SAV< Void >::value()) Void(Void());
		this->~FlowTestCase354ActorState();
		static_cast<FlowTestCase354Actor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
			if (e.code() != error_code_rest_invalid_rest_client_knob)
															#line 1006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
			{
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 1010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
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
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	UnitTestParameters params;
															#line 1037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase354()
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
class FlowTestCase354Actor final : public Actor<Void>, public FastAllocated<FlowTestCase354Actor>, public FlowTestCase354ActorState<FlowTestCase354Actor> {
															#line 1042 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase354Actor>::operator new;
	using FastAllocated<FlowTestCase354Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	FlowTestCase354Actor(UnitTestParameters const& params) 
															#line 1052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase354ActorState<FlowTestCase354Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase354", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase354");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase354", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
}
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
static Future<Void> flowTestCase354( UnitTestParameters const& params ) {
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
	return Future<Void>(new FlowTestCase354Actor(params));
															#line 1079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase354, "fdbrpc/RESTClient")

#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/RESTClient.actor.cpp"
