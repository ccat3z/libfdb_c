#define POST_ACTOR_COMPILER 1
#line 1 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
/*
 * BackupAgentBase.actor.cpp
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

#include <iomanip>
#include <time.h>

#include "fdbclient/BackupAgent.actor.h"
#include "fdbclient/BlobCipher.h"
#include "fdbclient/CommitProxyInterface.h"
#include "fdbclient/CommitTransaction.h"
#include "fdbclient/FDBTypes.h"
#include "fdbclient/GetEncryptCipherKeys.h"
#include "fdbclient/DatabaseContext.h"
#include "fdbclient/ManagementAPI.actor.h"
#include "fdbclient/MetaclusterRegistration.h"
#include "fdbclient/SystemData.h"
#include "fdbclient/TenantManagement.actor.h"
#include "fdbrpc/simulator.h"
#include "flow/ActorCollection.h"
#include "flow/network.h"

#include "flow/actorcompiler.h" // has to be last include

std::string BackupAgentBase::formatTime(int64_t epochs) {
	time_t curTime = (time_t)epochs;
	char buffer[30];
	struct tm timeinfo;
	getLocalTime(&curTime, &timeinfo);
	strftime(buffer, 30, "%Y/%m/%d.%H:%M:%S%z", &timeinfo);
	return buffer;
}

int64_t BackupAgentBase::parseTime(std::string timestamp) {
	struct tm out;
	out.tm_isdst = -1; // This field is not set by strptime. -1 tells mktime to determine whether DST is in effect

	std::string timeOnly = timestamp.substr(0, 19);

	// TODO:  Use std::get_time implementation for all platforms once supported
	// It would be nice to read the timezone using %z, but it seems not all get_time()
	// or strptime() implementations handle it correctly in all environments so we
	// will read the date and time independent of timezone at first and then adjust it.
#ifdef _WIN32
	std::istringstream s(timeOnly);
	s.imbue(std::locale(setlocale(LC_TIME, nullptr)));
	s >> std::get_time(&out, "%Y/%m/%d.%H:%M:%S");
	if (s.fail()) {
		return -1;
	}
#else
	if (strptime(timeOnly.c_str(), "%Y/%m/%d.%H:%M:%S", &out) == nullptr) {
		return -1;
	}
#endif

	// Read timezone offset in +/-HHMM format then convert to seconds
	int tzHH;
	int tzMM;
	if (sscanf(timestamp.substr(19, 5).c_str(), "%3d%2d", &tzHH, &tzMM) != 2) {
		return -1;
	}
	if (tzHH < 0) {
		tzMM = -tzMM;
	}
	// tzOffset is the number of seconds EAST of GMT
	int tzOffset = tzHH * 60 * 60 + tzMM * 60;

	// The goal is to convert the timestamp string to epoch seconds assuming the date/time was expressed in the timezone
	// at the end of the string. However, mktime() will ONLY return epoch seconds assuming the date/time is expressed in
	// local time (based on locale / environment) mktime() will set out.tm_gmtoff when available
	int64_t ts = mktime(&out);

	// localTZOffset is the number of seconds EAST of GMT
	long localTZOffset;
#ifdef _WIN32
	// _get_timezone() returns the number of seconds WEST of GMT
	if (_get_timezone(&localTZOffset) != 0) {
		return -1;
	}
	// Negate offset to match the orientation of tzOffset
	localTZOffset = -localTZOffset;
#else
	// tm.tm_gmtoff is the number of seconds EAST of GMT
	localTZOffset = out.tm_gmtoff;
#endif

	// Add back the difference between the local timezone assumed by mktime() and the intended timezone from the input
	// string
	ts += (localTZOffset - tzOffset);
	return ts;
}

const Key BackupAgentBase::keyFolderId = "config_folderid"_sr;
const Key BackupAgentBase::keyBeginVersion = "beginVersion"_sr;
const Key BackupAgentBase::keyEndVersion = "endVersion"_sr;
const Key BackupAgentBase::keyPrevBeginVersion = "prevBeginVersion"_sr;
const Key BackupAgentBase::keyConfigBackupTag = "config_backup_tag"_sr;
const Key BackupAgentBase::keyConfigLogUid = "config_log_uid"_sr;
const Key BackupAgentBase::keyConfigBackupRanges = "config_backup_ranges"_sr;
const Key BackupAgentBase::keyConfigStopWhenDoneKey = "config_stop_when_done"_sr;
const Key BackupAgentBase::keyStateStop = "state_stop"_sr;
const Key BackupAgentBase::keyStateStatus = "state_status"_sr;
const Key BackupAgentBase::keyStateLogBeginVersion = "last_begin_version"_sr;
const Key BackupAgentBase::keyLastUid = "last_uid"_sr;
const Key BackupAgentBase::keyBeginKey = "beginKey"_sr;
const Key BackupAgentBase::keyEndKey = "endKey"_sr;
const Key BackupAgentBase::keyDrVersion = "drVersion"_sr;
const Key BackupAgentBase::destUid = "destUid"_sr;
const Key BackupAgentBase::backupStartVersion = "backupStartVersion"_sr;

const Key BackupAgentBase::keyTagName = "tagname"_sr;
const Key BackupAgentBase::keyStates = "state"_sr;
const Key BackupAgentBase::keyConfig = "config"_sr;
const Key BackupAgentBase::keyErrors = "errors"_sr;
const Key BackupAgentBase::keyRanges = "ranges"_sr;
const Key BackupAgentBase::keyTasks = "tasks"_sr;
const Key BackupAgentBase::keyFutures = "futures"_sr;
const Key BackupAgentBase::keySourceStates = "source_states"_sr;
const Key BackupAgentBase::keySourceTagName = "source_tagname"_sr;

bool copyParameter(Reference<Task> source, Reference<Task> dest, Key key) {
	if (source) {
		dest->params[key] = source->params[key];
		return true;
	}

	return false;
}

Version getVersionFromString(std::string const& value) {
	Version version = invalidVersion;
	int n = 0;
	if (sscanf(value.c_str(), "%lld%n", (long long*)&version, &n) != 1 || n != value.size()) {
		TraceEvent(SevWarnAlways, "GetVersionFromString").detail("InvalidVersion", value);
		throw restore_invalid_version();
	}
	return version;
}

// Transaction log data is stored by the FoundationDB core in the
// "backupLogKeys" (i.e., \xff\x02/blog/) keyspace in a funny order for
// performance reasons.
// Returns the ranges of keys that contain the data for the given range
// of versions.
// assert CLIENT_KNOBS->LOG_RANGE_BLOCK_SIZE % blocksize = 0. Otherwise calculation of hash will be incorrect
Standalone<VectorRef<KeyRangeRef>> getLogRanges(Version beginVersion,
                                                Version endVersion,
                                                Key destUidValue,
                                                int blockSize) {
	Standalone<VectorRef<KeyRangeRef>> ret;

	Key baLogRangePrefix = destUidValue.withPrefix(backupLogKeys.begin);

	//TraceEvent("GetLogRanges").detail("DestUidValue", destUidValue).detail("Prefix", baLogRangePrefix);

	for (int64_t vblock = beginVersion / blockSize; vblock < (endVersion + blockSize - 1) / blockSize; ++vblock) {
		int64_t tb = vblock * blockSize / CLIENT_KNOBS->LOG_RANGE_BLOCK_SIZE;
		uint64_t bv = bigEndian64(std::max(beginVersion, vblock * blockSize));
		uint64_t ev = bigEndian64(std::min(endVersion, (vblock + 1) * blockSize));
		uint32_t data = tb & 0xffffffff;
		uint8_t hash = (uint8_t)hashlittle(&data, sizeof(uint32_t), 0);

		Key vblockPrefix = StringRef(&hash, sizeof(uint8_t)).withPrefix(baLogRangePrefix);

		ret.push_back_deep(ret.arena(),
		                   KeyRangeRef(StringRef((uint8_t*)&bv, sizeof(uint64_t)).withPrefix(vblockPrefix),
		                               StringRef((uint8_t*)&ev, sizeof(uint64_t)).withPrefix(vblockPrefix)));
	}

	return ret;
}

Standalone<VectorRef<KeyRangeRef>> getApplyRanges(Version beginVersion, Version endVersion, Key backupUid) {
	Standalone<VectorRef<KeyRangeRef>> ret;

	Key baLogRangePrefix = backupUid.withPrefix(applyLogKeys.begin);

	//TraceEvent("GetLogRanges").detail("BackupUid", backupUid).detail("Prefix", baLogRangePrefix);

	for (int64_t vblock = beginVersion / CLIENT_KNOBS->APPLY_BLOCK_SIZE;
	     vblock < (endVersion + CLIENT_KNOBS->APPLY_BLOCK_SIZE - 1) / CLIENT_KNOBS->APPLY_BLOCK_SIZE;
	     ++vblock) {
		int64_t tb = vblock * CLIENT_KNOBS->APPLY_BLOCK_SIZE / CLIENT_KNOBS->LOG_RANGE_BLOCK_SIZE;
		uint64_t bv = bigEndian64(std::max(beginVersion, vblock * CLIENT_KNOBS->APPLY_BLOCK_SIZE));
		uint64_t ev = bigEndian64(std::min(endVersion, (vblock + 1) * CLIENT_KNOBS->APPLY_BLOCK_SIZE));
		uint32_t data = tb & 0xffffffff;
		uint8_t hash = (uint8_t)hashlittle(&data, sizeof(uint32_t), 0);

		Key vblockPrefix = StringRef(&hash, sizeof(uint8_t)).withPrefix(baLogRangePrefix);

		ret.push_back_deep(ret.arena(),
		                   KeyRangeRef(StringRef((uint8_t*)&bv, sizeof(uint64_t)).withPrefix(vblockPrefix),
		                               StringRef((uint8_t*)&ev, sizeof(uint64_t)).withPrefix(vblockPrefix)));
	}

	return ret;
}

Key getApplyKey(Version version, Key backupUid) {
	int64_t vblock = (version - 1) / CLIENT_KNOBS->LOG_RANGE_BLOCK_SIZE;
	uint64_t v = bigEndian64(version);
	uint32_t data = vblock & 0xffffffff;
	uint8_t hash = (uint8_t)hashlittle(&data, sizeof(uint32_t), 0);
	Key k1 = StringRef((uint8_t*)&v, sizeof(uint64_t)).withPrefix(StringRef(&hash, sizeof(uint8_t)));
	Key k2 = k1.withPrefix(backupUid);
	return k2.withPrefix(applyLogKeys.begin);
}

Key getLogKey(Version version, Key backupUid) {
	int64_t vblock = (version - 1) / CLIENT_KNOBS->LOG_RANGE_BLOCK_SIZE;
	uint64_t v = bigEndian64(version);
	uint32_t data = vblock & 0xffffffff;
	uint8_t hash = (uint8_t)hashlittle(&data, sizeof(uint32_t), 0);
	Key k1 = StringRef((uint8_t*)&v, sizeof(uint64_t)).withPrefix(StringRef(&hash, sizeof(uint8_t)));
	Key k2 = k1.withPrefix(backupUid);
	return k2.withPrefix(backupLogKeys.begin);
}

Version getLogKeyVersion(Key key) {
	return bigEndian64(*(int64_t*)(key.begin() + backupLogPrefixBytes + sizeof(UID) + sizeof(uint8_t)));
}

bool validTenantAccess(std::map<int64_t, TenantName>* tenantMap,
                       MutationRef m,
                       bool provisionalProxy,
                       Version version) {
	if (isSystemKey(m.param1)) {
		return true;
	}
	int64_t tenantId = TenantInfo::INVALID_TENANT;
	if (m.isEncrypted()) {
		tenantId = m.encryptDomainId();
	} else {
		tenantId = TenantAPI::extractTenantIdFromMutation(m);
	}
	ASSERT(tenantMap != nullptr);
	if (m.isEncrypted() && isReservedEncryptDomain(tenantId)) {
		// These are valid encrypt domains so don't check the tenant map
	} else if (tenantMap->find(tenantId) == tenantMap->end()) {
		// If a tenant is not found for a given mutation then exclude it from the batch
		ASSERT(!provisionalProxy);
		TraceEvent(SevWarnAlways, "MutationLogRestoreTenantNotFound")
		    .detail("Version", version)
		    .detail("TenantId", tenantId);
		CODE_PROBE(true, "mutation log restore tenant not found");
		return false;
	}
	return true;
}

// Given a key from one of the ranges returned by get_log_ranges,
// returns(version, part) where version is the database version number of
// the transaction log data in the value, and part is 0 for the first such
// data for a given version, 1 for the second block of data, etc.
std::pair<Version, uint32_t> decodeBKMutationLogKey(Key key) {
	return std::make_pair(
	    getLogKeyVersion(key),
	    bigEndian32(*(int32_t*)(key.begin() + backupLogPrefixBytes + sizeof(UID) + sizeof(uint8_t) + sizeof(int64_t))));
}

void _addResult(bool* tenantMapChanging,
                VectorRef<MutationRef>* result,
                int* mutationSize,
                Arena* arena,
                MutationRef logValue,
                KeyRangeRef tenantMapRange) {
	*tenantMapChanging = *tenantMapChanging || TenantAPI::tenantMapChanging(logValue, tenantMapRange);
	result->push_back_deep(*arena, logValue);
	*mutationSize += logValue.expectedSize();
}

/*
 This actor is responsible for taking an original transaction which was added to the backup mutation log (represented
 by "value" parameter), breaking it up into the individual MutationRefs (that constitute the transaction), decrypting
 each mutation (if needed) and adding/removing prefixes from the mutations. The final mutations are then added to the
 "result" vector alongside their encrypted counterparts (which is added to the "encryptedResult" vector)
*/
															#line 297 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via decodeBackupLogValue()
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
template <class DecodeBackupLogValueActor>
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class DecodeBackupLogValueActorState {
															#line 304 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	DecodeBackupLogValueActorState(Arena* const& arena,VectorRef<MutationRef>* const& result,VectorRef<Optional<MutationRef>>* const& encryptedResult,int* const& mutationSize,bool* const& tenantMapChanging,Standalone<StringRef> const& value,Key const& addPrefix,Key const& removePrefix,Version const& version,Reference<KeyRangeMap<Version>> const& key_version,Database const& cx,std::map<int64_t, TenantName>* const& tenantMap,bool const& provisionalProxy) 
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		 : arena(arena),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   result(result),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   encryptedResult(encryptedResult),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   mutationSize(mutationSize),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   tenantMapChanging(tenantMapChanging),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   value(value),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   addPrefix(addPrefix),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   removePrefix(removePrefix),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   version(version),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   key_version(key_version),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   cx(cx),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   tenantMap(tenantMap),
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   provisionalProxy(provisionalProxy)
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
	{
		fdb_probe_actor_create("decodeBackupLogValue", reinterpret_cast<unsigned long>(this));

	}
	~DecodeBackupLogValueActorState() 
	{
		fdb_probe_actor_destroy("decodeBackupLogValue", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				offset = uint64_t(0);
															#line 310 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				uint64_t protocolVersion = 0;
															#line 311 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				memcpy(&protocolVersion, value.begin(), sizeof(uint64_t));
															#line 312 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				offset += sizeof(uint64_t);
															#line 313 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (protocolVersion <= 0x0FDB00A200090001)
															#line 359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 314 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					TraceEvent(SevError, "DecodeBackupLogValue") .detail("IncompatibleProtocolVersion", protocolVersion) .detail("ValueSize", value.size()) .detail("Value", value);
															#line 318 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					return a_body1Catch2(incompatible_protocol_version(), loopDepth);
															#line 365 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				totalBytes = 0;
															#line 322 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				memcpy(&totalBytes, value.begin() + offset, sizeof(uint32_t));
															#line 323 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				offset += sizeof(uint32_t);
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				consumed = 0;
															#line 326 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (totalBytes + offset > value.size())
															#line 377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 327 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					return a_body1Catch2(restore_missing_data(), loopDepth);
															#line 381 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				originalOffset = offset;
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				StrictFuture<DatabaseConfiguration> __when_expr_0 = getDatabaseConfiguration(cx);
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (static_cast<DecodeBackupLogValueActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 389 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<DecodeBackupLogValueActor*>(this)->actor_wait_state = 1;
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DecodeBackupLogValueActor, 0, DatabaseConfiguration >*>(static_cast<DecodeBackupLogValueActor*>(this)));
															#line 394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
		this->~DecodeBackupLogValueActorState();
		static_cast<DecodeBackupLogValueActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 490 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!static_cast<DecodeBackupLogValueActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DecodeBackupLogValueActorState(); static_cast<DecodeBackupLogValueActor*>(this)->destroy(); return 0; }
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		new (&static_cast<DecodeBackupLogValueActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DecodeBackupLogValueActorState();
		static_cast<DecodeBackupLogValueActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 483 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			TraceEvent(e.code() == error_code_restore_missing_data ? SevWarn : SevError, "BA_DecodeBackupLogValue") .error(e) .GetLastError() .detail("ValueSize", value.size()) .detail("Value", value);
															#line 488 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 438 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		tenantMapRange = TenantMetadata::tenantMap().subspace;
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		;
															#line 454 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(DatabaseConfiguration const& __config,int loopDepth) 
	{
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		config = __config;
															#line 463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1when1(DatabaseConfiguration && __config,int loopDepth) 
	{
		config = std::move(__config);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DecodeBackupLogValueActor*>(this)->actor_wait_state > 0) static_cast<DecodeBackupLogValueActor*>(this)->actor_wait_state = 0;
		static_cast<DecodeBackupLogValueActor*>(this)->ActorCallback< DecodeBackupLogValueActor, 0, DatabaseConfiguration >::remove();

	}
	void a_callback_fire(ActorCallback< DecodeBackupLogValueActor, 0, DatabaseConfiguration >*,DatabaseConfiguration const& value) 
	{
		fdb_probe_actor_enter("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DecodeBackupLogValueActor, 0, DatabaseConfiguration >*,DatabaseConfiguration && value) 
	{
		fdb_probe_actor_enter("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DecodeBackupLogValueActor, 0, DatabaseConfiguration >*,Error err) 
	{
		fdb_probe_actor_enter("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont5(int loopDepth) 
	{
															#line 472 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		ASSERT(consumed == totalBytes);
															#line 473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (value.size() != offset)
															#line 532 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 474 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			TraceEvent(SevError, "BA_DecodeBackupLogValue") .detail("UnexpectedExtraDataSize", value.size()) .detail("Offset", offset) .detail("TotalBytes", totalBytes) .detail("Consumed", consumed) .detail("OriginalOffset", originalOffset);
															#line 480 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			return a_body1Catch2(restore_corrupted_data(), loopDepth);
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		}
		loopDepth = a_body1cont8(loopDepth);

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
															#line 333 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!(consumed < totalBytes))
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
			return a_body1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		uint32_t type = 0;
															#line 335 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		memcpy(&type, value.begin() + offset, sizeof(uint32_t));
															#line 336 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		offset += sizeof(uint32_t);
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		len1 = 0;
															#line 338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		memcpy(&len1, value.begin() + offset, sizeof(uint32_t));
															#line 339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		offset += sizeof(uint32_t);
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		len2 = 0;
															#line 341 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		memcpy(&len2, value.begin() + offset, sizeof(uint32_t));
															#line 342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		offset += sizeof(uint32_t);
															#line 344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		ASSERT(offset + len1 + len2 <= value.size() && isValidMutationType(type));
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		logValue = MutationRef();
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		tempArena = Arena();
															#line 348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		logValue.type = type;
															#line 349 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		logValue.param1 = value.substr(offset, len1);
															#line 350 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		offset += len1;
															#line 351 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		logValue.param2 = value.substr(offset, len2);
															#line 352 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		offset += len2;
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		encryptedLogValue = Optional<MutationRef>();
															#line 354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		ASSERT(!config.encryptionAtRestMode.isEncryptionEnabled() || logValue.isEncrypted());
															#line 361 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (config.tenantMode == TenantMode::REQUIRED && config.encryptionAtRestMode.mode != EncryptionAtRestMode::CLUSTER_AWARE && !validTenantAccess(tenantMap, logValue, provisionalProxy, version))
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			consumed += BackupAgentBase::logHeaderSize + len1 + len2;
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			return a_body1cont2loopHead1(loopDepth); // continue
		}
															#line 369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (logValue.isEncrypted())
															#line 608 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 370 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			encryptedLogValue = logValue;
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			domainId = logValue.encryptDomainId();
															#line 372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			Reference<AsyncVar<ClientDBInfo> const> dbInfo = cx->clientInfo;
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			try {
															#line 374 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (CLIENT_KNOBS->ENABLE_CONFIGURABLE_ENCRYPTION)
															#line 620 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					StrictFuture<TextAndHeaderCipherKeys> __when_expr_1 = GetEncryptCipherKeys<ClientDBInfo>::getEncryptCipherKeys( dbInfo, logValue.configurableEncryptionHeader(), BlobCipherMetrics::RESTORE);
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					if (static_cast<DecodeBackupLogValueActor*>(this)->actor_wait_state < 0) return a_body1cont2loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont2loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont2loopBody1when1(__when_expr_1.get(), loopDepth); };
					static_cast<DecodeBackupLogValueActor*>(this)->actor_wait_state = 2;
															#line 375 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DecodeBackupLogValueActor, 1, TextAndHeaderCipherKeys >*>(static_cast<DecodeBackupLogValueActor*>(this)));
															#line 631 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					loopDepth = 0;
				}
				else
				{
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					StrictFuture<TextAndHeaderCipherKeys> __when_expr_2 = GetEncryptCipherKeys<ClientDBInfo>::getEncryptCipherKeys( dbInfo, *logValue.encryptionHeader(), BlobCipherMetrics::RESTORE);
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					if (static_cast<DecodeBackupLogValueActor*>(this)->actor_wait_state < 0) return a_body1cont2loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont2loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2loopBody1when2(__when_expr_2.get(), loopDepth); };
					static_cast<DecodeBackupLogValueActor*>(this)->actor_wait_state = 3;
															#line 380 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DecodeBackupLogValueActor, 2, TextAndHeaderCipherKeys >*>(static_cast<DecodeBackupLogValueActor*>(this)));
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					loopDepth = 0;
				}
			}
			catch (Error& error) {
				loopDepth = a_body1cont2loopBody1Catch1(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1cont2loopBody1Catch1(unknown_error(), loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1cont2loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2break1(int loopDepth) 
	{
		try {
			return a_body1cont5(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2loopBody1cont1(int loopDepth) 
	{
															#line 400 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		ASSERT(!logValue.isEncrypted());
															#line 403 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (config.tenantMode == TenantMode::REQUIRED && config.encryptionAtRestMode.mode == EncryptionAtRestMode::CLUSTER_AWARE && !validTenantAccess(tenantMap, logValue, provisionalProxy, version))
															#line 681 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			consumed += BackupAgentBase::logHeaderSize + len1 + len2;
															#line 685 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			return a_body1cont2loopHead1(loopDepth); // continue
		}
															#line 410 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		MutationRef originalLogValue = logValue;
															#line 412 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (logValue.type == MutationRef::ClearRange)
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 413 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			KeyRangeRef range(logValue.param1, logValue.param2);
															#line 414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			auto ranges = key_version->intersectingRanges(range);
															#line 415 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			for( auto r : ranges ) {
															#line 416 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (version > r.value() && r.value() != invalidVersion)
															#line 702 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					KeyRef minKey = std::min(r.range().end, range.end);
															#line 418 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					if (minKey == (removePrefix == StringRef() ? allKeys.end : strinc(removePrefix)))
															#line 708 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					{
															#line 419 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						logValue.param1 = std::max(r.range().begin, range.begin);
															#line 420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						if (removePrefix.size())
															#line 714 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
						{
															#line 421 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
							logValue.param1 = logValue.param1.removePrefix(removePrefix);
															#line 718 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
						}
															#line 423 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						if (addPrefix.size())
															#line 722 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
						{
															#line 424 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
							logValue.param1 = logValue.param1.withPrefix(addPrefix, tempArena);
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
						}
															#line 426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						logValue.param2 = addPrefix == StringRef() ? allKeys.end : strinc(addPrefix, tempArena);
															#line 427 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						_addResult(tenantMapChanging, result, mutationSize, arena, logValue, tenantMapRange);
															#line 732 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					}
					else
					{
															#line 429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						logValue.param1 = std::max(r.range().begin, range.begin);
															#line 430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						logValue.param2 = minKey;
															#line 431 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						if (removePrefix.size())
															#line 742 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
						{
															#line 432 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
							logValue.param1 = logValue.param1.removePrefix(removePrefix);
															#line 433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
							logValue.param2 = logValue.param2.removePrefix(removePrefix);
															#line 748 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
						}
															#line 435 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						if (addPrefix.size())
															#line 752 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
						{
															#line 436 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
							logValue.param1 = logValue.param1.withPrefix(addPrefix, tempArena);
															#line 437 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
							logValue.param2 = logValue.param2.withPrefix(addPrefix, tempArena);
															#line 758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
						}
															#line 439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						_addResult(tenantMapChanging, result, mutationSize, arena, logValue, tenantMapRange);
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					}
															#line 441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					if (originalLogValue.param1 == logValue.param1 && originalLogValue.param2 == logValue.param2)
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					{
															#line 442 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						encryptedResult->push_back_deep(*arena, encryptedLogValue);
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					}
					else
					{
															#line 444 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						encryptedResult->push_back_deep(*arena, Optional<MutationRef>());
															#line 776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					}
				}
			}
		}
		else
		{
															#line 449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			Version ver = key_version->rangeContaining(logValue.param1).value();
															#line 451 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (version > ver && ver != invalidVersion)
															#line 787 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 452 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (removePrefix.size())
															#line 791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 453 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					logValue.param1 = logValue.param1.removePrefix(removePrefix);
															#line 795 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
															#line 455 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (addPrefix.size())
															#line 799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 456 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					logValue.param1 = logValue.param1.withPrefix(addPrefix, tempArena);
															#line 803 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
															#line 458 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				_addResult(tenantMapChanging, result, mutationSize, arena, logValue, tenantMapRange);
															#line 461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (originalLogValue.param1 == logValue.param1 && originalLogValue.param2 == logValue.param2)
															#line 809 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 462 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					encryptedResult->push_back_deep(*arena, encryptedLogValue);
															#line 813 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
				else
				{
															#line 464 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					encryptedResult->push_back_deep(*arena, Optional<MutationRef>());
															#line 819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
			}
		}
															#line 469 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		consumed += BackupAgentBase::logHeaderSize + len1 + len2;
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (loopDepth == 0) return a_body1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont4(int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 387 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			TraceEvent(SevWarnAlways, "MutationLogRestoreEncryptKeyFetchFailed") .detail("Version", version) .detail("TenantId", domainId);
															#line 390 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (e.code() == error_code_encrypt_keys_fetch_failed || e.code() == error_code_encrypt_key_not_found)
															#line 843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 392 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				CODE_PROBE(true, "mutation log restore encrypt keys not found");
															#line 393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				consumed += BackupAgentBase::logHeaderSize + len1 + len2;
															#line 849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				return a_body1cont2loopHead1(loopDepth); // continue
			}
			else
			{
															#line 396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				return a_body1Catch2(e, std::max(0, loopDepth - 1));
															#line 856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1cont2loopBody1cont5(int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont6(TextAndHeaderCipherKeys const& cipherKeys,int loopDepth) 
	{
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		logValue = logValue.decrypt(cipherKeys, tempArena, BlobCipherMetrics::RESTORE);
															#line 877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1cont2loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont6(TextAndHeaderCipherKeys && cipherKeys,int loopDepth) 
	{
															#line 378 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		logValue = logValue.decrypt(cipherKeys, tempArena, BlobCipherMetrics::RESTORE);
															#line 886 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1cont2loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1when1(TextAndHeaderCipherKeys const& cipherKeys,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont6(cipherKeys, loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1when1(TextAndHeaderCipherKeys && cipherKeys,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont6(std::move(cipherKeys), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DecodeBackupLogValueActor*>(this)->actor_wait_state > 0) static_cast<DecodeBackupLogValueActor*>(this)->actor_wait_state = 0;
		static_cast<DecodeBackupLogValueActor*>(this)->ActorCallback< DecodeBackupLogValueActor, 1, TextAndHeaderCipherKeys >::remove();

	}
	void a_callback_fire(ActorCallback< DecodeBackupLogValueActor, 1, TextAndHeaderCipherKeys >*,TextAndHeaderCipherKeys const& value) 
	{
		fdb_probe_actor_enter("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DecodeBackupLogValueActor, 1, TextAndHeaderCipherKeys >*,TextAndHeaderCipherKeys && value) 
	{
		fdb_probe_actor_enter("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DecodeBackupLogValueActor, 1, TextAndHeaderCipherKeys >*,Error err) 
	{
		fdb_probe_actor_enter("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont2loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont2loopBody1cont8(TextAndHeaderCipherKeys const& cipherKeys,int loopDepth) 
	{
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		logValue = logValue.decrypt(cipherKeys, tempArena, BlobCipherMetrics::RESTORE);
															#line 958 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1cont2loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1cont8(TextAndHeaderCipherKeys && cipherKeys,int loopDepth) 
	{
															#line 383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		logValue = logValue.decrypt(cipherKeys, tempArena, BlobCipherMetrics::RESTORE);
															#line 967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1cont2loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1when2(TextAndHeaderCipherKeys const& cipherKeys,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont8(cipherKeys, loopDepth);

		return loopDepth;
	}
	int a_body1cont2loopBody1when2(TextAndHeaderCipherKeys && cipherKeys,int loopDepth) 
	{
		loopDepth = a_body1cont2loopBody1cont8(std::move(cipherKeys), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<DecodeBackupLogValueActor*>(this)->actor_wait_state > 0) static_cast<DecodeBackupLogValueActor*>(this)->actor_wait_state = 0;
		static_cast<DecodeBackupLogValueActor*>(this)->ActorCallback< DecodeBackupLogValueActor, 2, TextAndHeaderCipherKeys >::remove();

	}
	void a_callback_fire(ActorCallback< DecodeBackupLogValueActor, 2, TextAndHeaderCipherKeys >*,TextAndHeaderCipherKeys const& value) 
	{
		fdb_probe_actor_enter("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< DecodeBackupLogValueActor, 2, TextAndHeaderCipherKeys >*,TextAndHeaderCipherKeys && value) 
	{
		fdb_probe_actor_enter("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< DecodeBackupLogValueActor, 2, TextAndHeaderCipherKeys >*,Error err) 
	{
		fdb_probe_actor_enter("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont2loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1cont2loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont2loopBody1cont10(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont2loopBody1cont4(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1cont8(int loopDepth) 
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
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Arena* arena;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	VectorRef<MutationRef>* result;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	VectorRef<Optional<MutationRef>>* encryptedResult;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	int* mutationSize;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	bool* tenantMapChanging;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Standalone<StringRef> value;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key addPrefix;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key removePrefix;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Version version;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Reference<KeyRangeMap<Version>> key_version;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Database cx;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	std::map<int64_t, TenantName>* tenantMap;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	bool provisionalProxy;
															#line 309 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	uint64_t offset;
															#line 321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	uint32_t totalBytes;
															#line 324 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	uint32_t consumed;
															#line 329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	int originalOffset;
															#line 330 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	DatabaseConfiguration config;
															#line 331 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	KeyRangeRef tenantMapRange;
															#line 337 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	uint32_t len1;
															#line 340 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	uint32_t len2;
															#line 346 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	MutationRef logValue;
															#line 347 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Arena tempArena;
															#line 353 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Optional<MutationRef> encryptedLogValue;
															#line 371 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	EncryptCipherDomainId domainId;
															#line 1111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via decodeBackupLogValue()
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class DecodeBackupLogValueActor final : public Actor<Void>, public ActorCallback< DecodeBackupLogValueActor, 0, DatabaseConfiguration >, public ActorCallback< DecodeBackupLogValueActor, 1, TextAndHeaderCipherKeys >, public ActorCallback< DecodeBackupLogValueActor, 2, TextAndHeaderCipherKeys >, public FastAllocated<DecodeBackupLogValueActor>, public DecodeBackupLogValueActorState<DecodeBackupLogValueActor> {
															#line 1116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
	using FastAllocated<DecodeBackupLogValueActor>::operator new;
	using FastAllocated<DecodeBackupLogValueActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DecodeBackupLogValueActor, 0, DatabaseConfiguration >;
friend struct ActorCallback< DecodeBackupLogValueActor, 1, TextAndHeaderCipherKeys >;
friend struct ActorCallback< DecodeBackupLogValueActor, 2, TextAndHeaderCipherKeys >;
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	DecodeBackupLogValueActor(Arena* const& arena,VectorRef<MutationRef>* const& result,VectorRef<Optional<MutationRef>>* const& encryptedResult,int* const& mutationSize,bool* const& tenantMapChanging,Standalone<StringRef> const& value,Key const& addPrefix,Key const& removePrefix,Version const& version,Reference<KeyRangeMap<Version>> const& key_version,Database const& cx,std::map<int64_t, TenantName>* const& tenantMap,bool const& provisionalProxy) 
															#line 1129 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		 : Actor<Void>(),
		   DecodeBackupLogValueActorState<DecodeBackupLogValueActor>(arena, result, encryptedResult, mutationSize, tenantMapChanging, value, addPrefix, removePrefix, version, key_version, cx, tenantMap, provisionalProxy)
	{
		fdb_probe_actor_enter("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("decodeBackupLogValue");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("decodeBackupLogValue", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DecodeBackupLogValueActor, 0, DatabaseConfiguration >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DecodeBackupLogValueActor, 1, TextAndHeaderCipherKeys >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< DecodeBackupLogValueActor, 2, TextAndHeaderCipherKeys >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] static Future<Void> decodeBackupLogValue( Arena* const& arena, VectorRef<MutationRef>* const& result, VectorRef<Optional<MutationRef>>* const& encryptedResult, int* const& mutationSize, bool* const& tenantMapChanging, Standalone<StringRef> const& value, Key const& addPrefix, Key const& removePrefix, Version const& version, Reference<KeyRangeMap<Version>> const& key_version, Database const& cx, std::map<int64_t, TenantName>* const& tenantMap, bool const& provisionalProxy ) {
															#line 295 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new DecodeBackupLogValueActor(arena, result, encryptedResult, mutationSize, tenantMapChanging, value, addPrefix, removePrefix, version, key_version, cx, tenantMap, provisionalProxy));
															#line 1159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 492 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"

static double lastErrorTime = 0;

void logErrorWorker(Reference<ReadYourWritesTransaction> tr, Key keyErrors, std::string message) {
	tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
	tr->setOption(FDBTransactionOptions::LOCK_AWARE);
	if (now() - lastErrorTime > CLIENT_KNOBS->BACKUP_ERROR_DELAY) {
		TraceEvent("BA_LogError").detail("Key", keyErrors).detail("Message", message);
		lastErrorTime = now();
	}
	tr->set(keyErrors, message);
}

Future<Void> logError(Database cx, Key keyErrors, const std::string& message) {
	return runRYWTransaction(cx, [=](Reference<ReadYourWritesTransaction> tr) {
		logErrorWorker(tr, keyErrors, message);
		return Future<Void>(Void());
	});
}

Future<Void> logError(Reference<ReadYourWritesTransaction> tr, Key keyErrors, const std::string& message) {
	return logError(tr->getDatabase(), keyErrors, message);
}

															#line 1187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via readCommitted()
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
template <class ReadCommittedActor>
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class ReadCommittedActorState {
															#line 1194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	ReadCommittedActorState(Database const& cx,PromiseStream<RangeResultWithVersion> const& results,Reference<FlowLock> const& lock,KeyRangeRef const& range,Terminator const& terminator,AccessSystemKeys const& systemAccess,LockAware const& lockAware) 
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		 : cx(cx),
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   results(results),
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   lock(lock),
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   range(range),
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   terminator(terminator),
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   systemAccess(systemAccess),
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   lockAware(lockAware),
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   begin(firstGreaterOrEqual(range.begin)),
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   end(firstGreaterOrEqual(range.end)),
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   tr(cx),
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   releaser()
															#line 1221 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
	{
		fdb_probe_actor_create("readCommitted", reinterpret_cast<unsigned long>(this));

	}
	~ReadCommittedActorState() 
	{
		fdb_probe_actor_destroy("readCommitted", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 528 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			;
															#line 1236 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
		this->~ReadCommittedActorState();
		static_cast<ReadCommittedActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			limits = GetRangeLimits(GetRangeLimits::ROW_LIMIT_UNLIMITED, (g_network->isSimulated() && !g_simulator->speedUpSimulation) ? CLIENT_KNOBS->BACKUP_SIMULATED_LIMIT_BYTES : CLIENT_KNOBS->BACKUP_GET_RANGE_LIMIT_BYTES);
															#line 535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (systemAccess)
															#line 1269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 536 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1273 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 537 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (lockAware)
															#line 1277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 538 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			releaser.release();
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_0 = lock->take(TaskPriority::DefaultYield, limits.bytes + CLIENT_KNOBS->VALUE_SIZE_LIMIT + CLIENT_KNOBS->SYSTEM_KEY_SIZE_LIMIT);
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<ReadCommittedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1289 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReadCommittedActor*>(this)->actor_wait_state = 1;
															#line 542 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor, 0, Void >*>(static_cast<ReadCommittedActor*>(this)));
															#line 1294 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 579 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (e.code() == error_code_transaction_too_old)
															#line 1316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 582 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				tr.fullReset();
															#line 1320 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				loopDepth = a_body1loopBody1Catch1cont1(loopDepth);
			}
			else
			{
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				StrictFuture<Void> __when_expr_3 = tr.onError(e);
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (static_cast<ReadCommittedActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1329 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
				static_cast<ReadCommittedActor*>(this)->actor_wait_state = 4;
															#line 584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor, 3, Void >*>(static_cast<ReadCommittedActor*>(this)));
															#line 1334 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				loopDepth = 0;
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
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		releaser = FlowLock::Releaser( *lock, limits.bytes + CLIENT_KNOBS->VALUE_SIZE_LIMIT + CLIENT_KNOBS->SYSTEM_KEY_SIZE_LIMIT);
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr.getRange(begin, end, limits);
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<ReadCommittedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadCommittedActor*>(this)->actor_wait_state = 2;
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor, 1, RangeResult >*>(static_cast<ReadCommittedActor*>(this)));
															#line 1359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		releaser = FlowLock::Releaser( *lock, limits.bytes + CLIENT_KNOBS->VALUE_SIZE_LIMIT + CLIENT_KNOBS->SYSTEM_KEY_SIZE_LIMIT);
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr.getRange(begin, end, limits);
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<ReadCommittedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadCommittedActor*>(this)->actor_wait_state = 2;
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor, 1, RangeResult >*>(static_cast<ReadCommittedActor*>(this)));
															#line 1377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
		if (static_cast<ReadCommittedActor*>(this)->actor_wait_state > 0) static_cast<ReadCommittedActor*>(this)->actor_wait_state = 0;
		static_cast<ReadCommittedActor*>(this)->ActorCallback< ReadCommittedActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadCommittedActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont5(int loopDepth) 
	{
															#line 551 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (values.size() > 1 && BUGGIFY)
															#line 1449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			RangeResult copy;
															#line 554 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			for(int i = 0;i < values.size() / 2;i++) {
															#line 555 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				copy.push_back_deep(copy.arena(), values[i]);
															#line 1457 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 557 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			values = copy;
															#line 558 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			values.more = true;
															#line 560 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (deterministicRandom()->random01() < 0.5)
															#line 1465 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				StrictFuture<Void> __when_expr_2 = delay(6.0);
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (static_cast<ReadCommittedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_2.get(), loopDepth); };
				static_cast<ReadCommittedActor*>(this)->actor_wait_state = 3;
															#line 561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor, 2, Void >*>(static_cast<ReadCommittedActor*>(this)));
															#line 1476 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont7(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1loopBody1cont6(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(RangeResult const& __values,int loopDepth) 
	{
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		values = __values;
															#line 1495 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(RangeResult && __values,int loopDepth) 
	{
		values = std::move(__values);
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReadCommittedActor*>(this)->actor_wait_state > 0) static_cast<ReadCommittedActor*>(this)->actor_wait_state = 0;
		static_cast<ReadCommittedActor*>(this)->ActorCallback< ReadCommittedActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor, 1, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor, 1, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadCommittedActor, 1, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont6(int loopDepth) 
	{
															#line 564 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		releaser.remaining -= values.expectedSize();
															#line 566 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		ASSERT(releaser.remaining >= 0);
															#line 568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		results.send(RangeResultWithVersion(values, tr.getReadVersion().get()));
															#line 570 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (values.size() > 0)
															#line 1568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 571 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			begin = firstGreaterThan(values.end()[-1].key);
															#line 1572 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!values.more && !limits.isReached())
															#line 1576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 574 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (terminator)
															#line 1580 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				results.sendError(end_of_stream());
															#line 1584 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 576 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<ReadCommittedActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadCommittedActorState(); static_cast<ReadCommittedActor*>(this)->destroy(); return 0; }
															#line 1588 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			new (&static_cast<ReadCommittedActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ReadCommittedActorState();
			static_cast<ReadCommittedActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1loopBody1cont14(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ReadCommittedActor*>(this)->actor_wait_state > 0) static_cast<ReadCommittedActor*>(this)->actor_wait_state = 0;
		static_cast<ReadCommittedActor*>(this)->ActorCallback< ReadCommittedActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ReadCommittedActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont14(int loopDepth) 
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
	int a_body1loopBody1Catch1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ReadCommittedActor*>(this)->actor_wait_state > 0) static_cast<ReadCommittedActor*>(this)->actor_wait_state = 0;
		static_cast<ReadCommittedActor*>(this)->ActorCallback< ReadCommittedActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ReadCommittedActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Database cx;
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	PromiseStream<RangeResultWithVersion> results;
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Reference<FlowLock> lock;
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	KeyRangeRef range;
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Terminator terminator;
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	AccessSystemKeys systemAccess;
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	LockAware lockAware;
															#line 523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	KeySelector begin;
															#line 524 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	KeySelector end;
															#line 525 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Transaction tr;
															#line 526 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	FlowLock::Releaser releaser;
															#line 530 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	GetRangeLimits limits;
															#line 547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	RangeResult values;
															#line 1799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via readCommitted()
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class ReadCommittedActor final : public Actor<Void>, public ActorCallback< ReadCommittedActor, 0, Void >, public ActorCallback< ReadCommittedActor, 1, RangeResult >, public ActorCallback< ReadCommittedActor, 2, Void >, public ActorCallback< ReadCommittedActor, 3, Void >, public FastAllocated<ReadCommittedActor>, public ReadCommittedActorState<ReadCommittedActor> {
															#line 1804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
	using FastAllocated<ReadCommittedActor>::operator new;
	using FastAllocated<ReadCommittedActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadCommittedActor, 0, Void >;
friend struct ActorCallback< ReadCommittedActor, 1, RangeResult >;
friend struct ActorCallback< ReadCommittedActor, 2, Void >;
friend struct ActorCallback< ReadCommittedActor, 3, Void >;
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	ReadCommittedActor(Database const& cx,PromiseStream<RangeResultWithVersion> const& results,Reference<FlowLock> const& lock,KeyRangeRef const& range,Terminator const& terminator,AccessSystemKeys const& systemAccess,LockAware const& lockAware) 
															#line 1818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		 : Actor<Void>(),
		   ReadCommittedActorState<ReadCommittedActor>(cx, results, lock, range, terminator, systemAccess, lockAware)
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readCommitted");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadCommittedActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReadCommittedActor, 1, RangeResult >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ReadCommittedActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ReadCommittedActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] Future<Void> readCommitted( Database const& cx, PromiseStream<RangeResultWithVersion> const& results, Reference<FlowLock> const& lock, KeyRangeRef const& range, Terminator const& terminator, AccessSystemKeys const& systemAccess, LockAware const& lockAware ) {
															#line 516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new ReadCommittedActor(cx, results, lock, range, terminator, systemAccess, lockAware));
															#line 1849 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"

															#line 1854 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via readCommitted()
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
template <class ReadCommittedActor1>
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class ReadCommittedActor1State {
															#line 1861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	ReadCommittedActor1State(Database const& cx,PromiseStream<RCGroup> const& results,Future<Void> const& active,Reference<FlowLock> const& lock,KeyRangeRef const& range,std::function<std::pair<uint64_t, uint32_t>(Key key)> const& groupBy,Terminator const& terminator,AccessSystemKeys const& systemAccess,LockAware const& lockAware) 
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		 : cx(cx),
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   results(results),
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   active(active),
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   lock(lock),
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   range(range),
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   groupBy(groupBy),
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   terminator(terminator),
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   systemAccess(systemAccess),
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   lockAware(lockAware),
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   nextKey(firstGreaterOrEqual(range.begin)),
															#line 600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   end(firstGreaterOrEqual(range.end)),
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   rcGroup(RCGroup()),
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   skipGroup(ULLONG_MAX),
															#line 604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   tr(cx),
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   releaser()
															#line 1896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
	{
		fdb_probe_actor_create("readCommitted", reinterpret_cast<unsigned long>(this));

	}
	~ReadCommittedActor1State() 
	{
		fdb_probe_actor_destroy("readCommitted", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			;
															#line 1911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
		this->~ReadCommittedActor1State();
		static_cast<ReadCommittedActor1*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			limits = GetRangeLimits(GetRangeLimits::ROW_LIMIT_UNLIMITED, (g_network->isSimulated() && !g_simulator->speedUpSimulation) ? CLIENT_KNOBS->BACKUP_SIMULATED_LIMIT_BYTES : CLIENT_KNOBS->BACKUP_GET_RANGE_LIMIT_BYTES);
															#line 614 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (systemAccess)
															#line 1944 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 615 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 616 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (lockAware)
															#line 1952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 617 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr.getRange(nextKey, end, limits);
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1962 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 1;
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor1, 0, RangeResult >*>(static_cast<ReadCommittedActor1*>(this)));
															#line 1967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (e.code() == error_code_transaction_too_old)
															#line 1989 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 692 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				tr.fullReset();
															#line 1993 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				loopDepth = a_body1loopBody1Catch1cont1(loopDepth);
			}
			else
			{
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				StrictFuture<Void> __when_expr_4 = tr.onError(e);
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_4.get(), loopDepth); };
				static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 5;
															#line 694 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor1, 4, Void >*>(static_cast<ReadCommittedActor1*>(this)));
															#line 2007 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				loopDepth = 0;
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
															#line 623 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (rangevalue.size() > 1 && BUGGIFY)
															#line 2023 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 624 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			RangeResult copy;
															#line 626 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			for(int i = 0;i < rangevalue.size() / 2;i++) {
															#line 627 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				copy.push_back_deep(copy.arena(), rangevalue[i]);
															#line 2031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 629 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			rangevalue = copy;
															#line 630 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			rangevalue.more = true;
															#line 632 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (deterministicRandom()->random01() < 0.5)
															#line 2039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				StrictFuture<Void> __when_expr_1 = delay(6.0);
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
				static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 2;
															#line 633 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor1, 1, Void >*>(static_cast<ReadCommittedActor1*>(this)));
															#line 2050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont6(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1loopBody1cont5(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& __rangevalue,int loopDepth) 
	{
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		rangevalue = __rangevalue;
															#line 2069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && __rangevalue,int loopDepth) 
	{
		rangevalue = std::move(__rangevalue);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state > 0) static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 0;
		static_cast<ReadCommittedActor1*>(this)->ActorCallback< ReadCommittedActor1, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor1, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor1, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadCommittedActor1, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont5(int loopDepth) 
	{
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<Void> __when_expr_2 = active;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_2.get(), loopDepth); };
		static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 3;
															#line 637 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor1, 2, Void >*>(static_cast<ReadCommittedActor1*>(this)));
															#line 2143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state > 0) static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 0;
		static_cast<ReadCommittedActor1*>(this)->ActorCallback< ReadCommittedActor1, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor1, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor1, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadCommittedActor1, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont9(Void const& _,int loopDepth) 
	{
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		releaser.release();
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<Void> __when_expr_3 = lock->take(TaskPriority::DefaultYield, rangevalue.expectedSize() + rcGroup.items.expectedSize());
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont9when1(__when_expr_3.get(), loopDepth); };
		static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 4;
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor1, 3, Void >*>(static_cast<ReadCommittedActor1*>(this)));
															#line 2242 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void && _,int loopDepth) 
	{
															#line 638 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		releaser.release();
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<Void> __when_expr_3 = lock->take(TaskPriority::DefaultYield, rangevalue.expectedSize() + rcGroup.items.expectedSize());
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont9when1(__when_expr_3.get(), loopDepth); };
		static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 4;
															#line 639 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor1, 3, Void >*>(static_cast<ReadCommittedActor1*>(this)));
															#line 2260 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state > 0) static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 0;
		static_cast<ReadCommittedActor1*>(this)->ActorCallback< ReadCommittedActor1, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor1, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor1, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ReadCommittedActor1, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont10(Void const& _,int loopDepth) 
	{
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		releaser = FlowLock::Releaser(*lock, rangevalue.expectedSize() + rcGroup.items.expectedSize());
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		for( auto& s : rangevalue ) {
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			uint64_t groupKey = groupBy(s.key).first;
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (groupKey != skipGroup)
															#line 2338 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (rcGroup.version == -1)
															#line 2342 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					rcGroup.version = tr.getReadVersion().get();
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					rcGroup.groupKey = groupKey;
															#line 2348 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
				else
				{
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					if (rcGroup.groupKey != groupKey)
															#line 2354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					{
															#line 656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						releaser.remaining -= rcGroup.items .expectedSize();
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						ASSERT(releaser.remaining >= 0);
															#line 660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						results.send(rcGroup);
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						nextKey = firstGreaterThan(rcGroup.items.end()[-1].key);
															#line 662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						skipGroup = rcGroup.groupKey;
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						rcGroup = RCGroup();
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						rcGroup.version = tr.getReadVersion().get();
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						rcGroup.groupKey = groupKey;
															#line 2372 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					}
				}
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				rcGroup.items.push_back_deep(rcGroup.items.arena(), s);
															#line 2377 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
		}
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!rangevalue.more)
															#line 2382 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (rcGroup.version != -1)
															#line 2386 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				releaser.remaining -= rcGroup.items .expectedSize();
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				ASSERT(releaser.remaining >= 0);
															#line 679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				results.send(rcGroup);
															#line 2394 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (terminator)
															#line 2398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				results.sendError(end_of_stream());
															#line 2402 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<ReadCommittedActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadCommittedActor1State(); static_cast<ReadCommittedActor1*>(this)->destroy(); return 0; }
															#line 2406 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			new (&static_cast<ReadCommittedActor1*>(this)->SAV< Void >::value()) Void(Void());
			this->~ReadCommittedActor1State();
			static_cast<ReadCommittedActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		nextKey = firstGreaterThan(rangevalue.end()[-1].key);
															#line 2414 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont10cont10(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont10(Void && _,int loopDepth) 
	{
															#line 640 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		releaser = FlowLock::Releaser(*lock, rangevalue.expectedSize() + rcGroup.items.expectedSize());
															#line 642 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		for( auto& s : rangevalue ) {
															#line 643 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			uint64_t groupKey = groupBy(s.key).first;
															#line 645 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (groupKey != skipGroup)
															#line 2429 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 646 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (rcGroup.version == -1)
															#line 2433 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 647 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					rcGroup.version = tr.getReadVersion().get();
															#line 648 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					rcGroup.groupKey = groupKey;
															#line 2439 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
				else
				{
															#line 649 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					if (rcGroup.groupKey != groupKey)
															#line 2445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					{
															#line 656 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						releaser.remaining -= rcGroup.items .expectedSize();
															#line 659 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						ASSERT(releaser.remaining >= 0);
															#line 660 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						results.send(rcGroup);
															#line 661 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						nextKey = firstGreaterThan(rcGroup.items.end()[-1].key);
															#line 662 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						skipGroup = rcGroup.groupKey;
															#line 664 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						rcGroup = RCGroup();
															#line 665 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						rcGroup.version = tr.getReadVersion().get();
															#line 666 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						rcGroup.groupKey = groupKey;
															#line 2463 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					}
				}
															#line 668 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				rcGroup.items.push_back_deep(rcGroup.items.arena(), s);
															#line 2468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
		}
															#line 672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!rangevalue.more)
															#line 2473 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 673 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (rcGroup.version != -1)
															#line 2477 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 674 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				releaser.remaining -= rcGroup.items .expectedSize();
															#line 677 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				ASSERT(releaser.remaining >= 0);
															#line 679 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				results.send(rcGroup);
															#line 2485 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 682 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (terminator)
															#line 2489 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 683 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				results.sendError(end_of_stream());
															#line 2493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<ReadCommittedActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadCommittedActor1State(); static_cast<ReadCommittedActor1*>(this)->destroy(); return 0; }
															#line 2497 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			new (&static_cast<ReadCommittedActor1*>(this)->SAV< Void >::value()) Void(Void());
			this->~ReadCommittedActor1State();
			static_cast<ReadCommittedActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 687 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		nextKey = firstGreaterThan(rangevalue.end()[-1].key);
															#line 2505 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont10cont10(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont9when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont9when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state > 0) static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 0;
		static_cast<ReadCommittedActor1*>(this)->ActorCallback< ReadCommittedActor1, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor1, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont9when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor1, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont9when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ReadCommittedActor1, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1cont10cont10(int loopDepth) 
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
	int a_body1loopBody1Catch1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state > 0) static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 0;
		static_cast<ReadCommittedActor1*>(this)->ActorCallback< ReadCommittedActor1, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor1, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< ReadCommittedActor1, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< ReadCommittedActor1, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Database cx;
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	PromiseStream<RCGroup> results;
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Future<Void> active;
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Reference<FlowLock> lock;
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	KeyRangeRef range;
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	std::function<std::pair<uint64_t, uint32_t>(Key key)> groupBy;
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Terminator terminator;
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	AccessSystemKeys systemAccess;
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	LockAware lockAware;
															#line 599 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	KeySelector nextKey;
															#line 600 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	KeySelector end;
															#line 602 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	RCGroup rcGroup;
															#line 603 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	uint64_t skipGroup;
															#line 604 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Transaction tr;
															#line 605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	FlowLock::Releaser releaser;
															#line 609 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	GetRangeLimits limits;
															#line 619 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	RangeResult rangevalue;
															#line 2701 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via readCommitted()
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class ReadCommittedActor1 final : public Actor<Void>, public ActorCallback< ReadCommittedActor1, 0, RangeResult >, public ActorCallback< ReadCommittedActor1, 1, Void >, public ActorCallback< ReadCommittedActor1, 2, Void >, public ActorCallback< ReadCommittedActor1, 3, Void >, public ActorCallback< ReadCommittedActor1, 4, Void >, public FastAllocated<ReadCommittedActor1>, public ReadCommittedActor1State<ReadCommittedActor1> {
															#line 2706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
	using FastAllocated<ReadCommittedActor1>::operator new;
	using FastAllocated<ReadCommittedActor1>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadCommittedActor1, 0, RangeResult >;
friend struct ActorCallback< ReadCommittedActor1, 1, Void >;
friend struct ActorCallback< ReadCommittedActor1, 2, Void >;
friend struct ActorCallback< ReadCommittedActor1, 3, Void >;
friend struct ActorCallback< ReadCommittedActor1, 4, Void >;
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	ReadCommittedActor1(Database const& cx,PromiseStream<RCGroup> const& results,Future<Void> const& active,Reference<FlowLock> const& lock,KeyRangeRef const& range,std::function<std::pair<uint64_t, uint32_t>(Key key)> const& groupBy,Terminator const& terminator,AccessSystemKeys const& systemAccess,LockAware const& lockAware) 
															#line 2721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		 : Actor<Void>(),
		   ReadCommittedActor1State<ReadCommittedActor1>(cx, results, active, lock, range, groupBy, terminator, systemAccess, lockAware)
	{
		fdb_probe_actor_enter("readCommitted", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readCommitted");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readCommitted", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadCommittedActor1, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReadCommittedActor1, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ReadCommittedActor1, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ReadCommittedActor1, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< ReadCommittedActor1, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] Future<Void> readCommitted( Database const& cx, PromiseStream<RCGroup> const& results, Future<Void> const& active, Reference<FlowLock> const& lock, KeyRangeRef const& range, std::function<std::pair<uint64_t, uint32_t>(Key key)> const& groupBy, Terminator const& terminator, AccessSystemKeys const& systemAccess, LockAware const& lockAware ) {
															#line 590 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new ReadCommittedActor1(cx, results, active, lock, range, groupBy, terminator, systemAccess, lockAware));
															#line 2753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 699 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"

Future<Void> readCommitted(Database cx,
                           PromiseStream<RCGroup> results,
                           Reference<FlowLock> lock,
                           KeyRangeRef range,
                           std::function<std::pair<uint64_t, uint32_t>(Key key)> groupBy) {
	return readCommitted(
	    cx, results, Void(), lock, range, groupBy, Terminator::True, AccessSystemKeys::True, LockAware::True);
}

															#line 2767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via sendCommitTransactionRequest()
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
template <class SendCommitTransactionRequestActor>
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class SendCommitTransactionRequestActorState {
															#line 2774 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	SendCommitTransactionRequestActorState(CommitTransactionRequest const& req,Key const& uid,Version const& newBeginVersion,Key const& rangeBegin,NotifiedVersion* const& committedVersion,int* const& totalBytes,int* const& mutationSize,PromiseStream<Future<Void>> const& addActor,FlowLock* const& commitLock,PublicRequestStream<CommitTransactionRequest> const& commit) 
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		 : req(req),
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   uid(uid),
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   newBeginVersion(newBeginVersion),
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   rangeBegin(rangeBegin),
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   committedVersion(committedVersion),
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   totalBytes(totalBytes),
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   mutationSize(mutationSize),
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   addActor(addActor),
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   commitLock(commitLock),
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   commit(commit)
															#line 2799 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
	{
		fdb_probe_actor_create("sendCommitTransactionRequest", reinterpret_cast<unsigned long>(this));

	}
	~SendCommitTransactionRequestActorState() 
	{
		fdb_probe_actor_destroy("sendCommitTransactionRequest", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			Key applyBegin = uid.withPrefix(applyMutationsBeginRange.begin);
															#line 720 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			Key versionKey = BinaryWriter::toValue(newBeginVersion, Unversioned());
															#line 721 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			Key rangeEnd = getApplyKey(newBeginVersion, uid);
															#line 725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			req.transaction.mutations.push_back_deep(req.arena, MutationRef(MutationRef::SetValue, applyBegin, versionKey));
															#line 726 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			req.transaction.encryptedMutations.push_back_deep(req.arena, Optional<MutationRef>());
															#line 727 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			req.transaction.write_conflict_ranges.push_back_deep(req.arena, singleKeyRange(applyBegin));
															#line 728 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			req.transaction.mutations.push_back_deep(req.arena, MutationRef(MutationRef::ClearRange, rangeBegin, rangeEnd));
															#line 729 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			req.transaction.encryptedMutations.push_back_deep(req.arena, Optional<MutationRef>());
															#line 730 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			req.transaction.write_conflict_ranges.push_back_deep(req.arena, singleKeyRange(rangeBegin));
															#line 735 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			req.transaction.read_snapshot = committedVersion->get();
															#line 736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			req.flags = req.flags | CommitTransactionRequest::FLAG_IS_LOCK_AWARE;
															#line 738 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			*totalBytes += *mutationSize;
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_0 = commitLock->take(TaskPriority::DefaultYield, *mutationSize);
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<SendCommitTransactionRequestActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<SendCommitTransactionRequestActor*>(this)->actor_wait_state = 1;
															#line 739 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< SendCommitTransactionRequestActor, 0, Void >*>(static_cast<SendCommitTransactionRequestActor*>(this)));
															#line 2845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
		this->~SendCommitTransactionRequestActorState();
		static_cast<SendCommitTransactionRequestActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		addActor.send(commitLock->releaseWhen(success(commit.getReply(req)), *mutationSize));
															#line 741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!static_cast<SendCommitTransactionRequestActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendCommitTransactionRequestActorState(); static_cast<SendCommitTransactionRequestActor*>(this)->destroy(); return 0; }
															#line 2870 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		new (&static_cast<SendCommitTransactionRequestActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SendCommitTransactionRequestActorState();
		static_cast<SendCommitTransactionRequestActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		addActor.send(commitLock->releaseWhen(success(commit.getReply(req)), *mutationSize));
															#line 741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!static_cast<SendCommitTransactionRequestActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~SendCommitTransactionRequestActorState(); static_cast<SendCommitTransactionRequestActor*>(this)->destroy(); return 0; }
															#line 2884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		new (&static_cast<SendCommitTransactionRequestActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~SendCommitTransactionRequestActorState();
		static_cast<SendCommitTransactionRequestActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

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
		if (static_cast<SendCommitTransactionRequestActor*>(this)->actor_wait_state > 0) static_cast<SendCommitTransactionRequestActor*>(this)->actor_wait_state = 0;
		static_cast<SendCommitTransactionRequestActor*>(this)->ActorCallback< SendCommitTransactionRequestActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< SendCommitTransactionRequestActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("sendCommitTransactionRequest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendCommitTransactionRequest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< SendCommitTransactionRequestActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("sendCommitTransactionRequest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendCommitTransactionRequest", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< SendCommitTransactionRequestActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("sendCommitTransactionRequest", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("sendCommitTransactionRequest", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	CommitTransactionRequest req;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key uid;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Version newBeginVersion;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key rangeBegin;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	NotifiedVersion* committedVersion;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	int* totalBytes;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	int* mutationSize;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	PromiseStream<Future<Void>> addActor;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	FlowLock* commitLock;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	PublicRequestStream<CommitTransactionRequest> commit;
															#line 2975 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via sendCommitTransactionRequest()
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class SendCommitTransactionRequestActor final : public Actor<Void>, public ActorCallback< SendCommitTransactionRequestActor, 0, Void >, public FastAllocated<SendCommitTransactionRequestActor>, public SendCommitTransactionRequestActorState<SendCommitTransactionRequestActor> {
															#line 2980 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
	using FastAllocated<SendCommitTransactionRequestActor>::operator new;
	using FastAllocated<SendCommitTransactionRequestActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< SendCommitTransactionRequestActor, 0, Void >;
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	SendCommitTransactionRequestActor(CommitTransactionRequest const& req,Key const& uid,Version const& newBeginVersion,Key const& rangeBegin,NotifiedVersion* const& committedVersion,int* const& totalBytes,int* const& mutationSize,PromiseStream<Future<Void>> const& addActor,FlowLock* const& commitLock,PublicRequestStream<CommitTransactionRequest> const& commit) 
															#line 2991 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		 : Actor<Void>(),
		   SendCommitTransactionRequestActorState<SendCommitTransactionRequestActor>(req, uid, newBeginVersion, rangeBegin, committedVersion, totalBytes, mutationSize, addActor, commitLock, commit)
	{
		fdb_probe_actor_enter("sendCommitTransactionRequest", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("sendCommitTransactionRequest");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("sendCommitTransactionRequest", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< SendCommitTransactionRequestActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] Future<Void> sendCommitTransactionRequest( CommitTransactionRequest const& req, Key const& uid, Version const& newBeginVersion, Key const& rangeBegin, NotifiedVersion* const& committedVersion, int* const& totalBytes, int* const& mutationSize, PromiseStream<Future<Void>> const& addActor, FlowLock* const& commitLock, PublicRequestStream<CommitTransactionRequest> const& commit ) {
															#line 709 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new SendCommitTransactionRequestActor(req, uid, newBeginVersion, rangeBegin, committedVersion, totalBytes, mutationSize, addActor, commitLock, commit));
															#line 3019 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"

															#line 3024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via kvMutationLogToTransactions()
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
template <class KvMutationLogToTransactionsActor>
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class KvMutationLogToTransactionsActorState {
															#line 3031 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	KvMutationLogToTransactionsActorState(Database const& cx,PromiseStream<RCGroup> const& results,Reference<FlowLock> const& lock,Key const& uid,Key const& addPrefix,Key const& removePrefix,PublicRequestStream<CommitTransactionRequest> const& commit,NotifiedVersion* const& committedVersion,Optional<Version> const& endVersion,Key const& rangeBegin,PromiseStream<Future<Void>> const& addActor,FlowLock* const& commitLock,Reference<KeyRangeMap<Version>> const& keyVersion,std::map<int64_t, TenantName>* const& tenantMap,bool const& provisionalProxy) 
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		 : cx(cx),
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   results(results),
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   lock(lock),
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   uid(uid),
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   addPrefix(addPrefix),
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   removePrefix(removePrefix),
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   commit(commit),
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   committedVersion(committedVersion),
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   endVersion(endVersion),
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   rangeBegin(rangeBegin),
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   addActor(addActor),
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   commitLock(commitLock),
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   keyVersion(keyVersion),
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   tenantMap(tenantMap),
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   provisionalProxy(provisionalProxy),
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   lastVersion(invalidVersion),
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   endOfStream(false),
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   totalBytes(0)
															#line 3072 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
	{
		fdb_probe_actor_create("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this));

	}
	~KvMutationLogToTransactionsActorState() 
	{
		fdb_probe_actor_destroy("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			;
															#line 3087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
		this->~KvMutationLogToTransactionsActorState();
		static_cast<KvMutationLogToTransactionsActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		req = CommitTransactionRequest();
															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		newBeginVersion = invalidVersion;
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		mutationSize = 0;
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		tenantMapChanging = false;
															#line 767 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		;
															#line 3125 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<Void> __when_expr_3 = sendCommitTransactionRequest(req, uid, newBeginVersion, rangeBegin, committedVersion, &totalBytes, &mutationSize, addActor, commitLock, commit);
															#line 851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_3.get(), loopDepth); };
		static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state = 4;
															#line 851 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< KvMutationLogToTransactionsActor, 3, Void >*>(static_cast<KvMutationLogToTransactionsActor*>(this)));
															#line 3141 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = 0;

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
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			FutureStream<RCGroup> __when_expr_0 = results.getFuture();
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1loopBody1when1(__when_expr_0.pop(), loopDepth); };
			static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state = 1;
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< KvMutationLogToTransactionsActor, 0, RCGroup >*>(static_cast<KvMutationLogToTransactionsActor*>(this)));
															#line 3165 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 839 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (e.code() == error_code_end_of_stream)
															#line 3200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (endVersion.present() && endVersion.get() > lastVersion && endVersion.get() > newBeginVersion)
															#line 3204 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 841 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					newBeginVersion = endVersion.get();
															#line 3208 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
															#line 843 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (newBeginVersion == invalidVersion)
															#line 3212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					if (!static_cast<KvMutationLogToTransactionsActor*>(this)->SAV<int>::futures) { (void)(totalBytes); this->~KvMutationLogToTransactionsActorState(); static_cast<KvMutationLogToTransactionsActor*>(this)->destroy(); return 0; }
															#line 3216 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					new (&static_cast<KvMutationLogToTransactionsActor*>(this)->SAV< int >::value()) int(std::move(totalBytes)); // state_var_RVO
					this->~KvMutationLogToTransactionsActorState();
					static_cast<KvMutationLogToTransactionsActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 845 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				endOfStream = true;
															#line 3224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
			}
															#line 848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			return a_body1Catch1(e, std::max(0, loopDepth - 2));
															#line 3229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		curReq = CommitTransactionRequest();
															#line 771 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		lock->release(group.items.expectedSize());
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		curBatchMutationSize = 0;
															#line 773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		tenantMapChanging = false;
															#line 775 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		BinaryWriter bw(Unversioned());
															#line 776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		for(int i = 0;i < group.items.size();++i) {
															#line 777 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			bw.serializeBytes(group.items[i].value);
															#line 3255 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 780 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		Standalone<StringRef> value = bw.toValue();
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<Void> __when_expr_1 = decodeBackupLogValue(&curReq.arena, &curReq.transaction.mutations, &curReq.transaction.encryptedMutations, &curBatchMutationSize, &tenantMapChanging, value, addPrefix, removePrefix, group.groupKey, keyVersion, cx, tenantMap, provisionalProxy);
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3263 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state = 2;
															#line 781 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< KvMutationLogToTransactionsActor, 1, Void >*>(static_cast<KvMutationLogToTransactionsActor*>(this)));
															#line 3268 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(RCGroup const& __group,int loopDepth) 
	{
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		group = __group;
															#line 3277 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(RCGroup && __group,int loopDepth) 
	{
		group = std::move(__group);
		loopDepth = a_body1loopBody1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state > 0) static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state = 0;
		static_cast<KvMutationLogToTransactionsActor*>(this)->ActorSingleCallback< KvMutationLogToTransactionsActor, 0, RCGroup >::remove();

	}
	void a_callback_fire(ActorSingleCallback< KvMutationLogToTransactionsActor, 0, RCGroup >*,RCGroup const& value) 
	{
		fdb_probe_actor_enter("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< KvMutationLogToTransactionsActor, 0, RCGroup >*,RCGroup && value) 
	{
		fdb_probe_actor_enter("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< KvMutationLogToTransactionsActor, 0, RCGroup >*,Error err) 
	{
		fdb_probe_actor_enter("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (tenantMapChanging && req.transaction.mutations.size())
															#line 3344 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			TraceEvent("MutationLogRestoreTenantMapChanging").detail("BeginVersion", newBeginVersion);
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			CODE_PROBE(true, "mutation log tenant map changing");
															#line 808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_2 = sendCommitTransactionRequest(req, uid, newBeginVersion, rangeBegin, committedVersion, &totalBytes, &mutationSize, addActor, commitLock, commit);
															#line 808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
			static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state = 3;
															#line 808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< KvMutationLogToTransactionsActor, 2, Void >*>(static_cast<KvMutationLogToTransactionsActor*>(this)));
															#line 3359 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1loopBody1cont5(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 804 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (tenantMapChanging && req.transaction.mutations.size())
															#line 3373 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 806 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			TraceEvent("MutationLogRestoreTenantMapChanging").detail("BeginVersion", newBeginVersion);
															#line 807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			CODE_PROBE(true, "mutation log tenant map changing");
															#line 808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_2 = sendCommitTransactionRequest(req, uid, newBeginVersion, rangeBegin, committedVersion, &totalBytes, &mutationSize, addActor, commitLock, commit);
															#line 808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3383 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
			static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state = 3;
															#line 808 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< KvMutationLogToTransactionsActor, 2, Void >*>(static_cast<KvMutationLogToTransactionsActor*>(this)));
															#line 3388 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1loopBody1cont5(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state > 0) static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state = 0;
		static_cast<KvMutationLogToTransactionsActor*>(this)->ActorCallback< KvMutationLogToTransactionsActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< KvMutationLogToTransactionsActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< KvMutationLogToTransactionsActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< KvMutationLogToTransactionsActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1loopBody1cont5(int loopDepth) 
	{
															#line 822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		i = int();
															#line 823 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		for(i = 0;i < curReq.transaction.mutations.size();i++) {
															#line 824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			req.transaction.mutations.push_back_deep(req.arena, curReq.transaction.mutations[i]);
															#line 825 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			req.transaction.encryptedMutations.push_back_deep(req.arena, curReq.transaction.encryptedMutations[i]);
															#line 3471 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		mutationSize += curBatchMutationSize;
															#line 829 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		newBeginVersion = group.groupKey + 1;
															#line 835 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (tenantMapChanging || mutationSize >= CLIENT_KNOBS->BACKUP_LOG_WRITE_BATCH_MAX_SIZE)
															#line 3479 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
		loopDepth = a_body1loopBody1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont6(Void const& _,int loopDepth) 
	{
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		req = CommitTransactionRequest();
															#line 819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		mutationSize = 0;
															#line 3493 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		req = CommitTransactionRequest();
															#line 819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		mutationSize = 0;
															#line 3504 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state > 0) static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state = 0;
		static_cast<KvMutationLogToTransactionsActor*>(this)->ActorCallback< KvMutationLogToTransactionsActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< KvMutationLogToTransactionsActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< KvMutationLogToTransactionsActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< KvMutationLogToTransactionsActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1loopBody1cont11(int loopDepth) 
	{
		try {
			loopDepth = a_body1loopBody1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 2));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 2));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (endOfStream)
															#line 3589 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<KvMutationLogToTransactionsActor*>(this)->SAV<int>::futures) { (void)(totalBytes); this->~KvMutationLogToTransactionsActorState(); static_cast<KvMutationLogToTransactionsActor*>(this)->destroy(); return 0; }
															#line 3593 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			new (&static_cast<KvMutationLogToTransactionsActor*>(this)->SAV< int >::value()) int(std::move(totalBytes)); // state_var_RVO
			this->~KvMutationLogToTransactionsActorState();
			static_cast<KvMutationLogToTransactionsActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 861 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (endOfStream)
															#line 3607 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 862 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<KvMutationLogToTransactionsActor*>(this)->SAV<int>::futures) { (void)(totalBytes); this->~KvMutationLogToTransactionsActorState(); static_cast<KvMutationLogToTransactionsActor*>(this)->destroy(); return 0; }
															#line 3611 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			new (&static_cast<KvMutationLogToTransactionsActor*>(this)->SAV< int >::value()) int(std::move(totalBytes)); // state_var_RVO
			this->~KvMutationLogToTransactionsActorState();
			static_cast<KvMutationLogToTransactionsActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state > 0) static_cast<KvMutationLogToTransactionsActor*>(this)->actor_wait_state = 0;
		static_cast<KvMutationLogToTransactionsActor*>(this)->ActorCallback< KvMutationLogToTransactionsActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< KvMutationLogToTransactionsActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< KvMutationLogToTransactionsActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< KvMutationLogToTransactionsActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Database cx;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	PromiseStream<RCGroup> results;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Reference<FlowLock> lock;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key uid;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key addPrefix;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key removePrefix;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	PublicRequestStream<CommitTransactionRequest> commit;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	NotifiedVersion* committedVersion;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Optional<Version> endVersion;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key rangeBegin;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	PromiseStream<Future<Void>> addActor;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	FlowLock* commitLock;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Reference<KeyRangeMap<Version>> keyVersion;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	std::map<int64_t, TenantName>* tenantMap;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	bool provisionalProxy;
															#line 759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Version lastVersion;
															#line 760 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	bool endOfStream;
															#line 761 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	int totalBytes;
															#line 763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	CommitTransactionRequest req;
															#line 764 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Version newBeginVersion;
															#line 765 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	int mutationSize;
															#line 766 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	bool tenantMapChanging;
															#line 769 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	RCGroup group;
															#line 770 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	CommitTransactionRequest curReq;
															#line 772 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	int curBatchMutationSize;
															#line 822 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	int i;
															#line 3736 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via kvMutationLogToTransactions()
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class KvMutationLogToTransactionsActor final : public Actor<int>, public ActorSingleCallback< KvMutationLogToTransactionsActor, 0, RCGroup >, public ActorCallback< KvMutationLogToTransactionsActor, 1, Void >, public ActorCallback< KvMutationLogToTransactionsActor, 2, Void >, public ActorCallback< KvMutationLogToTransactionsActor, 3, Void >, public FastAllocated<KvMutationLogToTransactionsActor>, public KvMutationLogToTransactionsActorState<KvMutationLogToTransactionsActor> {
															#line 3741 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
	using FastAllocated<KvMutationLogToTransactionsActor>::operator new;
	using FastAllocated<KvMutationLogToTransactionsActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< KvMutationLogToTransactionsActor, 0, RCGroup >;
friend struct ActorCallback< KvMutationLogToTransactionsActor, 1, Void >;
friend struct ActorCallback< KvMutationLogToTransactionsActor, 2, Void >;
friend struct ActorCallback< KvMutationLogToTransactionsActor, 3, Void >;
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	KvMutationLogToTransactionsActor(Database const& cx,PromiseStream<RCGroup> const& results,Reference<FlowLock> const& lock,Key const& uid,Key const& addPrefix,Key const& removePrefix,PublicRequestStream<CommitTransactionRequest> const& commit,NotifiedVersion* const& committedVersion,Optional<Version> const& endVersion,Key const& rangeBegin,PromiseStream<Future<Void>> const& addActor,FlowLock* const& commitLock,Reference<KeyRangeMap<Version>> const& keyVersion,std::map<int64_t, TenantName>* const& tenantMap,bool const& provisionalProxy) 
															#line 3755 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		 : Actor<int>(),
		   KvMutationLogToTransactionsActorState<KvMutationLogToTransactionsActor>(cx, results, lock, uid, addPrefix, removePrefix, commit, committedVersion, endVersion, rangeBegin, addActor, commitLock, keyVersion, tenantMap, provisionalProxy)
	{
		fdb_probe_actor_enter("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("kvMutationLogToTransactions");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("kvMutationLogToTransactions", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< KvMutationLogToTransactionsActor, 0, RCGroup >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< KvMutationLogToTransactionsActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< KvMutationLogToTransactionsActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< KvMutationLogToTransactionsActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] Future<int> kvMutationLogToTransactions( Database const& cx, PromiseStream<RCGroup> const& results, Reference<FlowLock> const& lock, Key const& uid, Key const& addPrefix, Key const& removePrefix, PublicRequestStream<CommitTransactionRequest> const& commit, NotifiedVersion* const& committedVersion, Optional<Version> const& endVersion, Key const& rangeBegin, PromiseStream<Future<Void>> const& addActor, FlowLock* const& commitLock, Reference<KeyRangeMap<Version>> const& keyVersion, std::map<int64_t, TenantName>* const& tenantMap, bool const& provisionalProxy ) {
															#line 744 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	return Future<int>(new KvMutationLogToTransactionsActor(cx, results, lock, uid, addPrefix, removePrefix, commit, committedVersion, endVersion, rangeBegin, addActor, commitLock, keyVersion, tenantMap, provisionalProxy));
															#line 3786 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 866 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"

															#line 3791 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via coalesceKeyVersionCache()
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
template <class CoalesceKeyVersionCacheActor>
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class CoalesceKeyVersionCacheActorState {
															#line 3798 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	CoalesceKeyVersionCacheActorState(Key const& uid,Version const& endVersion,Reference<KeyRangeMap<Version>> const& keyVersion,PublicRequestStream<CommitTransactionRequest> const& commit,NotifiedVersion* const& committedVersion,PromiseStream<Future<Void>> const& addActor,FlowLock* const& commitLock) 
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		 : uid(uid),
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   endVersion(endVersion),
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   keyVersion(keyVersion),
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   commit(commit),
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   committedVersion(committedVersion),
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   addActor(addActor),
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   commitLock(commitLock)
															#line 3817 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
	{
		fdb_probe_actor_create("coalesceKeyVersionCache", reinterpret_cast<unsigned long>(this));

	}
	~CoalesceKeyVersionCacheActorState() 
	{
		fdb_probe_actor_destroy("coalesceKeyVersionCache", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			Version lastVersion = -1000;
															#line 875 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			int64_t removed = 0;
															#line 876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			req = CommitTransactionRequest();
															#line 877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			mutationSize = 0;
															#line 878 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			Key mapPrefix = uid.withPrefix(applyMutationsKeyVersionMapRange.begin);
															#line 880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			for( auto it : keyVersion->ranges() ) {
															#line 881 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (lastVersion == -1000)
															#line 3844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 882 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					lastVersion = it.value();
															#line 3848 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
				else
				{
															#line 884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					Version ver = it.value();
															#line 885 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					if (ver < endVersion && lastVersion < endVersion && ver != invalidVersion && lastVersion != invalidVersion)
															#line 3856 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					{
															#line 887 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						Key removeKey = it.range().begin.withPrefix(mapPrefix);
															#line 888 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						Key removeEnd = keyAfter(removeKey);
															#line 889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						req.transaction.mutations.push_back_deep(req.arena, MutationRef(MutationRef::ClearRange, removeKey, removeEnd));
															#line 891 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						mutationSize += removeKey.size() + removeEnd.size();
															#line 892 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						removed--;
															#line 3868 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					}
					else
					{
															#line 894 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						lastVersion = ver;
															#line 3874 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					}
				}
			}
															#line 899 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (removed != 0)
															#line 3880 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 900 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				Key countKey = uid.withPrefix(applyMutationsKeyVersionCountRange.begin);
															#line 901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				req.transaction.write_conflict_ranges.push_back_deep(req.arena, singleKeyRange(countKey));
															#line 902 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				req.transaction.mutations.push_back_deep( req.arena, MutationRef(MutationRef::AddValue, countKey, StringRef((uint8_t*)&removed, 8)));
															#line 904 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				req.transaction.read_snapshot = committedVersion->get();
															#line 905 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				req.flags = req.flags | CommitTransactionRequest::FLAG_IS_LOCK_AWARE;
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				StrictFuture<Void> __when_expr_0 = commitLock->take(TaskPriority::DefaultYield, mutationSize);
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (static_cast<CoalesceKeyVersionCacheActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3896 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<CoalesceKeyVersionCacheActor*>(this)->actor_wait_state = 1;
															#line 907 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CoalesceKeyVersionCacheActor, 0, Void >*>(static_cast<CoalesceKeyVersionCacheActor*>(this)));
															#line 3901 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
		this->~CoalesceKeyVersionCacheActorState();
		static_cast<CoalesceKeyVersionCacheActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 911 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!static_cast<CoalesceKeyVersionCacheActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CoalesceKeyVersionCacheActorState(); static_cast<CoalesceKeyVersionCacheActor*>(this)->destroy(); return 0; }
															#line 3929 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		new (&static_cast<CoalesceKeyVersionCacheActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CoalesceKeyVersionCacheActorState();
		static_cast<CoalesceKeyVersionCacheActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		addActor.send(commitLock->releaseWhen(success(commit.getReply(req)), mutationSize));
															#line 3941 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 908 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		addActor.send(commitLock->releaseWhen(success(commit.getReply(req)), mutationSize));
															#line 3950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CoalesceKeyVersionCacheActor*>(this)->actor_wait_state > 0) static_cast<CoalesceKeyVersionCacheActor*>(this)->actor_wait_state = 0;
		static_cast<CoalesceKeyVersionCacheActor*>(this)->ActorCallback< CoalesceKeyVersionCacheActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CoalesceKeyVersionCacheActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("coalesceKeyVersionCache", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("coalesceKeyVersionCache", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CoalesceKeyVersionCacheActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("coalesceKeyVersionCache", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("coalesceKeyVersionCache", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CoalesceKeyVersionCacheActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("coalesceKeyVersionCache", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("coalesceKeyVersionCache", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key uid;
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Version endVersion;
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Reference<KeyRangeMap<Version>> keyVersion;
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	PublicRequestStream<CommitTransactionRequest> commit;
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	NotifiedVersion* committedVersion;
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	PromiseStream<Future<Void>> addActor;
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	FlowLock* commitLock;
															#line 876 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	CommitTransactionRequest req;
															#line 877 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	int64_t mutationSize;
															#line 4036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via coalesceKeyVersionCache()
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class CoalesceKeyVersionCacheActor final : public Actor<Void>, public ActorCallback< CoalesceKeyVersionCacheActor, 0, Void >, public FastAllocated<CoalesceKeyVersionCacheActor>, public CoalesceKeyVersionCacheActorState<CoalesceKeyVersionCacheActor> {
															#line 4041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
	using FastAllocated<CoalesceKeyVersionCacheActor>::operator new;
	using FastAllocated<CoalesceKeyVersionCacheActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CoalesceKeyVersionCacheActor, 0, Void >;
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	CoalesceKeyVersionCacheActor(Key const& uid,Version const& endVersion,Reference<KeyRangeMap<Version>> const& keyVersion,PublicRequestStream<CommitTransactionRequest> const& commit,NotifiedVersion* const& committedVersion,PromiseStream<Future<Void>> const& addActor,FlowLock* const& commitLock) 
															#line 4052 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		 : Actor<Void>(),
		   CoalesceKeyVersionCacheActorState<CoalesceKeyVersionCacheActor>(uid, endVersion, keyVersion, commit, committedVersion, addActor, commitLock)
	{
		fdb_probe_actor_enter("coalesceKeyVersionCache", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("coalesceKeyVersionCache");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("coalesceKeyVersionCache", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CoalesceKeyVersionCacheActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] Future<Void> coalesceKeyVersionCache( Key const& uid, Version const& endVersion, Reference<KeyRangeMap<Version>> const& keyVersion, PublicRequestStream<CommitTransactionRequest> const& commit, NotifiedVersion* const& committedVersion, PromiseStream<Future<Void>> const& addActor, FlowLock* const& commitLock ) {
															#line 867 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new CoalesceKeyVersionCacheActor(uid, endVersion, keyVersion, commit, committedVersion, addActor, commitLock));
															#line 4080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 913 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"

															#line 4085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via applyMutations()
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
template <class ApplyMutationsActor>
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class ApplyMutationsActorState {
															#line 4092 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	ApplyMutationsActorState(Database const& cx,Key const& uid,Key const& addPrefix,Key const& removePrefix,Version const& beginVersion,Version* const& endVersion,PublicRequestStream<CommitTransactionRequest> const& commit,NotifiedVersion* const& committedVersion,Reference<KeyRangeMap<Version>> const& keyVersion,std::map<int64_t, TenantName>* const& tenantMap,bool const& provisionalProxy) 
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		 : cx(cx),
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   uid(uid),
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   addPrefix(addPrefix),
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   removePrefix(removePrefix),
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   beginVersion(beginVersion),
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   endVersion(endVersion),
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   commit(commit),
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   committedVersion(committedVersion),
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   keyVersion(keyVersion),
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   tenantMap(tenantMap),
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   provisionalProxy(provisionalProxy),
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   commitLock(CLIENT_KNOBS->BACKUP_LOCK_BYTES),
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   addActor(),
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   error(actorCollection(addActor.getFuture())),
															#line 928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   maxBytes(CLIENT_KNOBS->APPLY_MIN_LOCK_BYTES)
															#line 4127 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
	{
		fdb_probe_actor_create("applyMutations", reinterpret_cast<unsigned long>(this));

	}
	~ApplyMutationsActorState() 
	{
		fdb_probe_actor_destroy("applyMutations", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 930 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			keyVersion->insert(metadataVersionKey, 0);
															#line 4142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			try {
															#line 933 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				;
															#line 4146 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
		this->~ApplyMutationsActorState();
		static_cast<ApplyMutationsActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 990 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			TraceEvent(e.code() == error_code_restore_missing_data ? SevWarnAlways : SevError, "ApplyMutationsError") .error(e);
															#line 992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 4178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
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
															#line 934 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (beginVersion >= *endVersion)
															#line 4199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_0 = commitLock.take(TaskPriority::DefaultYield, CLIENT_KNOBS->BACKUP_LOCK_BYTES);
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<ApplyMutationsActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4205 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ApplyMutationsActor*>(this)->actor_wait_state = 1;
															#line 935 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ApplyMutationsActor, 0, Void >*>(static_cast<ApplyMutationsActor*>(this)));
															#line 4210 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 942 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		int rangeCount = std::max(1, CLIENT_KNOBS->APPLY_MAX_LOCK_BYTES / maxBytes);
															#line 943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		newEndVersion = std::min(*endVersion, ((beginVersion / CLIENT_KNOBS->APPLY_BLOCK_SIZE) + rangeCount) * CLIENT_KNOBS->APPLY_BLOCK_SIZE);
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		ranges = getApplyRanges(beginVersion, newEndVersion, uid);
															#line 947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		idx = size_t();
															#line 948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		results = std::vector<PromiseStream<RCGroup>>();
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		rc = std::vector<Future<Void>>();
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		locks = std::vector<Reference<FlowLock>>();
															#line 952 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		for(int i = 0;i < ranges.size();++i) {
															#line 953 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			results.push_back(PromiseStream<RCGroup>());
															#line 954 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			locks.push_back(makeReference<FlowLock>( std::max(CLIENT_KNOBS->APPLY_MAX_LOCK_BYTES / ranges.size(), CLIENT_KNOBS->APPLY_MIN_LOCK_BYTES)));
															#line 956 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			rc.push_back(readCommitted(cx, results[i], locks[i], ranges[i], decodeBKMutationLogKey));
															#line 4244 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 959 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		maxBytes = std::max<int>(maxBytes * CLIENT_KNOBS->APPLY_MAX_DECAY_RATE, CLIENT_KNOBS->APPLY_MIN_LOCK_BYTES);
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		idx = 0;
															#line 4250 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		commitLock.release(CLIENT_KNOBS->BACKUP_LOCK_BYTES);
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (beginVersion >= *endVersion)
															#line 4261 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<ApplyMutationsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ApplyMutationsActorState(); static_cast<ApplyMutationsActor*>(this)->destroy(); return 0; }
															#line 4265 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			new (&static_cast<ApplyMutationsActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ApplyMutationsActorState();
			static_cast<ApplyMutationsActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 936 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		commitLock.release(CLIENT_KNOBS->BACKUP_LOCK_BYTES);
															#line 937 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (beginVersion >= *endVersion)
															#line 4281 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 938 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<ApplyMutationsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ApplyMutationsActorState(); static_cast<ApplyMutationsActor*>(this)->destroy(); return 0; }
															#line 4285 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			new (&static_cast<ApplyMutationsActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~ApplyMutationsActorState();
			static_cast<ApplyMutationsActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

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
		if (static_cast<ApplyMutationsActor*>(this)->actor_wait_state > 0) static_cast<ApplyMutationsActor*>(this)->actor_wait_state = 0;
		static_cast<ApplyMutationsActor*>(this)->ActorCallback< ApplyMutationsActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ApplyMutationsActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("applyMutations", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("applyMutations", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ApplyMutationsActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("applyMutations", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("applyMutations", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ApplyMutationsActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("applyMutations", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("applyMutations", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont5(int loopDepth) 
	{
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<Void> __when_expr_2 = coalesceKeyVersionCache( uid, newEndVersion, keyVersion, commit, committedVersion, addActor, &commitLock);
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<ApplyMutationsActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4364 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont5when1(__when_expr_2.get(), loopDepth); };
		static_cast<ApplyMutationsActor*>(this)->actor_wait_state = 3;
															#line 982 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ApplyMutationsActor, 2, Void >*>(static_cast<ApplyMutationsActor*>(this)));
															#line 4369 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1(int loopDepth) 
	{
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!(idx < ranges.size()))
															#line 4385 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
			return a_body1loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<int> __when_expr_1 = kvMutationLogToTransactions(cx, results[idx], locks[idx], uid, addPrefix, removePrefix, commit, committedVersion, idx == ranges.size() - 1 ? newEndVersion : Optional<Version>(), ranges[idx].begin, addActor, &commitLock, keyVersion, tenantMap, provisionalProxy);
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<ApplyMutationsActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 4393 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ApplyMutationsActor*>(this)->actor_wait_state = 2;
															#line 961 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ApplyMutationsActor, 1, int >*>(static_cast<ApplyMutationsActor*>(this)));
															#line 4398 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont5(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch2(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch2(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont1(int const& bytes,int loopDepth) 
	{
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		maxBytes = std::max<int>(CLIENT_KNOBS->APPLY_MAX_INCREASE_FACTOR * bytes, maxBytes);
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (error.isError())
															#line 4422 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			return a_body1Catch2(error.getError(), std::max(0, loopDepth - 2));
															#line 4426 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		++idx;
															#line 4430 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont1(int && bytes,int loopDepth) 
	{
															#line 977 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		maxBytes = std::max<int>(CLIENT_KNOBS->APPLY_MAX_INCREASE_FACTOR * bytes, maxBytes);
															#line 978 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (error.isError())
															#line 4441 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 979 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			return a_body1Catch2(error.getError(), std::max(0, loopDepth - 2));
															#line 4445 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 960 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		++idx;
															#line 4449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when1(int const& bytes,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont1(bytes, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when1(int && bytes,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont1(std::move(bytes), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ApplyMutationsActor*>(this)->actor_wait_state > 0) static_cast<ApplyMutationsActor*>(this)->actor_wait_state = 0;
		static_cast<ApplyMutationsActor*>(this)->ActorCallback< ApplyMutationsActor, 1, int >::remove();

	}
	void a_callback_fire(ActorCallback< ApplyMutationsActor, 1, int >*,int const& value) 
	{
		fdb_probe_actor_enter("applyMutations", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("applyMutations", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ApplyMutationsActor, 1, int >*,int && value) 
	{
		fdb_probe_actor_enter("applyMutations", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("applyMutations", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ApplyMutationsActor, 1, int >*,Error err) 
	{
		fdb_probe_actor_enter("applyMutations", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("applyMutations", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont7(Void const& _,int loopDepth) 
	{
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		beginVersion = newEndVersion;
															#line 985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (BUGGIFY)
															#line 4523 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_3 = delay(2.0);
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<ApplyMutationsActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4529 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont7when1(__when_expr_3.get(), loopDepth); };
			static_cast<ApplyMutationsActor*>(this)->actor_wait_state = 4;
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ApplyMutationsActor, 3, Void >*>(static_cast<ApplyMutationsActor*>(this)));
															#line 4534 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont8(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void && _,int loopDepth) 
	{
															#line 984 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		beginVersion = newEndVersion;
															#line 985 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (BUGGIFY)
															#line 4550 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_3 = delay(2.0);
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<ApplyMutationsActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4556 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont7when1(__when_expr_3.get(), loopDepth); };
			static_cast<ApplyMutationsActor*>(this)->actor_wait_state = 4;
															#line 986 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ApplyMutationsActor, 3, Void >*>(static_cast<ApplyMutationsActor*>(this)));
															#line 4561 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont8(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont5when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ApplyMutationsActor*>(this)->actor_wait_state > 0) static_cast<ApplyMutationsActor*>(this)->actor_wait_state = 0;
		static_cast<ApplyMutationsActor*>(this)->ActorCallback< ApplyMutationsActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ApplyMutationsActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("applyMutations", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("applyMutations", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ApplyMutationsActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("applyMutations", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("applyMutations", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ApplyMutationsActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("applyMutations", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("applyMutations", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont8(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont9(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ApplyMutationsActor*>(this)->actor_wait_state > 0) static_cast<ApplyMutationsActor*>(this)->actor_wait_state = 0;
		static_cast<ApplyMutationsActor*>(this)->ActorCallback< ApplyMutationsActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ApplyMutationsActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("applyMutations", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont7when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("applyMutations", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ApplyMutationsActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("applyMutations", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont7when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("applyMutations", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ApplyMutationsActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("applyMutations", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("applyMutations", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Database cx;
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key uid;
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key addPrefix;
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key removePrefix;
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Version beginVersion;
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Version* endVersion;
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	PublicRequestStream<CommitTransactionRequest> commit;
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	NotifiedVersion* committedVersion;
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Reference<KeyRangeMap<Version>> keyVersion;
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	std::map<int64_t, TenantName>* tenantMap;
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	bool provisionalProxy;
															#line 925 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	FlowLock commitLock;
															#line 926 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	PromiseStream<Future<Void>> addActor;
															#line 927 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Future<Void> error;
															#line 928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	int maxBytes;
															#line 943 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Version newEndVersion;
															#line 946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Standalone<VectorRef<KeyRangeRef>> ranges;
															#line 947 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	size_t idx;
															#line 948 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	std::vector<PromiseStream<RCGroup>> results;
															#line 949 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	std::vector<Future<Void>> rc;
															#line 950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	std::vector<Reference<FlowLock>> locks;
															#line 4757 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via applyMutations()
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class ApplyMutationsActor final : public Actor<Void>, public ActorCallback< ApplyMutationsActor, 0, Void >, public ActorCallback< ApplyMutationsActor, 1, int >, public ActorCallback< ApplyMutationsActor, 2, Void >, public ActorCallback< ApplyMutationsActor, 3, Void >, public FastAllocated<ApplyMutationsActor>, public ApplyMutationsActorState<ApplyMutationsActor> {
															#line 4762 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
	using FastAllocated<ApplyMutationsActor>::operator new;
	using FastAllocated<ApplyMutationsActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ApplyMutationsActor, 0, Void >;
friend struct ActorCallback< ApplyMutationsActor, 1, int >;
friend struct ActorCallback< ApplyMutationsActor, 2, Void >;
friend struct ActorCallback< ApplyMutationsActor, 3, Void >;
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	ApplyMutationsActor(Database const& cx,Key const& uid,Key const& addPrefix,Key const& removePrefix,Version const& beginVersion,Version* const& endVersion,PublicRequestStream<CommitTransactionRequest> const& commit,NotifiedVersion* const& committedVersion,Reference<KeyRangeMap<Version>> const& keyVersion,std::map<int64_t, TenantName>* const& tenantMap,bool const& provisionalProxy) 
															#line 4776 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		 : Actor<Void>(),
		   ApplyMutationsActorState<ApplyMutationsActor>(cx, uid, addPrefix, removePrefix, beginVersion, endVersion, commit, committedVersion, keyVersion, tenantMap, provisionalProxy)
	{
		fdb_probe_actor_enter("applyMutations", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("applyMutations");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("applyMutations", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ApplyMutationsActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ApplyMutationsActor, 1, int >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ApplyMutationsActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ApplyMutationsActor, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] Future<Void> applyMutations( Database const& cx, Key const& uid, Key const& addPrefix, Key const& removePrefix, Version const& beginVersion, Version* const& endVersion, PublicRequestStream<CommitTransactionRequest> const& commit, NotifiedVersion* const& committedVersion, Reference<KeyRangeMap<Version>> const& keyVersion, std::map<int64_t, TenantName>* const& tenantMap, bool const& provisionalProxy ) {
															#line 914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new ApplyMutationsActor(cx, uid, addPrefix, removePrefix, beginVersion, endVersion, commit, committedVersion, keyVersion, tenantMap, provisionalProxy));
															#line 4807 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 995 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"

															#line 4812 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via _eraseLogData()
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
template <class _eraseLogDataActor>
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class _eraseLogDataActorState {
															#line 4819 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	_eraseLogDataActorState(Reference<ReadYourWritesTransaction> const& tr,Key const& logUidValue,Key const& destUidValue,Optional<Version> const& endVersion,CheckBackupUID const& checkBackupUid,Version const& backupUid) 
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		 : tr(tr),
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   logUidValue(logUidValue),
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   destUidValue(destUidValue),
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   endVersion(endVersion),
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   checkBackupUid(checkBackupUid),
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   backupUid(backupUid),
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   backupLatestVersionsPath(destUidValue.withPrefix(backupLatestVersionsPrefix)),
															#line 1003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   backupLatestVersionsKey(logUidValue.withPrefix(backupLatestVersionsPath))
															#line 4840 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
	{
		fdb_probe_actor_create("_eraseLogData", reinterpret_cast<unsigned long>(this));

	}
	~_eraseLogDataActorState() 
	{
		fdb_probe_actor_destroy("_eraseLogData", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1005 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (!destUidValue.size())
															#line 4855 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 1006 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (!static_cast<_eraseLogDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_eraseLogDataActorState(); static_cast<_eraseLogDataActor*>(this)->destroy(); return 0; }
															#line 4859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				new (&static_cast<_eraseLogDataActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~_eraseLogDataActorState();
				static_cast<_eraseLogDataActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 1009 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1010 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1012 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (checkBackupUid)
															#line 4871 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 1013 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				Subspace sourceStates = Subspace(databaseBackupPrefixRange.begin).get(BackupAgentBase::keySourceStates).get(logUidValue);
															#line 1015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				StrictFuture<Optional<Value>> __when_expr_0 = tr->get(sourceStates.pack(DatabaseBackupAgent::keyFolderId));
															#line 1015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (static_cast<_eraseLogDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<_eraseLogDataActor*>(this)->actor_wait_state = 1;
															#line 1015 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< _eraseLogDataActor, 0, Optional<Value> >*>(static_cast<_eraseLogDataActor*>(this)));
															#line 4884 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
		this->~_eraseLogDataActorState();
		static_cast<_eraseLogDataActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr->getRange(KeyRangeRef(backupLatestVersionsPath, strinc(backupLatestVersionsPath)), CLIENT_KNOBS->TOO_MANY);
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<_eraseLogDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4914 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<_eraseLogDataActor*>(this)->actor_wait_state = 2;
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< _eraseLogDataActor, 1, RangeResult >*>(static_cast<_eraseLogDataActor*>(this)));
															#line 4919 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Optional<Value> const& v,int loopDepth) 
	{
															#line 1016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (v.present() && BinaryReader::fromStringRef<Version>(v.get(), Unversioned()) > backupUid)
															#line 4928 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 1017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<_eraseLogDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_eraseLogDataActorState(); static_cast<_eraseLogDataActor*>(this)->destroy(); return 0; }
															#line 4932 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			new (&static_cast<_eraseLogDataActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~_eraseLogDataActorState();
			static_cast<_eraseLogDataActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(Optional<Value> && v,int loopDepth) 
	{
															#line 1016 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (v.present() && BinaryReader::fromStringRef<Version>(v.get(), Unversioned()) > backupUid)
															#line 4946 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 1017 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<_eraseLogDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_eraseLogDataActorState(); static_cast<_eraseLogDataActor*>(this)->destroy(); return 0; }
															#line 4950 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			new (&static_cast<_eraseLogDataActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~_eraseLogDataActorState();
			static_cast<_eraseLogDataActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& v,int loopDepth) 
	{
		loopDepth = a_body1cont3(v, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && v,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(v), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<_eraseLogDataActor*>(this)->actor_wait_state > 0) static_cast<_eraseLogDataActor*>(this)->actor_wait_state = 0;
		static_cast<_eraseLogDataActor*>(this)->ActorCallback< _eraseLogDataActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< _eraseLogDataActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("_eraseLogData", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_eraseLogData", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< _eraseLogDataActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("_eraseLogData", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_eraseLogData", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< _eraseLogDataActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("_eraseLogData", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_eraseLogData", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont6(int loopDepth) 
	{
															#line 1024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		currBeginVersion = invalidVersion;
															#line 1025 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		for( auto backupVersion : backupVersions ) {
															#line 1026 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			Key currLogUidValue = backupVersion.key.removePrefix(backupLatestVersionsPrefix).removePrefix(destUidValue);
															#line 1028 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (currLogUidValue == logUidValue)
															#line 5033 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 1029 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				currBeginVersion = BinaryReader::fromStringRef<Version>(backupVersion.value, Unversioned());
															#line 5037 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				break;
			}
		}
															#line 1035 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (currBeginVersion == invalidVersion)
															#line 5043 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 1036 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<_eraseLogDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_eraseLogDataActorState(); static_cast<_eraseLogDataActor*>(this)->destroy(); return 0; }
															#line 5047 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			new (&static_cast<_eraseLogDataActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~_eraseLogDataActorState();
			static_cast<_eraseLogDataActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 1039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		currEndVersion = std::numeric_limits<Version>::max();
															#line 1040 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (endVersion.present())
															#line 5057 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 1041 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			currEndVersion = std::min(currEndVersion, endVersion.get());
															#line 5061 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 1044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		nextSmallestVersion = currEndVersion;
															#line 1045 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		bool clearLogRangesRequired = true;
															#line 1048 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (backupVersions.size() > 1)
															#line 5069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 1049 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			for( auto backupVersion : backupVersions ) {
															#line 1050 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				Key currLogUidValue = backupVersion.key.removePrefix(backupLatestVersionsPrefix).removePrefix(destUidValue);
															#line 1051 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				Version currVersion = BinaryReader::fromStringRef<Version>(backupVersion.value, Unversioned());
															#line 1053 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (currLogUidValue == logUidValue)
															#line 5079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
					continue;
				}
				else
				{
															#line 1055 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					if (currVersion > currBeginVersion)
															#line 5087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					{
															#line 1056 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						nextSmallestVersion = std::min(currVersion, nextSmallestVersion);
															#line 5091 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					}
					else
					{
															#line 1059 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						clearLogRangesRequired = false;
															#line 5097 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
						break;
					}
				}
			}
		}
															#line 1065 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (endVersion.present() || backupVersions.size() != 1 || BUGGIFY)
															#line 5105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 1066 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (!endVersion.present())
															#line 5109 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 1068 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				tr->clear(backupLatestVersionsKey);
															#line 1069 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (backupVersions.size() == 1)
															#line 5115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 1070 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					tr->clear(prefixRange(destUidValue.withPrefix(logRangesRange.begin)));
															#line 5119 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
			}
			else
			{
															#line 1074 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				tr->set(backupLatestVersionsKey, BinaryWriter::toValue<Version>(currEndVersion, Unversioned()));
															#line 5126 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 1078 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (clearLogRangesRequired)
															#line 5130 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 1079 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if ((nextSmallestVersion - currBeginVersion) / CLIENT_KNOBS->LOG_RANGE_BLOCK_SIZE >= std::numeric_limits<uint8_t>::max() || BUGGIFY)
															#line 5134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 1082 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					Key baLogRangePrefix = destUidValue.withPrefix(backupLogKeys.begin);
															#line 1084 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					for(int h = 0;h <= std::numeric_limits<uint8_t>::max();h++) {
															#line 1085 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						uint64_t bv = bigEndian64(Version(0));
															#line 1086 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						uint64_t ev = bigEndian64(nextSmallestVersion);
															#line 1087 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						uint8_t h1 = h;
															#line 1088 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						Key vblockPrefix = StringRef(&h1, sizeof(uint8_t)).withPrefix(baLogRangePrefix);
															#line 1089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						tr->clear(KeyRangeRef(StringRef((uint8_t*)&bv, sizeof(uint64_t)).withPrefix(vblockPrefix), StringRef((uint8_t*)&ev, sizeof(uint64_t)).withPrefix(vblockPrefix)));
															#line 5150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					}
				}
				else
				{
															#line 1093 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					Standalone<VectorRef<KeyRangeRef>> ranges = getLogRanges(currBeginVersion, nextSmallestVersion, destUidValue);
															#line 1095 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					for( auto& range : ranges ) {
															#line 1096 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
						tr->clear(range);
															#line 5161 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
					}
				}
			}
		}
		else
		{
															#line 1102 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			tr->clear(prefixRange(backupLatestVersionsPath));
															#line 1105 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			tr->clear(prefixRange(destUidValue.withPrefix(backupLogKeys.begin)));
															#line 1108 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			tr->clear(prefixRange(destUidValue.withPrefix(logRangesRange.begin)));
															#line 5174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 1111 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!endVersion.present() && backupVersions.size() == 1)
															#line 5178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<RangeResult> __when_expr_2 = tr->getRange(KeyRangeRef(destUidLookupPrefix, strinc(destUidLookupPrefix)), CLIENT_KNOBS->TOO_MANY);
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<_eraseLogDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont6when1(__when_expr_2.get(), loopDepth); };
			static_cast<_eraseLogDataActor*>(this)->actor_wait_state = 3;
															#line 1112 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< _eraseLogDataActor, 2, RangeResult >*>(static_cast<_eraseLogDataActor*>(this)));
															#line 5189 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont7(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult const& __backupVersions,int loopDepth) 
	{
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		backupVersions = __backupVersions;
															#line 5203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult && __backupVersions,int loopDepth) 
	{
		backupVersions = std::move(__backupVersions);
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<_eraseLogDataActor*>(this)->actor_wait_state > 0) static_cast<_eraseLogDataActor*>(this)->actor_wait_state = 0;
		static_cast<_eraseLogDataActor*>(this)->ActorCallback< _eraseLogDataActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< _eraseLogDataActor, 1, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("_eraseLogData", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_eraseLogData", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< _eraseLogDataActor, 1, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("_eraseLogData", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_eraseLogData", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< _eraseLogDataActor, 1, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("_eraseLogData", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_eraseLogData", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont7(int loopDepth) 
	{
															#line 1121 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!static_cast<_eraseLogDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_eraseLogDataActorState(); static_cast<_eraseLogDataActor*>(this)->destroy(); return 0; }
															#line 5270 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		new (&static_cast<_eraseLogDataActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~_eraseLogDataActorState();
		static_cast<_eraseLogDataActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont28(RangeResult const& existingDestUidValues,int loopDepth) 
	{
															#line 1114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		for( auto it : existingDestUidValues ) {
															#line 1115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (it.value == destUidValue)
															#line 5284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 1116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				tr->clear(it.key);
															#line 5288 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
		}
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont28(RangeResult && existingDestUidValues,int loopDepth) 
	{
															#line 1114 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		for( auto it : existingDestUidValues ) {
															#line 1115 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (it.value == destUidValue)
															#line 5301 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 1116 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				tr->clear(it.key);
															#line 5305 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
		}
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(RangeResult const& existingDestUidValues,int loopDepth) 
	{
		loopDepth = a_body1cont28(existingDestUidValues, loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(RangeResult && existingDestUidValues,int loopDepth) 
	{
		loopDepth = a_body1cont28(std::move(existingDestUidValues), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<_eraseLogDataActor*>(this)->actor_wait_state > 0) static_cast<_eraseLogDataActor*>(this)->actor_wait_state = 0;
		static_cast<_eraseLogDataActor*>(this)->ActorCallback< _eraseLogDataActor, 2, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< _eraseLogDataActor, 2, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("_eraseLogData", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_eraseLogData", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< _eraseLogDataActor, 2, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("_eraseLogData", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_eraseLogData", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< _eraseLogDataActor, 2, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("_eraseLogData", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("_eraseLogData", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key logUidValue;
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key destUidValue;
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Optional<Version> endVersion;
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	CheckBackupUID checkBackupUid;
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Version backupUid;
															#line 1002 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key backupLatestVersionsPath;
															#line 1003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key backupLatestVersionsKey;
															#line 1020 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	RangeResult backupVersions;
															#line 1024 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Version currBeginVersion;
															#line 1039 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Version currEndVersion;
															#line 1044 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Version nextSmallestVersion;
															#line 5399 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via _eraseLogData()
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class _eraseLogDataActor final : public Actor<Void>, public ActorCallback< _eraseLogDataActor, 0, Optional<Value> >, public ActorCallback< _eraseLogDataActor, 1, RangeResult >, public ActorCallback< _eraseLogDataActor, 2, RangeResult >, public FastAllocated<_eraseLogDataActor>, public _eraseLogDataActorState<_eraseLogDataActor> {
															#line 5404 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
	using FastAllocated<_eraseLogDataActor>::operator new;
	using FastAllocated<_eraseLogDataActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< _eraseLogDataActor, 0, Optional<Value> >;
friend struct ActorCallback< _eraseLogDataActor, 1, RangeResult >;
friend struct ActorCallback< _eraseLogDataActor, 2, RangeResult >;
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	_eraseLogDataActor(Reference<ReadYourWritesTransaction> const& tr,Key const& logUidValue,Key const& destUidValue,Optional<Version> const& endVersion,CheckBackupUID const& checkBackupUid,Version const& backupUid) 
															#line 5417 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		 : Actor<Void>(),
		   _eraseLogDataActorState<_eraseLogDataActor>(tr, logUidValue, destUidValue, endVersion, checkBackupUid, backupUid)
	{
		fdb_probe_actor_enter("_eraseLogData", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("_eraseLogData");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("_eraseLogData", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< _eraseLogDataActor, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< _eraseLogDataActor, 1, RangeResult >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< _eraseLogDataActor, 2, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] static Future<Void> _eraseLogData( Reference<ReadYourWritesTransaction> const& tr, Key const& logUidValue, Key const& destUidValue, Optional<Version> const& endVersion, CheckBackupUID const& checkBackupUid, Version const& backupUid ) {
															#line 996 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new _eraseLogDataActor(tr, logUidValue, destUidValue, endVersion, checkBackupUid, backupUid));
															#line 5447 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 1123 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"

Future<Void> eraseLogData(Reference<ReadYourWritesTransaction> tr,
                          Key logUidValue,
                          Key destUidValue,
                          Optional<Version> endVersion,
                          CheckBackupUID checkBackupUid,
                          Version backupUid) {
	return _eraseLogData(tr, logUidValue, destUidValue, endVersion, checkBackupUid, backupUid);
}

															#line 5461 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via cleanupLogMutations()
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
template <class CleanupLogMutationsActor>
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class CleanupLogMutationsActorState {
															#line 5468 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	CleanupLogMutationsActorState(Database const& cx,Value const& destUidValue,bool const& deleteData) 
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		 : cx(cx),
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   destUidValue(destUidValue),
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   deleteData(deleteData),
															#line 1134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   backupLatestVersionsPath(destUidValue.withPrefix(backupLatestVersionsPrefix)),
															#line 1136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   tr(new ReadYourWritesTransaction(cx)),
															#line 1137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   removingLogUid(),
															#line 1138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   loggedLogUids()
															#line 5487 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
	{
		fdb_probe_actor_create("cleanupLogMutations", reinterpret_cast<unsigned long>(this));

	}
	~CleanupLogMutationsActorState() 
	{
		fdb_probe_actor_destroy("cleanupLogMutations", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1140 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			;
															#line 5502 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
		this->~CleanupLogMutationsActorState();
		static_cast<CleanupLogMutationsActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1142 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1143 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange( KeyRangeRef(backupLatestVersionsPath, strinc(backupLatestVersionsPath)), CLIENT_KNOBS->TOO_MANY);
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5539 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 1;
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CleanupLogMutationsActor, 0, RangeResult >*>(static_cast<CleanupLogMutationsActor*>(this)));
															#line 5544 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_4 = tr->onError(e);
															#line 1219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5568 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_4.get(), loopDepth); };
			static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 5;
															#line 1219 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< CleanupLogMutationsActor, 4, Void >*>(static_cast<CleanupLogMutationsActor*>(this)));
															#line 5573 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		readVer = tr->getReadVersion().get();
															#line 1149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		minVersion = std::numeric_limits<Version>::max();
															#line 1150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		minVersionLogUid = Key();
															#line 1152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		backupIdx = 0;
															#line 1153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		;
															#line 5596 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& __backupVersions,int loopDepth) 
	{
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		backupVersions = __backupVersions;
															#line 5605 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && __backupVersions,int loopDepth) 
	{
		backupVersions = std::move(__backupVersions);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state > 0) static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 0;
		static_cast<CleanupLogMutationsActor*>(this)->ActorCallback< CleanupLogMutationsActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< CleanupLogMutationsActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CleanupLogMutationsActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CleanupLogMutationsActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 1191 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (deleteData)
															#line 5672 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 1192 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (readVer - minVersion > CLIENT_KNOBS->MIN_CLEANUP_SECONDS * CLIENT_KNOBS->CORE_VERSIONSPERSECOND && (!removingLogUid.present() || minVersionLogUid == removingLogUid.get()))
															#line 5676 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 1194 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				removingLogUid = minVersionLogUid;
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				StrictFuture<Void> __when_expr_2 = eraseLogData(tr, minVersionLogUid, destUidValue);
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5684 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
				static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 3;
															#line 1195 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CleanupLogMutationsActor, 2, Void >*>(static_cast<CleanupLogMutationsActor*>(this)));
															#line 5689 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 1199 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (removingLogUid.present() && minVersionLogUid != removingLogUid.get())
															#line 5696 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 1200 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					printf("\nWARNING: The oldest tag was possibly removed, run again without `--delete-data' to " "check.\n\n");
															#line 5700 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
				else
				{
															#line 1203 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					printf("\nWARNING: Did not delete data because the tag is not at least %.4f hours behind. Change " "`--min-cleanup-seconds' to adjust this threshold.\n\n", CLIENT_KNOBS->MIN_CLEANUP_SECONDS / 3600.0);
															#line 5706 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
				loopDepth = a_body1loopBody1cont5(loopDepth);
			}
		}
		else
		{
															#line 1207 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (readVer - minVersion > CLIENT_KNOBS->MIN_CLEANUP_SECONDS * CLIENT_KNOBS->CORE_VERSIONSPERSECOND)
															#line 5715 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 1209 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				printf("\nPassing `--delete-data' would delete the tag that is %.4f hours behind.\n\n", (readVer - minVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 5719 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
			else
			{
															#line 1212 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				printf("\nPassing `--delete-data' would not delete the tag that is %.4f hours behind. Change " "`--min-cleanup-seconds' to adjust the cleanup threshold.\n\n", (readVer - minVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 5725 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
			loopDepth = a_body1loopBody1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1cont2loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1(int loopDepth) 
	{
															#line 1153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!(backupIdx < backupVersions.size()))
															#line 5743 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
			return a_body1loopBody1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		currVersion = BinaryReader::fromStringRef<Version>(backupVersions[backupIdx].value, Unversioned());
															#line 1156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		currLogUid = backupVersions[backupIdx].key.removePrefix(backupLatestVersionsPrefix).removePrefix(destUidValue);
															#line 1158 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (currVersion < minVersion)
															#line 5753 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 1159 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			minVersionLogUid = currLogUid;
															#line 1160 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			minVersion = currVersion;
															#line 5759 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 1163 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!loggedLogUids.count(currLogUid))
															#line 5763 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 1164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			foundDRKey = tr->get(Subspace(databaseBackupPrefixRange.begin) .get(BackupAgentBase::keySourceStates) .get(currLogUid) .pack(DatabaseBackupAgent::keyStateStatus));
															#line 1168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			foundBackupKey = tr->get( Subspace(currLogUid.withPrefix("uid->config/"_sr).withPrefix(fileBackupPrefixRange.begin)) .pack("stateEnum"_sr));
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_1 = success(foundDRKey) && success(foundBackupKey);
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5773 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 2;
															#line 1171 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CleanupLogMutationsActor, 1, Void >*>(static_cast<CleanupLogMutationsActor*>(this)));
															#line 5778 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont2loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont1(int loopDepth) 
	{
															#line 1153 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		backupIdx++;
															#line 5805 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (foundDRKey.get().present() && foundBackupKey.get().present())
															#line 5814 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 1174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			printf("WARNING: Found a tag that looks like both a backup and a DR. This tag is %.4f hours " "behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 5818 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		}
		else
		{
															#line 1177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (foundDRKey.get().present() && !foundBackupKey.get().present())
															#line 5824 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 1178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				printf("Found a DR that is %.4f hours behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 5828 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
			else
			{
															#line 1180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (!foundDRKey.get().present() && foundBackupKey.get().present())
															#line 5834 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					printf("Found a Backup that is %.4f hours behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 5838 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
				else
				{
															#line 1184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					printf("WARNING: Found an unknown tag that is %.4f hours behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 5844 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
			}
		}
															#line 1187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		loggedLogUids.insert(currLogUid);
															#line 5850 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 1173 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (foundDRKey.get().present() && foundBackupKey.get().present())
															#line 5859 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 1174 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			printf("WARNING: Found a tag that looks like both a backup and a DR. This tag is %.4f hours " "behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 5863 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		}
		else
		{
															#line 1177 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (foundDRKey.get().present() && !foundBackupKey.get().present())
															#line 5869 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 1178 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				printf("Found a DR that is %.4f hours behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 5873 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			}
			else
			{
															#line 1180 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
				if (!foundDRKey.get().present() && foundBackupKey.get().present())
															#line 5879 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 1181 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					printf("Found a Backup that is %.4f hours behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 5883 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
				else
				{
															#line 1184 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
					printf("WARNING: Found an unknown tag that is %.4f hours behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 5889 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
				}
			}
		}
															#line 1187 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		loggedLogUids.insert(currLogUid);
															#line 5895 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont4(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont4(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state > 0) static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 0;
		static_cast<CleanupLogMutationsActor*>(this)->ActorCallback< CleanupLogMutationsActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CleanupLogMutationsActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CleanupLogMutationsActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CleanupLogMutationsActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont4(int loopDepth) 
	{
															#line 1217 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!static_cast<CleanupLogMutationsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CleanupLogMutationsActorState(); static_cast<CleanupLogMutationsActor*>(this)->destroy(); return 0; }
															#line 5967 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		new (&static_cast<CleanupLogMutationsActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CleanupLogMutationsActorState();
		static_cast<CleanupLogMutationsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont5(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
															#line 1196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<Void> __when_expr_3 = tr->commit();
															#line 1196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5987 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 4;
															#line 1196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CleanupLogMutationsActor, 3, Void >*>(static_cast<CleanupLogMutationsActor*>(this)));
															#line 5992 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 1196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<Void> __when_expr_3 = tr->commit();
															#line 1196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 6003 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 4;
															#line 1196 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CleanupLogMutationsActor, 3, Void >*>(static_cast<CleanupLogMutationsActor*>(this)));
															#line 6008 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state > 0) static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 0;
		static_cast<CleanupLogMutationsActor*>(this)->ActorCallback< CleanupLogMutationsActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CleanupLogMutationsActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CleanupLogMutationsActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CleanupLogMutationsActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont7(Void const& _,int loopDepth) 
	{
															#line 1197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		printf("\nSuccessfully removed the tag that was %.4f hours behind.\n\n", (readVer - minVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 6080 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void && _,int loopDepth) 
	{
															#line 1197 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		printf("\nSuccessfully removed the tag that was %.4f hours behind.\n\n", (readVer - minVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 6089 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont5(loopDepth);

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
	void a_exitChoose4() 
	{
		if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state > 0) static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 0;
		static_cast<CleanupLogMutationsActor*>(this)->ActorCallback< CleanupLogMutationsActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CleanupLogMutationsActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< CleanupLogMutationsActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< CleanupLogMutationsActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 3);

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
		if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state > 0) static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 0;
		static_cast<CleanupLogMutationsActor*>(this)->ActorCallback< CleanupLogMutationsActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CleanupLogMutationsActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< CleanupLogMutationsActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< CleanupLogMutationsActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Database cx;
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Value destUidValue;
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	bool deleteData;
															#line 1134 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key backupLatestVersionsPath;
															#line 1136 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1137 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Optional<Key> removingLogUid;
															#line 1138 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	std::set<Key> loggedLogUids;
															#line 1145 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	RangeResult backupVersions;
															#line 1147 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Version readVer;
															#line 1149 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Version minVersion;
															#line 1150 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key minVersionLogUid;
															#line 1152 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	int backupIdx;
															#line 1154 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Version currVersion;
															#line 1156 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Key currLogUid;
															#line 1164 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Future<Optional<Value>> foundDRKey;
															#line 1168 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Future<Optional<Value>> foundBackupKey;
															#line 6264 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via cleanupLogMutations()
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class CleanupLogMutationsActor final : public Actor<Void>, public ActorCallback< CleanupLogMutationsActor, 0, RangeResult >, public ActorCallback< CleanupLogMutationsActor, 1, Void >, public ActorCallback< CleanupLogMutationsActor, 2, Void >, public ActorCallback< CleanupLogMutationsActor, 3, Void >, public ActorCallback< CleanupLogMutationsActor, 4, Void >, public FastAllocated<CleanupLogMutationsActor>, public CleanupLogMutationsActorState<CleanupLogMutationsActor> {
															#line 6269 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
	using FastAllocated<CleanupLogMutationsActor>::operator new;
	using FastAllocated<CleanupLogMutationsActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CleanupLogMutationsActor, 0, RangeResult >;
friend struct ActorCallback< CleanupLogMutationsActor, 1, Void >;
friend struct ActorCallback< CleanupLogMutationsActor, 2, Void >;
friend struct ActorCallback< CleanupLogMutationsActor, 3, Void >;
friend struct ActorCallback< CleanupLogMutationsActor, 4, Void >;
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	CleanupLogMutationsActor(Database const& cx,Value const& destUidValue,bool const& deleteData) 
															#line 6284 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		 : Actor<Void>(),
		   CleanupLogMutationsActorState<CleanupLogMutationsActor>(cx, destUidValue, deleteData)
	{
		fdb_probe_actor_enter("cleanupLogMutations", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("cleanupLogMutations");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("cleanupLogMutations", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CleanupLogMutationsActor, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CleanupLogMutationsActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CleanupLogMutationsActor, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< CleanupLogMutationsActor, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< CleanupLogMutationsActor, 4, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] Future<Void> cleanupLogMutations( Database const& cx, Value const& destUidValue, bool const& deleteData ) {
															#line 1133 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new CleanupLogMutationsActor(cx, destUidValue, deleteData));
															#line 6316 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 1223 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"

															#line 6321 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via cleanupBackup()
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
template <class CleanupBackupActor>
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class CleanupBackupActorState {
															#line 6328 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	CleanupBackupActorState(Database const& cx,DeleteData const& deleteData) 
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		 : cx(cx),
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   deleteData(deleteData),
															#line 1225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		   tr(new ReadYourWritesTransaction(cx))
															#line 6339 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
	{
		fdb_probe_actor_create("cleanupBackup", reinterpret_cast<unsigned long>(this));

	}
	~CleanupBackupActorState() 
	{
		fdb_probe_actor_destroy("cleanupBackup", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 1226 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			;
															#line 6354 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
		this->~CleanupBackupActorState();
		static_cast<CleanupBackupActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 1228 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1229 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(KeyRangeRef(destUidLookupPrefix, strinc(destUidLookupPrefix)), CLIENT_KNOBS->TOO_MANY);
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<CleanupBackupActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 6391 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CleanupBackupActor*>(this)->actor_wait_state = 1;
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CleanupBackupActor, 0, RangeResult >*>(static_cast<CleanupBackupActor*>(this)));
															#line 6396 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_2 = tr->onError(e);
															#line 1239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<CleanupBackupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6420 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<CleanupBackupActor*>(this)->actor_wait_state = 3;
															#line 1239 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CleanupBackupActor, 2, Void >*>(static_cast<CleanupBackupActor*>(this)));
															#line 6425 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		RangeForbody1loopBody1cont2Iterator0 = std::begin(destUids);
															#line 6440 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& __destUids,int loopDepth) 
	{
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		destUids = __destUids;
															#line 6449 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && __destUids,int loopDepth) 
	{
		destUids = std::move(__destUids);
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CleanupBackupActor*>(this)->actor_wait_state > 0) static_cast<CleanupBackupActor*>(this)->actor_wait_state = 0;
		static_cast<CleanupBackupActor*>(this)->ActorCallback< CleanupBackupActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< CleanupBackupActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("cleanupBackup", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupBackup", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CleanupBackupActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("cleanupBackup", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupBackup", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CleanupBackupActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("cleanupBackup", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupBackup", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(int loopDepth) 
	{
															#line 1237 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!static_cast<CleanupBackupActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CleanupBackupActorState(); static_cast<CleanupBackupActor*>(this)->destroy(); return 0; }
															#line 6516 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		new (&static_cast<CleanupBackupActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CleanupBackupActorState();
		static_cast<CleanupBackupActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1cont2loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1(int loopDepth) 
	{
															#line 1234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		if (!(RangeForbody1loopBody1cont2Iterator0 != std::end(destUids)))
															#line 6535 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
			return a_body1loopBody1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		auto destUid = *RangeForbody1loopBody1cont2Iterator0;
															#line 6541 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_1 = cleanupLogMutations(cx, destUid.value, deleteData);
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<CleanupBackupActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6547 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<CleanupBackupActor*>(this)->actor_wait_state = 2;
															#line 1235 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CleanupBackupActor, 1, Void >*>(static_cast<CleanupBackupActor*>(this)));
															#line 6552 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
			loopDepth = 0;
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont3(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont1(int loopDepth) 
	{
															#line 1234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
		++RangeForbody1loopBody1cont2Iterator0;
															#line 6575 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CleanupBackupActor*>(this)->actor_wait_state > 0) static_cast<CleanupBackupActor*>(this)->actor_wait_state = 0;
		static_cast<CleanupBackupActor*>(this)->ActorCallback< CleanupBackupActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CleanupBackupActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cleanupBackup", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupBackup", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CleanupBackupActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cleanupBackup", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupBackup", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CleanupBackupActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cleanupBackup", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupBackup", reinterpret_cast<unsigned long>(this), 1);

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
		if (static_cast<CleanupBackupActor*>(this)->actor_wait_state > 0) static_cast<CleanupBackupActor*>(this)->actor_wait_state = 0;
		static_cast<CleanupBackupActor*>(this)->ActorCallback< CleanupBackupActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CleanupBackupActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("cleanupBackup", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupBackup", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CleanupBackupActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("cleanupBackup", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupBackup", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CleanupBackupActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("cleanupBackup", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("cleanupBackup", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Database cx;
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	DeleteData deleteData;
															#line 1225 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1231 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	RangeResult destUids;
															#line 1234 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	decltype(std::begin(std::declval<RangeResult>())) RangeForbody1loopBody1cont2Iterator0;
															#line 6740 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via cleanupBackup()
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
class CleanupBackupActor final : public Actor<Void>, public ActorCallback< CleanupBackupActor, 0, RangeResult >, public ActorCallback< CleanupBackupActor, 1, Void >, public ActorCallback< CleanupBackupActor, 2, Void >, public FastAllocated<CleanupBackupActor>, public CleanupBackupActorState<CleanupBackupActor> {
															#line 6745 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
public:
	using FastAllocated<CleanupBackupActor>::operator new;
	using FastAllocated<CleanupBackupActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CleanupBackupActor, 0, RangeResult >;
friend struct ActorCallback< CleanupBackupActor, 1, Void >;
friend struct ActorCallback< CleanupBackupActor, 2, Void >;
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	CleanupBackupActor(Database const& cx,DeleteData const& deleteData) 
															#line 6758 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
		 : Actor<Void>(),
		   CleanupBackupActorState<CleanupBackupActor>(cx, deleteData)
	{
		fdb_probe_actor_enter("cleanupBackup", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("cleanupBackup");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("cleanupBackup", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CleanupBackupActor, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CleanupBackupActor, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CleanupBackupActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] Future<Void> cleanupBackup( Database const& cx, DeleteData const& deleteData ) {
															#line 1224 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new CleanupBackupActor(cx, deleteData));
															#line 6788 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 1243 "/home/ccat3z/Documents/moqi/foundationdb-client/src/fdbclient/BackupAgentBase.actor.cpp"

// Convert the status text to an enumerated value
BackupAgentBase::EnumState BackupAgentBase::getState(std::string const& stateText) {
	auto enState = EnumState::STATE_ERRORED;

	if (stateText.empty()) {
		enState = EnumState::STATE_NEVERRAN;
	}

	else if (!stateText.compare("has been submitted")) {
		enState = EnumState::STATE_SUBMITTED;
	}

	else if (!stateText.compare("has been started")) {
		enState = EnumState::STATE_RUNNING;
	}

	else if (!stateText.compare("is differential")) {
		enState = EnumState::STATE_RUNNING_DIFFERENTIAL;
	}

	else if (!stateText.compare("has been completed")) {
		enState = EnumState::STATE_COMPLETED;
	}

	else if (!stateText.compare("has been aborted")) {
		enState = EnumState::STATE_ABORTED;
	}

	else if (!stateText.compare("has been partially aborted")) {
		enState = EnumState::STATE_PARTIALLY_ABORTED;
	}

	return enState;
}

const char* BackupAgentBase::getStateText(EnumState enState) {
	const char* stateText;

	switch (enState) {
	case EnumState::STATE_ERRORED:
		stateText = "has errored";
		break;
	case EnumState::STATE_NEVERRAN:
		stateText = "has never been started";
		break;
	case EnumState::STATE_SUBMITTED:
		stateText = "has been submitted";
		break;
	case EnumState::STATE_RUNNING:
		stateText = "has been started";
		break;
	case EnumState::STATE_RUNNING_DIFFERENTIAL:
		stateText = "is differential";
		break;
	case EnumState::STATE_COMPLETED:
		stateText = "has been completed";
		break;
	case EnumState::STATE_ABORTED:
		stateText = "has been aborted";
		break;
	case EnumState::STATE_PARTIALLY_ABORTED:
		stateText = "has been partially aborted";
		break;
	default:
		stateText = "<undefined>";
		break;
	}

	return stateText;
}

const char* BackupAgentBase::getStateName(EnumState enState) {
	switch (enState) {
	case EnumState::STATE_ERRORED:
		return "Errored";
	case EnumState::STATE_NEVERRAN:
		return "NeverRan";
	case EnumState::STATE_SUBMITTED:
		return "Submitted";
		break;
	case EnumState::STATE_RUNNING:
		return "Running";
	case EnumState::STATE_RUNNING_DIFFERENTIAL:
		return "RunningDifferentially";
	case EnumState::STATE_COMPLETED:
		return "Completed";
	case EnumState::STATE_ABORTED:
		return "Aborted";
	case EnumState::STATE_PARTIALLY_ABORTED:
		return "Aborting";
	default:
		return "<undefined>";
	}
}

bool BackupAgentBase::isRunnable(EnumState enState) {
	switch (enState) {
	case EnumState::STATE_SUBMITTED:
	case EnumState::STATE_RUNNING:
	case EnumState::STATE_RUNNING_DIFFERENTIAL:
	case EnumState::STATE_PARTIALLY_ABORTED:
		return true;
	default:
		return false;
	}
}

Standalone<StringRef> BackupAgentBase::getCurrentTime() {
	double t = now();
	time_t curTime = t;
	char buffer[128];
	struct tm* timeinfo;
	timeinfo = localtime(&curTime);
	strftime(buffer, 128, "%Y-%m-%d-%H-%M-%S", timeinfo);

	std::string time(buffer);
	return StringRef(time + format(".%06d", (int)(1e6 * (t - curTime))));
}

std::string const BackupAgentBase::defaultTagName = "default";

void addDefaultBackupRanges(Standalone<VectorRef<KeyRangeRef>>& backupKeys) {
	backupKeys.push_back_deep(backupKeys.arena(), normalKeys);

	for (auto& r : getSystemBackupRanges()) {
		backupKeys.push_back_deep(backupKeys.arena(), r);
	}
}

VectorRef<KeyRangeRef> const& getSystemBackupRanges() {
	static Standalone<VectorRef<KeyRangeRef>> systemBackupRanges;
	if (systemBackupRanges.empty()) {
		systemBackupRanges.push_back_deep(systemBackupRanges.arena(), prefixRange(TenantMetadata::subspace()));
		systemBackupRanges.push_back_deep(systemBackupRanges.arena(),
		                                  singleKeyRange(metacluster::metadata::metaclusterRegistration().key));
		systemBackupRanges.push_back_deep(systemBackupRanges.arena(), tagQuotaKeys);
		systemBackupRanges.push_back_deep(systemBackupRanges.arena(), blobRangeKeys);
	}

	return systemBackupRanges;
}

KeyRangeMap<bool> const& systemBackupMutationMask() {
	static KeyRangeMap<bool> mask;
	if (mask.size() == 1) {
		for (auto r : getSystemBackupRanges()) {
			mask.insert(r, true);
		}
	}

	return mask;
}

KeyRangeRef const& getDefaultBackupSharedRange() {
	static KeyRangeRef defaultSharedRange(""_sr, ""_sr);
	return defaultSharedRange;
}
