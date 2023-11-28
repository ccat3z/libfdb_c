#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
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
#include "fdbclient/Knobs.h"
#include "fdbrpc/simulator.h"
#include "flow/ActorCollection.h"
#include "flow/actorcompiler.h" // has to be last include

FDB_DEFINE_BOOLEAN_PARAM(LockDB);
FDB_DEFINE_BOOLEAN_PARAM(UnlockDB);
FDB_DEFINE_BOOLEAN_PARAM(StopWhenDone);
FDB_DEFINE_BOOLEAN_PARAM(Verbose);
FDB_DEFINE_BOOLEAN_PARAM(WaitForComplete);
FDB_DEFINE_BOOLEAN_PARAM(ForceAction);
FDB_DEFINE_BOOLEAN_PARAM(Terminator);
FDB_DEFINE_BOOLEAN_PARAM(UsePartitionedLog);
FDB_DEFINE_BOOLEAN_PARAM(InconsistentSnapshotOnly);
FDB_DEFINE_BOOLEAN_PARAM(ShowErrors);
FDB_DEFINE_BOOLEAN_PARAM(AbortOldBackup);
FDB_DEFINE_BOOLEAN_PARAM(DstOnly);
FDB_DEFINE_BOOLEAN_PARAM(WaitForDestUID);
FDB_DEFINE_BOOLEAN_PARAM(CheckBackupUID);
FDB_DEFINE_BOOLEAN_PARAM(DeleteData);
FDB_DEFINE_BOOLEAN_PARAM(SetValidation);
FDB_DEFINE_BOOLEAN_PARAM(PartialBackup);

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

// Given a key from one of the ranges returned by get_log_ranges,
// returns(version, part) where version is the database version number of
// the transaction log data in the value, and part is 0 for the first such
// data for a given version, 1 for the second block of data, etc.
std::pair<Version, uint32_t> decodeBKMutationLogKey(Key key) {
	return std::make_pair(
	    getLogKeyVersion(key),
	    bigEndian32(*(int32_t*)(key.begin() + backupLogPrefixBytes + sizeof(UID) + sizeof(uint8_t) + sizeof(int64_t))));
}

