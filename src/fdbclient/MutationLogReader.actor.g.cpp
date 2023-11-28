#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
/*
 * MutationLogReader.actor.cpp
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

#include "fdbclient/MutationLogReader.actor.h"
#include "fdbrpc/simulator.h"
#include "flow/UnitTest.h"
#include "flow/flow.h"
#include "flow/actorcompiler.h" // This must be the last #include.

namespace {

Key versionToKey(Version version, Key prefix) {
	uint64_t versionBigEndian = bigEndian64(version);
	return KeyRef((uint8_t*)&versionBigEndian, sizeof(uint64_t)).withPrefix(prefix);
}

Version keyRefToVersion(KeyRef key, int prefixLen) {
	return (Version)bigEndian64(*((uint64_t*)key.substr(prefixLen).begin()));
}

} // namespace

namespace mutation_log_reader {

Standalone<RangeResultRef> RangeResultBlock::consume() {
	Version stopVersion = std::min(lastVersion,
	                               (firstVersion + CLIENT_KNOBS->LOG_RANGE_BLOCK_SIZE - 1) /
	                                   CLIENT_KNOBS->LOG_RANGE_BLOCK_SIZE * CLIENT_KNOBS->LOG_RANGE_BLOCK_SIZE) +
	                      1; // firstVersion rounded up to the nearest 1M versions, then + 1
	int startIndex = indexToRead;
	while (indexToRead < result.size() && keyRefToVersion(result[indexToRead].key, prefixLen) < stopVersion) {
		++indexToRead;
	}
	if (indexToRead < result.size()) {
		firstVersion = keyRefToVersion(result[indexToRead].key, prefixLen); // the version of result[indexToRead]
	}
	return Standalone<RangeResultRef>(
	    RangeResultRef(result.slice(startIndex, indexToRead), result.more, result.readThrough), result.arena());
}

void PipelinedReader::startReading(Database cx) {
	reader = getNext(cx);
}

Future<Void> PipelinedReader::getNext(Database cx) {
	return getNext_impl(this, cx);
}

															#line 68 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
// This generated class is to be used only via getNext_impl()
															#line 66 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
template <class PipelinedReader_GetNext_implActor>
															#line 66 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
class PipelinedReader_GetNext_implActorState {
															#line 74 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
public:
															#line 66 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	PipelinedReader_GetNext_implActorState(PipelinedReader* const& self,Database const& cx) 
															#line 66 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
															#line 66 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		 : self(self),
															#line 66 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		   cx(cx),
															#line 67 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		   tr(cx),
															#line 69 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		   limits(GetRangeLimits::ROW_LIMIT_UNLIMITED, (g_network->isSimulated() && !g_simulator.speedUpSimulation) ? CLIENT_KNOBS->BACKUP_SIMULATED_LIMIT_BYTES : CLIENT_KNOBS->BACKUP_GET_RANGE_LIMIT_BYTES),
															#line 74 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		   begin(versionToKey(self->currentBeginVersion, self->prefix)),
															#line 75 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		   end(versionToKey(self->endVersion, self->prefix))
															#line 91 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
	{
		fdb_probe_actor_create("getNext_impl", reinterpret_cast<unsigned long>(this));

	}
	~PipelinedReader_GetNext_implActorState() 
	{
		fdb_probe_actor_destroy("getNext_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 77 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			;
															#line 106 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
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
		this->~PipelinedReader_GetNext_implActorState();
		static_cast<PipelinedReader_GetNext_implActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 79 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		StrictFuture<Void> __when_expr_0 = self->readerLimit.take();
															#line 79 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		if (static_cast<PipelinedReader_GetNext_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 138 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<PipelinedReader_GetNext_implActor*>(this)->actor_wait_state = 1;
															#line 79 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< PipelinedReader_GetNext_implActor, 0, Void >*>(static_cast<PipelinedReader_GetNext_implActor*>(this)));
															#line 143 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 82 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		;
															#line 152 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 82 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		;
															#line 161 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont1loopHead1(loopDepth);

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
		if (static_cast<PipelinedReader_GetNext_implActor*>(this)->actor_wait_state > 0) static_cast<PipelinedReader_GetNext_implActor*>(this)->actor_wait_state = 0;
		static_cast<PipelinedReader_GetNext_implActor*>(this)->ActorCallback< PipelinedReader_GetNext_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PipelinedReader_GetNext_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< PipelinedReader_GetNext_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< PipelinedReader_GetNext_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

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
		try {
															#line 84 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			tr.setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 85 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			tr.setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 87 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			StrictFuture<RangeResult> __when_expr_1 = tr.getRange(KeyRangeRef(begin, end), limits);
															#line 87 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			if (static_cast<PipelinedReader_GetNext_implActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 253 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1cont1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<PipelinedReader_GetNext_implActor*>(this)->actor_wait_state = 2;
															#line 87 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< PipelinedReader_GetNext_implActor, 1, RangeResult >*>(static_cast<PipelinedReader_GetNext_implActor*>(this)));
															#line 258 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1cont1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1cont1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 110 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			if (e.code() == error_code_transaction_too_old)
															#line 293 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			{
															#line 113 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
				tr.fullReset();
															#line 297 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
				loopDepth = a_body1loopBody1cont1loopBody1Catch1cont1(loopDepth);
			}
			else
			{
															#line 115 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
				StrictFuture<Void> __when_expr_2 = tr.onError(e);
															#line 115 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
				if (static_cast<PipelinedReader_GetNext_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 306 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1cont1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
				static_cast<PipelinedReader_GetNext_implActor*>(this)->actor_wait_state = 3;
															#line 115 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
				__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< PipelinedReader_GetNext_implActor, 2, Void >*>(static_cast<PipelinedReader_GetNext_implActor*>(this)));
															#line 311 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
				loopDepth = 0;
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 2));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 2));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont2(RangeResult const& kvs,int loopDepth) 
	{
															#line 90 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		if (!kvs.empty())
															#line 327 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		{
															#line 92 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			self->reads.send( RangeResultBlock{ .result = kvs, .firstVersion = keyRefToVersion(kvs.front().key, self->prefix.size()), .lastVersion = keyRefToVersion(kvs.back().key, self->prefix.size()), .hash = self->hash, .prefixLen = self->prefix.size(), .indexToRead = 0 });
															#line 331 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		}
															#line 101 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		if (!kvs.more)
															#line 335 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		{
															#line 102 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			self->reads.sendError(end_of_stream());
															#line 103 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			if (!static_cast<PipelinedReader_GetNext_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~PipelinedReader_GetNext_implActorState(); static_cast<PipelinedReader_GetNext_implActor*>(this)->destroy(); return 0; }
															#line 341 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			new (&static_cast<PipelinedReader_GetNext_implActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~PipelinedReader_GetNext_implActorState();
			static_cast<PipelinedReader_GetNext_implActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 106 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		begin = kvs.readThrough.present() ? kvs.readThrough.get() : keyAfter(kvs.back().key);
															#line 349 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		return a_body1loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1cont2(RangeResult && kvs,int loopDepth) 
	{
															#line 90 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		if (!kvs.empty())
															#line 358 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		{
															#line 92 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			self->reads.send( RangeResultBlock{ .result = kvs, .firstVersion = keyRefToVersion(kvs.front().key, self->prefix.size()), .lastVersion = keyRefToVersion(kvs.back().key, self->prefix.size()), .hash = self->hash, .prefixLen = self->prefix.size(), .indexToRead = 0 });
															#line 362 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		}
															#line 101 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		if (!kvs.more)
															#line 366 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		{
															#line 102 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			self->reads.sendError(end_of_stream());
															#line 103 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			if (!static_cast<PipelinedReader_GetNext_implActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~PipelinedReader_GetNext_implActorState(); static_cast<PipelinedReader_GetNext_implActor*>(this)->destroy(); return 0; }
															#line 372 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			new (&static_cast<PipelinedReader_GetNext_implActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~PipelinedReader_GetNext_implActorState();
			static_cast<PipelinedReader_GetNext_implActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 106 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		begin = kvs.readThrough.present() ? kvs.readThrough.get() : keyAfter(kvs.back().key);
															#line 380 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		return a_body1loopBody1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when1(RangeResult const& kvs,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont2(kvs, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1when1(RangeResult && kvs,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont2(std::move(kvs), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<PipelinedReader_GetNext_implActor*>(this)->actor_wait_state > 0) static_cast<PipelinedReader_GetNext_implActor*>(this)->actor_wait_state = 0;
		static_cast<PipelinedReader_GetNext_implActor*>(this)->ActorCallback< PipelinedReader_GetNext_implActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< PipelinedReader_GetNext_implActor, 1, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< PipelinedReader_GetNext_implActor, 1, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< PipelinedReader_GetNext_implActor, 1, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont1loopBody1Catch1cont1(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1Catch1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1Catch1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1Catch1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1Catch1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1Catch1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1loopBody1Catch1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<PipelinedReader_GetNext_implActor*>(this)->actor_wait_state > 0) static_cast<PipelinedReader_GetNext_implActor*>(this)->actor_wait_state = 0;
		static_cast<PipelinedReader_GetNext_implActor*>(this)->ActorCallback< PipelinedReader_GetNext_implActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< PipelinedReader_GetNext_implActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< PipelinedReader_GetNext_implActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< PipelinedReader_GetNext_implActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 66 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	PipelinedReader* self;
															#line 66 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	Database cx;
															#line 67 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	Transaction tr;
															#line 69 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	GetRangeLimits limits;
															#line 74 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	Key begin;
															#line 75 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	Key end;
															#line 541 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
};
// This generated class is to be used only via getNext_impl()
															#line 66 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
class PipelinedReader_GetNext_implActor final : public Actor<Void>, public ActorCallback< PipelinedReader_GetNext_implActor, 0, Void >, public ActorCallback< PipelinedReader_GetNext_implActor, 1, RangeResult >, public ActorCallback< PipelinedReader_GetNext_implActor, 2, Void >, public FastAllocated<PipelinedReader_GetNext_implActor>, public PipelinedReader_GetNext_implActorState<PipelinedReader_GetNext_implActor> {
															#line 546 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
public:
	using FastAllocated<PipelinedReader_GetNext_implActor>::operator new;
	using FastAllocated<PipelinedReader_GetNext_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< PipelinedReader_GetNext_implActor, 0, Void >;
friend struct ActorCallback< PipelinedReader_GetNext_implActor, 1, RangeResult >;
friend struct ActorCallback< PipelinedReader_GetNext_implActor, 2, Void >;
															#line 66 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	PipelinedReader_GetNext_implActor(PipelinedReader* const& self,Database const& cx) 
															#line 559 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		 : Actor<Void>(),
		   PipelinedReader_GetNext_implActorState<PipelinedReader_GetNext_implActor>(self, cx)
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getNext_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< PipelinedReader_GetNext_implActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< PipelinedReader_GetNext_implActor, 1, RangeResult >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< PipelinedReader_GetNext_implActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 66 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
[[nodiscard]] Future<Void> PipelinedReader::getNext_impl( PipelinedReader* const& self, Database const& cx ) {
															#line 66 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	return Future<Void>(new PipelinedReader_GetNext_implActor(self, cx));
															#line 588 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
}

#line 121 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"

} // namespace mutation_log_reader

															#line 595 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
// This generated class is to be used only via initializePQ()
															#line 124 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
template <class MutationLogReader_InitializePQActor>
															#line 124 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
class MutationLogReader_InitializePQActorState {
															#line 601 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
public:
															#line 124 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	MutationLogReader_InitializePQActorState(MutationLogReader* const& self) 
															#line 124 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
															#line 124 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		 : self(self),
															#line 125 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		   h()
															#line 610 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
	{
		fdb_probe_actor_create("initializePQ", reinterpret_cast<unsigned long>(this));

	}
	~MutationLogReader_InitializePQActorState() 
	{
		fdb_probe_actor_destroy("initializePQ", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 126 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			h = 0;
															#line 625 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
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
		this->~MutationLogReader_InitializePQActorState();
		static_cast<MutationLogReader_InitializePQActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 137 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		if (!static_cast<MutationLogReader_InitializePQActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~MutationLogReader_InitializePQActorState(); static_cast<MutationLogReader_InitializePQActor*>(this)->destroy(); return 0; }
															#line 648 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		new (&static_cast<MutationLogReader_InitializePQActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~MutationLogReader_InitializePQActorState();
		static_cast<MutationLogReader_InitializePQActor*>(this)->finishSendAndDelPromiseRef();
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
															#line 126 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		if (!(h < 256))
															#line 667 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
		try {
															#line 128 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			FutureStream<mutation_log_reader::RangeResultBlock> __when_expr_0 = self->pipelinedReaders[h]->reads.getFuture();
															#line 128 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			if (static_cast<MutationLogReader_InitializePQActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 676 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.pop(), loopDepth); };
			static_cast<MutationLogReader_InitializePQActor*>(this)->actor_wait_state = 1;
															#line 128 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorSingleCallback< MutationLogReader_InitializePQActor, 0, mutation_log_reader::RangeResultBlock >*>(static_cast<MutationLogReader_InitializePQActor*>(this)));
															#line 681 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			loopDepth = 0;
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
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 126 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		++h;
															#line 709 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 131 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			if (e.code() != error_code_end_of_stream)
															#line 719 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			{
															#line 132 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 723 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			}
															#line 134 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			++self->finished;
															#line 727 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont3(mutation_log_reader::RangeResultBlock const& front,int loopDepth) 
	{
															#line 129 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		self->priorityQueue.push(front);
															#line 742 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(mutation_log_reader::RangeResultBlock && front,int loopDepth) 
	{
															#line 129 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		self->priorityQueue.push(front);
															#line 751 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(mutation_log_reader::RangeResultBlock const& front,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(front, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(mutation_log_reader::RangeResultBlock && front,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(front), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<MutationLogReader_InitializePQActor*>(this)->actor_wait_state > 0) static_cast<MutationLogReader_InitializePQActor*>(this)->actor_wait_state = 0;
		static_cast<MutationLogReader_InitializePQActor*>(this)->ActorSingleCallback< MutationLogReader_InitializePQActor, 0, mutation_log_reader::RangeResultBlock >::remove();

	}
	void a_callback_fire(ActorSingleCallback< MutationLogReader_InitializePQActor, 0, mutation_log_reader::RangeResultBlock >*,mutation_log_reader::RangeResultBlock const& value) 
	{
		fdb_probe_actor_enter("initializePQ", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("initializePQ", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorSingleCallback< MutationLogReader_InitializePQActor, 0, mutation_log_reader::RangeResultBlock >*,mutation_log_reader::RangeResultBlock && value) 
	{
		fdb_probe_actor_enter("initializePQ", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("initializePQ", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorSingleCallback< MutationLogReader_InitializePQActor, 0, mutation_log_reader::RangeResultBlock >*,Error err) 
	{
		fdb_probe_actor_enter("initializePQ", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("initializePQ", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont5(int loopDepth) 
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
															#line 124 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	MutationLogReader* self;
															#line 125 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	int h;
															#line 836 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
};
// This generated class is to be used only via initializePQ()
															#line 124 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
class MutationLogReader_InitializePQActor final : public Actor<Void>, public ActorSingleCallback< MutationLogReader_InitializePQActor, 0, mutation_log_reader::RangeResultBlock >, public FastAllocated<MutationLogReader_InitializePQActor>, public MutationLogReader_InitializePQActorState<MutationLogReader_InitializePQActor> {
															#line 841 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
public:
	using FastAllocated<MutationLogReader_InitializePQActor>::operator new;
	using FastAllocated<MutationLogReader_InitializePQActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorSingleCallback< MutationLogReader_InitializePQActor, 0, mutation_log_reader::RangeResultBlock >;
															#line 124 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	MutationLogReader_InitializePQActor(MutationLogReader* const& self) 
															#line 852 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		 : Actor<Void>(),
		   MutationLogReader_InitializePQActorState<MutationLogReader_InitializePQActor>(self)
	{
		fdb_probe_actor_enter("initializePQ", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("initializePQ");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("initializePQ", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorSingleCallback< MutationLogReader_InitializePQActor, 0, mutation_log_reader::RangeResultBlock >*)0, actor_cancelled()); break;
		}

	}
};
															#line 124 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
[[nodiscard]] Future<Void> MutationLogReader::initializePQ( MutationLogReader* const& self ) {
															#line 124 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	return Future<Void>(new MutationLogReader_InitializePQActor(self));
															#line 879 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
}

#line 139 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"

Future<Standalone<RangeResultRef>> MutationLogReader::getNext() {
	return getNext_impl(this);
}

															#line 888 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
// This generated class is to be used only via getNext_impl()
															#line 144 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
template <class MutationLogReader_GetNext_implActor>
															#line 144 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
class MutationLogReader_GetNext_implActorState {
															#line 894 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
public:
															#line 144 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	MutationLogReader_GetNext_implActorState(MutationLogReader* const& self) 
															#line 144 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
															#line 144 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		 : self(self)
															#line 901 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
	{
		fdb_probe_actor_create("getNext_impl", reinterpret_cast<unsigned long>(this));

	}
	~MutationLogReader_GetNext_implActorState() 
	{
		fdb_probe_actor_destroy("getNext_impl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 145 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			;
															#line 916 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
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
		this->~MutationLogReader_GetNext_implActorState();
		static_cast<MutationLogReader_GetNext_implActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 146 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		if (self->finished == 256)
															#line 946 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		{
															#line 147 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			i = int();
															#line 148 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			i = 0;
															#line 952 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			loopDepth = a_body1loopBody1loopHead1(loopDepth);
		}
		else
		{
			loopDepth = a_body1loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 153 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		mutation_log_reader::RangeResultBlock top = self->priorityQueue.top();
															#line 154 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		self->priorityQueue.pop();
															#line 155 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		uint8_t hash = top.hash;
															#line 156 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		ret = top.consume();
															#line 157 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		if (top.empty())
															#line 974 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		{
															#line 158 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			self->pipelinedReaders[(int)hash]->release();
															#line 978 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			try {
															#line 160 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
				FutureStream<mutation_log_reader::RangeResultBlock> __when_expr_1 = self->pipelinedReaders[(int)hash]->reads.getFuture();
															#line 160 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
				if (static_cast<MutationLogReader_GetNext_implActor*>(this)->actor_wait_state < 0) return a_body1loopBody1cont1Catch1(actor_cancelled(), loopDepth);
															#line 984 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1cont1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont1when1(__when_expr_1.pop(), loopDepth); };
				static_cast<MutationLogReader_GetNext_implActor*>(this)->actor_wait_state = 2;
															#line 160 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
				__when_expr_1.addCallbackAndClear(static_cast<ActorSingleCallback< MutationLogReader_GetNext_implActor, 1, mutation_log_reader::RangeResultBlock >*>(static_cast<MutationLogReader_GetNext_implActor*>(this)));
															#line 989 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
				loopDepth = 0;
			}
			catch (Error& error) {
				loopDepth = a_body1loopBody1cont1Catch1(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1loopBody1cont1Catch1(unknown_error(), loopDepth);
			}
		}
		else
		{
															#line 171 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			self->priorityQueue.push(top);
															#line 1002 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			loopDepth = a_body1loopBody1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 151 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		return a_body1Catch1(end_of_stream(), std::max(0, loopDepth - 1));
															#line 1012 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"

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
															#line 148 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		if (!(i < self->pipelinedReaders.size()))
															#line 1027 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		{
			return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break
		}
															#line 149 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		StrictFuture<Void> __when_expr_0 = self->pipelinedReaders[i]->done();
															#line 149 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		if (static_cast<MutationLogReader_GetNext_implActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 1035 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<MutationLogReader_GetNext_implActor*>(this)->actor_wait_state = 1;
															#line 149 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< MutationLogReader_GetNext_implActor, 0, Void >*>(static_cast<MutationLogReader_GetNext_implActor*>(this)));
															#line 1040 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 148 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		++i;
															#line 1062 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 148 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		++i;
															#line 1071 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<MutationLogReader_GetNext_implActor*>(this)->actor_wait_state > 0) static_cast<MutationLogReader_GetNext_implActor*>(this)->actor_wait_state = 0;
		static_cast<MutationLogReader_GetNext_implActor*>(this)->ActorCallback< MutationLogReader_GetNext_implActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< MutationLogReader_GetNext_implActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< MutationLogReader_GetNext_implActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< MutationLogReader_GetNext_implActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont4(int loopDepth) 
	{
															#line 173 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		if (ret.size() != 0)
															#line 1143 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		{
															#line 174 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			if (!static_cast<MutationLogReader_GetNext_implActor*>(this)->SAV<Standalone<RangeResultRef>>::futures) { (void)(ret); this->~MutationLogReader_GetNext_implActorState(); static_cast<MutationLogReader_GetNext_implActor*>(this)->destroy(); return 0; }
															#line 1147 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			new (&static_cast<MutationLogReader_GetNext_implActor*>(this)->SAV< Standalone<RangeResultRef> >::value()) Standalone<RangeResultRef>(std::move(ret)); // state_var_RVO
			this->~MutationLogReader_GetNext_implActorState();
			static_cast<MutationLogReader_GetNext_implActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont5(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 164 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			if (e.code() == error_code_end_of_stream)
															#line 1168 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			{
															#line 165 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
				++self->finished;
															#line 1172 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			}
			else
			{
															#line 167 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
				return a_body1Catch1(e, std::max(0, loopDepth - 1));
															#line 1178 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			}
			loopDepth = a_body1loopBody1cont5(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont6(mutation_log_reader::RangeResultBlock const& next,int loopDepth) 
	{
															#line 162 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		self->priorityQueue.push(next);
															#line 1194 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(mutation_log_reader::RangeResultBlock && next,int loopDepth) 
	{
															#line 162 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		self->priorityQueue.push(next);
															#line 1203 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		loopDepth = a_body1loopBody1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(mutation_log_reader::RangeResultBlock const& next,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(next, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont1when1(mutation_log_reader::RangeResultBlock && next,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(next), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<MutationLogReader_GetNext_implActor*>(this)->actor_wait_state > 0) static_cast<MutationLogReader_GetNext_implActor*>(this)->actor_wait_state = 0;
		static_cast<MutationLogReader_GetNext_implActor*>(this)->ActorSingleCallback< MutationLogReader_GetNext_implActor, 1, mutation_log_reader::RangeResultBlock >::remove();

	}
	void a_callback_fire(ActorSingleCallback< MutationLogReader_GetNext_implActor, 1, mutation_log_reader::RangeResultBlock >*,mutation_log_reader::RangeResultBlock const& value) 
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorSingleCallback< MutationLogReader_GetNext_implActor, 1, mutation_log_reader::RangeResultBlock >*,mutation_log_reader::RangeResultBlock && value) 
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorSingleCallback< MutationLogReader_GetNext_implActor, 1, mutation_log_reader::RangeResultBlock >*,Error err) 
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1cont1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont8(int loopDepth) 
	{
		try {
			loopDepth = a_body1loopBody1cont5(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
															#line 144 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	MutationLogReader* self;
															#line 147 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	int i;
															#line 156 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	Standalone<RangeResultRef> ret;
															#line 1290 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
};
// This generated class is to be used only via getNext_impl()
															#line 144 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
class MutationLogReader_GetNext_implActor final : public Actor<Standalone<RangeResultRef>>, public ActorCallback< MutationLogReader_GetNext_implActor, 0, Void >, public ActorSingleCallback< MutationLogReader_GetNext_implActor, 1, mutation_log_reader::RangeResultBlock >, public FastAllocated<MutationLogReader_GetNext_implActor>, public MutationLogReader_GetNext_implActorState<MutationLogReader_GetNext_implActor> {
															#line 1295 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
public:
	using FastAllocated<MutationLogReader_GetNext_implActor>::operator new;
	using FastAllocated<MutationLogReader_GetNext_implActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Standalone<RangeResultRef>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< MutationLogReader_GetNext_implActor, 0, Void >;
friend struct ActorSingleCallback< MutationLogReader_GetNext_implActor, 1, mutation_log_reader::RangeResultBlock >;
															#line 144 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	MutationLogReader_GetNext_implActor(MutationLogReader* const& self) 
															#line 1307 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		 : Actor<Standalone<RangeResultRef>>(),
		   MutationLogReader_GetNext_implActorState<MutationLogReader_GetNext_implActor>(self)
	{
		fdb_probe_actor_enter("getNext_impl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getNext_impl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getNext_impl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< MutationLogReader_GetNext_implActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorSingleCallback< MutationLogReader_GetNext_implActor, 1, mutation_log_reader::RangeResultBlock >*)0, actor_cancelled()); break;
		}

	}
};
															#line 144 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
[[nodiscard]] Future<Standalone<RangeResultRef>> MutationLogReader::getNext_impl( MutationLogReader* const& self ) {
															#line 144 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	return Future<Standalone<RangeResultRef>>(new MutationLogReader_GetNext_implActor(self));
															#line 1335 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
}

#line 178 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"

namespace {
// UNIT TESTS
															#line 1342 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
// This generated class is to be used only via flowTestCase181()
															#line 181 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
template <class FlowTestCase181Actor>
															#line 181 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
class FlowTestCase181ActorState {
															#line 1348 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
public:
															#line 181 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	FlowTestCase181ActorState(UnitTestParameters const& params) 
															#line 181 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
															#line 181 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
		 : params(params)
															#line 1355 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase181", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase181ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase181", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 182 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			Key prefix = LiteralStringRef("foos");
															#line 184 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			ASSERT(keyRefToVersion(versionToKey(0, prefix), prefix.size()) == 0);
															#line 185 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			ASSERT(keyRefToVersion(versionToKey(1, prefix), prefix.size()) == 1);
															#line 186 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			ASSERT(keyRefToVersion(versionToKey(-1, prefix), prefix.size()) == -1);
															#line 187 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			ASSERT(keyRefToVersion(versionToKey(std::numeric_limits<int64_t>::min(), prefix), prefix.size()) == std::numeric_limits<int64_t>::min());
															#line 189 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			ASSERT(keyRefToVersion(versionToKey(std::numeric_limits<int64_t>::max(), prefix), prefix.size()) == std::numeric_limits<int64_t>::max());
															#line 192 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
			if (!static_cast<FlowTestCase181Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase181ActorState(); static_cast<FlowTestCase181Actor*>(this)->destroy(); return 0; }
															#line 1382 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
			new (&static_cast<FlowTestCase181Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase181ActorState();
			static_cast<FlowTestCase181Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
		this->~FlowTestCase181ActorState();
		static_cast<FlowTestCase181Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 181 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	UnitTestParameters params;
															#line 1406 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase181()
															#line 181 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
class FlowTestCase181Actor final : public Actor<Void>, public FastAllocated<FlowTestCase181Actor>, public FlowTestCase181ActorState<FlowTestCase181Actor> {
															#line 1411 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase181Actor>::operator new;
	using FastAllocated<FlowTestCase181Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 181 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	FlowTestCase181Actor(UnitTestParameters const& params) 
															#line 1421 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase181ActorState<FlowTestCase181Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase181", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase181");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase181", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
															#line 181 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
static Future<Void> flowTestCase181( UnitTestParameters const& params ) {
															#line 181 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
	return Future<Void>(new FlowTestCase181Actor(params));
															#line 1447 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase181, "/fdbclient/mutationlogreader/VersionKeyRefConversion")

#line 194 "/usr/src/libfdb_c/fdbclient/MutationLogReader.actor.cpp"
} // namespace

void forceLinkMutationLogReaderTests() {}
