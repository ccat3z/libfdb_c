#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbrpc/HealthMonitor.actor.cpp"
/*
 * HealthMonitor.actor.cpp
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

#include "fdbrpc/FailureMonitor.h"
#include "fdbrpc/FlowTransport.h"
#include "fdbrpc/HealthMonitor.h"

void HealthMonitor::reportPeerClosed(const NetworkAddress& peerAddress) {
	purgeOutdatedHistory();
	peerClosedHistory.emplace_back(now(), peerAddress);
	peerClosedNum[peerAddress] += 1;
}

void HealthMonitor::purgeOutdatedHistory() {
	while (!peerClosedHistory.empty()) {
		auto const& p = peerClosedHistory.front();
		if (p.first < now() - FLOW_KNOBS->HEALTH_MONITOR_CLIENT_REQUEST_INTERVAL_SECS) {
			auto& count = peerClosedNum[p.second];
			--count;
			ASSERT(count >= 0);
			if (count == 0) {
				peerClosedNum.erase(p.second);
			}
			peerClosedHistory.pop_front();
		} else {
			break;
		}
	}
}

bool HealthMonitor::tooManyConnectionsClosed(const NetworkAddress& peerAddress) {
	purgeOutdatedHistory();
	if (peerClosedNum.find(peerAddress) == peerClosedNum.end()) {
		return false;
	}
	return peerClosedNum[peerAddress] > FLOW_KNOBS->HEALTH_MONITOR_CONNECTION_MAX_CLOSED;
}

int HealthMonitor::closedConnectionsCount(const NetworkAddress& peerAddress) {
	purgeOutdatedHistory();
	if (peerClosedNum.find(peerAddress) == peerClosedNum.end()) {
		return 0;
	}
	return peerClosedNum[peerAddress];
}

std::unordered_set<NetworkAddress> HealthMonitor::getRecentClosedPeers() {
	purgeOutdatedHistory();
	std::unordered_set<NetworkAddress> closedPeers;
	for (const auto& [peerAddr, count] : peerClosedNum) {
		if (count > 0) {
			closedPeers.insert(peerAddr);
		}
	}
	return closedPeers;
}