void decodeBackupLogValue(Arena& arena,
                          VectorRef<MutationRef>& result,
                          int& mutationSize,
                          StringRef value,
                          StringRef addPrefix,
                          StringRef removePrefix,
                          Version version,
                          Reference<KeyRangeMap<Version>> key_version) {
	try {
		uint64_t offset(0);
		uint64_t protocolVersion = 0;
		memcpy(&protocolVersion, value.begin(), sizeof(uint64_t));
		offset += sizeof(uint64_t);
		if (protocolVersion <= 0x0FDB00A200090001) {
			TraceEvent(SevError, "DecodeBackupLogValue")
			    .detail("IncompatibleProtocolVersion", protocolVersion)
			    .detail("ValueSize", value.size())
			    .detail("Value", value);
			throw incompatible_protocol_version();
		}

		uint32_t totalBytes = 0;
		memcpy(&totalBytes, value.begin() + offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		uint32_t consumed = 0;

		if (totalBytes + offset > value.size())
			throw restore_missing_data();

		int originalOffset = offset;

		while (consumed < totalBytes) {
			uint32_t type = 0;
			memcpy(&type, value.begin() + offset, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			uint32_t len1 = 0;
			memcpy(&len1, value.begin() + offset, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			uint32_t len2 = 0;
			memcpy(&len2, value.begin() + offset, sizeof(uint32_t));
			offset += sizeof(uint32_t);

			ASSERT(offset + len1 + len2 <= value.size() && isValidMutationType(type));

			MutationRef logValue;
			Arena tempArena;
			logValue.type = type;
			logValue.param1 = value.substr(offset, len1);
			offset += len1;
			logValue.param2 = value.substr(offset, len2);
			offset += len2;

			if (logValue.type == MutationRef::ClearRange) {
				KeyRangeRef range(logValue.param1, logValue.param2);
				auto ranges = key_version->intersectingRanges(range);
				for (auto r : ranges) {
					if (version > r.value() && r.value() != invalidVersion) {
						KeyRef minKey = std::min(r.range().end, range.end);
						if (minKey == (removePrefix == StringRef() ? normalKeys.end : strinc(removePrefix))) {
							logValue.param1 = std::max(r.range().begin, range.begin);
							if (removePrefix.size()) {
								logValue.param1 = logValue.param1.removePrefix(removePrefix);
							}
							if (addPrefix.size()) {
								logValue.param1 = logValue.param1.withPrefix(addPrefix, tempArena);
							}
							logValue.param2 = addPrefix == StringRef() ? normalKeys.end : strinc(addPrefix, tempArena);
							result.push_back_deep(arena, logValue);
							mutationSize += logValue.expectedSize();
						} else {
							logValue.param1 = std::max(r.range().begin, range.begin);
							logValue.param2 = minKey;
							if (removePrefix.size()) {
								logValue.param1 = logValue.param1.removePrefix(removePrefix);
								logValue.param2 = logValue.param2.removePrefix(removePrefix);
							}
							if (addPrefix.size()) {
								logValue.param1 = logValue.param1.withPrefix(addPrefix, tempArena);
								logValue.param2 = logValue.param2.withPrefix(addPrefix, tempArena);
							}
							result.push_back_deep(arena, logValue);
							mutationSize += logValue.expectedSize();
						}
					}
				}
			} else {
				Version ver = key_version->rangeContaining(logValue.param1).value();
				//TraceEvent("ApplyMutation").detail("LogValue", logValue).detail("Version", version).detail("Ver", ver).detail("Apply", version > ver && ver != invalidVersion);
				if (version > ver && ver != invalidVersion) {
					if (removePrefix.size()) {
						logValue.param1 = logValue.param1.removePrefix(removePrefix);
					}
					if (addPrefix.size()) {
						logValue.param1 = logValue.param1.withPrefix(addPrefix, tempArena);
					}
					result.push_back_deep(arena, logValue);
					mutationSize += logValue.expectedSize();
				}
			}

			consumed += BackupAgentBase::logHeaderSize + len1 + len2;
		}

		ASSERT(consumed == totalBytes);
		if (value.size() != offset) {
			TraceEvent(SevError, "BA_DecodeBackupLogValue")
			    .detail("UnexpectedExtraDataSize", value.size())
			    .detail("Offset", offset)
			    .detail("TotalBytes", totalBytes)
			    .detail("Consumed", consumed)
			    .detail("OriginalOffset", originalOffset);
			throw restore_corrupted_data();
		}
	} catch (Error& e) {
		TraceEvent(e.code() == error_code_restore_missing_data ? SevWarn : SevError, "BA_DecodeBackupLogValue")
		    .error(e)
		    .GetLastError()
		    .detail("ValueSize", value.size())
		    .detail("Value", value);
		throw;
	}
}

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

															#line 405 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via readCommitted()
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
template <class ReadCommittedActor>
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class ReadCommittedActorState {
															#line 412 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	ReadCommittedActorState(Database const& cx,PromiseStream<RangeResultWithVersion> const& results,Reference<FlowLock> const& lock,KeyRangeRef const& range,Terminator const& terminator,AccessSystemKeys const& systemAccess,LockAware const& lockAware) 
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		 : cx(cx),
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   results(results),
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   lock(lock),
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   range(range),
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   terminator(terminator),
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   systemAccess(systemAccess),
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   lockAware(lockAware),
															#line 410 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   begin(firstGreaterOrEqual(range.begin)),
															#line 411 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   end(firstGreaterOrEqual(range.end)),
															#line 412 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   tr(cx),
															#line 413 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   releaser()
															#line 439 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 415 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			;
															#line 454 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 417 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			limits = GetRangeLimits(GetRangeLimits::ROW_LIMIT_UNLIMITED, (g_network->isSimulated() && !g_simulator.speedUpSimulation) ? CLIENT_KNOBS->BACKUP_SIMULATED_LIMIT_BYTES : CLIENT_KNOBS->BACKUP_GET_RANGE_LIMIT_BYTES);
															#line 422 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (systemAccess)
															#line 487 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 423 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 491 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 424 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (lockAware)
															#line 495 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 425 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 499 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 428 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			releaser.release();
															#line 429 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_0 = lock->take(TaskPriority::DefaultYield, limits.bytes + CLIENT_KNOBS->VALUE_SIZE_LIMIT + CLIENT_KNOBS->SYSTEM_KEY_SIZE_LIMIT);
															#line 429 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<ReadCommittedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 507 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReadCommittedActor*>(this)->actor_wait_state = 1;
															#line 429 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor, 0, Void >*>(static_cast<ReadCommittedActor*>(this)));
															#line 512 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 466 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (e.code() == error_code_transaction_too_old)
															#line 534 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 469 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				tr.fullReset();
															#line 538 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				loopDepth = a_body1loopBody1Catch1cont1(loopDepth);
			}
			else
			{
															#line 471 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				StrictFuture<Void> __when_expr_3 = tr.onError(e);
															#line 471 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (static_cast<ReadCommittedActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 547 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
				static_cast<ReadCommittedActor*>(this)->actor_wait_state = 4;
															#line 471 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor, 3, Void >*>(static_cast<ReadCommittedActor*>(this)));
															#line 552 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 431 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		releaser = FlowLock::Releaser( *lock, limits.bytes + CLIENT_KNOBS->VALUE_SIZE_LIMIT + CLIENT_KNOBS->SYSTEM_KEY_SIZE_LIMIT);
															#line 434 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr.getRange(begin, end, limits);
															#line 434 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<ReadCommittedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 572 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadCommittedActor*>(this)->actor_wait_state = 2;
															#line 434 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor, 1, RangeResult >*>(static_cast<ReadCommittedActor*>(this)));
															#line 577 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 431 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		releaser = FlowLock::Releaser( *lock, limits.bytes + CLIENT_KNOBS->VALUE_SIZE_LIMIT + CLIENT_KNOBS->SYSTEM_KEY_SIZE_LIMIT);
															#line 434 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr.getRange(begin, end, limits);
															#line 434 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<ReadCommittedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 590 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadCommittedActor*>(this)->actor_wait_state = 2;
															#line 434 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor, 1, RangeResult >*>(static_cast<ReadCommittedActor*>(this)));
															#line 595 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 438 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (values.size() > 1 && BUGGIFY)
															#line 667 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 439 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			RangeResult copy;
															#line 441 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			for(int i = 0;i < values.size() / 2;i++) {
															#line 442 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				copy.push_back_deep(copy.arena(), values[i]);
															#line 675 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 444 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			values = copy;
															#line 445 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			values.more = true;
															#line 447 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (deterministicRandom()->random01() < 0.5)
															#line 683 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 448 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				StrictFuture<Void> __when_expr_2 = delay(6.0);
															#line 448 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (static_cast<ReadCommittedActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 689 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_2.get(), loopDepth); };
				static_cast<ReadCommittedActor*>(this)->actor_wait_state = 3;
															#line 448 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor, 2, Void >*>(static_cast<ReadCommittedActor*>(this)));
															#line 694 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 434 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		values = __values;
															#line 713 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 451 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		releaser.remaining -= values.expectedSize();
															#line 453 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		ASSERT(releaser.remaining >= 0);
															#line 455 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		results.send(RangeResultWithVersion(values, tr.getReadVersion().get()));
															#line 457 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (values.size() > 0)
															#line 786 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 458 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			begin = firstGreaterThan(values.end()[-1].key);
															#line 790 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 460 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (!values.more && !limits.isReached())
															#line 794 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 461 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (terminator)
															#line 798 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 462 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				results.sendError(end_of_stream());
															#line 802 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 463 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<ReadCommittedActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadCommittedActorState(); static_cast<ReadCommittedActor*>(this)->destroy(); return 0; }
															#line 806 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Database cx;
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	PromiseStream<RangeResultWithVersion> results;
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Reference<FlowLock> lock;
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	KeyRangeRef range;
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Terminator terminator;
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	AccessSystemKeys systemAccess;
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	LockAware lockAware;
															#line 410 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	KeySelector begin;
															#line 411 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	KeySelector end;
															#line 412 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Transaction tr;
															#line 413 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	FlowLock::Releaser releaser;
															#line 417 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	GetRangeLimits limits;
															#line 434 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	RangeResult values;
															#line 1017 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via readCommitted()
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class ReadCommittedActor final : public Actor<Void>, public ActorCallback< ReadCommittedActor, 0, Void >, public ActorCallback< ReadCommittedActor, 1, RangeResult >, public ActorCallback< ReadCommittedActor, 2, Void >, public ActorCallback< ReadCommittedActor, 3, Void >, public FastAllocated<ReadCommittedActor>, public ReadCommittedActorState<ReadCommittedActor> {
															#line 1022 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	ReadCommittedActor(Database const& cx,PromiseStream<RangeResultWithVersion> const& results,Reference<FlowLock> const& lock,KeyRangeRef const& range,Terminator const& terminator,AccessSystemKeys const& systemAccess,LockAware const& lockAware) 
															#line 1036 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] Future<Void> readCommitted( Database const& cx, PromiseStream<RangeResultWithVersion> const& results, Reference<FlowLock> const& lock, KeyRangeRef const& range, Terminator const& terminator, AccessSystemKeys const& systemAccess, LockAware const& lockAware ) {
															#line 403 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new ReadCommittedActor(cx, results, lock, range, terminator, systemAccess, lockAware));
															#line 1067 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 476 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"

															#line 1072 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via readCommitted()
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
template <class ReadCommittedActor1>
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class ReadCommittedActor1State {
															#line 1079 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	ReadCommittedActor1State(Database const& cx,PromiseStream<RCGroup> const& results,Future<Void> const& active,Reference<FlowLock> const& lock,KeyRangeRef const& range,std::function<std::pair<uint64_t, uint32_t>(Key key)> const& groupBy,Terminator const& terminator,AccessSystemKeys const& systemAccess,LockAware const& lockAware) 
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		 : cx(cx),
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   results(results),
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   active(active),
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   lock(lock),
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   range(range),
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   groupBy(groupBy),
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   terminator(terminator),
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   systemAccess(systemAccess),
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   lockAware(lockAware),
															#line 486 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   nextKey(firstGreaterOrEqual(range.begin)),
															#line 487 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   end(firstGreaterOrEqual(range.end)),
															#line 489 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   rcGroup(RCGroup()),
															#line 490 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   skipGroup(ULLONG_MAX),
															#line 491 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   tr(cx),
															#line 492 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   releaser()
															#line 1114 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 494 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			;
															#line 1129 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 496 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			limits = GetRangeLimits(GetRangeLimits::ROW_LIMIT_UNLIMITED, (g_network->isSimulated() && !g_simulator.speedUpSimulation) ? CLIENT_KNOBS->BACKUP_SIMULATED_LIMIT_BYTES : CLIENT_KNOBS->BACKUP_GET_RANGE_LIMIT_BYTES);
															#line 501 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (systemAccess)
															#line 1162 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 502 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1166 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 503 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (lockAware)
															#line 1170 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 504 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1174 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 506 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr.getRange(nextKey, end, limits);
															#line 506 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1180 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 1;
															#line 506 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor1, 0, RangeResult >*>(static_cast<ReadCommittedActor1*>(this)));
															#line 1185 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 576 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (e.code() == error_code_transaction_too_old)
															#line 1207 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 579 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				tr.fullReset();
															#line 1211 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				loopDepth = a_body1loopBody1Catch1cont1(loopDepth);
			}
			else
			{
															#line 581 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				StrictFuture<Void> __when_expr_4 = tr.onError(e);
															#line 581 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1220 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_4.get(), loopDepth); };
				static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 5;
															#line 581 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor1, 4, Void >*>(static_cast<ReadCommittedActor1*>(this)));
															#line 1225 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 510 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (rangevalue.size() > 1 && BUGGIFY)
															#line 1241 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 511 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			RangeResult copy;
															#line 513 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			for(int i = 0;i < rangevalue.size() / 2;i++) {
															#line 514 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				copy.push_back_deep(copy.arena(), rangevalue[i]);
															#line 1249 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 516 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			rangevalue = copy;
															#line 517 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			rangevalue.more = true;
															#line 519 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (deterministicRandom()->random01() < 0.5)
															#line 1257 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 520 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				StrictFuture<Void> __when_expr_1 = delay(6.0);
															#line 520 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1263 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
				static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 2;
															#line 520 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor1, 1, Void >*>(static_cast<ReadCommittedActor1*>(this)));
															#line 1268 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 506 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		rangevalue = __rangevalue;
															#line 1287 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 524 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<Void> __when_expr_2 = active;
															#line 524 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1356 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont5when1(__when_expr_2.get(), loopDepth); };
		static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 3;
															#line 524 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor1, 2, Void >*>(static_cast<ReadCommittedActor1*>(this)));
															#line 1361 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 525 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		releaser.release();
															#line 526 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<Void> __when_expr_3 = lock->take(TaskPriority::DefaultYield, rangevalue.expectedSize() + rcGroup.items.expectedSize());
															#line 526 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1455 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont9when1(__when_expr_3.get(), loopDepth); };
		static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 4;
															#line 526 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor1, 3, Void >*>(static_cast<ReadCommittedActor1*>(this)));
															#line 1460 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont9(Void && _,int loopDepth) 
	{
															#line 525 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		releaser.release();
															#line 526 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<Void> __when_expr_3 = lock->take(TaskPriority::DefaultYield, rangevalue.expectedSize() + rcGroup.items.expectedSize());
															#line 526 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<ReadCommittedActor1*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1473 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont9when1(__when_expr_3.get(), loopDepth); };
		static_cast<ReadCommittedActor1*>(this)->actor_wait_state = 4;
															#line 526 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ReadCommittedActor1, 3, Void >*>(static_cast<ReadCommittedActor1*>(this)));
															#line 1478 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 527 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		releaser = FlowLock::Releaser(*lock, rangevalue.expectedSize() + rcGroup.items.expectedSize());
															#line 529 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		for( auto& s : rangevalue ) {
															#line 530 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			uint64_t groupKey = groupBy(s.key).first;
															#line 532 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (groupKey != skipGroup)
															#line 1556 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 533 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (rcGroup.version == -1)
															#line 1560 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 534 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					rcGroup.version = tr.getReadVersion().get();
															#line 535 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					rcGroup.groupKey = groupKey;
															#line 1566 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				}
				else
				{
															#line 536 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					if (rcGroup.groupKey != groupKey)
															#line 1572 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
					{
															#line 543 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						releaser.remaining -= rcGroup.items .expectedSize();
															#line 546 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						ASSERT(releaser.remaining >= 0);
															#line 547 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						results.send(rcGroup);
															#line 548 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						nextKey = firstGreaterThan(rcGroup.items.end()[-1].key);
															#line 549 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						skipGroup = rcGroup.groupKey;
															#line 551 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						rcGroup = RCGroup();
															#line 552 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						rcGroup.version = tr.getReadVersion().get();
															#line 553 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						rcGroup.groupKey = groupKey;
															#line 1590 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
					}
				}
															#line 555 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				rcGroup.items.push_back_deep(rcGroup.items.arena(), s);
															#line 1595 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
		}
															#line 559 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (!rangevalue.more)
															#line 1600 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 560 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (rcGroup.version != -1)
															#line 1604 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 561 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				releaser.remaining -= rcGroup.items .expectedSize();
															#line 564 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				ASSERT(releaser.remaining >= 0);
															#line 566 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				results.send(rcGroup);
															#line 1612 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 569 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (terminator)
															#line 1616 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 570 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				results.sendError(end_of_stream());
															#line 1620 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 571 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<ReadCommittedActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadCommittedActor1State(); static_cast<ReadCommittedActor1*>(this)->destroy(); return 0; }
															#line 1624 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			new (&static_cast<ReadCommittedActor1*>(this)->SAV< Void >::value()) Void(Void());
			this->~ReadCommittedActor1State();
			static_cast<ReadCommittedActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 574 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		nextKey = firstGreaterThan(rangevalue.end()[-1].key);
															#line 1632 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont10cont10(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont10(Void && _,int loopDepth) 
	{
															#line 527 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		releaser = FlowLock::Releaser(*lock, rangevalue.expectedSize() + rcGroup.items.expectedSize());
															#line 529 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		for( auto& s : rangevalue ) {
															#line 530 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			uint64_t groupKey = groupBy(s.key).first;
															#line 532 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (groupKey != skipGroup)
															#line 1647 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 533 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (rcGroup.version == -1)
															#line 1651 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 534 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					rcGroup.version = tr.getReadVersion().get();
															#line 535 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					rcGroup.groupKey = groupKey;
															#line 1657 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				}
				else
				{
															#line 536 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					if (rcGroup.groupKey != groupKey)
															#line 1663 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
					{
															#line 543 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						releaser.remaining -= rcGroup.items .expectedSize();
															#line 546 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						ASSERT(releaser.remaining >= 0);
															#line 547 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						results.send(rcGroup);
															#line 548 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						nextKey = firstGreaterThan(rcGroup.items.end()[-1].key);
															#line 549 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						skipGroup = rcGroup.groupKey;
															#line 551 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						rcGroup = RCGroup();
															#line 552 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						rcGroup.version = tr.getReadVersion().get();
															#line 553 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						rcGroup.groupKey = groupKey;
															#line 1681 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
					}
				}
															#line 555 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				rcGroup.items.push_back_deep(rcGroup.items.arena(), s);
															#line 1686 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
		}
															#line 559 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (!rangevalue.more)
															#line 1691 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 560 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (rcGroup.version != -1)
															#line 1695 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 561 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				releaser.remaining -= rcGroup.items .expectedSize();
															#line 564 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				ASSERT(releaser.remaining >= 0);
															#line 566 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				results.send(rcGroup);
															#line 1703 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 569 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (terminator)
															#line 1707 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 570 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				results.sendError(end_of_stream());
															#line 1711 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 571 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<ReadCommittedActor1*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadCommittedActor1State(); static_cast<ReadCommittedActor1*>(this)->destroy(); return 0; }
															#line 1715 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			new (&static_cast<ReadCommittedActor1*>(this)->SAV< Void >::value()) Void(Void());
			this->~ReadCommittedActor1State();
			static_cast<ReadCommittedActor1*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 574 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		nextKey = firstGreaterThan(rangevalue.end()[-1].key);
															#line 1723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Database cx;
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	PromiseStream<RCGroup> results;
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Future<Void> active;
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Reference<FlowLock> lock;
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	KeyRangeRef range;
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	std::function<std::pair<uint64_t, uint32_t>(Key key)> groupBy;
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Terminator terminator;
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	AccessSystemKeys systemAccess;
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	LockAware lockAware;
															#line 486 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	KeySelector nextKey;
															#line 487 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	KeySelector end;
															#line 489 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	RCGroup rcGroup;
															#line 490 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	uint64_t skipGroup;
															#line 491 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Transaction tr;
															#line 492 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	FlowLock::Releaser releaser;
															#line 496 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	GetRangeLimits limits;
															#line 506 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	RangeResult rangevalue;
															#line 1919 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via readCommitted()
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class ReadCommittedActor1 final : public Actor<Void>, public ActorCallback< ReadCommittedActor1, 0, RangeResult >, public ActorCallback< ReadCommittedActor1, 1, Void >, public ActorCallback< ReadCommittedActor1, 2, Void >, public ActorCallback< ReadCommittedActor1, 3, Void >, public ActorCallback< ReadCommittedActor1, 4, Void >, public FastAllocated<ReadCommittedActor1>, public ReadCommittedActor1State<ReadCommittedActor1> {
															#line 1924 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	ReadCommittedActor1(Database const& cx,PromiseStream<RCGroup> const& results,Future<Void> const& active,Reference<FlowLock> const& lock,KeyRangeRef const& range,std::function<std::pair<uint64_t, uint32_t>(Key key)> const& groupBy,Terminator const& terminator,AccessSystemKeys const& systemAccess,LockAware const& lockAware) 
															#line 1939 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] Future<Void> readCommitted( Database const& cx, PromiseStream<RCGroup> const& results, Future<Void> const& active, Reference<FlowLock> const& lock, KeyRangeRef const& range, std::function<std::pair<uint64_t, uint32_t>(Key key)> const& groupBy, Terminator const& terminator, AccessSystemKeys const& systemAccess, LockAware const& lockAware ) {
															#line 477 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new ReadCommittedActor1(cx, results, active, lock, range, groupBy, terminator, systemAccess, lockAware));
															#line 1971 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 586 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"

Future<Void> readCommitted(Database cx,
                           PromiseStream<RCGroup> results,
                           Reference<FlowLock> lock,
                           KeyRangeRef range,
                           std::function<std::pair<uint64_t, uint32_t>(Key key)> groupBy) {
	return readCommitted(
	    cx, results, Void(), lock, range, groupBy, Terminator::True, AccessSystemKeys::True, LockAware::True);
}

															#line 1985 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via dumpData()
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
template <class DumpDataActor>
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class DumpDataActorState {
															#line 1992 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	DumpDataActorState(Database const& cx,PromiseStream<RCGroup> const& results,Reference<FlowLock> const& lock,Key const& uid,Key const& addPrefix,Key const& removePrefix,RequestStream<CommitTransactionRequest> const& commit,NotifiedVersion* const& committedVersion,Optional<Version> const& endVersion,Key const& rangeBegin,PromiseStream<Future<Void>> const& addActor,FlowLock* const& commitLock,Reference<KeyRangeMap<Version>> const& keyVersion) 
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		 : cx(cx),
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   results(results),
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   lock(lock),
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   uid(uid),
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   addPrefix(addPrefix),
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   removePrefix(removePrefix),
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   commit(commit),
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   committedVersion(committedVersion),
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   endVersion(endVersion),
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   rangeBegin(rangeBegin),
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   addActor(addActor),
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   commitLock(commitLock),
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   keyVersion(keyVersion),
															#line 609 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   lastVersion(invalidVersion),
															#line 610 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   endOfStream(false),
															#line 611 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   totalBytes(0)
															#line 2029 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
	{
		fdb_probe_actor_create("dumpData", reinterpret_cast<unsigned long>(this));

	}
	~DumpDataActorState() 
	{
		fdb_probe_actor_destroy("dumpData", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 612 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			;
															#line 2044 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
		this->~DumpDataActorState();
		static_cast<DumpDataActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 613 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		req = CommitTransactionRequest();
															#line 614 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		newBeginVersion = invalidVersion;
															#line 615 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		mutationSize = 0;
															#line 616 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		;
															#line 2080 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 651 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		Key applyBegin = uid.withPrefix(applyMutationsBeginRange.begin);
															#line 652 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		Key versionKey = BinaryWriter::toValue(newBeginVersion, Unversioned());
															#line 653 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		Key rangeEnd = getApplyKey(newBeginVersion, uid);
															#line 655 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		req.transaction.mutations.push_back_deep(req.arena, MutationRef(MutationRef::SetValue, applyBegin, versionKey));
															#line 656 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		req.transaction.write_conflict_ranges.push_back_deep(req.arena, singleKeyRange(applyBegin));
															#line 657 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		req.transaction.mutations.push_back_deep(req.arena, MutationRef(MutationRef::ClearRange, rangeBegin, rangeEnd));
															#line 658 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		req.transaction.write_conflict_ranges.push_back_deep(req.arena, singleKeyRange(rangeBegin));
															#line 663 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		req.transaction.read_snapshot = committedVersion->get();
															#line 664 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		req.flags = req.flags | CommitTransactionRequest::FLAG_IS_LOCK_AWARE;
															#line 666 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		totalBytes += mutationSize;
															#line 667 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<Void> __when_expr_1 = commitLock->take(TaskPriority::DefaultYield, mutationSize);
															#line 667 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<DumpDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2111 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DumpDataActor*>(this)->actor_wait_state = 2;
															#line 667 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DumpDataActor, 1, Void >*>(static_cast<DumpDataActor*>(this)));
															#line 2116 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 618 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			FutureStream<RCGroup> __when_expr_0 = results.getFuture();
															#line 618 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<DumpDataActor*>(this)->actor_wait_state < 0) return a_body1loopBody1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2135 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1loopBody1when1(__when_expr_0.pop(), loopDepth); };
			static_cast<DumpDataActor*>(this)->actor_wait_state = 1;
															#line 618 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< DumpDataActor, 0, RCGroup >*>(static_cast<DumpDataActor*>(this)));
															#line 2140 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 638 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (e.code() == error_code_end_of_stream)
															#line 2175 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 639 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (endVersion.present() && endVersion.get() > lastVersion && endVersion.get() > newBeginVersion)
															#line 2179 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 640 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					newBeginVersion = endVersion.get();
															#line 2183 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				}
															#line 642 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (newBeginVersion == invalidVersion)
															#line 2187 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 643 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					if (!static_cast<DumpDataActor*>(this)->SAV<int>::futures) { (void)(totalBytes); this->~DumpDataActorState(); static_cast<DumpDataActor*>(this)->destroy(); return 0; }
															#line 2191 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
					new (&static_cast<DumpDataActor*>(this)->SAV< int >::value()) int(std::move(totalBytes)); // state_var_RVO
					this->~DumpDataActorState();
					static_cast<DumpDataActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 644 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				endOfStream = true;
															#line 2199 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
			}
															#line 647 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			return a_body1Catch1(e, std::max(0, loopDepth - 2));
															#line 2204 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 2));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 2));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont2(RCGroup const& group,int loopDepth) 
	{
															#line 619 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		lock->release(group.items.expectedSize());
															#line 621 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		BinaryWriter bw(Unversioned());
															#line 622 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		for(int i = 0;i < group.items.size();++i) {
															#line 623 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			bw.serializeBytes(group.items[i].value);
															#line 2224 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 625 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		decodeBackupLogValue(req.arena, req.transaction.mutations, mutationSize, bw.toValue(), addPrefix, removePrefix, group.groupKey, keyVersion);
															#line 633 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		newBeginVersion = group.groupKey + 1;
															#line 634 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (mutationSize >= CLIENT_KNOBS->BACKUP_LOG_WRITE_BATCH_MAX_SIZE)
															#line 2232 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
		loopDepth = a_body1loopBody1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont2(RCGroup && group,int loopDepth) 
	{
															#line 619 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		lock->release(group.items.expectedSize());
															#line 621 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		BinaryWriter bw(Unversioned());
															#line 622 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		for(int i = 0;i < group.items.size();++i) {
															#line 623 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			bw.serializeBytes(group.items[i].value);
															#line 2250 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 625 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		decodeBackupLogValue(req.arena, req.transaction.mutations, mutationSize, bw.toValue(), addPrefix, removePrefix, group.groupKey, keyVersion);
															#line 633 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		newBeginVersion = group.groupKey + 1;
															#line 634 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (mutationSize >= CLIENT_KNOBS->BACKUP_LOG_WRITE_BATCH_MAX_SIZE)
															#line 2258 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
		loopDepth = a_body1loopBody1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(RCGroup const& group,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont2(group, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(RCGroup && group,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont2(std::move(group), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DumpDataActor*>(this)->actor_wait_state > 0) static_cast<DumpDataActor*>(this)->actor_wait_state = 0;
		static_cast<DumpDataActor*>(this)->ActorSingleCallback< DumpDataActor, 0, RCGroup >::remove();

	}
	void a_callback_fire(ActorSingleCallback< DumpDataActor, 0, RCGroup >*,RCGroup const& value) 
	{
		fdb_probe_actor_enter("dumpData", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dumpData", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< DumpDataActor, 0, RCGroup >*,RCGroup && value) 
	{
		fdb_probe_actor_enter("dumpData", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dumpData", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< DumpDataActor, 0, RCGroup >*,Error err) 
	{
		fdb_probe_actor_enter("dumpData", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dumpData", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1loopBody1cont6(int loopDepth) 
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
															#line 668 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		addActor.send(commitLock->releaseWhen(success(commit.getReply(req)), mutationSize));
															#line 670 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (endOfStream)
															#line 2348 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 671 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<DumpDataActor*>(this)->SAV<int>::futures) { (void)(totalBytes); this->~DumpDataActorState(); static_cast<DumpDataActor*>(this)->destroy(); return 0; }
															#line 2352 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			new (&static_cast<DumpDataActor*>(this)->SAV< int >::value()) int(std::move(totalBytes)); // state_var_RVO
			this->~DumpDataActorState();
			static_cast<DumpDataActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void && _,int loopDepth) 
	{
															#line 668 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		addActor.send(commitLock->releaseWhen(success(commit.getReply(req)), mutationSize));
															#line 670 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (endOfStream)
															#line 2368 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 671 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<DumpDataActor*>(this)->SAV<int>::futures) { (void)(totalBytes); this->~DumpDataActorState(); static_cast<DumpDataActor*>(this)->destroy(); return 0; }
															#line 2372 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			new (&static_cast<DumpDataActor*>(this)->SAV< int >::value()) int(std::move(totalBytes)); // state_var_RVO
			this->~DumpDataActorState();
			static_cast<DumpDataActor*>(this)->finishSendAndDelPromiseRef();
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
	void a_exitChoose2() 
	{
		if (static_cast<DumpDataActor*>(this)->actor_wait_state > 0) static_cast<DumpDataActor*>(this)->actor_wait_state = 0;
		static_cast<DumpDataActor*>(this)->ActorCallback< DumpDataActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DumpDataActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("dumpData", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dumpData", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DumpDataActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("dumpData", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dumpData", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DumpDataActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("dumpData", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("dumpData", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Database cx;
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	PromiseStream<RCGroup> results;
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Reference<FlowLock> lock;
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Key uid;
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Key addPrefix;
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Key removePrefix;
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	RequestStream<CommitTransactionRequest> commit;
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	NotifiedVersion* committedVersion;
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Optional<Version> endVersion;
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Key rangeBegin;
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	PromiseStream<Future<Void>> addActor;
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	FlowLock* commitLock;
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Reference<KeyRangeMap<Version>> keyVersion;
															#line 609 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Version lastVersion;
															#line 610 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	bool endOfStream;
															#line 611 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	int totalBytes;
															#line 613 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	CommitTransactionRequest req;
															#line 614 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Version newBeginVersion;
															#line 615 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	int mutationSize;
															#line 2483 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via dumpData()
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class DumpDataActor final : public Actor<int>, public ActorSingleCallback< DumpDataActor, 0, RCGroup >, public ActorCallback< DumpDataActor, 1, Void >, public FastAllocated<DumpDataActor>, public DumpDataActorState<DumpDataActor> {
															#line 2488 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
public:
	using FastAllocated<DumpDataActor>::operator new;
	using FastAllocated<DumpDataActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<int>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< DumpDataActor, 0, RCGroup >;
friend struct ActorCallback< DumpDataActor, 1, Void >;
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	DumpDataActor(Database const& cx,PromiseStream<RCGroup> const& results,Reference<FlowLock> const& lock,Key const& uid,Key const& addPrefix,Key const& removePrefix,RequestStream<CommitTransactionRequest> const& commit,NotifiedVersion* const& committedVersion,Optional<Version> const& endVersion,Key const& rangeBegin,PromiseStream<Future<Void>> const& addActor,FlowLock* const& commitLock,Reference<KeyRangeMap<Version>> const& keyVersion) 
															#line 2500 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		 : Actor<int>(),
		   DumpDataActorState<DumpDataActor>(cx, results, lock, uid, addPrefix, removePrefix, commit, committedVersion, endVersion, rangeBegin, addActor, commitLock, keyVersion)
	{
		fdb_probe_actor_enter("dumpData", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("dumpData");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("dumpData", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< DumpDataActor, 0, RCGroup >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DumpDataActor, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] Future<int> dumpData( Database const& cx, PromiseStream<RCGroup> const& results, Reference<FlowLock> const& lock, Key const& uid, Key const& addPrefix, Key const& removePrefix, RequestStream<CommitTransactionRequest> const& commit, NotifiedVersion* const& committedVersion, Optional<Version> const& endVersion, Key const& rangeBegin, PromiseStream<Future<Void>> const& addActor, FlowLock* const& commitLock, Reference<KeyRangeMap<Version>> const& keyVersion ) {
															#line 596 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	return Future<int>(new DumpDataActor(cx, results, lock, uid, addPrefix, removePrefix, commit, committedVersion, endVersion, rangeBegin, addActor, commitLock, keyVersion));
															#line 2529 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 675 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"

															#line 2534 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via coalesceKeyVersionCache()
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
template <class CoalesceKeyVersionCacheActor>
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class CoalesceKeyVersionCacheActorState {
															#line 2541 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	CoalesceKeyVersionCacheActorState(Key const& uid,Version const& endVersion,Reference<KeyRangeMap<Version>> const& keyVersion,RequestStream<CommitTransactionRequest> const& commit,NotifiedVersion* const& committedVersion,PromiseStream<Future<Void>> const& addActor,FlowLock* const& commitLock) 
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		 : uid(uid),
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   endVersion(endVersion),
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   keyVersion(keyVersion),
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   commit(commit),
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   committedVersion(committedVersion),
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   addActor(addActor),
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   commitLock(commitLock)
															#line 2560 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 683 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			Version lastVersion = -1000;
															#line 684 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			int64_t removed = 0;
															#line 685 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			req = CommitTransactionRequest();
															#line 686 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			mutationSize = 0;
															#line 687 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			Key mapPrefix = uid.withPrefix(applyMutationsKeyVersionMapRange.begin);
															#line 689 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			for( auto it : keyVersion->ranges() ) {
															#line 690 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (lastVersion == -1000)
															#line 2587 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 691 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					lastVersion = it.value();
															#line 2591 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				}
				else
				{
															#line 693 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					Version ver = it.value();
															#line 694 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					if (ver < endVersion && lastVersion < endVersion && ver != invalidVersion && lastVersion != invalidVersion)
															#line 2599 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
					{
															#line 696 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						Key removeKey = it.range().begin.withPrefix(mapPrefix);
															#line 697 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						Key removeEnd = keyAfter(removeKey);
															#line 698 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						req.transaction.mutations.push_back_deep(req.arena, MutationRef(MutationRef::ClearRange, removeKey, removeEnd));
															#line 700 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						mutationSize += removeKey.size() + removeEnd.size();
															#line 701 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						removed--;
															#line 2611 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
					}
					else
					{
															#line 703 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						lastVersion = ver;
															#line 2617 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
					}
				}
			}
															#line 708 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (removed != 0)
															#line 2623 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 709 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				Key countKey = uid.withPrefix(applyMutationsKeyVersionCountRange.begin);
															#line 710 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				req.transaction.write_conflict_ranges.push_back_deep(req.arena, singleKeyRange(countKey));
															#line 711 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				req.transaction.mutations.push_back_deep( req.arena, MutationRef(MutationRef::AddValue, countKey, StringRef((uint8_t*)&removed, 8)));
															#line 713 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				req.transaction.read_snapshot = committedVersion->get();
															#line 714 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				req.flags = req.flags | CommitTransactionRequest::FLAG_IS_LOCK_AWARE;
															#line 716 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				StrictFuture<Void> __when_expr_0 = commitLock->take(TaskPriority::DefaultYield, mutationSize);
															#line 716 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (static_cast<CoalesceKeyVersionCacheActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 2639 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<CoalesceKeyVersionCacheActor*>(this)->actor_wait_state = 1;
															#line 716 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CoalesceKeyVersionCacheActor, 0, Void >*>(static_cast<CoalesceKeyVersionCacheActor*>(this)));
															#line 2644 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 720 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (!static_cast<CoalesceKeyVersionCacheActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CoalesceKeyVersionCacheActorState(); static_cast<CoalesceKeyVersionCacheActor*>(this)->destroy(); return 0; }
															#line 2672 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		new (&static_cast<CoalesceKeyVersionCacheActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CoalesceKeyVersionCacheActorState();
		static_cast<CoalesceKeyVersionCacheActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(Void const& _,int loopDepth) 
	{
															#line 717 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		addActor.send(commitLock->releaseWhen(success(commit.getReply(req)), mutationSize));
															#line 2684 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(Void && _,int loopDepth) 
	{
															#line 717 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		addActor.send(commitLock->releaseWhen(success(commit.getReply(req)), mutationSize));
															#line 2693 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Key uid;
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Version endVersion;
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Reference<KeyRangeMap<Version>> keyVersion;
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	RequestStream<CommitTransactionRequest> commit;
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	NotifiedVersion* committedVersion;
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	PromiseStream<Future<Void>> addActor;
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	FlowLock* commitLock;
															#line 685 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	CommitTransactionRequest req;
															#line 686 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	int64_t mutationSize;
															#line 2779 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via coalesceKeyVersionCache()
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class CoalesceKeyVersionCacheActor final : public Actor<Void>, public ActorCallback< CoalesceKeyVersionCacheActor, 0, Void >, public FastAllocated<CoalesceKeyVersionCacheActor>, public CoalesceKeyVersionCacheActorState<CoalesceKeyVersionCacheActor> {
															#line 2784 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
public:
	using FastAllocated<CoalesceKeyVersionCacheActor>::operator new;
	using FastAllocated<CoalesceKeyVersionCacheActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CoalesceKeyVersionCacheActor, 0, Void >;
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	CoalesceKeyVersionCacheActor(Key const& uid,Version const& endVersion,Reference<KeyRangeMap<Version>> const& keyVersion,RequestStream<CommitTransactionRequest> const& commit,NotifiedVersion* const& committedVersion,PromiseStream<Future<Void>> const& addActor,FlowLock* const& commitLock) 
															#line 2795 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] Future<Void> coalesceKeyVersionCache( Key const& uid, Version const& endVersion, Reference<KeyRangeMap<Version>> const& keyVersion, RequestStream<CommitTransactionRequest> const& commit, NotifiedVersion* const& committedVersion, PromiseStream<Future<Void>> const& addActor, FlowLock* const& commitLock ) {
															#line 676 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new CoalesceKeyVersionCacheActor(uid, endVersion, keyVersion, commit, committedVersion, addActor, commitLock));
															#line 2823 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 722 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"

															#line 2828 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via applyMutations()
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
template <class ApplyMutationsActor>
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class ApplyMutationsActorState {
															#line 2835 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	ApplyMutationsActorState(Database const& cx,Key const& uid,Key const& addPrefix,Key const& removePrefix,Version const& beginVersion,Version* const& endVersion,RequestStream<CommitTransactionRequest> const& commit,NotifiedVersion* const& committedVersion,Reference<KeyRangeMap<Version>> const& keyVersion) 
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		 : cx(cx),
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   uid(uid),
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   addPrefix(addPrefix),
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   removePrefix(removePrefix),
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   beginVersion(beginVersion),
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   endVersion(endVersion),
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   commit(commit),
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   committedVersion(committedVersion),
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   keyVersion(keyVersion),
															#line 732 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   commitLock(CLIENT_KNOBS->BACKUP_LOCK_BYTES),
															#line 733 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   addActor(),
															#line 734 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   error(actorCollection(addActor.getFuture())),
															#line 735 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   maxBytes(CLIENT_KNOBS->APPLY_MIN_LOCK_BYTES)
															#line 2866 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 737 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			keyVersion->insert(metadataVersionKey, 0);
															#line 2881 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			try {
															#line 740 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				;
															#line 2885 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 794 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			TraceEvent(e.code() == error_code_restore_missing_data ? SevWarnAlways : SevError, "ApplyMutationsError") .error(e);
															#line 796 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 2917 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 741 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (beginVersion >= *endVersion)
															#line 2938 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 742 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_0 = commitLock.take(TaskPriority::DefaultYield, CLIENT_KNOBS->BACKUP_LOCK_BYTES);
															#line 742 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<ApplyMutationsActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2944 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ApplyMutationsActor*>(this)->actor_wait_state = 1;
															#line 742 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ApplyMutationsActor, 0, Void >*>(static_cast<ApplyMutationsActor*>(this)));
															#line 2949 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 749 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		int rangeCount = std::max(1, CLIENT_KNOBS->APPLY_MAX_LOCK_BYTES / maxBytes);
															#line 750 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		newEndVersion = std::min(*endVersion, ((beginVersion / CLIENT_KNOBS->APPLY_BLOCK_SIZE) + rangeCount) * CLIENT_KNOBS->APPLY_BLOCK_SIZE);
															#line 753 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		ranges = getApplyRanges(beginVersion, newEndVersion, uid);
															#line 754 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		idx = size_t();
															#line 755 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		results = std::vector<PromiseStream<RCGroup>>();
															#line 756 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		rc = std::vector<Future<Void>>();
															#line 757 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		locks = std::vector<Reference<FlowLock>>();
															#line 759 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		for(int i = 0;i < ranges.size();++i) {
															#line 760 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			results.push_back(PromiseStream<RCGroup>());
															#line 761 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			locks.push_back(makeReference<FlowLock>( std::max(CLIENT_KNOBS->APPLY_MAX_LOCK_BYTES / ranges.size(), CLIENT_KNOBS->APPLY_MIN_LOCK_BYTES)));
															#line 763 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			rc.push_back(readCommitted(cx, results[i], locks[i], ranges[i], decodeBKMutationLogKey));
															#line 2983 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 766 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		maxBytes = std::max<int>(maxBytes * CLIENT_KNOBS->APPLY_MAX_DECAY_RATE, CLIENT_KNOBS->APPLY_MIN_LOCK_BYTES);
															#line 767 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		idx = 0;
															#line 2989 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2(Void const& _,int loopDepth) 
	{
															#line 743 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		commitLock.release(CLIENT_KNOBS->BACKUP_LOCK_BYTES);
															#line 744 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (beginVersion >= *endVersion)
															#line 3000 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 745 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<ApplyMutationsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ApplyMutationsActorState(); static_cast<ApplyMutationsActor*>(this)->destroy(); return 0; }
															#line 3004 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 743 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		commitLock.release(CLIENT_KNOBS->BACKUP_LOCK_BYTES);
															#line 744 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (beginVersion >= *endVersion)
															#line 3020 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 745 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<ApplyMutationsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ApplyMutationsActorState(); static_cast<ApplyMutationsActor*>(this)->destroy(); return 0; }
															#line 3024 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 786 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<Void> __when_expr_2 = coalesceKeyVersionCache( uid, newEndVersion, keyVersion, commit, committedVersion, addActor, &commitLock);
															#line 786 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<ApplyMutationsActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3103 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch2(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont5when1(__when_expr_2.get(), loopDepth); };
		static_cast<ApplyMutationsActor*>(this)->actor_wait_state = 3;
															#line 786 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ApplyMutationsActor, 2, Void >*>(static_cast<ApplyMutationsActor*>(this)));
															#line 3108 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 767 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (!(idx < ranges.size()))
															#line 3124 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
			return a_body1loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 768 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<int> __when_expr_1 = dumpData(cx, results[idx], locks[idx], uid, addPrefix, removePrefix, commit, committedVersion, idx == ranges.size() - 1 ? newEndVersion : Optional<Version>(), ranges[idx].begin, addActor, &commitLock, keyVersion);
															#line 768 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<ApplyMutationsActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 3132 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch2(__when_expr_1.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ApplyMutationsActor*>(this)->actor_wait_state = 2;
															#line 768 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ApplyMutationsActor, 1, int >*>(static_cast<ApplyMutationsActor*>(this)));
															#line 3137 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 781 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		maxBytes = std::max<int>(CLIENT_KNOBS->APPLY_MAX_INCREASE_FACTOR * bytes, maxBytes);
															#line 782 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (error.isError())
															#line 3161 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 783 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			return a_body1Catch2(error.getError(), std::max(0, loopDepth - 2));
															#line 3165 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 767 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		++idx;
															#line 3169 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont1(int && bytes,int loopDepth) 
	{
															#line 781 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		maxBytes = std::max<int>(CLIENT_KNOBS->APPLY_MAX_INCREASE_FACTOR * bytes, maxBytes);
															#line 782 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (error.isError())
															#line 3180 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 783 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			return a_body1Catch2(error.getError(), std::max(0, loopDepth - 2));
															#line 3184 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 767 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		++idx;
															#line 3188 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 788 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		beginVersion = newEndVersion;
															#line 789 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (BUGGIFY)
															#line 3262 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 790 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_3 = delay(2.0);
															#line 790 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<ApplyMutationsActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3268 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont7when1(__when_expr_3.get(), loopDepth); };
			static_cast<ApplyMutationsActor*>(this)->actor_wait_state = 4;
															#line 790 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ApplyMutationsActor, 3, Void >*>(static_cast<ApplyMutationsActor*>(this)));
															#line 3273 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 788 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		beginVersion = newEndVersion;
															#line 789 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (BUGGIFY)
															#line 3289 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 790 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_3 = delay(2.0);
															#line 790 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<ApplyMutationsActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3295 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch2(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont7when1(__when_expr_3.get(), loopDepth); };
			static_cast<ApplyMutationsActor*>(this)->actor_wait_state = 4;
															#line 790 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ApplyMutationsActor, 3, Void >*>(static_cast<ApplyMutationsActor*>(this)));
															#line 3300 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Database cx;
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Key uid;
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Key addPrefix;
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Key removePrefix;
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Version beginVersion;
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Version* endVersion;
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	RequestStream<CommitTransactionRequest> commit;
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	NotifiedVersion* committedVersion;
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Reference<KeyRangeMap<Version>> keyVersion;
															#line 732 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	FlowLock commitLock;
															#line 733 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	PromiseStream<Future<Void>> addActor;
															#line 734 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Future<Void> error;
															#line 735 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	int maxBytes;
															#line 750 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Version newEndVersion;
															#line 753 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Standalone<VectorRef<KeyRangeRef>> ranges;
															#line 754 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	size_t idx;
															#line 755 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	std::vector<PromiseStream<RCGroup>> results;
															#line 756 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	std::vector<Future<Void>> rc;
															#line 757 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	std::vector<Reference<FlowLock>> locks;
															#line 3492 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via applyMutations()
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class ApplyMutationsActor final : public Actor<Void>, public ActorCallback< ApplyMutationsActor, 0, Void >, public ActorCallback< ApplyMutationsActor, 1, int >, public ActorCallback< ApplyMutationsActor, 2, Void >, public ActorCallback< ApplyMutationsActor, 3, Void >, public FastAllocated<ApplyMutationsActor>, public ApplyMutationsActorState<ApplyMutationsActor> {
															#line 3497 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	ApplyMutationsActor(Database const& cx,Key const& uid,Key const& addPrefix,Key const& removePrefix,Version const& beginVersion,Version* const& endVersion,RequestStream<CommitTransactionRequest> const& commit,NotifiedVersion* const& committedVersion,Reference<KeyRangeMap<Version>> const& keyVersion) 
															#line 3511 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		 : Actor<Void>(),
		   ApplyMutationsActorState<ApplyMutationsActor>(cx, uid, addPrefix, removePrefix, beginVersion, endVersion, commit, committedVersion, keyVersion)
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
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] Future<Void> applyMutations( Database const& cx, Key const& uid, Key const& addPrefix, Key const& removePrefix, Version const& beginVersion, Version* const& endVersion, RequestStream<CommitTransactionRequest> const& commit, NotifiedVersion* const& committedVersion, Reference<KeyRangeMap<Version>> const& keyVersion ) {
															#line 723 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new ApplyMutationsActor(cx, uid, addPrefix, removePrefix, beginVersion, endVersion, commit, committedVersion, keyVersion));
															#line 3542 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 799 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"

															#line 3547 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via _eraseLogData()
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
template <class _eraseLogDataActor>
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class _eraseLogDataActorState {
															#line 3554 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	_eraseLogDataActorState(Reference<ReadYourWritesTransaction> const& tr,Key const& logUidValue,Key const& destUidValue,Optional<Version> const& endVersion,CheckBackupUID const& checkBackupUid,Version const& backupUid) 
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		 : tr(tr),
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   logUidValue(logUidValue),
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   destUidValue(destUidValue),
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   endVersion(endVersion),
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   checkBackupUid(checkBackupUid),
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   backupUid(backupUid),
															#line 806 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   backupLatestVersionsPath(destUidValue.withPrefix(backupLatestVersionsPrefix)),
															#line 807 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   backupLatestVersionsKey(logUidValue.withPrefix(backupLatestVersionsPath))
															#line 3575 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 809 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (!destUidValue.size())
															#line 3590 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 810 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (!static_cast<_eraseLogDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_eraseLogDataActorState(); static_cast<_eraseLogDataActor*>(this)->destroy(); return 0; }
															#line 3594 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				new (&static_cast<_eraseLogDataActor*>(this)->SAV< Void >::value()) Void(Void());
				this->~_eraseLogDataActorState();
				static_cast<_eraseLogDataActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 813 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 814 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 816 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (checkBackupUid)
															#line 3606 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 817 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				Subspace sourceStates = Subspace(databaseBackupPrefixRange.begin).get(BackupAgentBase::keySourceStates).get(logUidValue);
															#line 819 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				StrictFuture<Optional<Value>> __when_expr_0 = tr->get(sourceStates.pack(DatabaseBackupAgent::keyFolderId));
															#line 819 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (static_cast<_eraseLogDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3614 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<_eraseLogDataActor*>(this)->actor_wait_state = 1;
															#line 819 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< _eraseLogDataActor, 0, Optional<Value> >*>(static_cast<_eraseLogDataActor*>(this)));
															#line 3619 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 824 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<RangeResult> __when_expr_1 = tr->getRange(KeyRangeRef(backupLatestVersionsPath, strinc(backupLatestVersionsPath)), CLIENT_KNOBS->TOO_MANY);
															#line 824 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<_eraseLogDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3649 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<_eraseLogDataActor*>(this)->actor_wait_state = 2;
															#line 824 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< _eraseLogDataActor, 1, RangeResult >*>(static_cast<_eraseLogDataActor*>(this)));
															#line 3654 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Optional<Value> const& v,int loopDepth) 
	{
															#line 820 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (v.present() && BinaryReader::fromStringRef<Version>(v.get(), Unversioned()) > backupUid)
															#line 3663 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 821 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<_eraseLogDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_eraseLogDataActorState(); static_cast<_eraseLogDataActor*>(this)->destroy(); return 0; }
															#line 3667 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 820 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (v.present() && BinaryReader::fromStringRef<Version>(v.get(), Unversioned()) > backupUid)
															#line 3681 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 821 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<_eraseLogDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_eraseLogDataActorState(); static_cast<_eraseLogDataActor*>(this)->destroy(); return 0; }
															#line 3685 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 828 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		currBeginVersion = invalidVersion;
															#line 829 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		for( auto backupVersion : backupVersions ) {
															#line 830 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			Key currLogUidValue = backupVersion.key.removePrefix(backupLatestVersionsPrefix).removePrefix(destUidValue);
															#line 832 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (currLogUidValue == logUidValue)
															#line 3768 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 833 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				currBeginVersion = BinaryReader::fromStringRef<Version>(backupVersion.value, Unversioned());
															#line 3772 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				break;
			}
		}
															#line 839 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (currBeginVersion == invalidVersion)
															#line 3778 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 840 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (!static_cast<_eraseLogDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_eraseLogDataActorState(); static_cast<_eraseLogDataActor*>(this)->destroy(); return 0; }
															#line 3782 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			new (&static_cast<_eraseLogDataActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~_eraseLogDataActorState();
			static_cast<_eraseLogDataActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 843 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		currEndVersion = std::numeric_limits<Version>::max();
															#line 844 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (endVersion.present())
															#line 3792 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 845 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			currEndVersion = std::min(currEndVersion, endVersion.get());
															#line 3796 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 848 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		nextSmallestVersion = currEndVersion;
															#line 849 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		bool clearLogRangesRequired = true;
															#line 852 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (backupVersions.size() > 1)
															#line 3804 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 853 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			for( auto backupVersion : backupVersions ) {
															#line 854 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				Key currLogUidValue = backupVersion.key.removePrefix(backupLatestVersionsPrefix).removePrefix(destUidValue);
															#line 855 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				Version currVersion = BinaryReader::fromStringRef<Version>(backupVersion.value, Unversioned());
															#line 857 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (currLogUidValue == logUidValue)
															#line 3814 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				{
					continue;
				}
				else
				{
															#line 859 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					if (currVersion > currBeginVersion)
															#line 3822 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
					{
															#line 860 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						nextSmallestVersion = std::min(currVersion, nextSmallestVersion);
															#line 3826 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
					}
					else
					{
															#line 863 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						clearLogRangesRequired = false;
															#line 3832 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
						break;
					}
				}
			}
		}
															#line 869 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (endVersion.present() || backupVersions.size() != 1 || BUGGIFY)
															#line 3840 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 870 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (!endVersion.present())
															#line 3844 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 872 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				tr->clear(backupLatestVersionsKey);
															#line 873 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (backupVersions.size() == 1)
															#line 3850 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 874 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					tr->clear(prefixRange(destUidValue.withPrefix(logRangesRange.begin)));
															#line 3854 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				}
			}
			else
			{
															#line 878 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				tr->set(backupLatestVersionsKey, BinaryWriter::toValue<Version>(currEndVersion, Unversioned()));
															#line 3861 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
															#line 882 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (clearLogRangesRequired)
															#line 3865 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 883 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if ((nextSmallestVersion - currBeginVersion) / CLIENT_KNOBS->LOG_RANGE_BLOCK_SIZE >= std::numeric_limits<uint8_t>::max() || BUGGIFY)
															#line 3869 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 886 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					Key baLogRangePrefix = destUidValue.withPrefix(backupLogKeys.begin);
															#line 888 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					for(int h = 0;h <= std::numeric_limits<uint8_t>::max();h++) {
															#line 889 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						uint64_t bv = bigEndian64(Version(0));
															#line 890 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						uint64_t ev = bigEndian64(nextSmallestVersion);
															#line 891 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						uint8_t h1 = h;
															#line 892 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						Key vblockPrefix = StringRef(&h1, sizeof(uint8_t)).withPrefix(baLogRangePrefix);
															#line 893 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						KeyRange range = KeyRangeRef(StringRef((uint8_t*)&bv, sizeof(uint64_t)).withPrefix(vblockPrefix), StringRef((uint8_t*)&ev, sizeof(uint64_t)).withPrefix(vblockPrefix));
															#line 895 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						tr->clear(range);
															#line 896 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						if (CLIENT_KNOBS->BACKUP_AGENT_VERBOSE_LOGGING)
															#line 3889 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
						{
															#line 897 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
							TraceEvent("EraseLogDataClearLogRanges") .detail("Range", range) .detail("Begin", 0) .detail("End", nextSmallestVersion) .detail("HexRangeBegin", range.begin.toHex()) .detail("HexRangeEnd", range.end.toHex());
															#line 3893 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
						}
					}
				}
				else
				{
															#line 906 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					Standalone<VectorRef<KeyRangeRef>> ranges = getLogRanges(currBeginVersion, nextSmallestVersion, destUidValue);
															#line 908 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					for( auto& range : ranges ) {
															#line 909 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						tr->clear(range);
															#line 910 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
						if (CLIENT_KNOBS->BACKUP_AGENT_VERBOSE_LOGGING)
															#line 3907 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
						{
															#line 911 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
							TraceEvent("EraseLogDataClearLogRanges") .detail("Range", range) .detail("Begin", currBeginVersion) .detail("End", nextSmallestVersion) .detail("HexRangeBegin", range.begin.toHex()) .detail("HexRangeEnd", range.end.toHex());
															#line 3911 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
						}
					}
				}
			}
		}
		else
		{
															#line 923 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			tr->clear(prefixRange(backupLatestVersionsPath));
															#line 926 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			tr->clear(prefixRange(destUidValue.withPrefix(backupLogKeys.begin)));
															#line 929 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			tr->clear(prefixRange(destUidValue.withPrefix(logRangesRange.begin)));
															#line 3925 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 932 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (!endVersion.present() && backupVersions.size() == 1)
															#line 3929 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 933 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<RangeResult> __when_expr_2 = tr->getRange(KeyRangeRef(destUidLookupPrefix, strinc(destUidLookupPrefix)), CLIENT_KNOBS->TOO_MANY);
															#line 933 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<_eraseLogDataActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 3935 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont6when1(__when_expr_2.get(), loopDepth); };
			static_cast<_eraseLogDataActor*>(this)->actor_wait_state = 3;
															#line 933 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< _eraseLogDataActor, 2, RangeResult >*>(static_cast<_eraseLogDataActor*>(this)));
															#line 3940 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 824 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		backupVersions = __backupVersions;
															#line 3954 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 942 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (!static_cast<_eraseLogDataActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~_eraseLogDataActorState(); static_cast<_eraseLogDataActor*>(this)->destroy(); return 0; }
															#line 4021 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		new (&static_cast<_eraseLogDataActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~_eraseLogDataActorState();
		static_cast<_eraseLogDataActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont30(RangeResult const& existingDestUidValues,int loopDepth) 
	{
															#line 935 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		for( auto it : existingDestUidValues ) {
															#line 936 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (it.value == destUidValue)
															#line 4035 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 937 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				tr->clear(it.key);
															#line 4039 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
		}
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont30(RangeResult && existingDestUidValues,int loopDepth) 
	{
															#line 935 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		for( auto it : existingDestUidValues ) {
															#line 936 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (it.value == destUidValue)
															#line 4052 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 937 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				tr->clear(it.key);
															#line 4056 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
		}
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(RangeResult const& existingDestUidValues,int loopDepth) 
	{
		loopDepth = a_body1cont30(existingDestUidValues, loopDepth);

		return loopDepth;
	}
	int a_body1cont6when1(RangeResult && existingDestUidValues,int loopDepth) 
	{
		loopDepth = a_body1cont30(std::move(existingDestUidValues), loopDepth);

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
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Key logUidValue;
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Key destUidValue;
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Optional<Version> endVersion;
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	CheckBackupUID checkBackupUid;
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Version backupUid;
															#line 806 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Key backupLatestVersionsPath;
															#line 807 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Key backupLatestVersionsKey;
															#line 824 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	RangeResult backupVersions;
															#line 828 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Version currBeginVersion;
															#line 843 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Version currEndVersion;
															#line 848 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Version nextSmallestVersion;
															#line 4150 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via _eraseLogData()
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class _eraseLogDataActor final : public Actor<Void>, public ActorCallback< _eraseLogDataActor, 0, Optional<Value> >, public ActorCallback< _eraseLogDataActor, 1, RangeResult >, public ActorCallback< _eraseLogDataActor, 2, RangeResult >, public FastAllocated<_eraseLogDataActor>, public _eraseLogDataActorState<_eraseLogDataActor> {
															#line 4155 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	_eraseLogDataActor(Reference<ReadYourWritesTransaction> const& tr,Key const& logUidValue,Key const& destUidValue,Optional<Version> const& endVersion,CheckBackupUID const& checkBackupUid,Version const& backupUid) 
															#line 4168 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] static Future<Void> _eraseLogData( Reference<ReadYourWritesTransaction> const& tr, Key const& logUidValue, Key const& destUidValue, Optional<Version> const& endVersion, CheckBackupUID const& checkBackupUid, Version const& backupUid ) {
															#line 800 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new _eraseLogDataActor(tr, logUidValue, destUidValue, endVersion, checkBackupUid, backupUid));
															#line 4198 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 944 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"

Future<Void> eraseLogData(Reference<ReadYourWritesTransaction> tr,
                          Key logUidValue,
                          Key destUidValue,
                          Optional<Version> endVersion,
                          CheckBackupUID checkBackupUid,
                          Version backupUid) {
	return _eraseLogData(tr, logUidValue, destUidValue, endVersion, checkBackupUid, backupUid);
}

															#line 4212 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via cleanupLogMutations()
															#line 954 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
template <class CleanupLogMutationsActor>
															#line 954 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class CleanupLogMutationsActorState {
															#line 4219 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 954 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	CleanupLogMutationsActorState(Database const& cx,Value const& destUidValue,bool const& deleteData) 
															#line 954 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
															#line 954 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		 : cx(cx),
															#line 954 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   destUidValue(destUidValue),
															#line 954 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   deleteData(deleteData),
															#line 955 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   backupLatestVersionsPath(destUidValue.withPrefix(backupLatestVersionsPrefix)),
															#line 957 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   tr(new ReadYourWritesTransaction(cx)),
															#line 958 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   removingLogUid(),
															#line 959 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   loggedLogUids()
															#line 4238 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 961 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			;
															#line 4253 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 963 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 964 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 966 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange( KeyRangeRef(backupLatestVersionsPath, strinc(backupLatestVersionsPath)), CLIENT_KNOBS->TOO_MANY);
															#line 966 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4290 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 1;
															#line 966 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CleanupLogMutationsActor, 0, RangeResult >*>(static_cast<CleanupLogMutationsActor*>(this)));
															#line 4295 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1041 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_4 = tr->onError(e);
															#line 1041 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4319 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_4.get(), loopDepth); };
			static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 5;
															#line 1041 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< CleanupLogMutationsActor, 4, Void >*>(static_cast<CleanupLogMutationsActor*>(this)));
															#line 4324 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 968 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		readVer = tr->getReadVersion().get();
															#line 970 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		minVersion = std::numeric_limits<Version>::max();
															#line 971 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		minVersionLogUid = Key();
															#line 973 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		backupIdx = 0;
															#line 974 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		;
															#line 4347 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& __backupVersions,int loopDepth) 
	{
															#line 966 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		backupVersions = __backupVersions;
															#line 4356 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1013 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (deleteData)
															#line 4423 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 1014 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (readVer - minVersion > CLIENT_KNOBS->MIN_CLEANUP_SECONDS * CLIENT_KNOBS->CORE_VERSIONSPERSECOND && (!removingLogUid.present() || minVersionLogUid == removingLogUid.get()))
															#line 4427 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 1016 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				removingLogUid = minVersionLogUid;
															#line 1017 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				StrictFuture<Void> __when_expr_2 = eraseLogData(tr, minVersionLogUid, destUidValue);
															#line 1017 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4435 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
				static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 3;
															#line 1017 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CleanupLogMutationsActor, 2, Void >*>(static_cast<CleanupLogMutationsActor*>(this)));
															#line 4440 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				loopDepth = 0;
			}
			else
			{
															#line 1021 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (removingLogUid.present() && minVersionLogUid != removingLogUid.get())
															#line 4447 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 1022 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					printf("\nWARNING: The oldest tag was possibly removed, run again without `--delete-data' to " "check.\n\n");
															#line 4451 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				}
				else
				{
															#line 1025 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					printf("\nWARNING: Did not delete data because the tag is not at least %.4f hours behind. Change " "`--min-cleanup-seconds' to adjust this threshold.\n\n", CLIENT_KNOBS->MIN_CLEANUP_SECONDS / 3600.0);
															#line 4457 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				}
				loopDepth = a_body1loopBody1cont5(loopDepth);
			}
		}
		else
		{
															#line 1029 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (readVer - minVersion > CLIENT_KNOBS->MIN_CLEANUP_SECONDS * CLIENT_KNOBS->CORE_VERSIONSPERSECOND)
															#line 4466 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 1031 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				printf("\nPassing `--delete-data' would delete the tag that is %.4f hours behind.\n\n", (readVer - minVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 4470 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
			else
			{
															#line 1034 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				printf("\nPassing `--delete-data' would not delete the tag that is %.4f hours behind. Change " "`--min-cleanup-seconds' to adjust the cleanup threshold.\n\n", (readVer - minVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 4476 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 974 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (!(backupIdx < backupVersions.size()))
															#line 4494 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
			return a_body1loopBody1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 975 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		currVersion = BinaryReader::fromStringRef<Version>(backupVersions[backupIdx].value, Unversioned());
															#line 977 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		currLogUid = backupVersions[backupIdx].key.removePrefix(backupLatestVersionsPrefix).removePrefix(destUidValue);
															#line 979 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (currVersion < minVersion)
															#line 4504 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 980 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			minVersionLogUid = currLogUid;
															#line 981 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			minVersion = currVersion;
															#line 4510 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		}
															#line 984 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (!loggedLogUids.count(currLogUid))
															#line 4514 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 985 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			foundDRKey = tr->get(Subspace(databaseBackupPrefixRange.begin) .get(BackupAgentBase::keySourceStates) .get(currLogUid) .pack(DatabaseBackupAgent::keyStateStatus));
															#line 989 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			foundBackupKey = tr->get(Subspace(currLogUid.withPrefix(LiteralStringRef("uid->config/")) .withPrefix(fileBackupPrefixRange.begin)) .pack(LiteralStringRef("stateEnum")));
															#line 993 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_1 = success(foundDRKey) && success(foundBackupKey);
															#line 993 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4524 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 2;
															#line 993 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CleanupLogMutationsActor, 1, Void >*>(static_cast<CleanupLogMutationsActor*>(this)));
															#line 4529 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 974 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		backupIdx++;
															#line 4556 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1cont2loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont4(Void const& _,int loopDepth) 
	{
															#line 995 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (foundDRKey.get().present() && foundBackupKey.get().present())
															#line 4565 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 996 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			printf("WARNING: Found a tag that looks like both a backup and a DR. This tag is %.4f hours " "behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 4569 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		}
		else
		{
															#line 999 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (foundDRKey.get().present() && !foundBackupKey.get().present())
															#line 4575 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 1000 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				printf("Found a DR that is %.4f hours behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 4579 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
			else
			{
															#line 1002 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (!foundDRKey.get().present() && foundBackupKey.get().present())
															#line 4585 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 1003 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					printf("Found a Backup that is %.4f hours behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 4589 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				}
				else
				{
															#line 1006 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					printf("WARNING: Found an unknown tag that is %.4f hours behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 4595 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				}
			}
		}
															#line 1009 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		loggedLogUids.insert(currLogUid);
															#line 4601 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2loopBody1cont4(Void && _,int loopDepth) 
	{
															#line 995 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (foundDRKey.get().present() && foundBackupKey.get().present())
															#line 4610 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 996 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			printf("WARNING: Found a tag that looks like both a backup and a DR. This tag is %.4f hours " "behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 4614 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		}
		else
		{
															#line 999 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (foundDRKey.get().present() && !foundBackupKey.get().present())
															#line 4620 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			{
															#line 1000 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				printf("Found a DR that is %.4f hours behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 4624 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			}
			else
			{
															#line 1002 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
				if (!foundDRKey.get().present() && foundBackupKey.get().present())
															#line 4630 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				{
															#line 1003 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					printf("Found a Backup that is %.4f hours behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 4634 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				}
				else
				{
															#line 1006 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
					printf("WARNING: Found an unknown tag that is %.4f hours behind.\n", (readVer - currVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 4640 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
				}
			}
		}
															#line 1009 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		loggedLogUids.insert(currLogUid);
															#line 4646 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1039 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (!static_cast<CleanupLogMutationsActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CleanupLogMutationsActorState(); static_cast<CleanupLogMutationsActor*>(this)->destroy(); return 0; }
															#line 4718 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1018 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<Void> __when_expr_3 = tr->commit();
															#line 1018 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4738 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 4;
															#line 1018 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CleanupLogMutationsActor, 3, Void >*>(static_cast<CleanupLogMutationsActor*>(this)));
															#line 4743 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 1018 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		StrictFuture<Void> __when_expr_3 = tr->commit();
															#line 1018 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4754 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont6when1(__when_expr_3.get(), loopDepth); };
		static_cast<CleanupLogMutationsActor*>(this)->actor_wait_state = 4;
															#line 1018 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CleanupLogMutationsActor, 3, Void >*>(static_cast<CleanupLogMutationsActor*>(this)));
															#line 4759 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1019 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		printf("\nSuccessfully removed the tag that was %.4f hours behind.\n\n", (readVer - minVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 4831 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void && _,int loopDepth) 
	{
															#line 1019 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		printf("\nSuccessfully removed the tag that was %.4f hours behind.\n\n", (readVer - minVersion) / (3600.0 * CLIENT_KNOBS->CORE_VERSIONSPERSECOND));
															#line 4840 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 954 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Database cx;
															#line 954 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Value destUidValue;
															#line 954 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	bool deleteData;
															#line 955 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Key backupLatestVersionsPath;
															#line 957 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 958 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Optional<Key> removingLogUid;
															#line 959 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	std::set<Key> loggedLogUids;
															#line 966 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	RangeResult backupVersions;
															#line 968 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Version readVer;
															#line 970 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Version minVersion;
															#line 971 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Key minVersionLogUid;
															#line 973 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	int backupIdx;
															#line 975 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Version currVersion;
															#line 977 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Key currLogUid;
															#line 985 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Future<Optional<Value>> foundDRKey;
															#line 989 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Future<Optional<Value>> foundBackupKey;
															#line 5015 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via cleanupLogMutations()
															#line 954 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class CleanupLogMutationsActor final : public Actor<Void>, public ActorCallback< CleanupLogMutationsActor, 0, RangeResult >, public ActorCallback< CleanupLogMutationsActor, 1, Void >, public ActorCallback< CleanupLogMutationsActor, 2, Void >, public ActorCallback< CleanupLogMutationsActor, 3, Void >, public ActorCallback< CleanupLogMutationsActor, 4, Void >, public FastAllocated<CleanupLogMutationsActor>, public CleanupLogMutationsActorState<CleanupLogMutationsActor> {
															#line 5020 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 954 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	CleanupLogMutationsActor(Database const& cx,Value const& destUidValue,bool const& deleteData) 
															#line 5035 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 954 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] Future<Void> cleanupLogMutations( Database const& cx, Value const& destUidValue, bool const& deleteData ) {
															#line 954 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new CleanupLogMutationsActor(cx, destUidValue, deleteData));
															#line 5067 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 1045 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"

															#line 5072 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
namespace {
// This generated class is to be used only via cleanupBackup()
															#line 1046 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
template <class CleanupBackupActor>
															#line 1046 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class CleanupBackupActorState {
															#line 5079 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
public:
															#line 1046 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	CleanupBackupActorState(Database const& cx,DeleteData const& deleteData) 
															#line 1046 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
															#line 1046 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		 : cx(cx),
															#line 1046 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   deleteData(deleteData),
															#line 1047 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		   tr(new ReadYourWritesTransaction(cx))
															#line 5090 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1048 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			;
															#line 5105 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1050 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 1051 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 1053 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<RangeResult> __when_expr_0 = tr->getRange(KeyRangeRef(destUidLookupPrefix, strinc(destUidLookupPrefix)), CLIENT_KNOBS->TOO_MANY);
															#line 1053 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<CleanupBackupActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5142 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CleanupBackupActor*>(this)->actor_wait_state = 1;
															#line 1053 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CleanupBackupActor, 0, RangeResult >*>(static_cast<CleanupBackupActor*>(this)));
															#line 5147 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1061 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_2 = tr->onError(e);
															#line 1061 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<CleanupBackupActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5171 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<CleanupBackupActor*>(this)->actor_wait_state = 3;
															#line 1061 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CleanupBackupActor, 2, Void >*>(static_cast<CleanupBackupActor*>(this)));
															#line 5176 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1056 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		RangeForbody1loopBody1cont2Iterator0 = std::begin(destUids);
															#line 5191 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		loopDepth = a_body1loopBody1cont2loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& __destUids,int loopDepth) 
	{
															#line 1053 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		destUids = __destUids;
															#line 5200 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1059 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (!static_cast<CleanupBackupActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CleanupBackupActorState(); static_cast<CleanupBackupActor*>(this)->destroy(); return 0; }
															#line 5267 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1056 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		if (!(RangeForbody1loopBody1cont2Iterator0 != std::end(destUids)))
															#line 5286 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
			return a_body1loopBody1cont2break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 1056 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		auto destUid = *RangeForbody1loopBody1cont2Iterator0;
															#line 5292 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
		{
															#line 1057 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			StrictFuture<Void> __when_expr_1 = cleanupLogMutations(cx, destUid.value, deleteData);
															#line 1057 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			if (static_cast<CleanupBackupActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5298 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont2loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<CleanupBackupActor*>(this)->actor_wait_state = 2;
															#line 1057 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CleanupBackupActor, 1, Void >*>(static_cast<CleanupBackupActor*>(this)));
															#line 5303 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1056 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
		++RangeForbody1loopBody1cont2Iterator0;
															#line 5326 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1046 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Database cx;
															#line 1046 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	DeleteData deleteData;
															#line 1047 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	Reference<ReadYourWritesTransaction> tr;
															#line 1053 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	RangeResult destUids;
															#line 1056 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	decltype(std::begin(std::declval<RangeResult>())) RangeForbody1loopBody1cont2Iterator0;
															#line 5491 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
};
// This generated class is to be used only via cleanupBackup()
															#line 1046 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
class CleanupBackupActor final : public Actor<Void>, public ActorCallback< CleanupBackupActor, 0, RangeResult >, public ActorCallback< CleanupBackupActor, 1, Void >, public ActorCallback< CleanupBackupActor, 2, Void >, public FastAllocated<CleanupBackupActor>, public CleanupBackupActorState<CleanupBackupActor> {
															#line 5496 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1046 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	CleanupBackupActor(Database const& cx,DeleteData const& deleteData) 
															#line 5509 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
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
															#line 1046 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
[[nodiscard]] Future<Void> cleanupBackup( Database const& cx, DeleteData const& deleteData ) {
															#line 1046 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"
	return Future<Void>(new CleanupBackupActor(cx, deleteData));
															#line 5539 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.g.cpp"
}

#line 1065 "/usr/src/libfdb_c/fdbclient/BackupAgentBase.actor.cpp"

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
