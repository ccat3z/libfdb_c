#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"
/*
 * AuditUtils.actor.h
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

#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_AUDITUTILS_ACTOR_G_H)
#define FDBCLIENT_AUDITUTILS_ACTOR_G_H
#include "fdbclient/AuditUtils.actor.g.h"
#elif !defined(FDBCLIENT_AUDITUTILS_ACTOR_H)
#define FDBCLIENT_AUDITUTILS_ACTOR_H
#pragma once

#include "fdbclient/Audit.h"
#include "fdbclient/FDBTypes.h"
#include "fdbclient/NativeAPI.actor.h"
#include "fdbrpc/fdbrpc.h"

#include "flow/actorcompiler.h" // has to be last include

struct MoveKeyLockInfo {
	UID prevOwner, myOwner, prevWrite;
};

															#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<Void> cancelAuditMetadata( Database const& cx, AuditType const& auditType, UID const& auditId );

#line 40 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"
															#line 45 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<UID> persistNewAuditState( Database const& cx, AuditStorageState const& auditState, MoveKeyLockInfo const& lock, bool const& ddEnabled );

#line 41 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"
															#line 49 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<Void> persistAuditState( Database const& cx, AuditStorageState const& auditState, std::string const& context, MoveKeyLockInfo const& lock, bool const& ddEnabled );

#line 46 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"
															#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<AuditStorageState> getAuditState( Database const& cx, AuditType const& type, UID const& id );

#line 47 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"
															#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<std::vector<AuditStorageState>> getAuditStates( Database const& cx, AuditType const& auditType, bool const& newFirst, Optional<int> const& num = Optional<int>(), Optional<AuditPhase> const& phase = Optional<AuditPhase>() );

#line 52 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"
															#line 61 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<Void> persistAuditStateByRange( Database const& cx, AuditStorageState const& auditState );

#line 53 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"
															#line 65 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<std::vector<AuditStorageState>> getAuditStateByRange( Database const& cx, AuditType const& type, UID const& auditId, KeyRange const& range );

#line 57 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"
															#line 69 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<Void> persistAuditStateByServer( Database const& cx, AuditStorageState const& auditState );

#line 58 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"
															#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<std::vector<AuditStorageState>> getAuditStateByServer( Database const& cx, AuditType const& type, UID const& auditId, UID const& auditServerId, KeyRange const& range );

#line 63 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"
															#line 77 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<Void> clearAuditMetadataForType( Database const& cx, AuditType const& auditType, UID const& maxAuditIdToClear, int const& numFinishAuditToKeep );

#line 67 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"
															#line 81 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<bool> checkStorageServerRemoved( Database const& cx, UID const& ssid );

#line 68 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"
AuditPhase stringToAuditPhase(std::string auditPhaseStr);
															#line 86 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<bool> checkAuditProgressCompleteByRange( Database const& cx, AuditType const& auditType, UID const& auditId, KeyRange const& auditRange );

#line 73 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"
															#line 90 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<bool> checkAuditProgressCompleteByServer( Database const& cx, AuditType const& auditType, UID const& auditId, KeyRange const& auditRange, UID const& serverId, std::shared_ptr<AsyncVar<int>> const& checkProgressBudget );

#line 79 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"
															#line 94 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<std::vector<AuditStorageState>> initAuditMetadata( Database const& cx, MoveKeyLockInfo const& lock, bool const& ddEnabled, UID const& dataDistributorId, int const& persistFinishAuditCount );

#line 84 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"

struct AuditGetServerKeysRes {
	KeyRange completeRange;
	Version readAtVersion;
	UID serverId;
	std::vector<KeyRange> ownRanges;
	int64_t readBytes;
	AuditGetServerKeysRes() = default;
	AuditGetServerKeysRes(KeyRange completeRange,
	                      Version readAtVersion,
	                      UID serverId,
	                      std::vector<KeyRange> ownRanges,
	                      int64_t readBytes)
	  : completeRange(completeRange), readAtVersion(readAtVersion), serverId(serverId), ownRanges(ownRanges),
	    readBytes(readBytes) {}
};

struct AuditGetKeyServersRes {
	KeyRange completeRange;
	Version readAtVersion;
	int64_t readBytes;
	std::unordered_map<UID, std::vector<KeyRange>> rangeOwnershipMap;
	AuditGetKeyServersRes() = default;
	AuditGetKeyServersRes(KeyRange completeRange,
	                      Version readAtVersion,
	                      std::unordered_map<UID, std::vector<KeyRange>> rangeOwnershipMap,
	                      int64_t readBytes)
	  : completeRange(completeRange), readAtVersion(readAtVersion), rangeOwnershipMap(rangeOwnershipMap),
	    readBytes(readBytes) {}
};

std::vector<KeyRange> coalesceRangeList(std::vector<KeyRange> ranges);
Optional<std::pair<KeyRange, KeyRange>> rangesSame(std::vector<KeyRange> rangesA, std::vector<KeyRange> rangesB);
															#line 131 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<AuditGetServerKeysRes> getThisServerKeysFromServerKeys( UID const& serverID, Transaction* const& tr, KeyRange const& range );

#line 118 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"
															#line 135 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.g.h"
[[nodiscard]] Future<AuditGetKeyServersRes> getShardMapFromKeyServers( UID const& auditServerId, Transaction* const& tr, KeyRange const& range );

#line 119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/include/fdbclient/AuditUtils.actor.h"

#include "flow/unactorcompiler.h"
#endif
